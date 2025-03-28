/**
 * @file affichage.h
 * @brief Fichier d'en-tête pour la gestion de l'affichage dans le jeu.
 *
 * Ce fichier contient les déclarations des fonctions responsables de l'affichage des éléments du jeu,
 * y compris le background, les matrices et les objets (joueurs, PNJ, objets divers).
 */
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
#include "../lib/combat.h"


//FONCTION BACKGROUND
int init_background();     //initalise la background
int init_calque() ;

void draw_background();    //dessine le background
void draw_calque() ;


//FONCTION MAT
int init_mat(int taille_x, int taille_y);               //initialise la matrice
int remplir_mat(int taille_x, int taille_y);           //rempli les matrices avec les txt
void aff_mat(int taille_x, int taille_y, int n_mat);   //affiche la matrice dans le terminal
void free_mat(int taille_x, int taille_y);              //libere la matrice

//FONCTION GESTION OBJETS
int init_player(char sexe);                                            //initialise le joueur 
int init_pnj() ;             
int init_mecha();                                            //initialise un pnj
SDL_Rect create_obj(int taille_w, int taille_h, int x, int y, int type_obj, int n_mat);      //creé un objet
void game_over(joueur_t *j);
void draw_player(SDL_Rect *obj, joueur_t * j);                 //dessine le joueur
void draw_pnj(SDL_Rect *obj, pnj_t *pnj ,joueur_t *j);                           //dessine un pnj
void draw_obj(SDL_Rect *obj, SDL_Texture * img );                                             //dessine un objet 
void draw_all(joueur_t *j,SDL_Rect *sprite_p,SDL_Rect *pnj_sprite);
//w : largeur, h, hauteur, x, coord x, y : coord y, "r,g,b" :  systeme de couleur, a : oppacité
void creer_rectangle(rectangle_t *rectangle,int w, int h, float x, float y, int r, int g, int b, int a, const char *text); //creer un rectangle avec du texte
void draw_text_left_middle(rectangle_t* rectangle);
void draw_text_center(rectangle_t* rectangle);
void draw_rect(rectangle_t *rectangle,void (*draw_func)(rectangle_t *));
void draw_text_pos(char *text, int x, int y);
void draw_all_rect(int n, ...);

int afficher_dialogue(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, char *pseudo, char *dialogue,int choix);
int afficher_dialogue_combat(mechas_joueur_t * mecha_joueur, mechas_joueur_t * mecha_ordi, char *pseudo, char *dialogue,int choix);

#endif
