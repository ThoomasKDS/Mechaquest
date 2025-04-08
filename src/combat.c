/**
 * \file combat.c
 * \brief fichier contenant la fonctions de combat
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "../lib/combat.h"
#include "../lib/sauv.h"

#define N 2

const int FPS_LIMITS = 60;
const int FRAME_DELAYS = 1000 / FPS_LIMITS; // Temps entre chaque frame (16 ms)

char nom[50] = "player1";
mechas_joueur_t ordi;
joueur_t joueur;

rectangle_t rect_bas; //fond des boutons


/**
 * @brief Initialise les dimensions et la couleur du rectangle inférieur utilisé comme fond des boutons d'action en combat.
 *
 * Cette fonction définit le rectangle situé en bas de l'écran, qui sert de support graphique pour afficher les boutons d'interaction durant les phases de combat. Elle calcule dynamiquement la taille et la position en fonction des dimensions actuelles de la fenêtre du jeu.
 *
 * - La largeur du rectangle correspond à la largeur complète de la fenêtre.
 * - La hauteur du rectangle est définie comme 1/4 de la hauteur totale de l'écran.
 * - Le rectangle est positionné tout en bas de l'écran.
 * - La couleur du rectangle est définie en noir semi-transparent (RGBA : 0, 0, 0, 150).
 *
 * @note Cette fonction utilise les attributs globaux (`game.renderer` et `rect_bas`) définis en dehors de la fonction.
 *
 * @warning Cette fonction est statique, ce qui signifie qu'elle est uniquement accessible à l'intérieur du fichier source (`combat.c`) où elle est définie.
 *
 * @pre Le renderer SDL (`game.renderer`) doit être initialisé avant l'appel de cette fonction.
 * @post Le rectangle inférieur (`rect_bas`) est prêt à être rendu à l'écran.
 */
static void init_rect_bas() {            
    int win_w, win_h;
    SDL_GetRendererOutputSize(game.renderer, &win_w, &win_h);       //dimensions ecran
    // Définition de rect_bas (fond des boutons)
    rect_bas.rect.w = win_w;
    rect_bas.rect.h = win_h / 4;
    rect_bas.rect.x = 0;
    rect_bas.rect.y = win_h - rect_bas.rect.h;
    rect_bas.couleur.r = 0;
    rect_bas.couleur.g = 0;
    rect_bas.couleur.b = 0;
    rect_bas.couleur.a = 150;
}

/**
 * @brief Affiche à l'écran tous les éléments graphiques d'un combat entre deux Mechas.
 *
 * Cette fonction gère l'affichage complet d'un combat, incluant :
 * - Les noms et niveaux des Mechas en combat.
 * - Les points de vie (PV) des deux Mechas sous forme de barres graphiques.
 * - Les textures des Mechas du joueur et de l'ordinateur.
 * - Les rectangles décoratifs servant de fond à l'affichage des informations.
 *
 * @param mecha_joueur Pointeur vers la structure représentant le Mecha du joueur.
 * @param mecha_ordi   Pointeur vers la structure représentant le Mecha adverse contrôlé par l'ordinateur.
 *
 * @pre Les ressources graphiques (`game.renderer`, textures des Mechas, rectangles graphiques) doivent être chargées et initialisées correctement avant l'appel de cette fonction.
 *
 * @post L'écran affiche toutes les informations nécessaires à la compréhension visuelle du combat en cours.
 *
 * @note Cette fonction utilise des variables globales (notamment `game`, `rect_bas`, `mecha[]`) définies ailleurs dans le programme.
 *
 * @warning Assure-toi que les structures `mecha_joueur` et `mecha_ordi` contiennent des informations valides afin d'éviter tout problème d'affichage ou de segmentation.
 */
void draw_combat( mechas_joueur_t * mecha_joueur, mechas_joueur_t * mecha_ordi) {
    int win_w, win_h;
    SDL_GetRendererOutputSize(game.renderer, &win_w, &win_h);       //dimensions ecran

    //texte à afficher
    int pv_mecha_joueur = mecha_joueur->pv;
    int pv_mecha_ordi = mecha_ordi->pv;
    char nom_mecha_joueur[50];
    strcpy(nom_mecha_joueur, mecha[mecha_joueur->id_mechas - 1].nom);
    strcat(nom_mecha_joueur, " Niv.");
    concat(nom_mecha_joueur, mecha_joueur->niveau);
    char nom_mecha_ordi[50];
    strcpy(nom_mecha_ordi, mecha[mecha_ordi->id_mechas - 1].nom);
    strcat(nom_mecha_ordi, " Niv.");
    concat(nom_mecha_ordi, mecha_ordi->niveau);
    char texte_joueur[256];
    char texte_ordi[256];
    strcpy(texte_joueur, "Pv : ");
    strcpy(texte_ordi, "Pv : ");


    rectangle_t rect_joueur, rect_ordi, rect_bordure_joueur, rect_bordure_ordi, rect_pv_joueur, rect_pv_ordi, border_pv_joueur, border_pv_ordi, fond_pv_joueur, fond_pv_ordi;

    int bordure_size = 25 * game.scale;
    int w = win_w / 8;
    int h = win_h / 14;
    int x_joueur = 0 + win_w / 12;
    int y = win_h / 10;
    int x_ordi = win_w - win_w / 12 - w;
    int h_pv = win_h / 60;
    int w_fond_pv = win_w / 10;
    int w_pv_joueur = win_w / 10 * pv_mecha_joueur / mecha_joueur->pv_max;
    int w_pv_ordi = win_w / 10 * pv_mecha_ordi / mecha_ordi->pv_max;
    int x_pv_joueur = x_joueur + w/2 - w_fond_pv/2;
    int x_pv_ordi = x_ordi + w/2 - w_fond_pv/2;
    int y_pv = y + win_h / 30;
    int x_mecha_g = win_w / 10;
    int x_mecha_d  = win_w / 8 * 5;
    int y_mecha = win_h / 2 - win_h / 10;
    int w_mecha = win_w/4;
    int h_mecha = win_h/3;
    int border_pv = 3 * game.scale;

    creer_rectangle(&rect_joueur, w, h, x_joueur, y, 160, 160, 160, 255, NULL);
    creer_rectangle(&rect_ordi, w, h, x_ordi, y, 160, 160, 160, 255, NULL);
    creer_rectangle(&rect_bordure_joueur, w, bordure_size, x_joueur, y - bordure_size,160, 160, 160, 255, nom_mecha_joueur);
    creer_rectangle(&rect_bordure_ordi, w, bordure_size, x_ordi, y - bordure_size, 160, 160, 160, 255, nom_mecha_ordi);
    creer_rectangle(&rect_pv_joueur, w_pv_joueur, h_pv, x_pv_joueur, y_pv, 0, 255, 0, 255, NULL);
    creer_rectangle(&rect_pv_ordi, w_pv_ordi, h_pv, x_pv_ordi, y_pv, 0, 255, 0, 255, NULL);
    creer_rectangle(&fond_pv_joueur, w_fond_pv, h_pv, x_pv_joueur, y_pv, 116, 117, 117, 255, NULL);
    creer_rectangle(&fond_pv_ordi, w_fond_pv, h_pv, x_pv_ordi, y_pv, 116, 117, 117, 255, NULL);
    creer_rectangle(&border_pv_joueur, w_fond_pv + 2 * border_pv, h_pv + 2 * border_pv, x_pv_joueur - border_pv, y_pv - border_pv, 0, 0, 0, 255, NULL);
    creer_rectangle(&border_pv_ordi, w_fond_pv + 2 * border_pv, h_pv + 2 * border_pv, x_pv_ordi - border_pv, y_pv - border_pv, 0, 0, 0, 255, NULL);
    draw_background();
    draw_mecha(&mecha[mecha_joueur->id_mechas - 1], x_mecha_g, y_mecha, h_mecha, w_mecha, 0);
    draw_mecha(&mecha[mecha_ordi->id_mechas - 1], x_mecha_d, y_mecha, h_mecha, w_mecha, 1);
    draw_all_rect(11, &rect_bas,&rect_bordure_joueur,&rect_bordure_ordi ,&rect_joueur, &rect_ordi, &border_pv_joueur, &border_pv_ordi,&fond_pv_joueur, &fond_pv_ordi, &rect_pv_joueur, &rect_pv_ordi);
    draw_text_pos(texte_joueur, x_pv_joueur, y);
    draw_text_pos(texte_ordi, x_pv_ordi, y);
    
    

}

/**
 * @brief Anime visuellement la diminution progressive des PV d'un Mecha durant un combat.
 *
 * Cette fonction génère une animation fluide montrant la baisse progressive des points de vie (PV) après une attaque réussie. 
 * Elle ajuste dynamiquement la vitesse de l'animation en fonction des dégâts subis et limite sa durée maximale à 2 secondes 
 * pour maintenir le rythme du combat.
 *
 * @param mecha_att Indique quel Mecha reçoit les dégâts :
 *                  - `0` pour le Mecha du joueur.
 *                  - `1` pour le Mecha adverse contrôlé par l'ordinateur.
 * @param pv_old    Valeur initiale des PV avant les dégâts.
 * @param pv_new    Valeur finale des PV après les dégâts.
 * @param mecha_joueur Pointeur vers le Mecha du joueur en combat.
 * @param mecha_ordi   Pointeur vers le Mecha adverse contrôlé par l'ordinateur.
 *
 * @pre Les ressources graphiques (`game.renderer`, `mecha[]`, textures, etc.) doivent être chargées et initialisées avant l'appel.
 *
 * @post Le Mecha concerné affiche la nouvelle valeur de PV, avec une animation fluide entre les deux états.
 *
 * @note La durée maximale de l'animation est limitée à 2 secondes pour éviter une attente excessive.
 *
 * @warning La fonction est bloquante pendant la durée de l'animation (utilise `SDL_Delay`). Elle doit donc être utilisée à bon escient pour préserver la fluidité générale du jeu.
 */
void animation_degat(int mecha_att, int pv_old, int pv_new, mechas_joueur_t *mecha_joueur, mechas_joueur_t *mecha_ordi) {
    int temp_pv = pv_old;
    int diff_pv = pv_old - pv_new;
    const int duree_max = 2000; // Durée max de l'animation en ms (2 secondes)
    
    if (diff_pv <= 0) return; // Pas d'animation si aucun dégât

    int frame_time = duree_max / diff_pv; // Temps par étape
    if (frame_time < 10) frame_time = 10; // Limite minimale

    Uint32 start_time = SDL_GetTicks();
    
    while (temp_pv > pv_new) {
        Uint32 frameStart = SDL_GetTicks(); // Début de la frame
        
        SDL_RenderClear(game.renderer);

        if (!mecha_att) {
            mecha_joueur->pv = temp_pv;
        } else {
            mecha_ordi->pv = temp_pv;
        }

        draw_combat(mecha_joueur, mecha_ordi);
        SDL_RenderPresent(game.renderer);

        Uint32 elapsed = SDL_GetTicks() - start_time;
        if (elapsed >= duree_max) break; // Arrêt si l'animation dépasse la durée max

        SDL_Delay(frame_time);
        temp_pv--;

        // Gestion du FPS
        int frameTime = SDL_GetTicks() - frameStart;
        const int FRAME_DELAYS = 16; // ~60 FPS (1000ms / 60)
        if (FRAME_DELAYS > frameTime) {
            SDL_Delay(FRAME_DELAYS - frameTime);
        }
    }

    // Mise à jour finale des PV
    if (!mecha_att) mecha_joueur->pv = pv_new;
    else mecha_ordi->pv = pv_new;
}



/**
 * @brief Affiche l'interface de sélection du Mecha du joueur durant un combat.
 *
 * Cette fonction permet au joueur de sélectionner visuellement un Mecha parmi les quatre disponibles dans son équipe, ou bien de revenir au menu précédent (option "Retour"). 
 * Elle affiche les Mechas disponibles avec leurs informations clés (nom, niveau, PV) sous forme de rectangles interactifs, ainsi qu'un bouton retour. 
 *
 * La sélection est réalisée via les touches directionnelles (gauche/droite), et validée avec la touche `A`.
 *
 * @param joueur Pointeur vers la structure du joueur contenant l'équipe de Mechas à afficher.
 *
 * @return int
 *         - L'indice du Mecha sélectionné (0 à 3) si la sélection est validée.
 *         - `KO` si le joueur choisit l'option "Retour".
 *
 * @pre Le renderer SDL (`game.renderer`), les textures des Mechas, ainsi que les structures du joueur doivent être initialisés.
 *
 * @post Le Mecha sélectionné est prêt à être utilisé dans le combat, ou la fonction retourne au menu précédent.
 *
 * @note Cette fonction bloque l'exécution en attendant que l'utilisateur effectue une sélection.
 *
 * @warning Les informations des Mechas doivent être valides pour éviter les erreurs d'affichage.
 *          Assure-toi également que les constantes globales comme `FRAME_DELAYS` sont correctement définies.
 */
