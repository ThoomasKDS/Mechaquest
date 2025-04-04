/**
 * \file affichage.c
 * \brief fichier contenant la fonctions d'affichage utilisant la librérie de SDL
*/
#include "../lib/affichage.h"
#include "../lib/pointDePassage.h"
//FONCTION BACKGROUND
/*=================================================*/

/**
 * @brief Initialise les textures de fond et adapte l'affichage à la taille de la fenêtre.
 *
 * Cette fonction :
 * - Charge 10 images de fond depuis le répertoire "img/background/".
 * - Convertit ces images en textures utilisables par le moteur de rendu SDL.
 * - Ajuste dynamiquement les dimensions d'affichage pour conserver le ratio largeur/hauteur de l'image par rapport à la fenêtre.
 * - Calcule le facteur d'échelle (`scale`) pour les futurs dessins (sprites, objets, etc.).
 *
 * @return Retourne 1 si l'initialisation est réussie, 0 en cas d'erreur (chargement ou création de texture échouée).
 *
 * @pre
 * - SDL Renderer (`game.renderer`) doit être initialisé avant d'appeler cette fonction.
 * - Les images de fond doivent être présentes dans "img/background/" sous les noms "fond1.png" à "fond10.png".
 *
 * @post
 * - `game.backgroundTexture[i]` est rempli avec les textures des images chargées.
 * - `game.dms_win` est mis à jour pour centrer et adapter correctement l'image.
 * - `game.scale` est défini pour maintenir la proportion lors du rendu des autres éléments.
 *
 * @warning
 * - En cas d'échec de chargement d'une image, un message d'erreur est affiché, les textures ne sont pas initialisées complètement et `IMG_Quit` est appelé prématurément.
 * - L'affichage peut être déséquilibré si les images de fond n'ont pas le même ratio largeur/hauteur que prévu.
 *
 * @note
 * - Cette fonction prépare aussi indirectement les bases pour tous les affichages suivants (sprites, maps, etc.) en déterminant `game.scale`.
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
 * @brief Initialise les textures des calques utilisés pour l'affichage du jeu.
 *
 * Cette fonction :
 * - Charge 6 images de calques depuis le répertoire "img/calque/".
 * - Les fichiers doivent être nommés "calque1.png" à "calque6.png".
 * - Convertit chaque image chargée en une texture SDL stockée dans `game.calqueTexture`.
 *
 * @return Retourne OK (1) si toutes les textures sont correctement chargées et créées, sinon 0 en cas d'erreur.
 *
 * @pre
 * - SDL Renderer (`game.renderer`) doit être initialisé.
 * - Les images de calques doivent exister dans le répertoire "img/calque/".
 *
 * @post
 * - `game.calqueTexture[i]` est rempli avec la texture correspondante à l'image chargée pour chaque `i` de 0 à 5.
 *
 * @warning
 * - Si une image ne peut pas être chargée ou transformée en texture, la fonction arrête le chargement, imprime une erreur et retourne 0.
 *
 * @note
 * - Les textures de calques sont utilisées pour dessiner des éléments superposés à l'arrière-plan, comme des couches additionnelles sur la map.
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
 * @brief Affiche l'arrière-plan (background) actuel du jeu.
 *
 * Cette fonction utilise le renderer SDL pour copier la texture du background correspondant 
 * à la map active (`game.mat_active`) sur toute la fenêtre (`game.dms_win`).
 *
 * @pre
 * - `game.renderer` doit être initialisé.
 * - `game.backgroundTexture` doit contenir les textures chargées via `init_background`.
 * - `game.mat_active` doit être un indice valide dans le tableau `backgroundTexture`.
 *
 * @post
 * - Le fond d'écran correspondant à la map active est dessiné à l'écran.
 *
 * @note
 * - Aucune mise à l'échelle supplémentaire n'est faite ici : la texture est directement redimensionnée via `game.dms_win`.
 */
void draw_background() {
    SDL_RenderCopy(game.renderer,game.backgroundTexture[game.mat_active] , NULL, &game.dms_win);                 // Dessiner le background
}

/**
 * @brief Affiche le calque (overlay) actuel du jeu.
 *
 * Cette fonction utilise le renderer SDL pour copier la texture du calque correspondant 
 * à la map active (`game.mat_active`) sur toute la fenêtre (`game.dms_win`).
 *
 * @pre
 * - `game.renderer` doit être initialisé.
 * - `game.calqueTexture` doit contenir les textures chargées via `init_calque`.
 * - `game.mat_active` doit être un indice valide dans le tableau `calqueTexture`.
 *
 * @post
 * - Le calque correspondant à la map active est dessiné à l'écran.
 *
 * @note
 * - Le calque est superposé par-dessus le fond, couvrant la zone définie par `game.dms_win`.
 */
void draw_calque() {
    SDL_RenderCopy(game.renderer,game.calqueTexture[game.mat_active] , NULL, &game.dms_win);                 // Dessiner le background
}

/*=================================================*/






//FONCTION MAT
/*=================================================*/

