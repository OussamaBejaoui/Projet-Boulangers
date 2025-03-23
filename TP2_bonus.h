
/*
 * Fichier : TP2_bonus.h
 * Auteur : Bejaoui Oussama
 * UE : Outils pour la commande des systèmes parallèles
 * Tp : fournisseurs/boulangers
 */

#ifndef TP2_BONUS_H
#define TP2_BONUS_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_PAINS 10
#define NUM_BOULANGERS 3
#define BUFFER_SIZE 5

// Structure pour une paire d'ingrédients (0: Farine, 1: Eau, 2: Levure)
typedef struct {
    int ing1;
    int ing2;
} PaireIngredients;

// Buffer circulaire pour les paires d'ingrédients
typedef struct {
    PaireIngredients elements[BUFFER_SIZE];
    int debut;    // Indice de la prochaine paire à consommer
    int fin;      // Indice de la prochaine place libre
    int count;    // Nombre de paires dans le buffer
    pthread_mutex_t mutex;
    pthread_cond_t non_vide;
    pthread_cond_t non_plein;
} Buffer;

// Variables globales
extern Buffer buffers[NUM_BOULANGERS];
extern int IngredientPossede[NUM_BOULANGERS];  // Ingrédient possédé par chaque boulanger
extern int pains_cuits;
extern pthread_mutex_t mutex_pains;

// Fonctions pour manipuler le buffer
void buffer_init(Buffer *buffer);
void buffer_destroy(Buffer *buffer);
bool buffer_est_plein(Buffer *buffer);
bool buffer_est_vide(Buffer *buffer);
bool buffer_ajouter(Buffer *buffer, int ing1, int ing2);
bool buffer_retirer(Buffer *buffer, PaireIngredients *paire);

// Fonctions pour les threads
void* thread_boulanger(void* arg);
void* thread_fournisseur(void* arg);

// Fonctions d'aide
const char* nom_ingredient(int ing);

#endif
