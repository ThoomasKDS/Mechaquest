#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/combat.h"
#include "../lib/joueur.h"
#include "../lib/initSDL.h"
#include "../lib/sauv.h"


int main() {
    Game game;

    if (!initGame(&game)) {
        printf("Échec de l'initialisation du jeu.\n");
        return 1;
    }
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_RenderClear(game.renderer);
        SDL_RenderPresent(game.renderer);
    }
    cleanUp(&game);

    return 0;

}