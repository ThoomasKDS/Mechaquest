#ifndef SAUV_H
#define SAUV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//CONSTANTES
#define ERREUR_OUVERTURE -1
#define OK 1
#define ERR 0


//STRUCTURE DE DONNEES//

typedef struct{
    int id_attaques;
    char nom[50];
    char type[20];
    int niveau;
    int degats;
    int precision;
    int utilisations;
} attaque_t;

typedef struct{
    int id_mechas;
    char nom[50];
    char type[20];
    int evolution;
    int niveau_evolution;
    char description[200];
    int liste_attaque[5];
    int nb_attaques;
} mechas_t;

typedef struct{
    int mechaball;
    int carburant;
    int repousse;
    int rappel;
} inventaire_t;

typedef struct{
    int numero;
    int id_mechas;
    int niveau;
    int xp;
    int pv;
    int pv_max;
    int attaque;
    int defense;
    int vitesse;
    int attaque_1;
    int attaque_2;
    int utilisation_1;
    int utilisation_2;
} mechas_joueur_t;

typedef struct{
    char pseudo[50];
    char sexe;
    int numMap;
    int x, y;
    float screen_x, screen_y;       // Position réelle à l'écran 
    int moving;                     // Indique si un déplacement est en cours
    float move_dx, move_dy;           // Direction du mouvement en pixels
    int derniere_touche;
    int pointSauvegarde;
    int nb_mechas;
    inventaire_t *inventaire;
    mechas_joueur_t mechas_joueur [50];
    int proba_combat;
} joueur_t;

typedef struct{
    int id_pnj;
    int id_map;
    char pseudo[50];
    int etat;
    char dialogueDebut[500];
    char dialogueFin[500];
    int x, y;
    int orientation;
    int nb_mechas;
    inventaire_t *inventaire;
    mechas_joueur_t mechas_joueur [50];
} pnj_t;

typedef struct{
    int id_zone;
    int NiveauMoyenApparition;
    int PvMoyen;
    int VitesseMoyenne;
    int Attaque;
    int Defense;
    int listeMechasZone[10];
    int nb_mechas;
}zone_t;

// FONCTIONS DE RECUPERATION : 

int recuperation_joueur(joueur_t *joueur, char pseudo[50]);

int recuperation_inventaire(inventaire_t *inventaire, char pseudo[50]);

int recuperation_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[50]);

int recuperation_attaques(attaque_t *attaques);

int recuperation_mechas(mechas_t *mechas_l);

int recuperation_pnj(pnj_t *pnj, int id_pnj);

int recuperation_zone(zone_t *zone);



//FONCTIONS DE SAUVEGARDE :

int sauvegarde_partie(joueur_t *joueur, char pseudo[50]);

int sauvegarde_inventaire(inventaire_t *inventaire, char pseudo[50]);

int sauvegarde_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[50],int nb_mechas);

int sauvegarde_pnj(pnj_t *pnj, int id_pnj);

//FONCTION DE SUPPRESSION DE SAUVEGARDES

int suppression_partie(joueur_t *joueur, char pseudo[50]);

int suppression_inventaire(inventaire_t *inventaire, char pseudo[50]);

int suppression_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[50],int nb_mechas);

//FONCTIONS DE DESTRUCTION ALLOCATION DYNAMIQUE

int destruction_joueur(joueur_t *joueur);

int destruction_pnj(pnj_t *pnj);

#endif