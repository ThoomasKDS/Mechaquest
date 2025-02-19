#ifndef MENU_H
#define MENU_H

#include "sauv.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>

#define LARGEUR_BOUTON 200
#define HAUTEUR_BOUTON 50
#define LONGUEUR_MAX_PSEUDO 50

typedef struct {
    int volume;
    int difficulte;
} parametre_t;

typedef struct {
    SDL_Rect rect;
    SDL_Color couleur;
    const char* texte;
} Bouton;

void afficherBouton(SDL_Renderer* renderer, TTF_Font* police, Bouton bouton);

int afficherSaisiePseudo(SDL_Renderer* renderer, TTF_Font* police, char* pseudo);

void afficherParametres(SDL_Renderer* renderer, TTF_Font* police, parametre_t* parametres);

void afficherMenu(SDL_Renderer* renderer, TTF_Font* police, parametre_t* parametres, char* pseudo);

#endif
