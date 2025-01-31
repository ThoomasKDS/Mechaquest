#include "../lib/affichage.h"

void draw_obj(game_t *game, SDL_Rect *obj, SDL_Texture *imageTexture) {
    SDL_RenderCopy(game->renderer, imageTexture, NULL, obj);
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

SDL_Rect create_player(game_t * game, int taille_w, int taille_h, int x, int y, case_t ** mat, int type_obj) {
    SDL_Rect obj;
    mat[y/PX][x/PX].obj = type_obj;

    obj.w = taille_w * game->scale;
    obj.h = taille_h * game->scale;
    
    obj.x = game->dms_win.x + (x * game->scale);
    obj.y = (game->dms_win.y + (y * game->scale));

    return obj;
}

int init_background(const char * img, game_t * game) {

    //charger fond
    SDL_Surface * imageSurface = IMG_Load(img);
    if (!imageSurface) {
        printf("Erreur de chargement de l'image : %s\n", IMG_GetError());
        IMG_Quit();

        return 0;
    }

    // Convertir la fond en texture
    game->backgroundTexture = SDL_CreateTextureFromSurface(game->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!game->backgroundTexture) {
        printf("Erreur de création de la texture : %s\n", SDL_GetError());
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

int init_player_h(game_t * game, img_player_t * sprite_playerH) {
    char c;
    char img_bas[40] = "img/skin/skin_player_homme/bas";
    char img_haut[40] = "img/skin/skin_player_homme/haut";
    char img_gauche[40] = "img/skin/skin_player_homme/gauche";
    char img_droite[40] = "img/skin/skin_player_homme/droite";
    char ext[5] = ".png";
    SDL_Surface * imageSprite[16];

    for(int i = 1, j = 0; i < 5; i++) {
        c = '0' + i;
        //BAS
        img_bas[30] = c;
        img_bas[31] = '\0';
        strcat(img_bas, ext);
        printf("%s\n", img_bas);
        //HAUT
        img_haut[31] = c;
        img_haut[32] = '\0';
        strcat(img_haut, ext);              
        printf("%s\n", img_haut);
        //GAUCHE
        img_gauche[33] = c;
        img_gauche[34] = '\0';
        strcat(img_gauche, ext);
        printf("%s\n", img_gauche);
        //DROITE
        img_droite[33] = c;
        img_droite[34] = '\0';
        strcat(img_droite, ext);
         printf("%s\n", img_droite);

        //charger image
        imageSprite[j++] = IMG_Load(img_bas);
        imageSprite[j++] = IMG_Load(img_haut);
        imageSprite[j++] = IMG_Load(img_gauche);
        imageSprite[j++] = IMG_Load(img_droite);
        j -= 4;

        for(int h = 0; h < 4; h++) {

            if (!imageSprite[j]) {
                
                printf("Erreur de chargement de l'image dans init player: %s\n", IMG_GetError());
                IMG_Quit();
                return 0;
            }

            // Convertir l'image en texture en fonction de sa direction
            switch(h) {
                case 0 :
                    sprite_playerH->bas[i-1] = SDL_CreateTextureFromSurface(game->renderer, imageSprite[j]);
                    if (!sprite_playerH->bas[i-1]) {
                        printf("Erreur de création de la texture dans init player : %s\n", SDL_GetError());
                        SDL_Quit();
                        return 0;
                    }
                    break;
                case 1 :
                    sprite_playerH->haut[i-1] = SDL_CreateTextureFromSurface(game->renderer, imageSprite[j]);
                    if (!sprite_playerH->haut[i-1]) {
                        printf("Erreur de création de la texture dans init player: %s\n", SDL_GetError());
                        SDL_Quit();
                        return 0;
                    }
                    break;
                case 2 :
                    sprite_playerH->gauche[i-1] = SDL_CreateTextureFromSurface(game->renderer, imageSprite[j]);
                    if (!sprite_playerH->gauche[i-1]) {
                        printf("Erreur de création de la texture dans init player: %s\n", SDL_GetError());
                        SDL_Quit();
                        return 0;
                    }
                    break;
                case 4 :
                    sprite_playerH->droite[i-1] = SDL_CreateTextureFromSurface(game->renderer, imageSprite[j]);
                    if (!sprite_playerH->droite[i-1]) {
                        printf("Erreur de création de la texture dans init player : %s\n", SDL_GetError());
                        SDL_Quit();
                        return 0;
                    }
                    break;
            }

            SDL_FreeSurface(imageSprite[j++]);

            

        }


    }

    return 1;
}
