#include "../lib/pointDePassage.h"
#include "../lib/combat.h"


int soigner(joueur_t *j){
    j->inventaire->mechaball = NB_OBJET;
    j->inventaire->carburant = NB_OBJET;
    j->inventaire->rappel = NB_OBJET;
    j->inventaire->repousse = NB_OBJET;
    for(int i = 0;i < j->nb_mechas && i < NB_MECHAS_INVENTAIRE;i++){
        j->mechas_joueur[i].pv = j->mechas_joueur[i].pv_max;
        j->mechas_joueur[i].utilisation_1 = attaque[j->mechas_joueur[i].attaque_1-1].utilisations;
        j->mechas_joueur[i].utilisation_2 = attaque[j->mechas_joueur[i].attaque_2-1].utilisations;
    }
    sauvegarde_partie(j,j->pseudo);
    return OK;
}

int copie_mechas(joueur_t *j,mechas_joueur_t *mecha){
    j->mechas_joueur[j->nb_mechas].id_mechas = mecha->id_mechas;
    j->mechas_joueur[j->nb_mechas].numero = j->nb_mechas+1;
    j->mechas_joueur[j->nb_mechas].niveau = mecha->niveau;
    j->mechas_joueur[j->nb_mechas].xp = mecha->xp;
    j->mechas_joueur[j->nb_mechas].pv = mecha->pv;
    j->mechas_joueur[j->nb_mechas].pv_max = mecha->pv_max;
    j->mechas_joueur[j->nb_mechas].attaque = mecha->attaque;
    j->mechas_joueur[j->nb_mechas].defense = mecha->defense;
    j->mechas_joueur[j->nb_mechas].vitesse = mecha->vitesse;
    j->mechas_joueur[j->nb_mechas].attaque_1 = mecha->attaque_1;
    j->mechas_joueur[j->nb_mechas].attaque_2 = mecha->attaque_2;
    j->mechas_joueur[j->nb_mechas].utilisation_1 = mecha->utilisation_1;
    j->mechas_joueur[j->nb_mechas].utilisation_2 = mecha->utilisation_2;
    j->nb_mechas+=1;
    return OK;
}

int choix_starter(joueur_t *j,pnj_t *vinGazole){
    int choix = 0;
    SDL_Event event;
    printf("Choisissez Le mechas que vous souhaité: \n");
    printf("1: Tournicoton \n");
    printf("2: Rasetout \n");
    printf("3: Tikart\n");
    while(!choix){
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_KEYDOWN) {  
                if (event.key.keysym.sym == SDLK_1 || event.key.keysym.sym == SDLK_KP_1){
                    choix = copie_mechas(j,&vinGazole->mechas_joueur[0]);
                    printf("Vous avez choisit Tournicoton\n");
                }
                else if (event.key.keysym.sym == SDLK_2 || event.key.keysym.sym == SDLK_KP_2){
                    choix = copie_mechas(j,&vinGazole->mechas_joueur[1]); 
                    printf("Vous avez choisit Rasetout\n");
                }
                else if (event.key.keysym.sym == SDLK_3 || event.key.keysym.sym == SDLK_KP_3){
                    choix = copie_mechas(j,&vinGazole->mechas_joueur[2]); 
                    printf("Vous avez choisit Tikart\n");
                }
            }
        }
        if(choix){
            j->inventaire->mechaball = vinGazole->inventaire->mechaball;
            j->inventaire->carburant = vinGazole->inventaire->carburant;
            j->inventaire->rappel = vinGazole->inventaire->rappel;
            j->inventaire->repousse = vinGazole->inventaire->repousse;
        }
    }
    j->pointSauvegarde = 1;
    sauvegarde_partie(j,j->pseudo);
    return OK;
}