/**
 * @brief Initialise une matrice 3D pour la gestion des calques du jeu.
 *
 * Cette fonction alloue dynamiquement une matrice 3D `game.mat` composée de 6 couches,
 * chacune représentant un calque de la carte du jeu. Chaque couche est une matrice
 * de dimensions `taille_y` x `taille_x`.
 *
 * @param taille_x La largeur de chaque matrice (nombre de colonnes).
 * @param taille_y La hauteur de chaque matrice (nombre de lignes).
 *
 * @return Retourne `1` si l'allocation est réussie, `0` en cas d'échec.
 *
 * @pre
 * - `taille_x` et `taille_y` doivent être strictement positifs.
 *
 * @post
 * - Alloue et initialise `game.mat` si toutes les allocations sont réussies.
 * - En cas d'échec, libère toute la mémoire précédemment allouée pour éviter les fuites.
 *
 * @note
 * - La structure `game` doit être accessible et correctement initialisée avant appel.
 *
 * @warning
 * - En cas d'échec d'allocation mémoire à n'importe quelle étape, toute la mémoire déjà réservée est libérée.
 * - Aucun contrôle supplémentaire n'est effectué sur la validité des tailles fournies (`taille_x`, `taille_y`).
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
 * @brief Remplit les matrices de calques avec les données de fichiers de sauvegarde.
 *
 * Cette fonction lit les fichiers `save/map1.txt` à `save/map6.txt` et remplit
 * les matrices correspondantes dans `game.mat` avec les valeurs lues.
 *
 * @param taille_x La largeur de chaque matrice (nombre de colonnes).
 * @param taille_y La hauteur de chaque matrice (nombre de lignes).
 *
 * @return Retourne `1` si tous les fichiers sont lus correctement, sinon `0` en cas d'erreur d'ouverture.
 *
 * @pre
 * - `init_mat` doit avoir été appelé avec succès pour initialiser `game.mat`.
 * - Les fichiers `save/map1.txt` à `save/map6.txt` doivent exister et être correctement formatés.
 *
 * @post
 * - Remplit entièrement `game.mat` avec les données lues depuis les fichiers.
 *
 * @note
 * - Chaque fichier doit contenir exactement `taille_x * taille_y` entiers séparés par des espaces ou retours à la ligne.
 *
 * @warning
 * - Si un fichier ne peut pas être ouvert, la fonction retourne immédiatement `0` sans remplir les autres matrices.
 * - Aucun contrôle n'est effectué sur la validité du contenu des fichiers (format des données lues).
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
 * @brief Affiche le contenu d'une matrice spécifique à l'écran.
 *
 * Cette fonction parcourt et affiche dans la console la matrice `game.mat[n_mat]`
 * sous forme d'une grille d'entiers.
 *
 * @param taille_x La largeur de la matrice à afficher (nombre de colonnes).
 * @param taille_y La hauteur de la matrice à afficher (nombre de lignes).
 * @param n_mat L'indice du calque (de 0 à 5) à afficher dans `game.mat`.
 *
 * @pre
 * - `game.mat` doit avoir été initialisé et rempli correctement.
 * - `n_mat` doit être compris entre 0 et 5 inclus.
 *
 * @post
 * - Affiche la matrice spécifiée dans la console.
 *
 * @note
 * - Un saut de ligne est ajouté entre chaque ligne de la matrice pour une meilleure lisibilité.
 *
 * @warning
 * - Aucun contrôle n'est effectué sur la validité de `n_mat`. Un indice invalide peut entraîner un comportement indéfini.
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
 * @brief Libère la mémoire allouée pour les matrices des calques du jeu.
 *
 * Cette fonction libère toute la mémoire précédemment allouée pour `game.mat`
 * via `init_mat`, en parcourant et libérant chaque ligne, chaque matrice, puis la structure principale.
 *
 * @param taille_x La largeur de chaque matrice (nombre de colonnes). (Non utilisé dans cette fonction mais requis pour cohérence d'interface)
 * @param taille_y La hauteur de chaque matrice (nombre de lignes).
 *
 * @pre
 * - `game.mat` doit avoir été correctement alloué par `init_mat`.
 * - `taille_y` doit correspondre au nombre de lignes effectivement allouées.
 *
 * @post
 * - Toute la mémoire associée à `game.mat` est libérée.
 *
 * @note
 * - `taille_x` n'est pas utilisé dans la fonction, mais il est conservé pour cohérence avec les autres fonctions de manipulation de matrices.
 *
 * @warning
 * - Un appel incorrect (par exemple, sans initialisation préalable) peut entraîner un comportement indéfini.
 * - Après l'appel, `game.mat` devient invalide et ne doit plus être utilisé sans réinitialisation.
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
 * @brief Initialise les textures d'animation du joueur selon son sexe.
 *
 * Cette fonction charge les images de sprite correspondant aux différentes directions
 * (bas, haut, gauche, droite) du joueur, en fonction du sexe sélectionné ('M' ou autre),
 * puis les convertit en textures SDL stockées dans `game.sprite_joueur`.
 *
 * @param sexe Le sexe du joueur ('M' pour masculin, tout autre caractère pour féminin).
 *
 * @return Retourne `1` si toutes les textures sont correctement chargées et créées, sinon `0` en cas d'erreur.
 *
 * @pre
 * - Le module SDL_image doit être initialisé avant l'appel à cette fonction.
 * - Le champ `renderer` de la structure `game` doit être valide et initialisé.
 *
 * @post
 * - Remplit les champs `bas`, `haut`, `gauche` et `droite` de `game.sprite_joueur` avec les textures correspondantes.
 *
 * @note
 * - Les fichiers images doivent suivre une nomenclature précise selon le sexe, sous `img/skin/skin_player_<sexe>/`.
 * - Seules les images 1 à 4 pour chaque direction sont chargées.
 *
 * @warning
 * - En cas d'échec de chargement ou de création de texture, la fonction arrête immédiatement l'initialisation et retourne `0`.
 * - Après une erreur, le module SDL_image est quitté avec `IMG_Quit()`, et SDL peut être quitté via `SDL_Quit()`.
 * - L'absence ou la corruption des fichiers images attendus entraînera un échec.
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
 * @brief Initialise les textures des PNJ (personnages non-joueurs) du jeu.
 *
 * Cette fonction charge les images associées aux différentes orientations et personnages spéciaux,
 * puis les convertit en textures SDL stockées dans la structure `game.sprite_pnj`.
 *
 * @return Retourne `1` si toutes les textures sont correctement chargées et créées, sinon `0` en cas d'erreur.
 *
 * @pre
 * - Le module SDL_image doit être initialisé avant l'appel à cette fonction.
 * - Le champ `renderer` de la structure `game` doit être valide et initialisé.
 *
 * @post
 * - Remplit les champs de `game.sprite_pnj` avec les textures correspondantes aux différentes orientations et personnages.
 *
 * @note
 * - Les fichiers images doivent exister dans le dossier `img/skin/skin_pnj/` et porter les noms attendus.
 * - Toutes les surfaces SDL utilisées sont libérées après la création des textures correspondantes.
 *
 * @warning
 * - En cas d'échec de chargement d'une image ou de création d'une texture, la fonction interrompt l'initialisation et retourne `0`.
 * - Après une erreur, le module SDL_image est quitté via `IMG_Quit()`, et SDL peut être quitté via `SDL_Quit()`.
 * - Un fichier manquant ou corrompu entraînera l'échec de l'initialisation complète des PNJ.
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

/**
 * @brief Initialise les textures des Méchas du jeu.
 *
 * Cette fonction charge les images représentant les Méchas en orientation droite et gauche,
 * les convertit en textures SDL, et les stocke dans la structure `mecha`.
 *
 * @return Retourne `1` si toutes les textures sont chargées et créées correctement, sinon `0` en cas d'erreur.
 *
 * @pre
 * - Le module SDL_image doit être initialisé avant l'appel à cette fonction.
 * - Le champ `renderer` de la structure `game` doit être valide et initialisé.
 * - La constante `NB_MECHAS` doit être définie et correcte.
 *
 * @post
 * - Chaque élément du tableau `mecha[i].texture[j]` est initialisé avec une texture SDL valide.
 *
 * @note
 * - Les fichiers images doivent être présents sous le chemin `img/mechas/` et nommés sous la forme `<index>D.png` ou `<index>G.png`.
 * - Chaque surface chargée est libérée après la création de la texture correspondante.
 *
 * @warning
 * - En cas d'échec de chargement ou de création d'une texture, la fonction s'arrête immédiatement et retourne `0`.
 * - Après une erreur, le module SDL_image est quitté via `IMG_Quit()`, et SDL peut être quitté via `SDL_Quit()`.
 * - Un fichier manquant ou mal nommé entraînera l'échec de l'initialisation.
 */
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
 * @brief Crée un objet de type SDL_Rect et met à jour la matrice du jeu.
 *
 * Cette fonction initialise un `SDL_Rect` représentant un objet du jeu (joueur, PNJ ou autre),
 * en fonction de sa taille, sa position, son type et du calque (`n_mat`) où il doit être placé.
 *
 * @param taille_w La largeur de l'objet (avant mise à l'échelle).
 * @param taille_h La hauteur de l'objet (avant mise à l'échelle).
 * @param x La position horizontale de l'objet (en pixels).
 * @param y La position verticale de l'objet (en pixels).
 * @param type_obj Le type de l'objet (`JOUEUR`, `PNJ` ou autre type d'objet).
 * @param n_mat L'indice du calque (`game.mat`) où placer l'objet.
 *
 * @return Retourne un `SDL_Rect` représentant l'objet positionné et mis à l'échelle.
 *
 * @pre
 * - `game.scale` et `game.dms_win` doivent être initialisés correctement.
 * - `game.mat` doit être alloué et accessible.
 *
 * @post
 * - Met à jour la matrice `game.mat[n_mat]` avec la position de l'objet créé.
 *
 * @note
 * - Pour les types `JOUEUR` et `PNJ`, la position verticale est ajustée avec un décalage de 24 pixels avant division.
 *
 * @warning
 * - Aucun contrôle n'est effectué sur la validité des indices donnés (risque de débordement hors matrice).
 * - Un mauvais type ou des coordonnées incorrectes peuvent entraîner un comportement indéfini.
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
 * @brief Dessine le joueur à l'écran en fonction de sa direction et de son mouvement.
 *
 * Cette fonction sélectionne la bonne texture du joueur en fonction de ses mouvements
 * (`move_dx`, `move_dy`) ou de sa dernière touche directionnelle (`derniere_touche`),
 * puis la rend sur la fenêtre de jeu à l'emplacement spécifié.
 *
 * @param obj Un pointeur vers un `SDL_Rect` représentant la position et la taille du joueur à l'écran.
 * @param j Un pointeur vers la structure `joueur_t` représentant l'état actuel du joueur.
 *
 * @pre
 * - Le champ `renderer` de `game` doit être correctement initialisé.
 * - Les textures de `game.sprite_joueur` doivent être chargées.
 * - Les pointeurs `obj` et `j` doivent être valides.
 *
 * @post
 * - Affiche la texture appropriée du joueur à l'écran en fonction de sa direction ou de son dernier mouvement.
 *
 * @note
 * - L'animation utilise une division entière du champ `moving` pour choisir l'image à afficher.
 * - En l'absence de mouvement, la dernière direction connue (`derniere_touche`) est utilisée pour afficher le joueur immobile.
 *
 * @warning
 * - Aucun contrôle n'est effectué sur la validité du contenu des champs de la structure `joueur_t`.
 * - Une mauvaise initialisation des textures peut entraîner un crash au moment du rendu.
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
 * @brief Dessine un Mécha à l'écran avec la texture correspondant à son orientation.
 *
 * Cette fonction affiche un Mécha sur la fenêtre de jeu en utilisant ses textures
 * et les coordonnées/dimensions spécifiées.
 *
 * @param m Un pointeur vers la structure `mechas_t` représentant le Mécha à afficher.
 * @param x La position horizontale du Mécha à l'écran (en pixels).
 * @param y La position verticale du Mécha à l'écran (en pixels).
 * @param h La hauteur du Mécha (en pixels).
 * @param w La largeur du Mécha (en pixels).
 * @param orientation L'orientation actuelle du Mécha (0 pour droite, 1 pour gauche).
 *
 * @pre
 * - `game.renderer` doit être initialisé et valide.
 * - Le pointeur `m` doit être valide et ses textures correctement chargées.
 *
 * @post
 * - Le Mécha est affiché à l'écran à la position et avec la dimension spécifiées.
 *
 * @note
 * - L'orientation permet de choisir entre la texture droite (`0`) et gauche (`1`).
 *
 * @warning
 * - Aucun contrôle n'est effectué sur la validité de l'orientation fournie.
 * - En cas de texture non initialisée pour l'orientation donnée, SDL peut provoquer une erreur de rendu.
 */