int aff_mechas_combat(joueur_t * joueur) {
    int sauv = game.mat_active;
    game.mat_active = 8;
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    int running = 1;
    int win_w, win_h;
    SDL_GetRendererOutputSize(game.renderer, &win_w, &win_h);       //dimensions ecran
    int choix = 0;
    

    //texte à afficher
    char texte_mecha[4][256];

    int existe[4] = {0, 0, 0, 0};
    for(int i = 0; i < 4; i++){
        if(joueur->mechas_joueur[i].numero == i + 1 && i < joueur->nb_mechas){
            existe[i] = 1;
            strcpy(texte_mecha[i], mecha[joueur->mechas_joueur[i].id_mechas - 1].nom);
            strcat(texte_mecha[i], "\nNiveau : ");
            concat(texte_mecha[i], joueur->mechas_joueur[i].niveau);
            strcat(texte_mecha[i], "\nPV : ");
            concat(texte_mecha[i], joueur->mechas_joueur[i].pv);
            strcat(texte_mecha[i], "/");
            concat(texte_mecha[i], joueur->mechas_joueur[i].pv_max);
        }
        else{
            strcpy(texte_mecha[i], "\0");
        }
    }

    //couleur des bordures
    SDL_Color couleur_bordure =  {94, 99, 102, 250};
    SDL_Color couleur_bordure_selec = {0, 0, 0, 230};
    
    //declaration des rectangles
    rectangle_t rect1, rect2, rect3, rect4, rect_retour;
    rectangle_t rect_bodure1, rect_bordure2, rect_bordure3, rect_bordure4, rect_bordure_retour;

    //initialisation des rectangles
    int rect_w = win_w * 0.3;
    int rect_h = win_h * 0.2;
    int margin = 50;  // Ajuste la marge selon tes besoins

    // Position et taille du 5e rectangle 
    int rect5_w = 2 * rect_w + margin;
    int rect5_h = rect_h;
    int x5 = (win_w - rect5_w) / 2;  // Centré
    int y5 = win_h - rect_h - margin;  // Juste en dessous des 4 rects

    int border_size = 5 * game.scale; // Épaisseur des bords

    int x1 = 0 + win_w * 0.1;
    int y1 = win_h * 0.1;

    int x2 = 0 + win_w * 0.1;
    int y2 = win_h * 0.1 + rect_h + margin;

    int x3 = win_w - win_w * 0.1 - rect_w;
    int y3 = win_h * 0.1;

    int x4 = win_w - win_w * 0.1 - rect_w;
    int y4 = win_h * 0.1 + rect_h + margin;

    int w = win_w * 0.08;
    int h = win_h * 0.1;

    //Creation des rectangles
    creer_rectangle(&rect1, rect_w, rect_h, x1, y1, 255, 255, 255, 255, texte_mecha[0]);
    creer_rectangle(&rect2, rect_w, rect_h, x2, y2, 255, 255, 255, 255, texte_mecha[1]);
    creer_rectangle(&rect3, rect_w, rect_h, x3, y3, 255, 255, 255, 255, texte_mecha[2]);
    creer_rectangle(&rect4, rect_w, rect_h, x4, y4, 255, 255, 255, 255, texte_mecha[3]);
    creer_rectangle(&rect_retour, rect5_w, rect5_h, x5, y5, 255, 255, 255, 255, "Retour");
    

    //bordure des rectangles
    creer_rectangle(&rect_bodure1, rect_w + 2 * border_size, rect_h + 2 * border_size, x1 - border_size, y1 - border_size, 255, 255, 255, 230, NULL);
    creer_rectangle(&rect_bordure2, rect_w + 2 * border_size, rect_h + 2 * border_size, x2 - border_size, y2 - border_size, 94, 99, 102, 250, NULL);
    creer_rectangle(&rect_bordure3, rect_w + 2 * border_size, rect_h + 2 * border_size, x3 - border_size, y3 - border_size, 94, 99, 102, 250, NULL);
    creer_rectangle(&rect_bordure4, rect_w + 2 * border_size, rect_h + 2 * border_size, x4 - border_size, y4 - border_size, 94, 99, 102, 250, NULL);
    creer_rectangle(&rect_bordure_retour, rect5_w + 2 * border_size, rect5_h + 2 * border_size, x5 - border_size, y5 - border_size, 94, 99, 102, 250, NULL);

    while(running) {
        frameStart = SDL_GetTicks();
        
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_LEFT){
                    choix = (choix - 1 + 5) % 5;
                }
                if(event.key.keysym.sym == SDLK_RIGHT){
                    choix = (choix + 1) % 5;
                }
                if(event.key.keysym.sym == SDLK_a){
                    if(existe[choix]){
                        game.mat_active = sauv;
                        return choix;
                    }
                }
                SDL_Event e;
                while (SDL_WaitEvent(&e) && e.type != SDL_KEYUP);
            }
        }
        switch(choix) {
            case 0 : rect_bodure1.couleur = couleur_bordure_selec; 
            rect_bordure2.couleur = couleur_bordure;
            rect_bordure3.couleur = couleur_bordure;
            rect_bordure4.couleur = couleur_bordure;
            rect_bordure_retour.couleur = couleur_bordure;
            break;
            case 1 : rect_bodure1.couleur = couleur_bordure;
            rect_bordure2.couleur = couleur_bordure_selec;
            rect_bordure3.couleur = couleur_bordure;
            rect_bordure4.couleur = couleur_bordure;
            rect_bordure_retour.couleur = couleur_bordure;
            break;
            case 2 : rect_bodure1.couleur = couleur_bordure;
            rect_bordure2.couleur = couleur_bordure;
            rect_bordure3.couleur = couleur_bordure_selec;
            rect_bordure4.couleur = couleur_bordure;
            rect_bordure_retour.couleur = couleur_bordure;
            break;
            case 3 : rect_bodure1.couleur = couleur_bordure;
            rect_bordure2.couleur = couleur_bordure;
            rect_bordure3.couleur = couleur_bordure;
            rect_bordure4.couleur = couleur_bordure_selec;
            rect_bordure_retour.couleur = couleur_bordure;
            break;
            case 4 : rect_bodure1.couleur = couleur_bordure;
            rect_bordure2.couleur = couleur_bordure;
            rect_bordure3.couleur = couleur_bordure;
            rect_bordure4.couleur = couleur_bordure;
            rect_bordure_retour.couleur = couleur_bordure_selec;
            break;
        }
        SDL_RenderClear(game.renderer);
        draw_background();
        draw_all_rect(10,&rect_bodure1, &rect_bordure2, &rect_bordure3, &rect_bordure4, &rect_bordure_retour, &rect1, &rect2, &rect3, &rect4, &rect_retour);
        
        if(existe[0]) {
            draw_mecha(&mecha[joueur->mechas_joueur[0].id_mechas - 1], x1, y1 + (rect_h / 2) - (h/2), h, w, 0);   
        }
        if(existe[1]) {
            draw_mecha(&mecha[joueur->mechas_joueur[1].id_mechas - 1], x2, y2 + (rect_h / 2) - (h/2), h, w, 0);
        }   
        if(existe[2]) {
            draw_mecha(&mecha[joueur->mechas_joueur[2].id_mechas - 1], x3, y3 + (rect_h / 2) - (h/2), h, w, 0);
        }
        if(existe[3]) {
            draw_mecha(&mecha[joueur->mechas_joueur[3].id_mechas - 1], x4, y4 + (rect_h / 2) - (h/2), h, w, 0);
        }
        
        SDL_RenderPresent(game.renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAYS > frameTime) SDL_Delay(FRAME_DELAYS - frameTime);
    }
    game.mat_active = sauv;
    return KO;
}

/*
===========================================
FONCTIONS UTILISATION OBJET
===========================================
*/

/**
 * @brief Affiche l'interface d'utilisation d'objets pendant un combat.
 *
 * Cette fonction permet au joueur de choisir et d'utiliser des objets spécifiques lors d'un combat entre son Mecha et un adversaire (Mecha sauvage ou combat scénarisé). 
 * Les objets disponibles sont :
 * - **Carburant** : Restaure les PV d'un Mecha.
 * - **Rappel** : Réanime un Mecha KO.
 * - **Mechaball** : Permet de capturer un Mecha sauvage (uniquement dans certains types de combat).
 * - **Retour** : Revenir au menu précédent.
 *
 * Le joueur navigue entre ces choix à l'aide des touches directionnelles gauche/droite et valide sa sélection avec la touche `A`.
 *
 * @param joueur Pointeur vers la structure du joueur en combat.
 * @param ordi   Pointeur vers le Mecha adverse en combat.
 * @param actif  Pointeur vers l'indice du Mecha actif du joueur actuellement utilisé en combat.
 * @param type_combat Type du combat en cours :
 *                    - `0` : Combat contre un Mecha sauvage (capture possible).
 *                    - `1` : Combat scénarisé (capture impossible).
 *
 * @return int
 *         - `OK` : Objet utilisé avec succès ou retour effectué.
 *         - `CAPTURE` : Capture réussie du Mecha sauvage avec une Mechaball.
 *         - `FAUX` : La capture a échoué, le combat continue.
 *         - `RETOUR` : L'utilisateur a choisi de revenir en arrière sans utiliser d'objet.
 *
 * @pre Les ressources graphiques (`game.renderer`, rectangles graphiques, Mechas, etc.) doivent être correctement initialisées.
 *
 * @post Le combat est mis à jour selon l'effet de l'objet choisi.
 *
 * @note La fonction bloque l'exécution jusqu'à ce que l'utilisateur fasse une sélection valide ou annule l'action.
 *
 * @warning Veille à ce que le joueur dispose bien des objets dans son inventaire avant leur utilisation pour éviter des résultats inattendus.
 */
