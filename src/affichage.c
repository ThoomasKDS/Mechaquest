/**
 * \file affichage.c
 * \brief fichier contenant la fonctions d'affichage utilisant la librérie de SDL
*/
#include "../lib/affichage.h"
#include "../lib/pointDePassage.h"
//FONCTION BACKGROUND
/*=================================================*/

/**
 * @brief Initialise et charge les arrière-plans du jeu.
 * 
 * Cette fonction charge huit images d'arrière-plan depuis le dossier 
 * "img/background" et les convertit en textures SDL. Elle ajuste également 
 * la taille de l'affichage pour s'adapter à l'écran du joueur.
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer et les textures.
 * @return Retourne 1 en cas de succès, 0 en cas d'échec du chargement des textures.
 */
int init_background() {

    char chemin[40];

    for(int i = 0; i < 10; i++) {
        sprintf(chemin,"img/background/fond%d.png",i+1);
        //charger fond
        SDL_Surface * imageSurface = IMG_Load(chemin);
        if (!imageSurface) {
            printf("Erreur de chargement de l'image : %s\n", IMG_GetError());
            IMG_Quit();

            return 0;
        }

        // Convertir la fond en texture
        game.backgroundTexture[i] = SDL_CreateTextureFromSurface(game.renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
        if (!game.backgroundTexture[i]) {
            printf("Erreur de création de la texture : %s\n", SDL_GetError());
            return 0;
        }
    }

    //pour que le fond s'adapte à l'ecran
    //ici
    //chaque fond etant de meme dimension on ne prend que le premier

    SDL_GetRendererOutputSize(game.renderer, &game.dms_win.w, &game.dms_win.h);       //dimensions ecran

    SDL_QueryTexture(game.backgroundTexture[0], NULL, NULL, &game.img_w, &game.img_h);       //dimensions img

    float img_ratio = (float)game.img_w / (float)game.img_h;
    float win_ratio = (float)game.dms_win.w / (float)game.dms_win.h;

    if (win_ratio > img_ratio) {
        // L'écran est plus large que l'image
        int new_w = game.dms_win.h * img_ratio;
        game.dms_win.x = (game.dms_win.w - new_w) / 2; // centrage horizontal
        game.dms_win.w = new_w;
        game.dms_win.y = 0;
    } else {
        // L'écran est plus haut que l'image
        int new_h = game.dms_win.w / img_ratio;
        game.dms_win.y = (game.dms_win.h - new_h) / 2; // centrage vertical
        game.dms_win.h = new_h;
        game.dms_win.x = 0;
    }
    //a ici

    //calcule l'echelle pour dessiner les futurs objets
    float scale_x = (float)game.dms_win.w/game.img_w; 
    float scale_y = (float)game.dms_win.h/game.img_h;

    game.scale = (scale_x < scale_y) ? scale_x : scale_y; //prend l'echelle la plus petite
    


    return 1;
}

/**
 * @brief Initialise et charge les textures des calques du jeu.
 * 
 * Cette fonction charge deux images de calque depuis le dossier 
 * "img/calque" et les convertit en textures SDL pour l'affichage.
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer et les textures de calque.
 * @return Retourne OK (1) en cas de succès, 0 en cas d'échec du chargement des textures.
 */
int init_calque() {

    char ext[5] = ".png";
    char chemin[30] = "img/calque/calque";

    for(int i = 0; i < 6; i++) {
        chemin[17] = '0' + (i + 1);
        chemin[18] = '\0';
        strcat(chemin, ext);

        //charger fond
        SDL_Surface * imageSurface = IMG_Load(chemin);
        if (!imageSurface) {
            printf("Erreur de chargement de l'image : %s\n", IMG_GetError());
            IMG_Quit();

            return 0;
        }

        // Convertir la fond en texture
        game.calqueTexture[i] = SDL_CreateTextureFromSurface(game.renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
        if (!game.calqueTexture[i]) {
            printf("Erreur de création de la texture : %s\n", SDL_GetError());
            return 0;
        }
    }
    return OK;
}

/**
 * @brief Dessine l'arrière-plan du jeu à l'écran.
 * 
 * Cette fonction copie la texture d'arrière-plan active sur le renderer principal
 * afin de l'afficher dans la fenêtre du jeu.
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer et les textures d'arrière-plan.
 */
void draw_background() {
    SDL_RenderCopy(game.renderer,game.backgroundTexture[game.mat_active] , NULL, &game.dms_win);                 // Dessiner le background
}

/**
 * @brief Dessine le calque actif du jeu à l'écran.
 * 
 * Cette fonction copie la texture du calque actif sur le renderer principal
 * afin de l'afficher dans la fenêtre du jeu.
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer et les textures de calque.
 */
void draw_calque() {
    SDL_RenderCopy(game.renderer,game.calqueTexture[game.mat_active] , NULL, &game.dms_win);                 // Dessiner le background
}

/*=================================================*/






//FONCTION MAT
/*=================================================*/

/**
 * @brief Initialise la matrice de jeu pour stocker les données de la carte.
 * 
 * Cette fonction alloue dynamiquement une matrice tridimensionnelle utilisée pour stocker 
 * les informations des différentes couches de la carte du jeu. Elle gère également 
 * la libération de la mémoire en cas d'échec d'allocation.
 * 
 * @param game Pointeur vers la structure du jeu contenant la matrice de carte.
 * @param taille_x Nombre de colonnes de la matrice.
 * @param taille_y Nombre de lignes de la matrice.
 * @return Retourne 1 en cas de succès, 0 en cas d'échec d'allocation mémoire.
 */
int init_mat(int taille_x, int taille_y) {
    game.mat = malloc(6 * sizeof(int **));
    if (game.mat == NULL) {
        printf("Échec de l'allocation mémoire principale\n");
        return 0;
    }

    for (int i = 0; i < 6; i++) {
        game.mat[i] = malloc(taille_y * sizeof(int *));
        if (game.mat[i] == NULL) {
            printf("Échec de l'allocation mémoire (1) pour mat[%d]\n", i);
            // Libération de la mémoire déjà allouée
            for (int k = 0; k < i; k++) {
                free(game.mat[k]);
            }
            free(game.mat);
            return 0;
        }

        for (int j = 0; j < taille_y; j++) {
            game.mat[i][j] = malloc(taille_x * sizeof(int));
            if (game.mat[i][j] == NULL) {
                printf("Échec de l'allocation mémoire (2) pour mat[%d][%d]\n", i, j);
                // Libération de la mémoire déjà allouée dans mat[i]
                for (int h = 0; h < j; h++) {
                    free(game.mat[i][h]);
                }
                free(game.mat[i]);

                // Libération des matrices précédentes
                for (int k = 0; k < i; k++) {
                    for (int l = 0; l < taille_y; l++) {
                        free(game.mat[k][l]);
                    }
                    free(game.mat[k]);
                }
                free(game.mat);
                return 0;
            }
        }
    }

    return 1;
}


/**
 * @brief Remplit la matrice de jeu avec les données des fichiers de carte.
 * 
 * Cette fonction lit les fichiers de sauvegarde des cartes situés dans le dossier "save/" 
 * et remplit la matrice de jeu en fonction des valeurs lues. Elle gère six fichiers 
 * correspondant aux différentes couches de la carte.
 * 
 * @param game Pointeur vers la structure du jeu contenant la matrice de carte.
 * @param taille_x Nombre de colonnes de la matrice.
 * @param taille_y Nombre de lignes de la matrice.
 * @return Retourne 1 en cas de succès, 0 en cas d'échec d'ouverture des fichiers.
 */
int remplir_mat(int taille_x, int taille_y) {
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
                game.mat[i][j][h] = n;
            }
            
        }

        fclose(file);
    }
    
    return 1;
}

/**
 * @brief Affiche la matrice de jeu dans la console.
 * 
 * Cette fonction affiche la matrice spécifiée en parcourant ses lignes et ses colonnes,
 * puis imprime les valeurs stockées dans la console.
 * 
 * @param game Pointeur vers la structure du jeu contenant la matrice.
 * @param taille_x Nombre de colonnes de la matrice.
 * @param taille_y Nombre de lignes de la matrice.
 * @param n_mat Index de la matrice à afficher (correspondant à une couche de la carte).
 */
void aff_mat(int taille_x, int taille_y, int n_mat)  {
    for(int i = 0; i < taille_y; i++) {
        for(int j = 0;j < taille_x; j++){
            printf("%d ", game.mat[n_mat][i][j]);
        }
        printf("\n"); 
    }

    printf("\n\n\n"); 

}