void draw_mecha(mechas_t * m, int x, int y, int h, int w, int orientation) {
    SDL_Rect hitbox = {x, y, w, h};
    SDL_RenderCopy(game.renderer, m->texture[orientation], NULL, &hitbox);
}

/**
 * @brief Gère et affiche tous les éléments visuels principaux du jeu.
 *
 * Cette fonction dessine successivement l'arrière-plan, les PNJ, le joueur, et les calques de décor
 * en fonction de leur position relative et des obstacles présents sur la carte active.
 *
 * @param j Un pointeur vers la structure `joueur_t` représentant l'état actuel du joueur.
 * @param sprite_p Un pointeur vers un `SDL_Rect` représentant le sprite du joueur à l'écran.
 * @param pnj_sprite Un tableau de `SDL_Rect` représentant les sprites des PNJ à l'écran.
 *
 * @pre
 * - Les structures `game`, `pnj`, et les textures correspondantes doivent être initialisées.
 * - Les pointeurs `j`, `sprite_p` et `pnj_sprite` doivent être valides.
 *
 * @post
 * - Tous les éléments visibles sont dessinés correctement à l'écran selon leur état et position.
 *
 * @note
 * - Les PNJ peuvent être dessinés devant ou derrière certains éléments selon la présence d'obstacles (`BARRIERE`, `BAT`, `JOUEUR`).
 * - Le joueur peut être dessiné devant ou derrière selon son environnement immédiat.
 *
 * @warning
 * - Aucune vérification n'est effectuée sur les dimensions ou les limites des matrices de jeu (`game.mat`).
 * - Des erreurs de rendu peuvent survenir si des textures ou des pointeurs sont incorrectement initialisés.
 */
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

/**
 * @brief Dessine un PNJ à l'écran en fonction de son identité et de son orientation.
 *
 * Cette fonction sélectionne et affiche la texture appropriée pour un PNJ donné,
 * en tenant compte de son nom, de son ID spécial ou de son orientation.
 *
 * @param obj Un pointeur vers un `SDL_Rect` représentant la position et la taille du PNJ à l'écran.
 * @param pnj Un pointeur vers la structure `pnj_t` représentant le PNJ à afficher.
 * @param j Un pointeur vers la structure `joueur_t` représentant l'état actuel du joueur.
 *
 * @pre
 * - Le champ `renderer` de la structure `game` doit être correctement initialisé.
 * - Les textures de `game.sprite_pnj` doivent être chargées.
 * - Les pointeurs `obj`, `pnj` et `j` doivent être valides.
 *
 * @post
 * - Affiche la texture du PNJ choisie en fonction de son état et de son contexte.
 *
 * @note
 * - Certains PNJ spécifiques (ex: "Vin Gazole", "Iron Musk") ont des textures particulières selon l'ID ou les conditions de jeu.
 * - Si aucun cas particulier n'est détecté, la texture est choisie selon l'orientation du PNJ.
 *
 * @warning
 * - Une mauvaise initialisation des textures ou un ID invalide peut entraîner un comportement imprévu.
 * - Aucun contrôle n'est effectué sur la validité du champ `pseudo` ou des valeurs d'orientation.
 */
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
 * @brief Dessine un objet générique à l'écran.
 *
 * Cette fonction affiche une texture SDL donnée à l'emplacement spécifié par un `SDL_Rect`.
 *
 * @param obj Un pointeur vers un `SDL_Rect` représentant la position et la taille de l'objet à afficher.
 * @param img Un pointeur vers une `SDL_Texture` représentant l'image de l'objet.
 *
 * @pre
 * - Le champ `renderer` de la structure `game` doit être correctement initialisé.
 * - Les pointeurs `obj` et `img` doivent être valides.
 *
 * @post
 * - La texture est rendue sur la fenêtre de jeu aux coordonnées spécifiées par `obj`.
 *
 * @note
 * - Aucun ajustement ou transformation n'est effectué sur l'image avant le rendu.
 *
 * @warning
 * - Un pointeur `img` nul peut entraîner une erreur d'affichage via SDL.
 * - Aucun contrôle n'est effectué sur la validité des dimensions du rectangle `obj`.
 */
