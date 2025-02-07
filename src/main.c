#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/combat.h"
#include "../lib/player.h"
#include "../lib/initGame.h"
#include "../lib/sauv.h"
#include "../lib/affichage.h"


//CONSTANTE
const int FPS_LIMIT = 60;
const int FRAME_DELAY = 1000 / FPS_LIMIT; // Temps entre chaque frame (16 ms)


int main() {
    //VARIABLES UTILES AU PROGRAMME
    game_t game;
    img_player_t sprite_playerH;
    joueur_t j;
    int running = 1;
    SDL_Event event;
    Uint32 frameStart;  
    int frameTime;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    int last_case = RIEN;

    game.mat_active = 5;


    //INITIALISATION SDL    
    if (!init_game(&game)) {            
        printf("Échec de l'initialisation du jeu.\n");
        return -1;
    }        

    //INITIALISATION BACKGROUND
    if(!init_background(&game)) {
        return -1;
    }

    //TAILLE DE LA MATRICE
    int taille_x_mat = game.img_w/PX;
    int taille_y_mat = game.img_h/PX;

    //ALLOUE MEMOIRE POUR MATRICE
    if(!init_mat(&game, taille_x_mat, taille_y_mat)) {
        printf("Echec init mat\n");
        return -1;
    }

    //REMPLI LA MATRICE DE 0
    remplir_mat(&game, taille_x_mat, taille_y_mat);
    aff_mat(&game, taille_x_mat, taille_y_mat, 5);
    //INITIALISE LES MOUVEMENTS DU JOUEUR ET COORS
    j.derniere_touche = 1;
    j.x = 17;
    j.y = 2;
    j.moving = 0;
    j.derniere_touche = 4;
    j.screen_x = (float)(game.dms_win.x + (j.x * PX * game.scale));      //position du joueur en px
    j.screen_y = (float)(game.dms_win.y + (j.y * PX * game.scale));
    if(!init_player_h(&game, &sprite_playerH)){
        return -1;
    }

    //SPRITE JOUEUR
    SDL_Rect sprite_p = create_obj(&game, PX, 48, j.x*PX, j.y * PX - 24, JOUEUR, 1);

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

        //deplacement du joueur 
        deplacement(&game,taille_x_mat, taille_y_mat, keys, &j, &last_case, &sprite_p);
        animation(&j, &sprite_p);

        SDL_RenderClear(game.renderer);     //efface l'ecran

        draw_background(&game);
        draw_player(&game, &sprite_p, &sprite_playerH, &j);           //dessine le joueur
    

        SDL_RenderPresent(game.renderer);      //affiche rendu
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }


    cleanUp(&game);
    free_mat(&game,taille_x_mat, taille_y_mat);


    return 0;

}