int utilisation_objet(joueur_t *joueur, mechas_joueur_t *ordi, int *actif, int type_combat){
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    int running = 1;
    int win_w, win_h;
    int res;
    SDL_GetRendererOutputSize(game.renderer, &win_w, &win_h);       //dimensions ecran

    //texte à afficher
    char texte_carburant[100] = "Carburant : ";
    char texte_rappel[100] = "Rappel : ";
    char texte_mechaball[100] = "Mechaball : ";


    //stocke du joueur
    int nb_carburant = joueur->inventaire->carburant;
    int nb_rappel = joueur->inventaire->rappel;
    int nb_mechaball = joueur->inventaire->mechaball;

    concat(texte_carburant, nb_carburant);
    concat(texte_rappel, nb_rappel);
    concat(texte_mechaball, nb_mechaball);

    //Couleurs des bordures
    SDL_Color couleur_bordure =  {94, 99, 102, 250};
    SDL_Color couleur_bordure_carburant = {210, 145, 132, 230};
    SDL_Color couleur_bordure_rappel = {229, 185, 135, 230};
    SDL_Color couleur_bordure_mechaball = {162, 202, 154, 230};
    SDL_Color couleur_bordure_retour = {255, 255, 255, 230};

    // Définition des tailles des boutons
    int btn_w = rect_bas.rect.w / 6; // Largeur des boutons (1/6 de l'écran)
    int btn_h = rect_bas.rect.h / 2; // Hauteur des boutons (1/2 du rect_bas)
    int border_size = 5 * game.scale; // Épaisseur des bords

    // Calcul de l'espacement entre les boutons
    int espacement = ((rect_bas.rect.w - (4 * btn_w)) / 3) * 0.7;

    // Positionnement des boutons 
    int btn_center_y = rect_bas.rect.y + (rect_bas.rect.h - btn_h) / 2;
    int total_width = (4 * btn_w) + (3 * espacement);  // Largeur totale de la ligne
    int btn_carbu_x = (rect_bas.rect.w - total_width) / 2; // Centre le premier bouton
    int btn_rappel_x = btn_carbu_x + btn_w + espacement;
    int btn_mechaball_x = btn_rappel_x + btn_w + espacement;
    int btn_retour_x = btn_mechaball_x + btn_w + espacement;

    rectangle_t btn_carbu, btn_rappel, btn_mechaball, btn_retour;
    rectangle_t bordure_carbu, bordure_rappel, bordure_mechaball, bordure_retour;

    //creation des rectangles
    creer_rectangle(&btn_carbu,btn_w, btn_h, btn_carbu_x, btn_center_y,  210, 145, 132, 255, texte_carburant);
    creer_rectangle(&btn_rappel,btn_w, btn_h, btn_rappel_x, btn_center_y,  229, 185, 135, 255, texte_rappel);
    creer_rectangle(&btn_mechaball, btn_w, btn_h, btn_mechaball_x, btn_center_y, 162, 202, 154, 255, texte_mechaball);
    creer_rectangle(&btn_retour, btn_w, btn_h, btn_retour_x, btn_center_y, 255, 255, 255, 255, "Retour");

    //creation des bordures
    creer_rectangle(&bordure_carbu, btn_w+ 2 * border_size, btn_h+ 2 * border_size, btn_carbu_x - border_size, btn_center_y - border_size, 210, 145, 132, 230, NULL);
    creer_rectangle(&bordure_rappel, btn_w+ 2 * border_size, btn_h+ 2 * border_size, btn_rappel_x - border_size, btn_center_y - border_size, 94, 99, 102, 250, NULL);
    creer_rectangle(&bordure_mechaball, btn_w+ 2 * border_size, btn_h+ 2 * border_size, btn_mechaball_x - border_size, btn_center_y - border_size, 94, 99, 102, 250, NULL);
    creer_rectangle(&bordure_retour, btn_w+ 2 * border_size, btn_h+ 2 * border_size, btn_retour_x - border_size, btn_center_y - border_size, 94, 99, 102, 250, NULL);

    int choix = 0;

    while (running) {
        frameStart = SDL_GetTicks();
        
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
            }
            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_LEFT){
                    choix = (choix - 1 + 4) % 4;
                }
                if(event.key.keysym.sym == SDLK_RIGHT){
                    choix = (choix + 1) % 4;
                }
                if(event.key.keysym.sym == SDLK_a){
                    switch(choix) {
                        case 0: 
                            res = utilisation_carburant(joueur, ordi, actif);
                            if(res == OK) running = 0;
                             break;
                        case 1: 
                            res = utilisation_rappel(joueur, ordi, actif);
                            if(res == OK) running = 0;
                            break;
                        case 2:
                            if(!type_combat) {
                                res = utilisation_mechaball(joueur, ordi, actif);
                                if(res == CAPTURE) return CAPTURE;
                                else if(res == FAUX) running = 0;
                            }
                            else {
                                afficher_dialogue_combat(&(joueur->mechas_joueur[*actif]), ordi, "Systeme", "  Impossible voyons ! ",false);
                            }
                            break;
                        case 3: return (RETOUR);
                    }
                }
                SDL_Event e;
                while (SDL_PollEvent(&e)){
                    if (e.type == SDL_KEYUP) break;
                }
            }
            
        }
        if(running) {

            switch(choix){      //couleur de la bordure du bouton selectionné
                case 0 : bordure_carbu.couleur = couleur_bordure_carburant;
                            bordure_rappel.couleur = couleur_bordure;
                            bordure_mechaball.couleur = couleur_bordure;
                            bordure_retour.couleur = couleur_bordure;
                            break;
                case 1 : bordure_carbu.couleur = couleur_bordure;
                            bordure_rappel.couleur = couleur_bordure_rappel;
                            bordure_mechaball.couleur = couleur_bordure;
                            bordure_retour.couleur = couleur_bordure;
                            break;
                case 2 : bordure_carbu.couleur = couleur_bordure;
                            bordure_rappel.couleur = couleur_bordure;
                            bordure_mechaball.couleur = couleur_bordure_mechaball;
                            bordure_retour.couleur = couleur_bordure;
                            break;
                case 3 : bordure_carbu.couleur = couleur_bordure;
                            bordure_rappel.couleur = couleur_bordure;
                            bordure_mechaball.couleur = couleur_bordure;
                            bordure_retour.couleur = couleur_bordure_retour;
                            break;
            }

            SDL_RenderClear(game.renderer);
            draw_combat(&(joueur->mechas_joueur[*actif]), ordi);
            draw_all_rect(8, &bordure_carbu, &bordure_rappel, &bordure_mechaball, &bordure_retour, &btn_carbu, &btn_rappel, &btn_mechaball, &btn_retour);

            SDL_RenderPresent(game.renderer);


            frameTime = SDL_GetTicks() - frameStart;
            if (FRAME_DELAYS > frameTime) SDL_Delay(FRAME_DELAYS - frameTime);
        }

    }

    return OK;
}

/**
 * @brief Gère l'utilisation d'un objet "carburant" durant un combat.
 *
 * Cette fonction permet au joueur d'utiliser un objet carburant pour restaurer les points de vie (PV) d'un Mecha spécifique de son équipe durant un combat. 
 * Elle effectue plusieurs vérifications avant utilisation :
 * - Vérifie si le joueur possède au moins un carburant dans son inventaire.
 * - Vérifie que le Mecha choisi n'est pas déjà à ses PV max ou KO.
 * - Permet au joueur de choisir le Mecha cible de l'utilisation via `aff_mechas_combat`.
 *
 * En cas de succès, 50 PV sont restaurés au Mecha sélectionné, sans dépasser les PV max, et l'objet est retiré de l'inventaire.
 *
 * @param joueur Pointeur vers la structure du joueur actuellement en combat.
 * @param ordi   Pointeur vers le Mecha adverse (utilisé pour l'affichage du combat).
 * @param actif  Pointeur vers l'indice du Mecha actif du joueur actuellement utilisé en combat.
 *
 * @return int
 *         - `OK` : Le carburant a été utilisé avec succès.
 *         - `KO` : Échec de l'utilisation (pas de carburant disponible).
 *         - `RETOUR` : L'action a été annulée ou impossible (Mecha sélectionné KO, PV déjà au max ou aucun choix effectué).
 *
 * @pre Les structures joueur, inventaire et Mechas doivent être correctement initialisées.
 *
 * @post Le Mecha sélectionné a reçu des PV supplémentaires, et l'inventaire est mis à jour.
 *
 * @note Des dialogues informatifs sont affichés à l'écran pour expliquer chaque situation rencontrée par le joueur.
 *
 * @warning Vérifie toujours que le joueur dispose de l'objet avant son utilisation pour éviter des résultats incorrects.
 */
int utilisation_carburant(joueur_t *joueur, mechas_joueur_t *ordi, int *actif){
    if(joueur->inventaire->carburant <= 0){
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "  Auncun carburant.",false);
        return (KO);
    }
    else {
        int i = aff_mechas_combat(joueur); //recupere le numero du mechas selectionne
        if(i == -1 || i == 4){  //si on a rien choisi
            return (RETOUR);
        }
        else if(joueur->mechas_joueur[i].pv == joueur->mechas_joueur[i].pv_max){    //Si les PV sont au Max, on ne peut pas utiliser de carburant
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "  Les PV sont deja au maximun !",false);
            return (RETOUR);
        }
        else if(joueur->mechas_joueur[i].pv < 0){  //Si les PV sont a 0, impossible d'utiliser du carburant
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "  Vous ne pouvez pas utiliser de carburant.",false);
            return (RETOUR);
        }
        else{
            joueur->mechas_joueur[i].pv += 50;  //Ajouter 50 aux PV actuels
            if(joueur->mechas_joueur[i].pv > joueur->mechas_joueur[i].pv_max){
                joueur->mechas_joueur[i].pv = joueur->mechas_joueur[i].pv_max;
            }
            joueur->inventaire->carburant--;
            char message[200] = "  Utilisation carburant, nouveau PV : ";
            concat(message, joueur->mechas_joueur[i].pv);
            strcat(message, ".");
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", message,false);
        }
    }

    return (OK);

}

/**
 * @brief Gère l'utilisation d'un objet "rappel" durant un combat.
 *
 * Cette fonction permet au joueur d'utiliser un rappel afin de ranimer un Mecha mis K.O. (dont les PV sont à zéro) durant un combat.
 * Elle effectue plusieurs vérifications avant l'utilisation :
 * - Vérifie si le joueur possède au moins un rappel dans son inventaire.
 * - Vérifie que le Mecha sélectionné est bien mis K.O. (PV à zéro).
 * - Permet au joueur de choisir le Mecha cible via l'interface `aff_mechas_combat`.
 *
 * En cas de succès, le Mecha choisi revient au combat avec la moitié de ses PV max et l'objet est retiré de l'inventaire.
 *
 * @param joueur Pointeur vers la structure du joueur actuellement en combat.
 * @param ordi   Pointeur vers le Mecha adverse (utilisé pour l'affichage du combat).
 * @param actif  Pointeur vers l'indice du Mecha actif actuellement utilisé par le joueur en combat.
 *
 * @return int
 *         - `OK` : Le rappel a été utilisé avec succès.
 *         - `KO` : Échec de l'utilisation (aucun rappel disponible).
 *         - `RETOUR` : L'action a été annulée ou impossible (Mecha sélectionné encore en vie ou aucun choix effectué).
 *
 * @pre Les structures joueur, inventaire et Mechas doivent être correctement initialisées.
 *
 * @post Le Mecha sélectionné a été ranimé, ses PV ont été restaurés à la moitié de leur maximum, et l'inventaire du joueur est mis à jour.
 *
 * @note Des dialogues informatifs s'affichent pour expliquer clairement chaque résultat à l'utilisateur.
 *
 * @warning Vérifie toujours que le joueur possède l'objet avant son utilisation pour éviter des erreurs inattendues.
 */
int utilisation_rappel(joueur_t *joueur, mechas_joueur_t *ordi, int *actif){
    if(joueur->inventaire->rappel <= 0){
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "  Aucun rappel.",false);
        return (KO);
    }
    else {
        int i = aff_mechas_combat(joueur); //recupere le numero du mechas selectionne
        if(i == -1 || i == 4){ //si on a pas choisi de mecha
            return (RETOUR);
        }
        else if(joueur->mechas_joueur[i].pv != 0){  //Si les PV sont pas a 0, impossible d'utiliser du rappel
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "  Vous ne pouvez pas utiliser de rappel sur un mechas encore en vie.",false);
            return (RETOUR);
        }
        else{
            joueur->mechas_joueur[i].pv = joueur->mechas_joueur[i].pv_max/2;  //Remettre les PV au max
            char message[200] = "  ";
            strcat(message, mecha[joueur->mechas_joueur[i].id_mechas - 1].nom);
            strcat(message, "est de retour !");
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", message,false);
            joueur->inventaire->rappel--;
            
        }
    }

    return (OK);
}

/**
 * @brief Gère l'utilisation d'une "mechaball" pour capturer un Mecha adverse durant un combat.
 *
 * Cette fonction permet au joueur d'utiliser une mechaball afin d'essayer de capturer un Mecha sauvage en combat.
 * Elle réalise plusieurs étapes clés :
 * - Vérifie que le joueur dispose d'au moins une mechaball dans son inventaire.
 * - Calcule le taux de capture en fonction du type du Mecha adverse :
 *   - Type "Nucléaire" : taux de base faible (10% max).
 *   - Autres types : taux de base plus élevé (50% max).
 * - Le taux effectif de capture dépend des PV restants du Mecha adverse : plus les PV sont bas, plus le taux est élevé.
 * - Un tirage aléatoire détermine si la capture est réussie ou non.
 *
 * En cas de succès, le Mecha sauvage est ajouté à l'équipe du joueur.
 *
 * @param joueur Pointeur vers la structure du joueur en combat.
 * @param ordi   Pointeur vers le Mecha adverse que le joueur tente de capturer.
 * @param actif  Pointeur vers l'indice du Mecha actif du joueur utilisé actuellement en combat.
 *
 * @return int
 *         - `CAPTURE` : La capture a réussi, le Mecha adverse rejoint l'équipe.
 *         - `FAUX` : La capture a échoué, la mechaball est consommée.
 *         - `KO` : Échec d'utilisation (aucune mechaball disponible).
 *
 * @pre Les structures joueur, inventaire et Mechas doivent être initialisées et valides.
 *
 * @post La mechaball est retirée de l'inventaire, et le Mecha sauvage est ajouté à l'équipe en cas de succès.
 *
 * @note Des dialogues informatifs sont affichés pour expliquer le résultat de l'action au joueur.
 *
 * @warning Cette fonction ne devrait être utilisée que durant des combats contre des Mechas sauvages. Pour les combats spéciaux ou scénarisés, la capture pourrait être désactivée.
 */