void draw_obj(SDL_Rect *obj, SDL_Texture * img ) {
    SDL_RenderCopy(game.renderer, img, NULL, obj);
}

/**
 * @brief Initialise un rectangle avec ses dimensions, sa couleur et un texte optionnel.
 *
 * Cette fonction remplit la structure `rectangle_t` avec les dimensions, la position,
 * la couleur RGBA spécifiée et un texte associé s'il est fourni.
 *
 * @param rectangle Un pointeur vers la structure `rectangle_t` à initialiser.
 * @param w La largeur du rectangle.
 * @param h La hauteur du rectangle.
 * @param x La position horizontale du rectangle (coordonnée flottante).
 * @param y La position verticale du rectangle (coordonnée flottante).
 * @param r La composante rouge de la couleur (0-255).
 * @param g La composante verte de la couleur (0-255).
 * @param b La composante bleue de la couleur (0-255).
 * @param a La composante alpha (opacité) de la couleur (0-255).
 * @param text Un pointeur vers une chaîne de caractères optionnelle à associer au rectangle.
 *
 * @pre
 * - Le pointeur `rectangle` doit être valide.
 * - Si fourni, `text` doit être une chaîne de caractères valide.
 *
 * @post
 * - Initialise tous les champs du rectangle, y compris la couleur et le texte (si donné).
 *
 * @note
 * - Si `text` est `NULL`, le champ texte du rectangle est initialisé avec une chaîne vide.
 *
 * @warning
 * - Aucun contrôle n'est effectué sur les bornes des valeurs de couleur (supposées entre 0 et 255).
 * - Si `text` dépasse la taille autorisée dans `rectangle->text`, il est tronqué automatiquement.
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
 * @brief Dessine un texte centré verticalement et aligné à gauche dans un rectangle.
 *
 * Cette fonction affiche le texte contenu dans une structure `rectangle_t` en utilisant la police
 * chargée dans `game.police`. Le texte est aligné à gauche et centré verticalement dans le rectangle.
 *
 * @param rectangle Un pointeur vers la structure `rectangle_t` contenant les informations du texte et du rectangle d'affichage.
 *
 * @pre
 * - La police `game.police` doit être chargée et valide.
 * - Le pointeur `rectangle` doit être valide et contenir un texte non vide.
 *
 * @post
 * - Le texte est rendu à l'écran, aligné à gauche et centré verticalement par rapport au rectangle spécifié.
 *
 * @note
 * - Si aucun texte n'est présent (`text[0] == '\0'`), la fonction quitte immédiatement sans dessiner.
 * - La largeur maximale de rendu du texte est fixée à 1500 pixels pour le wrapping automatique.
 *
 * @warning
 * - Si la police n'est pas chargée ou si une erreur SDL/SDL_ttf survient, le texte ne sera pas affiché et un message d'erreur sera affiché dans la console.
 * - Une erreur dans la création de la texture ou du rendu peut entraîner une perte du texte à l'affichage.
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
 * @brief Dessine du texte centré dans un rectangle, ligne par ligne si nécessaire.
 *
 * Cette fonction affiche le texte contenu dans une structure `rectangle_t`, centré
 * horizontalement et verticalement par rapport au rectangle. Si plusieurs lignes sont présentes,
 * elles sont affichées avec un décalage vertical fixe.
 *
 * @param rectangle Un pointeur vers la structure `rectangle_t` contenant le texte et les dimensions du rectangle d'affichage.
 *
 * @pre
 * - La police `game.police` doit être chargée et valide.
 * - Le pointeur `rectangle` doit être valide et contenir un texte non vide.
 *
 * @post
 * - Le texte est affiché centré dans le rectangle, ligne par ligne si nécessaire.
 *
 * @note
 * - Chaque saut de ligne (`\n`) dans le texte entraîne une nouvelle ligne avec un décalage vertical fixé à 20 pixels.
 * - Le texte est centré horizontalement ligne par ligne, mais le bloc de texte global est également centré verticalement.
 *
 * @warning
 * - Si la police n'est pas chargée ou si une erreur SDL/SDL_ttf survient, le texte ne sera pas affiché.
 * - Des erreurs peuvent survenir si le texte est trop long pour le rectangle sans mécanisme de coupure automatique.
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

/**
 * @brief Dessine du texte à une position spécifique à l'écran, avec gestion des retours à la ligne.
 *
 * Cette fonction affiche un texte à partir des coordonnées spécifiées (`x`, `y`), en prenant en compte
 * les retours à la ligne (`\n`) pour afficher plusieurs lignes avec un décalage vertical fixe.
 *
 * @param text Un pointeur vers la chaîne de caractères à afficher.
 * @param x La position horizontale de départ du texte (en pixels).
 * @param y La position verticale de départ du texte (en pixels).
 *
 * @pre
 * - La police `game.police` doit être chargée et valide.
 * - Le pointeur `text` doit être valide et contenir une chaîne de caractères non vide.
 *
 * @post
 * - Le texte est affiché à partir de la position `(x, y)`, en sautant de 20 pixels vers le bas à chaque nouvelle ligne.
 *
 * @note
 * - Chaque saut de ligne (`\n`) est interprété pour créer une nouvelle ligne affichée en dessous de la précédente.
 * - Le texte est aligné à gauche sans centrage horizontal automatique.
 *
 * @warning
 * - Si la police n'est pas chargée ou en cas d'erreur de rendu SDL/SDL_ttf, le texte ne sera pas affiché.
 * - Aucun contrôle n'est effectué sur la longueur des lignes ou sur l'espace disponible à l'écran.
 */
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
 * @brief Dessine un rectangle coloré et son texte associé si présent.
 *
 * Cette fonction remplit un rectangle avec la couleur spécifiée, puis utilise une fonction
 * de dessin (`draw_func`) pour afficher le texte associé s'il existe.
 *
 * @param rectangle Un pointeur vers la structure `rectangle_t` contenant les informations du rectangle et du texte.
 * @param draw_func Un pointeur vers une fonction de dessin prenant un `rectangle_t*` en paramètre (ex: `draw_text_center`, `draw_text_left_middle`).
 *
 * @pre
 * - Le champ `renderer` de `game` doit être initialisé et valide.
 * - Le pointeur `rectangle` doit être valide.
 * - La fonction `draw_func` doit être une fonction valide acceptant un `rectangle_t*`.
 *
 * @post
 * - Le rectangle est dessiné avec la couleur spécifiée.
 * - Si du texte est présent, il est affiché selon la fonction `draw_func`.
 *
 * @note
 * - Le mode de fusion SDL est activé pour permettre la transparence (blend mode).
 * - La couleur du renderer est réinitialisée en noir (`0, 0, 0, 255`) après le dessin.
 *
 * @warning
 * - Aucun contrôle n'est effectué sur la validité ou la compatibilité de `draw_func` avec le contexte actuel.
 * - Si `rectangle->text` est vide (`'\0'`), la fonction de dessin du texte n'est pas appelée.
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
 * @brief Dessine plusieurs rectangles avec leurs textes centrés.
 *
 * Cette fonction permet de dessiner un ensemble de rectangles (`rectangle_t`) en appelant `draw_rect`
 * pour chacun d'eux, en utilisant systématiquement la fonction `draw_text_center` pour afficher le texte.
 *
 * @param n Le nombre de rectangles à dessiner.
 * @param ... Une liste de pointeurs vers des structures `rectangle_t`.
 *
 * @pre
 * - Le champ `renderer` de `game` doit être initialisé et valide.
 * - Chaque argument variable doit être un pointeur valide vers un `rectangle_t`.
 *
 * @post
 * - Tous les rectangles fournis sont dessinés avec leur texte centré.
 *
 * @note
 * - Utilise `va_list` pour parcourir dynamiquement les pointeurs reçus en paramètres.
 * - Le texte associé est affiché uniquement si le champ `text` du rectangle n'est pas vide.
 *
 * @warning
 * - Si un des pointeurs est invalide, cela peut provoquer un comportement indéfini.
 * - Aucun contrôle n'est effectué sur la validité du nombre de rectangles réellement passés en arguments.
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
 * @param j              Un pointeur vers la structure `joueur_t` représentant l'état actuel du joueur.
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

