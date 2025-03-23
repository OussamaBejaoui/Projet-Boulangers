
/*
 * Fichier : TP2_bonus_main.c
 * Auteur : Bejaoui Oussama
 * UE : Outils pour la commande des systèmes parallèles
 * Tp : fournisseurs/boulangers
 */

#include "TP2_bonus.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int main(void) {
    // Initialisation des buffers
    for (int i = 0; i < NUM_BOULANGERS; i++) {
        buffer_init(&buffers[i]);
    }
    
    pthread_t boulangers[NUM_BOULANGERS];
    pthread_t fournisseur_thread;
    int ids[NUM_BOULANGERS] = {0, 1, 2};
    
    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));
    
    printf("=== Démarrage de la simulation des boulangers (BONUS) ===\n");
    printf("Le fournisseur approvisionne en continu dans 3 buffers\n");
    printf("Chaque boulanger cherche dans son buffer les ingrédients dont il a besoin\n");
    printf("Le programme s'arrête après la cuisson de %d pains\n\n", NUM_PAINS);
    
    // Création des threads
    pthread_create(&fournisseur_thread, NULL, thread_fournisseur, NULL);
    for (int i = 0; i < NUM_BOULANGERS; i++) {
        pthread_create(&boulangers[i], NULL, thread_boulanger, &ids[i]);
    }
    
    // Attendre la fin des threads
    pthread_join(fournisseur_thread, NULL);
    for (int i = 0; i < NUM_BOULANGERS; i++) {
        pthread_join(boulangers[i], NULL);
    }
    
    // Libérer les ressources
    for (int i = 0; i < NUM_BOULANGERS; i++) {
        buffer_destroy(&buffers[i]);
    }
    
    printf("\n=== Simulation terminée ===\n");
    printf("Total de pains cuits: %d\n", pains_cuits);
    
    return 0;
}
