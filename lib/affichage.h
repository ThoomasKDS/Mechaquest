#ifndef AFF_H
#define AFF_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include "../lib/player.h"
#include "../lib/initGame.h"
#include "../lib/sauv.h"


#define PX 32
#define RIEN 0
#define JOUEUR 1
#define OBSTACLE 2
#define PNG 3



typedef struct{
    SDL_Texture * gauche[4];
    SDL_Texture * droite[4];
    SDL_Texture * haut[4];
    SDL_Texture * bas[4];

}img_player_t;

void draw_obj(game_t *game, SDL_Rect *obj, SDL_Texture * img );
void draw_player(game_t *game, SDL_Rect *obj, img_player_t * sprite_playerH, joueur_t * j);
SDL_Rect create_obj(game_t * game, int taille_w, int taille_h, int x, int y, int** mat, int type_obj);
int init_background(const char *img, game_t * game);
void draw_background(game_t * game);
int init_mat(int*** mat, int taille_x, int taille_y);
void remplir_mat(int** mat, int taille_x, int taille_y);
void aff_mat(int** mat, int taille_x, int taille_y);
int init_player_h(game_t * game, img_player_t * sprite_playerH);

#endif