/**
 * \file sauv.c
 * \brief fichier contenant les fonctions pour gérer les sons du jeux
*/
#include "../lib/initGame.h"
#include "../lib/son.h"

/**
 * @brief Ajuste le volume de la musique et des bruitages selon les paramètres du joueur.
 *
 * Cette fonction met à jour le volume général du jeu (musique et effets sonores)
 * en fonction du pourcentage défini dans `parametres->volume`.
 *
 * @param parametres Un pointeur vers la structure `parametre_t` contenant les réglages de volume du joueur.
 *
 * @pre
 * - Le module SDL_mixer (`Mix_OpenAudio`) doit être initialisé.
 * - Le pointeur `parametres` doit être valide.
 *
 * @post
 * - Le volume de la musique et des effets sonores est ajusté proportionnellement au pourcentage de `volume`.
 * - Si le volume est mis à `0`, tous les bruitages en cours sont arrêtés (`Mix_HaltChannel`).
 *
 * @note
 * - `volume` est un pourcentage entre 0 et 100, converti sur l'échelle de `MIX_MAX_VOLUME`.
 * - `Mix_VolumeMusic` ajuste uniquement la musique, tandis que `Mix_Volume(-1, ...)` ajuste tous les canaux sonores.
 *
 * @warning
 * - Aucun contrôle n'est effectué sur les bornes de `volume`, supposé toujours entre 0 et 100.
 * - Si SDL_mixer n'est pas initialisé, les appels à `Mix_VolumeMusic` ou `Mix_Volume` peuvent échouer silencieusement.
 */
void regler_volume(parametre_t * parametres) {
    int volSon = (parametres->volume * MIX_MAX_VOLUME) / 100;
    Mix_VolumeMusic(volSon);  // Met à jour le volume de la musique
    Mix_Volume(-1, volSon);  // Met à jour le volume des bruitages

    if (parametres->volume == 0) {
        Mix_HaltChannel(-1);  // Arrête tous les bruitages
    }
}

/**
 * @brief Charge et lance la lecture en boucle d'une musique de fond.
 *
 * Cette fonction charge un fichier musical et démarre sa lecture en boucle
 * si aucune musique n'est actuellement en cours de lecture.
 *
 * @param fichierMusique Le chemin vers le fichier musical à charger (format compatible SDL_mixer, ex: `.mp3`, `.ogg`, `.wav`).
 *
 * @pre
 * - SDL_mixer (`Mix_OpenAudio`) doit être initialisé.
 * - Le chemin `fichierMusique` doit être valide et pointant vers un fichier musical existant.
 *
 * @post
 * - La musique est jouée en boucle infinie si elle est correctement chargée et aucune autre musique n'est active.
 *
 * @note
 * - `Mix_PlayingMusic()` est utilisé pour vérifier s'il y a déjà une musique en cours.
 * - Si une musique est déjà en lecture, aucun changement n'est effectué.
 *
 * @warning
 * - En cas d'erreur de chargement (`Mix_LoadMUS`), un message est affiché dans la console via `SDL_LogError`.
 * - Le fichier musique chargé n'est pas libéré dans cette fonction, ce qui peut entraîner une fuite mémoire si relancée plusieurs fois sans gestion.
 */
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

/**
 * @brief Joue un effet sonore ponctuel dans le jeu.
 *
 * Cette fonction charge un fichier sonore (effet type bruitage) et le joue immédiatement
 * sur un canal libre.
 *
 * @param fichierSon Le chemin vers le fichier sonore à charger (format compatible SDL_mixer, ex: `.wav`, `.ogg`).
 *
 * @pre
 * - SDL_mixer (`Mix_OpenAudio`) doit être initialisé.
 * - Le chemin `fichierSon` doit être valide et pointer vers un fichier existant.
 *
 * @post
 * - Le son est joué une seule fois sur le premier canal libre disponible.
 *
 * @note
 * - Utilise `Mix_PlayChannel(-1, sound, 0)` pour choisir automatiquement un canal libre.
 * - La fonction charge et joue le son à chaque appel ; elle ne garde pas le son en mémoire.
 *
 * @warning
 * - Aucun appel à `Mix_FreeChunk` n'est fait après le chargement, ce qui peut entraîner des fuites mémoire
 *   si appelée fréquemment sans gestion externe.
 * - En cas d'échec de chargement (`Mix_LoadWAV`), une erreur est loggée avec `SDL_LogError`.
 */
void jouer_bruit(const char* fichierSon) {  

    Mix_Chunk* sound = Mix_LoadWAV(fichierSon);
    if (!sound) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement son : %s", Mix_GetError());
        return;
    }

    Mix_PlayChannel(-1, sound, 0); 
}


