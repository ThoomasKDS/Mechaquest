/**
 * \file main.c
 * \brief fichier contenant la fonctions principale
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/combat.h"
#include "../lib/player.h"
#include "../lib/initGame.h"
#include "../lib/sauv.h"
#include "../lib/affichage.h"
#include "../lib/pointDePassage.h"
#include "../lib/menu.h"

mechas_t mecha[24];
attaque_t attaque[64];
zone_t zone[10];
pnj_t pnj[24];

const int FPS_LIMIT = 60;
const int FRAME_DELAY = 1000 / FPS_LIMIT; // Temps entre chaque frame (16 ms)


int main() {
    //VARIABLES UTILES AU PROGRAMME

    game_t game;
    img_player_t sprite_playerH;
    img_pnj_t sprite_pnj;
    joueur_t j;
    parametre_t parametres;
    mechas_joueur_t mecha_sauvage;
    parametres.volume = 50;
    char pseudo[50] = "";
    int premier_tour = 0;
    int obj_case;
    int running = 1;
    int last_case = 0;

    Uint32 frameStart;  
    int frameTime;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    recuperation_mechas(mecha);
    recuperation_attaques(attaque);
    recuperation_zone(zone);
    
    //INITIALISATION SDL    
    if (!init_game(&game)) {            
        printf("Échec de l'initialisation du jeu.\n");
        return -1;
    }        

    //INITIALISATION BACKGROUND
    if(!init_background(&game)) {
        return -1;
    }

    //Affichage du menu
    afficherMenu(&game,&parametres,&j,pseudo);
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    if(strlen(pseudo) < 1){
        return -1;
    }
    game.mat_active = j.numMap;
    //TAILLE DE LA MATRICE
    int taille_x_mat = game.img_w/PX;
    int taille_y_mat = game.img_h/PX;
    frameStart = SDL_GetTicks();    //obtien heure

    SDL_PumpEvents();  // Met à jour l'état des événements (telles que les touches pressées)
    keys = SDL_GetKeyboardState(NULL);  // Met à jour l'état des touches

    //ALLOUE MEMOIRE POUR MATRICE
    if(!init_mat(&game, taille_x_mat, taille_y_mat)) {
        printf("Echec init mat\n");
        return -1;
    }
  
    //REMPLI LA MATRICE DE 0
    remplir_mat(&game, taille_x_mat, taille_y_mat);
    aff_mat(&game, taille_x_mat, taille_y_mat, 5);
    //INITIALISE LES MOUVEMENTS DU JOUEUR ET COORS
    j.moving = 0;
    j.derniere_touche = 4;
    j.proba_combat = 0;
    j.screen_x = (float)(game.dms_win.x + (j.x * PX * game.scale));      //position du joueur en px
    j.screen_y = (float)(game.dms_win.y + (j.y * PX * game.scale));
    if(!init_player_h(&game, &sprite_playerH)){
        return -1;
    }
    if(!init_pnj(&game, &sprite_pnj)){
        
        return -1;
    }
    
    recuperation_pnj(pnj,j.pseudo);
    //SPRITE JOUEUR
    SDL_Rect sprite_p = create_obj(&game, PX, 48, j.x*PX, j.y * PX - 24, JOUEUR, 1);
    SDL_Rect pnj_sprite[24];
    for(int i =0; i < 24;i++){
        pnj_sprite[i] = create_obj(&game, PX, 48, (pnj[i].x)*PX, (pnj[i].y) * PX - 24, PNJ, pnj[i].id_map - 1);

    }
    if(j.pointSauvegarde > 1){
            game.mat[2][8][0] = -16;
            game.mat[2][9][0] = -16;
            game.mat[2][10][0] = -16;
            game.mat[2][9][19] = 0;
    }
    while(running){
        
        frameStart = SDL_GetTicks(); 
        if(j.pointSauvegarde == 0){
            if(premier_tour == 0){
                game.mat[0][0][15] = 2;
                game.mat[0][0][16] = 2;
                game.mat[0][0][17] = 2;
                premier_tour++;
            }
            running = parler_a_vin_gazole(&game,&sprite_playerH,&j,&sprite_p,&parametres);
            if(j.pointSauvegarde == 1){
                premier_tour = 0;
            }
        }        
        if(j.pointSauvegarde == 1){
            if(premier_tour == 0){
                game.mat[2][0][4] = 2;
                game.mat[2][0][5] = 2;
                game.mat[2][0][6] = 2;
                premier_tour++;
            }
            running = premier_combat_musk(&game,&sprite_playerH,&j,&sprite_p,&parametres);
            if(j.pointSauvegarde == 2){
                premier_tour = 0;
                running = 0;
            }
        }
        
        if(j.pointSauvegarde == 2){
            if(premier_tour == 0){
                game.mat[0][0][15] = 2;
                game.mat[0][0][16] = 2;
                game.mat[0][0][17] = 2;
                premier_tour++;
            }
            running = retourner_parler_a_vin_gazole(&game,&sprite_playerH,&j,&sprite_p,&parametres);
        }         
        if(j.pointSauvegarde == 3){
            running = combat_final(&game,&sprite_playerH,&j,&sprite_p,&parametres);
        }
        if(j.pointSauvegarde == 4)
            running = jeu_libre(&game,&sprite_playerH,&j,&sprite_p,&parametres);

        obj_case = deplacement(&game,taille_x_mat, taille_y_mat, keys, &j, &last_case, &sprite_p);
        /*if(spawn_mecha(&j, obj_case, zone, mecha, &mecha_sauvage)) {
            printf("oui");
            combat_sauvage(&j, &mecha_sauvage, &game);
        }*/
        animation(&j, &sprite_p);

        SDL_RenderClear(game.renderer);     //efface l'ecran

        draw_all(&game,&j,&sprite_p,pnj_sprite,&sprite_pnj,&sprite_playerH);


        SDL_RenderPresent(game.renderer);      //affiche rendu
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }
    sauvegarde_partie(&j,pseudo);
    cleanUp(&game);
    free_mat(&game,taille_x_mat, taille_y_mat);
    destruction_joueur(&j);
    for(int i = 0;i<24;i++){
        destruction_pnj(&pnj[i]);
    }
    return 0;

}
