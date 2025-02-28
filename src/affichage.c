/**
 * \file affichage.c
 * \brief fichier contenant la fonctions d'affichage utilisant la librérie de SDL
*/
#include "../lib/affichage.h"
#include "../lib/pointDePassage.h"
//FONCTION BACKGROUND
/*=================================================*/

//initalise la background
int init_background(game_t * game) {

    char ext[5] = ".png";
    char chemin[100] = "img/background/fond";

    for(int i = 0; i < 7; i++) {
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



//dessine le background
void draw_background(game_t * game) {
    SDL_RenderCopy(game->renderer,game->backgroundTexture[game->mat_active] , NULL, &game->dms_win);                 // Dessiner le background
}


/*=================================================*/






//FONCTION MAT
/*=================================================*/

//initialise la matrice
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


//rempli les matrices avec les txt
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

//affiche la matrice dans le terminal
void aff_mat(game_t * game, int taille_x, int taille_y, int n_mat)  {
    for(int i = 0; i < taille_y; i++) {
        for(int j = 0;j < taille_x; j++){
            printf("%d\t", game->mat[n_mat][i][j]);
        }
        printf("\n"); 
    }

    printf("\n\n\n"); 

}

//libere la matrice
void free_mat(game_t *game, int taille_x, int taille_y) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < taille_y; j++) {
            free(game->mat[i][j]);
        }
        free(game->mat[i]);
    }
    free(game->mat);
}
/*=================================================*/




