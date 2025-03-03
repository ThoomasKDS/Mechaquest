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

static void concat(char *dest, int nb) { //concatene un entier a une chaine de caractere
    char tmp[10];
    sprintf(tmp, "%d", nb);
    strcat(dest, tmp);
}



//Affiche les  mechas et permet d'en selectionner un
int aff_mechas_combat(game_t * game, joueur_t * joueur) {
    game->mat_active = 7;
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    int running = 1;
    int win_w, win_h;
    SDL_GetRendererOutputSize(game->renderer, &win_w, &win_h);       //dimensions ecran
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

    int border_size = 5 * game->scale; // Épaisseur des bords

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
                        game->mat_active = 6;
                        return choix;
                    }
                    else {
                        printf("Veuillez selectionner un mechas\n");
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
        SDL_RenderClear(game->renderer);
        draw_background(game);
        draw_all_rect(game, 10,rect_bodure1, rect_bordure2, rect_bordure3, rect_bordure4, rect_bordure_retour, rect1, rect2, rect3, rect4, rect_retour);
    
        
        SDL_RenderPresent(game->renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAYS > frameTime) SDL_Delay(FRAME_DELAYS - frameTime);
    }
    game->mat_active = 6;
    return -1;
}

/*
===========================================
FONCTIONS UTILISATION OBJET
===========================================
*/

//PERMET AU JOUEUR D'UTILISER UN OBJET
int utilisation_objet(game_t *game, joueur_t *joueur, mechas_joueur_t *ordi){
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    int running = 1;
    int win_w, win_h;
    SDL_GetRendererOutputSize(game->renderer, &win_w, &win_h);       //dimensions ecran

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

    // Définition de rect_bas (fond des boutons)
    int rect_bas_w = win_w;
    int rect_bas_h = win_h / 4;
    int rect_bas_x = 0; // Centrer horizontalement
    int rect_bas_y = win_h - rect_bas_h;

    // Définition des tailles des boutons
    int btn_w = rect_bas_w / 6; // Largeur des boutons (1/6 de l'écran)
    int btn_h = rect_bas_h / 2; // Hauteur des boutons (1/2 du rect_bas)
    int border_size = 5 * game->scale; // Épaisseur des bords

    // Calcul de l'espacement entre les boutons
    int espacement = ((rect_bas_w - (4 * btn_w)) / 3) * 0.7;

    // Positionnement des boutons 
    int btn_center_y = rect_bas_y + (rect_bas_h - btn_h) / 2;
    int total_width = (4 * btn_w) + (3 * espacement);  // Largeur totale de la ligne
    int btn_carbu_x = (rect_bas_w - total_width) / 2; // Centre le premier bouton
    int btn_rappel_x = btn_carbu_x + btn_w + espacement;
    int btn_mechaball_x = btn_rappel_x + btn_w + espacement;
    int btn_retour_x = btn_mechaball_x + btn_w + espacement;

    rectangle_t rect_bas, btn_carbu, btn_rappel, btn_mechaball, btn_retour;
    rectangle_t bordure_carbu, bordure_rappel, bordure_mechaball, bordure_retour;

    //creation des rectangles
    creer_rectangle(&rect_bas,rect_bas_w, rect_bas_h, rect_bas_x, rect_bas_y,  0, 0, 0, 150, NULL);
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
                        case 0: if(utilisation_carburant(game, joueur)) running = 0; break;
                        case 1: if(utilisation_rappel(game, joueur)) running = 0; break;
                        case 2: if(utilisation_mechaball(game, joueur, ordi)) return 0; break;
                        case 3: running = 0; return 0;
                    }
                }
                SDL_Event e;
                while (SDL_PollEvent(&e)){
                    if (e.type == SDL_KEYUP) break;
                }
            }
            
        }

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

        SDL_RenderClear(game->renderer);
        draw_background(game);

        draw_all_rect(game, 9,&rect_bas, &bordure_carbu, &bordure_rappel, &bordure_mechaball, &bordure_retour, &btn_carbu, &btn_rappel, &btn_mechaball, &btn_retour);

        SDL_RenderPresent(game->renderer);


        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAYS > frameTime) SDL_Delay(FRAME_DELAYS - frameTime);

    }

    return 1;
}