int utilisation_mechaball(joueur_t * joueur, mechas_joueur_t *ordi, int *actif) {
    if(joueur->inventaire->mechaball <= 0){
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "  Aucune mechaball.",false);
        return (KO);
    }
    else{
        joueur->inventaire->mechaball--;
        float taux_capture;   //Un taux de capture different en fonction du type de mecha (nucleaire + difficile a attraper)
        if((strcmp(mecha[ordi->id_mechas-1].type, "Nucleaire") == 0)){
            taux_capture = 0.1;
        }
        else{
            taux_capture = 0.5;
        }
        int proba_capture = (1 - (float)ordi->pv / ordi->pv_max) * taux_capture * 100;

        int nbr_rand_capture = (rand() % 100) + 1;

        if(proba_capture >= nbr_rand_capture){  //Si le mecha est attrapé
            char message[200] = "  Vous avez attrape ";
            strcat(message,  mecha[ordi->id_mechas-1].nom);
            strcat(message, ".");
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", message,false);
            copie_mechas(joueur, ordi);
            return (CAPTURE);
        }
        else {
            char message[200] = "  Vous n'avez pas reussi a attrape ";
            strcat(message,  mecha[ordi->id_mechas-1].nom);
            strcat(message, ".");
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", message,false);
        }
    }
    return (FAUX);
}



/*
===========================================
*/

/*
===========================================
FONCTIONS ATTAQUE
===========================================
*/

/**
 * @brief Calcule et applique les effets d'une attaque lors d'un combat entre deux Mechas.
 *
 * Cette fonction gère le calcul des dégâts infligés par un Mecha attaquant à un Mecha défenseur en tenant compte :
 * - De l'attaque choisie (attaque_1 ou attaque_2).
 * - De la précision de l'attaque.
 * - Des affinités de types entre l'attaque et le Mecha défenseur.
 * - Des statistiques (attaque et défense) des deux Mechas impliqués.
 *
 * En cas de succès, les dégâts sont calculés, et une animation de perte de PV est jouée.
 * La fonction affiche également des dialogues informatifs sur les actions réalisées et leur résultat (échec ou succès de l'attaque).
 *
 * @param choix        Indice de l'attaque choisie par le joueur (0 pour attaque_1, 1 pour attaque_2, 2 pour passer le tour).
 * @param mecha_att    Pointeur vers la structure du Mecha attaquant.
 * @param mecha_def    Pointeur vers la structure du Mecha défenseur.
 * @param mecha_choix  Indicateur du Mecha attaquant (1 si joueur attaque, 0 si adversaire attaque).
 *
 * @return int
 *         - `OK` : Attaque exécutée avec succès.
 *         - `KO` : Impossible d'exécuter l'attaque (plus d'utilisations disponibles).
 *         - `0`  : Action spéciale "passer son tour", aucune action réalisée.
 *
 * @pre Les structures des Mechas et les tableaux globaux des attaques doivent être initialisés correctement.
 *
 * @post La fonction modifie les PV du Mecha défenseur et réduit le compteur d'utilisation de l'attaque effectuée.
 *
 * @note
 * - L'efficacité des attaques dépend des types :
 *   - "Carburant" efficace contre "Électrique"
 *   - "Électrique" efficace contre "Renouvelable"
 *   - "Renouvelable" efficace contre "Carburant"
 *   - "Uranium" inflige des dégâts légèrement supérieurs contre tous les types sauf lui-même.
 *
 * @warning La fonction ne vérifie pas si les Mechas sont déjà KO avant de calculer les dégâts ; cela doit être géré en amont.
 *
 * @see animation_degat() pour l'affichage animé de la perte de PV.
 */
int algo_attaque(int choix, mechas_joueur_t *mecha_att, mechas_joueur_t *mecha_def, int mecha_choix) {
    if(choix == 2) return 0;
    if(choix == 0 && mecha_att->utilisation_1 == 0) {
        afficher_dialogue_combat(  mecha_att, mecha_def, "Systeme", "  Vous ne pouvez plus utiliser cette attaque.",false);
        return KO;
    }
    if(choix == 1 && mecha_att->utilisation_2 == 0) {
        afficher_dialogue_combat(  mecha_att, mecha_def, "Systeme", "  Vous ne pouvez plus utiliser cette attaque.",false);
        return KO;
    }
   // int ancien_pv = mecha_def->pv;
    float att_degat[2] = {attaque[mecha_att->attaque_1].degats, attaque[mecha_att->attaque_2].degats};
    float stat_att_mecha = mecha_att->attaque;
    char *mecha_att_type[2] = {attaque[mecha_att->attaque_1 -1].type, attaque[mecha_att->attaque_2- 1].type};
    char *mecha_def_type = mecha[mecha_def->id_mechas-1].type;
    char *nom_attaque[2] = {attaque[mecha_att->attaque_1-1].nom, attaque[mecha_att->attaque_2-1].nom};
    int precision[2] = {attaque[mecha_att->attaque_1].precision, attaque[mecha_att->attaque_2].precision};
    char att_type[4][20] = {"Carburant", "Electrique", "Renouvelable", "Carburant"};
    int nbr_rand = (rand() % 100) + 1;
    char msg[300] = "  ";
    strcat(msg, mecha[mecha_att->id_mechas - 1].nom);
    strcat(msg, " utilise ");
    strcat(msg, nom_attaque[choix]);
    strcat(msg, ".");

    if(mecha_choix) 
        afficher_dialogue_combat(  mecha_att, mecha_def, "Systeme", msg,false);
    else 
        afficher_dialogue_combat(  mecha_def, mecha_att, "Systeme", msg,false);

    if(nbr_rand <= precision[choix]){  //Test si l'attaque touche (precision)
        
        if((strcmp(mecha_att_type[choix], "Uranium"))){
            for(int i = 0; i < 3; i++) {
                if((!strcmp(mecha_att_type[choix], att_type[i])) && !strcmp(mecha_def_type, att_type[i+1])){ //Test si l'attaque est efficace
                    att_degat[choix] *= 1.5;
                }
            }
        }
        else if(!strcmp(mecha_att_type[choix], "Uranium") && strcmp(mecha_def_type, "Uranium")){    //Test si l'attaque est du type uranium sur un autre type
            att_degat[choix] *= 1.2;
        }
        if(stat_att_mecha + att_degat[choix] > mecha_def->defense) {
            int pv_old = mecha_def->pv;
            int pv_new = pv_old - (stat_att_mecha + att_degat[choix] - mecha_def->defense);
            if(pv_new < 0) pv_new = 0;
        
            if(mecha_choix) 
                animation_degat(mecha_choix, pv_old, pv_new,  mecha_att, mecha_def);
            else 
                animation_degat(mecha_choix, pv_old, pv_new,  mecha_def, mecha_att);
        }
        if(!choix) mecha_att->utilisation_1--;
        else mecha_att->utilisation_2--;
        
    }
    else {
        if(mecha_choix) 
            afficher_dialogue_combat(mecha_att, mecha_def, "Systeme", "  L'attaque echoue.",false);
        else
            afficher_dialogue_combat(mecha_def,mecha_att, "Systeme", "  L'attaque echoue.",false);
    }
    return OK;
}

/**
 * @brief Affiche l'interface permettant au joueur de choisir et effectuer une attaque durant un combat.
 *
 * Cette fonction permet au joueur de sélectionner l'une des deux attaques disponibles pour son Mecha actif ou de revenir en arrière.
 * Elle affiche les informations sur chaque attaque (nom, dégâts, type, utilisations restantes) et gère les interactions utilisateur via le clavier.
 *
 * L'utilisateur navigue entre les choix avec les touches gauche/droite et valide avec la touche 'a'. Une fois validée, l'attaque est traitée via la fonction `algo_attaque()`.
 *
 * @param j            Un pointeur vers la structure `joueur_t` représentant l'état actuel du joueur.
 * @param mecha_ordi   Pointeur vers la structure du Mecha adverse (cible de l'attaque).
 * @param actif        Pointeur vers l'indice du Mecha actuellement actif du joueur dans son équipe.
 *
 * @return int
 *         - `OK` : L'attaque a été effectuée avec succès.
 *         - `KO` : Impossible d'exécuter l'attaque (plus d'utilisations disponibles).
 *         - `RETOUR` : Le joueur a choisi l'option "Retour" pour annuler l'action d'attaque.
 *
 * @pre Les structures globales `game`, `attaque` et `mecha` doivent être initialisées correctement.
 *
 * @post 
 * - La fonction met à jour les utilisations restantes de l'attaque choisie.
 * - Applique les dégâts au Mecha adverse via `algo_attaque()`.
 *
 * @note 
 * - L'interface utilise un système de couleur pour distinguer clairement les attaques selon leur type.
 * - Les boutons affichés sont dynamiquement adaptés à la taille de l'écran.
 *
 * @warning Cette fonction suppose que les structures globales (`game`, `attaque`, `mecha`) sont déjà initialisées, sinon le comportement est indéfini.
 *
 * @see algo_attaque() pour le traitement des attaques et le calcul des dégâts.
 * @see draw_combat() pour l'affichage graphique de l'état actuel du combat.
 */
