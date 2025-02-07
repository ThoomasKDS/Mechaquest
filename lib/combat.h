#ifndef COMBAT_H
#define COMBAT_H

#include "sauv.h"

void affiche_mecha(mechas_joueur_t tab_mecha[], mechas_t mecha[]);
int choix_mecha();
int tour_jeu(int i, int j, mechas_joueur_t tab_mecha[], mechas_t mecha[]);
void level_mechas(int gagnant, int perdant, mechas_joueur_t tab_mecha[], mechas_t mecha[]);
void combat_init();

#endif
