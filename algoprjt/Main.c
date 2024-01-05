#include <stdio.h>
#include <SDL.h>
#include "Liste.h"



int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Init(SDL_INIT_VIDEO);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("erreur", SDL_GetError());
    }

    SDL_Delay(7000);

    window = SDL_CreateWindowAndRenderer(400,300,0, &window ,&renderer);

    SDL_RenderSetScale(renderer,4 ,4);

    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

  
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderDrawPoint(renderer,320,280);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
      SDL_QUIT;
      SDL_Event e;
      int quit = 0;
      while (!quit) {
          while (SDL_PollEvent(&e) != 0) {
              if (e.type == SDL_QUIT) {
                  quit = 1;
              }
          }

          // Draw the linked list
          drawLinkedList(renderer, head);

          SDL_Delay(1000); // Add delay to make visualization visible
      }

      // Clean up
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      SDL_Quit();

        

    struct Noeud *tete = NULL;
    // Insertion d'éléments non triés
    insererAuDebut(&tete, 3);
    insererAuDebut(&tete, 1);
    insererAuDebut(&tete, 2);
    insererAuDebut(&tete, 5);
    insererAuDebut(&tete, 4);

    // Affichage avant les opérations
    printf("Liste avant les opérations : ");
    afficherListe(tete);

    // Recherche d'un élément
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

    // Affichage après le tri
    printf(" liste apres trie par insertion : ");
    afficherListe(tete);

    // Suppression au debut
    supprimerAuDebut(&tete);

    // Suppression a la fin
    supprimerALaFin(&tete);

    // Affichage après les suppressions
    printf("liste apres  suppressions : ");
    afficherListe(tete);

    return 0;

}