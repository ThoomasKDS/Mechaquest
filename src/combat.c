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
 * @brief Affiche l'écran de sélection des mechas pour le combat.
 * 
 * Cette fonction affiche une interface permettant au joueur de sélectionner l'un de ses mechas 
 * pour le combat en cours. Elle crée des rectangles interactifs affichant les informations des mechas 
 * disponibles et gère la navigation avec les touches directionnelles.
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer et les textures.
 * @param joueur Pointeur vers la structure du joueur contenant ses mechas.
 * @return L'index du mecha sélectionné (0 à 3) en cas de succès, -1 si aucun mecha n'est sélectionné.
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
        if(joueur->mechas_joueur[i].numero == i + 1){
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
 * @brief Affiche l'interface de sélection des objets et gère leur utilisation en combat.
 * 
 * Cette fonction permet au joueur de sélectionner et d'utiliser un objet en combat 
 * parmi le carburant, le rappel ou la mechaball. L'interface affiche les quantités 
 * disponibles et met en surbrillance l'option sélectionnée. La navigation se fait avec 
 * les touches directionnelles et la sélection avec la touche d'action.
 * 
 * @param game Pointeur vers la structure du jeu contenant le renderer et l'affichage.
 * @param joueur Pointeur vers la structure du joueur contenant son inventaire.
 * @param ordi Pointeur vers le mecha adverse (utilisé si la mechaball est sélectionnée).
 * @return Retourne 1 si un objet a été utilisé avec succès, 0 si le joueur quitte l'interface.
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
 * @brief Utilise un carburant pour restaurer les PV d'un mecha en combat.
 * 
 * Cette fonction permet au joueur d'utiliser un carburant pour restaurer 20 PV 
 * au mecha sélectionné. L'utilisation est soumise à certaines conditions :
 * - Le joueur doit posséder du carburant.
 * - Le mecha sélectionné ne doit pas avoir ses PV au maximum.
 * - Un mecha KO (PV à 0) ne peut pas être soigné avec du carburant.
 * 
 * @param game Pointeur vers la structure du jeu contenant l'affichage et l'état du combat.
 * @param joueur Pointeur vers la structure du joueur contenant son inventaire et ses mechas.
 * @return Retourne 1 si l'utilisation est réussie, 0 si l'utilisation est impossible.
 */
int utilisation_carburant(joueur_t *joueur, mechas_joueur_t *ordi, int *actif){
    if(joueur->inventaire->carburant <= 0){
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Auncun carburant.",false);
        return (KO);
    }
    else {
        int i = aff_mechas_combat(joueur); //recupere le numero du mechas selectionne
        if(i == -1 || i == 4){  //si on a rien choisi
            return (RETOUR);
        }
        else if(joueur->mechas_joueur[i].pv == joueur->mechas_joueur[i].pv_max){    //Si les PV sont au Max, on ne peut pas utiliser de carburant
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Les PV sont deja au maximun !",false);
            return (RETOUR);
        }
        else if(joueur->mechas_joueur[i].pv < 0){  //Si les PV sont a 0, impossible d'utiliser du carburant
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Vous ne pouvez pas utiliser de carburant.",false);
            return (RETOUR);
        }
        else{
            joueur->mechas_joueur[i].pv += 20;  //Ajouter 20 aux PV actuels
            if(joueur->mechas_joueur[i].pv > joueur->mechas_joueur[i].pv_max){
                joueur->mechas_joueur[i].pv = joueur->mechas_joueur[i].pv_max;
            }
            joueur->inventaire->carburant--;
            char message[200] = "Utilisation carburant, nouveau PV : ";
            concat(message, joueur->mechas_joueur[i].pv);
            strcat(message, ".");
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", message,false);
        }
    }

    return (OK);

}

/**
 * @brief Utilise un rappel pour ranimer un mecha KO en combat.
 * 
 * Cette fonction permet au joueur d'utiliser un rappel pour restaurer à 50% de ses PV max 
 * un mecha qui a été mis KO (PV à 0). L'utilisation est soumise à certaines conditions :
 * - Le joueur doit posséder un rappel dans son inventaire.
 * - Le mecha sélectionné doit être KO (PV = 0).
 * - Un rappel ne peut pas être utilisé sur un mecha encore en vie.
 * 
 * @param game Pointeur vers la structure du jeu contenant l'affichage et l'état du combat.
 * @param joueur Pointeur vers la structure du joueur contenant son inventaire et ses mechas.
 * @return Retourne 1 si l'utilisation est réussie, 0 si l'utilisation est impossible.
 */
int utilisation_rappel(joueur_t *joueur, mechas_joueur_t *ordi, int *actif){
    if(joueur->inventaire->rappel <= 0){
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Aucun rappel.",false);
        return (KO);
    }
    else {
        int i = aff_mechas_combat(joueur); //recupere le numero du mechas selectionne
        if(i == -1 || i == 4){ //si on a pas choisi de mecha
            return (RETOUR);
        }
        else if(joueur->mechas_joueur[i].pv != 0){  //Si les PV sont pas a 0, impossible d'utiliser du rappel
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Vous ne pouvez pas utiliser de rappel sur un mechas encore en vie.",false);
            return (RETOUR);
        }
        else{
            joueur->mechas_joueur[i].pv = joueur->mechas_joueur[i].pv_max/2;  //Remettre les PV au max
            char message[200] = "";
            strcat(message, mecha[joueur->mechas_joueur[i].id_mechas - 1].nom);
            strcat(message, "est de retour !");
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", message,false);
            joueur->inventaire->rappel--;
            
        }
    }

    return (OK);
}

/**
 * @brief Utilise une Mechaball pour tenter de capturer un mecha sauvage.
 * 
 * Cette fonction permet au joueur de lancer une Mechaball pour capturer un mecha sauvage.
 * Le taux de capture est influencé par le type du mecha (les mechas nucléaires sont plus difficiles à attraper) 
 * et par le rapport entre ses PV actuels et ses PV max. 
 * 
 * Conditions d'utilisation :
 * - Le joueur doit posséder au moins une Mechaball.
 * - Le taux de capture est calculé en fonction des PV restants du mecha sauvage et de son type.
 * - Si la capture réussit, le mecha est ajouté à l'équipe du joueur.
 * 
 * @param game Pointeur vers la structure du jeu contenant l'affichage et les mécaniques de combat.
 * @param joueur Pointeur vers la structure du joueur contenant son inventaire et ses mechas.
 * @param ordi Pointeur vers la structure du mecha sauvage à capturer.
 * @return Retourne 1 si la capture réussit, -1 si elle échoue, et 0 si le joueur n'a pas de Mechaball.
 */
int utilisation_mechaball(joueur_t * joueur, mechas_joueur_t *ordi, int *actif) {
    if(joueur->inventaire->mechaball <= 0){
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Auncune mechaball.",false);
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
            char message[200] = "Vous avez attrape ";
            strcat(message,  mecha[ordi->id_mechas-1].nom);
            strcat(message, ".");
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", message,false);
            copie_mechas(joueur, ordi);
            return (CAPTURE);
        }
        else {
            char message[200] = "Vous n avez pas reussi a attrape ";
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
 * @brief Gère le calcul des dégâts et l'application d'une attaque en combat.
 * 
 * Cette fonction applique une attaque choisie par un mecha attaquant sur un mecha défenseur.
 * Elle prend en compte plusieurs paramètres :
 * - La précision de l'attaque (si elle touche ou échoue).
 * - Les dégâts de base de l'attaque.
 * - Les statistiques d'attaque et de défense des mechas.
 * - L'efficacité de l'attaque en fonction des types de mechas (avantages et désavantages élémentaires).
 * 
 * Conditions :
 * - Si l'attaque rate, aucun dégât n'est infligé.
 * - Une attaque de type "Uranium" inflige des dégâts supplémentaires sauf contre un autre mecha de type "Uranium".
 * - Certaines attaques peuvent être plus efficaces contre certains types de mechas.
 * - Les utilisations de l'attaque sont décrémentées après chaque action.
 * 
 * @param choix Index de l'attaque choisie (0 ou 1).
 * @param mecha_att Pointeur vers le mecha attaquant.
 * @param mecha_def Pointeur vers le mecha défenseur.
 * @return Retourne 1 si l'attaque a été effectuée, 0 si elle est annulée.
 */
int algo_attaque(int choix, mechas_joueur_t *mecha_att, mechas_joueur_t *mecha_def, int mecha_choix) {
    if(choix == 2) return 0;
    if(choix == 0 && mecha_att->utilisation_1 == 0) {
        afficher_dialogue_combat(  mecha_att, mecha_def, "Systeme", "Vous ne pouvez plus utiliser cette attaque.",false);
        return KO;
    }
    if(choix == 1 && mecha_att->utilisation_2 == 0) {
        afficher_dialogue_combat(  mecha_att, mecha_def, "Systeme", "Vous ne pouvez plus utiliser cette attaque.",false);
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
    char msg[300];
    strcpy(msg, mecha[mecha_att->id_mechas - 1].nom);
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
            afficher_dialogue_combat(mecha_att, mecha_def, "Systeme", "L attaque echoue.",false);
        else
            afficher_dialogue_combat(mecha_def,mecha_att, "Systeme", "L attaque echoue.",false);
    }
    return OK;
}

/**
 * @brief Affiche l'interface de sélection d'attaque et applique l'attaque choisie par le joueur.
 * 
 * Cette fonction permet au joueur de sélectionner une attaque parmi les deux disponibles 
 * pour son mecha actif et de l'appliquer sur le mecha adverse. L'interface affiche les attaques 
 * avec leurs statistiques (dégâts, utilisations restantes) et permet de naviguer avec les touches 
 * directionnelles. Une attaque est déclenchée avec la touche d'action.
 * 
 * Fonctionnalités :
 * - Affichage des attaques disponibles avec leur type, dégâts et utilisations restantes.
 * - Navigation entre les attaques et l'option de retour.
 * - Application de l'attaque sélectionnée via `algo_attaque`.
 * 
 * @param game Pointeur vers la structure du jeu contenant l'affichage et le moteur de combat.
 * @param j Pointeur vers la structure du joueur contenant ses mechas.
 * @param mecha_ordi Pointeur vers le mecha adverse.
 * @param actif Pointeur vers l'index du mecha actuellement utilisé par le joueur.
 * @return Retourne le résultat de `algo_attaque`, ou `OK` si aucune attaque n'est sélectionnée.
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
 * @brief Gère l'attaque d'un mecha sauvage contre le joueur.
 * 
 * Cette fonction sélectionne aléatoirement l'une des deux attaques du mecha sauvage 
 * et l'applique au mecha du joueur. Si l'attaque choisie initialement n'est plus utilisable 
 * (nombre d'utilisations épuisé), l'autre attaque est tentée. Si les deux attaques ne peuvent 
 * pas être utilisées, le mecha sauvage ne peut plus attaquer.
 * 
 * Fonctionnalités :
 * - Sélection aléatoire entre les deux attaques disponibles.
 * - Vérification du nombre d'utilisations restantes avant de lancer une attaque.
 * - Exécution de l'attaque via `algo_attaque`.
 * 
 * @param mecha_ordi Pointeur vers la structure du mecha sauvage attaquant.
 * @param mecha_joueur Pointeur vers la structure du mecha du joueur subissant l'attaque.
 * @return Retourne `OK` si une attaque a été effectuée, ou `0` si le mecha sauvage ne peut plus attaquer.
 */
int attaque_ordi_sauvage(mechas_joueur_t *mecha_ordi, mechas_joueur_t *mecha_joueur){  //Retourne 0 si ne peut pas attaquer
    int numero_attaque;
    int utilisation[2] = {mecha_ordi->utilisation_1, mecha_ordi->utilisation_2};
    numero_attaque = rand() % 2;
    if(!utilisation[numero_attaque]){
        numero_attaque = 1 - numero_attaque;
        if(!utilisation[numero_attaque]) {
            afficher_dialogue_combat(  mecha_joueur, mecha_ordi, "Systeme", "Le mecha sauvage ne peut plus attaquer.",false);            
            return 0;
        }
        algo_attaque(numero_attaque, mecha_ordi, mecha_joueur, 0);
       
    }
    else{
        algo_attaque(numero_attaque, mecha_ordi, mecha_joueur, 0);
    }
    return OK;
}


int attaque_ordi_pnj(pnj_t * pnj, mechas_joueur_t *mecha_joueur, int * actif){
    int nbr_rand_choix1 = 1, nbr_rand_choix = 0;

    if(pnj->mechas_joueur[*actif].pv < pnj->mechas_joueur[*actif].pv_max/4) { //si les pv sont < a un quart des pv max on a 1/4 chances de changer de mechas
        nbr_rand_choix1 = (rand() % 4);
        printf("nbr_rand_choix1 = %d\n, pv actif : %d, pv max : %d\n", nbr_rand_choix1, pnj->mechas_joueur[*actif].pv, pnj->mechas_joueur[*actif].pv_max);
    }
    if(nbr_rand_choix1 > 0) {
        if(pnj->mechas_joueur[*actif].pv <= pnj->mechas_joueur[*actif].pv_max / 2){   //Si les PV sont inferieurs a la moitie des PV Max 1 chance sur 2 d'utiliser un carburant
            nbr_rand_choix = (rand() % 2);
        }

        if(nbr_rand_choix){    //Utilisation d'un carburant
            if(pnj->inventaire->carburant <= 0){
                nbr_rand_choix = 0;
            }
            else{
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
        printf("Changement de mecha\n");
        for(int i = 0; i < 4; i++) {
            if(pnj->mechas_joueur[i].pv > 0 && i != (*actif)) {
                (*actif) = i;
                break;
            }
        }
        afficher_dialogue_combat(mecha_joueur ,  &(pnj->mechas_joueur[*actif]), "Systeme", "  L adversaire change de mecha.",false); 
    }

    return OK;
}

/*
===========================================
*/

/**
 * @brief Permet au joueur de changer de mecha actif en combat.
 * 
 * Cette fonction affiche l'interface de sélection des mechas via `aff_mechas_combat` 
 * et permet au joueur de choisir un nouveau mecha actif. 
 * 
 * Conditions :
 * - Si le joueur sélectionne le même mecha que celui déjà actif, le changement est annulé.
 * - Si l'option de retour est choisie (`choix == 4`), le changement est annulé.
 * - Sinon, le mecha actif est mis à jour avec la sélection du joueur.
 * 
 * @param game Pointeur vers la structure du jeu contenant l'affichage et l'état du combat.
 * @param joueur Pointeur vers la structure du joueur contenant ses mechas.
 * @param actif Pointeur vers l'index du mecha actuellement actif.
 * @return Retourne 1 si le changement de mecha est effectué, 0 si aucune modification n'a été faite.
 */
int changer_mecha(joueur_t *joueur, int *actif, mechas_joueur_t *ordi){
    int choix ;
    choix = aff_mechas_combat(joueur);
    if (joueur->mechas_joueur[choix].pv == 0 && choix != 4) {
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Ce mecha est mort.",false); 
        return KO;
    }
    if(choix == *actif) {
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Ce mecha est deja actif.",false);
        return KO;
    }
    if(choix == 4) return RETOUR;
    afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Vous changez de mecha.",false);
    *actif = choix;
    return OK;
}

/**
 * @brief Permet à un mecha d'apprendre une nouvelle attaque lorsqu'il atteint un niveau requis.
 * 
 * Cette fonction vérifie si un mecha a atteint le niveau nécessaire pour apprendre une nouvelle attaque. 
 * Si c'est le cas, le joueur a la possibilité de remplacer l'une des deux attaques existantes du mecha par la nouvelle attaque.
 * 
 * Fonctionnalités :
 * - Vérifie si une nouvelle attaque est disponible pour le mecha à son niveau actuel.
 * - Affiche les détails de la nouvelle attaque ainsi que celles actuellement équipées.
 * - Permet au joueur de choisir s'il souhaite remplacer une attaque existante ou conserver ses attaques actuelles.
 * - Met à jour les attaques du mecha en fonction du choix du joueur.
 * 
 * @param mecha_joueur Pointeur vers la structure du mecha du joueur qui peut apprendre une nouvelle attaque.
 * @return Retourne `OK` après l'exécution, indépendamment du choix du joueur.
 */
int apprentissage_attaque(mechas_joueur_t *mecha_joueur, mechas_joueur_t *mecha_ordi){
    int i;
    int choix = -1;
    char text[300];

    for(i = 0;attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1 ].niveau != mecha_joueur->niveau && i<5 ;i++);
    if(attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1 ].niveau == mecha_joueur->niveau){
        sprintf(text," Votre mecha souhaite apprendre une nouvelle attaque: %s\n Degats:%d\n Precision:%d.",
        attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].nom,attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].degats,
        attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i]-1].precision);
        afficher_dialogue_combat(mecha_joueur,mecha_ordi,"Systeme",text,false);
        sprintf(text," Choisissez l'attaque a remplacer: \n 1 - Attaque 1: %s - Degats: %d - Precision: %d\n 2 - Attaque 2: %s - Degats: %d - Precision: %d \n 3: Garder ses attaques.",
        attaque[mecha_joueur->attaque_1 -1].nom,attaque[mecha_joueur->attaque_1 -1].degats,attaque[mecha_joueur->attaque_1 -1].precision,
        attaque[mecha_joueur->attaque_2 -1].nom,attaque[mecha_joueur->attaque_2 -1].degats,attaque[mecha_joueur->attaque_2 -1].precision);
        choix = afficher_dialogue_combat(mecha_joueur,mecha_ordi,"Systeme",text,true);
        switch(choix){
            case 1:
                sprintf(text," Vous avez remplace votre attaque %s par %s.",
                attaque[mecha_joueur->attaque_1-1].nom,attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i]-1].nom);
                mecha_joueur->attaque_1 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].id_attaques;
                mecha_joueur->utilisation_1 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].utilisations;
                 afficher_dialogue_combat(mecha_joueur,mecha_ordi,"Systeme",text,false);
                break;
            case 2:
                sprintf(text," Vous avez remplace votre attaque %s par %s.",
                attaque[mecha_joueur->attaque_1 -1].nom,attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].nom);
                mecha_joueur->attaque_2 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].id_attaques;
                mecha_joueur->utilisation_2 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].utilisations;
                afficher_dialogue_combat(mecha_joueur,mecha_ordi,"Systeme",text,false);
                break;
            default:
                afficher_dialogue_combat(mecha_joueur,mecha_ordi,"Systeme"," Vous gardez votre attaque actuelle.",false);
        }
           
    }
    return OK;
}

