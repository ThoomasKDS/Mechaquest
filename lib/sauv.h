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
    char description[200];
    int liste_attaque[10];
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
} mechas_joueur_t;

typedef struct{
    char pseudo[50];
    char sexe;
    int x, y;
    float screen_x, screen_y;       // Position réelle à l'écran 
    int moving;                     // Indique si un déplacement est en cours
    float move_dx, move_dy;           // Direction du mouvement en pixels
    int pointSauvegarde;
    int nb_mechas;
    Inventaire *inventaire;
    Mechas_Joueur mechas_joueur [50];
} joueur_t;


//FONCTIONS

int RecuperationJoueur(joueur_t *joueur, char pseudo[50]);

int RecuperationInventaire(inventaire_t *inventaire, char pseudo[50]);

int RecuperationAttaques(attaque_t *attaques);

int RecuperationMechasJoueur(mechas_joueur_t * mechas_joueur,char pseudo[50]);

int RecuperationMechas(mechas_t *mechas_l);

#endif