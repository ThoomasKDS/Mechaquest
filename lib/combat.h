#ifndef COMBAT_H
#define COMBAT_H
#include "sauv.h"
#include "pointDePassage.h"
#include "../lib/global.h"



int aff_mechas_combat(game_t * game, joueur_t * joueur);

void draw_combat(game_t * game, mechas_joueur_t * mecha_joueur, mechas_joueur_t * mecha_ordi);

void animation_degat(int mecha_att, int pv_old, int pv_new, game_t *game, mechas_joueur_t * mecha_joueur, mechas_joueur_t * mecha_ordi);//mecha 0 pour le joueur, 1 pour l'ordi

int utilisation_carburant(game_t *game, joueur_t *joueur, mechas_joueur_t *ordi, int *actif);

int utilisation_rappel(game_t *game, joueur_t *joueur, mechas_joueur_t *ordi, int *actif);

int utilisation_mechaball(game_t *game,joueur_t * joueur, mechas_joueur_t *ordi, int *actif);

int utilisation_objet(game_t *game, joueur_t *joueur, mechas_joueur_t *ordi, int *actif);

int changer_mecha(game_t *game, joueur_t *joueur, int *actif, mechas_joueur_t *ordi);

int algo_attaque(int choix, mechas_joueur_t *mecha_att, mechas_joueur_t *mecha_def, game_t *game, int mecha_choix);

int attaque_joueur(game_t *game, joueur_t *j, mechas_joueur_t *mecha_ordi, int * actif);

int attaque_ordi_sauvage(mechas_joueur_t *mecha_ordi, mechas_joueur_t *mecha_joueur, game_t *game);

int attaque_ordi_pnj(mechas_joueur_t *mecha_ordi, mechas_joueur_t *mecha_joueur);

int tour_joueur(joueur_t *joueur, mechas_joueur_t *mecha_sauvage, game_t *game, int *actif);

void combat_sauvage(joueur_t *joueur, mechas_joueur_t *mecha_sauvage, game_t *game);

int apprentissage_attaque(mechas_joueur_t *mecha_joueur);

void level_mechas(joueur_t *mechas_presents, joueur_t *mecha_tue);

int evolution_mechas(mechas_joueur_t *mecha_joueur);

void montee_niveau(mechas_joueur_t *mecha, int xp_partage, int lvlup);

void distribuer_xp(joueur_t *mechas_presents, int xp_gagne);



#endif
