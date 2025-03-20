#include "../lib/initGame.h"
#include "../lib/son.h"

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

void jouer_bruit(const char* fichierSon) {  //bruitages
    Mix_Chunk* sound = Mix_LoadWAV(fichierSon);
    if (!sound) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement son : %s", Mix_GetError());
        return;
    }

    int canal = Mix_PlayChannel(-1, sound, 0); 
    if (canal == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur lecture son : %s", Mix_GetError());
    }
}


