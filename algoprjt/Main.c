
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <SDL.h>



#include "Liste.h"

#define TRUE 1
#define FALSE 0
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)
#define MAX_TEXT_LENGTH 100

// Structure Bouton
typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    SDL_Texture* texture;
    char* text;
} Button_Text;

// Bordure d'un bouton
typedef struct {
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
} Border;

// Structure d'un Noeud de liste
typedef struct {
    Button_Text bouton;
    Border border;
    int valeur;
} Element_Liste;

typedef struct {
    char text[MAX_TEXT_LENGTH];
    SDL_Rect rect;
    SDL_Color color;
    
    SDL_Texture* texture;
    int isFocused;
} InputText;

int last_frame_time = 0;
int fenetre_ouverte = FALSE;
int isCreerPressed = 0, isAjouterPressed = 0, isSupprimerPressed = 0, isTrierPressed = 0, isOKPressed = 0;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Button_Text creer, ajouter, supprimer, trier, ok;
Element_Liste liste[100];
int nbrElements = 0;
Noeud* noeud;
InputText inputText1, inputText2, inputText3, inputText4;
int operation_ajouter = FALSE, operation_supprimer = FALSE;

// Dessiner une fl�che
void drawArrow(SDL_Renderer* renderer, int startX, int startY, int endX, int endY) {
    // Set the drawing color to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Draw a line for the arrow's body
    SDL_RenderDrawLine(renderer, startX, startY, endX, endY);

    // Calculate arrowhead points
    int arrowSize = 5;
    double angle = atan2(endY - startY, endX - startX);
    double arrowX1 = endX - arrowSize * cos(angle - M_PI / 6);
    double arrowY1 = endY - arrowSize * sin(angle - M_PI / 6);
    double arrowX2 = endX - arrowSize * cos(angle + M_PI / 6);
    double arrowY2 = endY - arrowSize * sin(angle + M_PI / 6);

    // Draw lines for the arrowhead
    SDL_RenderDrawLine(renderer, endX, endY, (int)arrowX1, (int)arrowY1);
    SDL_RenderDrawLine(renderer, endX, endY, (int)arrowX2, (int)arrowY2);
}

// V�rifier si la sourie est sur le bouton
int isPointInsideButtonText(int x, int y, const Button_Text* button) {
    return (x >= button->rect.x && x <= button->rect.x + button->rect.w &&
        y >= button->rect.y && y <= button->rect.y + button->rect.h);
}

// Initialiser un bouton
void initButton(Button_Text* button, SDL_Renderer* renderer, const char* text, int x, int y, int w, int h) {
    // Positions et Mesures du bouton
    button->rect.x = x;
    button->rect.y = y;
    button->rect.w = w;
    button->rect.h = h;

    // La couleur du texte bouton
    button->color = (SDL_Color){ 255, 255, 255, 255 };

    // La copie du texte au boutton
    button->text = strdup(text);

    // Charger le font
  

    // Cr�er une surface avec le texte
    SDL_Surface* surface = TTF_RenderText_Solid(font2, text, button->color);

    // Cr�er une texture � partir de la surface
    button->texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Lib�rer la m�moire de la surface et fermer le font
    SDL_FreeSurface(surface);
    TTF_CloseFont(font2);
}

// Cr�ation du rendu du bouton
void renderButtonText(Button_Text* button, SDL_Renderer* renderer) {
    // Rendu arri�re plan du bouton
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, button->color.a);
    SDL_RenderFillRect(renderer, &button->rect);

    // Rendu du texte du bouton
    SDL_RenderCopy(renderer, button->texture, NULL, &button->rect);
}

// Lib�rer le bouton
void freeButton(Button_Text* button) {
    free(button->text);
    //SDL_DestroyTexture(button->texture);
}

// Initialiser un bouton de la liste
void initElementListe(Element_Liste* button, SDL_Renderer* renderer, const char* text, int x, int y, int w, int h) {
    // Positions et Mesures du bouton
    button->bouton.rect.x = x;
    button->bouton.rect.y = y;
    button->bouton.rect.w = w;
    button->bouton.rect.h = h;
    button->border.xStart = x;
    button->border.yStart = y;
    button->border.xEnd = x + w;
    button->border.yEnd = y + h;

    // La couleur du texte bouton
    button->bouton.color = (SDL_Color){ 0, 0, 0, 255 };

    // La copie du texte au boutton
    button->bouton.text = strdup(text);

    // La copie de la valeur
    button->valeur = atoi(text);

    // Charger le font
    

    // Cr�er une surface avec le texte
    SDL_Surface* surface = TTF_RenderText_Solid(font2, text, button->bouton.color);

    // Cr�er une texture � partir de la surface
    button->bouton.texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Lib�rer la m�moire de la surface et fermer le font
    SDL_FreeSurface(surface);
    TTF_CloseFont(font2);
}

