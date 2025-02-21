#ifndef MENU_H
#define MENU_H

#include "sauv.h"
#include "../lib/initGame.h"
#include "../lib/affichage.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>

#define LARGEUR_BOUTON 200
#define HAUTEUR_BOUTON 50
#define LONGUEUR_MAX_PSEUDO 50

typedef struct {
    int volume;
} parametre_t;

typedef struct {
    SDL_Rect rect;
    SDL_Color couleur;
    const char* texte;
} Bouton;

extern SDL_Color rouge;
extern SDL_Color vert;
extern SDL_Color bleu;
extern SDL_Color noir;

Bouton creerBouton(int x, int y, int largeur, int hauteur, SDL_Color couleur, const char* texte);

void afficherBouton(game_t* game, Bouton bouton);

int afficherSaisiePseudo(game_t* game, char* pseudo);

void afficherParametres(game_t* game, parametre_t* parametres);

void afficherMenu(game_t* game, parametre_t* parametres, char* pseudo);

#endif
