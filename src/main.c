#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/combat.h"
#include "../lib/player.h"
#include "../lib/initGame.h"
#include "../lib/sauv.h"
#include "../lib/affichage.h"

const int FPS_LIMIT = 60;
const int FRAME_DELAY = 1000 / FPS_LIMIT; // Temps entre chaque frame (16 ms)

int main() {
    game_t game;

    if (!init_game(&game)) {            //initialisation sdl
        printf("Échec de l'initialisation du jeu.\n");
        return -1;
    }


    char background[100] = "img/background/backgroundtest1.png";
    init_background(background, &game);

    case_t ** mat = NULL;
    int taille_x_mat = game.img_w/PX;
    int taille_y_mat = game.img_h/PX;
    if(!init_mat(&mat, taille_x_mat, taille_y_mat)) {
        printf("Echec init mat\n");
        return -1;
    }
    Joueur j;
    j.x = 0;
    j.y = 0;
    remplir_mat(mat, taille_x_mat, taille_y_mat);
    SDL_Rect hitbox_player = create_obj(game, PX, PX, mat[j.y][j.x].x, mat[j.y][j.x].y, mat, JOUEUR); //position initialisé manuellement pour l'instant
    aff_mat(mat, taille_x_mat, taille_y_mat);
    int running = 1;
    SDL_Event event;
    Uint32 frameStart;  
    int frameTime;

    const Uint8 *keys = SDL_GetKeyboardState(NULL); //etat touche

    while (running) {
        frameStart = SDL_GetTicks();    //obtien heure

        SDL_PumpEvents();  // Met à jour l'état des événements (telles que les touches pressées)
        keys = SDL_GetKeyboardState(NULL);  // Met à jour l'état des touches

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {       //si on appuie sur fermer la fenetre running = 0
                running = 0;
            }
            if(event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) 
                     running = 0;
            }
            

        }

        deplacement(game, mat, &hitbox_player,taille_x_mat, taille_y_mat, keys, &j);
        SDL_RenderClear(game.renderer);     //efface l'ecran

        draw_background(game);
        draw_obj(game, hitbox_player);           //dessine le joueur

        SDL_RenderPresent(game.renderer);      //affiche rendu
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }


    cleanUp(&game);
    for (int i = 0; i < game.img_h / PX; i++) {
        free(mat[i]); 
    }
    free(mat); 

    return 0;

}