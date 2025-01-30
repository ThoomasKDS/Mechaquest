#ifndef COMBAT_H
#define COMBAT_H

#include "mecha.h"

void liste_mecha(mecha_t tab_mecha[]);
int choix_mecha();
int tour_jeu(int i, int j, mecha_t tab_mecha[]);
void level_mechas(int gagnant, int perdant, mecha_t tab_mecha[]);
void combat_init();

#endif
