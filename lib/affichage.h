#ifndef AFF_H
#define AFF_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdarg.h>
#include "../lib/player.h"
#include "../lib/initGame.h"
#include "../lib/sauv.h"
#include "../lib/global.h"


//FONCTION BACKGROUND
int init_background(game_t * game);     //initalise la background
int init_calque(game_t * game) ;

void draw_background(game_t * game);    //dessine le background
void draw_calque(game_t * game) ;


//FONCTION MAT
int init_mat(game_t *game, int taille_x, int taille_y);               //initialise la matrice
int remplir_mat(game_t * game, int taille_x, int taille_y);           //rempli les matrices avec les txt
void aff_mat(game_t * game, int taille_x, int taille_y, int n_mat);   //affiche la matrice dans le terminal
void free_mat(game_t *game, int taille_x, int taille_y);              //libere la matrice

//FONCTION GESTION OBJETS
int init_player(game_t * game, img_player_t * sprite_player,char sexe);                                            //initialise le joueur 
int init_pnj(game_t * game, img_pnj_t * sprite_pnj) ;             
int init_mecha(game_t *game, mechas_t *mecha);                                            //initialise un pnj
SDL_Rect create_obj(game_t * game, int taille_w, int taille_h, int x, int y, int type_obj, int n_mat);      //creé un objet
void draw_player(game_t *game, SDL_Rect *obj, img_player_t * sprite_playerH, joueur_t * j);                 //dessine le joueur
void draw_pnj(game_t *game, SDL_Rect *obj, img_pnj_t * sprite_pnj, pnj_t * pnj, joueur_t *j);                           //dessine un pnj
void draw_obj(game_t *game, SDL_Rect *obj, SDL_Texture * img );                                             //dessine un objet 
void draw_all(game_t *game,joueur_t *j,SDL_Rect *sprite_p,SDL_Rect *pnj_sprite, img_pnj_t * sprite_pnj,img_player_t * sprite_playerH);
//w : largeur, h, hauteur, x, coord x, y : coord y, "r,g,b" :  systeme de couleur, a : oppacité
void creer_rectangle(rectangle_t *rectangle,int w, int h, float x, float y, int r, int g, int b, int a, char text[500]); //creer un rectangle avec du texte
void draw_text_left_middle(game_t *game, rectangle_t* rectangle);
void draw_text_center(game_t *game, rectangle_t* rectangle);
void draw_rect(game_t *game, rectangle_t *rectangle,void (*draw_func)(game_t *, rectangle_t *));
void draw_text_pos(game_t  *game, char *text, int x, int y);
void draw_all_rect(game_t *game, int n, ...);

int afficher_dialogue(game_t *game, joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, img_pnj_t * sprite_pnj, img_player_t * sprite_playerH, char *pseudo, char *dialogue,int choix);


#endif