/**
 * @brief Vérifie si un mecha peut évoluer et applique son évolution.
 * 
 * Cette fonction vérifie si un mecha a atteint le niveau requis pour évoluer 
 * et si une évolution est disponible. Si ces conditions sont remplies, 
 * le mecha évolue en son stade supérieur.
 * 
 * Fonctionnalités :
 * - Vérifie si le niveau du mecha atteint le niveau d'évolution défini.
 * - Vérifie si le mecha possède une évolution disponible.
 * - Met à jour l'ID du mecha pour refléter son évolution.
 * - Affiche un message indiquant la réussite de l'évolution.
 * 
 * @param mecha_joueur Pointeur vers la structure du mecha du joueur pouvant évoluer.
 * @return Retourne `OK` après l'exécution, que le mecha évolue ou non.
 */
int evolution_mechas(mechas_joueur_t *mecha_joueur, mechas_joueur_t *mecha_ordi){
    char text[200];
    if(mecha_joueur->niveau >= mecha[mecha_joueur->id_mechas -1].niveau_evolution && mecha[mecha_joueur->id_mechas -1].evolution > 0){
        mecha_joueur->id_mechas++;
        sprintf(text,"%s a evolue en %s.",mecha[mecha_joueur->id_mechas -2].nom,mecha[mecha_joueur->id_mechas -1].nom );

        afficher_dialogue_combat(mecha_joueur, mecha_ordi, "Systeme", text,false);
        printf("%s\n",text);

    }
    return OK;
}

