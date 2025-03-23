/*
 * Fichier : TP2_boulanger.h
 * Auteur : Bejaoui Oussama
 * UE : Outils pour la commande des systèmes parallèles
 * Tp : fournisseurs/boulangers
 */



#ifndef TP2_BOULANGER_H
#define TP2_BOULANGER_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PAINS 5
#define NUM_BOULANGERS 3

// Ingrédients disponibles sur la table
extern int Ing[3];  // 0: Farine, 1: Eau, 2: Levure

// Variables pour synchronisation
extern pthread_mutex_t mut;
extern pthread_cond_t Ing_Dispo, Fou_Dispo;

// Ingrédient possédé par chaque boulanger
extern int IngredientPossede[NUM_BOULANGERS];

// Total des pains cuits par tous les boulangers
extern int pains_cuits;

// Dépose deux ingrédients sur la table
void depoIngredients(int Ing1, int Ing2);

// Boulanger prend les ingrédients dont il a besoin
void prendreIngredients(int i);

// Signale au fournisseur que le pain est terminé
void signalerFinPain(int i);

// Conversion nom d'ingrédient en texte
const char* nomIngredient(int ing);

#endif
