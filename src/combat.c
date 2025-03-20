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

static void concat(char *dest, int nb) { //concatene un entier a une chaine de caractere
    char tmp[10];
    sprintf(tmp, "%d", nb);
    strcat(dest, tmp);
}

void draw_combat( mechas_joueur_t * mecha_joueur, mechas_joueur_t * mecha_ordi) {
    int win_w, win_h;
    SDL_GetRendererOutputSize(game.renderer, &win_w, &win_h);       //dimensions ecran

    //texte à afficher
    int pv_mecha_joueur = mecha_joueur->pv;
    int pv_mecha_ordi = mecha_ordi->pv;
    int niv_mecha_joueur = mecha_joueur->niveau;
    int niv_mecha_ordi = mecha_ordi->niveau;
    char nom_mecha_joueur[50];
    strcpy(nom_mecha_joueur, mecha[mecha_joueur->id_mechas - 1].nom);
    char nom_mecha_ordi[50];
    strcpy(nom_mecha_ordi, mecha[mecha_ordi->id_mechas - 1].nom);
    char texte_joueur[256];
    char texte_ordi[256];
    strcpy(texte_joueur, "Pv : ");
   //concat(texte_joueur, pv_mecha_joueur);
    /*strcat(texte_joueur, "\nNiv : ");
    concat(texte_joueur, niv_mecha_joueur);*/
    strcpy(texte_ordi, "Pv : ");
    //concat(texte_ordi, pv_mecha_ordi);
    /*strcat(texte_ordi, "\nNiv : ");
    concat(texte_ordi, niv_mecha_ordi);*/

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
    int border_pv = 3 * game.scale;


    creer_rectangle(&rect_joueur, w, h, x_joueur, y, 160, 160, 160, 255, NULL);
    creer_rectangle(&rect_ordi, w, h, x_ordi, y, 160, 160, 160, 255, NULL);
    creer_rectangle(&rect_bordure_joueur, w, bordure_size, x_joueur, y - bordure_size, 0, 0, 0, 0, nom_mecha_joueur);
    creer_rectangle(&rect_bordure_ordi, w, bordure_size, x_ordi, y - bordure_size, 0, 0, 0, 0, nom_mecha_ordi);
    creer_rectangle(&rect_pv_joueur, w_pv_joueur, h_pv, x_pv_joueur, y_pv, 0, 255, 0, 255, NULL);
    creer_rectangle(&rect_pv_ordi, w_pv_ordi, h_pv, x_pv_ordi, y_pv, 0, 255, 0, 255, NULL);
    creer_rectangle(&fond_pv_joueur, w_fond_pv, h_pv, x_pv_joueur, y_pv, 116, 117, 117, 255, NULL);
    creer_rectangle(&fond_pv_ordi, w_fond_pv, h_pv, x_pv_ordi, y_pv, 116, 117, 117, 255, NULL);
    creer_rectangle(&border_pv_joueur, w_fond_pv + 2 * border_pv, h_pv + 2 * border_pv, x_pv_joueur - border_pv, y_pv - border_pv, 0, 0, 0, 255, NULL);
    creer_rectangle(&border_pv_ordi, w_fond_pv + 2 * border_pv, h_pv + 2 * border_pv, x_pv_ordi - border_pv, y_pv - border_pv, 0, 0, 0, 255, NULL);
    draw_background();
    draw_all_rect(11, rect_bas,&rect_bordure_joueur,&rect_bordure_ordi ,&rect_joueur, &rect_ordi, &border_pv_joueur, &border_pv_ordi,&fond_pv_joueur, &fond_pv_ordi, &rect_pv_joueur, &rect_pv_ordi);
    draw_text_pos(texte_joueur, x_pv_joueur, y);
    draw_text_pos(texte_ordi, x_pv_ordi, y);
    
    

}

void animation_degat(int mecha_att, int pv_old, int pv_new, mechas_joueur_t *mecha_joueur, mechas_joueur_t *mecha_ordi) {
    int temp_pv = pv_old;  // Utiliser une variable temporaire pour éviter des bugs d'affichage
    while(temp_pv > pv_new) {
        SDL_RenderClear(game.renderer);
        if(!mecha_att) {
            mecha_joueur->pv = temp_pv;  // Temporairement réduire les PV affichés
        } else {
            mecha_ordi->pv = temp_pv;   // Temporairement réduire les PV affichés
        }
        draw_combat( mecha_joueur, mecha_ordi);
        SDL_RenderPresent(game.renderer);
        SDL_Delay(60);
        temp_pv--;
    }

    // Mise à jour finale des PV réels après l'animation
    if(!mecha_att) mecha_joueur->pv = pv_new;
    else mecha_ordi->pv = pv_new;
}


