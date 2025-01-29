#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/combat.h"
#include "../lib/player.h"
#include "../lib/initGame.h"
#include "../lib/sauv.h"


int main() {
    game_t game;

    if (!init_game(&game)) {            //initialisation sdl
        printf("Échec de l'initialisation du jeu.\n");
        return -1;
    }        

    char background[100] = "img/background/backgroundtest1.png";
    if(!init_background(background, &game)) {
        return -1;
    }

    int ** mat = NULL;
    if(!init_mat(game, &mat)) {
        printf("Echec init mat\n");
        return -1;
    }

    SDL_Rect hitbox_player = create_obj(game, 32, 32, 0, 0);
    int running = 1;
    SDL_Event event;


    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {       //si on appuie sur fermer la fenetre running = 0
                running = 0;
            }
        }
        SDL_RenderClear(game.renderer);     //efface l'ecran

        draw_background(game);
        draw_obj(game, hitbox_player);           //dessine le joueur

        SDL_RenderPresent(game.renderer);      //affiche rendu
    }
    cleanUp(&game);
    for (int i = 0; i < game.img_h / 32; i++) {
        free(mat[i]); 
    }
    free(mat); 

    combat_init();

    return 0;

}