/**
 * @brief Libère la mémoire allouée à la matrice du jeu.
 * 
 * Cette fonction désalloue l'espace mémoire occupé par la matrice tridimensionnelle 
 * utilisée pour stocker les différentes couches de la carte du jeu.
 * 
 * @param game Pointeur vers la structure du jeu contenant la matrice.
 * @param taille_x Nombre de colonnes de la matrice (non utilisé dans la fonction, mais peut être utile pour modification future).
 * @param taille_y Nombre de lignes de la matrice.
 */
void free_mat(int taille_x, int taille_y) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < taille_y; j++) {
            free(game.mat[i][j]);
        }
        free(game.mat[i]);
    }
    free(game.mat);
}
/*=================================================*/




//FONCTION GESTION OBJET
/*=================================================*/
/**
 * @brief Initialise les textures du joueur en fonction de son sexe.
 * 
 * Cette fonction charge les sprites du joueur (haut, bas, gauche, droite) 
 * en fonction du sexe sélectionné et les convertit en textures SDL. 
 * Elle gère également les erreurs de chargement des images.
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer.
 * @param sprite_player Pointeur vers la structure des textures du joueur.
 * @param sexe Caractère représentant le sexe du joueur ('M' pour homme, autre pour femme).
 * @return Retourne 1 en cas de succès, 0 en cas d'échec du chargement des textures.
 */
