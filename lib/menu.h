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



SDL_Texture* charger_texture(const char *chemin, game_t *game);

int afficher_choix_sexe(game_t* game, joueur_t* j, char* pseudo);

int afficher_choix_suppression(game_t* game, joueur_t* j,char* pseudo);

int aff_saisie_pseudo(game_t* game, joueur_t* j, char* pseudo);

void afficher_reglage(game_t* game, parametre_t* parametres);

void afficher_menu(game_t* game, parametre_t* parametres, joueur_t* j, char* pseudo);

int afficher_menu_pause(game_t* game, parametre_t* parametres);

void afficher_informations(game_t* game);

void aff_parametre(game_t* game, parametre_t* parametres);

int afficher_dialogue(game_t *game, joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, img_pnj_t * sprite_pnj, img_player_t * sprite_playerH, char *pseudo, char *dialogue,int choix);

#endif