//FONCTION GESTION OBJET
/*=================================================*/
//initialise le joueur 
int init_player(game_t * game, img_player_t * sprite_player,char sexe) {
    char c;
    char img_bas[40];
    char img_haut[40];
    char img_gauche[40];
    char img_droite[40];
    if(sexe == 'M'){
        strcpy(img_bas,"img/skin/skin_player_homme/bas");
        strcpy(img_haut,"img/skin/skin_player_homme/haut");
         strcpy(img_gauche,"img/skin/skin_player_homme/gauche");
        strcpy(img_droite,"img/skin/skin_player_homme/droite");
    }
    else{
         strcpy(img_bas,"img/skin/skin_player_femme/bas");
         strcpy(img_haut,"img/skin/skin_player_femme/haut");
         strcpy(img_gauche,"img/skin/skin_player_femme/gauche");
         strcpy(img_droite,"img/skin/skin_player_femme/droite");
    }
    
    char ext[5] = ".png";
    SDL_Surface * imageSprite[16];

    for(int i = 1, j = 0; i < 5; i++) {
        c = '0' + i;
        //BAS
        img_bas[30] = c;
        img_bas[31] = '\0';
        strcat(img_bas, ext);
        //HAUT
        img_haut[31] = c;
        img_haut[32] = '\0';
        strcat(img_haut, ext);              
        //GAUCHE
        img_gauche[33] = c;
        img_gauche[34] = '\0';
        strcat(img_gauche, ext);
        //DROITE
        img_droite[33] = c;
        img_droite[34] = '\0';
        strcat(img_droite, ext);

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
                    sprite_player->bas[i-1] = SDL_CreateTextureFromSurface(game->renderer, imageSprite[j]);
                    if (!sprite_player->bas[i-1]) {
                        printf("Erreur de création de la texture dans init player : %s\n", SDL_GetError());
                        SDL_Quit();
                        return 0;
                    }
                    break;
                case 1 :
                    sprite_player->haut[i-1] = SDL_CreateTextureFromSurface(game->renderer, imageSprite[j]);
                    if (!sprite_player->haut[i-1]) {
                        printf("Erreur de création de la texture dans init player: %s\n", SDL_GetError());
                        SDL_Quit();
                        return 0;
                    }
                    break;
                case 2 :
                    sprite_player->gauche[i-1] = SDL_CreateTextureFromSurface(game->renderer, imageSprite[j]);
                    if (!sprite_player->gauche[i-1]) {
                        printf("Erreur de création de la texture dans init player: %s\n", SDL_GetError());
                        SDL_Quit();
                        return 0;
                    }
                    break;
                case 3 :
                    sprite_player->droite[i-1] = SDL_CreateTextureFromSurface(game->renderer, imageSprite[j]);
                    if (!sprite_player->droite[i-1]) {
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

int init_pnj(game_t * game, img_pnj_t * sprite_pnj) {
    char img_haut[40] = "img/skin/skin_pnj/pnj_1.png";
    char img_droite[40] = "img/skin/skin_pnj/pnj_2.png";
    char img_bas[40] = "img/skin/skin_pnj/pnj_3.png";
    char img_gauche[40] = "img/skin/skin_pnj/pnj_4.png";
    char img_vin_gazole[40] = "img/skin/skin_pnj/vin_gazole.png";
    char img_iron_musk_bas[40] = "img/skin/skin_pnj/iron_musk_bas.png";
    char img_iron_musk_gauche[40] = "img/skin/skin_pnj/iron_musk_gauche.png";
    SDL_Surface * imageSprite[7];

    //charger image
    imageSprite[0] = IMG_Load(img_haut);
    imageSprite[1] = IMG_Load(img_droite);
    imageSprite[2] = IMG_Load(img_bas);
    imageSprite[3] = IMG_Load(img_gauche);
    imageSprite[4] = IMG_Load(img_vin_gazole);
    imageSprite[5] = IMG_Load(img_iron_musk_bas);
    imageSprite[6] = IMG_Load(img_iron_musk_gauche);
    for(int i = 0; i < 7; i++) {
        if (!imageSprite[i]) {
            printf("Erreur de chargement de l'image numero %d dans init pnj: %s\n",i+1, IMG_GetError());
            IMG_Quit();
            return 0;
        }
    }
    sprite_pnj->haut = SDL_CreateTextureFromSurface(game->renderer, imageSprite[0]);
    if (!sprite_pnj->haut) {
        printf("Erreur de création de la texture dans init player : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    sprite_pnj->droite = SDL_CreateTextureFromSurface(game->renderer, imageSprite[1]);
    if (!sprite_pnj->droite) {
        printf("Erreur de création de la texture dans init player: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    sprite_pnj->bas = SDL_CreateTextureFromSurface(game->renderer, imageSprite[2]);
    if (!sprite_pnj->bas) {
        printf("Erreur de création de la texture dans init player: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    sprite_pnj->gauche = SDL_CreateTextureFromSurface(game->renderer, imageSprite[3]);
    if (!sprite_pnj->gauche) {
        printf("Erreur de création de la texture dans init player : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    sprite_pnj->vin_gazole = SDL_CreateTextureFromSurface(game->renderer, imageSprite[4]);
    if (!sprite_pnj->vin_gazole) {
        printf("Erreur de création de la texture dans init player : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    sprite_pnj->iron_musk_bas = SDL_CreateTextureFromSurface(game->renderer, imageSprite[5]);
    if (!sprite_pnj->iron_musk_bas) {
        printf("Erreur de création de la texture dans init player : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    sprite_pnj->iron_musk_droite = SDL_CreateTextureFromSurface(game->renderer, imageSprite[6]);
    if (!sprite_pnj->iron_musk_droite) {
        printf("Erreur de création de la texture dans init player : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    SDL_FreeSurface(imageSprite[0]);
    SDL_FreeSurface(imageSprite[1]);
    SDL_FreeSurface(imageSprite[2]);
    SDL_FreeSurface(imageSprite[3]);
    SDL_FreeSurface(imageSprite[4]);
    SDL_FreeSurface(imageSprite[5]);
    SDL_FreeSurface(imageSprite[6]);
    return 1;
}

//creé un objet
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

//dessine le joueur
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
void draw_all(game_t *game,joueur_t *j,SDL_Rect *sprite_p,SDL_Rect *pnj_sprite, img_pnj_t * sprite_pnj,img_player_t * sprite_playerH){
        draw_background(game);
        if(game->mat[game->mat_active][j->y-1][j->x] == PNJ){
            for(int i = 0; i < 24;i++){
                if((pnj[i].id_map -1) == j->numMap){
                    draw_pnj(game,&pnj_sprite[i],sprite_pnj,&pnj[i],j);
                }
             }
            draw_player(game, sprite_p, sprite_playerH, j);           //dessine le joueur
        }
        else{
            draw_player(game, sprite_p, sprite_playerH, j);           //dessine le joueur 
            for(int i = 0; i < 24;i++){
                if((pnj[i].id_map -1) == j->numMap){
                    draw_pnj(game,&pnj_sprite[i],sprite_pnj,&pnj[i],j);
                }
            }
        }
}

void draw_pnj(game_t *game, SDL_Rect *obj, img_pnj_t * sprite_pnj, pnj_t *pnj, joueur_t *j) { 
    if(!strcmp(pnj->pseudo,"Vin Gazole"))
        SDL_RenderCopy(game->renderer,sprite_pnj->vin_gazole , NULL, obj);

    else if(pnj->id_pnj == 20 && j->pointSauvegarde <= 1)
        SDL_RenderCopy(game->renderer,sprite_pnj->iron_musk_droite , NULL, obj);

    else if(pnj->id_pnj == 23)
        SDL_RenderCopy(game->renderer,sprite_pnj->iron_musk_bas , NULL, obj);

    else if(pnj->id_pnj != 20){
        switch(pnj->orientation){
        case 1 :
            SDL_RenderCopy(game->renderer, sprite_pnj->haut, NULL, obj);
            break;
        case 2 : 
            SDL_RenderCopy(game->renderer, sprite_pnj->droite, NULL, obj);
            break;
        case 3 :
            SDL_RenderCopy(game->renderer, sprite_pnj->bas, NULL, obj);
            break;
        case 4 :
            SDL_RenderCopy(game->renderer, sprite_pnj->gauche, NULL, obj);
            break;
        } 
    }
    
}

//dessine un objet 
void draw_obj(game_t *game, SDL_Rect *obj, SDL_Texture * img ) {
    SDL_RenderCopy(game->renderer, img, NULL, obj);
}

//creer un rectangle avec du texte
void creer_rectangle(game_t *game,rectangle_t *rectangle,int w, int h, float x, float y, int r, int g, int b, int a, char text[50]) { //creer un rectangle avec tt les parametres
    rectangle->rect.x = x ;
    rectangle->rect.y = y ;
    rectangle->rect.w = w ;
    rectangle->rect.h = h ;
    rectangle->couleur.r = r;
    rectangle->couleur.g = g;
    rectangle->couleur.b = b;
    rectangle->couleur.a = a;       //oppacité
    if (text) {
        strncpy(rectangle->text, text, sizeof(rectangle->text) - 1);
        rectangle->text[sizeof(rectangle->text) - 1] = '\0';
    } else {
        rectangle->text[0] = '\0'; // eviter une chaîne non initialisée
    }
 
}

void draw_text(game_t *game, rectangle_t* rectangle) {
    if (rectangle->text[0] == '\0') return; // Vérifier si le texte est valide
    if (!game->police) {
        printf("Erreur : Police non chargée\n");
        return;
    }

    SDL_Color textColor = {0, 0, 0, 255}; 
    SDL_Surface* surface = TTF_RenderText_Solid(game->police, rectangle->text, textColor);
    if (!surface) {
        printf("Erreur SDL_ttf : %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    if (!texture) {
        printf("Erreur SDL : %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    int text_width = surface->w;
    int text_height = surface->h;
    SDL_FreeSurface(surface);

    SDL_Rect textRect = {
        rectangle->rect.x + (rectangle->rect.w - text_width) / 2,
        rectangle->rect.y + (rectangle->rect.h - text_height) / 2,
        text_width,
        text_height
    };

    SDL_RenderCopy(game->renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
}


//dessine un rectangle
void draw_rect(game_t *game, rectangle_t *rectangle) {
        SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(game->renderer, rectangle->couleur.r, rectangle->couleur.g, rectangle->couleur.b, rectangle->couleur.a);
        SDL_RenderFillRect(game->renderer, &rectangle->rect);
        // Dessiner le texte
        if(rectangle->text[0] != '\0') draw_text(game, rectangle);
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);

}


/*=================================================*/


















