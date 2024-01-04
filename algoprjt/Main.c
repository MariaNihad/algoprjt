#include <stdio.h>
#include <SDL.h>
#include "Liste.h"



int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Init(SDL_INIT_VIDEO);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("erreur", SDL_GetError());
    }
    window = SDL_CreateWindow("sdl avec les liste",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1000,
        1000,
        SDL_WINDOW_SHOWN);
        SDL_DestroyWindow(window);
        SDL_QUIT;
        

    struct Noeud *tete = NULL;

    // Insertion d'�l�ments non tri�s
    insererAuDebut(&tete, 3);
    insererAuDebut(&tete, 1);
    insererAuDebut(&tete, 2);
    insererAuDebut(&tete, 5);
    insererAuDebut(&tete, 4);

    // Affichage avant les op�rations
    printf("Liste avant les op�rations : ");
    afficherListe(tete);

    // Recherche d'un �l�ment
    int valeurRecherchee = 3;
    struct Noeud* elementRecherche = rechercherElement(tete, valeurRecherchee);
    if (elementRecherche != NULL) {
        printf("Element %d trouve dans la liste.\n", valeurRecherchee);
    }
    else {
        printf("Element %d non trouve dans la liste.\n", valeurRecherchee);
    }

    // Tri par insertion
    trierParInsertion(&tete);

    // Affichage apr�s le tri
    printf(" liste apres trie par insertion : ");
    afficherListe(tete);

    // Suppression au debut
    supprimerAuDebut(&tete);

    // Suppression a la fin
    supprimerALaFin(&tete);

    // Affichage apr�s les suppressions
    printf("liste apres  suppressions : ");
    afficherListe(tete);

    return 0;

}