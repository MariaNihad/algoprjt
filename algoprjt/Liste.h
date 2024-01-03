#ifndef INCLUDED_H_LISTE
#define INCLUDED_H_LISTE

struct Noeud {
    int donnee;
    struct Noeud* suivant;
};

// Fonction pour cr�er un nouveau n�ud
struct Noeud* creerNoeud(int valeur);

// Fonction pour ins�rer un �l�ment au d�but de la liste
void insererAuDebut(struct Noeud** tete, int valeur);

// Fonction pour ins�rer un �l�ment � la fin de la liste
void insererALaFin(struct Noeud** tete, int valeur);

// Fonction pour supprimer un �l�ment au d�but de la liste
void supprimerAuDebut(struct Noeud** tete);

// Fonction pour supprimer un �l�ment � la fin de la liste
void supprimerALaFin(struct Noeud** tete);

// Fonction pour rechercher un �l�ment dans la liste
struct Noeud* rechercherElement(struct Noeud* tete, int valeur);

// Fonction pour trier la liste par insertion
void trierParInsertion(struct Noeud** tete);

// Fonction pour afficher les �l�ments de la liste
void afficherListe(struct Noeud* tete);

#endif // !INCLUDED_H_LISTE
