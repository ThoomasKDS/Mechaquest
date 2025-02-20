#ifndef INIT_SDL_H
#define INIT_SDL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define H 768
#define L 1280

// Structure qui contient les initialisation de rendu du jeu 
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture[6];
    TTF_Font* police;
    int *** mat ;
    float scale;                        //echelle qui adapte l'affichage à l'ecran et à la taille de l'image
    SDL_Rect dms_win;
    int img_w;
    int img_h;
    int mat_active;
} game_t;


//Initialisation de sdl 
//Initialisation de sdl mixer
//Initialisation de sdl image
int init_game(game_t* game);
// Libération des ressources et fermeture SDL
void cleanUp(game_t* game); 


#endif