/**
 * @brief Affiche un dialogue interactif durant un combat entre deux Méchas.
 *
 * Cette fonction affiche progressivement un texte de dialogue pendant un combat,
 * en attendant des actions du joueur à certains moments (soit continuer, soit choisir une option).
 *
 * @param mecha_joueur Un pointeur vers la structure `mechas_joueur_t` représentant le Mécha du joueur.
 * @param mecha_ordi Un pointeur vers la structure `mechas_joueur_t` représentant le Mécha adverse.
 * @param pseudo Une chaîne de caractères représentant le nom du personnage qui parle.
 * @param dialogue Le texte du dialogue à afficher, pouvant contenir des ponctuations pour forcer l'attente.
 * @param choix Un entier indiquant si le joueur doit faire un choix (`0` pour continuer normalement, `1` pour choisir entre plusieurs options).
 *
 * @return Retourne :
 * - `OK` si le dialogue est déroulé normalement sans choix,
 * - `1`, `2` ou `3` selon la sélection du joueur si un choix est demandé,
 * - `KO` si l'utilisateur quitte la fenêtre SDL.
 *
 * @pre
 * - `game.renderer` doit être initialisé.
 * - `game.police` doit être chargée correctement.
 * - Les pointeurs `mecha_joueur`, `mecha_ordi`, `pseudo` et `dialogue` doivent être valides.
 *
 * @post
 * - Affiche l'état du combat mis à jour avec le dialogue par-dessus.
 * - Le texte est affiché progressivement caractère par caractère.
 *
 * @note
 * - Une pause est imposée après les signes de ponctuation (`.`, `!`, `?`) jusqu'à ce qu'une touche soit pressée.
 * - Le joueur peut interagir avec `A` pour continuer ou avec `1`, `2`, `3` pour effectuer un choix selon le contexte.
 *
 * @warning
 * - Aucun contrôle n'est effectué sur la longueur du texte (`dialogue`), limité ici à 255 caractères affichables.
 * - Le texte affiché est tronqué si sa longueur dépasse la capacité du buffer.
 * - Un mauvais état de `game.renderer` ou `game.police` peut provoquer des erreurs d'affichage ou de crash.
 */
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

/**
 * @brief Gère l'écran de fin de partie et réinitialise le joueur.
 *
 * Cette fonction affiche un écran de "Game Over", attend une action de l'utilisateur
 * pour continuer, puis réinitialise la position et la carte du joueur.
 *
 * @param j Un pointeur vers la structure `joueur_t` représentant l'état actuel du joueur.
 *
 * @pre
 * - Le champ `renderer` de `game` doit être initialisé.
 * - Le pointeur `j` doit être valide.
 *
 * @post
 * - Affiche l'écran de fin de partie jusqu'à l'appui sur la touche `A` ou la fermeture de la fenêtre.
 * - Réinitialise la position du joueur sur la carte de départ (`numMap = 0`, `x = 23`, `y = 8`).
 *
 * @note
 * - L'image de fond affichée correspond à la carte d'indice `9` dans `game.mat_active`.
 * - Le joueur est repositionné à des coordonnées fixes après la fin du Game Over.
 *
 * @warning
 * - Une fermeture de la fenêtre pendant l'écran de Game Over termine immédiatement la boucle.
 * - La fonction suppose que la carte 9 existe et est correctement chargée.
 */
