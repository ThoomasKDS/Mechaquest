#ifndef AFF_H
#define AFF_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include "../lib/player.h"
#include "../lib/initGame.h"
#include "../lib/sauv.h"


#define PX 32

//COLLISION
#define RIEN 0
#define JOUEUR 1
#define BARRIERE 2
#define BAT 3
#define DECHET 5

//INTERACTION
#define PNJ 6
#define PC 7
#define PORTE 4

//CHAGEMENT MAP

#define TPMAP1 -11
#define TPMAP2 -12
#define TPMAP3 -13
#define TPMAP4 -14
#define TPMAP5 -15
#define TPMAP6 -16

//ZONE SPAWN MECHA
#define Z1 -1
#define Z2 -2
#define Z3 -3
#define Z4 -4
#define Z5 -5
#define Z6 -6
#define Z7 -7
#define Z8 -8
#define Z9 -9
#define Z10 -10 




typedef struct{
    SDL_Texture * gauche[4];
    SDL_Texture * droite[4];
    SDL_Texture * haut[4];
    SDL_Texture * bas[4];

}img_player_t;

void draw_obj(game_t *game, SDL_Rect *obj, SDL_Texture * img );
void draw_player(game_t *game, SDL_Rect *obj, img_player_t * sprite_playerH, joueur_t * j);
SDL_Rect create_obj(game_t * game, int taille_w, int taille_h, int x, int y, int type_obj, int n_mat);
int init_background(game_t * game);
void draw_background(game_t * game);
int init_mat(game_t *game, int taille_x, int taille_y);
int remplir_mat(game_t * game, int taille_x, int taille_y);
void aff_mat(game_t * game, int taille_x, int taille_y, int n_mat);
void free_mat(game_t *game, int taille_x, int taille_y);
int init_player_h(game_t * game, img_player_t * sprite_playerH);

#endif