#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/player.h"

const int FRAME = 12 ;

void deplacement(game_t * game, case_t ** mat, int taille_x, int taille_y, const Uint8 *keys, joueur_t * j) {


    if (j->moving) return;  // si joueur deja entrain de se deplacer on ne fait rien

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
    if (dx != 0 && dy != 0) return; //empeche deplacement en diagonal
    
    //stocke la nouvelle case 
    int new_x = j->x + dx;
    int new_y = j->y + dy;

    //verifie qu'on peut se deplacer
    if (new_x >= 0 && new_x < taille_x && new_y >= 0 && new_y < taille_y && mat[new_y][new_x].obj == RIEN) {   
        // met à jour la mat
        mat[j->y][j->x].obj = RIEN;
        mat[new_y][new_x].obj = JOUEUR;

        // initialise l'animation
        j->x = new_x;
        j->y = new_y;
        j->move_dx = dx * (PX * game->scale) / FRAME;  // divise le déplacement en 12 étapes
        j->move_dy = dy * (PX * game->scale) / FRAME;
        j->moving = FRAME;  // animation sur 16 frames

    }
    


}


void animation(joueur_t *j, SDL_Rect *sprite_p) {
    if (j->moving > 0) {
        j->screen_x += j->move_dx;
        j->screen_y += j->move_dy;
        sprite_p->x = (int)j->screen_x;
        sprite_p->y = (int)j->screen_y - 38;
        j->moving--; // Réduit le compteur
    }

    
}