void game_over(joueur_t *j) {
    int frameTime;
    Uint32 frameStart;
    int running = 1;
    SDL_Event event;

    for(int i = 0; i <4; i++) {
        j->mechas_joueur[i].pv = j->mechas_joueur[i].pv_max;
    }

    game.mat_active = 9;
    
    while (running) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym  == SDLK_a) {
                    running = 0;
                }
            }
        }

        if (running) {
            // Efface l'écran et dessine l'image
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

/**
 * @brief Affiche un texte multiligne à l'écran en gérant les sauts de ligne.
 *
 * Cette fonction parcourt une chaîne de texte, détecte les sauts de ligne (`\n` ou séquence `\\n`),
 * et affiche chaque ligne séparément à l'écran avec un décalage vertical entre elles.
 *
 * @param renderer Un pointeur vers le `SDL_Renderer` utilisé pour le rendu.
 * @param font Un pointeur vers la police `TTF_Font` utilisée pour dessiner le texte.
 * @param texte La chaîne de caractères à afficher, pouvant contenir des sauts de ligne.
 * @param x La position horizontale de départ du texte (en pixels).
 * @param y La position verticale de départ du texte (en pixels).
 *
 * @return Retourne la position `y` finale après avoir affiché toutes les lignes (utile pour continuer un rendu plus bas si besoin).
 *
 * @pre
 * - Le renderer SDL (`renderer`) doit être initialisé et valide.
 * - La police (`font`) doit être correctement chargée.
 * - Le pointeur `texte` doit être non nul et contenir une chaîne valide.
 *
 * @post
 * - Toutes les lignes du texte sont affichées à l'écran avec un décalage vertical de 25 pixels entre chaque ligne.
 *
 * @note
 * - Les sauts de ligne peuvent être interprétés sous deux formes : un vrai caractère `\n` ou la séquence de deux caractères `\\n`.
 *
 * @warning
 * - Une erreur dans la création des surfaces ou des textures peut empêcher l'affichage d'une ligne sans provoquer de crash.
 * - `strdup` est utilisé pour copier le texte, veillez à libérer correctement cette mémoire même en cas d'erreur.
 */
int afficherTexte(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y) {
    SDL_Color couleur = {255, 255, 255};
    int current_y = y;
    
    char *texte_copie = strdup(texte);
    char *ligne_start = texte_copie;
    char *ptr = texte_copie;
    
    while (*ptr) {
        // Détection des sauts de ligne (vrais \n ou séquence \n)
        if ((*ptr == '\n') || (ptr[0] == '\\' && ptr[1] == 'n')) {
            // Temporairement termine la ligne
            char old_char = *ptr;
            *ptr = '\0';
            
            // Affiche la ligne si elle n'est pas vide
            if (strlen(ligne_start) > 0) {
                SDL_Surface *surface = TTF_RenderText_Solid(font, ligne_start, couleur);
                if (surface) {
                    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                    if (texture) {
                        SDL_Rect dst = {x, current_y, surface->w, surface->h};
                        SDL_RenderCopy(renderer, texture, NULL, &dst);
                        SDL_DestroyTexture(texture);
                    }
                    current_y += 25;
                    SDL_FreeSurface(surface);
                }
            }
            
            // Restaure le caractère et avance
            *ptr = old_char;
            ligne_start = ptr + ((old_char == '\n') ? 1 : 2);
            ptr = ligne_start;
            continue;
        }
        
        ptr++;
    }
    
    // Affiche le reste du texte après le dernier saut de ligne
    if (strlen(ligne_start) > 0) {
        SDL_Surface *surface = TTF_RenderText_Solid(font, ligne_start, couleur);
        if (surface) {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture) {
                SDL_Rect dst = {x, current_y, surface->w, surface->h};
                SDL_RenderCopy(renderer, texture, NULL, &dst);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
        }
    }
    current_y += 25;
    free(texte_copie);
    return current_y;
}

/**
 * @brief Utilise un objet de l'inventaire du joueur sur un Mécha donné.
 *
 * Cette fonction applique les effets d'un objet (`carburant` ou `rappel`) sur un Mécha du joueur,
 * en modifiant ses points de vie (`pv`) et en mettant à jour l'inventaire du joueur.
 * Le résultat de l'action est stocké dans `message_buffer`.
 *
 * @param objet_id L'identifiant de l'objet à utiliser (`0` pour carburant, `1` pour rappel).
 * @param mecha Un pointeur vers la structure `mechas_joueur_t` représentant le Mécha ciblé.
 * @param j Un pointeur vers la structure `joueur_t` représentant l'état actuel du joueur.
 * @param message_buffer Un pointeur vers un tampon de caractères pour stocker le message d'information.
 *
 * @pre
 * - Les pointeurs `mecha`, `j` et `message_buffer` doivent être valides.
 * - L'inventaire du joueur (`j->inventaire`) doit être correctement initialisé.
 *
 * @post
 * - Modifie les points de vie du Mécha et les quantités d'objets dans l'inventaire en fonction de l'action effectuée.
 * - Remplit `message_buffer` avec un message indiquant le succès ou l'échec de l'opération.
 *
 * @note
 * - L'objet "carburant" restaure jusqu'à 50 PV, sans dépasser les PV maximum.
 * - L'objet "rappel" réanime un Mécha KO à la moitié de ses PV maximum.
 *
 * @warning
 * - Si les conditions d'utilisation de l'objet ne sont pas respectées, l'objet n'est pas consommé et une erreur est renvoyée dans `message_buffer`.
 * - Aucun contrôle n'est effectué sur la validité de `objet_id` en dehors des cas `0` et `1`.
 */
void utiliserObjetSurMecha(int objet_id, mechas_joueur_t *mecha, joueur_t *j, char *message_buffer) {
    switch (objet_id) {
        case 0: // Carburant
            if (j->inventaire->carburant > 0 && mecha->pv > 0 && mecha->pv < mecha->pv_max) {
                mecha->pv += 50;
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

/**
 * @brief Affiche l'écran d'information détaillée d'un Mécha du joueur.
 *
 * Cette fonction ouvre un écran présentant les caractéristiques d'un Mécha,
 * son nom, son type, son niveau, ses PV actuels, ses attaques ainsi que sa description.
 * L'affichage reste actif jusqu'à ce que le joueur appuie sur `A`, `Escape`, ou quitte la fenêtre.
 *
 * @param j Un pointeur vers la structure `joueur_t` représentant l'état actuel du joueur.
 * @param sprite_p Un pointeur vers le `SDL_Rect` représentant le sprite du joueur à afficher.
 * @param pnj_sprite Un tableau de `SDL_Rect` représentant les sprites des PNJ.
 * @param mecha_j Un pointeur vers la structure `mechas_joueur_t` représentant le Mécha sélectionné.
 * @param quitter_total Un pointeur vers un entier permettant de quitter entièrement l'affichage global.
 *
 * @pre
 * - Le `renderer` de `game` doit être initialisé.
 * - La police (`game.police`) doit être chargée.
 * - Les pointeurs `j`, `sprite_p`, `pnj_sprite` et `mecha_j` doivent être valides.
 *
 * @post
 * - Affiche les informations du Mécha jusqu'à réception d'un événement de sortie ou d'une touche spécifique.
 * - Met à jour `quitter_total` si une sortie globale est demandée (`Escape` ou `i`).
 *
 * @note
 * - Le fond d'affichage est composé de plusieurs rectangles semi-transparents pour organiser les informations.
 * - Les barres de PV sont colorées en fonction du pourcentage de vie du Mécha (vert, orange, rouge).
 * - Les attaques sont affichées avec leurs noms, dégâts, précisions et descriptions.
 *
 * @warning
 * - Si `mecha_j->id_mechas` est invalide (hors limites du tableau `mecha`), l'affichage est annulé.
 * - Une erreur dans les ressources chargées (sprites, police) peut empêcher le rendu correct.
 */
void afficherInfosMecha(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, mechas_joueur_t* mecha_j, int *quitter_total) {
    SDL_Event event;
    int quitter = 0;
    int id = mecha_j->id_mechas;
    if (id <= 0 || id > NB_MECHAS) return;

    while (!quitter && !*quitter_total) {
        Uint32 frameStart = SDL_GetTicks();

        SDL_RenderClear(game.renderer);
        draw_all(j, sprite_p, pnj_sprite);

        // Fond noir + bleu avec tes rectangles
        rectangle_t fondNoir;
        creer_rectangle(&fondNoir, 440, 700, game.dms_win.x + game.dms_win.w - 460, 40, 0, 0, 0, 180, "");
        draw_all_rect(1, &fondNoir);

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

        int pv = mecha_j->pv;
        int pv_max = mecha_j->pv_max;
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
        y_info=afficherTexte(game.renderer, game.police, buffer, fondX + 30, y_info);

        sprintf(buffer, "Niveau : %d", mecha_j->niveau);
        y_info=afficherTexte(game.renderer, game.police, buffer, fondX + 30, y_info);

        y_info=afficherTexte(game.renderer, game.police, "Description :", fondX + 30, y_info);

        y_info=afficherTexte(game.renderer, game.police, mecha[id - 1].description, fondX + 30, y_info);

        sprintf(buffer,"Attaque 1: %s \nDegats: %d \nPrecision: %d\nDescription: %s",
        attaque[mecha_j->attaque_1 -1].nom,attaque[mecha_j->attaque_1 -1].degats,attaque[mecha_j->attaque_1 -1].precision
        ,attaque[mecha_j->attaque_1 -1].description);
        y_info=afficherTexte(game.renderer, game.police,buffer, fondX + 50, y_info);
         y_info += 25;
        sprintf(buffer,"Attaque 2: %s \nDegats: %d \nPrecision: %d\nDescription: %s",
        attaque[mecha_j->attaque_2 -1].nom,attaque[mecha_j->attaque_2 -1].degats,attaque[mecha_j->attaque_2 -1].precision
        ,attaque[mecha_j->attaque_2 -1].description);
        y_info=afficherTexte(game.renderer, game.police, buffer, fondX + 50, y_info);
         y_info += 25;


        // Footer centré
        const char *footer = "Appuyez sur 'A' pour revenir";
        int footerWidth = strlen(footer) * 9;
        int footerX = fondX + (440 - footerWidth) / 2;
        y_info=afficherTexte(game.renderer, game.police, footer, footerX, 700);

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

/**
 * @brief Affiche le Mechadex du joueur, listant tous ses Méchas capturés.
 *
 * Cette fonction affiche une liste des Méchas du joueur avec leurs informations principales (nom, PV),
 * et permet de consulter les détails d'un Mécha sélectionné ou de quitter l'affichage.
 *
 * @param j Un pointeur vers la structure `joueur_t` représentant l'état actuel du joueur.
 * @param sprite_p Un pointeur vers le `SDL_Rect` représentant le sprite du joueur.
 * @param pnj_sprite Un tableau de `SDL_Rect` représentant les sprites des PNJ.
 * @param game Pointeur vers la structure du jeu contenant le renderer SDL.
 * @param quitter_total Un pointeur vers un entier indiquant si la fermeture complète de l'affichage est demandée.
 *
 * @pre
 * - Le `renderer` et la `police` de `game` doivent être initialisés.
 * - Les pointeurs `j`, `sprite_p`, `pnj_sprite` et `game` doivent être valides.
 *
 * @post
 * - Affiche tous les Méchas du joueur sous forme de liste scrollable.
 * - Permet de consulter les informations détaillées d'un Mécha via la touche `Entrée`.
 * - Met à jour `quitter_total` si une sortie globale est demandée (`Escape` ou `i`).
 *
 * @note
 * - La sélection est mise en valeur avec un cadre jaune.
 * - Le joueur peut naviguer dans la liste avec les flèches haut/bas.
 * - La touche `A` permet de revenir au jeu principal.
 *
 * @warning
 * - Si un Mécha du joueur a un ID invalide, des erreurs d'affichage peuvent survenir.
 * - Un mauvais chargement des textures ou de la police peut entraîner des erreurs SDL.
 */
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
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 200);
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
                        afficherInfosMecha(j, sprite_p, pnj_sprite, &(j->mechas_joueur[selection]), quitter_total);
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

/**
 * @brief Affiche l'écran de sélection d'un Mécha pour utiliser un objet.
 *
 * Cette fonction permet au joueur de parcourir ses Méchas et de sélectionner celui
 * sur lequel utiliser un objet (comme un carburant ou un rappel). Le joueur peut naviguer,
 * choisir un Mécha, ou quitter l'écran.
 *
 * @param j Un pointeur vers la structure `joueur_t` représentant l'état actuel du joueur.
 * @param sprite_p Un pointeur vers le `SDL_Rect` représentant le sprite du joueur.
 * @param pnj_sprite Un tableau de `SDL_Rect` représentant les sprites des PNJ.
 * @param game Pointeur vers la structure du jeu contenant le renderer SDL.
 * @param objet_id L'identifiant de l'objet à utiliser (`0` pour carburant, `1` pour rappel, etc.).
 * @param quitter_total Un pointeur vers un entier permettant de quitter complètement l'affichage si nécessaire.
 *
 * @pre
 * - Le `renderer` et la `police` de `game` doivent être initialisés.
 * - Les pointeurs `j`, `sprite_p`, `pnj_sprite` et `game` doivent être valides.
 *
 * @post
 * - Permet de sélectionner un Mécha et d'appliquer l'objet spécifié.
 * - Affiche un message de succès ou d'échec après utilisation d'un objet.
 * - Met à jour `quitter_total` si une sortie globale est demandée (`Escape` ou `i`).
 *
 * @note
 * - La sélection est encadrée visuellement avec un rectangle jaune.
 * - Le joueur peut naviguer avec `Flèche Haut` et `Flèche Bas`, utiliser un objet avec `Entrée`, et quitter avec `A`.
 *
 * @warning
 * - Si l'objet ne peut pas être utilisé (conditions non remplies), un message d'erreur est affiché.
 * - Le système ne vérifie pas la validité de l'`objet_id` en dehors des cas prévus dans `utiliserObjetSurMecha`.
 */
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
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 200);
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

/**
 * @brief Affiche l'inventaire du joueur et permet d'utiliser certains objets.
 *
 * Cette fonction affiche une interface listant les objets disponibles dans l'inventaire du joueur,
 * permettant la navigation, la consultation des quantités, et l'utilisation directe de certains objets (comme le repousse).
 *
 * @param j Un pointeur vers la structure `joueur_t` représentant l'état actuel du joueur.
 * @param sprite_p Un pointeur vers le `SDL_Rect` représentant le sprite du joueur.
 * @param pnj_sprite Un tableau de `SDL_Rect` représentant les sprites des PNJ.
 * @param game Pointeur vers la structure du jeu contenant le renderer SDL.
 * @param repousse Un pointeur vers un entier représentant la durée d'effet du repousse actif.
 *
 * @return Retourne `0` après avoir quitté l'inventaire.
 *
 * @pre
 * - Le `renderer` et la `police` de `game` doivent être initialisés.
 * - Les pointeurs `j`, `sprite_p`, `pnj_sprite`, `game` et `repousse` doivent être valides.
 *
 * @post
 * - Affiche les objets de l'inventaire du joueur avec leurs quantités et descriptions.
 * - Permet d'utiliser un objet de soin ou un repousse selon le contexte.
 *
 * @note
 * - La navigation dans l'inventaire se fait avec les flèches haut et bas.
 * - `Entrée` permet d'utiliser un objet ou d'interagir selon l'objet sélectionné.
 * - La touche `A` permet d'ouvrir directement le Mechadex.
 *
 * @warning
 * - L'utilisation d'un objet dépend de la disponibilité dans l'inventaire et de l'état du joueur.
 * - L'affichage suppose que les images des objets existent aux chemins spécifiés, sinon un carré coloré est affiché.
 */
int afficherInventaire(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, game_t *game, int *repousse) {
    int selection = 0;
    int quitter = 0;
    int quitter_total = 0;
    SDL_Event event;
    char buffer[100];
    int verif = 0;

    const char *noms[] = {"Carburant", "Rappel", "Mechaball", "Repousse"};
    const char *descriptions[] = {
        "Soigne de 50 PV",
        "Ranime un mecha K.O.",
        "Permet de capturer un mecha",
        "Repousse les combats"
    };

    const char *chemins_img[] = {
        "img/inventaire/carburant.png",
        "img/inventaire/rappel.png",
        NULL,
        "img/inventaire/repousse.png"
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

        rectangle_t fondNoir;
        creer_rectangle(&fondNoir, 440, 700, game->dms_win.x + game->dms_win.w - 460, 40, 0, 0, 0, 180, "");
        draw_all_rect(1, &fondNoir);

        int fondX = fondNoir.rect.x;

        // Titre
        const char *titre = "Inventaire";
        int titreWidth = strlen(titre) * 9;
        int titreX = fondX + (440 - titreWidth) / 2;
        afficherTexte(game->renderer, game->police, titre, titreX, 60);

        for (int i = 0; i < NB_OBJET - 1; i++) {
            rectangle_t itemBox;
            creer_rectangle(&itemBox, 420, 110, fondX + 10, 90 + i * 130, 0, 0, 0, 200, "");
            draw_all_rect(1, &itemBox);

            if (i == selection) {
                SDL_SetRenderDrawColor(game->renderer, 255, 255, 0, 255);
                SDL_RenderDrawRect(game->renderer, &itemBox.rect);
            }

            SDL_Rect imgRect = {fondX + 20, 100 + i * 130, 80, 80};

            if (chemins_img[i]) {
                SDL_Texture *img = charger_texture(chemins_img[i]);
                if (img) {
                    SDL_RenderCopy(game->renderer, img, NULL, &imgRect);
                    SDL_DestroyTexture(img);
                }
            } else {
                SDL_SetRenderDrawColor(game->renderer, 0, 200, 0, 255);
                SDL_RenderFillRect(game->renderer, &imgRect);
            }

            afficherTexte(game->renderer, game->police, noms[i], fondX + 110, 105 + i * 130);
            afficherTexte(game->renderer, game->police, descriptions[i], fondX + 110, 135 + i * 130);

            sprintf(buffer, "x%d", *quantites[i]);
            afficherTexte(game->renderer, game->police, buffer, fondX + 360, 105 + i * 130);
        }

        const char *footer = "Appuyez sur 'A' pour le MechaDex";
        int footerWidth = strlen(footer) * 9;
        int footerX = fondX + (440 - footerWidth) / 2;
        afficherTexte(game->renderer, game->police, footer, footerX, 670);

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
                                verif = 1;
                            }
                        } else if (selection == 3) { // Repousse
                            if (j->inventaire->repousse > 0) {
                                const char *msg = "Repousse utilise avec succes !";
                                int msgWidth = strlen(msg) * 9;
                                int msgX = fondX + (440 - msgWidth) / 2;
                                afficherTexte(game->renderer, game->police, msg, msgX, 620);
                                *repousse += 50;
                                j->inventaire->repousse--;
                                verif = 1;
                            } else {
                                const char *msg = "Aucun repousse disponible.";
                                int msgWidth = strlen(msg) * 9;
                                int msgX = fondX + (440 - msgWidth) / 2;
                                afficherTexte(game->renderer, game->police, msg, msgX, 620);
                                verif = 1;
                            }
                        } else {
                            const char *msg = "Utilisable uniquement en combat";
                            int msgWidth = strlen(msg) * 9;
                            int msgX = fondX + (440 - msgWidth) / 2;
                            afficherTexte(game->renderer, game->police, msg, msgX, 620);
                            verif = 1;
                        }
                        break;
                }
            }
        }
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderPresent(game->renderer);
        if (verif) SDL_Delay(1000);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (1000 / 60 > frameTime)
            SDL_Delay((1000 / 60) - frameTime);

        verif = 0;
    }

    return 0;
}

