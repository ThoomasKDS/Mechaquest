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
const char background[100] = "img/background/backgroundtest1.png";

int main() {
    //VARIABLES UTILES AU PROGRAMME
    game_t game;
    case_t ** mat = NULL;
    img_player_t sprite_playerH;
    joueur_t j;
    int running = 1;
    SDL_Event event;
    Uint32 frameStart;  
    int frameTime;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    

    //INITIALISATION SDL    
    if (!init_game(&game)) {            
        printf("Échec de l'initialisation du jeu.\n");
        return -1;
    }        

    //INITIALISATION BACKGROUND
    if(!init_background(background, &game)) {
        return -1;
    }

    //TAILLE DE LA MATRICE
    int taille_x_mat = game.img_w/PX;
    int taille_y_mat = game.img_h/PX;

    //ALLOUE MEMOIRE POUR MATRICE
    if(!init_mat(&mat, taille_x_mat, taille_y_mat)) {
        printf("Echec init mat\n");
        return -1;
    }

    //REMPLI LA MATRICE DE 0
    remplir_mat(mat, taille_x_mat, taille_y_mat);

    //INITIALISE LES MOUVEMENTS DU JOUEUR ET COORS
    j.derniere_touche = 1;
    j.x = 0;
    j.y = 1;
    j.moving = 0;
    j.derniere_touche = 3;
    mat[1][0].obj = JOUEUR;
    j.screen_x = (float)(game.dms_win.x + (mat[j.y][j.x].x * game.scale));      //position du joueur en px
    j.screen_y = (float)(game.dms_win.y + (mat[j.y][j.x].y * game.scale));
    if(!init_player_h(&game, &sprite_playerH)){
        return -1;
    }

    //SPRITE JOUEUR
    SDL_Rect sprite_p = create_player(&game, PX, 48, mat[j.y][j.x].x, mat[j.y][j.x].y - 24, mat);

    
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
        deplacement(&game, mat,taille_x_mat, taille_y_mat, keys, &j);
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
    for (int i = 0; i < game.img_h / PX; i++) {
        free(mat[i]); 
    }
    free(mat); 

    //combat_init();

    return 0;

}