int utilisation_carburant(game_t *game, joueur_t *joueur){
    if(joueur->inventaire->carburant <= 0){
        printf("Vous n'avez pas de carburant\n");
        return 0;
    }
    else {
        int i = aff_mechas_combat(game, joueur); //recupere le numero du mechas selectionne
        if(i == -1 || i == 4){
            return 0;
        }
        else if(joueur->mechas_joueur[i].pv == joueur->mechas_joueur[i].pv_max){    //Si les PV sont au Max, on ne peut pas utiliser de carburant
            printf("Les PV sont déjà au maximun !\n");
            return 0;
        }
        else if(joueur->mechas_joueur[i].pv < 0){  //Si les PV sont a 0, impossible d'utiliser du carburant
            printf("Vous ne pouvez pas utiliser de carburant (PV à 0)\n");
            return 0;
        }
        else{
            printf("Ancien PV : %d\n", joueur->mechas_joueur[i].pv);
            joueur->mechas_joueur[i].pv += 20;  //Ajouter 20 aux PV actuels
            if(joueur->mechas_joueur[i].pv > joueur->mechas_joueur[i].pv_max){
                joueur->mechas_joueur[i].pv = joueur->mechas_joueur[i].pv_max;
            }
            joueur->inventaire->carburant--;
            printf("Nouveau PV : %d\n", joueur->mechas_joueur[i].pv);
        }
    }

    return 1;

}

int utilisation_rappel(game_t *game, joueur_t *joueur){
    if(joueur->inventaire->rappel <= 0){
        printf("Vous n'avez pas de rappel\n");
        return 0;
    }
    else {
        int i = aff_mechas_combat(game, joueur); //recupere le numero du mechas selectionne
        if(i == -1 || i == 5){
            return 0;
        }
        else if(joueur->mechas_joueur[i].pv != 0){  //Si les PV sont pas a 0, impossible d'utiliser du rappel
            printf("Vous ne pouvez pas utiliser de rappel sur un mechas encore en vie\n");
            return 0;
        }
        else{
            joueur->mechas_joueur[i].pv = joueur->mechas_joueur[i].pv_max/2;  //Remettre les PV au max
            printf("%s est de retour, PV : %d\n",mecha[joueur->mechas_joueur[i].id_mechas - 1].nom, joueur->mechas_joueur[i].pv);
            joueur->inventaire->rappel--;
            
        }
    }

    return 1;
}

int utilisation_mechaball(game_t *game,joueur_t * joueur, mechas_joueur_t *ordi) {
    if(joueur->inventaire->mechaball <= 0){
        printf("Vous n'avez pas de mechaball\n");
        return 0;
    }
    else{
        joueur->inventaire->mechaball--;
        float taux_capture;   //Un taux de capture different en fonction du type de mecha (nucleaire + difficile a attraper)
        if((strcmp(mecha[ordi->id_mechas-1].type, "Nucleaire") == 0)){
            taux_capture = 0.3;
        }
        else{
            taux_capture = 0.8;
        }
        int proba_capture = (1 - ordi->pv / ordi->pv_max) * taux_capture;   //Calcul les chances d'attraper le mecha

        int nbr_rand_capture = (rand() % 100) + 1;

        if(proba_capture <= nbr_rand_capture){  //Si le mecha est attrapé
            printf("Vous avez attrapé %s\n", mecha[ordi->id_mechas-1].nom);
        
            copie_mechas(joueur, ordi);
            return 1;
        }
        else {
            printf("Vous n'avez pas réussi à attraper %s\n", mecha[ordi->id_mechas-1].nom);
        }
    }
    return -1;
}



/*
===========================================
*/

/*
===========================================
FONCTIONS ATTAQUE
===========================================
*/

