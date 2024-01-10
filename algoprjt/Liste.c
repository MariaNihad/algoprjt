#include <stdio.h>
#include <stdlib.h>

#include "Liste.h"

// Fonction pour créer un nouveau nœud
 Noeud* creerNoeud(int valeur) {
    Noeud* nouveauNoeud = ( Noeud*)malloc(sizeof( Noeud));
    if (nouveauNoeud == NULL) {
        printf("Erreur d'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
    }
    nouveauNoeud->donnee = valeur;
    nouveauNoeud->suivant = NULL;
    return nouveauNoeud;
}

// Fonction pour insérer un élément au début de la liste
void insererAuDebut( Noeud** tete, int valeur) {
     Noeud* nouveauNoeud = creerNoeud(valeur);
    nouveauNoeud->suivant = *tete;
    *tete = nouveauNoeud;
}

// Fonction pour insérer un élément à la fin de la liste
void insererALaFin( Noeud** tete, int valeur) {
     Noeud* nouveauNoeud = creerNoeud(valeur);
    if (*tete == NULL) {
        *tete = nouveauNoeud;
    }
    else {
         Noeud* courant = *tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouveauNoeud;
    }
}
// Fonction pour supprimer un élément au début de la liste
void supprimerAuDebut(Noeud * *tete) {
    if (*tete != NULL) {
        Noeud* temp = *tete;
        *tete = (*tete)->suivant;
        free(temp);
    }
    else {
        printf("La liste est vide. Impossible de supprimer.\n");
    }
}

// Fonction pour supprimer un élément à la fin de la liste
void supprimerALaFin( Noeud** tete) {
    if (*tete != NULL) {
        if ((*tete)->suivant == NULL) {
            free(*tete);
            *tete = NULL;
        }
        else {
           Noeud* courant = *tete;
           Noeud* precedent = NULL;
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
// Fonction pour supprimer un élément donné de la liste, il retourne la position du noeud sinon -1
int supprimerValeur(Noeud** tete, int valeur)
{
    if (*tete != NULL)
    {
        if ((*tete)->donnee == valeur)
        {
            supprimerAuDebut(tete);
            return 0;
        }
        int pos = 0;
        Noeud* courant = *tete;
        Noeud* precedent = NULL;
        while (courant->donnee != valeur && courant->suivant != NULL) {
            precedent = courant;
            courant = courant->suivant;
            pos++;
        }
        if (courant->donnee == valeur)
        {
            if (precedent != NULL)
            {
                precedent->suivant = courant->suivant;
            }
            free(courant);
            return pos;
        }
        else {
            printf("L'element recherche n'existe pas.\n");
            return -1;
        }
    }
    else {
        printf("La liste est vide. Impossible de supprimer.\n");
        return -1;
    }
}


// Fonction pour rechercher un élément dans la liste
 Noeud* rechercherElement( Noeud* tete, int valeur) {
   Noeud* courant = tete;
    while (courant != NULL) {
        if (courant->donnee == valeur) {
            return courant;
        }
        courant = courant->suivant;
    }
    return NULL; // Élément non trouvé
}

// Fonction pour trier la liste par insertion
void trierParInsertion( Noeud** tete) {
    if (*tete == NULL || (*tete)->suivant == NULL) {
        return; // La liste est déjà triée
    }

   Noeud* listeTriee = NULL;
   Noeud* courant = *tete;

    while (courant != NULL) {
       Noeud* suivant = courant->suivant;

        // Insérer le nœud courant dans la liste triée
        if (listeTriee == NULL || listeTriee->donnee > courant->donnee) {
            courant->suivant = listeTriee;
            listeTriee = courant;
        }
        else {
        Noeud* precedent = listeTriee;
            while (precedent->suivant != NULL && precedent->suivant->donnee < courant->donnee) {
                precedent = precedent->suivant;
            }
            courant->suivant = precedent->suivant;
            precedent->suivant = courant;
        }

        courant = suivant;
    }*tete = listeTriee;
}

// Fonction pour afficher les éléments de la liste
void afficherListe( Noeud* tete) {
    Noeud* courant = tete;
    while (courant != NULL) {
        printf("%d -> ", courant->donnee);
        courant = courant->suivant;
    }
    printf("NULL\n");
}



 