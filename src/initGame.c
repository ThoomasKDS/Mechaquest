#include "../lib/initGame.h"



int init_game(game_t* game) {

    SDL_Rect display_bounds; //struture contenant dimensions de l'ecran
    int display_index = 0; //choix de l'ecran 0 pour le principal etc

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL n'a pas pu être initialisé.\n");
        return 0;
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
    game->window = SDL_CreateWindow(
            "Mechaquest",
            display_bounds.x,
            display_bounds.y,
            L,
            H,
            SDL_WINDOW_SHOWN
    );
    if (!game->window) {
        printf("Erreur de création de la fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    if (SDL_SetWindowFullscreen(game->window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) {     //pleine ecran
        printf("Erreur lors du passage en plein écran : %s\n", SDL_GetError());
    }

    // Création du renderer
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (!game->renderer) {
        printf("Erreur de création du renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        return 0;
    }

    

 
    

    return 1;
}

// Libération des ressources et fermeture SDL
void cleanUp(game_t* game) {
    SDL_DestroyTexture(game->backgroundTexture);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    IMG_Quit();
    SDL_Quit();
}

int init_background(char * img, game_t * game) {

    //charger fond
    SDL_Surface* imageSurface = IMG_Load(img);
    if (!imageSurface) {
        printf("Erreur de chargement de l'image : %s\n", IMG_GetError());
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        IMG_Quit();
        SDL_Quit();
        return 0;
    }

    // Convertir la fond en texture
    game->backgroundTexture = SDL_CreateTextureFromSurface(game->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!game->backgroundTexture) {
        printf("Erreur de création de la texture : %s\n", SDL_GetError());
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        return 0;
    }

    //pour que le fond s'adapte à l'ecran
    //ici
    SDL_GetRendererOutputSize(game->renderer, &game->dms_win.w, &game->dms_win.h);       //dimensions ecran

    SDL_QueryTexture(game->backgroundTexture, NULL, NULL, &game->img_w, &game->img_h);       //dimensions img

    float img_ratio = (float)game->img_w / (float)game->img_h;
    float win_ratio = (float)game->dms_win.w / (float)game->dms_win.h;

    if (win_ratio > img_ratio) {
        // L'écran est plus large que l'image
        int new_w = game->dms_win.h * img_ratio;
        game->dms_win.x = (game->dms_win.w - new_w) / 2; // centrage horizontal
        game->dms_win.w = new_w;
        game->dms_win.y = 0;
    } else {
        // L'écran est plus haut que l'image
        int new_h = game->dms_win.w / img_ratio;
        game->dms_win.y = (game->dms_win.h - new_h) / 2; // centrage vertical
        game->dms_win.h = new_h;
        game->dms_win.x = 0;
    }
    //a ici

    return 1;
}

void draw_background(game_t game) {
    SDL_RenderCopy(game.renderer,game.backgroundTexture , NULL, &game.dms_win);                 // Dessiner le background
}

int init_mat(game_t game, int *** mat) {
    int nb_case_x = game.img_w / 32;
    int nb_case_y = game.img_h / 32;
    *mat = malloc(nb_case_y * sizeof(int *));
    if(*mat == NULL) {
        printf("Echec de l'allocation memoire (1)\n");
        return 0;
    }
    for(int i = 0; i < nb_case_y; i++) {
        (*mat)[i] = malloc(nb_case_x * sizeof(int));
        if ((*mat)[i] == NULL) {
            printf("Échec de l'allocation de mémoire (2)\n");
            for (int j = 0; j < i; j++) {
                free((*mat)[j]);                   //en cas d'echec libere la memoire deja alloué
            }
            free(*mat);                          //de meme
            return 0;
        }
    }

    return 1;

}