int attaque_joueur(joueur_t *j, mechas_joueur_t *mecha_ordi, int * actif){
   
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    int running = 1;
    int win_w, win_h;
    int attaque_type[2];
    SDL_GetRendererOutputSize(game.renderer, &win_w, &win_h);       //dimensions ecran
    char tmp[20];
    //texte à afficher
    char texte_retour[7] = "Retour";
    char texte_attaque[2][400] = {"Attaque 1 : ", "Attaque 2 : "};
    int attaque_num[2] = {j->mechas_joueur[*actif].attaque_1-1, j->mechas_joueur[*actif].attaque_2-1};
    int utilisation_num[2] = {j->mechas_joueur[*actif].utilisation_1, j->mechas_joueur[*actif].utilisation_2};
    for(int i = 0; i < 2; i++){
        strcpy(texte_attaque[i], attaque[attaque_num[i]].nom);
        strcat(texte_attaque[i], "\nDegats:");
        concat(texte_attaque[i], attaque[attaque_num[i]].degats);
        strcat(texte_attaque[i], "\nUtilisation:");
        concat(texte_attaque[i], utilisation_num[i]);
        strcat(texte_attaque[i], "/");
        concat(texte_attaque[i], attaque[attaque_num[i]].utilisations);
        strcpy(tmp, attaque[attaque_num[i]].type);
        if(!strcmp(tmp, "Electrique")){
            attaque_type[i] = 0;
        }
        else if(!strcmp(tmp, "Renouvelable")){
            attaque_type[i] = 1;
        }
        else if(!strcmp(tmp, "Carburant")){
            attaque_type[i] = 2;
        }
        else if(!strcmp(tmp, "Uranium")){
            attaque_type[i] = 3;
        }
    }

    
    //Couleur des bordure
    SDL_Color couleur_bordure[4] =  {{233, 218, 1, 255}, {40, 83, 254, 255}, {210, 121, 5, 255}, {57, 189, 1, 255}};
    SDL_Color couleur_bordure_retour = {109, 110, 110, 255};
    
    //Couleur bordure selectionné
    SDL_Color selec_bordure[4] = {
        {100, 90, 0, 255},   // Jaune très foncé
        {10, 20, 100, 255},  // Bleu très foncé
        {80, 40, 0, 255},    // Orange très foncé
        {10, 60, 0, 255}     // Vert très foncé
    };
    SDL_Color selec_bordure_retour = {80, 80, 80, 255};


    rectangle_t btn_attaque1, btn_attaque2, btn_retour;
    rectangle_t border_attaque1, border_attaque2, border_retour;

    int border_size = 5 * game.scale; // Épaisseur des bords

        // Définition des tailles des boutons
    int btn_w = rect_bas.rect.w / 5; // Chaque bouton fait environ 1/5 de la largeur de rect_bas
    int btn_h = rect_bas.rect.h / 2; // Hauteur du bouton = moitié de rect_bas
    int btn_spacing = rect_bas.rect.w / 20; // Espacement entre les boutons (ajustable)

    // Positionnement des boutons
    int btn_center_x = (rect_bas.rect.w - btn_w) / 2;  // Centré horizontalement dans rect_bas
    int btn_center_y = rect_bas.rect.y + (rect_bas.rect.h - btn_h) / 2; // Centré verticalement

    int btn_left_x = btn_center_x - btn_w - btn_spacing;  // À gauche du centre
    int btn_right_x = btn_center_x + btn_w + btn_spacing; // À droite du centre

    //creer les boutons
    creer_rectangle(&btn_attaque1, btn_w, btn_h, btn_left_x, btn_center_y, 142, 142, 142, 255, texte_attaque[0]);
    creer_rectangle(&btn_attaque2, btn_w, btn_h, btn_center_x, btn_center_y, 142, 142, 142, 255, texte_attaque[1]);
    creer_rectangle(&btn_retour, btn_w, btn_h, btn_right_x, btn_center_y, 142, 142, 142, 255, texte_retour);

    //creer les bords
    creer_rectangle(&border_attaque1, btn_w+ 2 * border_size, btn_h+ 2 * border_size, btn_left_x - border_size, btn_center_y - border_size, selec_bordure[attaque_type[0]].r, selec_bordure[attaque_type[0]].g, selec_bordure[attaque_type[0]].b, 255, NULL);
    creer_rectangle(&border_attaque2, btn_w+ 2 * border_size, btn_h+ 2 * border_size, btn_center_x - border_size, btn_center_y - border_size, couleur_bordure[attaque_type[1]].r, couleur_bordure[attaque_type[1]].g, couleur_bordure[attaque_type[1]].b, 255, NULL);
    creer_rectangle(&border_retour, btn_w+ 2 * border_size, btn_h+ 2 * border_size, btn_right_x - border_size, btn_center_y - border_size, couleur_bordure_retour.r, couleur_bordure_retour.g, couleur_bordure_retour.b, 255, NULL);

    int choix = 0;

    

    while(running) {
        frameStart = SDL_GetTicks();
        
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_LEFT){
                    choix = (choix - 1 + 3) % 3;
                }
                if(event.key.keysym.sym == SDLK_RIGHT){
                    choix = (choix + 1) % 3;
                }
                if(event.key.keysym.sym == SDLK_a){
                    if(choix == 2)
                        return RETOUR;
                    int res = algo_attaque(choix, &j->mechas_joueur[*actif], mecha_ordi,  1);
                    if(res != KO) {
                        return res;
                    }
                    
                }
                SDL_Event e;
                while (SDL_WaitEvent(&e) && e.type != SDL_KEYUP);
            }
        }
        switch(choix) {
            case 0 : border_attaque1.couleur = selec_bordure[attaque_type[0]]; 
            border_attaque2.couleur = couleur_bordure[attaque_type[1]];
            border_retour.couleur = couleur_bordure_retour;
            break;
            case 1 : border_attaque1.couleur = couleur_bordure[attaque_type[0]];
            border_attaque2.couleur = selec_bordure[attaque_type[1]];
            border_retour.couleur = couleur_bordure_retour;
            break;
            case 2 : border_attaque1.couleur = couleur_bordure[attaque_type[0]];
            border_attaque2.couleur = couleur_bordure[attaque_type[1]];
            border_retour.couleur = selec_bordure_retour;
            break;
        }
        SDL_RenderClear(game.renderer);
        draw_combat(&(j->mechas_joueur[*actif]), mecha_ordi);
        draw_all_rect(6, &border_attaque1, &border_attaque2, &border_retour, &btn_attaque1, &btn_attaque2, &btn_retour);
        
        SDL_RenderPresent(game.renderer);


        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAYS > frameTime) {
            SDL_Delay(FRAME_DELAYS - frameTime);
        }
    }
    return OK;
}

/**
 * @brief Gère l'attaque aléatoire d'un Mecha sauvage durant un combat contre le joueur.
 *
 * Cette fonction sélectionne aléatoirement une attaque parmi les deux disponibles pour le Mecha sauvage. 
 * Si l'attaque sélectionnée ne possède plus d'utilisations, elle tente automatiquement l'autre attaque.
 * Si aucune attaque n'est disponible, un message indiquant que le Mecha sauvage ne peut plus attaquer est affiché.
 * 
 * L'attaque choisie est exécutée via la fonction `algo_attaque()`.
 *
 * @param mecha_ordi     Pointeur vers la structure du Mecha sauvage effectuant l'attaque.
 * @param mecha_joueur   Pointeur vers la structure du Mecha du joueur ciblé par l'attaque.
 *
 * @return int
 *         - `OK` : L'attaque a été exécutée avec succès.
 *         - `0` : Aucune attaque disponible pour le Mecha sauvage (toutes les utilisations sont épuisées).
 *
 * @pre Les structures globales `attaque` et `mecha` doivent être initialisées correctement.
 *
 * @post 
 * - La fonction décrémente les utilisations restantes de l'attaque choisie du Mecha sauvage.
 * - Applique les dégâts au Mecha du joueur via `algo_attaque()`.
 *
 * @note 
 * - Un message informatif est affiché à l'utilisateur si aucune attaque ne peut être effectuée.
 * - L'attaque est sélectionnée aléatoirement afin de varier les stratégies du Mecha sauvage.
 *
 * @warning Cette fonction suppose que les Mechas et attaques correspondantes sont correctement initialisées. Un comportement imprévisible pourrait se produire en cas d'erreur d'initialisation.
 *
 * @see algo_attaque() pour le traitement concret des attaques et le calcul des dégâts infligés.
 * @see afficher_dialogue_combat() pour afficher des messages à l'utilisateur.
 */
int attaque_ordi_sauvage(mechas_joueur_t *mecha_ordi, mechas_joueur_t *mecha_joueur){  //Retourne 0 si ne peut pas attaquer
    int numero_attaque;
    int utilisation[2] = {mecha_ordi->utilisation_1, mecha_ordi->utilisation_2};
    numero_attaque = rand() % 2;
    if(!utilisation[numero_attaque]){
        numero_attaque = 1 - numero_attaque;
        if(!utilisation[numero_attaque]) {
            afficher_dialogue_combat(  mecha_joueur, mecha_ordi, "Systeme", "  Le mecha sauvage ne peut plus attaquer.",false);            
            return 0;
        }
        algo_attaque(numero_attaque, mecha_ordi, mecha_joueur, 0);
       
    }
    else{
        algo_attaque(numero_attaque, mecha_ordi, mecha_joueur, 0);
    }
    return OK;
}

/**
 * @brief Gère les actions automatiques d'un PNJ (personnage non-joueur) lors d'un combat contre le joueur.
 *
 * Cette fonction décide automatiquement des actions du PNJ selon l'état actuel du Mecha actif :
 * - Si les PV du Mecha actif sont inférieurs à 25% des PV maximum, le PNJ a 1 chance sur 4 de changer de Mecha.
 * - Si les PV du Mecha actif sont inférieurs ou égaux à 50% des PV maximum, le PNJ a une chance sur deux d'utiliser un carburant pour récupérer 20 PV.
 * - Sinon, le PNJ effectue une attaque aléatoire parmi celles disponibles. Si aucune attaque n'est disponible, un message indique que le Mecha ne peut plus attaquer.
 *
 * @param pnj           Pointeur vers la structure du PNJ effectuant l'action.
 * @param mecha_joueur  Pointeur vers le Mecha du joueur cible de l'action du PNJ.
 * @param actif         Pointeur vers l'indice du Mecha actuellement actif chez le PNJ. Peut être modifié si un changement de Mecha est effectué.
 *
 * @return int
 *         - `OK` : Une action a été effectuée avec succès (attaque, utilisation de carburant, ou changement de Mecha).
 *         - `KO` : Aucune attaque disponible pour le Mecha actif du PNJ (toutes les utilisations sont épuisées).
 *
 * @pre Les structures globales (`attaque`, `mecha`) et l'inventaire du PNJ doivent être initialisés.
 *
 * @post 
 * - Les PV du Mecha actif du PNJ peuvent augmenter si un carburant est utilisé.
 * - Les utilisations restantes des attaques du Mecha actif peuvent être décrémentées.
 * - L'indice du Mecha actif peut changer si le PNJ décide de changer de Mecha.
 *
 * @note 
 * - Des dialogues informatifs sont affichés à l'utilisateur en fonction des actions du PNJ.
 * - Le comportement est semi-aléatoire, dépendant des PV actuels du Mecha actif pour simuler une stratégie plus réaliste.
 *
 * @warning Cette fonction suppose que les Mechas et leurs attaques correspondantes sont correctement initialisés, et que le PNJ dispose d'un inventaire valide. Un comportement indéterminé pourrait se produire sinon.
 *
 * @see algo_attaque() pour le traitement des attaques et des dégâts.
 * @see afficher_dialogue_combat() pour afficher les messages durant le combat.
 */
int attaque_ordi_pnj(pnj_t * pnj, mechas_joueur_t *mecha_joueur, int * actif){
    int nbr_rand_choix1 = 1, nbr_rand_choix = 0;
    int cmp = 0;
    for(int i = 0; i < pnj->nb_mechas; i++) {
        if(pnj->mechas_joueur[i].pv > 0) {
            cmp++;
        }
    }

    if(pnj->mechas_joueur[*actif].pv < pnj->mechas_joueur[*actif].pv_max/4 && cmp > 1) { //si les pv sont < a un quart des pv max on a 1/4 chances de changer de mechas
        nbr_rand_choix1 = (rand() % 4);
    }
    if(nbr_rand_choix1 > 0) {
        if(pnj->mechas_joueur[*actif].pv <= pnj->mechas_joueur[*actif].pv_max / 2 && pnj->inventaire->mechaball > 0){   //Si les PV sont inferieurs a la moitie des PV Max 1 chance sur 2 d'utiliser un carburant
            nbr_rand_choix = (rand() % 2);
        }

        if(nbr_rand_choix){    //Utilisation d'un carburant
            if(pnj->inventaire->carburant <= 0){
                nbr_rand_choix = 0;
            }
            else{
                afficher_dialogue_combat(mecha_joueur ,  &(pnj->mechas_joueur[*actif]), "Systeme", "  Le mechas utilise un carburant.",false);
                pnj->mechas_joueur[*actif].pv += 20;
                if(pnj->mechas_joueur[*actif].pv > pnj->mechas_joueur[*actif].pv_max){
                    pnj->mechas_joueur[*actif].pv = pnj->mechas_joueur[*actif].pv_max;
                }
                pnj->inventaire->carburant--;
            }
        }
        if(!nbr_rand_choix){   //Attaque
            int numero_attaque;
            int utilisation[2] = {pnj->mechas_joueur[*actif].utilisation_1, pnj->mechas_joueur[*actif].utilisation_2};
            numero_attaque = rand() % 2;
            if(!utilisation[numero_attaque]){
                numero_attaque = 1 - numero_attaque;
                if(!utilisation[numero_attaque]) {
                afficher_dialogue_combat(mecha_joueur ,  &(pnj->mechas_joueur[*actif]), "Systeme", "  Le mechas n a plus d utilisations.",false); 
                return KO;
                }
                algo_attaque(numero_attaque, &(pnj->mechas_joueur[*actif]), mecha_joueur, 0);
        
            }
            else{
                algo_attaque(numero_attaque, &(pnj->mechas_joueur[*actif]), mecha_joueur, 0);
            }
        }
    }
    else {
        for(int i = 0; i < 4; i++) {
            if(pnj->mechas_joueur[i].pv > 0 && i != (*actif)) {
                (*actif) = i;
                break;
            }
        }
        afficher_dialogue_combat(mecha_joueur ,  &(pnj->mechas_joueur[*actif]), "Systeme", "  L'adversaire change de mecha.",false); 
    }

    return OK;
}

/*
===========================================
*/

