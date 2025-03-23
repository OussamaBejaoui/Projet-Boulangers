# Projet Boulangers

Ce projet est une simulation de boulangers et d'un fournisseur utilisant des threads en C. Le but est de simuler la production de pains en utilisant des ingrédients (farine, eau, levure) qui sont fournis par un fournisseur et utilisés par les boulangers.

## Aperçu du Projet

Le projet est divisé en deux versions principales :
1. **Version de base** : Les boulangers et le fournisseur partagent une table commune pour les ingrédients.
2. **Version bonus** : Chaque boulanger a son propre buffer pour les ingrédients, et le fournisseur approvisionne ces buffers.

### Structure des Fichiers

- **TP2_boulanger.c** : Implémentation de la version de base.
- **TP2_boulanger_main.c** : Programme principal pour la version de base.
- **TP2_boulanger.h** : Fichier d'en-tête pour la version de base.
- **TP2_bonus.c** : Implémentation de la version bonus.
- **TP2_bonus_main.c** : Programme principal pour la version bonus.
- **TP2_bonus.h** : Fichier d'en-tête pour la version bonus.

## Comment Compiler et Exécuter

### Prérequis

- **GCC** : Le compilateur C doit être installé sur votre machine.
- **Git** : Pour cloner le dépôt.

### Version de Base

1. **Cloner le dépôt** :
   ```bash
   git clone https://github.com/OussamaBejaoui/Projet-Boulangers.git
   cd Projet-Boulangers
