#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <SDL.h>
#include <SDL_ttf.h>


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
    TTF_Font* font;
    SDL_Texture* texture;
    int isFocused;
} InputText;

int last_frame_time = 0;
int fenetre_ouverte = FALSE;
int isCreerPressed = 0, isAjouterPressed = 0, isSupprimerPressed = 0, isTrierPressed = 0, isOKPressed = 0;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
Button_Text creer, ajouter, supprimer, trier, ok;
Element_Liste liste[100];
int nbrElements = 0;
Noeud* noeud;
InputText inputText1, inputText2, inputText3, inputText4;
int operation_ajouter = FALSE, operation_supprimer = FALSE;

// Dessiner une flèche
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

// Vérifier si la sourie est sur le bouton
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
    TTF_Font* font2 = TTF_OpenFont("fonts/times.ttf", 12);

    // Créer une surface avec le texte
    SDL_Surface* surface = TTF_RenderText_Solid(font2, text, button->color);

    // Créer une texture à partir de la surface
    button->texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Libérer la mémoire de la surface et fermer le font
    SDL_FreeSurface(surface);
    TTF_CloseFont(font2);
}

// Création du rendu du bouton
void renderButtonText(Button_Text* button, SDL_Renderer* renderer) {
    // Rendu arrière plan du bouton
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, button->color.a);
    SDL_RenderFillRect(renderer, &button->rect);

    // Rendu du texte du bouton
    SDL_RenderCopy(renderer, button->texture, NULL, &button->rect);
}

// Libérer le bouton
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
    TTF_Font* font2 = TTF_OpenFont("fonts/times.ttf", 12);

    // Créer une surface avec le texte
    SDL_Surface* surface = TTF_RenderText_Solid(font2, text, button->bouton.color);

    // Créer une texture à partir de la surface
    button->bouton.texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Libérer la mémoire de la surface et fermer le font
    SDL_FreeSurface(surface);
    TTF_CloseFont(font2);
}

// Création du rendu du bouton
void renderElementListe(Element_Liste* button, SDL_Renderer* renderer, int r, int g, int b) {
    // Rendu arrière plan du bouton
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

// Libérer l'élément de la liste
void freeElementListe(Element_Liste* button) {
    free(button->bouton.text);
    //SDL_DestroyTexture(button->bouton.texture);
}

