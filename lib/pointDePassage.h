#ifndef POINT_DE_PASSAGE_H
#define POINT_DE_PASSAGE_H
#include "affichage.h"
#include "menu.h"
#include "sauv.h"

extern mechas_t mecha[24];
extern attaque_t attaque[64];
extern zone_t zone[10];
//extern pnj_t pnj[24];

int soigner(joueur_t *j);

int copie_mechas(joueur_t *j,mechas_joueur_t *mecha);

int choix_starter(joueur_t *j,pnj_t *vinGazole);

int parler_a_vin_gazole(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p, const Uint8 *keys);

int premier_combat_musk(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p, const Uint8 *keys);

int retourner_parler_a_vin_gazole(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p,const Uint8 *keys);

int combat_final(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p,const Uint8 *keys);

int jeu_libre(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p,const Uint8 *keys);

#endif

