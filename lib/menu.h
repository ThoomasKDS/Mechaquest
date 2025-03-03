#ifndef MENU_H
#define MENU_H

#include "sauv.h"
#include "../lib/initGame.h"
#include "../lib/affichage.h"
#include "../lib/global.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>



SDL_Texture* chargerTexture(const char *chemin, game_t *game);

BoutonImage creerBoutonImage(int x, int y, int largeur, int hauteur, char *cheminImage, game_t* game);

int afficherChoixSexe(game_t* game, joueur_t* j, char* pseudo);

int afficherChoixSuppression(game_t* game, joueur_t* j,char* pseudo);

int afficherSaisiePseudo(game_t* game, joueur_t* joueur, char* pseudo);

void afficherReglage(game_t* game, parametre_t* parametres);

void afficherMenu(game_t* game, parametre_t* parametres, joueur_t* j, char* pseudo);

int afficherMenuPause(game_t* game, parametre_t* parametres);

void afficherInformations(game_t* game);

void afficherReglage(game_t* game, parametre_t* parametres) ;

#endif
