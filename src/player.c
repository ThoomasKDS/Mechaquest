#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/player.h"

#define W_HITBOX 32
#define H_HITBOX 32

SDL_Rect init_player(player_t * p, float scale) {

    p->pos.x = 0;
    p->pos.y = 0; //attribution manuel de position pour l'instant

    SDL_Rect player;
    player.w = W_HITBOX * scale;
    player.h = H_HITBOX * scale;
    player.x = p->pos.x;
    player.y = p->pos.y;
    return player;
}

void draw_player(game_t game, SDL_Rect player) {
    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255); // Rouge
    SDL_RenderFillRect(game.renderer, &player); // Dessiner le rectangle du joueur
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255); // remet la couleur en noir pour le fond

}