/**
 * @file player.h
 * @brief Gestion du joueur dans le jeu.
 * 
 * Ce fichier contient les déclarations des fonctions permettant de gérer les déplacements,
 * les animations et l'apparition des mechas ainsi que les interactions avec les PNJ.
 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include <stdlib.h>
#include "../lib/initGame.h"
#include "../lib/affichage.h"
#include "../lib/sauv.h"
#include "../lib/global.h"



//Gestion de deplacement du joueur et du tp entre les maps
int deplacement(int taille_x, int taille_y, const Uint8 *keys, joueur_t * j, int * last_case, SDL_Rect *sprite_p);
//animation du joueur en switchant entre les sprites
void animation(joueur_t *j, SDL_Rect *sprite_p);   
//gere l'apparition des mechas 
int spawn_mecha(joueur_t * j, int obj_case, mechas_joueur_t * mecha_sauvage);
//Detecte quand on passe devant un pnj pour lancer un combat
int detection_combat_pnj(joueur_t *joueur);