//Affiche les  mechas et permet d'en selectionner un
int aff_mechas_combat(joueur_t * joueur) {
    game.mat_active = 7;
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
    int rect_w = win_w * 0.4;
    int rect_h = win_h * 0.2;
    int margin = 5;  // Ajuste la marge selon tes besoins
    int margin_vertical = rect_h / 8;
    
    int x1 = margin;
    int y1 = margin;  // Remonté

    int x2 = win_w - rect_w - margin;
    int y2 = margin;  // Remonté

    int x3 = margin;
    int y3 = win_h - rect_h - margin - rect_h - margin_vertical;  // Remonté

    int x4 = win_w - rect_w - margin;
    int y4 = win_h - rect_h - margin - rect_h - margin_vertical;  // Remonté

    // Position et taille du 5e rectangle 
    int rect5_w = 2 * rect_w + margin;
    int rect5_h = rect_h;
    int x5 = (win_w - rect5_w) / 2;  // Centré
    int y5 = win_h - rect_h - margin;  // Juste en dessous des 4 rects

    int border_size = 5 * game.scale; // Épaisseur des bords

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
                        game.mat_active = 6;
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
        draw_all_rect(10,rect_bodure1, rect_bordure2, rect_bordure3, rect_bordure4, rect_bordure_retour, rect1, rect2, rect3, rect4, rect_retour);
    
        
        SDL_RenderPresent(game.renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAYS > frameTime) SDL_Delay(FRAME_DELAYS - frameTime);
    }
    game.mat_active = 6;
    return KO;
}

/*
===========================================
FONCTIONS UTILISATION OBJET
===========================================
*/

