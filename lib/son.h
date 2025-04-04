/**
 * @file son.h
 * @brief Gestion de la musique et des effets sonore du jeu.
 * 
 * Ce fichier contient les déclarations des fonctions permettant de lancer une musique,
 * faire des bruitage à partir des paramètres
 */
#include "../lib/global.h"

void regler_volume(parametre_t * parametres);

void lancer_musique(const char* fichierMusique);

void jouer_bruit(const char* fichierSon);
