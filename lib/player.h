#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../lib/initGame.h"


typedef struct {
    int x;
    int y;
}position_t;

void draw_obj(game_t game, SDL_Rect obj);
SDL_Rect create_obj(game_t game, int taille_w, int taille_h, int x, int y);