#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/combat.h"
#include "../lib/joueur.h"
#include "../lib/initSDL.h"
#include "../lib/sauv.h"


int main() {
    game_t game;

    if (!initGame(&game)) {            //initialisation sdl
        printf("Échec de l'initialisation du jeu.\n");
        return -1;
    }

    //pour que le fond s'adapte à l'ecran
    //ici
    SDL_Rect dms_win;
    SDL_GetRendererOutputSize(game.renderer, &dms_win.w, &dms_win.h);       //dimensions ecran

    int img_w, img_h;
    SDL_QueryTexture(game.backgroundTexture, NULL, NULL, &img_w, &img_h);       //dimensions img

    float img_ratio = (float)img_w / (float)img_h;
    float win_ratio = (float)dms_win.w / (float)dms_win.h;

    if (win_ratio > img_ratio) {
        // L'écran est plus large que l'image
        int new_w = dms_win.h * img_ratio;
        dms_win.x = (dms_win.w - new_w) / 2; // centrage horizontal
        dms_win.w = new_w;
        dms_win.y = 0;
    } else {
        // L'écran est plus haut que l'image
        int new_h = dms_win.w / img_ratio;
        dms_win.y = (dms_win.h - new_h) / 2; // centrage vertical
        dms_win.h = new_h;
        dms_win.x = 0;
    }
    //a ici


    player_t player;
    float scale_x = (float)dms_win.w/img_w; 
    float scale_y = (float)dms_win.h/img_h; 

    float scale = (scale_x < scale_y) ? scale_x : scale_y; //prend l'echelle la plus petite
    SDL_Rect hitbox_player = init_player(&player, scale);   //dessinne le joueur propor à l'image et l'ecran
    int running = 1;
    SDL_Event event;


    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {       //si on appuie sur fermer la fenetre running = 0
                running = 0;
            }
        }
        SDL_RenderClear(game.renderer);     //efface l'ecran

        SDL_RenderCopy(game.renderer,game.backgroundTexture , NULL, &dms_win);                 // Dessiner le background
        draw_player(game, hitbox_player);           //dessine le joueur

        SDL_RenderPresent(game.renderer);      //affiche rendu
    }
    cleanUp(&game);

    return 0;

}