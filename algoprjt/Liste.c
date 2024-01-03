#include <stdio.h>
#include <stdlib.h>

#include "Liste.h"

// Fonction pour créer un nouveau nœud
struct Noeud* creerNoeud(int valeur) {
    struct Noeud* nouveauNoeud = (struct Noeud*)malloc(sizeof(struct Noeud));
    if (nouveauNoeud == NULL) {
        printf("Erreur d'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
    }
    nouveauNoeud->donnee = valeur;
    nouveauNoeud->suivant = NULL;
    return nouveauNoeud;
}

// Fonction pour insérer un élément au début de la liste
void insererAuDebut(struct Noeud** tete, int valeur) {
    struct Noeud* nouveauNoeud = creerNoeud(valeur);
    nouveauNoeud->suivant = *tete;
    *tete = nouveauNoeud;
}

// Fonction pour insérer un élément à la fin de la liste
void insererALaFin(struct Noeud** tete, int valeur) {
    struct Noeud* nouveauNoeud = creerNoeud(valeur);
    if (*tete == NULL) {
        *tete = nouveauNoeud;
    }
    else {
        struct Noeud* courant = *tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouveauNoeud;
    }
}

// Fonction pour supprimer un élément au début de la liste
void supprimerAuDebut(struct Noeud** tete) {
    if (*tete != NULL) {
        struct Noeud* temp = *tete;
        *tete = (*tete)->suivant;
        free(temp);
    }
    else {
        printf("La liste est vide. Impossible de supprimer.\n");
    }
}

// Fonction pour supprimer un élément à la fin de la liste
void supprimerALaFin(struct Noeud** tete) {
    if (*tete != NULL) {
        if ((*tete)->suivant == NULL) {
            free(*tete);
            *tete = NULL;
        }
        else {
            struct Noeud* courant = *tete;
            struct Noeud* precedent = NULL;
            while (courant->suivant != NULL) {
                precedent = courant;
                courant = courant->suivant;
            }
            free(courant);
            precedent->suivant = NULL;
        }
    }
    else {
        printf("La liste est vide. Impossible de supprimer.\n");
    }
}

// Fonction pour rechercher un élément dans la liste
struct Noeud* rechercherElement(struct Noeud* tete, int valeur) {
    struct Noeud* courant = tete;
    while (courant != NULL) {
        if (courant->donnee == valeur) {
            return courant;
        }
        courant = courant->suivant;
    }
    return NULL; // Élément non trouvé
}

// Fonction pour trier la liste par insertion
void trierParInsertion(struct Noeud** tete) {
    if (*tete == NULL || (*tete)->suivant == NULL) {
        return; // La liste est déjà triée
    }

    struct Noeud* listeTriee = NULL;
    struct Noeud* courant = *tete;

    while (courant != NULL) {
        struct Noeud* suivant = courant->suivant;

        // Insérer le nœud courant dans la liste triée
        if (listeTriee == NULL || listeTriee->donnee > courant->donnee) {
            courant->suivant = listeTriee;
            listeTriee = courant;
        }
        else {
            struct Noeud* precedent = listeTriee;
            while (precedent->suivant != NULL && precedent->suivant->donnee < courant->donnee) {
                precedent = precedent->suivant;
            }
            courant->suivant = precedent->suivant;
            precedent->suivant = courant;
        }

        courant = suivant;
    }

    // Mettre à jour la tête de la liste avec la liste triée
    *tete = listeTriee;
}

// Fonction pour afficher les éléments de la liste
void afficherListe(struct Noeud* tete) {
    struct Noeud* courant = tete;
    while (courant != NULL) {
        printf("%d -> ", courant->donnee);
        courant = courant->suivant;
    }
    printf("NULL\n");
}
