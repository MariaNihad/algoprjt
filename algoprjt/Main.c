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

// Une fonction qui donne le focus ç un champ de texte
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

void freeInputText(InputText* inputText) {
    TTF_CloseFont(inputText->font);
    SDL_DestroyTexture(inputText->texture);
}

void message(InputText* input, const char* msg, int flag)
{
    strcpy(input->text, msg);
    input->rect.w = 200;
    SDL_Color color = (SDL_Color){ 250, 0, 0, 255 };
    if (flag == 0)
    {
        color.r = 0;
        color.b = 250;
    }
    SDL_Surface* surface = TTF_RenderText_Solid(input->font, input->text, color);
    input->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

int initialiser_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL.\n");
        return FALSE;
    }
    if (TTF_Init() < 0)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de TTF.\n");
        return FALSE;
    }
    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        fprintf(stderr, "Erreur lors de la creation de la fenetre.\n");
        return FALSE;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        fprintf(stderr, "Erreur lors de la creation ddu rendue de la fenetre.\n");
        return FALSE;
    }
    font = TTF_OpenFont("fonts/times.ttf", 18);
    if (!font)
    {
        fprintf(stderr, "Erreur lors de la creation du font des textes.\n");
        return FALSE;
    }
    return TRUE;
}

void installer(void)
{
    initInputText(&inputText1, renderer, 700, 570, 50, 30);
    initInputText(&inputText2, renderer, 100, 100, 300, 30);
    initInputText(&inputText3, renderer, 0, 570, 200, 30);
    initInputText(&inputText4, renderer, 50, 200, 50, 30);
    strcpy(inputText2.text, "Liste : Aucune Liste n'est créée (Cliquer sur \"Créer\")");
    SDL_Surface* surface = TTF_RenderText_Solid(inputText2.font, inputText2.text, inputText2.color);
    inputText2.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    strcpy(inputText4.text, "NULL");
    SDL_Surface* surface2 = TTF_RenderText_Solid(inputText4.font, inputText4.text, inputText4.color);
    inputText4.texture = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_FreeSurface(surface2);
}