/**
 * @brief Affiche le PC du joueur permettant de gérer et échanger les Méchas capturés.
 *
 * Cette fonction affiche l'ensemble des Méchas capturés par le joueur (hors équipe principale),
 * permet de naviguer entre différentes pages, de sélectionner un Mécha, et de l'échanger avec un Mécha actif.
 *
 * @param joueur Un pointeur vers la structure `joueur_t` représentant le joueur et ses Méchas capturés.
 *
 * @return Retourne `0` après fermeture de l'interface PC.
 *
 * @pre
 * - Le `renderer` et les textures de `game` doivent être initialisés.
 * - Le pointeur `joueur` doit être valide et son tableau de Méchas correctement rempli.
 *
 * @post
 * - Affiche dynamiquement la liste paginée des Méchas capturés.
 * - Permet l'échange d'un Mécha capturé avec un Mécha actif de l'équipe du joueur.
 *
 * @note
 * - 50 emplacements de stockage sont disponibles (5 pages de 10 Méchas affichés).
 * - La sélection est visuellement mise en évidence par un changement de couleur.
 * - Le joueur utilise les touches `←` `→` pour se déplacer, `P`/`S` pour changer de page, `A` pour échanger, `Échap` pour quitter.
 *
 * @warning
 * - Aucun contrôle n'est effectué sur l'intégrité des données Mécha au moment de l'échange.
 * - En cas de données invalides dans `mechas_joueur`, des erreurs d'affichage ou de logique peuvent apparaître.
 */
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

/**
 * @brief Concatène un entier à la fin d'une chaîne de caractères.
 *
 * Cette fonction transforme un entier en chaîne de caractères, puis l'ajoute
 * à la fin de la chaîne `dest`.
 *
 * @param dest Un pointeur vers la chaîne de caractères à modifier.
 * @param nb L'entier à convertir et à concaténer à `dest`.
 *
 * @pre
 * - `dest` doit être un pointeur valide vers une chaîne de caractères suffisamment grande pour accueillir le résultat.
 *
 * @post
 * - La chaîne `dest` est modifiée pour inclure la valeur numérique de `nb` à sa fin.
 *
 * @note
 * - Utilise `sprintf` pour convertir l'entier en texte, puis `strcat` pour concaténer.
 *
 * @warning
 * - Aucune vérification n'est faite sur la taille de `dest`, ce qui peut provoquer un débordement si l'espace disponible est insuffisant.
 */
void concat(char *dest, int nb) { //concatene un entier a une chaine de caractere
    char tmp[10];
    sprintf(tmp, "%d", nb);
    strcat(dest, tmp);
}
