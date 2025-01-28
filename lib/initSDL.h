#ifndef INIT_SDL_H
#define INIT_SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

// Structure qui contient les initialisation de rendu du jeu 
typedef struct games {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
} game_t;

int initGame(game_t* game); //initialiser SDL
void cleanUp(game_t* game); //quitter proprement SDL


#endif // INIT_SDL_H