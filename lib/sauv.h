#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//CONSTANTES
#define ERREUR_OUVERTURE -1
#define OK 1
#define ERR 0


//STRUCTURE DE DONNEES//

typedef struct {
    int id_attaques;
    char nom[50];
    char type[20];
    int niveau;
    int degats;
    int precision;
    int utilisations;
} Attaque;

typedef struct {
    int id_mechas;
    char nom[50];
    char type[20];
    int evolution;
    char description[200];
    int liste_attaque[10];
    int nb_attaques;
} Mechas;

typedef struct {
    int mechaball;
    int carburant;
    int repousse;
    int rappel;
} Inventaire;

typedef struct {
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
} Mechas_Joueur;

typedef struct {
    char pseudo[50];
    char sexe;
    int x;
    int y;
    int pointSauvegarde;
    Inventaire *inventaire;
} Joueur;



//FONCTIONS

int RecuperationJoueur(Joueur *joueur, char pseudo[50]);

int RecuperationInventaire(Inventaire *inventaire, char pseudo[50]);

int RecuperationAttaques(Attaque *attaques);
