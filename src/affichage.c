#include "../lib/affichage.h"



void draw_obj(game_t *game, SDL_Rect *obj, SDL_Texture * img ) {
    SDL_RenderCopy(game->renderer, img, NULL, obj);
}

void draw_player(game_t *game, SDL_Rect *obj, img_player_t * sprite_playerH, joueur_t * j) {
    int n_img = j->moving/4;
    
    if(j->move_dx<0) {
        SDL_RenderCopy(game->renderer, sprite_playerH->gauche[n_img], NULL, obj);
    }
    else if(j->move_dx>0){
        SDL_RenderCopy(game->renderer, sprite_playerH->droite[n_img], NULL, obj);
    }
    else if(j->move_dy>0){
        SDL_RenderCopy(game->renderer, sprite_playerH->bas[n_img], NULL, obj);
    }
    else if(j->move_dy<0){
        SDL_RenderCopy(game->renderer, sprite_playerH->haut[n_img], NULL, obj);
    }
    else {
        
        switch(j->derniere_touche){
            case 1 :
                SDL_RenderCopy(game->renderer, sprite_playerH->gauche[0], NULL, obj);
                break;
            case 2 : 
                SDL_RenderCopy(game->renderer, sprite_playerH->droite[0], NULL, obj);
                break;
            case 3 :
                SDL_RenderCopy(game->renderer, sprite_playerH->haut[0], NULL, obj);
                break;
            case 4 :
                SDL_RenderCopy(game->renderer, sprite_playerH->bas[0], NULL, obj);
                break;
        }
    }
    
}

SDL_Rect create_obj(game_t * game, int taille_w, int taille_h, int x, int y, int type_obj, int n_mat) {
    if(type_obj == JOUEUR || type_obj == PNJ) {
        game->mat[n_mat][(y+24)/PX][x/PX] = type_obj;
    }
    else {
        game->mat[n_mat][y/PX][x/PX] = type_obj;
    }
    SDL_Rect obj;
    
    obj.w = taille_w * game->scale;
    obj.h = taille_h * game->scale;
    
    obj.x = game->dms_win.x + (x * game->scale);
    obj.y = game->dms_win.y + (y * game->scale);

    return obj;
}



void draw_background(game_t * game) {
    SDL_RenderCopy(game->renderer,game->backgroundTexture[game->mat_active] , NULL, &game->dms_win);                 // Dessiner le background
}

int init_mat(game_t *game, int taille_x, int taille_y) {
    game->mat = malloc(6 * sizeof(int **));
    if (game->mat == NULL) {
        printf("Échec de l'allocation mémoire principale\n");
        return 0;
    }

    for (int i = 0; i < 6; i++) {
        game->mat[i] = malloc(taille_y * sizeof(int *));
        if (game->mat[i] == NULL) {
            printf("Échec de l'allocation mémoire (1) pour mat[%d]\n", i);
            // Libération de la mémoire déjà allouée
            for (int k = 0; k < i; k++) {
                free(game->mat[k]);
            }
            free(game->mat);
            return 0;
        }

        for (int j = 0; j < taille_y; j++) {
            game->mat[i][j] = malloc(taille_x * sizeof(int));
            if (game->mat[i][j] == NULL) {
                printf("Échec de l'allocation mémoire (2) pour mat[%d][%d]\n", i, j);
                // Libération de la mémoire déjà allouée dans mat[i]
                for (int h = 0; h < j; h++) {
                    free(game->mat[i][h]);
                }
                free(game->mat[i]);

                // Libération des matrices précédentes
                for (int k = 0; k < i; k++) {
                    for (int l = 0; l < taille_y; l++) {
                        free(game->mat[k][l]);
                    }
                    free(game->mat[k]);
                }
                free(game->mat);
                return 0;
            }
        }
    }

    return 1;
}

void free_mat(game_t *game, int taille_x, int taille_y) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < taille_y; j++) {
            free(game->mat[i][j]);
        }
        free(game->mat[i]);
    }
    free(game->mat);
}

int remplir_mat(game_t * game, int taille_x, int taille_y) {
    char chemin[100] = "save/map";
    char ext[5] = ".txt";
    for(int i = 0; i < 6; i++) {
        chemin[8] = '0' + i + 1;
        chemin[9] = '\0';
        strcat(chemin, ext);
        FILE *file = fopen(chemin, "r");   
        int n;       
        if (file == NULL) {                                     
            printf("Erreur d'ouverture du fichier matrice");
            return 0;
        }
        for(int j = 0; j < taille_y; j++) {
            for(int h = 0; h < taille_x; h++) {
                fscanf(file, "%d", &n);
                game->mat[i][j][h] = n;
            }
            
        }

        fclose(file);
    }
    
    return 1;
}

void aff_mat(game_t * game, int taille_x, int taille_y, int n_mat)  {
    for(int i = 0; i < taille_y; i++) {
        for(int j = 0;j < taille_x; j++){
            printf("%d", game->mat[n_mat][i][j]);
        }
        printf("\n"); 
    }

    printf("\n\n\n"); 

}


int init_background(game_t * game) {

    char ext[5] = ".png";
    char chemin[100] = "img/background/fond";

    for(int i = 0; i < 6; i++) {
        chemin[19] = '0' + (i + 1);
        chemin[20] = '\0';
        strcat(chemin, ext);
        printf("%s\n", chemin);

        //charger fond
        SDL_Surface * imageSurface = IMG_Load(chemin);
        if (!imageSurface) {
            printf("Erreur de chargement de l'image : %s\n", IMG_GetError());
            IMG_Quit();

            return 0;
        }

        // Convertir la fond en texture
        game->backgroundTexture[i] = SDL_CreateTextureFromSurface(game->renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
        if (!game->backgroundTexture[i]) {
            printf("Erreur de création de la texture : %s\n", SDL_GetError());
            return 0;
        }
    }

    //pour que le fond s'adapte à l'ecran
    //ici
    //chaque fond etant de meme dimension on ne prend que le premier

    SDL_GetRendererOutputSize(game->renderer, &game->dms_win.w, &game->dms_win.h);       //dimensions ecran

    SDL_QueryTexture(game->backgroundTexture[0], NULL, NULL, &game->img_w, &game->img_h);       //dimensions img

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
                case 3 :
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
