/*
 * Fichier : TP2_boulanger.c
 * Auteur : Bejaoui Oussama
 * UE : Outils pour la commande des systèmes parallèles
 * Tp : fournisseurs/boulangers
 */



#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include "TP2_boulanger.h"

// Variables globales
int Ing[3] = {0, 0, 0};  // Ingrédients disponibles sur la table
int IngredientPossede[NUM_BOULANGERS] = {0, 1, 2};  // Ingrédient possédé par chaque boulanger
int pains_cuits = 0;  // Compteur de pains cuits

// Variables pour synchronisation
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Ing_Dispo = PTHREAD_COND_INITIALIZER;
pthread_cond_t Fou_Dispo = PTHREAD_COND_INITIALIZER;

// Obtenir le nom de l'ingrédient
const char* nomIngredient(int ing) {
    const char* noms[] = {"FARINE", "EAU", "LEVURE"};
    return noms[ing];
}

// Fonction pour que le boulanger prenne les ingrédients dont il a besoin
void prendreIngredients(int i) {
    int ingPossede = IngredientPossede[i];
    int ing1, ing2;

    // Déterminer les ingrédients manquants pour ce boulanger
    if (ingPossede == 0) {       // Possède la farine
        ing1 = 1;  // Eau
        ing2 = 2;  // Levure
    } else if (ingPossede == 1) { // Possède l'eau
        ing1 = 0;  // Farine
        ing2 = 2;  // Levure
    } else {                      // Possède la levure
        ing1 = 0;  // Farine
        ing2 = 1;  // Eau
    }

    pthread_mutex_lock(&mut);
    
    // Vérifier si on a atteint le nombre total de pains
    if (pains_cuits >= NUM_PAINS) {
        pthread_mutex_unlock(&mut);
        return;
    }
    
    printf("Boulanger %d (possède %s) a besoin de %s et %s - ", 
           i, nomIngredient(ingPossede), nomIngredient(ing1), nomIngredient(ing2));
    
    // Vérifier si les ingrédients nécessaires sont disponibles
    if (!Ing[ing1] || !Ing[ing2]) {
        printf("en attente d'ingrédients\n");
        // Attendre que les ingrédients soient disponibles
        while (!Ing[ing1] || !Ing[ing2]) {
            pthread_cond_wait(&Ing_Dispo, &mut);
            
            // Vérifier si on a atteint le nombre total de pains ou si on doit continuer d'attendre
            if (pains_cuits >= NUM_PAINS) {
                pthread_mutex_unlock(&mut);
                return;
            }
            
            // Vérifier à nouveau après le réveil
            if (!Ing[ing1] || !Ing[ing2]) {
                printf("Boulanger %d - toujours en attente\n", i);
            }
        }
    }
    
    // Prendre les ingrédients
    Ing[ing1] = 0;
    Ing[ing2] = 0;
    printf("Boulanger %d a reçu ses ingrédients et commence à travailler\n", i);
    
    pthread_mutex_unlock(&mut);
}

// Fonction pour signaler au fournisseur que le pain est terminé
void signalerFinPain(int i) {
    pthread_mutex_lock(&mut);
    
    printf("Boulanger %d a terminé son pain et signale au fournisseur\n", i);
    
    // Réveiller le fournisseur
    pthread_cond_signal(&Fou_Dispo);
    
    pthread_mutex_unlock(&mut);
}

// Fonction pour que le fournisseur dépose des ingrédients
void depoIngredients(int ing1, int ing2) {
    pthread_mutex_lock(&mut);
    
    // Vérifier si on a atteint le nombre total de pains
    if (pains_cuits >= NUM_PAINS) {
        pthread_mutex_unlock(&mut);
        return;
    }
    
    // Attendre que la table soit vide
    printf("Fournisseur attend de pouvoir déposer des ingrédients\n");
    while (Ing[0] || Ing[1] || Ing[2]) {
        pthread_cond_wait(&Fou_Dispo, &mut);
        
        // Vérifier si tous les pains sont cuits après le réveil
        if (pains_cuits >= NUM_PAINS) {
            pthread_mutex_unlock(&mut);
            return;
        }
    }
    
    // Déposer les ingrédients
    Ing[ing1] = 1;
    Ing[ing2] = 1;
    
    // Déterminer quel boulanger peut utiliser ces ingrédients
    int boulanger_servi = -1;
    for (int i = 0; i < NUM_BOULANGERS; i++) {
        int ingPossede = IngredientPossede[i];
        if (ingPossede != ing1 && ingPossede != ing2) {
            boulanger_servi = i;
            break;
        }
    }
    
    printf("Fournisseur a déposé %s et %s (pour le boulanger %d)\n", 
           nomIngredient(ing1), nomIngredient(ing2), boulanger_servi);
    
    // Réveiller les boulangers
    pthread_cond_broadcast(&Ing_Dispo);
    
    pthread_mutex_unlock(&mut);
}
