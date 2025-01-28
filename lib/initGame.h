#ifndef INIT_SDL_H
#define INIT_SDL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#define H 768
#define L 1280

// Structure qui contient les initialisation de rendu du jeu 
typedef struct games {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    SDL_Rect dms_win;
    int img_w;
    int img_h;

} game_t;

int init_game(game_t* game); //initialiser SDL
void cleanUp(game_t* game); //quitter proprement SDL
int init_background(char * img, game_t * game);// initialise le background
void draw_background(game_t game);
int init_mat(game_t game, int *** mat);

#endif // INIT_SDL_H