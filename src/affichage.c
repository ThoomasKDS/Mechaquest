#include "../lib/affichage.h"

void draw_obj(game_t * game, SDL_Rect * obj) {
    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255); // Rouge
    SDL_RenderFillRect(game->renderer, obj); // Dessiner le rectangle du joueur
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255); // remet la couleur en noir pour le fond
}

SDL_Rect create_obj(game_t * game, int taille_w, int taille_h, int x, int y, case_t ** mat, int type_obj) {
    SDL_Rect obj;
    mat[y/PX][x/PX].obj = type_obj;

    obj.w = taille_w * game->scale;
    obj.h = taille_h * game->scale;
    
    obj.x = game->dms_win.x + (x * game->scale);
    obj.y = game->dms_win.y + (y * game->scale);

    return obj;
}

int init_background(const char * img, game_t * game) {

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

    //calcule l'echelle pour dessiner les futurs objets
    float scale_x = (float)game->dms_win.w/game->img_w; 
    float scale_y = (float)game->dms_win.h/game->img_h;

    game->scale = (scale_x < scale_y) ? scale_x : scale_y; //prend l'echelle la plus petite

    return 1;
}

void draw_background(game_t * game) {
    SDL_RenderCopy(game->renderer,game->backgroundTexture , NULL, &game->dms_win);                 // Dessiner le background
}

int init_mat(case_t *** mat, int taille_x, int taille_y) {
    *mat = malloc(taille_y * sizeof(case_t *));
    if(*mat == NULL) {
        printf("Echec de l'allocation memoire (1)\n");
        return 0;
    }
    for(int i = 0; i < taille_y; i++) {
        (*mat)[i] = malloc(taille_x * sizeof(case_t));
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

void remplir_mat(case_t ** mat, int taille_x, int taille_y) {
    //printf("%d  %d\n", game.dms_win.x, game.dms_win.y);
    for (int i = 0; i < taille_y; i++) {
        for (int j = 0; j < taille_x; j++) {
            mat[i][j].x = (j * PX);       
            mat[i][j].y = (i * PX);
            mat[i][j].obj = 0;
        }
    }
}

void aff_mat(case_t ** mat, int taille_x, int taille_y)  {
    for(int i = 0; i < taille_y; i++) {
        for(int j = 0;j < taille_x; j++){
            //printf("x = %d, y = %d", mat[i][j].x, mat[i][j].y);
            printf("%d ", mat[i][j].obj);
        }
        printf("\n"); 
    }

    printf("\n\n\n"); 

}