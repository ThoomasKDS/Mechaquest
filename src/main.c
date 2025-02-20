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

mechas_t mecha[24];
attaque_t attaque[64];
zone_t zone[10];
pnj_t pnj;


int main() {
    //VARIABLES UTILES AU PROGRAMME

    game_t game;
    img_player_t sprite_playerH;
    joueur_t j;
    char pseudo[50] = "player1";
    recuperation_joueur(&j,pseudo);
    game.mat_active = j.numMap;

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
    aff_mat(&game, taille_x_mat, taille_y_mat, 1);
    //INITIALISE LES MOUVEMENTS DU JOUEUR ET COORS
    j.moving = 0;
    j.derniere_touche = 4;
    j.proba_combat = 0;
    j.screen_x = (float)(game.dms_win.x + (j.x * PX * game.scale));      //position du joueur en px
    j.screen_y = (float)(game.dms_win.y + (j.y * PX * game.scale));
    if(!init_player_h(&game, &sprite_playerH)){
        return -1;
    }
    //SPRITE JOUEUR
    SDL_Rect sprite_p = create_obj(&game, PX, 48, j.x*PX, j.y * PX - 24, JOUEUR, 1);
    if(j.pointSauvegarde == 0)
        parler_a_vin_gazole(&game,&sprite_playerH,&j,&sprite_p, mecha, zone);
    if(j.pointSauvegarde == 1)
        premier_combat_musk(&game,&sprite_playerH,&j,&sprite_p, mecha, zone);
    if(j.pointSauvegarde == 2)
        retourner_parler_a_vin_gazole(&game,&sprite_playerH,&j,&sprite_p, mecha, zone);
    if(j.pointSauvegarde == 3)
        combat_final(&game,&sprite_playerH,&j,&sprite_p, mecha, zone);
    if(j.pointSauvegarde == 4)
        jeu_libre(&game,&sprite_playerH,&j,&sprite_p, mecha, zone);
    sauvegarde_partie(&j,pseudo);
    cleanUp(&game);
    free_mat(&game,taille_x_mat, taille_y_mat);
    destruction_joueur(&j);

    return 0;

}