/**
 * @brief Permet au joueur de changer le Mecha actif pendant un combat.
 *
 * Cette fonction affiche un menu pour permettre au joueur de sélectionner un nouveau Mecha actif. 
 * Elle vérifie plusieurs conditions avant d'autoriser le changement :
 * - Le Mecha sélectionné ne doit pas être KO (PV != 0).
 * - Le Mecha sélectionné ne doit pas être déjà actif.
 * - Le joueur peut revenir en arrière en choisissant « retour ».
 *
 * Des messages de dialogues informatifs sont affichés selon le résultat de l'opération.
 *
 * @param joueur  Pointeur vers la structure du joueur.
 * @param actif   Pointeur vers l'indice du Mecha actuellement actif du joueur (sera mis à jour si le changement est accepté).
 * @param ordi    Pointeur vers le Mecha adverse (utilisé uniquement pour afficher des dialogues informatifs en cas d'erreur).
 *
 * @return int
 *         - `OK` : Changement de Mecha effectué avec succès.
 *         - `KO` : Le changement de Mecha a échoué (Mecha mort ou déjà actif).
 *         - `RETOUR` : Le joueur a annulé l'action.
 *
 * @pre Le joueur doit avoir au moins un Mecha valide dans son équipe (autre que celui actif).
 *
 * @post L'indice du Mecha actif (`*actif`) est modifié si un changement est validé par le joueur.
 *
 * @note Cette fonction fait appel à l'interface graphique via `aff_mechas_combat()` pour permettre le choix du joueur.
 *
 * @warning Aucune vérification n'est effectuée pour le cas extrême où tous les Mechas seraient KO. Une telle vérification doit être faite en amont si nécessaire.
 *
 * @see aff_mechas_combat() pour l'interface de sélection graphique.
 * @see afficher_dialogue_combat() pour les messages informatifs.
 */
int changer_mecha(joueur_t *joueur, int *actif, mechas_joueur_t *ordi){
    int choix ;
    choix = aff_mechas_combat(joueur);
    if (joueur->mechas_joueur[choix].pv == 0 && choix != 4) {
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "  Ce mecha est mort.",false); 
        return KO;
    }
    if(choix == *actif) {
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "  Ce mecha est deja actif.",false);
        return KO;
    }
    if(choix == 4) return RETOUR;
    afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "  Vous changez de mecha.",false);
    *actif = choix;
    return OK;
}

/**
 * @brief Gère l'apprentissage d'une nouvelle attaque par un Mecha lors d'un changement de niveau.
 *
 * Cette fonction vérifie si le Mecha joueur acquiert une nouvelle attaque lorsqu'il atteint un niveau précis.
 * Si une nouvelle attaque est disponible au niveau actuel du Mecha, la fonction propose au joueur de remplacer une de ses deux attaques actuelles, 
 * ou bien de ne pas changer ses attaques. 
 *
 * Des messages informatifs sur l'attaque actuelle et les attaques disponibles sont affichés au joueur pour guider son choix.
 *
 * @param mecha_joueur Pointeur vers le Mecha du joueur concerné par l'apprentissage.
 * @param mecha_ordi   Pointeur vers le Mecha adverse, utilisé uniquement pour l'affichage de dialogues.
 *
 * @return int
 *         - `OK` : L'apprentissage a été géré correctement (même si le joueur décide de ne pas changer d'attaque ou qu'aucune attaque n'est disponible).
 *
 * @pre Le Mecha joueur doit avoir un niveau valide et cohérent avec les attaques définies dans le tableau `attaque` associé au type du Mecha.
 *
 * @post L'attaque du Mecha joueur peut être modifiée en fonction du choix du joueur. Si une attaque est remplacée, son nombre d'utilisations est réinitialisé à la valeur par défaut de la nouvelle attaque.
 *
 * @note Cette fonction utilise `afficher_dialogue_combat` pour interagir avec le joueur. 
 *       La fonction est sans effet si aucune attaque ne correspond au niveau actuel du Mecha.
 *
 * @warning Cette fonction suppose que les attaques associées au Mecha sont correctement définies et triées par niveau d'apprentissage.
 *
 * @see afficher_dialogue_combat() pour l'affichage interactif des dialogues avec le joueur.
 */
int apprentissage_attaque(mechas_joueur_t *mecha_joueur, mechas_joueur_t *mecha_ordi){
    int i;
    int choix = -1;
    char text[300];

    for(i = 0;attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1 ].niveau != mecha_joueur->niveau && i<5 ;i++);
    if(attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1 ].niveau == mecha_joueur->niveau){
        sprintf(text,"  Votre mecha souhaite apprendre une nouvelle attaque: %s\n Degats:%d\n Precision:%d.",
        attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].nom,attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].degats,
        attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i]-1].precision);
        afficher_dialogue_combat(mecha_joueur,mecha_ordi,"Systeme",text,false);
        sprintf(text,"  Choisissez l'attaque a remplacer: \n 1 - Attaque 1: %s - Degats: %d - Precision: %d\n 2 - Attaque 2: %s - Degats: %d - Precision: %d \n 3: Garder ses attaques.",
        attaque[mecha_joueur->attaque_1 -1].nom,attaque[mecha_joueur->attaque_1 -1].degats,attaque[mecha_joueur->attaque_1 -1].precision,
        attaque[mecha_joueur->attaque_2 -1].nom,attaque[mecha_joueur->attaque_2 -1].degats,attaque[mecha_joueur->attaque_2 -1].precision);
        choix = afficher_dialogue_combat(mecha_joueur,mecha_ordi,"Systeme",text,true);
        switch(choix){
            case 1:
                sprintf(text,"  Vous avez remplace votre attaque %s par %s.",
                attaque[mecha_joueur->attaque_1-1].nom,attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i]-1].nom);
                mecha_joueur->attaque_1 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].id_attaques;
                mecha_joueur->utilisation_1 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].utilisations;
                 afficher_dialogue_combat(mecha_joueur,mecha_ordi,"Systeme",text,false);
                break;
            case 2:
                sprintf(text,"  Vous avez remplace votre attaque %s par %s.",
                attaque[mecha_joueur->attaque_1 -1].nom,attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].nom);
                mecha_joueur->attaque_2 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].id_attaques;
                mecha_joueur->utilisation_2 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].utilisations;
                afficher_dialogue_combat(mecha_joueur,mecha_ordi,"Systeme",text,false);
                break;
            default:
                afficher_dialogue_combat(mecha_joueur,mecha_ordi,"Systeme","  Vous gardez votre attaque actuelle.",false);
        }
           
    }
    return OK;
}

/**
 * @brief Gère l'évolution d'un Mecha joueur lorsqu'il atteint le niveau requis.
 *
 * Cette fonction vérifie si le Mecha joueur répond aux conditions nécessaires pour évoluer.
 * Si le niveau actuel du Mecha est supérieur ou égal au niveau requis pour l'évolution 
 * et qu'une évolution est définie, alors l'ID du Mecha est mis à jour pour refléter cette évolution.
 * Un message informant le joueur de l'évolution effectuée est affiché à l'écran.
 *
 * @param mecha_joueur Pointeur vers le Mecha joueur susceptible d'évoluer.
 * @param mecha_ordi   Pointeur vers le Mecha adverse, utilisé uniquement pour l'affichage de dialogues contextuels.
 *
 * @return int
 *         - `OK` : L'évolution a été vérifiée et traitée correctement.
 *
 * @pre Les attributs `niveau_evolution` et `evolution` du Mecha doivent être correctement initialisés.
 *
 * @post Si une évolution a eu lieu, `mecha_joueur->id_mechas` est incrémenté pour pointer vers le Mecha évolué.
 *
 * @note La fonction utilise `afficher_dialogue_combat` pour notifier le joueur de l'évolution.
 *
 * @warning La fonction suppose que le Mecha suivant immédiatement dans le tableau `mecha` correspond à l'évolution logique du Mecha actuel.
 *          Assurez-vous que le tableau `mecha` respecte strictement cet ordre d'évolution.
 *
 * @see afficher_dialogue_combat() pour l'affichage interactif des dialogues avec le joueur.
 */
int evolution_mechas(mechas_joueur_t *mecha_joueur, mechas_joueur_t *mecha_ordi){
    char text[200];
    if(mecha_joueur->niveau >= mecha[mecha_joueur->id_mechas -1].niveau_evolution && mecha[mecha_joueur->id_mechas -1].evolution > 0){
        mecha_joueur->id_mechas++;
        sprintf(text,"  %s a evolue en %s.",mecha[mecha_joueur->id_mechas -2].nom,mecha[mecha_joueur->id_mechas -1].nom );

        afficher_dialogue_combat(mecha_joueur, mecha_ordi, "Systeme", text,false);

    }
    return OK;
}

/**
 * @brief Gère la montée en niveau d'un Mecha joueur après avoir reçu des points d'expérience (XP).
 *
 * Cette fonction ajoute un nombre spécifié d'XP au Mecha joueur. Si le seuil d'XP pour le niveau suivant est atteint ou dépassé,
 * le Mecha monte en niveau, réinitialise son compteur d'XP en excès pour le prochain niveau et met à jour ses statistiques (PV, attaque, défense).
 * Elle vérifie également les possibilités d'évolution et d'apprentissage d'une nouvelle attaque à chaque montée en niveau.
 *
 * @param mecha       Pointeur vers le Mecha joueur recevant l'expérience.
 * @param mecha_ordi  Pointeur vers le Mecha adverse (pour l'affichage des dialogues contextuels).
 * @param xp_partage  Nombre de points d'expérience à ajouter au Mecha.
 * @param lvlup       Seuil d'expérience nécessaire pour atteindre le niveau supérieur actuel.
 *
 * @pre
 *  - `mecha` doit pointer vers un Mecha joueur correctement initialisé.
 *  - `lvlup` doit correspondre au nombre d'XP nécessaires à l'atteinte du prochain niveau avant appel de la fonction.
 *
 * @post
 *  - Les attributs du Mecha (niveau, XP, PV, attaque, défense) sont mis à jour en conséquence.
 *  - Si le Mecha atteint un niveau où il peut évoluer ou apprendre une attaque, ces changements sont appliqués immédiatement.
 *
 * @note
 *  - La fonction utilise un calcul exponentiel pour déterminer l'XP nécessaire pour chaque nouveau niveau :
 *    \f[ \text{lvlup} = \left\lfloor 15 \times e^{0.05 \times \text{niveau}} \right\rfloor \f]
 *
 * @warning
 *  - Cette fonction suppose que les fonctions `evolution_mechas` et `apprentissage_attaque` ne provoquent pas d'erreurs d'accès mémoire ou d'index hors limites.
 *  - Assurez-vous que les modifications de statistiques ne dépassent pas les limites prévues par le jeu.
 */
void montee_niveau(mechas_joueur_t *mecha, mechas_joueur_t *mecha_ordi, int xp_partage, int lvlup){
    int nouv_level;
    int ajout;
    nouv_level = mecha->xp + xp_partage;
    if(nouv_level >= lvlup){
        while(nouv_level >= lvlup){
            mecha->niveau++;
            nouv_level -= lvlup;
            mecha->xp = 0;
            mecha->xp += nouv_level;
            lvlup = (int)(15 * exp(0.05 * mecha->niveau));   //Calculer le nouveau nombre d'XP necessaire
            evolution_mechas(mecha,mecha_ordi);
            apprentissage_attaque(mecha,mecha_ordi);
            ajout = 1 + rand()%3;
            mecha->pv += ajout;
            mecha->pv_max += ajout;
            mecha->attaque += 1 + rand()%2;
            mecha->defense += 1 + rand()%2;
        }
    }
    else{
        mecha->xp += xp_partage;
    }
}

/**
 * @brief Distribue les points d'expérience (XP) gagnés lors d'un combat à un ensemble de Méchas ayant participé.
 *
 * Cette fonction calcule la répartition des points d'expérience gagnés à la fin d'un combat selon le nombre de Méchas du joueur ayant participé.
 * La répartition suit une logique prédéfinie : 
 *  - 1 Mécha reçoit 100% de l'XP,
 *  - 2 Méchas reçoivent respectivement 2/3 et 1/3 de l'XP,
 *  - 3 Méchas reçoivent respectivement 3/5, 1/5, 1/5 de l'XP,
 *  - 4 Méchas reçoivent respectivement 4/7, 1/7, 1/7, 1/7 de l'XP.
 *
 * Chaque Mécha reçoit son montant d'XP calculé et gère ensuite individuellement sa montée de niveau éventuelle.
 *
 * @param mechas_presents  Pointeur vers la structure du joueur contenant les Méchas ayant participé au combat.
 * @param mecha_ordi       Pointeur vers le Mécha adverse vaincu (pour contexte d'évolution ou apprentissage d'attaques).
 * @param xp_gagne         Quantité totale d'XP gagnée à répartir entre les Méchas participants.
 *
 * @pre
 *  - `mechas_presents` doit pointer vers une structure joueur initialisée avec un nombre correct de Méchas (`nb_mechas` renseigné).
 *  - `xp_gagne` doit être une valeur positive représentant l'expérience totale à distribuer.
 *
 * @post
 *  - Chaque Mécha ayant participé voit ses XP augmenter selon la répartition définie.
 *  - Si un Mécha dépasse le seuil d'XP pour monter de niveau, la fonction `montee_niveau` est appelée et met à jour ses statistiques en conséquence.
 *
 * @warning
 *  - Si plus de 4 Méchas sont présents, seuls les 4 premiers Méchas reçoivent de l'XP.
 *  - Assurez-vous que l'ordre des Méchas présents est correct, car il influence la répartition des XP.
 *
 * @note
 *  - Le calcul du seuil d'expérience (`lvlup`) requis pour chaque Mécha utilise la formule exponentielle suivante :
 *    \f[ \text{lvlup} = \left\lfloor 15 \times e^{0.05 \times \text{niveau}} \right\rfloor \f]
 *
 */
