

/*
 * Fichier : TP2_bonus.c
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
#include <stdbool.h>

// Variables globales
Buffer buffers[NUM_BOULANGERS];
int IngredientPossede[NUM_BOULANGERS] = {0, 1, 2};  // 0: Farine, 1: Eau, 2: Levure
int pains_cuits = 0;
pthread_mutex_t mutex_pains = PTHREAD_MUTEX_INITIALIZER;

// Initialiser un buffer
void buffer_init(Buffer *buffer) {
    buffer->debut = 0;
    buffer->fin = 0;
    buffer->count = 0;
    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->non_vide, NULL);
    pthread_cond_init(&buffer->non_plein, NULL);
}

// Libérer les ressources d'un buffer
void buffer_destroy(Buffer *buffer) {
    pthread_mutex_destroy(&buffer->mutex);
    pthread_cond_destroy(&buffer->non_vide);
    pthread_cond_destroy(&buffer->non_plein);
}

// Vérifier si un buffer est plein
bool buffer_est_plein(Buffer *buffer) {
    return buffer->count >= BUFFER_SIZE;
}

// Vérifier si un buffer est vide
bool buffer_est_vide(Buffer *buffer) {
    return buffer->count <= 0;
}

// Ajouter une paire d'ingrédients au buffer (non bloquant)
bool buffer_ajouter(Buffer *buffer, int ing1, int ing2) {
    bool succes = false;
    
    pthread_mutex_lock(&buffer->mutex);
    
    if (!buffer_est_plein(buffer)) {
        buffer->elements[buffer->fin].ing1 = ing1;
        buffer->elements[buffer->fin].ing2 = ing2;
        buffer->fin = (buffer->fin + 1) % BUFFER_SIZE;
        buffer->count++;
        succes = true;
        
        // Signaler qu'il y a des données disponibles
        pthread_cond_signal(&buffer->non_vide);
    }
    
    pthread_mutex_unlock(&buffer->mutex);
    
    return succes;
}

// Retirer une paire d'ingrédients du buffer 
bool buffer_retirer(Buffer *buffer, PaireIngredients *paire) {
    pthread_mutex_lock(&buffer->mutex);
    
    // Attendre que le buffer ne soit plus vide
    while (buffer_est_vide(buffer)) {
        // Vérifier si on a atteint le nombre de pains requis
        pthread_mutex_lock(&mutex_pains);
        bool termine = pains_cuits >= NUM_PAINS;
        pthread_mutex_unlock(&mutex_pains);
        
        if (termine) {
            pthread_mutex_unlock(&buffer->mutex);
            return false;
        }
        
        pthread_cond_wait(&buffer->non_vide, &buffer->mutex);
    }
    
    // Récupérer la paire d'ingrédients
    *paire = buffer->elements[buffer->debut];
    buffer->debut = (buffer->debut + 1) % BUFFER_SIZE;
    buffer->count--;
    
    // Signaler qu'il y a de la place dans le buffer
    pthread_cond_signal(&buffer->non_plein);
    
    pthread_mutex_unlock(&buffer->mutex);
    
    return true;
}

// Obtenir le nom textuel d'un ingrédient
const char* nom_ingredient(int ing) {
    const char* noms[] = {"FARINE", "EAU", "LEVURE"};
    return noms[ing];
}



// Thread pour un boulanger
void* thread_boulanger(void* arg) {
    int id = *((int*)arg);
    int ingredient_possede = IngredientPossede[id];
    int ingredient_manquant1, ingredient_manquant2;
    
    // Déterminer les ingrédients manquants pour ce boulanger
    if (ingredient_possede == 0) {
        ingredient_manquant1 = 1;  // Eau
        ingredient_manquant2 = 2;  // Levure
    } else if (ingredient_possede == 1) {
        ingredient_manquant1 = 0;  // Farine
        ingredient_manquant2 = 2;  // Levure
    } else {
        ingredient_manquant1 = 0;  // Farine
        ingredient_manquant2 = 1;  // Eau
    }
    
    while (1) {
        // Vérifier si on a atteint le nombre total de pains
        pthread_mutex_lock(&mutex_pains);
        if (pains_cuits >= NUM_PAINS) {
            pthread_mutex_unlock(&mutex_pains);
            break;
        }
        pthread_mutex_unlock(&mutex_pains);
        
        printf("Boulanger %d (possède %s) recherche %s et %s dans son buffer\n", 
               id, nom_ingredient(ingredient_possede), 
               nom_ingredient(ingredient_manquant1), 
               nom_ingredient(ingredient_manquant2));
        
        // Essayer de récupérer une paire d'ingrédients
        PaireIngredients paire;
        bool obtenu = buffer_retirer(&buffers[id], &paire);
        
        if (!obtenu) {
            // Si on n'a pas pu obtenir d'ingrédients, le programme se termine
            break;
        }
        
        // Vérifier qu'on a bien les ingrédients dont on a besoin
        bool a_ing1 = (paire.ing1 == ingredient_manquant1 || paire.ing2 == ingredient_manquant1);
        bool a_ing2 = (paire.ing1 == ingredient_manquant2 || paire.ing2 == ingredient_manquant2);
        
        if (a_ing1 && a_ing2) {
            printf("Boulanger %d a trouvé les ingrédients nécessaires : %s et %s\n", 
                   id, nom_ingredient(ingredient_manquant1), 
                   nom_ingredient(ingredient_manquant2));
            
            // Pétrir le pain (0.3s)
            printf("Boulanger %d pétrit son pain\n", id);
            usleep(300000);
            
            // Cuire le pain (0.7s)
            printf("Boulanger %d cuit son pain\n", id);
            usleep(700000);
            
            // Incrémenter le compteur de pains
            pthread_mutex_lock(&mutex_pains);
            pains_cuits++;
            printf("Boulanger %d a terminé son pain ! (Total: %d/%d)\n", 
                   id, pains_cuits, NUM_PAINS);
            pthread_mutex_unlock(&mutex_pains);
            
        } else {
            printf("Boulanger %d n'a pas trouvé les bons ingrédients, il continue de chercher\n", id);
        }
    }
    
    printf("Boulanger %d termine son travail\n", id);
    return NULL;
}

// Thread pour le fournisseur
void* thread_fournisseur(void* arg) {
    (void)arg;  
    
    while (1) {
        // Vérifier si on a atteint le nombre total de pains
        pthread_mutex_lock(&mutex_pains);
        if (pains_cuits >= NUM_PAINS) {
            // Réveiller tous les boulangers qui pourraient être en attente
            for (int i = 0; i < NUM_BOULANGERS; i++) {
                pthread_cond_broadcast(&buffers[i].non_vide);
            }
            pthread_mutex_unlock(&mutex_pains);
            break;
        }
        pthread_mutex_unlock(&mutex_pains);
        
        // Générer une paire d'ingrédients aléatoire
        int ing1 = rand() % 3;
        int ing2;
        do {
            ing2 = rand() % 3;
        } while (ing2 == ing1);  // S'assurer que les deux ingrédients sont différents
        
        // Déterminer pour quel boulanger
        int boulanger_cible = -1;
        for (int i = 0; i < NUM_BOULANGERS; i++) {
            int possede = IngredientPossede[i];
            if (possede != ing1 && possede != ing2) {
                boulanger_cible = i;
                break;
            }
        }
        
        if (boulanger_cible != -1) {
            // Essayer d'ajouter la paire au buffer du boulanger cible
            bool ajoute = buffer_ajouter(&buffers[boulanger_cible], ing1, ing2);
            
            if (ajoute) {
                printf("Fournisseur a ajouté %s et %s au buffer du boulanger %d\n", 
                       nom_ingredient(ing1), nom_ingredient(ing2), boulanger_cible);
            } else {
                printf("Fournisseur doit jeter %s et %s (buffer du boulanger %d plein)\n", 
                       nom_ingredient(ing1), nom_ingredient(ing2), boulanger_cible);
            }
        }

        usleep(100000);  
    }
    
    printf("Fournisseur termine son travail\n");
    return NULL;
}