int init_player(char sexe) {
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
                    game.sprite_joueur.bas[i-1] = SDL_CreateTextureFromSurface(game.renderer, imageSprite[j]);
                    if (!game.sprite_joueur.bas[i-1]) {
                        printf("Erreur de création de la texture dans init player : %s\n", SDL_GetError());
                        SDL_Quit();
                        return 0;
                    }
                    break;
                case 1 :
                    game.sprite_joueur.haut[i-1] = SDL_CreateTextureFromSurface(game.renderer, imageSprite[j]);
                    if (!game.sprite_joueur.haut[i-1]) {
                        printf("Erreur de création de la texture dans init player: %s\n", SDL_GetError());
                        SDL_Quit();
                        return 0;
                    }
                    break;
                case 2 :
                    game.sprite_joueur.gauche[i-1] = SDL_CreateTextureFromSurface(game.renderer, imageSprite[j]);
                    if (!game.sprite_joueur.gauche[i-1]) {
                        printf("Erreur de création de la texture dans init player: %s\n", SDL_GetError());
                        SDL_Quit();
                        return 0;
                    }
                    break;
                case 3 :
                    game.sprite_joueur.droite[i-1] = SDL_CreateTextureFromSurface(game.renderer, imageSprite[j]);
                    if (!game.sprite_joueur.droite[i-1]) {
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

/**
 * @brief Initialise les textures des PNJ du jeu.
 * 
 * Cette fonction charge les sprites des PNJ (haut, bas, gauche, droite) ainsi que 
 * les sprites spécifiques à certains personnages comme Vin Gazole et Iron Musk. 
 * Elle convertit les images en textures SDL et gère les erreurs de chargement.
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer.
 * @param sprite_pnj Pointeur vers la structure des textures des PNJ.
 * @return Retourne 1 en cas de succès, 0 en cas d'échec du chargement des textures.
 */
int init_pnj() {
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
    game.sprite_pnj.haut = SDL_CreateTextureFromSurface(game.renderer, imageSprite[0]);
    if (!game.sprite_pnj.haut) {
        printf("Erreur de création de la texture dans init player : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    game.sprite_pnj.droite = SDL_CreateTextureFromSurface(game.renderer, imageSprite[1]);
    if (!game.sprite_pnj.droite) {
        printf("Erreur de création de la texture dans init player: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    game.sprite_pnj.bas = SDL_CreateTextureFromSurface(game.renderer, imageSprite[2]);
    if (!game.sprite_pnj.bas) {
        printf("Erreur de création de la texture dans init player: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    game.sprite_pnj.gauche = SDL_CreateTextureFromSurface(game.renderer, imageSprite[3]);
    if (!game.sprite_pnj.gauche) {
        printf("Erreur de création de la texture dans init player : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    game.sprite_pnj.vin_gazole = SDL_CreateTextureFromSurface(game.renderer, imageSprite[4]);
    if (!game.sprite_pnj.vin_gazole) {
        printf("Erreur de création de la texture dans init player : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    game.sprite_pnj.iron_musk_bas = SDL_CreateTextureFromSurface(game.renderer, imageSprite[5]);
    if (!game.sprite_pnj.iron_musk_bas) {
        printf("Erreur de création de la texture dans init player : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    game.sprite_pnj.iron_musk_droite = SDL_CreateTextureFromSurface(game.renderer, imageSprite[6]);
    if (!game.sprite_pnj.iron_musk_droite) {
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


int init_mecha() {
    int i, j;
    char img[40];
    char ext[6]; // "D.png" ou "G.png"

    SDL_Surface *imageSprite[NB_MECHAS][2];

    for (i = 0; i < NB_MECHAS; i++) {
        for (j = 0; j < 2; j++) {
            snprintf(ext, sizeof(ext), "%c.png", (j == 0) ? 'D' : 'G');     //choisit la lettre par rapport a j
            snprintf(img, sizeof(img), "img/mechas/%d%s", i + 1, ext);      

            imageSprite[i][j] = IMG_Load(img);
            if (!imageSprite[i][j]) {
                printf("Erreur de chargement de l'image %s: %s\n", img, IMG_GetError());
                IMG_Quit();
                return 0;
            }
        }
    }

    // Conversion en textures et libération des surfaces
    for (i = 0; i < NB_MECHAS; i++) {
        for (j = 0; j < 2; j++) {
            mecha[i].texture[j] = SDL_CreateTextureFromSurface(game.renderer, imageSprite[i][j]);
            if (!mecha[i].texture[j]) {
                printf("Erreur de chargement de l'image %s: %s\n", img, IMG_GetError());
                SDL_Quit();
                return 0;
            }
            SDL_FreeSurface(imageSprite[i][j]);
        }
    }

    return 1;
}

/**
 * @brief Crée un objet et l'ajoute à la matrice de jeu.
 * 
 * Cette fonction initialise un objet graphique (ex: joueur, PNJ, objet interactif) 
 * avec ses dimensions, sa position à l'écran et l'ajoute dans la matrice du jeu. 
 * Les entités comme le joueur et les PNJ ont un ajustement de leur position dans la matrice.
 * 
 * @param game Pointeur vers la structure du jeu contenant la matrice et l'affichage.
 * @param taille_w Largeur de l'objet avant mise à l'échelle.
 * @param taille_h Hauteur de l'objet avant mise à l'échelle.
 * @param x Position horizontale de l'objet en pixels.
 * @param y Position verticale de l'objet en pixels.
 * @param type_obj Type de l'objet (JOUEUR, PNJ, etc.).
 * @param n_mat Index de la couche de la matrice où ajouter l'objet.
 * @return Un SDL_Rect contenant les dimensions et la position mise à l'échelle de l'objet.
 */
SDL_Rect create_obj(int taille_w, int taille_h, int x, int y, int type_obj, int n_mat) {
    if(type_obj == JOUEUR || type_obj == PNJ) {
        game.mat[n_mat][(y+24)/PX][x/PX] = type_obj;
    }
    else {
        game.mat[n_mat][y/PX][x/PX] = type_obj;
    }
    SDL_Rect obj;
    
    obj.w = taille_w * game.scale;
    obj.h = taille_h * game.scale;
    
    obj.x = game.dms_win.x + (x * game.scale);
    obj.y = game.dms_win.y + (y * game.scale);

    return obj;
}

/**
 * @brief Affiche le joueur à l'écran en fonction de sa direction et de son animation.
 * 
 * Cette fonction affiche le sprite du joueur en fonction de son mouvement 
 * actuel ou de sa dernière direction. Elle sélectionne la bonne texture pour 
 * donner un effet d'animation lorsque le joueur est en mouvement.
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer.
 * @param obj Pointeur vers le rectangle SDL définissant la position et la taille du joueur.
 * @param sprite_playerH Pointeur vers la structure contenant les sprites animés du joueur.
 * @param j Pointeur vers la structure du joueur contenant son état de déplacement.
 */
void draw_player(SDL_Rect *obj, joueur_t * j) {
    int n_img = j->moving/4;
    
    if(j->move_dx<0) {
        SDL_RenderCopy(game.renderer, game.sprite_joueur.gauche[n_img], NULL, obj);
    }
    else if(j->move_dx>0){
        SDL_RenderCopy(game.renderer, game.sprite_joueur.droite[n_img], NULL, obj);
    }
    else if(j->move_dy>0){
        SDL_RenderCopy(game.renderer, game.sprite_joueur.bas[n_img], NULL, obj);
    }
    else if(j->move_dy<0){
        SDL_RenderCopy(game.renderer, game.sprite_joueur.haut[n_img], NULL, obj);
    }
    else {
        
        switch(j->derniere_touche){
            case 1 :
                SDL_RenderCopy(game.renderer, game.sprite_joueur.gauche[0], NULL, obj);
                break;
            case 2 : 
                SDL_RenderCopy(game.renderer, game.sprite_joueur.droite[0], NULL, obj);
                break;
            case 3 :
                SDL_RenderCopy(game.renderer,game.sprite_joueur.haut[0], NULL, obj);
                break;
            case 4 :
                SDL_RenderCopy(game.renderer, game.sprite_joueur.bas[0], NULL, obj);
                break;
        }
    }
    
}

/**
 * @brief Dessine tous les éléments visibles sur l'écran de jeu.
 * 
 * Cette fonction gère l'affichage de l'arrière-plan, des calques, du joueur et des PNJ. 
 * Elle prend en compte la position du joueur et les éléments environnants (barrières, bâtiments, PNJ) 
 * pour gérer l'ordre d'affichage afin d'assurer une cohérence visuelle.
 * 
 * @param game Pointeur vers la structure du jeu contenant les éléments graphiques.
 * @param j Pointeur vers la structure du joueur.
 * @param sprite_p Pointeur vers le rectangle SDL représentant la position du joueur.
 * @param pnj_sprite Pointeur vers le rectangle SDL des PNJ.
 * @param sprite_pnj Pointeur vers la structure des sprites des PNJ.
 * @param sprite_playerH Pointeur vers la structure contenant les sprites animés du joueur.
 */
void draw_mecha(mechas_t * m, int x, int y, int h, int w, int orientation) {
    SDL_Rect hitbox = {x, y, w, h};
    SDL_RenderCopy(game.renderer, m->texture[orientation], NULL, &hitbox);
}

void draw_all(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite) {
    draw_background();
    
    int joueur_x = j->x;
    int joueur_y = j->y;
    int map_active = game.mat_active;
    
    int joueur_derriere = (game.mat[map_active][joueur_y + 1][joueur_x] == BARRIERE || 
                               game.mat[map_active][joueur_y + 1][joueur_x] == BAT || 
                               game.mat[map_active][joueur_y + 1][joueur_x] == PNJ);
    
    pnj_pos_t pnj_states[NB_PNJ] = {PNJ_NORMAL};
    for (int i = 0; i < NB_PNJ; i++) {
        if ((pnj[i].id_map - 1) != j->numMap) continue; 
        int pnj_x = pnj[i].x;
        int pnj_y = pnj[i].y;
        int devant = (game.mat[map_active][pnj_y + 1][pnj_x] == BARRIERE || 
                          game.mat[map_active][pnj_y + 1][pnj_x] == BAT ||
                          game.mat[map_active][pnj_y + 1][pnj_x] == JOUEUR);
        int derriere = (game.mat[map_active][pnj_y - 1][pnj_x] == BARRIERE || 
                            game.mat[map_active][pnj_y - 1][pnj_x] == BAT ||
                            game.mat[map_active][pnj_y + 1][pnj_x] == JOUEUR);

        if (devant) {
            pnj_states[i] = PNJ_DEVANT;
        } else if (derriere) {
            pnj_states[i] = PNJ_DERRIERE;
        }
    }
    for (int i = 0; i < NB_PNJ; i++) {
        if (pnj_states[i] == PNJ_NORMAL && (pnj[i].id_map - 1) == j->numMap) {
            draw_pnj(&pnj_sprite[i], &pnj[i], j);
        }
    }

    if (joueur_derriere) {
        draw_player(sprite_p, j);
    }

    draw_calque();

    for (int i = 0; i < NB_PNJ; i++) {
        if (pnj_states[i] == PNJ_DEVANT && (pnj[i].id_map - 1) == j->numMap) {
            draw_pnj(&pnj_sprite[i], &pnj[i], j);
        }
    }
    for (int i = 0; i < NB_PNJ; i++) {
        if (pnj_states[i] == PNJ_DERRIERE && (pnj[i].id_map - 1) == j->numMap) {
            draw_pnj(&pnj_sprite[i], &pnj[i], j);
        }
    }

    if (!joueur_derriere) {
        draw_player(sprite_p, j);
    }
}


void draw_pnj(SDL_Rect *obj, pnj_t *pnj, joueur_t *j) { 
    if(!strcmp(pnj->pseudo,"Vin Gazole"))
        SDL_RenderCopy(game.renderer,game.sprite_pnj.vin_gazole , NULL, obj);

    else if(pnj->id_pnj == 20 && j->pointSauvegarde <= 1)
        SDL_RenderCopy(game.renderer,game.sprite_pnj.iron_musk_droite , NULL, obj);

    else if(pnj->id_pnj == 23)
        SDL_RenderCopy(game.renderer,game.sprite_pnj.iron_musk_bas , NULL, obj);

    else if(pnj->id_pnj != 20){
        switch(pnj->orientation){
        case 1 :
            SDL_RenderCopy(game.renderer, game.sprite_pnj.haut, NULL, obj);
            break;
        case 2 : 
            SDL_RenderCopy(game.renderer, game.sprite_pnj.droite, NULL, obj);
            break;
        case 3 :
            SDL_RenderCopy(game.renderer, game.sprite_pnj.bas, NULL, obj);
            break;
        case 4 :
            SDL_RenderCopy(game.renderer, game.sprite_pnj.gauche, NULL, obj);
            break;
        } 
    }
    
}

/**
 * @brief Dessine un PNJ à l'écran en fonction de son orientation et de son type.
 * 
 * Cette fonction affiche un PNJ en fonction de son identité et de son orientation. 
 * Certains PNJ spécifiques, comme "Vin Gazole" et "Iron Musk", ont des sprites dédiés. 
 * Les autres PNJ sont affichés selon leur direction (haut, bas, gauche, droite).
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer.
 * @param obj Pointeur vers le rectangle SDL définissant la position et la taille du PNJ.
 * @param sprite_pnj Pointeur vers la structure contenant les textures des PNJ.
 * @param pnj Pointeur vers la structure du PNJ à afficher.
 * @param j Pointeur vers la structure du joueur (utilisé pour déterminer certaines conditions d'affichage).
 */
void draw_obj(SDL_Rect *obj, SDL_Texture * img ) {
    SDL_RenderCopy(game.renderer, img, NULL, obj);
}

/**
 * @brief Crée un rectangle avec des propriétés personnalisées.
 * 
 * Cette fonction initialise un rectangle avec une position, des dimensions, 
 * une couleur et un texte optionnel. Elle assure également que la chaîne de texte 
 * est correctement copiée et terminée.
 * 
 * @param rectangle Pointeur vers la structure du rectangle à initialiser.
 * @param w Largeur du rectangle.
 * @param h Hauteur du rectangle.
 * @param x Position horizontale du rectangle.
 * @param y Position verticale du rectangle.
 * @param r Composante rouge de la couleur du rectangle (0-255).
 * @param g Composante verte de la couleur du rectangle (0-255).
 * @param b Composante bleue de la couleur du rectangle (0-255).
 * @param a Opacité du rectangle (0-255).
 * @param text Chaîne de caractères à afficher dans le rectangle (peut être NULL).
 */
void creer_rectangle(rectangle_t *rectangle,int w, int h, float x, float y, int r, int g, int b, int a, const char *text) { //creer un rectangle avec tt les parametres
    rectangle->rect.x = x ;
    rectangle->rect.y = y ;
    rectangle->rect.w = w ;
    rectangle->rect.h = h ;
    rectangle->couleur.r = r;
    rectangle->couleur.g = g;
    rectangle->couleur.b = b;
    rectangle->couleur.a = a;       //oppacité
    if (text != NULL) {
        strncpy(rectangle->text, text, sizeof(rectangle->text) - 1);
    } else {
        rectangle->text[0] = '\0'; // eviter une chaîne non initialisée
    }
 
}

/**
 * @brief Affiche un texte aligné à gauche et centré verticalement dans un rectangle.
 * 
 * Cette fonction dessine un texte à l'intérieur d'un rectangle en l'alignant à gauche 
 * et en le centrant verticalement. Elle utilise SDL_ttf pour le rendu du texte 
 * et gère les erreurs de chargement de la police et de création de texture.
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer et la police.
 * @param rectangle Pointeur vers la structure du rectangle où afficher le texte.
 */
void draw_text_left_middle(rectangle_t* rectangle) {
    if (rectangle->text[0] == '\0') return; // Vérifier si le texte est valide
    if (!game.police) {
        printf("Erreur : Police non chargée\n");
        return;
    }

    SDL_Color textColor = {0, 0, 0, 255}; 
    SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(game.police, rectangle->text, textColor,1500);
    if (!surface) {
        printf("Erreur SDL_ttf : %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(game.renderer, surface);
    if (!texture) {
        printf("Erreur SDL : %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    int text_width = surface->w;
    int text_height = surface->h;
    SDL_FreeSurface(surface);

    SDL_Rect textRect = {
        rectangle->rect.x, // Aligné à gauche
        rectangle->rect.y + (rectangle->rect.h - text_height) / 2, // Centré verticalement
        text_width,
        text_height
    };
    

    SDL_RenderCopy(game.renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
}

/**
 * @brief Affiche un texte centré horizontalement et verticalement dans un rectangle.
 * 
 * Cette fonction affiche un texte au centre d'un rectangle donné. Elle utilise SDL_ttf 
 * pour le rendu du texte et ajuste automatiquement la position du texte 
 * afin qu'il soit parfaitement centré dans le rectangle.
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer et la police.
 * @param rectangle Pointeur vers la structure du rectangle où afficher le texte.
 */
void draw_text_center(rectangle_t* rectangle) {
    if (rectangle->text[0] == '\0') {
        return; // Vérifier si le texte est valide
    } 
    if (!game.police) {
        printf("Erreur : Police non chargée\n");
        return;
    }

    SDL_Color textColor = {0, 0, 0, 255}; 
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect textRect;
    char ligne[256];
    char *debut = rectangle->text;

    int num_lignes = 1;
    for (int j = 0; rectangle->text[j] != '\0'; j++) {
        if (rectangle->text[j] == '\n') num_lignes++; // Compter le nombre de lignes
    }

    int y_offset = 20; // Décalage vertical entre chaque ligne

    int cmp = 0; // Compteur de lignes

    while (*debut) {
        int i = 0;
        while (debut[i] != '\0' && debut[i] != '\n') {
            ligne[i] = debut[i];
            i++;
        }
        ligne[i] = '\0'; // Terminer la ligne

        // Créer la texture pour la ligne actuelle
        surface = TTF_RenderText_Solid(game.police, ligne, textColor);
        if (!surface) {
            printf("Erreur SDL_ttf : %s\n", TTF_GetError());
            return;
        }
        texture = SDL_CreateTextureFromSurface(game.renderer, surface);

        int text_width = surface->w;
        int text_height = surface->h;

        // Calcul de la position centrée
        textRect.x = rectangle->rect.x + (rectangle->rect.w - text_width) / 2;
        textRect.y = rectangle->rect.y + (rectangle->rect.h - text_height * num_lignes) / 2 + cmp * y_offset;
        textRect.w = text_width;
        textRect.h = text_height;

        // Afficher le texte
        SDL_RenderCopy(game.renderer, texture, NULL, &textRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Passer à la ligne suivante
        if (debut[i] == '\n') i++; // Sauter le '\n'
        debut += i; // Déplacer le pointeur
        cmp++;
    }
}

void draw_text_pos(char *text, int x, int y) {
    if (!game.police) {
        printf("Erreur : Police non chargée\n");
        return;
    }

    SDL_Color textColor = {0, 0, 0, 255}; 
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect textRect;
    char ligne[256];
    char *debut = text;
    
    int num_lignes = 1;  
    for (int j = 0; text[j] != '\0'; j++) {
        if (text[j] == '\n') num_lignes++; // Compter le nombre de lignes
    }

    int y_offset = 20; // Centrer verticalement les lignes

    int cmp = 0; // Compteur de lignes
    
    while (*debut) {
        int i = 0;
        while (debut[i] != '\0' && debut[i] != '\n') {
            ligne[i] = debut[i]; 
            i++;
        }
        ligne[i] = '\0'; // Terminer la ligne

        // Créer la texture pour la ligne actuelle
        surface = TTF_RenderText_Solid(game.police, ligne, textColor);
        if (!surface) {
            printf("Erreur SDL_ttf : %s\n", TTF_GetError());
            return;
        }
        texture = SDL_CreateTextureFromSurface(game.renderer, surface);

        int text_width = surface->w;
        int text_height = surface->h;

        // Calcul de la position centrée
        textRect.x = x;
        textRect.y = y + cmp * y_offset;
        textRect.w = text_width;
        textRect.h = text_height;

        // Afficher le texte
        SDL_RenderCopy(game.renderer, texture, NULL, &textRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Passer à la ligne suivante
        if (debut[i] == '\n') i++; // Sauter le '\n'
        debut += i; // Déplacer le pointeur
        cmp++;
    }
}


/**
 * @brief Dessine un rectangle coloré et y affiche un texte en fonction d'une fonction de rendu.
 * 
 * Cette fonction dessine un rectangle rempli avec une couleur donnée et applique 
 * une fonction de rendu pour afficher du texte à l'intérieur si nécessaire. 
 * La fonction de rendu (`draw_func`) peut être utilisée pour aligner le texte 
 * différemment (ex: centré, aligné à gauche).
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer.
 * @param rectangle Pointeur vers la structure du rectangle à afficher.
 * @param draw_func Pointeur vers une fonction de rendu du texte (ex: `draw_text_left_middle` ou `draw_text_center`).
 */
void draw_rect(rectangle_t *rectangle,void (*draw_func)(rectangle_t *)) {
        SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(game.renderer, rectangle->couleur.r, rectangle->couleur.g, rectangle->couleur.b, rectangle->couleur.a);
        SDL_RenderFillRect(game.renderer, &rectangle->rect);
        // Dessiner le texte
        if(rectangle->text[0] != '\0') draw_func(rectangle);
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);

}

/**
 * @brief Dessine plusieurs rectangles avec du texte centré.
 * 
 * Cette fonction permet d'afficher plusieurs rectangles avec du texte en appelant 
 * `draw_rect` pour chacun d'eux. Le texte est automatiquement centré dans chaque rectangle.
 * Elle utilise une liste d'arguments variables (`va_list`) pour gérer un nombre dynamique 
 * de rectangles à afficher.
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer.
 * @param n Nombre de rectangles à dessiner.
 * @param ... Liste des pointeurs vers les structures `rectangle_t` à afficher.
 */
void draw_all_rect(int n, ...) {
    va_list args;
    va_start(args, n);
    for (int i = 0; i < n; i++) {
        rectangle_t *rect = va_arg(args, rectangle_t *);
        draw_rect(rect,draw_text_center);
    }  
    va_end(args);

}

/**
 * @brief Affiche un dialogue interactif à l'écran, lettre par lettre, avec gestion des choix utilisateur.
 *
 * Cette fonction affiche un texte de dialogue progressivement (effet typewriter) dans une zone dédiée de l'écran.
 * Un rectangle de fond sombre est affiché, accompagné du pseudo du personnage parlant et d'une consigne utilisateur :
 * - Sans choix (`choix = 0`) : Affiche "A pour continuer" et attend l'appui sur la touche 'A'.
 * - Avec choix (`choix != 0`) : Affiche "Choisir entre 1 et 3" et attend la sélection utilisateur par touche (1, 2 ou 3).
 *
 * L'affichage du texte se fait progressivement avec des pauses plus longues en fin de phrases (., !, ?).
 * La fonction retourne immédiatement la valeur sélectionnée par l'utilisateur en cas de choix, ou OK (défini comme constante)
 * pour signaler la fin du dialogue sans choix particulier.
 *
 * @param game           Pointeur vers la structure du jeu contenant le renderer SDL.
 * @param j              Pointeur vers la structure joueur pour l'affichage éventuel du personnage.
 * @param sprite_p       Rectangle définissant la position du sprite joueur.
 * @param pnj_sprite     Rectangle définissant la position du sprite du PNJ en interaction.
 * @param sprite_pnj     Pointeur vers la structure contenant les textures du PNJ.
 * @param sprite_playerH Pointeur vers la structure contenant les textures du joueur.
 * @param pseudo         Chaîne de caractères indiquant le pseudo ou nom du personnage parlant.
 * @param dialogue       Chaîne de caractères contenant le texte à afficher.
 * @param choix          Entier indiquant si le dialogue inclut un choix utilisateur (0 : pas de choix, sinon choix activé).
 *
 * @return int
 *         - `OK` si le dialogue se termine normalement sans choix utilisateur (appui sur 'A').
 *         - `1`, `2` ou `3` en cas de choix utilisateur correspondant à la touche pressée.
 *         - `ERR` si les paramètres passés sont invalides.
 *
 * @pre Les ressources graphiques et les pointeurs passés (`game`, `j`, `sprite_p`, etc.) doivent être valides et initialisés.
 *      Le renderer SDL (`game->renderer`) doit être initialisé avant l'appel.
 *
 * @post La fonction bloque tant que l'utilisateur n'a pas interagi conformément aux instructions affichées.
 *       Aucun changement d'état du jeu n’est effectué hormis l'affichage.
 *
 * @note La fonction gère elle-même les événements SDL et contrôle finement le framerate pour un affichage fluide du texte.
 *
 * @warning Le dialogue passé doit avoir une longueur inférieure à 256 caractères.
 *          Vérifier que les constantes `ERR` et `OK` soient définies correctement dans le programme.
 */
int afficher_dialogue(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, char *pseudo, char *dialogue, int choix) {
    if (!pseudo || !dialogue) return ERR;
    
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game.renderer, &largeurEcran, &hauteurEcran);
    
    int dialogueWidth = largeurEcran * 3 / 4;
    int dialogueHeight = hauteurEcran / 4;
    int dialogueX = (largeurEcran - dialogueWidth) / 2;
    int dialogueY = hauteurEcran - dialogueHeight - 20;
    
    rectangle_t fondDialogue, textRect, pseudoRect, infoRect;
    
    creer_rectangle(&fondDialogue, dialogueWidth, dialogueHeight, dialogueX, dialogueY, 0, 0, 0, 180, "");
    creer_rectangle(&pseudoRect, 200, 40, dialogueX + 20, dialogueY + 10, 255, 255, 255, 255, pseudo);
    creer_rectangle(&textRect, dialogueWidth - 40, dialogueHeight - 80, dialogueX + 20, dialogueY + 60, 255, 255, 255, 255, "");
    
    if (!choix)
        creer_rectangle(&infoRect, 300, 30, dialogueX + (dialogueWidth / 2) - 150, dialogueY + dialogueHeight - 35, 255, 255, 255, 255, "A pour continuer");
    else
        creer_rectangle(&infoRect, 300, 30, dialogueX + (dialogueWidth / 2) - 150, dialogueY + dialogueHeight - 35, 255, 255, 255, 255, "Choisir entre 1 et 3");

    int len = strlen(dialogue);
    char displayedText[256] = "";
    int index = 0, textIndex = 0;
    bool waitingForKey = false;
    SDL_Event event;
    Uint32 frameStart, frameTime;
    
    SDL_FlushEvent(SDL_KEYDOWN);  // Vider les touches avant d'afficher
    
    while (index < len) {
        SDL_RenderClear(game.renderer);
        frameStart = SDL_GetTicks();
        displayedText[textIndex] = dialogue[index];
        displayedText[textIndex + 1] = '\0';
        strncpy(textRect.text, displayedText, sizeof(textRect.text) - 1);
        draw_all(j, sprite_p, pnj_sprite);
        draw_rect( &fondDialogue, draw_text_center);
        draw_rect(&pseudoRect, draw_text_center);
        draw_rect(&textRect, draw_text_left_middle);
        draw_rect(&infoRect, draw_text_center);
        if(choix) {
            draw_mecha(&mecha[0], largeurEcran / 4 -  largeurEcran/6, (hauteurEcran / 2) - ((hauteurEcran / 4) / 2), hauteurEcran / 4, largeurEcran/6, 0);
            draw_mecha(&mecha[2], (largeurEcran / 2) -  ((largeurEcran/6)) / 2, (hauteurEcran / 2) - ((hauteurEcran / 4) / 2), hauteurEcran / 4, largeurEcran/6, 0);
            draw_mecha(&mecha[4], (largeurEcran / 4)*3, (hauteurEcran / 2) - ((hauteurEcran / 4) / 2), hauteurEcran / 4, largeurEcran/6, 0);

        }
        SDL_RenderPresent(game.renderer);
        
        int delay = (dialogue[index] == '.' || dialogue[index] == '!' || dialogue[index] == '?') ? 500 : 10;
        waitingForKey = (dialogue[index] == '.' || dialogue[index] == '!' || dialogue[index] == '?');
        index++;
        textIndex++;

        if (waitingForKey == true) {
            SDL_FlushEvent(SDL_KEYDOWN);  // Éviter la répétition du dernier appui
            
            while (waitingForKey == true) {
                if (SDL_WaitEvent(&event)) {
                    if (event.type == SDL_QUIT) return OK;
                    if (!choix && event.type == SDL_KEYDOWN && !event.key.repeat) {
                        if (event.key.keysym.sym == SDLK_a) {
                            waitingForKey = false;
                            textIndex = 0;
                            memset(displayedText, 0, sizeof(displayedText));
                        }
                    } else if (choix && event.type == SDL_KEYDOWN && !event.key.repeat) {
                        if (event.key.keysym.sym == SDLK_1 || event.key.keysym.sym == SDLK_KP_1) return 1;
                        if (event.key.keysym.sym == SDLK_2 || event.key.keysym.sym == SDLK_KP_2) return 2;
                        if (event.key.keysym.sym == SDLK_3 || event.key.keysym.sym == SDLK_KP_3) return 3;
                    }
                }
            }
        }
        
        frameTime = SDL_GetTicks() - frameStart;
        if (delay > frameTime) SDL_Delay(delay - frameTime);
    }
    
    return OK;
}

int afficher_dialogue_combat(mechas_joueur_t * mecha_joueur, mechas_joueur_t * mecha_ordi, char *pseudo, char *dialogue, int choix) {
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize( game.renderer, &largeurEcran, &hauteurEcran);
    
    int dialogueWidth = largeurEcran * 3 / 4;
    int dialogueHeight = hauteurEcran / 5;
    int dialogueX = (largeurEcran - dialogueWidth) / 2;
    int dialogueY = hauteurEcran - dialogueHeight - 20;
    
    rectangle_t fondDialogue, textRect, pseudoRect, infoRect;
    
    creer_rectangle(&fondDialogue, dialogueWidth, dialogueHeight, dialogueX, dialogueY, 0, 0, 0, 180, NULL);
    creer_rectangle(&pseudoRect, 200, 40, dialogueX + 20, dialogueY + 10, 255, 255, 255, 255, pseudo?pseudo:NULL);
    creer_rectangle(&textRect, dialogueWidth - 40, dialogueHeight - 80, dialogueX + 20, dialogueY + 60, 255, 255, 255, 255, NULL);
    creer_rectangle(&infoRect, 300, 30, dialogueX + (dialogueWidth / 2) - 150, dialogueY + dialogueHeight - 35, 255, 255, 255, 255, "A pour continuer");
    if (!choix)
        creer_rectangle(&infoRect, 300, 30, dialogueX + (dialogueWidth / 2) - 150, dialogueY + dialogueHeight - 35, 255, 255, 255, 255, "A pour continuer");
    else
        creer_rectangle(&infoRect, 300, 30, dialogueX + (dialogueWidth / 2) - 150, dialogueY + dialogueHeight - 35, 255, 255, 255, 255, "Choisir entre 1 et 3");
    int len = strlen(dialogue);
    char displayedText[256] = "";
    int index = 0, textIndex = 0;
    bool waitingForKey = false;
    SDL_Event event;
    Uint32 frameStart, frameTime;
    
    SDL_FlushEvent(SDL_KEYDOWN);  // Vider les touches avant d'afficher
    
    while (index < len) {
        frameStart = SDL_GetTicks();
        displayedText[textIndex] = dialogue[index];
        displayedText[textIndex + 1] = '\0';
        strncpy(textRect.text, displayedText, sizeof(textRect.text) - 1);
        SDL_RenderClear( game.renderer);
        draw_combat(mecha_joueur, mecha_ordi);
        draw_rect(&fondDialogue, draw_text_center);
        draw_rect(&pseudoRect, draw_text_center);
        draw_rect(&textRect, draw_text_left_middle);
        draw_rect(&infoRect, draw_text_center);
        SDL_RenderPresent( game.renderer);
        
        int delay = (dialogue[index] == '.' || dialogue[index] == '!' || dialogue[index] == '?') ? 500 : 10;
        waitingForKey = (dialogue[index] == '.' || dialogue[index] == '!' || dialogue[index] == '?');
        index++;
        textIndex++;

        if (waitingForKey == true) {
            SDL_FlushEvent(SDL_KEYDOWN);  // Éviter la répétition du dernier appui
            
            while (waitingForKey == true) {
                if (SDL_WaitEvent(&event)) {
                    if (event.type == SDL_QUIT) return KO;
                    if (event.type == SDL_KEYDOWN && !event.key.repeat) {
                        if (!choix && event.key.keysym.sym == SDLK_a) {
                            waitingForKey = false;
                            textIndex = 0;
                            memset(displayedText, 0, sizeof(displayedText));
                        }else if (choix && event.type == SDL_KEYDOWN && !event.key.repeat) {
                        if (event.key.keysym.sym == SDLK_1 || event.key.keysym.sym == SDLK_KP_1) return 1;
                        if (event.key.keysym.sym == SDLK_2 || event.key.keysym.sym == SDLK_KP_2) return 2;
                        if (event.key.keysym.sym == SDLK_3 || event.key.keysym.sym == SDLK_KP_3) return 3;
                        }
                    }
                }   
            }
        }
        
        frameTime = SDL_GetTicks() - frameStart;
        if (delay > frameTime) SDL_Delay(delay - frameTime);
    }
    return OK;
}
/*=================================================*/

void game_over(joueur_t *j) {
    int frameTime;
    Uint32 frameStart;
    int running = 1;
    SDL_Event event;

    game.mat_active = 9;
    
    while (running) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A]) {
                    running = 0;
                }
            }
        }

        if (running) {
            // Efface l'écran et dessine l'image
            SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
            SDL_RenderClear(game.renderer);
            draw_background();
            SDL_RenderPresent(game.renderer);
        }

        // Gestion du framerate
        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    // Réinitialisation du joueur
    j->numMap = 0;
    game.mat_active = j->numMap;
    j->x = 23;
    j->y = 8;
    j->derniere_touche = 4;
    j->screen_x = (float)(game.dms_win.x + (j->x * PX * game.scale));
    j->screen_y = (float)(game.dms_win.y + (j->y * PX * game.scale));
}

/*----------Inventaire-----------*/

void afficherTexte(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y) {
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, texte, couleur);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void utiliserObjetSurMecha(int objet_id, mechas_joueur_t *mecha, joueur_t *j, char *message_buffer) {
    switch (objet_id) {
        case 0: // Carburant
            if (j->inventaire->carburant > 0 && mecha->pv > 0 && mecha->pv < mecha->pv_max) {
                mecha->pv += 20;
                if (mecha->pv > mecha->pv_max) mecha->pv = mecha->pv_max;
                j->inventaire->carburant--;
                strcpy(message_buffer, "Mecha soigne avec carburant.");
            } else {
                strcpy(message_buffer, "Impossible d'utiliser le carburant sur ce mecha.");
            }
            break;
        case 1: // Rappel
            if (j->inventaire->rappel > 0 && mecha->pv <= 0) {
                mecha->pv = mecha->pv_max / 2;
                j->inventaire->rappel--;
                strcpy(message_buffer, "Mecha rappele avec succes.");
            } else {
                strcpy(message_buffer, "Impossible d'utiliser le rappel sur ce mecha.");
            }
            break;
    }
}

void afficherInfosMecha(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, mechas_joueur_t mecha_j, int *quitter_total) {
    SDL_Event event;
    int quitter = 0;
    int id = mecha_j.id_mechas;
    if (id <= 0 || id > NB_MECHAS) return;

    while (!quitter && !*quitter_total) {
        Uint32 frameStart = SDL_GetTicks();

        SDL_RenderClear(game.renderer);
        draw_all(j, sprite_p, pnj_sprite);

        // Fond noir + bleu avec tes rectangles
        rectangle_t fondNoir, fondBleu;
        creer_rectangle(&fondNoir, 440, 700, game.dms_win.x + game.dms_win.w - 460, 40, 0, 0, 0, 180, "");
        creer_rectangle(&fondBleu, 420, 680, game.dms_win.x + game.dms_win.w - 450, 50, 0, 0, 255, 220, "");
        draw_all_rect(2, &fondNoir, &fondBleu);

        int fondX = fondNoir.rect.x;

        // Titre "Informations Mecha"
        const char *titre = "Informations Mecha";
        int titreWidth = strlen(titre) * 9;
        int titreX = fondX + (440 - titreWidth) / 2;
        afficherTexte(game.renderer, game.police, titre, titreX, 60);

        // ➔ Image du mecha (utilisation de draw_mecha ici)
        draw_mecha(&mecha[id - 1], fondX + 30, 100, 80, 80, 0);

        // Rectangle noir sous nom + PV
        rectangle_t rectNomPV;
        creer_rectangle(&rectNomPV, 280, 100, fondX + 120, 90, 0, 0, 0, 180, "");
        draw_all_rect(1, &rectNomPV);

        // Nom du mecha
        afficherTexte(game.renderer, game.police, mecha[id - 1].nom, fondX + 130, 100);

        // Barre de PV
        SDL_Rect pvBg = { fondX + 130, 130, 200, 16 };
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(game.renderer, &pvBg);

        int pv = mecha_j.pv;
        int pv_max = mecha_j.pv_max;
        int largeur = (pv_max > 0) ? (200 * pv / pv_max) : 0;

        SDL_Rect pvFill = { fondX + 130, 130, largeur, 16 };
        if (pv <= 0)
            SDL_SetRenderDrawColor(game.renderer, 200, 0, 0, 255);
        else if (pv < pv_max / 3)
            SDL_SetRenderDrawColor(game.renderer, 255, 165, 0, 255);
        else
            SDL_SetRenderDrawColor(game.renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(game.renderer, &pvFill);

        // Rectangle sous les infos
        rectangle_t rectInfos;
        creer_rectangle(&rectInfos, 400, 460, fondX + 20, 200, 0, 0, 0, 150, "");
        draw_all_rect(1, &rectInfos);

        // Infos du Mecha
        char buffer[256];
        int y_info = 220;

        sprintf(buffer, "Type : %s", mecha[id - 1].type);
        afficherTexte(game.renderer, game.police, buffer, fondX + 30, y_info);
        y_info += 40;

        sprintf(buffer, "Evolution : niveau %d", mecha[id - 1].niveau_evolution);
        afficherTexte(game.renderer, game.police, buffer, fondX + 30, y_info);
        y_info += 40;

        afficherTexte(game.renderer, game.police, "Description :", fondX + 30, y_info);
        y_info += 30;

        afficherTexte(game.renderer, game.police, mecha[id - 1].description, fondX + 30, y_info);
        y_info += 50;

        afficherTexte(game.renderer, game.police, "Attaques :", fondX + 30, y_info);
        y_info += 30;

        for (int i = 0; i < mecha[id - 1].nb_attaques; i++) {
            int id_attaque = mecha[id - 1].liste_attaque[i] - 1;
            if (id_attaque >= 0 && id_attaque < NB_ATTAQUES) {
                afficherTexte(game.renderer, game.police, attaque[id_attaque].nom, fondX + 50, y_info);
                y_info += 25;
            }
        }

        // Footer centré
        const char *footer = "Appuyez sur 'A' pour revenir";
        int footerWidth = strlen(footer) * 9;
        int footerX = fondX + (440 - footerWidth) / 2;
        afficherTexte(game.renderer, game.police, footer, footerX, 700);

        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderPresent(game.renderer);

        // Gestion événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN &&
                (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_i))) {
                quitter = 1;
                *quitter_total = 1;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_a)
                quitter = 1;
        }

        // Limitation FPS
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (1000 / 60 > frameTime)
            SDL_Delay((1000 / 60) - frameTime);
    }
}




void afficherMechadex(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, game_t *game, int *quitter_total) {
    int selection = 0;
    int quitter = 0;
    SDL_Event event;
    char buffer[100];

    while (!quitter && !*quitter_total) {
        Uint32 frameStart = SDL_GetTicks();

        SDL_RenderClear(game->renderer);
        draw_all(j, sprite_p, pnj_sprite);

        // Fond
        rectangle_t fondNoir;
        creer_rectangle(&fondNoir, 440, 700, game->dms_win.x + game->dms_win.w - 460, 40, 0, 0, 0, 180, "");
        draw_all_rect(1, &fondNoir);

        int fondX = fondNoir.rect.x;

        // Titre
        const char *titre = "Mechadex";
        int titreWidth = strlen(titre) * 9;
        int titreX = fondX + (440 - titreWidth) / 2;
        afficherTexte(game->renderer, game->police, titre, titreX, 50);

        for (int i = 0; i < NB_MECHAS_INVENTAIRE && i < j->nb_mechas; i++) {
            SDL_Rect mechaRect = {fondX + 10, 90 + i * 130, 420, 110};
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 200, 255);
            SDL_RenderFillRect(game->renderer, &mechaRect);

            if (i == selection) {
                SDL_SetRenderDrawColor(game->renderer, 255, 255, 0, 255);
                SDL_RenderDrawRect(game->renderer, &mechaRect);
            }

            // ➔ Image du mecha
            draw_mecha(&mecha[j->mechas_joueur[i].id_mechas - 1], fondX + 20, 100 + i * 130, 80, 80, 0);

            // Texte
            strcpy(buffer, mecha[j->mechas_joueur[i].id_mechas - 1].nom);
            afficherTexte(game->renderer, game->police, buffer, fondX + 110, 105 + i * 130);
            afficherTexte(game->renderer, game->police, "PV:", fondX + 110, 135 + i * 130);

            SDL_Rect pvBg = {fondX + 150, 138 + i * 130, 200, 16};
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(game->renderer, &pvBg);

            int pv = j->mechas_joueur[i].pv;
            int pv_max = j->mechas_joueur[i].pv_max;
            int largeur = (pv_max > 0) ? (200 * pv / pv_max) : 0;
            SDL_Rect pvFill = {fondX + 150, 138 + i * 130, largeur, 16};

            if (pv <= 0)
                SDL_SetRenderDrawColor(game->renderer, 200, 0, 0, 255);
            else if (pv < pv_max / 3)
                SDL_SetRenderDrawColor(game->renderer, 255, 165, 0, 255);
            else
                SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);

            SDL_RenderFillRect(game->renderer, &pvFill);
        }

        // Footer centré
        const char *footer = "Appuyez sur 'A' pour l'inventaire";
        int footerWidth = strlen(footer) * 9;
        int footerX = fondX + (440 - footerWidth) / 2;
        afficherTexte(game->renderer, game->police, footer, footerX, 670);

        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderPresent(game->renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN &&
                (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_i))) {
                quitter = 1;
                *quitter_total = 1;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_a:
                        quitter = 1;
                        break;
                    case SDLK_RETURN:
                        afficherInfosMecha(j, sprite_p, pnj_sprite, j->mechas_joueur[selection], quitter_total);
                        break;
                    case SDLK_UP:
                        if (selection > 0) selection--;
                        break;
                    case SDLK_DOWN:
                        if (selection < NB_MECHAS_INVENTAIRE - 1 && selection < j->nb_mechas - 1) selection++;
                        break;
                }
            }
        }

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (1000 / 60 > frameTime)
            SDL_Delay((1000 / 60) - frameTime);
    }
}



void afficherSelectionMecha(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, game_t *game, int objet_id, int *quitter_total) {
    int selection = 0;
    int quitter = 0;
    SDL_Event event;
    char buffer[100];
    char message[128] = "";

    while (!quitter && !*quitter_total) {
        Uint32 frameStart = SDL_GetTicks();

        SDL_RenderClear(game->renderer);
        draw_all(j, sprite_p, pnj_sprite);

        int fondX = game->dms_win.x + game->dms_win.w - 460;
        rectangle_t fond;
        creer_rectangle(&fond, 440, 700, fondX, 40, 0, 0, 0, 180, "");
        draw_all_rect(1, &fond);

        afficherTexte(game->renderer, game->police, "Choix du Mecha", fondX + 140, 50);

        for (int i = 0; i < j->nb_mechas && i < NB_MECHAS_INVENTAIRE; i++) {
            SDL_Rect mechaRect = {fondX + 10, 90 + i * 130, 420, 110};
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 200, 255);
            SDL_RenderFillRect(game->renderer, &mechaRect);

            if (i == selection) {
                SDL_SetRenderDrawColor(game->renderer, 255, 255, 0, 255);
                SDL_RenderDrawRect(game->renderer, &mechaRect);
            }

            // ➔ Image mecha
            draw_mecha(&mecha[j->mechas_joueur[i].id_mechas - 1], fondX + 20, 100 + i * 130, 80, 80, 0);

            sprintf(buffer, "Mecha #%d", j->mechas_joueur[i].id_mechas);
            afficherTexte(game->renderer, game->police, buffer, fondX + 110, 105 + i * 130);
            afficherTexte(game->renderer, game->police, "PV:", fondX + 110, 135 + i * 130);

            SDL_Rect pvBarBg = {fondX + 150, 138 + i * 130, 200, 16};
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(game->renderer, &pvBarBg);

            int pvWidth = (int)(200.0 * j->mechas_joueur[i].pv / j->mechas_joueur[i].pv_max);
            SDL_Rect pvBar = {fondX + 150, 138 + i * 130, pvWidth, 16};

            if (j->mechas_joueur[i].pv <= 0)
                SDL_SetRenderDrawColor(game->renderer, 200, 0, 0, 255);
            else if (j->mechas_joueur[i].pv < j->mechas_joueur[i].pv_max / 3)
                SDL_SetRenderDrawColor(game->renderer, 255, 165, 0, 255);
            else
                SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(game->renderer, &pvBar);
        }

        if (strlen(message) > 0) {
            int textWidth = strlen(message) * 9;
            int textX = fondX + (440 - textWidth) / 2;
            afficherTexte(game->renderer, game->police, message, textX, 610);
        }

        const char *footer = "Appuyez sur 'A' pour revenir";
        int footerWidth = strlen(footer) * 9;
        int footerX = fondX + (440 - footerWidth) / 2;
        afficherTexte(game->renderer, game->police, footer, footerX, 670);

        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderPresent(game->renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN &&
                (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_i))) {
                quitter = 1;
                *quitter_total = 1;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (selection > 0) selection--;
                        break;
                    case SDLK_DOWN:
                        if (selection < NB_MECHAS_INVENTAIRE - 1 && selection < j->nb_mechas - 1) selection++;
                        break;
                    case SDLK_RETURN:
                        message[0] = '\0';
                        utiliserObjetSurMecha(objet_id, &j->mechas_joueur[selection], j, message);
                        break;
                    case SDLK_a:
                        quitter = 1;
                        break;
                }
            }
        }

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (1000 / 60 > frameTime)
            SDL_Delay((1000 / 60) - frameTime);
    }
}


int afficherInventaire(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, game_t *game) {
    int selection = 0;
    int quitter = 0;
    int quitter_total = 0;
    SDL_Event event;
    char buffer[100];

    const char *noms[] = {"Carburant", "Rappel", "Mechaball", "Repousse"};
    const char *descriptions[] = {
        "Rend 50% des PV",
        "Ranime un mecha K.O.",
        "Permet de capturer un mecha",
        "Repousse les combats"
    };

    int *quantites[] = {
        &j->inventaire->carburant,
        &j->inventaire->rappel,
        &j->inventaire->mechaball,
        &j->inventaire->repousse
    };

    while (!quitter && !quitter_total) {
        Uint32 frameStart = SDL_GetTicks();

        SDL_RenderClear(game->renderer);
        draw_all(j, sprite_p, pnj_sprite);

        // Fond
        rectangle_t fondNoir, fondBleu;
        creer_rectangle(&fondNoir, 440, 700, game->dms_win.x + game->dms_win.w - 460, 40, 0, 0, 0, 180, "");
        creer_rectangle(&fondBleu, 420, 680, game->dms_win.x + game->dms_win.w - 450, 50, 0, 0, 255, 220, "");
        draw_all_rect(2, &fondNoir, &fondBleu);

        int fondX = fondNoir.rect.x;

        // Titre
        const char *titre = "Inventaire";
        int titreWidth = strlen(titre) * 9;
        int titreX = fondX + (440 - titreWidth) / 2;
        afficherTexte(game->renderer, game->police, titre, titreX, 60);

        for (int i = 0; i < NB_OBJET - 1; i++) {
            rectangle_t itemBox;
            creer_rectangle(&itemBox, 420, 110, fondX + 10, 90 + i * 130, 0, 0, 180, 255, "");
            draw_all_rect(1, &itemBox);

            if (i == selection) {
                SDL_SetRenderDrawColor(game->renderer, 255, 255, 0, 255);
                SDL_RenderDrawRect(game->renderer, &itemBox.rect);
            }

            SDL_Rect imgRect = {fondX + 20, 100 + i * 130, 80, 80};
            SDL_SetRenderDrawColor(game->renderer, 0, 200, 0, 255);
            SDL_RenderFillRect(game->renderer, &imgRect);

            afficherTexte(game->renderer, game->police, noms[i], fondX + 110, 105 + i * 130);
            afficherTexte(game->renderer, game->police, descriptions[i], fondX + 110, 135 + i * 130);

            sprintf(buffer, "x%d", *quantites[i]);
            afficherTexte(game->renderer, game->police, buffer, fondX + 360, 105 + i * 130);
        }

        const char *footer = "Appuyez sur 'A' pour le MechaDex";
        int footerWidth = strlen(footer) * 9;
        int footerX = fondX + (440 - footerWidth) / 2;
        afficherTexte(game->renderer, game->police, footer, footerX, 670);
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderPresent(game->renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || 
                (event.type == SDL_KEYDOWN && 
                (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_i))) {
                quitter = 1;
                quitter_total = 1;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_a:
                        afficherMechadex(j, sprite_p, pnj_sprite, game, &quitter_total);
                        break;
                    case SDLK_UP:
                        if (selection > 0) selection--;
                        break;
                    case SDLK_DOWN:
                        if (selection < NB_OBJET - 1) selection++;
                        break;
                    case SDLK_RETURN:
                        if (selection == 0 || selection == 1) {
                            if (*quantites[selection] > 0 && j->nb_mechas > 0) {
                                afficherSelectionMecha(j, sprite_p, pnj_sprite, game, selection, &quitter_total);
                            } else {
                                const char *msg = "Aucun mecha disponible ou pas assez d'objet.";
                                int msgWidth = strlen(msg) * 9;
                                int msgX = fondX + (440 - msgWidth) / 2;
                                afficherTexte(game->renderer, game->police, msg, msgX, 620);
                                SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
                                SDL_RenderPresent(game->renderer);
                                SDL_Delay(1000);
                            }
                        } else {
                            const char *msg = "Utilisable uniquement en combat";
                            int msgWidth = strlen(msg) * 9;
                            int msgX = fondX + (440 - msgWidth) / 2;
                            afficherTexte(game->renderer, game->police, msg, msgX, 620);
                            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
                            SDL_RenderPresent(game->renderer);
                            SDL_Delay(1000);
                        }
                        break;
                }
            }
        }

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (1000 / 60 > frameTime)
            SDL_Delay((1000 / 60) - frameTime);
    }

    return 0;
}


int affichage_pc(joueur_t *joueur) {
    int mat_sauv = game.mat_active;
    game.mat_active = 8;
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    int running = 1;
    int win_w, win_h;
    SDL_GetRendererOutputSize(game.renderer, &win_w, &win_h);       //dimensions ecran
    int choix = 0;
    int page = 0;
    int start_index = 0, end_index = 10;
    int num_mecha;
    mechas_joueur_t temp;

    //texte à afficher
    char texte_mecha[50][256];

    int existe[50];
    for(int i = 4, j = 0; i < 54; i++, j++){
        if(joueur->mechas_joueur[i].numero == i + 1){
            existe[j] = 1;
            strcpy(texte_mecha[j], mecha[joueur->mechas_joueur[i].id_mechas - 1].nom);
            strcat(texte_mecha[j], "\t | \t Niveau : ");
            concat(texte_mecha[j], joueur->mechas_joueur[i].niveau);
        }
        else{
            strcpy(texte_mecha[j], "\0");
            existe[j] = 0;
        }

    }

    //couleur des bordures
    SDL_Color couleur_bordure =  {94, 99, 102, 250};
    SDL_Color couleur_bordure_selec = {0, 0, 0, 230};
    
    //declaration des rectangles
    rectangle_t rect_info;
    rectangle_t rect[50];
    rectangle_t rect_bordure[50];
    rectangle_t rect_bordure_info;

    //initialisation des rectangles
    int rect_w = win_w * 0.3;
    int rect_h = win_h * 0.1;
    int info_w = win_w * 0.15;
    int info_h = win_h * 0.4;
    int h =  win_h * 0.07;
    int w = win_w * 0.07;

    int info_x = win_w  / 2 - (info_w / 2);
    int info_y = win_h / 2 - (info_h / 2);

    int border_size = 5 * game.scale; // Épaisseur des bords
    int spacing_y = (win_h - (5*rect_h)) /6;

    int x1 = (win_w/4) - (rect_w/2);
    int x2 = ((3*win_w)/4) - (rect_w/2);
    int x[10];
    for (int i = 0; i < 10; i++) {
        x[i] = (i < 5) ? x1 : x2;  // Les 5 premiers à gauche, les 5 suivants à droite
    }
    int y[10];
    for(int i = 0; i < 10; i++){
        y[i] = spacing_y + (i % 5) * (rect_h + spacing_y);
    }

    for(int i = 0; i < 50; i++) {
        creer_rectangle(&rect[i], rect_w, rect_h, x[i%10], y[i%10], 255, 255, 255, 255, texte_mecha[i]);
        creer_rectangle(&rect_bordure[i], rect_w + 2 * border_size, rect_h + 2 * border_size, x[i%10] - border_size, y[i%10] - border_size, 255, 255, 255, 230, NULL);
    }    
    creer_rectangle(&rect_info, info_w, info_h, info_x, info_y, 255, 255, 255, 255, "<------- \n P : Precedent \n \n \n A : Echanger dans\n \n votre equipe \n \n \n  ESC : Retour  \n \n \n  S : Suivant \n ------->");
    creer_rectangle(&rect_bordure_info, info_w + 2 * border_size, info_h + 2 * border_size, info_x - border_size, info_y - border_size, 94, 99, 102, 250, NULL);

    while(running) {
        frameStart = SDL_GetTicks();
        
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_LEFT){
                    choix = (choix - 1 + 10) % 10;
                }
                if(event.key.keysym.sym == SDLK_RIGHT){
                    choix = (choix + 1) % 10;
                }
                if(event.key.keysym.sym == SDLK_s){
                    page = (page + 1) % 5;
                }
                if(event.key.keysym.sym == SDLK_p){
                    page = (page -1 + 5) % 5;
                }
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    running = 0;
                }
                if(event.key.keysym.sym == SDLK_a) {
                    if(existe[choix + start_index]) {
                        num_mecha = aff_mechas_combat(joueur);
                        if(num_mecha != 4) {
                            temp = joueur->mechas_joueur[num_mecha];
                            joueur->mechas_joueur[num_mecha] = joueur->mechas_joueur[choix + start_index + 4];
                            joueur->mechas_joueur[choix + start_index + 4] = temp;
                            joueur->mechas_joueur[num_mecha].numero = num_mecha + 1;
                            joueur->mechas_joueur[choix + start_index + 4].numero = choix + start_index + 5;
                            
                            strcpy(texte_mecha[choix + start_index], mecha[joueur->mechas_joueur[choix + start_index + 4].id_mechas - 1].nom);
                            strcat(texte_mecha[choix + start_index], "\t | \t Niveau : ");
                            concat(texte_mecha[choix + start_index], joueur->mechas_joueur[choix + start_index + 4].niveau);
                            strcpy(rect[choix + start_index].text, texte_mecha[choix + start_index]);


                        }
                    }
                }
                SDL_Event e;
                while (SDL_WaitEvent(&e) && e.type != SDL_KEYUP);
            }
        }
        if(running) {
            start_index = page * 10; // Début de la page
            end_index = start_index + 10; // Fin de la page
            for(int i = start_index; i < end_index; i++) {
                rect_bordure[i].couleur = couleur_bordure;
            }
            rect_bordure[choix + start_index].couleur = couleur_bordure_selec;
            SDL_RenderClear(game.renderer);
            draw_background();

            
            for (int i = start_index; i < end_index; i++) {
                draw_rect(&rect_bordure[i],draw_text_center);
            }
            
            for (int i = start_index; i < end_index; i++) {
                draw_rect(&rect[i],draw_text_center);
                if(existe[i]) {
                    draw_mecha(&mecha[joueur->mechas_joueur[i+4].id_mechas - 1], x[i%10], y[i%10], h, w, 0); 
                }

            }

            draw_all_rect(2, &rect_bordure_info, &rect_info);
            
            SDL_RenderPresent(game.renderer);

            frameTime = SDL_GetTicks() - frameStart;
            if (FRAME_DELAY > frameTime) SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    game.mat_active = mat_sauv;
    return 0;

}

void concat(char *dest, int nb) { //concatene un entier a une chaine de caractere
    char tmp[10];
    sprintf(tmp, "%d", nb);
    strcat(dest, tmp);
}
