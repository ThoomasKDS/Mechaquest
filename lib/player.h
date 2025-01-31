#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include "../lib/initGame.h"
#include "../lib/affichage.h"
#include "../lib/sauv.h"



void deplacement(game_t *game, case_t ** mat, SDL_Rect * hitbox_player, int taille_x, int taille_y, const Uint8 *keys, Joueur * j);
void animation(Joueur *j, SDL_Rect *hitbox_player);