//PERMET AU JOUEUR D'UTILISER UN OBJET
int utilisation_objet(joueur_t *joueur, mechas_joueur_t *ordi, int *actif){
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
                            res = utilisation_mechaball(joueur, ordi, actif);
                            if(res == CAPTURE) return CAPTURE;
                            else if(res == FAUX) running = 0;
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

int utilisation_carburant(joueur_t *joueur, mechas_joueur_t *ordi, int *actif){
    if(joueur->inventaire->carburant <= 0){
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Vous n'avez pas de carburant.");
        return (KO);
    }
    else {
        int i = aff_mechas_combat(joueur); //recupere le numero du mechas selectionne
        if(i == -1 || i == 4){  //si on a rien choisi
            return (RETOUR);
        }
        else if(joueur->mechas_joueur[i].pv == joueur->mechas_joueur[i].pv_max){    //Si les PV sont au Max, on ne peut pas utiliser de carburant
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Les PV sont déjà au maximun !");
            return (RETOUR);
        }
        else if(joueur->mechas_joueur[i].pv < 0){  //Si les PV sont a 0, impossible d'utiliser du carburant
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Vous ne pouvez pas utiliser de carburant (PV à 0).");
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
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", message);
        }
    }

    return (OK);

}

int utilisation_rappel(joueur_t *joueur, mechas_joueur_t *ordi, int *actif){
    if(joueur->inventaire->rappel <= 0){
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Vous n'avez pas de rappel.");
        return (KO);
    }
    else {
        int i = aff_mechas_combat(joueur); //recupere le numero du mechas selectionne
        if(i == -1 || i == 4){ //si on a pas choisi de mecha
            return (RETOUR);
        }
        else if(joueur->mechas_joueur[i].pv != 0){  //Si les PV sont pas a 0, impossible d'utiliser du rappel
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Vous ne pouvez pas utiliser de rappel sur un mechas encore en vie.");
            return (RETOUR);
        }
        else{
            joueur->mechas_joueur[i].pv = joueur->mechas_joueur[i].pv_max/2;  //Remettre les PV au max
            char message[200] = "";
            strcat(message, mecha[joueur->mechas_joueur[i].id_mechas - 1].nom);
            strcat(message, "est de retour !");
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", message);
            joueur->inventaire->rappel--;
            
        }
    }

    return (OK);
}

int utilisation_mechaball(joueur_t * joueur, mechas_joueur_t *ordi, int *actif) {
    if(joueur->inventaire->mechaball <= 0){
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Vous n'avez pas de mechaball.");
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
            char message[200] = "Vous avez attrapé ";
            strcat(message,  mecha[ordi->id_mechas-1].nom);
            strcat(message, ".");
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", message);
        
            copie_mechas(joueur, ordi);
            return (CAPTURE);
        }
        else {
            char message[200] = "Vous n'avez pas reussi a attrape ";
            strcat(message,  mecha[ordi->id_mechas-1].nom);
            strcat(message, ".");
            afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", message);
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

int algo_attaque(int choix, mechas_joueur_t *mecha_att, mechas_joueur_t *mecha_def, int mecha_choix) {
    if(choix == 2) return 0;
    if(choix == 0 && mecha_att->utilisation_1 == 0) {
        afficher_dialogue_combat(  mecha_att, mecha_def, "Systeme", "Vous ne pouvez plus utiliser cette attaque.");
        return KO;
    }
    if(choix == 1 && mecha_att->utilisation_2 == 0) {
        afficher_dialogue_combat(  mecha_att, mecha_def, "Systeme", "Vous ne pouvez plus utiliser cette attaque.");
        return KO;
    }
    int ancien_pv = mecha_def->pv;
    float att_degat[2] = {attaque[mecha_att->attaque_1].degats, attaque[mecha_att->attaque_2].degats};
    float stat_att_mecha = mecha_att->attaque;
    char *mecha_att_type[2] = {attaque[mecha_att->attaque_1].type, attaque[mecha_att->attaque_2].type};
    char *mecha_def_type = mecha[mecha_def->id_mechas-1].type;
    char *nom_attaque[2] = {attaque[mecha_att->attaque_1].nom, attaque[mecha_att->attaque_2].nom};
    int precision[2] = {attaque[mecha_att->attaque_1].precision, attaque[mecha_att->attaque_2].precision};
    char att_type[4][20] = {"Carburant", "Electrique", "Renouvelable", "Carburant"};
    int nbr_rand = (rand() % 100) + 1;
    char msg[300];
    strcpy(msg, mecha[mecha_att->id_mechas - 1].nom);
    strcat(msg, " utilise ");
    strcat(msg, nom_attaque[choix]);
    strcat(msg, ".");

    if(mecha_choix) 
        afficher_dialogue_combat(  mecha_att, mecha_def, "Systeme", msg);
    else 
        afficher_dialogue_combat(  mecha_def, mecha_att, "Systeme", msg);

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
        afficher_dialogue_combat(  mecha_att, mecha_def, "Systeme", "L'attaque echoue.");
    }
    return OK;
}

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
    SDL_Color selec_bordure[4] = {{200, 187, 0, 255}, {30, 63, 200, 255}, {180, 100, 0, 255}, {40, 150, 0, 255}};
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

int attaque_ordi_sauvage(mechas_joueur_t *mecha_ordi, mechas_joueur_t *mecha_joueur){  //Retourne 0 si ne peut pas attaquer
    int numero_attaque;
    int utilisation[2] = {mecha_ordi->utilisation_1, mecha_ordi->utilisation_2};
    numero_attaque = rand() % 2;
    if(!utilisation[numero_attaque]){
        numero_attaque = 1 - numero_attaque;
        if(!utilisation[numero_attaque]) {
            afficher_dialogue_combat(  mecha_joueur, mecha_ordi, "Systeme", "Le mecha sauvage ne peut plus attaquer.");            
            return 0;
        }
        algo_attaque(numero_attaque, mecha_ordi, mecha_joueur, 0);
       
    }
    else{
        algo_attaque(numero_attaque, mecha_ordi, mecha_joueur, 0);
    }
    return OK;
}


/*int attaque_ordi_pnj(mechas_joueur_t *mecha_ordi, mechas_joueur_t *mecha_joueur){
    int ancien_pv;
    int nbr_rand_choix = 0;

    
    if(mecha_ordi->pv <= mecha_ordi->pv_max / 2){   //Si les PV sont inferieurs a la moitie des PV Max 1 chance sur 2 d'utiliser un carburant
        nbr_rand_choix = (rand() % 2) + 1;
    }

    if(nbr_rand_choix){    //Utilisation d'un carburant
        if(mecha_ordi->inventaire->carburant <= 0){
            nbr_rand_choix = 0;
        }
        else{
            mecha_ordi->mechas_joueur->pv += 20;
            if(mecha_ordi->mechas_joueur->pv > mecha_ordi->mechas_joueur->pv_max){
                mecha_ordi->mechas_joueur->pv = mecha_ordi->mechas_joueur->pv_max;
            }
            mecha_ordi->inventaire->carburant--;
        }
    }
    if(!nbr_rand_choix){   //Attaque
        int numero_attaque;
        int utilisation[2] = {mecha_ordi->utilisation_1, mecha_ordi->utilisation_2};
        numero_attaque = rand() % 2;
        if(!utilisation[numero_attaque]){
            numero_attaque = 1 - numero_attaque;
            if(!utilisation[numero_attaque]) {
                printf("Le mecha sauvage ne peut plus attaquer\n");
                return 0;
            }
            algo_attaque(numero_attaque, mecha_ordi, mecha_joueur);
       
        }
        else{
            algo_attaque(numero_attaque, mecha_ordi, mecha_joueur);
        }
            printf("PV : %d --> %d\n", ancien_pv, mecha_joueur->pv);
    }
    return OK;
}*/

/*
===========================================
*/

//premet de changer de meca dans le combat
int changer_mecha(joueur_t *joueur, int *actif, mechas_joueur_t *ordi){
    int choix ;
    choix = aff_mechas_combat(joueur);
    if (joueur->mechas_joueur[choix].pv == 0) {
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Ce mecha est mort."); 
        return KO;
    }
    if(choix == *actif) {
        afficher_dialogue_combat(  &(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Ce mecha est déjà actif.");
        return KO;
    }
    if(choix == 4) return RETOUR;
    *actif = choix;
    return OK;
}


int apprentissage_attaque(mechas_joueur_t *mecha_joueur){
    int i;
    int choix = -1;
    printf("FONCTION ATTAQUE :\n");
    for(i = 0;attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1 ].niveau != mecha_joueur->niveau && i<5 ;i++){
        printf("%d\n",i);
    }
    if(attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1 ].niveau == mecha_joueur->niveau){
        printf("Votre mecha souhaite apprendre une nouvelle attaque: %s\nDégats:%d\nPrecision:%d\n",
        attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].nom,attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].degats,
        attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i]-1].precision);
        printf("Choisissez l'attaque à remplacer ou appuyer sur 0 pour ne rien faire\n");
        printf("Attaque 1:%s\nDégats:%d\nPrecision:%d\n",
        attaque[mecha_joueur->attaque_1 -1].nom,attaque[mecha_joueur->attaque_1 -1].degats,attaque[mecha_joueur->attaque_1 -1].precision);
        printf("Attaque 2:%s\nDégats:%d\nPrecision:%d\nAttaque à remplacer :",
        attaque[mecha_joueur->attaque_2 -1].nom,attaque[mecha_joueur->attaque_2 -1].degats,attaque[mecha_joueur->attaque_2 -1].precision);
        do{
            scanf("%d",&choix);
            if(choix < 0 || choix > 2)
                printf("Veuillez choisir entre 0 et 2 : \n");
        } while(choix < 0 || choix > 2);
        switch(choix){
            case 1:
                printf("Vous avez remplacé votre attaque %s par %s\n",
                attaque[mecha_joueur->attaque_1-1].nom,attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i]-1].nom);
                mecha_joueur->attaque_1 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].id_attaques;
                mecha_joueur->utilisation_1 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].utilisations;
                break;
            case 2:
                printf("Vous avez remplacé votre attaque %s par %s\n",
                attaque[mecha_joueur->attaque_1 -1].nom,attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].nom);
                mecha_joueur->attaque_2 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].id_attaques;
                mecha_joueur->utilisation_2 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].utilisations;
                break;
            default:
                printf("Vous gardez votre attaque actuelle\n");
        }
           
    }
    printf("Attaque 1 : %d\nUtilisations : %d\n",mecha_joueur->attaque_1,mecha_joueur->utilisation_1);
    printf("Attaque 2 : %d\nUtilisations : %d\n",mecha_joueur->attaque_2,mecha_joueur->utilisation_2);
    return OK;
}

