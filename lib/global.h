#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//*********DEFINE********//
#define LONGUEUR_MAX_PSEUDO 50
#define LONGUEUR_LIGNE 256
#define LONGUEUR_LIGNE_DESC 1070
#define LONGUEUR_DIALOGUE 500
#define LONGUEUR_TYPE 20
#define LONGUEUR_DESC 200

//TAILLE TAB
#define NB_MECHAS_MAX 54
#define NB_MECHAS 24
#define NB_ATTAQUES 64
#define NB_ZONES 10
#define NB_PNJ 24

//INVENTAIRE
#define NB_MECHAS_INVENTAIRE 4
#define NB_OBJET 5

//PNJ IMPORTANT 
#define VIN_GAZOLE_1 17
#define VIN_GAZOLE_2 18
#define VIN_GAZOLE_3 20
#define VIN_GAZOLE_4 21
#define VIN_GAZOLE_5 23

#define IRON_MUSK_DEB 19
#define IRON_MUSK_FIN 22



//RETURN 
#define ERREUR_OUVERTURE -1
#define OK 1
#define ERR 0
#define KO 0
#define RETOUR -1
#define VRAI 1
#define FAUX -2
#define CAPTURE -3
#define FUITE -4

//TAILLE D'UNE CASE
#define PX 32

//COLLISION
#define RIEN 0
#define JOUEUR 1
#define BARRIERE 2
#define BAT 3
#define DECHET 5

//INTERACTION
#define PNJ 6
#define PC 7
#define PORTE 4

//CHANGEMENT MAP

#define TPMAP1 -11
#define TPMAP2 -12
#define TPMAP3 -13
#define TPMAP4 -14
#define TPMAP5 -15
#define TPMAP6 -16

//ZONE SPAWN MECHA
#define Z1 -1
#define Z2 -2
#define Z3 -3
#define Z4 -4
#define Z5 -5
#define Z6 -6
#define Z7 -7
#define Z8 -8
#define Z9 -9
#define Z10 -10 

//DIMENSIONS
#define H 768
#define L 1280

//DIMENSIONS BOUTON
#define LARGEUR_BOUTON 200
#define HAUTEUR_BOUTON 50

//*********CONSTANTES********//
extern const int FRAME_DELAY;


//*********STRUCTURES********//
// Structure qui contient les initialisation de rendu du jeu 
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture[8];
    TTF_Font* police;
    int *** mat ;
    float scale;                        //echelle qui adapte l'affichage à l'ecran et à la taille de l'image
    SDL_Rect dms_win;
    int img_w;
    int img_h;
    int mat_active;
} game_t;

//Structure qui contient les images animés du joueur
typedef struct{
    SDL_Texture * gauche[4];
    SDL_Texture * droite[4];
    SDL_Texture * haut[4];
    SDL_Texture * bas[4];

}img_player_t;
typedef struct{
    SDL_Texture * gauche;
    SDL_Texture * droite;
    SDL_Texture * haut;
    SDL_Texture * bas;
    SDL_Texture * vin_gazole;
    SDL_Texture * iron_musk_bas;
    SDL_Texture * iron_musk_droite;
}img_pnj_t;


//STRUCTURES D'AFFICHAGE
typedef struct {
    SDL_Rect rect;
    SDL_Color couleur;
    char text[50];
    SDL_Color couleur_text;
} rectangle_t;

typedef struct {
    int volume;
} parametre_t;

//STRUCTURES DE DONNEES
typedef struct{
    int id_attaques;
    char nom[LONGUEUR_MAX_PSEUDO];
    char type[LONGUEUR_TYPE];
    int niveau;
    int degats;
    int precision;
    int utilisations;
} attaque_t;

typedef struct{
    SDL_Texture * texture;
    int id_mechas;
    char nom[LONGUEUR_MAX_PSEUDO];
    char type[LONGUEUR_TYPE];
    int evolution;
    int niveau_evolution;
    char description[LONGUEUR_DESC];
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
    char pseudo[LONGUEUR_MAX_PSEUDO];
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
    mechas_joueur_t mechas_joueur [NB_MECHAS_MAX];
    int proba_combat;
} joueur_t;

typedef struct{
    int id_pnj;
    int id_map;
    char pseudo[LONGUEUR_MAX_PSEUDO];
    int etat;
    char dialogueDebut[LONGUEUR_DIALOGUE];
    char dialogueFin[LONGUEUR_DIALOGUE];
    int x, y;
    int orientation;
    int nb_mechas;
    inventaire_t *inventaire;
    mechas_joueur_t mechas_joueur [NB_MECHAS_MAX];
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


//VARIABLES GLOBALE
extern mechas_t mecha[NB_MECHAS];
extern attaque_t attaque[NB_ATTAQUES];
extern zone_t zone[NB_ZONES];
extern pnj_t pnj[NB_PNJ];


#endif