int algo_attaque(int choix, mechas_joueur_t *mecha_att, mechas_joueur_t *mecha_def) {
    if(choix == 2) return 0;
    int ancien_pv = mecha_def->pv;
    float att_degat[2] = {attaque[mecha_att->attaque_1].degats, attaque[mecha_att->attaque_2].degats};
    float stat_att_mecha = mecha_att->attaque;
    char *mecha_att_type[2] = {attaque[mecha_att->attaque_1].type, attaque[mecha_att->attaque_2].type};
    char *mecha_def_type = mecha[mecha_def->id_mechas-1].type;
    char att_type[4][20] = {"Carburant", "Electrique", "Renouvelable", "Carburant"};
    int nbr_rand = (rand() % 100) + 1;
    printf("%f\n", att_degat[choix]);
    printf("%f\n", stat_att_mecha);
    printf("%d\n", mecha_def->defense);
    printf("%s\n%s\n", mecha_att_type[choix], mecha_def_type);

    if(nbr_rand <= attaque[mecha_att->attaque_1].precision){  //Test si l'attaque touche (precision)
        
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
        printf("%.2f\n%d\n", stat_att_mecha + att_degat[choix], mecha_def->defense);
        if(stat_att_mecha + att_degat[choix] > mecha_def->defense){
            mecha_def->pv -= (stat_att_mecha + att_degat[choix] - mecha_def->defense);
            if (mecha_def->pv < 0) mecha_def->pv = 0;
        }
        if(!choix) mecha_att->utilisation_1--;
        else mecha_att->utilisation_2--;
        
    }
    else {
        printf("L'attaque a échoué\n");
    }
    printf("PV : %d --> %d\n", ancien_pv, mecha_def->pv);
    return 1;
}

int attaque_joueur(game_t *game, joueur_t *j, mechas_joueur_t *mecha_ordi, int * actif){
   
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    int running = 1;
    int win_w, win_h;
    int attaque_type[2];
    SDL_GetRendererOutputSize(game->renderer, &win_w, &win_h);       //dimensions ecran
    char tmp[20];
    //texte à afficher
    char texte_retour[7] = "Retour";
    char texte_attaque[2][400] = {"Attaque 1 : ", "Attaque 2 : "};
    int attaque_num[2] = {j->mechas_joueur[*actif].attaque_1-1, j->mechas_joueur[*actif].attaque_2-1};
    int utilisation_num[2] = {j->mechas_joueur[*actif].utilisation_1-1, j->mechas_joueur[*actif].utilisation_2-1};
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


    rectangle_t btn_attaque1, btn_attaque2, btn_retour, rect_bas;
    rectangle_t border_attaque1, border_attaque2, border_retour;

    int border_size = 5 * game->scale; // Épaisseur des bords

    // Définition de rect_bas (fond des boutons)
    int rect_bas_w = win_w;
    int rect_bas_h = win_h / 4;
    int rect_bas_x = 0; // Centrer horizontalement
    int rect_bas_y = win_h - rect_bas_h;

        // Définition des tailles des boutons
    int btn_w = rect_bas_w / 5; // Chaque bouton fait environ 1/5 de la largeur de rect_bas
    int btn_h = rect_bas_h / 2; // Hauteur du bouton = moitié de rect_bas
    int btn_spacing = rect_bas_w / 20; // Espacement entre les boutons (ajustable)

    // Positionnement des boutons
    int btn_center_x = (rect_bas_w - btn_w) / 2;  // Centré horizontalement dans rect_bas
    int btn_center_y = rect_bas_y + (rect_bas_h - btn_h) / 2; // Centré verticalement

    int btn_left_x = btn_center_x - btn_w - btn_spacing;  // À gauche du centre
    int btn_right_x = btn_center_x + btn_w + btn_spacing; // À droite du centre

    //creer les boutons
    creer_rectangle(&rect_bas, rect_bas_w, rect_bas_h, rect_bas_x, rect_bas_y, 0, 0, 0, 150, NULL);
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
                    return algo_attaque(choix, &j->mechas_joueur[*actif], mecha_ordi);
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
        SDL_RenderClear(game->renderer);
        draw_background(game);
        draw_all_rect(game, 7, &rect_bas, &border_attaque1, &border_attaque2, &border_retour, &btn_attaque1, &btn_attaque2, &btn_retour);
        
        SDL_RenderPresent(game->renderer);


        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAYS > frameTime) {
            SDL_Delay(FRAME_DELAYS - frameTime);
        }
    }
    return OK;
}

/*
===========================================
*/

//premet de changer de meca dans le combat
int changer_mecha(game_t *game, joueur_t *joueur, int *actif){
    int choix = aff_mechas_combat(game, joueur);
    if(choix == *actif) {
        printf("Ce mecha est déjà actif\n");
        return 0;
    }
    if(choix == 4) return 0;
    *actif = choix;
    return 1;
}

