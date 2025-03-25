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
#include "../lib/global.h"
#include "../lib/son.h"

mechas_t mecha[NB_PNJ];
attaque_t attaque[NB_ATTAQUES];
zone_t zone[NB_ZONES];
pnj_t pnj[NB_PNJ];
game_t game;


const int FPS_LIMIT = 60;
const int FRAME_DELAY = 1000 / FPS_LIMIT; // Temps entre chaque frame (16 ms)


int main() {
    //VARIABLES UTILES AU PROGRAMME

    
    joueur_t j;
    parametre_t parametres;
    mechas_joueur_t mecha_sauvage;
    parametres.volume = 50;
    char pseudo[LONGUEUR_MAX_PSEUDO] ="";
    int premier_tour = 0;
    int obj_case;
    int running = 1,jeux = 1;
    int last_case = 0;
    SDL_Event event;
    Uint32 frameStart;  
    int frameTime;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    //INITIALISATION SDL    
    if (!init_game()) {            
        printf("Échec de l'initialisation du jeu.\n");
        return -1;
    }        

    //INITIALISATION BACKGROUND
    if(!init_background()) {
        return -1;
    }
    if(!init_calque()) {
        return -1;
    }

    while(jeux){
        pseudo[0] = '\0';
        running = 1;

        regler_volume(&parametres);
        lancer_musique("son/musique_general.wav");

        recuperation_mechas(mecha);
        recuperation_attaques(attaque);
        recuperation_zone(zone);
        
        game.mat_active = 0;
        //Affichage du menu
        afficher_menu(&parametres,&j,pseudo);
        if(pseudo[0] == '\0'){
            jeux = 0;
            running = 0;
        }
        else{
            SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
            if(strlen(pseudo) < 1){
                return -1;
            }
            game.mat_active = j.numMap;
            //TAILLE DE LA MATRICE
            int taille_x_mat = game.img_w/PX;
            int taille_y_mat = game.img_h/PX;
            frameStart = SDL_GetTicks();    //obtien heure

            //ALLOUE MEMOIRE POUR MATRICE
            if(!init_mat(taille_x_mat, taille_y_mat)) {
                printf("Echec init mat\n");
                return -1;
            }

        
            //REMPLI LA MATRICE DE 0 DU JOUEUR ET COORS
            remplir_mat(taille_x_mat, taille_y_mat);

            //INITIALISE LES MOUVEMENT DU JOUEUR 
            j.moving = 0;
            j.derniere_touche = 4;
            j.proba_combat = 0;
            j.screen_x = (float)(game.dms_win.x + (j.x * PX * game.scale));      //position du joueur en px
            j.screen_y = (float)(game.dms_win.y + (j.y * PX * game.scale));
            if(!init_player(j.sexe)){
                return -1;
            }
            if(!init_pnj()){
                
                return -1;
            }
            
            recuperation_pnj(pnj,j.pseudo);
            //SPRITE JOUEUR
            SDL_Rect sprite_p = create_obj(PX, 48, j.x*PX, j.y * PX - 24, JOUEUR, 1);
            SDL_Rect pnj_sprite[NB_PNJ];
            for(int i =0; i < NB_PNJ;i++){
                pnj_sprite[i] = create_obj(PX, 48, (pnj[i].x)*PX, (pnj[i].y) * PX - 24, PNJ, pnj[i].id_map - 1);

            }
            if(j.pointSauvegarde > 1){
                    game.mat[2][8][0] = TPMAP6;
                    game.mat[2][9][0] = TPMAP6;
                    game.mat[2][10][0] = TPMAP6;
                    game.mat[2][9][19] = 0;
                    
            }
            while(running){
                frameStart = SDL_GetTicks(); //obtien l'heure


                SDL_PumpEvents();  // Met à jour l'état des événements (telles que les touches pressées)
                keys = SDL_GetKeyboardState(NULL);  // Met à jour l'état des touches

            
                while (SDL_PollEvent(&event)) {
                    if(event.type == SDL_KEYDOWN) {
                        if (event.key.keysym.sym == SDLK_ESCAPE){
                            if(!afficher_menu_pause(&parametres)) running = 0;
                        } 
                    }
                }
                if(running){
                    if(j.pointSauvegarde == 0){
                        if(premier_tour == 0){
                            game.mat[0][0][15] = BARRIERE;
                            game.mat[0][0][16] = BARRIERE;
                            game.mat[0][0][17] = BARRIERE;
                            premier_tour++;
                        }
                        parler_a_vin_gazole(&j, &sprite_p, pnj_sprite,keys);
                        if(j.pointSauvegarde == 1){
                            premier_tour = 0;
                        }
                    }        
                    if(j.pointSauvegarde == 1){
                        if(premier_tour == 0){
                            game.mat[2][0][4] = BARRIERE;
                            game.mat[2][0][5] = BARRIERE;
                            game.mat[2][0][6] = BARRIERE;
                            premier_tour++;
                        }
                        premier_combat_musk(&j, &sprite_p, pnj_sprite,keys);
                        if(j.pointSauvegarde == 2){
                            premier_tour = 0;
                        }
                    }
                    
                    if(j.pointSauvegarde == 2){
                        if(premier_tour == 0){
                            game.mat[0][0][15] = BARRIERE;
                            game.mat[0][0][16] = BARRIERE;
                            game.mat[0][0][17] = BARRIERE;
                            premier_tour++;
                        }
                        retourner_parler_a_vin_gazole(&j, &sprite_p, pnj_sprite,keys);
                        
                    }         
                    if(j.pointSauvegarde == 3){
                        combat_final(&j, &sprite_p, pnj_sprite,keys);
                    }
                    if(j.pointSauvegarde == 4)
                        jeu_libre(&j, &sprite_p, pnj_sprite,keys);
                    obj_case = deplacement(taille_x_mat, taille_y_mat, keys, &j, &last_case, &sprite_p);

                    if(spawn_mecha(&j, obj_case,&mecha_sauvage)) {
                        combat_sauvage(&j, &mecha_sauvage);
                    }

                    if(detection_combat_pnj(&j)){
                        //attaque_ordi_pnj(pnj, &mecha_sauvage);
                    }

                    animation(&j, &sprite_p);

                    SDL_RenderClear(game.renderer);     //efface l'ecran

                    draw_all(&j,&sprite_p,pnj_sprite);


                    SDL_RenderPresent(game.renderer);      //affiche rendu


                    frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

                    if (FRAME_DELAY > frameTime) {
                        SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
                    }
                }   
            }
            sauvegarde_partie(&j,pseudo);
            free_mat(taille_x_mat, taille_y_mat);
            destruction_joueur(&j);
            for(int i = 0;i<NB_PNJ;i++){
                destruction_pnj(&pnj[i]);
            }
        }
    }
    cleanUp();
    return 0;

}