// Cr�ation du rendu du bouton
void renderElementListe(Element_Liste* button, SDL_Renderer* renderer, int r, int g, int b) {
    // Rendu arri�re plan du bouton
    SDL_SetRenderDrawColor(renderer, button->bouton.color.r, button->bouton.color.g, button->bouton.color.b, button->bouton.color.a);
    SDL_RenderFillRect(renderer, &button->bouton.rect);

    // Rendu du texte du bouton
    SDL_RenderCopy(renderer, button->bouton.texture, NULL, &button->bouton.rect);

    // Bordure
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderDrawLine(renderer, button->border.xStart, button->border.yStart, button->border.xEnd, button->border.yStart);
    SDL_RenderDrawLine(renderer, button->border.xEnd, button->border.yStart, button->border.xEnd, button->border.yEnd);
    SDL_RenderDrawLine(renderer, button->border.xEnd, button->border.yEnd, button->border.xStart, button->border.yEnd);
    SDL_RenderDrawLine(renderer, button->border.xStart, button->border.yEnd, button->border.xStart, button->border.yStart);
}

// Lib�rer l'�l�ment de la liste
void freeElementListe(Element_Liste* button) {
    free(button->bouton.text);
    //SDL_DestroyTexture(button->bouton.texture);
}

// Une fonction qui donne le focus � un champ de texte
void setFocus(InputText* inputText) {
    inputText->isFocused = 1;
}

void clearFocus(InputText* inputText1) {
    inputText1->isFocused = 0;
}

void initInputText(InputText* inputText, SDL_Renderer* renderer, int x, int y, int w, int h) {
    inputText->rect.x = x;
    inputText->rect.y = y;
    inputText->rect.w = w;
    inputText->rect.h = h;
    inputText->color = (SDL_Color){ 0, 0, 0, 255 };
    inputText->font = TTF_OpenFont("fonts/times.ttf", 18);

    strcpy(inputText->text, "");
    SDL_Surface* surface = TTF_RenderText_Solid(inputText->font, inputText->text, inputText->color);
    inputText->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    inputText->isFocused = 0;
}

void handleInputText(InputText* inputText, SDL_Event* e) {
    if (inputText->isFocused) {
        if (e->type == SDL_KEYDOWN) {
            // Handle backspace
            if (e->key.keysym.sym == SDLK_BACKSPACE && strlen(inputText->text) > 0) {
                inputText->text[strlen(inputText->text) - 1] = '\0';
            }
            // Handle other key presses
            else if (isdigit(SDL_GetKeyName(e->key.keysym.sym)[0]) && strlen(inputText->text) < MAX_TEXT_LENGTH - 1) {
                strcat(inputText->text, SDL_GetKeyName(e->key.keysym.sym));
            }

            SDL_Surface* surface = TTF_RenderText_Solid(inputText->font, inputText->text, inputText->color);
            SDL_DestroyTexture(inputText->texture);
            inputText->texture = SDL_CreateTextureFromSurface(SDL_GetRenderer(SDL_GetWindowFromID(e->key.windowID)), surface);
            SDL_FreeSurface(surface);
        }
    }
}

void renderInputText(InputText* inputText, SDL_Renderer* renderer, int r, int v, int b) {
    SDL_SetRenderDrawColor(renderer, r, v, b, 255);
    SDL_RenderFillRect(renderer, &inputText->rect);
    SDL_RenderCopy(renderer, inputText->texture, NULL, &inputText->rect);
}

v

// Fonction principale
int main()
{

    fenetre_ouverte = initialiser_window();

    installer();

    while (fenetre_ouverte)
    {
        entrees_processus();
        mise_a_jour();
        rendu();
    }

    destruction_fenetre();

    /*
    Noeud* tete = NULL;

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
    Noeud* elementRecherche = rechercherElement(tete, valeurRecherchee);
    if (elementRecherche != NULL) {
        printf("Element %d trouve dans la liste.\n", valeurRecherchee);
    }
    else {
        printf("Element %d non trouve dans la liste.\n", valeurRecherchee);
    }

    // Tri par insertion
    trierParInsertion(&tete);

    // Affichage apr�s le tri
    printf("Liste apr�s le tri par insertion : ");
    afficherListe(tete);

    // Suppression au d�but
    supprimerAuDebut(&tete);

    // Suppression � la fin
    supprimerALaFin(&tete);

    // Affichage apr�s les suppressions
    printf("Liste apr�s les suppressions : ");
    afficherListe(tete);
    */

    return 0;
}