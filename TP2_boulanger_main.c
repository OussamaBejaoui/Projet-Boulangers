/*
 * Fichier : TP2_boulanger_main.c
 * Auteur : Bejaoui Oussama
 * UE : Outils pour la commande des systèmes parallèles
 * Tp : fournisseurs/boulangers
 */
 
 
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "TP2_boulanger.h"

// Simulation de l'action de pétrir le pain (0,3s)
void petrir(int i) {
    printf("Le boulanger %d est en train de pétrir son pain\n", i);
    usleep(300000);  // 0.3 secondes
}

// Simulation de l'action de cuire le pain (0,7s)
void cuire(int i) {
    printf("Le boulanger %d est en train de cuire son pain\n", i);
    usleep(700000);  // 0.7 secondes
    
    pthread_mutex_lock(&mut);
    pains_cuits++;
    printf("Le boulanger %d a terminé son pain! (Total: %d/%d)\n", 
           i, pains_cuits, NUM_PAINS);
    pthread_mutex_unlock(&mut);
}

// Fonction exécutée par chaque thread boulanger
void* boulanger(void* arg) {
    int id = *((int*)arg);
    
    while (1) {
        pthread_mutex_lock(&mut);
        // Vérifier si on a atteint le nombre total de pains
        if (pains_cuits >= NUM_PAINS) {
            pthread_mutex_unlock(&mut);
            break;
        }
        pthread_mutex_unlock(&mut);
        
        // Cycle de travail du boulanger
        prendreIngredients(id);  // Prendre les ingrédients
        
        // Vérifier à nouveau si le nombre de pains a été atteint pendant qu'on attendait
        pthread_mutex_lock(&mut);
        if (pains_cuits >= NUM_PAINS) {
            pthread_mutex_unlock(&mut);
            break;
        }
        pthread_mutex_unlock(&mut);
        
        petrir(id);              // Pétrir (0.3s)
        cuire(id);               // Cuire (0.7s)
        
        pthread_mutex_lock(&mut);
        // Signaler au fournisseur seulement si on n'a pas encore atteint le nombre de pains
        if (pains_cuits < NUM_PAINS) {
            pthread_mutex_unlock(&mut);
            signalerFinPain(id);     // Signaler au fournisseur
        } else {
            // Réveiller le fournisseur pour qu'il puisse terminer
            pthread_cond_signal(&Fou_Dispo);
            pthread_mutex_unlock(&mut);
            break;
        }
    }
    
    printf("Boulanger %d termine son travail\n", id);
    return NULL;
}

// Fonction exécutée par le thread fournisseur
void* fournisseur(void* arg) {
    (void)arg;  // Éviter l'avertissement de compilation
    
    while (1) {
        pthread_mutex_lock(&mut);
        // Vérifier si on a atteint le nombre total de pains
        if (pains_cuits >= NUM_PAINS) {
            pthread_cond_broadcast(&Ing_Dispo);  // Réveiller les boulangers en attente
            pthread_mutex_unlock(&mut);
            break;
        }
        pthread_mutex_unlock(&mut);
        
        // Choisir aléatoirement deux ingrédients à déposer
        int choix = rand() % 3;
        
        switch (choix) {
            case 0:
                depoIngredients(1, 2);  // Eau et Levure
                break;
            case 1:
                depoIngredients(0, 2);  // Farine et Levure
                break;
            case 2:
                depoIngredients(0, 1);  // Farine et Eau
                break;
        }
    }
    
    printf("Fournisseur termine son travail\n");
    return NULL;
}

int main(void) {
    // Initialisation
    pthread_t boulangers[NUM_BOULANGERS];
    pthread_t fournisseur_thread;
    int ids[NUM_BOULANGERS] = {0, 1, 2};
    
    srand(time(NULL));
    
    printf("=== Démarrage de la simulation des boulangers ===\n");
    printf("Chaque boulanger doit faire son pain avec les 3 ingrédients\n");
    printf("Le programme s'arrête après la cuisson de %d pains\n\n", NUM_PAINS);
    
    // Création des threads
    pthread_create(&fournisseur_thread, NULL, fournisseur, NULL);
    for (int i = 0; i < NUM_BOULANGERS; i++) {
        pthread_create(&boulangers[i], NULL, boulanger, &ids[i]);
    }
    
    // Attendre la fin des threads
    pthread_join(fournisseur_thread, NULL);
    for (int i = 0; i < NUM_BOULANGERS; i++) {
        pthread_join(boulangers[i], NULL);
    }
    
    printf("\n=== Simulation terminée ===\n");
    printf("Total de pains cuits: %d\n", pains_cuits);
    
    return 0;
}
