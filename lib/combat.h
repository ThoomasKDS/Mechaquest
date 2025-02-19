#ifndef COMBAT_H
#define COMBAT_H
#include "sauv.h"

extern mechas_t mecha[24];
extern attaque_t attaque[64];
extern zone_t zone[10];
extern pnj_t pnj;

void affichage_mecha(mechas_joueur_t tab_mecha[], mechas_t mecha[], char nom[]);

int utilisation_objet(joueur_t *joueur, int i);

int changer_mecha(joueur_t *joueur, mechas_t mecha[], char nom[], int etat);

int attaque_joueur(mechas_joueur_t *mecha_joueur, mechas_joueur_t *mecha_ordi, attaque_t *attaque, mechas_t *mecha);

int choix_action(char nom[], int i);

int apprentissage_attaque(mechas_joueur_t *mecha_joueur);

int choix_mecha();

void level_mechas(joueur_t *mechas_presents, joueur_t *mecha_tue);

//void combat_init();

int tour_jeu(joueur_t *joueur, joueur_t *mecha_joueur, joueur_t *mecha_ordi);


int evolution_mechas(mechas_joueur_t *mecha_joueur);

void montee_niveau(mechas_joueur_t *mecha, int xp_partage, int lvlup);

void distribuer_xp(joueur_t *mechas_presents, int xp_gagne);

#endif
