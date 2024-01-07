
#ifndef INCLUDED_H_LISTE
#define INCLUDED_H_LISTE

typedef struct Noeud {
    int donnee;
    struct Noeud* suivant;
}Noeud;

// Fonction pour créer un nouveau nœud
Noeud* creerNoeud(int valeur);

// Fonction pour insérer un élément au début de la liste
void insererAuDebut(Noeud** tete, int valeur);

// Fonction pour insérer un élément à la fin de la liste
void insererALaFin(Noeud** tete, int valeur);

// Fonction pour supprimer un élément au début de la liste
void supprimerAuDebut(Noeud** tete);

// Fonction pour supprimer un élément à la fin de la liste
void supprimerALaFin(Noeud** tete);

// Fonction pour supprimer un élément de la liste s'il existe
int supprimerValeur(Noeud** tete, int valeur);

// Fonction pour rechercher un élément dans la liste
Noeud* rechercherElement(Noeud* tete, int valeur);

// Fonction pour trier la liste par insertion
void trierParInsertion(Noeud** tete);

// Fonction pour afficher les éléments de la liste
void afficherListe(Noeud* tete);

#endif // !INCLUDED_H_LISTE
