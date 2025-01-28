#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../lib/initSDL.h"


typedef struct positions {
    int x;
    int y;
}position_t;

typedef struct players {
    position_t pos; 
}player_t;

SDL_Rect init_player(player_t * p, float scale);
void draw_player(game_t game, SDL_Rect player);