int evolution_mechas(mechas_joueur_t *mecha_joueur){
    if(mecha_joueur->niveau >= mecha[mecha_joueur->id_mechas -1].niveau_evolution && mecha[mecha_joueur->id_mechas -1].evolution > 0){
        mecha_joueur->id_mechas++;
        printf("%s à évolué en %s\n",mecha[mecha_joueur->id_mechas -2].nom,mecha[mecha_joueur->id_mechas -1].nom );
    }
    return OK;
}

void montee_niveau(mechas_joueur_t *mecha, int xp_partage, int lvlup){
    int nouv_level;

    nouv_level = mecha->xp + xp_partage;
    if(nouv_level >= lvlup){
        while(nouv_level >= lvlup){
            mecha->niveau++;
            nouv_level -= lvlup;
            mecha->xp = 0;
            mecha->xp += nouv_level;
            lvlup = (int)(15 * exp(0.05 * mecha->niveau));   //Calculer le nouveau nombre d'XP necessaire
            evolution_mechas(mecha);
            apprentissage_attaque(mecha);
        }
    }
    else{
        mecha->xp += xp_partage;
    }
}


void distribuer_xp(joueur_t *mechas_presents, int xp_gagne) {

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
        montee_niveau(&mechas_presents->mechas_joueur[i], xp_partage, lvlup);
    }
}