void entrees_processus(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        fenetre_ouverte = FALSE;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            fenetre_ouverte = FALSE;
        }
        if (event.key.keysym.sym == SDLK_c)
        {
            isCreerPressed = 1;
            if (nbrElements == 0)
            {
                strcpy(inputText2.text, "Liste : ");
                inputText2.rect.w = 100;
                SDL_Surface* surface = TTF_RenderText_Solid(inputText2.font, inputText2.text, inputText2.color);
                inputText2.texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                inputText1.isFocused = 1;
                message(&inputText3, "Donner la valeur de l'élément à ajouter : ", 0);
                operation_ajouter = TRUE;
            }
            else
            {
                message(&inputText3, "Il existe déjà une liste.", 1);
            }
        }
        if (event.key.keysym.sym == SDLK_a)
        {
            isAjouterPressed = 1;
            inputText1.isFocused = 1;
            message(&inputText3, "Donner la valeur de l'élément à ajouter : ", 0);
            operation_ajouter = TRUE;
        }
        if (event.key.keysym.sym == SDLK_s)
        {
            isSupprimerPressed = 1;
            inputText1.isFocused = 1;
            message(&inputText3, "Donner la valeur de l'élément à supprimer : ", 0);
            operation_supprimer = TRUE;
        }
        if (event.key.keysym.sym == SDLK_t)
        {
            isTrierPressed = 1;
            trierParInsertion(&noeud);
            int i;
            Noeud* courant = noeud;
            for (i = 0; i < nbrElements; i++)
            {
                Element_Liste e;
                char t[sizeof(int) * 8 + 1];
                _itoa_s(courant->donnee, t, sizeof(int) * 8 + 1, 10);
                initElementListe(&e, renderer, t, (i + 1) * 50 + i * 30, 200, 50, 30);
                e.valeur = courant->donnee;
                liste[i] = e;
                courant = courant->suivant;
            }
        }
        if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_RETURN2 || event.key.keysym.scancode == SDL_SCANCODE_KP_CLEARENTRY)
        {
            isOKPressed = 1;
            if (operation_ajouter)
            {
                if (nbrElements == 0)
                {
                    noeud = creerNoeud(atoi(inputText1.text));
                    Element_Liste e;
                    initElementListe(&e, renderer, inputText1.text, 50, 200, 50, 30);
                    e.valeur = atoi(inputText1.text);
                    liste[nbrElements] = e;
                    nbrElements++;
                    strcpy(inputText1.text, "");
                    SDL_Surface* surface = TTF_RenderText_Solid(inputText1.font, inputText1.text, inputText1.color);
                    inputText1.texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_FreeSurface(surface);
                    inputText1.isFocused = 0;
                }
                else
                {
                    insererALaFin(&noeud, atoi(inputText1.text));
                    Element_Liste e;
                    initElementListe(&e.bouton, renderer, inputText1.text, (nbrElements + 1) * 50 + nbrElements * 30, 200, 50, 30);
                    e.valeur = atoi(inputText1.text);
                    liste[nbrElements] = e;
                    nbrElements++;
                    strcpy(inputText1.text, "");
                    SDL_Surface* surface = TTF_RenderText_Solid(inputText1.font, inputText1.text, inputText1.color);
                    inputText1.texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_FreeSurface(surface);
                    inputText1.isFocused = 0;
                }
            }
            else if (operation_supprimer)
            {
                if (nbrElements != 0)
                {
                    int pos = supprimerValeur(&noeud, atoi(inputText1.text));
                    if (pos != -1)
                    {
                        int i;
                        for (i = 0; i <= pos; i++)
                        {
                            if (i < pos)
                            {
                                liste[i].bouton.color = (SDL_Color){ 10, 255, 50, 255 };
                                renderElementListe(&liste[i], renderer, 255, 0, 0);
                                drawArrow(renderer, (i + 1) * 50 + i * 30 + 50, 215, (i + 1) * 50 + i * 30 + 80, 215);
                                //freeElementListe(&liste[nbrElements]);
                            }
                            else
                            {
                                liste[i].bouton.color = (SDL_Color){ 250, 10, 50, 255 };
                                renderElementListe(&liste[i], renderer, 255, 0, 0);
                                drawArrow(renderer, (i + 1) * 50 + i * 30 + 50, 215, (i + 1) * 50 + i * 30 + 80, 215);
                                int j;
                                for (j = pos; j < nbrElements - 1; j++)
                                {
                                    Element_Liste e;
                                    char t[sizeof(int) * 8 + 1];
                                    e.valeur = liste[j + 1].valeur;
                                    _itoa_s(e.valeur, t, sizeof(int) * 8 + 1, 10);
                                    initElementListe(&e.bouton, renderer, t, liste[j].border.xStart, 200, 50, 30);
                                    liste[j] = e;
                                }
                                nbrElements--;
                            }
                            SDL_RenderPresent(renderer);
                            SDL_Delay(2000);
                        }
                        strcpy(inputText1.text, "");
                        SDL_Surface* surface = TTF_RenderText_Solid(inputText1.font, inputText1.text, inputText1.color);
                        inputText1.texture = SDL_CreateTextureFromSurface(renderer, surface);
                        SDL_FreeSurface(surface);
                        inputText1.isFocused = 0;
                    }
                    else
                    {
                        int i;
                        for (i = 0; i < nbrElements; i++)
                        {
                            liste[i].bouton.color = (SDL_Color){ 10, 255, 50, 255 };
                            renderElementListe(&liste[i], renderer, 255, 0, 0);
                            drawArrow(renderer, (i + 1) * 50 + i * 30 + 50, 215, (i + 1) * 50 + i * 30 + 80, 215);
                            SDL_RenderPresent(renderer);
                            SDL_Delay(2000);
                        }
                        message(&inputText3, "L'élément n'existe pas.", 1);
                    }
                    strcpy(inputText1.text, "");
                    SDL_Surface* surface = TTF_RenderText_Solid(inputText1.font, inputText1.text, inputText1.color);
                    inputText1.texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_FreeSurface(surface);
                    inputText1.isFocused = 0;
                }
                else
                {
                    message(&inputText3, "La liste est vide, pas d'éléments à supprimer. ", 1);
                }
            }
            else
            {
                message(&inputText3, "Cliquer sur 'Supprimer' ou 'Ajouter'. ", 1);
            }
            operation_ajouter = FALSE;
            operation_supprimer = FALSE;
        }
        break;
    case SDL_KEYUP:
        isCreerPressed = 0;
        isAjouterPressed = 0;
        isSupprimerPressed = 0;
        isTrierPressed = 0;
        isOKPressed = 0;
        break;
    case SDL_MOUSEBUTTONDOWN:
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (isPointInsideButtonText(mouseX, mouseY, &creer)) {
            isCreerPressed = 1;
            if (nbrElements == 0)
            {
                strcpy(inputText2.text, "Liste : ");
                inputText2.rect.w = 100;
                SDL_Surface* surface = TTF_RenderText_Solid(inputText2.font, inputText2.text, inputText2.color);
                inputText2.texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                inputText1.isFocused = 1;
                message(&inputText3, "Donner la valeur de l'élément à ajouter : ", 0);
                operation_ajouter = TRUE;
            }
            else
            {
                message(&inputText3, "Il existe déjà une liste.", 1);
            }
        }
        if (isPointInsideButtonText(mouseX, mouseY, &ajouter)) {
            isAjouterPressed = 1;
            inputText1.isFocused = 1;
            message(&inputText3, "Donner la valeur de l'élément à ajouter : ", 0);
            operation_ajouter = TRUE;
        }
        if (isPointInsideButtonText(mouseX, mouseY, &supprimer)) {
            isSupprimerPressed = 1;
            inputText1.isFocused = 1;
            message(&inputText3, "Donner la valeur de l'élément à supprimer : ", 0);
            operation_supprimer = TRUE;
        }
        if (isPointInsideButtonText(mouseX, mouseY, &trier)) {
            isTrierPressed = 1;
            trierParInsertion(&noeud);
            int i;
            Noeud* courant = noeud;
            for (i = 0; i < nbrElements; i++)
            {
                Element_Liste e;
                char t[sizeof(int) * 8 + 1];
                _itoa_s(courant->donnee, t, sizeof(int) * 8 + 1, 10);
                initElementListe(&e, renderer, t, (i + 1) * 50 + i * 30, 200, 50, 30);
                e.valeur = courant->donnee;
                liste[i] = e;
                courant = courant->suivant;
            }
        }
        if (isPointInsideButtonText(mouseX, mouseY, &ok)) {
            isOKPressed = 1;
            if (operation_ajouter)
            {
                if (nbrElements == 0)
                {
                    noeud = creerNoeud(atoi(inputText1.text));
                    Element_Liste e;
                    initElementListe(&e, renderer, inputText1.text, 50, 200, 50, 30);
                    e.valeur = atoi(inputText1.text);
                    liste[nbrElements] = e;
                    nbrElements++;
                    strcpy(inputText1.text, "");
                    SDL_Surface* surface = TTF_RenderText_Solid(inputText1.font, inputText1.text, inputText1.color);
                    inputText1.texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_FreeSurface(surface);
                    inputText1.isFocused = 0;
                }
                else
                {
                    insererALaFin(&noeud, atoi(inputText1.text));
                    Element_Liste e;
                    initElementListe(&e.bouton, renderer, inputText1.text, (nbrElements + 1) * 50 + nbrElements * 30, 200, 50, 30);
                    e.valeur = atoi(inputText1.text);
                    liste[nbrElements] = e;
                    nbrElements++;
                    strcpy(inputText1.text, "");
                    SDL_Surface* surface = TTF_RenderText_Solid(inputText1.font, inputText1.text, inputText1.color);
                    inputText1.texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_FreeSurface(surface);
                    inputText1.isFocused = 0;
                }
            }
            else if (operation_supprimer)
            {
                if (nbrElements != 0)
                {
                    int pos = supprimerValeur(&noeud, atoi(inputText1.text));
                    if (pos != -1)
                    {
                        int i;
                        for (i = 0; i <= pos; i++)
                        {
                            if (i < pos)
                            {
                                liste[i].bouton.color = (SDL_Color){ 10, 255, 50, 255 };
                                renderElementListe(&liste[i], renderer, 255, 0, 0);
                                drawArrow(renderer, (i + 1) * 50 + i * 30 + 50, 215, (i + 1) * 50 + i * 30 + 80, 215);
                                //freeElementListe(&liste[nbrElements]);
                            }
                            else
                            {
                                liste[i].bouton.color = (SDL_Color){ 250, 10, 50, 255 };
                                renderElementListe(&liste[i], renderer, 255, 0, 0);
                                drawArrow(renderer, (i + 1) * 50 + i * 30 + 50, 215, (i + 1) * 50 + i * 30 + 80, 215);
                                int j;
                                for (j = pos; j < nbrElements - 1; j++)
                                {
                                    Element_Liste e;
                                    char t[sizeof(int) * 8 + 1];
                                    e.valeur = liste[j + 1].valeur;
                                    _itoa_s(e.valeur, t, sizeof(int) * 8 + 1, 10);
                                    initElementListe(&e.bouton, renderer, t, liste[j].border.xStart, 200, 50, 30);
                                    liste[j] = e;
                                }
                                nbrElements--;
                            }
                            SDL_RenderPresent(renderer);
                            SDL_Delay(2000);
                        }
                        strcpy(inputText1.text, "");
                        SDL_Surface* surface = TTF_RenderText_Solid(inputText1.font, inputText1.text, inputText1.color);
                        inputText1.texture = SDL_CreateTextureFromSurface(renderer, surface);
                        SDL_FreeSurface(surface);
                        inputText1.isFocused = 0;
                    }
                    else
                    {
                        int i;
                        for (i = 0; i < nbrElements; i++)
                        {
                            liste[i].bouton.color = (SDL_Color){ 10, 255, 50, 255 };
                            renderElementListe(&liste[i], renderer, 255, 0, 0);
                            drawArrow(renderer, (i + 1) * 50 + i * 30 + 50, 215, (i + 1) * 50 + i * 30 + 80, 215);
                            SDL_RenderPresent(renderer);
                            SDL_Delay(2000);
                        }
                        message(&inputText3, "L'élément n'existe pas.", 1);
                    }
                    strcpy(inputText1.text, "");
                    SDL_Surface* surface = TTF_RenderText_Solid(inputText1.font, inputText1.text, inputText1.color);
                    inputText1.texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_FreeSurface(surface);
                    inputText1.isFocused = 0;
                }
                else
                {
                    message(&inputText3, "La liste est vide, pas d'éléments à supprimer. ", 1);
                }
            }
            else
            {
                message(&inputText3, "Cliquer sur 'Supprimer' ou 'Ajouter'. ", 1);
            }
            operation_ajouter = FALSE;
            operation_supprimer = FALSE;
        }
        break;
    }
    case SDL_MOUSEBUTTONUP:
        isCreerPressed = 0;
        isAjouterPressed = 0;
        isSupprimerPressed = 0;
        isTrierPressed = 0;
        isOKPressed = 0;
        break;
    default:
        break;
    }
    handleInputText(&inputText1, &event);
}void mise_a_jour(void)
{
    int time_to_sleep = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    if (time_to_sleep > 0 && time_to_sleep <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_sleep);
    }
    //while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));

    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks();
}

