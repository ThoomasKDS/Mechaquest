#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include "../lib/initGame.h"
#include "../lib/affichage.h"
#include "../lib/sauv.h"



//Gestion de deplacement du joueur et du tp entre les maps
void deplacement(game_t * game, int taille_x, int taille_y, const Uint8 *keys, joueur_t * j, int * last_case, SDL_Rect *sprite_p);
//animation du joueur en switchant entre les sprites
void animation(joueur_t *j, SDL_Rect *sprite_p);   