int parler_a_vin_gazole(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p, const Uint8 *keys){
                if (keys[SDL_SCANCODE_P]){
                    if(j->numMap == 0 && j->x+1 == pnj[VIN_GAZOLE_1].x && j->y == pnj[VIN_GAZOLE_1].y && j->derniere_touche == 2 && pnj[VIN_GAZOLE_1].etat == 0){
                        printf("%s\n",pnj[VIN_GAZOLE_1].dialogueDebut);
                        choix_starter(j,&pnj[VIN_GAZOLE_1]);
                        printf("%s\n",pnj[VIN_GAZOLE_1].dialogueFin);
                        pnj[VIN_GAZOLE_1].etat = 1;
                        sauvegarde_pnj(&pnj[VIN_GAZOLE_1],pnj[VIN_GAZOLE_1].id_pnj,j->pseudo);
                        game->mat[0][0][15] = TPMAP2;
                        game->mat[0][0][16] = TPMAP2;
                        game->mat[0][0][17] = TPMAP2;
                        j->pointSauvegarde = 1;
                    }
                }
                if(keys[SDL_SCANCODE_UP]){
                    if(j->numMap == 0 && (  j->x == 15 || j->x == 16 ||j->x == 17) && j->y == 1 ){
                        printf("Allez d'abord parler à Vin Gazole\n");
                    }
                }
    return OK;
}

int premier_combat_musk(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p, const Uint8 *keys){
    int soin = 0;
    if (keys[SDL_SCANCODE_P]){
        if(j->numMap == 0 && j->x+1 == pnj[VIN_GAZOLE_2].x && j->y == pnj[VIN_GAZOLE_2].y && j->derniere_touche == 2){
            if(j->inventaire->mechaball < NB_OBJET || j->inventaire->carburant < NB_OBJET|| j->inventaire->repousse < NB_OBJET || j->inventaire->rappel< NB_OBJET)
                soin = 1;
            for(int i = 0;i < NB_MECHAS_INVENTAIRE && soin == 0;i++){
                if(j->mechas_joueur[0].pv < j->mechas_joueur[0].pv_max)
                    soin = 1;       
            }
            if(soin){
                printf("%s\n",pnj[VIN_GAZOLE_2].dialogueFin);
                soigner(j);
                soin = 0;
            }
            else{
                printf("%s\n",pnj[VIN_GAZOLE_2].dialogueDebut);
            }
            
        }
        if(j->numMap == 2 && j->x+1 == pnj[IRON_MUSK_DEB].x && j->y == pnj[IRON_MUSK_DEB].y && j->derniere_touche == 2){
            printf("%s\n",pnj[IRON_MUSK_DEB].dialogueDebut);
            
            //combat(j,&pnj[IRON_MUSK_DEB]);
        
            printf("%s\n",pnj[IRON_MUSK_DEB].dialogueFin);
            pnj[IRON_MUSK_DEB].etat = 1;
            sauvegarde_pnj(&pnj[IRON_MUSK_DEB],pnj[IRON_MUSK_DEB].id_pnj,j->pseudo);
            game->mat[2][0][4] = TPMAP4;
            game->mat[2][0][5] = TPMAP4;
            game->mat[2][0][6] = TPMAP4;
            game->mat[2][9][19] = 0;

            game->mat[2][8][0] = TPMAP6;
            game->mat[2][9][0] = TPMAP6;
            game->mat[2][10][0] = TPMAP6;

            j->x = 23;
            j->y = 8;
            j->numMap = 0;
            j->pointSauvegarde = 2;
            j->derniere_touche = 4;
            game->mat_active = j->numMap;
            sauvegarde_partie(j,j->pseudo);
            j->screen_x = (float)(game->dms_win.x + (j->x * PX * game->scale));      //position du joueur en px
            j->screen_y = (float)(game->dms_win.y + (j->y * PX * game->scale));
            if(!init_player(game, sprite_playerH,j->sexe)){
                return -1;
            }   
        }
    }
    if(keys[SDL_SCANCODE_UP]){
        if(j->numMap == 2 && (j->x == 4 || j->x == 5 ||j->x == 6) && j->y == 1){
            printf("Allez combattre Iron Musk avant\n");
        }
    }
    return OK;
}