void distribuer_xp(joueur_t *mechas_presents, mechas_joueur_t *mecha_ordi, int xp_gagne) {

    int nb;
    static const float coef_repartition[4][4] = {
        {1.0},              // 1 mécha -> 100% XP
        {2.0/3, 1.0/3},     // 2 méchas -> 2/3, 1/3
        {3.0/5, 1.0/5, 1.0/5},  // 3 méchas -> 3/5, 1/5, 1/5
        {4.0/7, 1.0/7, 1.0/7, 1.0/7}  // 4 méchas -> 4/7, 1/7, 1/7, 1/7
    };

    if(mechas_presents->nb_mechas > 4){
        nb = 4;
    }
    else{
        nb = mechas_presents->nb_mechas;
    }
    for (int i = 0; i < nb; i++) {
        int lvlup = (int)(15 * exp(0.05 * mechas_presents->mechas_joueur[i].niveau));
        int xp_partage = (int)(xp_gagne * coef_repartition[nb-1][i]); // Conversion propre en int
        montee_niveau(&mechas_presents->mechas_joueur[i],mecha_ordi, xp_partage, lvlup);
    }
}

/**
 * @brief Calcule et distribue les points d'expérience (XP) aux Méchas du joueur après avoir vaincu un Mécha adverse.
 *
 * Cette fonction est appelée après la défaite d'un Mécha adverse. Elle effectue les opérations suivantes :
 * - Calcule la quantité totale d'expérience gagnée en fonction du niveau du Mécha adverse, selon la formule :
 *   \f[
 *      \text{xp\_gagne} = \left\lfloor 20 \times e^{0.02 \times \text{niveau adversaire}} \right\rfloor
 *   \f]
 * - Répartit cette expérience aux Méchas du joueur qui ont participé au combat (maximum 4 Méchas).
 * - Affiche dans la console les niveaux mis à jour des Méchas participants après la distribution d'expérience.
 *
 * @param mechas_presents  Pointeur vers la structure du joueur, contenant les Méchas participants au combat.
 * @param mecha_ordi       Pointeur vers le Mécha adverse vaincu, utilisé pour déterminer la quantité d'expérience à distribuer.
 *
 * @pre
 * - Les structures pointées par `mechas_presents` et `mecha_ordi` doivent être initialisées et valides.
 *
 * @post
 * - Les Méchas du joueur ayant participé voient leur expérience et potentiellement leurs niveaux augmentés en fonction de l'expérience gagnée.
 * - Les informations mises à jour des Méchas sont affichées à l'écran via `printf`.
 *
 * @warning
 * - Cette fonction limite explicitement la distribution d'expérience aux quatre premiers Méchas du joueur (`nb_mechas`).
 *   Les Méchas au-delà du quatrième ne reçoivent pas d'expérience.
 *
 * @note
 * - La distribution réelle d'expérience aux Méchas est gérée par la fonction `distribuer_xp`.
 * - L'affichage des niveaux après distribution sert à fournir un retour visuel rapide sur les résultats de l'attribution d'expérience.
 *
 * @param[in] joueur_t Structure contenant les informations des Méchas du joueur.
 * @param[in] mechas_joueur_t Structure du Mécha adverse vaincu, pour calcul d'expérience.
 *
 * @see distribuer_xp() Pour la répartition détaillée de l'expérience aux Méchas.
 */
void level_mechas(joueur_t *mechas_presents, mechas_joueur_t *mecha_ordi){
    
    int i, nb;
    int xp_gagne;

    
    xp_gagne = (int)(20 * exp(0.02 * mecha_ordi->niveau));   //Calculer l'XP recu en tuant l'adversaire
    distribuer_xp(mechas_presents,mecha_ordi, xp_gagne);
    if(mechas_presents->nb_mechas > 4){
        nb = 4;
    }
    else{
        nb = mechas_presents->nb_mechas;
    }

    for(i = 0; i < nb; i++){
       
        printf("LVL : %d\n", mechas_presents->mechas_joueur[i].niveau);
    }
    
}

/**
 * @brief Gère le tour du joueur lors d'un combat contre un Mécha sauvage ou un PNJ.
 *
 * Cette fonction permet au joueur de réaliser une action pendant son tour. Le joueur peut :
 * - Utiliser un objet (carburant, rappel, mechaball).
 * - Attaquer avec son Mécha actif.
 * - Changer de Mécha actif.
 *
 * L'interface graphique affiche les choix possibles (Attaquer, Objet, Mechas, Fuite) et permet au joueur de naviguer entre eux avec les touches directionnelles.
 * L'action sélectionnée est validée avec la touche `A`.
 *
 * @param joueur        Pointeur vers la structure contenant les informations du joueur (inventaire, Méchas disponibles).
 * @param mecha_sauvage Pointeur vers le Mécha adverse (sauvage ou PNJ).
 * @param actif         Pointeur vers l'entier représentant l'indice du Mécha actuellement actif du joueur.
 * @param type_combat   Type du combat : 
 *                      - `0` pour combat contre un Mécha sauvage.
 *                      - `1` pour combat contre un PNJ.
 *
 * @return Retourne un entier indiquant l'action effectuée ou le résultat du tour :
 *  - `OK` (action réussie, tour terminé normalement).
 *  - `CAPTURE` (capture réussie du Mécha sauvage).
 *  - `FUITE` (le joueur quitte le combat en fermant la fenêtre ou via la touche Escape).
 *
 * @pre
 * - Les pointeurs `joueur`, `mecha_sauvage` et `actif` doivent être valides et initialisés.
 * - La fenêtre SDL et le renderer doivent être initialisés.
 *
 * @post
 * - Met à jour les PV, l'inventaire et/ou les Méchas actifs en fonction des actions effectuées.
 * - Affiche l'état actualisé du combat après chaque action.
 *
 * @note
 * - L'action "Fuite" est affichée à l'écran mais n'est pas sélectionnable par le joueur (implémentation visuelle uniquement).
 * - Les choix d'action se font uniquement entre les options "Attaquer", "Objet" et "Mechas".
 * - L'utilisation d'un objet (notamment Mechaball) peut mener à la fin du combat via capture.
 *
 * @warning
 * - Cette fonction gère son propre cycle d'affichage et d'événements SDL, bloquant ainsi jusqu'à ce qu'une action valide soit choisie par le joueur.
 * - Elle ne vérifie pas si tous les Méchas du joueur sont hors combat (cette vérification doit être faite à l'extérieur).
 *
 * @param[in,out] joueur Structure du joueur mise à jour selon les actions effectuées.
 * @param[in,out] actif Indice du Mécha actif pouvant être modifié si le joueur change de Mécha.
 *
 * @bug
 * - L'option "Fuite" n'est pas fonctionnelle via le menu visuel (prévue en cas de fermeture directe du jeu).
 *
 */
int tour_joueur(joueur_t *joueur, mechas_joueur_t *mecha_sauvage, int * actif, int type_combat) {       //type_combat : combat_sauv = 0 o pnj = 1
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    int running = 1;
    int win_w, win_h;
    int res;
    SDL_GetRendererOutputSize(game.renderer, &win_w, &win_h);       //dimensions ecran
    
    //couleurs des bordures
    SDL_Color couleur_bordure =  {94, 99, 102, 250};
    SDL_Color couleur_bordure_attaque = {210, 145, 132, 200};
    SDL_Color couleur_bordure_objet = {229, 185, 135, 200};
    SDL_Color couleur_bordure_mecha = {162, 202, 154, 200};
    int choix = 1;
    
    // Définition des tailles des boutons
    int btn_w = rect_bas.rect.w / 5; // Largeur des boutons (1/5 de l'écran)
    int btn_h = rect_bas.rect.h / 2; // Hauteur des boutons (1/2 du rect_bas)
    int btn_margin = btn_w / 2; // Marge entre les boutons
    int border_size = 5 * game.scale; // Épaisseur des bords
    
    // Positionnement des boutons
    int btn_center_x = (rect_bas.rect.w - btn_w) / 2;
    int btn_center_y = rect_bas.rect.y + (rect_bas.rect.h - btn_h) / 2;
    int btn_left_x = btn_center_x - btn_w - btn_margin;
    int btn_right_x = btn_center_x + btn_w + btn_margin;
    int btn_fuite_y = btn_center_y + btn_h + btn_margin;

    
    rectangle_t rect_attaque, rect_objet, rect_changer_mecha, rect_fuite;
    rectangle_t border_attaque, border_objet, border_mecha;
    
    // Création des rectangles
    creer_rectangle(&rect_attaque, btn_w, btn_h, btn_center_x, btn_center_y, 165, 36, 8, 250, "Attaquer");
    creer_rectangle(&rect_objet, btn_w, btn_h, btn_left_x, btn_center_y, 204, 116, 16, 250, "Objet");
    creer_rectangle(&rect_changer_mecha, btn_w, btn_h, btn_right_x, btn_center_y, 69, 150, 54, 250, "Mechas");
    creer_rectangle(&rect_fuite, btn_w, btn_h / 1.5, btn_center_x, btn_fuite_y, 7, 103, 160, 250, "Fuite");
    
    // Création des bordures
    creer_rectangle(&border_attaque, btn_w + 2 * border_size, btn_h + 2 * border_size, btn_center_x - border_size, btn_center_y - border_size, 210, 145, 132, 255, NULL);
    creer_rectangle(&border_objet, btn_w + 2 * border_size, btn_h + 2 * border_size, btn_left_x - border_size, btn_center_y - border_size, 94, 99, 102, 255, NULL);
    creer_rectangle(&border_mecha, btn_w + 2 * border_size, btn_h + 2 * border_size, btn_right_x - border_size, btn_center_y - border_size, 94, 99, 102, 255, NULL);
    //creer_rectangle(&border_fuite, btn_w + 2 * border_size, (btn_h / 1.5) + 2 * border_size, btn_center_x - border_size, btn_fuite_y - border_size, 94, 99, 102, 255, NULL);
    
    while (running) {
        frameStart = SDL_GetTicks();
        
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                return FUITE;
            }
            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_LEFT){
                    choix = (choix - 1 + 3) % 3;
                }
                if(event.key.keysym.sym == SDLK_RIGHT){
                    choix = (choix + 1) % 3;
                }
                if(event.key.keysym.sym == SDLK_a) {
                    switch(choix) {
                        case 0 : 
                                res = utilisation_objet( joueur, mecha_sauvage, actif, type_combat);
                                if(res == CAPTURE) {
                                    SDL_Delay(T); 
                                    return CAPTURE;
                                }
                                else if(res!= RETOUR){
                                    SDL_Delay(T); 
                                    running = 0;
                                }
                                break;
                        case 1 :
                            if(attaque_joueur( joueur, mecha_sauvage, actif) == OK) {
                                SDL_Delay(T); 
                                return OK;
                            }
                            break;
                        case 2 : 
                            res = changer_mecha(joueur, actif, mecha_sauvage);
                            if(res == OK){
                                SDL_Delay(T);
                                return OK;
                            }
                    }
                }
                SDL_FlushEvent(SDL_KEYDOWN);
                SDL_Event e;
                while (SDL_WaitEvent(&e) && e.type != SDL_KEYUP);
            }
            
        }
        if(running) {
            switch(choix){
                case 0 : border_objet.couleur = couleur_bordure_objet;
                    border_attaque.couleur = couleur_bordure; 
                    border_mecha.couleur = couleur_bordure; break;
                case 1 : border_objet.couleur = couleur_bordure; 
                    border_attaque.couleur = couleur_bordure_attaque; 
                    border_mecha.couleur = couleur_bordure; break;
                case 2 : border_objet.couleur = couleur_bordure; 
                    border_attaque.couleur = couleur_bordure; 
                    border_mecha.couleur = couleur_bordure_mecha; break;
            }
            
            SDL_RenderClear(game.renderer);
            draw_combat(&(joueur->mechas_joueur[*actif]), mecha_sauvage);
            draw_all_rect(6, &border_attaque, &border_objet, &border_mecha, &rect_attaque, &rect_objet, &rect_changer_mecha, &rect_fuite);
            SDL_RenderPresent(game.renderer);
            
            frameTime = SDL_GetTicks() - frameStart;
            if (FRAME_DELAYS > frameTime) {
                SDL_Delay(FRAME_DELAYS - frameTime);
            }
        }
    }
    return OK;
}

