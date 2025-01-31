#ifndef AFF_H
#define AFF_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../lib/initGame.h"

#define PX 32
#define RIEN 0
#define JOUEUR 1

typedef struct {
    int x;
    int y;
    int obj;
} case_t;

void draw_obj(game_t * game, SDL_Rect * obj);
SDL_Rect create_obj(game_t * game, int taille_w, int taille_h, int x, int y, case_t ** mat, int type_obj);
int init_background(const char *img, game_t * game);
void draw_background(game_t * game);
int init_mat(case_t *** mat, int taille_x, int taille_y);
void remplir_mat(case_t ** mat, int taille_x, int taille_y) ;
void aff_mat(case_t ** mat, int taille_x, int taille_y) ;

#endif