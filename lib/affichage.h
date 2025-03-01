#ifndef AFF_H
#define AFF_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include "../lib/player.h"
#include "../lib/initGame.h"
#include "../lib/sauv.h"


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

extern const int FRAME_DELAY;

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

typedef struct {
    SDL_Rect rect;
    SDL_Color couleur;
    char text[50];
    SDL_Color couleur_text;
} rectangle_t;

//FONCTION BACKGROUND
int init_background(game_t * game);     //initalise la background
void draw_background(game_t * game);    //dessine le background

//FONCTION MAT
int init_mat(game_t *game, int taille_x, int taille_y);               //initialise la matrice
int remplir_mat(game_t * game, int taille_x, int taille_y);           //rempli les matrices avec les txt
void aff_mat(game_t * game, int taille_x, int taille_y, int n_mat);   //affiche la matrice dans le terminal
void free_mat(game_t *game, int taille_x, int taille_y);              //libere la matrice

//FONCTION GESTION OBJETS
int init_player(game_t * game, img_player_t * sprite_player,char sexe);                                            //initialise le joueur 
int init_pnj(game_t * game, img_pnj_t * sprite_pnj) ;                                                         //initialise un pnj
SDL_Rect create_obj(game_t * game, int taille_w, int taille_h, int x, int y, int type_obj, int n_mat);      //creé un objet
void draw_player(game_t *game, SDL_Rect *obj, img_player_t * sprite_playerH, joueur_t * j);                 //dessine le joueur
void draw_pnj(game_t *game, SDL_Rect *obj, img_pnj_t * sprite_pnj, pnj_t * pnj, joueur_t *j);                           //dessine un pnj
void draw_obj(game_t *game, SDL_Rect *obj, SDL_Texture * img );                                             //dessine un objet 
void draw_all(game_t *game,joueur_t *j,SDL_Rect *sprite_p,SDL_Rect *pnj_sprite, img_pnj_t * sprite_pnj,img_player_t * sprite_playerH);
//w : largeur, h, hauteur, x, coord x, y : coord y, "r,g,b" :  systeme de couleur, a : oppacité
void creer_rectangle(game_t *game,rectangle_t *rectangle,int w, int h, float x, float y, int r, int g, int b, int a, char text[50]); //creer un rectangle avec du texte
void draw_text(game_t *game, rectangle_t* rectangle);
void draw_rect(game_t *game, rectangle_t *rectangle);

#endif