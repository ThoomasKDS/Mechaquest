#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>


#include "../lib/initGame.h"



int initGame(game_t* game) {

    SDL_Rect display_bounds; //struture contenant dimensions de l'ecran
    int display_index = 0; //choix de l'ecran 0 pour le principal etc

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL n'a pas pu être initialisé.\n");
        return 0;
    }

    // Initialiser SDL_image pour charger des images
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Erreur d'initialisation de SDL_image.\n");
        SDL_Quit();
        return 0;
    }

    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG)) {
    printf("Erreur d'initialisation de SDL_mixer: %s\n", Mix_GetError());
    SDL_Quit();
    return 0;
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) == -1) {
        printf("Erreur d'ouverture du périphérique audio: %s\n", Mix_GetError());
        Mix_Quit();
        SDL_Quit();
        return 0;
    }

    if (SDL_GetDisplayBounds(display_index, &display_bounds) != 0) {
        printf("Erreur lors de l'obtention des dimensions de l'écran.\n");
        SDL_Quit();
        return 0;
    }

    // Création de la fenêtre
    game->window = SDL_CreateWindow(
            "Mechaquest",
            display_bounds.x,
            display_bounds.y,
            L,
            H,
            SDL_WINDOW_SHOWN
    );
    if (!game->window) {
        printf("Erreur de création de la fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    if (SDL_SetWindowFullscreen(game->window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) {     //pleine ecran
        printf("Erreur lors du passage en plein écran : %s\n", SDL_GetError());
    }

    // Création du renderer
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (!game->renderer) {
        printf("Erreur de création du renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        return 0;
    }

    //charger fond

    SDL_Surface* imageSurface = IMG_Load("../img/background/backgroundtest1.png");
    if (!imageSurface) {
        printf("Erreur de chargement de l'image : %s\n", IMG_GetError());
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Convertir la surface en texture
    //Laisser en commentaire tant que aucune image
    
    game->backgroundTexture = SDL_CreateTextureFromSurface(game->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!game->backgroundTexture) {
        printf("Erreur de création de la texture : %s\n", SDL_GetError());
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        return 0;
    }
    

    return 1;
}

// Libération des ressources et fermeture SDL
void cleanUp(game_t* game) {
    SDL_DestroyTexture(game->backgroundTexture);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    IMG_Quit();
    SDL_Quit();
}