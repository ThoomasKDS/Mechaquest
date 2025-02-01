#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include "../lib/initGame.h"
#include "../lib/affichage.h"
#include "../lib/sauv.h"




void deplacement(game_t *game, int** mat, int taille_x, int taille_y, const Uint8 *keys, joueur_t * j);
void animation(joueur_t *j, SDL_Rect *sprite_p);