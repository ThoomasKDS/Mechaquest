/**
 * @file pointDePassage.h
 * @brief Gestion des points de passage et événements du jeu.
 * 
 * Ce fichier contient les déclarations des fonctions permettant de gérer les événements
 * clés du jeu, tels que les soins, la sélection du starter, et les combats majeurs.
 */
#ifndef POINT_DE_PASSAGE_H
#define POINT_DE_PASSAGE_H

#include "../lib/menu.h"
#include "../lib/sauv.h"
#include "../lib/global.h"

int soigner(joueur_t *j);

int copie_mechas(joueur_t *j,mechas_joueur_t *mecha);

int choix_starter(joueur_t *j, pnj_t *vinGazole, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite);

int parler_a_vin_gazole(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, const Uint8 *keys);

int premier_combat_musk(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, const Uint8 *keys);

int retourner_parler_a_vin_gazole(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, const Uint8 *keys);

int combat_final(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, const Uint8 *keys);

int jeu_libre(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite,const Uint8 *keys);

#endif