/**
 * @brief Gère la montée de niveau d'un mecha en fonction de l'expérience gagnée.
 * 
 * Cette fonction ajoute l'expérience gagnée (`xp_partage`) au mecha et vérifie 
 * si le niveau suivant est atteint. Si le mecha atteint un nouveau niveau :
 * - Son niveau est incrémenté.
 * - Son XP est ajusté en conséquence.
 * - Le seuil d'XP pour le prochain niveau est recalculé.
 * - L'évolution du mecha est vérifiée et appliquée si possible.
 * - Une nouvelle attaque peut être apprise via `apprentissage_attaque`.
 * 
 * Fonctionnalités :
 * - Ajoute l'expérience gagnée et vérifie si le mecha atteint un nouveau niveau.
 * - Gère la montée de plusieurs niveaux en une seule fois si nécessaire.
 * - Applique l'évolution et l'apprentissage de nouvelles attaques.
 * - Recalcule dynamiquement l'expérience requise pour monter au niveau suivant.
 * 
 * @param mecha Pointeur vers la structure du mecha gagnant de l'expérience.
 * @param xp_partage Quantité d'expérience gagnée par le mecha.
 * @param lvlup Expérience requise pour atteindre le niveau suivant.
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
 * @brief Répartit l'expérience gagnée entre les mechas présents en combat.
 * 
 * Cette fonction distribue l'expérience gagnée à un maximum de 4 mechas présents. 
 * L'expérience est répartie selon un coefficient de répartition fixe, favorisant 
 * les mechas qui ont contribué le plus au combat.
 * 
 * Fonctionnalités :
 * - Détermine combien de mechas reçoivent l'expérience (maximum 4).
 * - Applique un coefficient de répartition basé sur le nombre de mechas actifs.
 * - Calcule l'expérience nécessaire pour le niveau suivant.
 * - Ajoute l'expérience gagnée et déclenche une montée de niveau si applicable via `montee_niveau`.
 * 
 * @param mechas_presents Pointeur vers la structure du joueur contenant les mechas à répartir.
 * @param xp_gagne Quantité d'expérience totale à répartir entre les mechas présents.
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
    printf("%d\n",nb);
    for (int i = 0; i < nb; i++) {
        int lvlup = (int)(15 * exp(0.05 * mechas_presents->mechas_joueur[i].niveau));
        int xp_partage = (int)(xp_gagne * coef_repartition[nb-1][i]); // Conversion propre en int
        montee_niveau(&mechas_presents->mechas_joueur[i],mecha_ordi, xp_partage, lvlup);
    }
}

/**
 * @brief Gère l'attribution de l'expérience et la montée de niveau après la défaite d'un adversaire.
 * 
 * Cette fonction calcule l'expérience gagnée lorsqu'un mecha adverse est vaincu 
 * et la distribue entre les mechas présents en combat. Elle affiche également 
 * les niveaux des mechas après l'attribution de l'expérience.
 * 
 * Fonctionnalités :
 * - Calcule l'expérience gagnée en fonction du niveau du mecha vaincu.
 * - Distribue l'expérience aux mechas actifs via `distribuer_xp`.
 * - Affiche les niveaux des mechas après la distribution.
 * 
 * @param mechas_presents Pointeur vers la structure du joueur contenant les mechas actifs.
 * @param mecha_tue Pointeur vers la structure du joueur ou mecha adverse vaincu.
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
 * @brief Gère le tour du joueur en combat, permettant de choisir une action.
 * 
 * Cette fonction affiche une interface de combat où le joueur peut choisir entre 
 * attaquer, utiliser un objet, changer de mecha ou tenter de fuir. 
 * 
 * Fonctionnalités :
 * - Affiche une interface avec des options interactives.
 * - Permet de naviguer entre les options avec les touches directionnelles.
 * - Exécute l'action sélectionnée par le joueur :
 *   - **Attaquer** : Lance `attaque_joueur`.
 *   - **Utiliser un objet** : Ouvre `utilisation_objet`.
 *   - **Changer de mecha** : Ouvre `changer_mecha`.
 *   - **Fuite** (non implémentée dans ce code).
 * - Met à jour l'affichage des boutons et leur sélection en temps réel.
 * 
 * @param joueur Pointeur vers la structure du joueur participant au combat.
 * @param mecha_sauvage Pointeur vers le mecha sauvage adverse.
 * @param game Pointeur vers la structure du jeu contenant le moteur de rendu et les événements.
 * @param actif Pointeur vers l'index du mecha actif du joueur.
 * @return Retourne 1 si le tour se déroule normalement, 0 si le joueur quitte ou annule.
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
    return OK;
}




int combat_pnj(joueur_t *joueur, pnj_t *pnj) {
    int save_map_active = game.mat_active;
    init_rect_bas();
    game.mat_active = 6;
    int actif_joueur = 0, actif_pnj = 0;
    int res = OK;
    int existe_joueur[4] = {0}, existe_pnj[4] = {0};
    
    // Vérification des méchas disponibles
    for(int i = 0; i < 4; i++) {
        if (joueur->mechas_joueur[i].numero == i+1 && joueur->mechas_joueur[i].pv > 0)
            existe_joueur[i] = 1;
        if (pnj->mechas_joueur[i].numero == i+1 && pnj->mechas_joueur[i].pv > 0)
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
                        afficher_dialogue_combat(&(joueur->mechas_joueur[actif_joueur]), &(pnj->mechas_joueur[actif_pnj]), "Systeme", " Vous ne pouvez pas fuir pendant ce combat !", false);
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
 * @brief Lance un combat entre le joueur et un Mecha sauvage.
 * 
 * Cette fonction gère entièrement le déroulement d'un combat sauvage entre l'un des Mechas du joueur
 * et un Mecha ennemi sauvage. Le combat se déroule à tour de rôle en tenant compte de la vitesse de chaque Mecha.
 * Le Mecha ayant la plus grande vitesse attaque en premier. Le combat se poursuit jusqu'à ce qu'un des deux Mechas 
 * atteigne 0 point de vie.
 *
 * Durant le combat, l'état de la carte active (`game->mat_active`) est modifié temporairement et restauré à la fin.
 * 
 * @param joueur         Pointeur vers la structure du joueur (contenant ses Mechas).
 * @param mecha_sauvage  Pointeur vers le Mecha sauvage rencontré.
 * @param game           Pointeur vers l'état actuel du jeu.
 *
 * @pre joueur, mecha_sauvage et game doivent être des pointeurs valides initialisés avant l'appel.
 * @post Le combat est résolu, affichant un message indiquant si le joueur a gagné ou perdu.
 *       L'état original de `game->mat_active` est restauré.
 *
 * @note Cette fonction appelle `tour_joueur()` pour le tour du joueur et `attaque_ordi_sauvage()` pour le tour du Mecha sauvage.
 */
int combat_sauvage(joueur_t *joueur, mechas_joueur_t *mecha_sauvage) {
    int save_map_active = game.mat_active;
    init_rect_bas();
    game.mat_active = 6;
    int actif = 0;
    int res = OK;
    int verif;
    int existe[4] = {0,0,0,0};
    for(int i = 0; i < 4; i++) {
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
        afficher_dialogue_combat(  &(joueur->mechas_joueur[actif]), mecha_sauvage, "Systeme", "Vous avez perdu !",false);
        return FAUX;
    }
    else if(mecha_sauvage->pv == 0 && joueur->mechas_joueur[actif].pv != 0){
        level_mechas(joueur,mecha_sauvage);
        afficher_dialogue_combat(  &(joueur->mechas_joueur[actif]), mecha_sauvage, "Systeme", "Vous avez gagne !",false);
        game.mat_active = save_map_active;
        return VRAI;
    }
    else if(res == FUITE){
        afficher_dialogue_combat( &(joueur->mechas_joueur[actif]), mecha_sauvage, "Systeme", "Vous prennez la fuite!",false);
        game.mat_active = save_map_active;
        return VRAI;
    }
    game.mat_active = save_map_active;
    return VRAI;
}


