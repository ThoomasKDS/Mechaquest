#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/player.h"


void draw_obj(game_t game, SDL_Rect obj) {
    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255); // Rouge
    SDL_RenderFillRect(game.renderer, &obj); // Dessiner le rectangle du joueur
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255); // remet la couleur en noir pour le fond

}

SDL_Rect create_obj(game_t game, int taille_w, int taille_h, int x, int y) {
    SDL_Rect obj;

    float scale_x = (float)game.dms_win.w/game.img_w; 
    float scale_y = (float)game.dms_win.h/game.img_h;

    float scale = (scale_x < scale_y) ? scale_x : scale_y; //prend l'echelle la plus petite

    obj.w = taille_w * scale;
    obj.h = taille_h * scale;
    obj.x = x;
    obj.y = y;

    return obj;
}