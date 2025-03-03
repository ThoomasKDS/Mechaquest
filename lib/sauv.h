#ifndef SAUV_H
#define SAUV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/global.h"

// FONCTIONS D'INITIALISATION : 

int nouveau_fichier_pnj(char pseudo[LONGUEUR_MAX_PSEUDO]);

int init_partie(joueur_t *joueur, char pseudo[LONGUEUR_MAX_PSEUDO],char sexe);

// FONCTIONS DE RECUPERATION : 

int recuperation_joueur(joueur_t *joueur, char pseudo[LONGUEUR_MAX_PSEUDO]);

int recuperation_inventaire(inventaire_t *inventaire, char pseudo[LONGUEUR_MAX_PSEUDO]);

int recuperation_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[LONGUEUR_MAX_PSEUDO]);

int recuperation_attaques(attaque_t *attaques);

int recuperation_mechas(mechas_t *mechas_l);

int recuperation_pnj(pnj_t *pnj,char pseudo[LONGUEUR_MAX_PSEUDO]);

int recuperation_zone(zone_t *zone);



//FONCTIONS DE SAUVEGARDE :

int sauvegarde_partie(joueur_t *joueur, char pseudo[LONGUEUR_MAX_PSEUDO]);

int sauvegarde_inventaire(inventaire_t *inventaire, char pseudo[LONGUEUR_MAX_PSEUDO]);

int sauvegarde_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[LONGUEUR_MAX_PSEUDO],int nb_mechas);

int sauvegarde_pnj(pnj_t *pnj, int id_pnj,char pseudo[LONGUEUR_MAX_PSEUDO]);

//FONCTION DE SUPPRESSION DE SAUVEGARDES

int suppression_partie(joueur_t *joueur, char pseudo[LONGUEUR_MAX_PSEUDO]);

int suppression_inventaire(inventaire_t *inventaire, char pseudo[LONGUEUR_MAX_PSEUDO]);

int suppression_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[LONGUEUR_MAX_PSEUDO],int nb_mechas);

//FONCTIONS DE DESTRUCTION ALLOCATION DYNAMIQUE

int destruction_joueur(joueur_t *joueur);

int destruction_pnj(pnj_t *pnj);

#endif