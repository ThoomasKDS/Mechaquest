#ifndef MENU_H
#define MENU_H

#include "sauv.h"
#include "../lib/initGame.h"
#include "../lib/affichage.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
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
    SDL_Texture *image;
} BoutonImage;

SDL_Texture* chargerTexture(const char *chemin, game_t *game);

BoutonImage creerBoutonImage(int x, int y, int largeur, int hauteur, char *cheminImage, game_t* game);

int afficherChoixSexe(game_t* game, joueur_t* j, char* pseudo);

int afficherChoixSuppression(game_t* game, joueur_t* j,char* pseudo);

int afficherSaisiePseudo(game_t* game, joueur_t* joueur, char* pseudo);

void afficherParametres(game_t* game, parametre_t* parametres);

void afficherMenu(game_t* game, parametre_t* parametres, joueur_t* j, char* pseudo);

int afficherMenuPause(game_t* game, parametre_t* parametres);

#endif
