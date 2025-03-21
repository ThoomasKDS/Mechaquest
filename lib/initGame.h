/**
 * @file initGame.h
 * @brief Initialisation et gestion de la SDL pour le jeu.
 * 
 * Ce fichier contient les déclarations des fonctions permettant d'initialiser et de libérer
 * les ressources de la SDL, incluant SDL_Mixer, SDL_Image et SDL_TTF.
 */
#ifndef INIT_SDL_H
#define INIT_SDL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/global.h"

//Initialisation de sdl 
//Initialisation de sdl mixer
//Initialisation de sdl image
int init_game();
// Libération des ressources et fermeture SDL
void cleanUp(); 


#endif
