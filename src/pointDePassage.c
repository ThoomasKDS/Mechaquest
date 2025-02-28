#include "../lib/pointDePassage.h"
#include "../lib/combat.h"


int soigner(joueur_t *j){
    j->inventaire->mechaball = 5;
    j->inventaire->carburant = 5;
    j->inventaire->rappel = 5;
    j->inventaire->repousse = 5;
    for(int i = 0;i < j->nb_mechas && i < 4;i++){
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
                    if(j->numMap == 0 && j->x+1 == pnj[17].x && j->y == pnj[17].y && j->derniere_touche == 2 && pnj[17].etat == 0){
                        printf("%s\n",pnj[17].dialogueDebut);
                        choix_starter(j,&pnj[17]);
                        printf("%s\n",pnj[17].dialogueFin);
                        pnj[17].etat = 1;
                        sauvegarde_pnj(&pnj[17],18,j->pseudo);
                        game->mat[0][0][15] = -12;
                        game->mat[0][0][16] = -12;
                        game->mat[0][0][17] = -12;
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
        if(j->numMap == 0 && j->x+1 == pnj[18].x && j->y == pnj[18].y && j->derniere_touche == 2){
            if(j->inventaire->mechaball < 5 || j->inventaire->carburant < 5|| j->inventaire->repousse <5 || j->inventaire->rappel< 5)
                soin = 1;
            for(int i = 0;i < 4 && soin == 0;i++){
                if(j->mechas_joueur[0].pv < j->mechas_joueur[0].pv_max)
                    soin = 1;       
            }
            if(soin){
                printf("%s\n",pnj[18].dialogueFin);
                soigner(j);
                soin = 0;
            }
            else{
                printf("%s\n",pnj[18].dialogueDebut);
            }
            
        }
        if(j->numMap == 2 && j->x+1 == pnj[19].x && j->y == pnj[19].y && j->derniere_touche == 2){
            printf("%s\n",pnj[19].dialogueDebut);
            
            //combat(j,&pnj[19]);
        
            printf("%s\n",pnj[19].dialogueFin);
            pnj[19].etat = 1;
            sauvegarde_pnj(&pnj[19],20,j->pseudo);
            game->mat[2][0][4] = -14;
            game->mat[2][0][5] = -14;
            game->mat[2][0][6] = -14;
            game->mat[2][9][19] = 0;

            game->mat[2][8][0] = -16;
            game->mat[2][9][0] = -16;
            game->mat[2][10][0] = -16;

            j->x = 23;
            j->y = 8;
            j->numMap = 0;
            j->pointSauvegarde = 2;
            j->derniere_touche = 4;
            game->mat_active = j->numMap;
            sauvegarde_partie(j,j->pseudo);
            j->screen_x = (float)(game->dms_win.x + (j->x * PX * game->scale));      //position du joueur en px
            j->screen_y = (float)(game->dms_win.y + (j->y * PX * game->scale));
            if(!init_player_h(game, sprite_playerH)){
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
        if(j->x+1 == pnj[20].x && j->y == pnj[20].y && j->derniere_touche == 2 && pnj[20].etat == 0){
            printf("%s\n",pnj[20].dialogueDebut);
            printf("%s\n",pnj[20].dialogueFin);
            soigner(j);
            pnj[20].etat = 1;
            sauvegarde_pnj(&pnj[20],21,j->pseudo);
            game->mat[0][0][15] = -12;
            game->mat[0][0][16] = -12;
            game->mat[0][0][17] = -12;
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
        if(j->numMap == 0 && j->x+1 == pnj[21].x && j->y == pnj[21].y && j->derniere_touche == 2){
            if(j->inventaire->mechaball < 5 || j->inventaire->carburant < 5|| j->inventaire->repousse <5 || j->inventaire->rappel< 5)
                soin = 1;
            for(int i = 0;i < 4 && soin == 0;i++){
                if(j->mechas_joueur[0].pv < j->mechas_joueur[0].pv_max)
                    soin = 1;       
            }
            if(soin){
                printf("%s\n",pnj[21].dialogueFin);
                soigner(j);
                soin = 0;
            }
            else{
                printf("%s\n",pnj[21].dialogueDebut);
            }
            
        }
        if(j->numMap == 4 && j->x == pnj[22].x && j->y-1 == pnj[22].y && j->derniere_touche == 3 && pnj[22].etat == 0){
            printf("%s\n",pnj[22].dialogueDebut);
            
            //combat avec iron musk
            
            printf("%s\n",pnj[22].dialogueFin);
            copie_mechas(j,&pnj[22].mechas_joueur[0]);
            sauvegarde_partie(j,j->pseudo);
            pnj[22].etat = 1;
            sauvegarde_pnj(&pnj[22],23,j->pseudo);

            j->pointSauvegarde = 4;

        }
    }
    return OK;
}

int jeu_libre(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p,const Uint8 *keys){
    int soin = 0;
    if (keys[SDL_SCANCODE_P]){
        if(j->numMap == 0 && j->x+1 == pnj[23].x && j->y == pnj[23].y && j->derniere_touche == 2){
            if(pnj[23].etat == 0){
                printf("%s\n",pnj[23].dialogueDebut);
                printf("%s\n",pnj[23].dialogueFin);
                soigner(j);
                pnj[23].etat = 1;
                sauvegarde_pnj(&pnj[23],24,j->pseudo);
            }
            else if(pnj[23].etat == 1){
                if(j->inventaire->mechaball < 5 || j->inventaire->carburant < 5|| j->inventaire->repousse <5 || j->inventaire->rappel< 5)
                    soin = 1;
                for(int i = 0;i < 4 && soin == 0;i++){
                    if(j->mechas_joueur[0].pv < j->mechas_joueur[0].pv_max)
                        soin = 1;       
                }
                if(soin){
                    printf("%s\n",pnj[21].dialogueFin);
                    soigner(j);
                    soin = 0;
                }
                else{
                    printf("%s\n",pnj[21].dialogueDebut);
                }
            }
            
        }
    }
    return OK;
}
