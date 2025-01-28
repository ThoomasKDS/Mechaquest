#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

// Structure qui contient les initialisation de rendu du jeu 
typedef struct Game {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
} Game;

int initGame(Game* game); //initialiser SDL
void cleanUp(Game* game); //quitter proprement SDL
