#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/player.h"

Uint32 lastMoveTime = 0;
const Uint32 moveDelay = 100; // 200 ms entre les déplacements

void deplacement(game_t game, case_t ** mat, SDL_Rect * hitbox_player, int taille_x, int taille_y, const Uint8 *keys, Joueur * j) {

    Uint32 now = SDL_GetTicks();
    if (now - lastMoveTime < moveDelay) return;  // Empêche le déplacement trop rapide

    lastMoveTime = now; // Met à jour le dernier déplacement

    if(keys[SDL_SCANCODE_LEFT]) {
        if (j->x-1 >= 0 && mat[j->y][j->x - 1].obj == RIEN) {
                mat[j->y][j->x].obj = RIEN;
                mat[j->y][--j->x].obj = JOUEUR;
                hitbox_player->x -= PX * game.scale;
        }

    }
    else if(keys[SDL_SCANCODE_RIGHT]) {
        if (j->x+1 < taille_x && mat[j->y][j->x + 1].obj == RIEN) { 
                mat[j->y][j->x].obj = RIEN;
                mat[j->y][++j->x].obj = JOUEUR;
                hitbox_player->x += PX * game.scale;

        }

    }
    else if(keys[SDL_SCANCODE_UP]) {
        if (j->y-1 >= 0 && mat[j->y - 1][j->x].obj == RIEN) { 
                mat[j->y][j->x].obj = RIEN;
                mat[--j->y][j->x].obj = JOUEUR;
                hitbox_player->y -= PX * game.scale;

        }

    }
    else if(keys[SDL_SCANCODE_DOWN]) {
        if (j->y+1 < taille_y && mat[j->y + 1][j->x].obj == RIEN) { 
                mat[j->y][j->x].obj = RIEN;
                mat[++j->y][j->x].obj = JOUEUR;
                hitbox_player->y += PX * game.scale;
        
        }


    }


}