int attaque_ordi_sauvage(mechas_joueur_t *mecha_ordi, mechas_joueur_t *mecha_joueur){  //Retourne 0 si ne peut pas attaquer

    int ok = -1;
    int cpt = 0;
    int att1, att2;
    int ancien_pv;
    int nbr_rand_att, nbr_rand_preci;

    
    ancien_pv = mecha_joueur->pv;
    att1 = attaque[mecha_ordi->attaque_1].degats;
    att2 = attaque[mecha_ordi->attaque_2].degats;

    srand(time(NULL));
    nbr_rand_att = (rand() % 2) + 1;

    while(ok != 0){ //Test si les attaques ont encore des utilisations
        if((nbr_rand_att == 1) && (attaque[mecha_ordi->attaque_1].utilisations > 0)){
            ok = 0;
        }
        else{
            nbr_rand_att = 2;   //Si l'utilisation est a 0, on test directement l'autre attaque
        }
        if((nbr_rand_att == 2) && (attaque[mecha_ordi->attaque_2].utilisations > 0)){
            ok = 0;
        }
        else{
            nbr_rand_att = 1;   //Si l'utilisation est a 0, on test directement l'autre attaque
        }
        cpt++;
        if(cpt > 2){    //Si aucune attaque n'a d'utilisation
            return 0;
        }
    }

    nbr_rand_preci = (rand() % 100) + 1;

    if(nbr_rand_att == 1){
        if(nbr_rand_preci <= attaque[mecha_joueur->attaque_1].precision){  //Test si l'attaque touche (precision)

            // Test en fonction des types (+ de degats ou non)
            if(((strcmp(attaque[mecha_ordi->attaque_1].type, "Carburant") == 0) && (strcmp(mecha[mecha_joueur->id_mechas-1].type, "Electrique") == 0))
            || ((strcmp(attaque[mecha_ordi->attaque_1].type, "Electrique") == 0) && (strcmp(mecha[mecha_joueur->id_mechas-1].type, "Renouvelable") == 0))
            || ((strcmp(attaque[mecha_ordi->attaque_1].type, "Renouvelable") == 0) && (strcmp(mecha[mecha_joueur->id_mechas-1].type, "Carburant") == 0))){
                att1 *= 1.5;
            }
            else if((strcmp(attaque[mecha_ordi->attaque_1].type, "Nucleaire" ) == 0) && strcmp(mecha[mecha_joueur->id_mechas-1].type, "Nucleaire")){
                att1 *= 1.2;
            }

            if(mecha_ordi->attaque + att1 > mecha_joueur->defense){
                mecha_joueur->pv -= (mecha_ordi->attaque + att1 - mecha_joueur->defense);
            }

            attaque[mecha_ordi->attaque_1].utilisations--;    //Decrementer le nombre d'utilisations restantes
        }
    }
    else{
        if(nbr_rand_preci <= attaque[mecha_joueur->attaque_2].precision){ //Test si l'attaque touche (precision)
            // Test en fonction des types (+ de degats ou non)
            if(((strcmp(attaque[mecha_ordi->attaque_2].type, "Carburant") == 0) && (strcmp(mecha[mecha_joueur->id_mechas-1].type, "Electrique") == 0))
            || ((strcmp(attaque[mecha_ordi->attaque_2].type, "Electrique") == 0) && (strcmp(mecha[mecha_joueur->id_mechas-1].type, "Renouvelable") == 0))
            || ((strcmp(attaque[mecha_ordi->attaque_2].type, "Renouvelable") == 0) && (strcmp(mecha[mecha_joueur->id_mechas-1].type, "Carburant") == 0))){
                att2 *= 1.5;
            }
            else if((strcmp(attaque[mecha_ordi->attaque_2].type, "Nucleaire" ) == 0) && strcmp(mecha[mecha_joueur->id_mechas-1].type, "Nucleaire")){
                att2 *= 1.2;
            }

            if(mecha_ordi->attaque + att2 > mecha_joueur->defense){
                mecha_joueur->pv -= (mecha_ordi->attaque + att2 - mecha_joueur->defense);
            }

            attaque[mecha_ordi->attaque_1].utilisations--;    //Decrementer le nombre d'utilisations restantes
        }
    }
    printf("PV : %d --> %d\n", ancien_pv, mecha_joueur->pv);
    return OK;
}


