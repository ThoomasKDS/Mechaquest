/**
 * \file initGame.c
 * \brief fichier contenant la fonctions d'initialisation du Jeux
*/

#include "../lib/initGame.h"


/**
 * @brief Initialise l'environnement du jeu en préparant SDL et ses modules associés.
 *
 * Cette fonction initialise SDL, SDL_ttf, SDL_image et SDL_mixer pour gérer l'affichage graphique, les polices, les images et le son.
 * Elle crée une fenêtre principale en mode plein écran ainsi qu'un renderer pour gérer l'affichage.
 * Une police spécifique est également chargée pour le rendu des textes dans le jeu.
 *
 * @param game  Pointeur vers une structure `game_t` où seront stockées les ressources créées (fenêtre, renderer, police).
 *
 * @return int
 *         - `1` si l'initialisation complète réussit.
 *         - `0` en cas d'erreur dans l'initialisation de SDL, SDL_image, SDL_mixer, ou à la création de la fenêtre/renderer.
 *         - `1` si l'erreur provient de SDL_ttf (ce cas particulier peut être une incohérence et devrait être vérifié dans l'implémentation).
 *
 * @pre Le pointeur `game` doit être valide et non NULL.
 * @post La structure `game` est remplie avec les ressources initialisées. En cas d'erreur, toutes les ressources allouées précédemment sont libérées.
 *
 * @note La fonction initialise également la génération aléatoire (`rand()`).
 *
 * @warning Le chemin vers le fichier de police `"img/police_temporaire.ttf"` doit être valide, sinon l'initialisation échoue.
 */
int init_game() {
    srand(time(NULL)); //initialise de la lib rand
    
    SDL_Rect display_bounds; //struture contenant dimensions de l'ecran
    int display_index = 0; //choix de l'ecran 0 pour le principal etc

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL n'a pas pu être initialisé.\n");
        return 0;
    }

    // Initialisation de l'affichage de l'écriture avec TTF
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Initialiser SDL_image pour charger des images
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Erreur d'initialisation de SDL_image.\n");
        SDL_Quit();
        return 0;
    }

    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG)) {
    printf("Erreur d'initialisation de SDL_mixer: %s\n", Mix_GetError());
    SDL_Quit();
    return 0;
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) == -1) {
        printf("Erreur d'ouverture du périphérique audio: %s\n", Mix_GetError());
        Mix_Quit();
        SDL_Quit();
        return 0;
    }

    if (SDL_GetDisplayBounds(display_index, &display_bounds) != 0) {
        printf("Erreur lors de l'obtention des dimensions de l'écran.\n");
        SDL_Quit();
        return 0;
    }

    // Création de la fenêtre
    game.window = SDL_CreateWindow(
            "Mechaquest",
            display_bounds.x,
            display_bounds.y,
            L,
            H,
            SDL_WINDOW_SHOWN
    );
    if (!game.window) {
        printf("Erreur de création de la fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    if (SDL_SetWindowFullscreen(game.window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) {     //pleine ecran
        printf("Erreur lors du passage en plein écran : %s\n", SDL_GetError());
    }

    // Création du renderer
    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
    if (!game.renderer) {
        printf("Erreur de création du renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(game.window);
        SDL_Quit();
        return 0;
    }

    // Création de la police
    game.police = TTF_OpenFont("police/police_temporaire.ttf", 19);
    if (!game.police) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
        SDL_DestroyRenderer(game.renderer);
        SDL_DestroyWindow(game.window);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }
    

 
    

    return 1;
}


/**
 * @brief Libère proprement toutes les ressources utilisées par le jeu avant de quitter.
 *
 * Cette fonction détruit les textures des arrière-plans, le renderer, la fenêtre, la police chargée,
 * et quitte correctement les modules SDL (SDL_ttf, SDL_image, SDL).
 *
 * @param game  Pointeur vers une structure `game_t` contenant toutes les ressources à libérer.
 *
 * @pre Le pointeur `game` doit être valide et initialisé, ainsi que toutes les ressources associées (textures, renderer, fenêtre, police).
 * @post Toutes les ressources allouées dynamiquement par SDL sont libérées, et SDL est correctement quitté.
 *
 * @note Cette fonction doit être appelée systématiquement avant la fermeture de l'application pour éviter les fuites mémoire.
 */
 void cleanUp() {
    for (int i = 0; i < 10; i++) {
        if (game.backgroundTexture[i]) {
            SDL_DestroyTexture(game.backgroundTexture[i]);
            game.backgroundTexture[i] = NULL;
        }
    }

    for (int i = 0; i < 6; i++) {
        if (game.calqueTexture[i]) {
            SDL_DestroyTexture(game.calqueTexture[i]);
            game.calqueTexture[i] = NULL;
        }
    }

    if (game.renderer) {
        SDL_DestroyRenderer(game.renderer);
        game.renderer = NULL;
    }

    if (game.window) {
        SDL_DestroyWindow(game.window);
        game.window = NULL;
    }

    if (game.police) {
        TTF_CloseFont(game.police);
        game.police = NULL;
    }

    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
