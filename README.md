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
- **Git** : Pour cloner le dépôt (optionnel).

### Version de Base

1. **Cloner le dépôt** :
   ```bash
   git clone https://github.com/OussamaBejaoui/Projet-Boulangers.git
   cd Projet-Boulangers

    Compiler le programme :
    bash
    Copy

    gcc TP2_boulanger.c TP2_boulanger_main.c -o boulanger_simulation -lpthread

    Exécuter le programme :
    bash
    Copy

    ./boulanger_simulation

Version Bonus

    Cloner le dépôt :
    bash
    Copy

    git clone https://github.com/OussamaBejaoui/Projet-Boulangers.git
    cd Projet-Boulangers

    Compiler le programme :
    bash
    Copy

    gcc TP2_bonus.c TP2_bonus_main.c -o boulanger_bonus_simulation -lpthread

    Exécuter le programme :
    bash
    Copy

    ./boulanger_bonus_simulation

Explication des Paramètres

    NUM_PAINS : Nombre total de pains à cuire avant que le programme ne se termine.

    NUM_BOULANGERS : Nombre de boulangers participant à la simulation.

    BUFFER_SIZE : Taille du buffer pour chaque boulanger dans la version bonus.

Synchronisation

Le projet utilise des mutex et des variables conditionnelles pour synchroniser l'accès aux ressources partagées entre les threads.
Auteur

    Oussama Bejaoui : Étudiant en systèmes parallèles.

Licence

Ce projet est sous licence MIT. Voir le fichier LICENSE pour plus de détails.
Comment Contribuer

Si vous souhaitez contribuer à ce projet, suivez ces étapes :

    Forker le dépôt :

        Cliquez sur le bouton "Fork" en haut à droite de la page du dépôt.

    Cloner votre fork :
    bash
    Copy

    git clone https://github.com/votre-utilisateur/Projet-Boulangers.git
    cd Projet-Boulangers

    Créer une nouvelle branche :
    bash
    Copy

    git checkout -b nom-de-la-branche

    Faire des modifications :

        Ajoutez vos modifications et testez-les.

    Pousser les modifications :
    bash
    Copy

    git add .
    git commit -m "Description des modifications"
    git push origin nom-de-la-branche

    Ouvrir une pull request :

        Allez sur votre fork GitHub et cliquez sur "New pull request".

        Décrivez vos modifications et soumettez la pull request.

Issues

Si vous rencontrez des problèmes ou si vous avez des suggestions, vous pouvez ouvrir une issue sur GitHub :

    Allez dans l'onglet "Issues" et cliquez sur "New issue".

Remerciements

    Merci à tous ceux qui ont contribué à ce projet !

    Merci à GitHub pour l'hébergement du dépôt.

Contact

Pour toute question ou suggestion, vous pouvez me contacter à l'adresse suivante :

    Email : votre_email@example.com

    GitHub : OussamaBejaoui

Copy


---

### **Explication des sections**

1. **Titre et description** : Explique brièvement le projet.
2. **Structure des fichiers** : Liste les fichiers importants du projet.
3. **Comment compiler et exécuter** : Donne des instructions pour compiler et exécuter les deux versions du projet.
4. **Explication des paramètres** : Décrit les variables globales du projet.
5. **Synchronisation** : Explique comment la synchronisation est gérée dans le projet.
6. **Auteur et licence** : Donne des informations sur l'auteur et la licence du projet.
7. **Comment contribuer** : Explique comment les autres peuvent contribuer au projet.
8. **Issues** : Explique comment signaler des problèmes ou des suggestions.
9. **Remerciements et contact** : Ajoute une touche personnelle et fournit des informations de contact.

---

### **Comment utiliser ce fichier**

1. Copiez ce contenu dans un fichier `README.md` dans votre projet.
2. Personnalisez les sections si nécessaire (par exemple, remplacez `votre_email@example.com` par votre email réel).
3. Poussez le fichier sur GitHub :
   ```bash
   git add README.md
   git commit -m "Ajout du fichier README.md"
   git push origin main