void level_mechas(joueur_t *mechas_presents, joueur_t *mecha_tue){
    
    int i, nb;
    int xp_gagne;

   
    xp_gagne = (int)(20 * exp(0.02 * mecha_tue->mechas_joueur->niveau));   //Calculer l'XP recu en tuant l'adversaire

    distribuer_xp(mechas_presents, xp_gagne);
    
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



int tour_joueur(joueur_t *joueur, mechas_joueur_t *mecha_sauvage, int * actif) {
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    int running = 1;
    int win_w, win_h;
    int res;
    SDL_GetRendererOutputSize(game.renderer, &win_w, &win_h);       //dimensions ecran
    
    //couleurs des bordures
    SDL_Color couleur_bordure =  {94, 99, 102, 250};
    SDL_Color couleur_bordure_attaque = {210, 145, 132, 150};
    SDL_Color couleur_bordure_objet = {229, 185, 135, 150};
    SDL_Color couleur_bordure_mecha = {162, 202, 154, 150};
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
                                res = utilisation_objet( joueur, mecha_sauvage, actif);
                                if(res == CAPTURE) 
                                    return CAPTURE;
                                else if(res!= RETOUR){
                                    running = 0;
                                }
                                break;
                        case 1 :
                            if(attaque_joueur( joueur, mecha_sauvage, actif) == OK)
                                running = 0;
                            break;
                        case 2 : 
                            res = changer_mecha(joueur, actif, mecha_sauvage);
                            if(res == OK){
                                return OK;
                            }
                    }
                }
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




void combat_pnj(joueur_t *joueur, pnj_t *pnj) {

}

void combat_sauvage(joueur_t *joueur, mechas_joueur_t *mecha_sauvage) {
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
                res = tour_joueur(joueur, mecha_sauvage,  &actif);
                
                if(mecha_sauvage->pv != 0 && res == OK) 
                    attaque_ordi_sauvage(mecha_sauvage, &(joueur->mechas_joueur[actif]));
            }
            else {
                attaque_ordi_sauvage(mecha_sauvage, &(joueur->mechas_joueur[actif]));
                if(joueur->mechas_joueur[actif].pv != 0) {
                    res = tour_joueur(joueur, mecha_sauvage,  &actif);
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
    if(mecha_sauvage->pv != 0 && joueur->mechas_joueur[actif].pv == 0) afficher_dialogue_combat(  &(joueur->mechas_joueur[actif]), mecha_sauvage, "Systeme", "Vous avez perdu !");
    else if(mecha_sauvage->pv == 0 && joueur->mechas_joueur[actif].pv != 0)afficher_dialogue_combat(  &(joueur->mechas_joueur[actif]), mecha_sauvage, "Systeme", "Vous avez gagne !");
    else if(res == FUITE)afficher_dialogue_combat(  &(joueur->mechas_joueur[actif]), mecha_sauvage, "Systeme", "Vous prennez la fuite!");
    
    game.mat_active = save_map_active;
}