int retourner_parler_a_vin_gazole(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p, const Uint8 *keys){
    if (keys[SDL_SCANCODE_P]){
        if(j->x+1 == pnj[VIN_GAZOLE_3].x && j->y == pnj[VIN_GAZOLE_3].y && j->derniere_touche == 2 && pnj[VIN_GAZOLE_3].etat == 0){
            printf("%s\n",pnj[VIN_GAZOLE_3].dialogueDebut);
            printf("%s\n",pnj[VIN_GAZOLE_3].dialogueFin);
            soigner(j);
            pnj[VIN_GAZOLE_3].etat = 1;
            sauvegarde_pnj(&pnj[VIN_GAZOLE_3],pnj[VIN_GAZOLE_3].id_pnj,j->pseudo);
            game->mat[0][0][15] = TPMAP2;
            game->mat[0][0][16] = TPMAP2;
            game->mat[0][0][17] = TPMAP2;
            j->pointSauvegarde = 3;
        }
    }
    if(keys[SDL_SCANCODE_UP]){
        if((j->x == 15 || j->x == 16 ||j->x == 17) && j->y == 1){
            printf("Allez vous soigner\n");
        }
    } 
    return OK;
}

int combat_final(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p,const Uint8 *keys){
    int soin = 0;
    if (keys[SDL_SCANCODE_P]){
        if(j->numMap == 0 && j->x+1 == pnj[VIN_GAZOLE_4].x && j->y == pnj[VIN_GAZOLE_4].y && j->derniere_touche == 2){
            if(j->inventaire->mechaball < NB_OBJET || j->inventaire->carburant < NB_OBJET|| j->inventaire->repousse <NB_OBJET || j->inventaire->rappel< NB_OBJET)
                soin = 1;
            for(int i = 0;i < NB_MECHAS_INVENTAIRE && soin == 0;i++){
                if(j->mechas_joueur[0].pv < j->mechas_joueur[0].pv_max)
                    soin = 1;       
            }
            if(soin){
                printf("%s\n",pnj[VIN_GAZOLE_4].dialogueFin);
                soigner(j);
                soin = 0;
            }
            else{
                printf("%s\n",pnj[VIN_GAZOLE_4].dialogueDebut);
            }
            
        }
        if(j->numMap == 4 && j->x == pnj[IRON_MUSK_FIN].x && j->y-1 == pnj[IRON_MUSK_FIN].y && j->derniere_touche == 3 && pnj[IRON_MUSK_FIN].etat == 0){
            printf("%s\n",pnj[IRON_MUSK_FIN].dialogueDebut);
            
            //combat avec iron musk
            
            printf("%s\n",pnj[IRON_MUSK_FIN].dialogueFin);
            copie_mechas(j,&pnj[IRON_MUSK_FIN].mechas_joueur[0]);
            sauvegarde_partie(j,j->pseudo);
            pnj[IRON_MUSK_FIN].etat = 1;
            sauvegarde_pnj(&pnj[IRON_MUSK_FIN],pnj[IRON_MUSK_FIN].id_pnj,j->pseudo);

            j->pointSauvegarde = 4;

        }
    }
    return OK;
}

int jeu_libre(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p,const Uint8 *keys){
    int soin = 0;
    if (keys[SDL_SCANCODE_P]){
        if(j->numMap == 0 && j->x+1 == pnj[VIN_GAZOLE_5].x && j->y == pnj[VIN_GAZOLE_5].y && j->derniere_touche == 2){
            if(pnj[VIN_GAZOLE_5].etat == 0){
                printf("%s\n",pnj[VIN_GAZOLE_5].dialogueDebut);
                printf("%s\n",pnj[VIN_GAZOLE_5].dialogueFin);
                soigner(j);
                pnj[VIN_GAZOLE_5].etat = 1;
                sauvegarde_pnj(&pnj[VIN_GAZOLE_5],pnj[VIN_GAZOLE_5].id_pnj,j->pseudo);
            }
            else if(pnj[VIN_GAZOLE_5].etat == 1){
                if(j->inventaire->mechaball < NB_OBJET || j->inventaire->carburant < NB_OBJET|| j->inventaire->repousse < NB_OBJET || j->inventaire->rappel< NB_OBJET)
                    soin = 1;
                for(int i = 0;i < NB_MECHAS_INVENTAIRE && soin == 0;i++){
                    if(j->mechas_joueur[0].pv < j->mechas_joueur[0].pv_max)
                        soin = 1;       
                }
                if(soin){
                    printf("%s\n",pnj[VIN_GAZOLE_4].dialogueFin);
                    soigner(j);
                    soin = 0;
                }
                else{
                    printf("%s\n",pnj[VIN_GAZOLE_4].dialogueDebut);
                }
            }
            
        }
    }
    return OK;
}
