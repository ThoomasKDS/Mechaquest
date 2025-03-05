#ifndef POINT_DE_PASSAGE_H
#define POINT_DE_PASSAGE_H

#include "../lib/menu.h"
#include "../lib/sauv.h"
#include "../lib/global.h"

int soigner(joueur_t *j);

int copie_mechas(joueur_t *j,mechas_joueur_t *mecha);

int choix_starter(game_t *game, joueur_t *j, pnj_t *vinGazole, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, img_pnj_t * sprite_pnj, img_player_t * sprite_playerH);

int parler_a_vin_gazole(game_t *game, joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, img_pnj_t * sprite_pnj, img_player_t * sprite_playerH, const Uint8 *keys);

int premier_combat_musk(game_t *game, joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, img_pnj_t * sprite_pnj, img_player_t * sprite_playerH, const Uint8 *keys);

int retourner_parler_a_vin_gazole(game_t *game, joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, img_pnj_t * sprite_pnj, img_player_t * sprite_playerH, const Uint8 *keys);

int combat_final(game_t *game, joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, img_pnj_t * sprite_pnj, img_player_t * sprite_playerH, const Uint8 *keys);

int jeu_libre(game_t *game, joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, img_pnj_t * sprite_pnj, img_player_t * sprite_playerH, const Uint8 *keys);

#endif

