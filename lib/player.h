#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include <stdlib.h>
#include "../lib/initGame.h"
#include "../lib/affichage.h"
#include "../lib/sauv.h"



//Gestion de deplacement du joueur et du tp entre les maps
int deplacement(game_t * game, int taille_x, int taille_y, const Uint8 *keys, joueur_t * j, int * last_case, SDL_Rect *sprite_p, zone_t * zone, mechas_t * mechas, mechas_joueur_t * mecha_sauvage);
//animation du joueur en switchant entre les sprites
void animation(joueur_t *j, SDL_Rect *sprite_p);   
//gere l'apparition des mechas 
int spawn_mecha(joueur_t * j, int obj_case, zone_t * zone, mechas_t * mechas, mechas_joueur_t * mecha_sauvage);