void rendu(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_Rect menu = { 0, 0, 800, 30 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &menu);

    initButton(&creer, renderer, "Créer (C)", 0, 0, 60, 30);

    if (isCreerPressed) {
        creer.color = (SDL_Color){ 0, 0, 255, 255 };
    }
    else {
        creer.color = (SDL_Color){ 0, 0, 0, 255 };
    }

    renderButtonText(&creer, renderer);

    initButton(&ajouter, renderer, "Ajouter (A)", 75, 0, 60, 30);

    if (isAjouterPressed) {
        ajouter.color = (SDL_Color){ 0, 0, 255, 255 };
    }
    else {
        ajouter.color = (SDL_Color){ 0, 0, 0, 255 };
    }

    renderButtonText(&ajouter, renderer);

    initButton(&supprimer, renderer, "Supprimer (S)", 150, 0, 70, 30);

    if (isSupprimerPressed) {
        supprimer.color = (SDL_Color){ 0, 0, 255, 255 };
    }
    else {
        supprimer.color = (SDL_Color){ 0, 0, 0, 255 };
    }

    renderButtonText(&supprimer, renderer);

    initButton(&trier, renderer, "Trier (T)", 235, 0, 60, 30);

    if (isTrierPressed) {
        trier.color = (SDL_Color){ 0, 0, 255, 255 };
    }
    else {
        trier.color = (SDL_Color){ 0, 0, 0, 255 };
    }

    renderButtonText(&trier, renderer);

    initButton(&ok, renderer, "OK", 750, 570, 50, 30);

    if (isOKPressed) {
        ok.color = (SDL_Color){ 0, 0, 255, 255 };
    }
    else {
        ok.color = (SDL_Color){ 0, 0, 0, 255 };
    }

    renderButtonText(&ok, renderer);

    renderInputText(&inputText1, renderer, 150, 150, 150);
    renderInputText(&inputText2, renderer, 255, 255, 255);
    renderInputText(&inputText3, renderer, 255, 255, 255);

    int i;
    for (i = 0; i < nbrElements; i++)
    {
        if (i < (nbrElements - 1))
        {
            liste[i].bouton.color = (SDL_Color){ 255, 255, 255, 255 };
            renderElementListe(&liste[i], renderer, 0, 0, 0);
            drawArrow(renderer, (i + 1) * 50 + i * 30 + 50, 215, (i + 1) * 50 + i * 30 + 80, 215);
        }
        else
        {
            liste[i].bouton.color = (SDL_Color){ 255, 255, 255, 255 };
            renderElementListe(&liste[i], renderer, 0, 0, 0);
            drawArrow(renderer, (i + 1) * 50 + i * 30 + 50, 215, (i + 1) * 50 + i * 30 + 80, 215);
            inputText4.rect.x = (i + 1) * 50 + i * 30 + 85;
            inputText4.rect.y = 200;
            renderInputText(&inputText4, renderer, 255, 255, 255);
        }
    }
    SDL_RenderPresent(renderer);
}

void destruction_fenetre(void)
{
    TTF_CloseFont(font);
    TTF_Quit();
    freeInputText(&inputText1);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Fonction principale
int main() {

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
    Noeud* elementRecherche = rechercherElement(tete, valeurRecherchee);
    if (elementRecherche != NULL) {
        printf("Element %d trouve dans la liste.\n", valeurRecherchee);
    }
    else {
        printf("Element %d non trouve dans la liste.\n", valeurRecherchee);
    }

    // Tri par insertion
    trierParInsertion(&tete);

    // Affichage après le tri
    printf("Liste après le tri par insertion : ");
    afficherListe(tete);

    // Suppression au début
    supprimerAuDebut(&tete);

    // Suppression à la fin
    supprimerALaFin(&tete);

    // Affichage après les suppressions
    printf("Liste après les suppressions : ");
    afficherListe(tete);
    */

    return 0;
}