/**
 * @brief Gère un combat complet entre le joueur et un PNJ.
 *
 * Cette fonction orchestre l'intégralité d'un combat contre un PNJ, en gérant :
 *  - La sélection automatique du premier Mécha actif disponible chez le joueur et le PNJ.
 *  - Le cycle complet des tours joueur et PNJ jusqu’à la victoire ou la défaite.
 *  - Le changement automatique de Méchas lorsque l'un d'entre eux tombe KO.
 *  - La gestion spécifique empêchant le joueur de fuir un combat contre un PNJ.
 *  - Le gain d'expérience et la montée en niveau des Méchas après avoir vaincu un adversaire.
 *  - La sauvegarde automatique de l'état du PNJ après le combat (si victoire).
 *
 * @param joueur Un pointeur vers la structure `joueur_t` représentant le joueur.
 * @param pnj Un pointeur vers la structure `pnj_t` représentant l'adversaire PNJ.
 *
 * @return Retourne un entier indiquant le résultat du combat :
 * - `VRAI` si le joueur gagne le combat.
 * - `FAUX` si le joueur perd le combat (tous ses Méchas sont KO).
 *
 * @pre
 * - Les pointeurs `joueur` et `pnj` doivent être initialisés.
 * - Les structures associées aux Méchas des deux participants doivent être valides.
 *
 * @post
 * - Met à jour les PV des Méchas impliqués dans le combat.
 * - Modifie l'état du PNJ à `1` (déjà combattu) si le joueur gagne.
 * - Sauvegarde automatiquement l'état modifié du PNJ après victoire.
 *
 * @warning
 * - Le joueur ne peut pas fuir un combat contre un PNJ (tentative déclenche un message spécifique).
 * - Si le joueur possède le Mécha `Aeroshima` (id_mechas = 23), celui-ci ne peut pas participer contre les fils d'Iron Musk. Un changement forcé est demandé.
 *
 * @note
 * - Cette fonction gère entièrement la boucle de combat, bloquant ainsi jusqu’à son issue finale (victoire ou défaite).
 * - La montée de niveau (`level_mechas`) est automatiquement gérée à chaque victoire contre un Mécha adverse.
 * - En cas de défaite, les PV du PNJ sont restaurés à leur maximum initial.
 *
 * @param[in,out] joueur La structure du joueur mise à jour selon l’issue du combat (PV, expérience gagnée).
 * @param[in,out] pnj La structure du PNJ mise à jour selon l’issue du combat (PV mis à jour ou restaurés, état modifié en cas de victoire du joueur).
 *
 */
int combat_pnj(joueur_t *joueur, pnj_t *pnj) {
    int save_map_active = game.mat_active;
    init_rect_bas();
    game.mat_active = 6;
    int actif_joueur = 0, actif_pnj = 0;
    int res = OK;
    int existe_joueur[4] = {0}, existe_pnj[4] = {0};
    
    // Vérification des méchas disponibles
    for(int i = 0; i < 4; i++) {
        if (joueur->mechas_joueur[i].numero == i+1 && joueur->mechas_joueur[i].pv > 0 && joueur->nb_mechas > i)
            existe_joueur[i] = 1;
        if (pnj->mechas_joueur[i].numero == i+1 && pnj->mechas_joueur[i].pv > 0 && pnj->nb_mechas > i)
            existe_pnj[i] = 1;
    }
    
    // Sélection du premier mécha joueur en état de combattre
    while (actif_joueur < 4 && joueur->mechas_joueur[actif_joueur].pv == 0) {
        actif_joueur++;
    }
    int combat_encours = 1;
    while (combat_encours && res == OK) {
        while (pnj->mechas_joueur[actif_pnj].pv > 0 && joueur->mechas_joueur[actif_joueur].pv > 0 && res == OK) {
            if (joueur->mechas_joueur[actif_joueur].vitesse > pnj->mechas_joueur[actif_pnj].vitesse) {
                do {
                    res = tour_joueur(joueur, &(pnj->mechas_joueur[actif_pnj]), &actif_joueur, 1);
                    if (res == FUITE) {
                        afficher_dialogue_combat(&(joueur->mechas_joueur[actif_joueur]), &(pnj->mechas_joueur[actif_pnj]), "Systeme", "  Vous ne pouvez pas fuir pendant ce combat !", false);
                    }
                } while (res == FUITE);
                
                if (pnj->mechas_joueur[actif_pnj].pv > 0 && res == OK) {
                    attaque_ordi_pnj(pnj, &(joueur->mechas_joueur[actif_joueur]), &actif_pnj);
                }
            } else {
                attaque_ordi_pnj(pnj, &(joueur->mechas_joueur[actif_joueur]), &actif_pnj);
                if (joueur->mechas_joueur[actif_joueur].pv > 0) {
                    res = tour_joueur(joueur, &(pnj->mechas_joueur[actif_pnj]), &actif_joueur, 1);
                }
            }
        }
        
        if (joueur->mechas_joueur[actif_joueur].pv == 0) {
            int nouveau_joueur = -1;
            for (int i = 0; i < 4; i++) {
                if (existe_joueur[i] && joueur->mechas_joueur[i].pv > 0) {
                    nouveau_joueur = i;
                    break;
                }
            }
            if (nouveau_joueur != -1) {
                while (changer_mecha(joueur, &actif_joueur, &(pnj->mechas_joueur[actif_pnj])) != 1);
            } else {
                combat_encours = 0;
            }
        } else if (pnj->mechas_joueur[actif_pnj].pv == 0) {
            int nouveau_pnj = -1;
            level_mechas(joueur,&pnj->mechas_joueur[actif_pnj]);
            for (int i = 0; i < 4; i++) {
                if (existe_pnj[i] && pnj->mechas_joueur[i].pv > 0) {
                    nouveau_pnj = i;
                    break;
                }
            }
            if (nouveau_pnj != -1) {
                actif_pnj = nouveau_pnj;
                afficher_dialogue_combat(&(joueur->mechas_joueur[actif_joueur]), &(pnj->mechas_joueur[actif_pnj]), "Systeme", "  L'adversaire change de mecha.", false);
            } else {
                combat_encours = 0;
            }
        }
    }
    
    if (joueur->mechas_joueur[actif_joueur].pv == 0) { 
        afficher_dialogue_combat(&(joueur->mechas_joueur[actif_joueur]), &(pnj->mechas_joueur[actif_pnj]), "Systeme", "Vous avez perdu !", false);
        for(int i = 0;i<pnj->nb_mechas;i++) pnj->mechas_joueur[i].pv = pnj->mechas_joueur[i].pv_max;
        game.mat_active = save_map_active;
        return FAUX;
    }
    pnj->etat = 1;
    sauvegarde_pnj(pnj,pnj->id_pnj,joueur->pseudo);
    afficher_dialogue_combat(  &(joueur->mechas_joueur[actif_joueur]), &(pnj->mechas_joueur[actif_pnj]), "Systeme", "Vous avez gagne !",false);
    game.mat_active = save_map_active;
    return VRAI;
}


/**
 * @brief Gère un combat entre le joueur et un Mécha sauvage.
 *
 * Cette fonction permet au joueur de combattre un Mécha sauvage en gérant :
 *  - L'ordre des tours basé sur la vitesse des Méchas.
 *  - La possibilité pour le joueur d'utiliser des objets, d'attaquer ou de changer de Mécha.
 *  - La fuite possible du joueur en cours de combat.
 *  - Le changement automatique de Mécha si celui-ci est KO.
 *  - La montée de niveau des Méchas du joueur en cas de victoire.
 *
 * @param joueur Un pointeur vers la structure `joueur_t` représentant le joueur.
 * @param mecha_sauvage Un pointeur vers la structure `mechas_joueur_t` représentant le Mécha sauvage.
 *
 * @return Retourne un entier indiquant le résultat du combat :
 * - `VRAI` si le joueur gagne le combat ou réussit à fuir.
 * - `FAUX` si tous les Méchas du joueur sont KO.
 *
 * @pre
 * - Les pointeurs `joueur` et `mecha_sauvage` doivent être valides.
 * - Le Mécha sauvage doit être initialisé avec ses PV, attaques, etc.
 *
 * @post
 * - Met à jour les PV et l'expérience (`xp`) des Méchas du joueur.
 * - Permet éventuellement au joueur d'augmenter de niveau ou d'apprendre de nouvelles attaques après une victoire.
 * - Permet de fuir le combat en conservant l'état actuel des Méchas.
 *
 * @note
 * - Le joueur peut fuir un combat contre un Mécha sauvage (à l'inverse des combats contre les PNJ).
 * - L'XP gagnée est distribuée uniquement si le Mécha sauvage est vaincu.
 * - L'affichage du combat est restauré après la fin du combat (restauration de `game.mat_active`).
 *
 * @warning
 * - Si tous les Méchas du joueur sont KO, le combat est perdu automatiquement.
 * - Si le joueur choisit de fuir, le combat se termine immédiatement.
 */
int combat_sauvage(joueur_t *joueur, mechas_joueur_t *mecha_sauvage) {
    int save_map_active = game.mat_active;
    init_rect_bas();
    game.mat_active = 6;
    int actif = 0;
    int res = OK;
    int verif;
    int existe[4] = {0,0,0,0};
    for(int i = 0; i < 4 && i < joueur->nb_mechas; i++) {
        if(joueur->mechas_joueur[i].numero == i+1 && joueur->mechas_joueur[i].pv > 0)
            existe[i] = 1;
    }
    while(joueur->mechas_joueur[actif].pv == 0) {
        actif++;
    }
    do {
        while(mecha_sauvage->pv > 0 && joueur->mechas_joueur[actif].pv > 0 && res == OK) {
            if(joueur->mechas_joueur[actif].vitesse > mecha_sauvage->vitesse) {
                res = tour_joueur(joueur, mecha_sauvage,  &actif, 0);
                
                if(mecha_sauvage->pv != 0 && res == OK)
                    attaque_ordi_sauvage(mecha_sauvage, &(joueur->mechas_joueur[actif]));
            }
            else {
                attaque_ordi_sauvage(mecha_sauvage, &(joueur->mechas_joueur[actif]));
                if(joueur->mechas_joueur[actif].pv != 0) {
                    res = tour_joueur(joueur, mecha_sauvage,  &actif, 0);
                }
            }
        }
        if(mecha_sauvage->pv > 0 && res == OK) {
            verif = 0;
            for(int i = 0; i < 4; i++) {
                if(existe[i] && joueur->mechas_joueur[i].pv > 0)
                    verif = 1;
            }
            if(verif) {
                while( changer_mecha(joueur, &actif, mecha_sauvage) != 1);
            }
        }
    }while(verif && mecha_sauvage->pv > 0 && res == OK);
    if(mecha_sauvage->pv != 0 && joueur->mechas_joueur[actif].pv == 0) {
        afficher_dialogue_combat(  &(joueur->mechas_joueur[actif]), mecha_sauvage, "Systeme", "  Vous avez perdu !",false);
        return FAUX;
    }
    else if(mecha_sauvage->pv == 0 && joueur->mechas_joueur[actif].pv != 0){
        level_mechas(joueur,mecha_sauvage);
        afficher_dialogue_combat(  &(joueur->mechas_joueur[actif]), mecha_sauvage, "Systeme", "  Vous avez gagne !",false);
        game.mat_active = save_map_active;
        return VRAI;
    }
    else if(res == FUITE){
        afficher_dialogue_combat( &(joueur->mechas_joueur[actif]), mecha_sauvage, "Systeme", "  Vous prennez la fuite!",false);
        game.mat_active = save_map_active;
        return VRAI;
    }
    game.mat_active = save_map_active;
    return VRAI;
}


