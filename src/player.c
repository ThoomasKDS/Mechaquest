/**
 * \file player.c
 * \brief fichier contenant les fonctions de déplacement
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/player.h"
#include "../lib/affichage.h"
#include <pointDePassage.h>


const int FRAME = 15 ;      //Nombre d'image dans l'animation


//Gestion de deplacement du joueur et du tp entre les maps
int deplacement(game_t * game, int taille_x, int taille_y, const Uint8 *keys, joueur_t * j, int * last_case, SDL_Rect *sprite_p) {
    int obj_case = RIEN;
    if (!j->moving){  // si joueur deja entrain de se deplacer on ne fait rien

        int dx = 0, dy = 0;

        //direction du deplacement
        if (keys[SDL_SCANCODE_LEFT]) {
            dx = -1;
            j->derniere_touche = 1;
        }  
        if (keys[SDL_SCANCODE_RIGHT]) {
            dx = 1;
            j->derniere_touche = 2;

        } 
        if (keys[SDL_SCANCODE_UP]){
            dy = -1;
            j->derniere_touche = 3;
        }   
        if (keys[SDL_SCANCODE_DOWN]){
            j->derniere_touche = 4;
            dy = 1;
        }  
        if (dx != 0 && dy != 0) return 0; //empeche deplacement en diagonal
        
        //stocke la nouvelle case 
        int new_x = j->x + dx;
        int new_y = j->y + dy;

        //verifie qu'on peut se deplacer
        if (new_x >= 0 && new_x <= taille_x && new_y >= 0 && new_y <= taille_y && game->mat[game->mat_active][new_y][new_x] <= RIEN) {   
            obj_case = game->mat[game->mat_active][new_y][new_x];
            if(game->mat[game->mat_active][new_y][new_x] <= TPMAP1) { //si on change de map
                game->mat[game->mat_active][j->y][j->x] = RIEN;
                game->mat_active = -(game->mat[game->mat_active][new_y][new_x] + 11); //converti avec le bonne indice de la map
                

                //place au bonne endroit le joueur en fonction de ou le tp se trouve
                if(new_y == 0) j->y = taille_y - 2;
                else if(new_y == taille_y-1) j->y = 1;
                if(new_x == taille_x - 1) j->x = 1;
                else if(new_x == 0) j->x = taille_x - 2;



                j->screen_x = game->dms_win.x + (PX * j->x * game->scale);
                j->screen_y = game->dms_win.y + (PX * j->y * game->scale);
                sprite_p->x = j->screen_x;
                sprite_p->y = j->screen_y;
                game->mat[game->mat_active][j->y][j->x] = JOUEUR;
                j->numMap = game->mat_active;
                
            }

            else {
                // met à jour la mat            
                game->mat[game->mat_active][j->y][j->x] = *last_case;
                *last_case = game->mat[game->mat_active][new_y][new_x];
                game->mat[game->mat_active][new_y][new_x] = JOUEUR;

                // initialise l'animation
                j->x = new_x;
                j->y = new_y;
                j->move_dx = dx * (PX * game->scale) / FRAME;  // divise le déplacement en 15 étapes
                j->move_dy = dy * (PX * game->scale) / FRAME;
                j->moving = FRAME;  // animation sur 16 frames
                //vérification sapwn mechas
                
            }
        }
    }
    return obj_case;
}


//animation du joueur en switchant entre les sprites
void animation(joueur_t *j, SDL_Rect *sprite_p) {
    if (j->moving > 0) {
        j->screen_x += j->move_dx;
        j->screen_y += j->move_dy;
        sprite_p->x = (int)j->screen_x;
        sprite_p->y = (int)j->screen_y - 38;
        j->moving--;
    }

    if (j->moving == 0) {
        j->move_dx = 0;
        j->move_dy = 0;
    }

    
}

//gere l'apparition des mechas 
int spawn_mecha(joueur_t * j, int obj_case, zone_t * zone, mechas_t * mechas, mechas_joueur_t * mecha_sauvage) {
    if(obj_case <= Z1 && obj_case >= Z10) {     //Z1 => Z10 nombres negatifs
        j->proba_combat += 100;
        int n = rand() % 100;
        if(n < j->proba_combat) {
            j->proba_combat = 0;
            obj_case = -obj_case - 1;               //donne l'indice de la zone
            int indice_liste = rand() % zone[obj_case].nb_mechas;
            int indice_mechas =  zone[obj_case].listeMechasZone[indice_liste];      
            mecha_sauvage->niveau = (rand() % 5) + (zone[obj_case].NiveauMoyenApparition - 2);
            mecha_sauvage->pv_max = (rand() % 5) + (zone[obj_case].PvMoyen - 2);
            mecha_sauvage->pv = mecha_sauvage->pv_max;
            mecha_sauvage->id_mechas = indice_mechas;
            mecha_sauvage->attaque = (rand() % 10) + (zone[obj_case].Attaque - 4);
            mecha_sauvage->defense = (rand() % 10) + (zone[obj_case].Defense - 4);
            mecha_sauvage->vitesse = (rand() % 6) + (zone[obj_case].VitesseMoyenne  - 3);
            return 1;
        }
    }
    return 0;
}

int detection_combat_pnj(game_t game, joueur_t joueur){
    for(int i = 0; i < 24; i++){
        if(pnj[i].id_map == joueur.numMap){
            for(int j = 1; j <= 4; j++){
                switch(pnj[i].orientation){
                    case 0:
                        if(game.mat[game.mat_active][pnj[i].x][pnj[i].y + j] == 1){
                            return 1;
                        }
                    break;
                    case 1:
                        if(game.mat[game.mat_active][pnj[i].x - j][pnj[i].y] == 1){
                            return 1;
                        }
                    break;
                    case 2:
                        if(game.mat[game.mat_active][pnj[i].x][pnj[i].y - j] == 1){
                            return 1;
                        }
                    break;
                    case 3:
                        if(game.mat[game.mat_active][pnj[i].x + j][pnj[i].y] == 1){
                            return 1;
                        }
                    break;
                }
            }
            
        }
    }
    return 0;
}