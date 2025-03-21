/**
 * @file menu.h
 * @brief Gestion des menus et des interfaces utilisateur.
 * 
 * Ce fichier contient les déclarations des fonctions permettant d'afficher et de gérer
 * les différents menus du jeu, notamment la sélection du sexe, la suppression de données,
 * les paramètres et les dialogues.
 */
#ifndef MENU_H
#define MENU_H

#include "sauv.h"
#include "../lib/initGame.h"
#include "../lib/affichage.h"
#include "../lib/global.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>



SDL_Texture* charger_texture(const char *chemin);

int afficher_choix_sexe(joueur_t* j, char* pseudo);

int afficher_choix_suppression(joueur_t* j,char* pseudo);

int aff_saisie_pseudo(joueur_t* j, char* pseudo);

void afficher_reglage(parametre_t* parametres);

void afficher_menu(parametre_t* parametres, joueur_t* j, char* pseudo);

int afficher_menu_pause(parametre_t* parametres);

void afficher_informations();

void aff_parametre(parametre_t* parametres);


#endif
