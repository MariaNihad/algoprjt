#ifndef INCLUDED_H_LISTE
#define INCLUDED_H_LISTE

struct Noeud {
    int donnee;
    struct Noeud* suivant;
};

// Fonction pour créer un nouveau nœud
struct Noeud* creerNoeud(int valeur);

// Fonction pour insérer un élément au début de la liste
void insererAuDebut(struct Noeud** tete, int valeur);

// Fonction pour insérer un élément à la fin de la liste
void insererALaFin(struct Noeud** tete, int valeur);

// Fonction pour supprimer un élément au début de la liste
void supprimerAuDebut(struct Noeud** tete);

// Fonction pour supprimer un élément à la fin de la liste
void supprimerALaFin(struct Noeud** tete);

// Fonction pour rechercher un élément dans la liste
struct Noeud* rechercherElement(struct Noeud* tete, int valeur);

// Fonction pour trier la liste par insertion
void trierParInsertion(struct Noeud** tete);

// Fonction pour afficher les éléments de la liste
void afficherListe(struct Noeud* tete);

#endif // !INCLUDED_H_LISTE
