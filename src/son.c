#include "../lib/initGame.h"
#include "../lib/son.h"

void regler_volume(parametre_t * parametres) {
    int volSon = (parametres->volume * MIX_MAX_VOLUME) / 100;
    Mix_VolumeMusic(volSon);  // Met à jour le volume de la musique
    Mix_Volume(-1, volSon);  // Met à jour le volume des bruitages

    if (parametres->volume == 0) {
        Mix_HaltChannel(-1);  // Arrête tous les bruitages
    }
}

void lancer_musique(const char* fichierMusique) {   //musique generale
    Mix_Music* musique = Mix_LoadMUS(fichierMusique);
    if (!musique) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement musique : %s", Mix_GetError());
        return;
    }

    if (Mix_PlayingMusic() == 0) {  
        Mix_PlayMusic(musique, -1); //-1 = en boucle
    }
}

void jouer_bruit(const char* fichierSon) {  

    Mix_Chunk* sound = Mix_LoadWAV(fichierSon);
    if (!sound) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement son : %s", Mix_GetError());
        return;
    }

    Mix_PlayChannel(-1, sound, 0); 
}