int attaque_ordi_pnj(pnj_t *mecha_ordi, mechas_joueur_t *mecha_joueur){

    int ok = -1;
    int cpt = 0;
    int att1, att2;
    int ancien_pv;
    int nbr_rand_att, nbr_rand_preci;
    int nbr_rand_choix = 0;

    srand(time(NULL));

    if(mecha_ordi->mechas_joueur->pv <= mecha_ordi->mechas_joueur->pv_max / 2){   //Si les PV sont inferieurs a la moitie des PV Max
        nbr_rand_choix = (rand() % 2) + 1;
    }

    if(nbr_rand_choix == 1){    //Utilisation d'un carburant
        if(mecha_ordi->inventaire->carburant <= 0){
            nbr_rand_choix = 2;
        }
        else if(mecha_ordi->mechas_joueur->pv + 20 > mecha_ordi->mechas_joueur->pv_max){ //Si les PV + 20 sont sup aux PV Max, les PV deviennent les PV Max
            mecha_ordi->mechas_joueur->pv = mecha_ordi->mechas_joueur->pv_max;
            mecha_ordi->inventaire->carburant--;
        }
        else{
            mecha_ordi->mechas_joueur->pv += 20;
            mecha_ordi->inventaire->carburant--;
        }
    }
    else{   //Attaque
        ancien_pv = mecha_joueur->pv;
        att1 = attaque[mecha_ordi->mechas_joueur->attaque_1].degats;
        att2 = attaque[mecha_ordi->mechas_joueur->attaque_2].degats;

        nbr_rand_att = (rand() % 2) + 1;

        while(ok != 0){ //Test si les attaques ont encore des utilisations
            if((nbr_rand_att == 1) && (attaque[mecha_ordi->mechas_joueur->attaque_1].utilisations > 0)){
                ok = 0;
            }
            else{
                nbr_rand_att = 2;   //Si l'utilisation est a 0, on test directement l'autre attaque
            }
            if((nbr_rand_att == 2) && (attaque[mecha_ordi->mechas_joueur->attaque_2].utilisations > 0)){
                ok = 0;
            }
            else{
                nbr_rand_att = 1;   //Si l'utilisation est a 0, on test directement l'autre attaque
            }
            cpt++;
            if(cpt > 2){    //Si aucune attaque n'a d'utilisation
                return 0;
            }
        }

        nbr_rand_preci = (rand() % 100) + 1;

        if(nbr_rand_att == 1){
            if(nbr_rand_preci <= attaque[mecha_joueur->attaque_1].precision){  //Test si l'attaque touche (precision)

                // Test en fonction des types (+ de degats ou non)
                if(((strcmp(attaque[mecha_ordi->mechas_joueur->attaque_1].type, "Carburant") == 0) && (strcmp(mecha[mecha_joueur->id_mechas-1].type, "Electrique") == 0))
                || ((strcmp(attaque[mecha_ordi->mechas_joueur->attaque_1].type, "Electrique") == 0) && (strcmp(mecha[mecha_joueur->id_mechas-1].type, "Renouvelable") == 0))
                || ((strcmp(attaque[mecha_ordi->mechas_joueur->attaque_1].type, "Renouvelable") == 0) && (strcmp(mecha[mecha_joueur->id_mechas-1].type, "Carburant") == 0))){
                    att1 *= 1.5;
                }
                else if((strcmp(attaque[mecha_ordi->mechas_joueur->attaque_1].type, "Nucleaire" ) == 0) && strcmp(mecha[mecha_joueur->id_mechas-1].type, "Nucleaire")){
                    att1 *= 1.2;
                }

                if(mecha_ordi->mechas_joueur->attaque + att1 > mecha_joueur->defense){
                    mecha_joueur->pv -= (mecha_ordi->mechas_joueur->attaque + att1 - mecha_joueur->defense);
                }

                attaque[mecha_ordi->mechas_joueur->attaque_1].utilisations--;    //Decrementer le nombre d'utilisations restantes
            }
        }
        else{
            if(nbr_rand_preci <= attaque[mecha_joueur->attaque_2].precision){ //Test si l'attaque touche (precision)
                // Test en fonction des types (+ de degats ou non)
                if(((strcmp(attaque[mecha_ordi->mechas_joueur->attaque_2].type, "Carburant") == 0) && (strcmp(mecha[mecha_joueur->id_mechas-1].type, "Electrique") == 0))
                || ((strcmp(attaque[mecha_ordi->mechas_joueur->attaque_2].type, "Electrique") == 0) && (strcmp(mecha[mecha_joueur->id_mechas-1].type, "Renouvelable") == 0))
                || ((strcmp(attaque[mecha_ordi->mechas_joueur->attaque_2].type, "Renouvelable") == 0) && (strcmp(mecha[mecha_joueur->id_mechas-1].type, "Carburant") == 0))){
                    att2 *= 1.5;
                }
                else if((strcmp(attaque[mecha_ordi->mechas_joueur->attaque_2].type, "Nucleaire" ) == 0) && strcmp(mecha[mecha_joueur->id_mechas-1].type, "Nucleaire")){
                    att2 *= 1.2;
                }

                if(mecha_ordi->mechas_joueur->attaque + att2 > mecha_joueur->defense){
                    mecha_joueur->pv -= (mecha_ordi->mechas_joueur->attaque + att2 - mecha_joueur->defense);
                }

                attaque[mecha_ordi->mechas_joueur->attaque_2].utilisations--;    //Decrementer le nombre d'utilisations restantes
            }
        }
        printf("PV : %d --> %d\n", ancien_pv, mecha_joueur->pv);
    }
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




int tour_joueur(joueur_t *joueur, mechas_joueur_t *mecha_sauvage, game_t *game, int * actif) {
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    int running = 1;
    int win_w, win_h;
    SDL_GetRendererOutputSize(game->renderer, &win_w, &win_h);       //dimensions ecran
    
    //couleurs des bordures
    SDL_Color couleur_bordure =  {94, 99, 102, 250};
    SDL_Color couleur_bordure_attaque = {210, 145, 132, 150};
    SDL_Color couleur_bordure_objet = {229, 185, 135, 150};
    SDL_Color couleur_bordure_mecha = {162, 202, 154, 150};
    int choix = 1;


    // Définition de rect_bas (fond des boutons)
    int rect_bas_w = win_w;
    int rect_bas_h = win_h / 4;
    int rect_bas_x = 0; // Centrer horizontalement
    int rect_bas_y = win_h - rect_bas_h;
    
    // Définition des tailles des boutons
    int btn_w = rect_bas_w / 5; // Largeur des boutons (1/5 de l'écran)
    int btn_h = rect_bas_h / 2; // Hauteur des boutons (1/2 du rect_bas)
    int btn_margin = btn_w / 2; // Marge entre les boutons
    int border_size = 5 * game->scale; // Épaisseur des bords
    
    // Positionnement des boutons
    int btn_center_x = (rect_bas_w - btn_w) / 2;
    int btn_center_y = rect_bas_y + (rect_bas_h - btn_h) / 2;
    int btn_left_x = btn_center_x - btn_w - btn_margin;
    int btn_right_x = btn_center_x + btn_w + btn_margin;
    int btn_fuite_y = btn_center_y + btn_h + btn_margin;

    
    rectangle_t rect_bas, rect_attaque, rect_objet, rect_changer_mecha, rect_fuite;
    rectangle_t border_attaque, border_objet, border_mecha, border_fuite;
    
    // Création des rectangles
    creer_rectangle(&rect_bas, rect_bas_w, rect_bas_h, rect_bas_x, rect_bas_y, 0, 0, 0, 150, NULL);
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
                return 0;
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
                        case 0 : if(!utilisation_objet(game, joueur, mecha_sauvage)) return 0;
                                else running  = 0; break;
                        case 1 : attaque_joueur(game, joueur, mecha_sauvage, actif);running = 0; break;
                        case 2 : changer_mecha(game, joueur, actif);running = 0; break;
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
        
        SDL_RenderClear(game->renderer);
        draw_background(game);
        draw_all_rect(game, 7, &rect_bas, &border_attaque, &border_objet, &border_mecha, &rect_attaque, &rect_objet, &rect_changer_mecha, &rect_fuite);
        //draw_rect(game, &border_fuite);
        //draw_rect(game, &rect_fuite);
        SDL_RenderPresent(game->renderer);
        
        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAYS > frameTime) {
            SDL_Delay(FRAME_DELAYS - frameTime);
        }
    }
    return 1;
}

void combat_sauvage(joueur_t *joueur, mechas_joueur_t *mecha_sauvage, game_t *game) {
    int save_map_active = game->mat_active;
    game->mat_active = 6;
    int actif = 0;
    while(mecha_sauvage->pv > 0 && joueur->mechas_joueur[actif].pv > 0) {
        if(!tour_joueur(joueur, mecha_sauvage, game, &actif)) break;
        
    }
    
    
    
    game->mat_active = save_map_active;
}

