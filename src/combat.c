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
joueur_t joueur;

void affichage_mecha(mechas_joueur_t tab_mecha[], mechas_t mecha[], char nom[]){

    //char type[20];
    recuperation_mechas(mecha);
    
    recuperation_joueur(&joueur, nom);
    int i, j;
    for(i = 0; i < N; i++){

        for(j = 0; j < N; j++){
            if(tab_mecha[i].id_mechas == mecha[j].id_mechas){
                strcpy(nom, mecha[j].nom);
            }
        }

        printf("Mecha %d : \n", i+1);
        //printf("Nom : %s\n", mecha[joueur.mechas_joueur[i].id_mechas-1].nom); //
        //printf("Nom : %s\n", nom);
        printf("Niveau : %d\n", joueur.mechas_joueur[i].niveau);
        printf("PV : %d\n", joueur.mechas_joueur[i].pv);
        printf("PV Max: %d\n", joueur.mechas_joueur[i].pv_max);
        printf("Vitesse : %d\n", joueur.mechas_joueur[i].vitesse);
        printf("Attaque 1 : %d\n", joueur.mechas_joueur[i].attaque_1);
        printf("Attaque 2 : %d\n", joueur.mechas_joueur[i].attaque_2);
        printf("Attaque générale : %d\n", joueur.mechas_joueur[i].attaque);
        printf("Défense : %d\n", joueur.mechas_joueur[i].defense);
        printf("\n");
    }
}

int utilisation_objet(joueur_t *joueur, int i){
    int choix = 0;
    while(choix < 1 || choix > 3){
        printf("Que voulez-vous utiliser ?\n");
        printf("1 : Carburant (reste %d)\n", joueur->inventaire->carburant);
        printf("2 : Rappel (reste %d)\n", joueur->inventaire->rappel);
        printf("3 : Retour\n");
        scanf("%d", &choix);
        if(choix == 1){
            if(joueur->inventaire->carburant == 0){
                printf("Vous n'avez pas de carburant\n");
                choix = 0;
            }
            else if(joueur->mechas_joueur[i].pv == joueur->mechas_joueur[i].pv_max){    //Si les PV sont au Max, on ne peut pas utiliser de carburant
                printf("Les PV sont déjà au maximun !\n");
                choix = 0;
            }
            else if(joueur->mechas_joueur[i].pv + 20 > joueur->mechas_joueur[i].pv_max){ //Si les PV + 20 sont sup aux PV Max, les PV deviennent les PV Max
                printf("Ancien PV : %d\n", joueur->mechas_joueur[i].pv);
                joueur->mechas_joueur[i].pv = joueur->mechas_joueur[i].pv_max;
                joueur->inventaire->carburant--;
                printf("Nouveau PV : %d\n", joueur->mechas_joueur[i].pv);
            }
            else if(joueur->mechas_joueur[i].pv < 0){  //Si les PV sont a 0, impossible d'utiliser du carburant
                printf("Vous ne pouvez pas utiliser de carburant (PV à 0)\n");
                choix = 0;
            }
            else{
                printf("Ancien PV : %d\n", joueur->mechas_joueur[i].pv);
                joueur->mechas_joueur[i].pv += 20;  //Ajouter 20 aux PV actuels
                joueur->inventaire->carburant--;
                printf("Nouveau PV : %d\n", joueur->mechas_joueur[i].pv);
            }
        }
        else if(choix == 2){
            if(joueur->inventaire->rappel == 0){
                printf("Vous n'avez pas de rappel\n");
                choix = 0;
            }
            else if(joueur->mechas_joueur[i].pv < 0){
                joueur->inventaire->rappel--;
                printf("Ancien PV : %d\n", joueur->mechas_joueur[i].pv);
                joueur->mechas_joueur[i].pv = joueur->mechas_joueur[i].pv_max / 2;  //Remplacer les PV actuels par les PV Max
                printf("Nouveau PV : %d\n", joueur->mechas_joueur[i].pv);
            }
            else{   //Si les PV ne sont pas à 0, impossible d'utiliser un rappel
                printf("Vous ne pouvez pas utiliser de rappel (PV > 0)\n");
                choix = 0;
            }
        }
        else if(choix == 3){
            return 3;
        }
        else{
            printf("Choix indisponible\n");
        }
    }
    return 0;

}


int changer_mecha(joueur_t *joueur, mechas_t mecha[], char nom[], int etat){
    
    int choix = -1;
    int choix2 = -1;
    int i, max, j;
    mechas_joueur_t mecha_temp;

    if(etat){   //Etat : changer de Mecha dans un combat (entre 1 et 3 mechas)
        i = 1;
        max = 4;
        printf("\nMecha actuel : %s\n\n", mecha[joueur->mechas_joueur[0].id_mechas-1].nom);
    }
    else{   //Non Etat : changer de Mecha n'importe quand (choix entre tout les mechas)
        i = 0;
        max = joueur->nb_mechas;
    }

    for(j = i; j < max; j++){
        printf("Mecha %d : %s\n", j, mecha[joueur->mechas_joueur[j].id_mechas-1].nom);
    }

    while(choix < i || choix > max){
        printf("\nQuel Mecha échanger ?\n");
        printf("Choix : ");
        scanf("%d", &choix);
        if(choix < i || choix > max){
            printf("Le choix n'est pas correct\n");
        }
    }

    if(etat){
        mecha_temp = joueur->mechas_joueur[0];
        joueur->mechas_joueur[0] = joueur->mechas_joueur[choix];
        joueur->mechas_joueur[choix] = mecha_temp;
    }
    else{
        do{
            printf("Choix 2 : ");
            scanf("%d", &choix2);
            if(choix2 < i || choix2 > max){
                printf("Le choix n'est pas correct\n");
            }
            if(choix == choix2){
                printf("Vous ne pouvez pas choisir les 2 mêmes Mechas\n");
            }
        }while((choix == choix2) || (choix2 < i || choix2 > max));

        mecha_temp = joueur->mechas_joueur[choix2];
        joueur->mechas_joueur[choix2] = joueur->mechas_joueur[choix];
        joueur->mechas_joueur[choix] = mecha_temp;
    }

    for(j = i; j < max; j++){
        printf("Nom : %s\n", mecha[joueur->mechas_joueur[j].id_mechas-1].nom);
    }

    return 1;
}


int attaque_joueur(mechas_joueur_t *mecha_joueur, mechas_joueur_t *mecha_ordi, attaque_t *attaque, mechas_t *mecha){

    int choix_attaque = -1;
    int att1, att2;
    int ancien_pv;
    int nbr_rand;

    //Lister les 2 attaques
    printf("%s : %d dégats\n", attaque[mecha_joueur->attaque_1].nom, attaque[mecha_joueur->attaque_1].degats);
    printf("%s : %d dégats\n", attaque[mecha_joueur->attaque_2].nom, attaque[mecha_joueur->attaque_2].degats);

    
    while(choix_attaque < 1 || choix_attaque > 2){
        printf("Choisir l'attaque : ");
        scanf("%d", &choix_attaque);
        if(choix_attaque < 1 || choix_attaque > 2){
            printf("Attaque indisponible");
        }
        if((choix_attaque == 1) && (attaque[mecha_joueur->attaque_1].utilisations <= 0)){
            printf("Vous ne pouvez pas choisir cette attaque (utilisation = 0)\n");
            choix_attaque = 0;
        }
        else if((choix_attaque == 2) && (attaque[mecha_joueur->attaque_2].utilisations <= 0)){
            printf("Vous ne pouvez pas choisir cette attaque (utilisation = 0)\n");
            choix_attaque = 0;
        }
    }

    ancien_pv = mecha_ordi->pv;
    att1 = attaque[mecha_joueur->attaque_1].degats;
    att2 = attaque[mecha_joueur->attaque_2].degats;

    srand(time(NULL));
    nbr_rand = (rand() % 100) + 1;

    if(choix_attaque == 1){
        if(nbr_rand <= attaque[mecha_joueur->attaque_1].precision){  //Test si l'attaque touche (precision)

            // Test en fonction des types (+ de degats ou non)
            if(((strcmp(attaque[mecha_joueur->attaque_1].type, "Carburant") == 0) && (strcmp(mecha[mecha_ordi->id_mechas-1].type, "Electrique") == 0))
            || ((strcmp(attaque[mecha_joueur->attaque_1].type, "Electrique") == 0) && (strcmp(mecha[mecha_ordi->id_mechas-1].type, "Renouvelable") == 0))
            || ((strcmp(attaque[mecha_joueur->attaque_1].type, "Renouvelable") == 0) && (strcmp(mecha[mecha_ordi->id_mechas-1].type, "Carburant") == 0))){
                att1 *= 1.5;
            }
            else if((strcmp(attaque[mecha_joueur->attaque_1].type, "Nucleaire" ) == 0) && strcmp(mecha[mecha_ordi->id_mechas-1].type, "Nucleaire")){
                att1 *= 1.2;
            }

            if(mecha_joueur->attaque + att1 > mecha_ordi->defense){
                mecha_ordi->pv -= (mecha_joueur->attaque + att1 - mecha_ordi->defense);
            }

            attaque[mecha_joueur->attaque_1].utilisations--;    //Decrementer le nombre d'utilisations restantes
        }
    }
    else{
        if(nbr_rand <= attaque[mecha_joueur->attaque_2].precision){ //Test si l'attaque touche (precision)
            // Test en fonction des types (+ de degats ou non)
            if(((strcmp(attaque[mecha_joueur->attaque_2].type, "Carburant") == 0) && (strcmp(mecha[mecha_ordi->id_mechas-1].type, "Electrique") == 0))
            || ((strcmp(attaque[mecha_joueur->attaque_2].type, "Electrique") == 0) && (strcmp(mecha[mecha_ordi->id_mechas-1].type, "Renouvelable") == 0))
            || ((strcmp(attaque[mecha_joueur->attaque_2].type, "Renouvelable") == 0) && (strcmp(mecha[mecha_ordi->id_mechas-1].type, "Carburant") == 0))){
                att2 *= 1.5;
            }
            else if((strcmp(attaque[mecha_joueur->attaque_1].type, "Nucleaire" ) == 0) && strcmp(mecha[mecha_ordi->id_mechas-1].type, "Nucleaire")){
                att2 *= 1.2;
            }

            if(mecha_joueur->attaque + att2 > mecha_ordi->defense){
                mecha_ordi->pv -= (mecha_joueur->attaque + att2 - mecha_ordi->defense);
            }

            attaque[mecha_joueur->attaque_2].utilisations--;    //Decrementer le nombre d'utilisations restantes
        }
    }
    printf("PV : %d --> %d\n", ancien_pv, mecha_ordi->pv);
    return OK;
}


int choix_action(char nom[], int i){

    int choix = 0;
    int etat = 1;

    printf("Quelle action faire ?\n");
    printf("1 : Attaquer\n");
    printf("2 : Utiliser un objet\n");
    printf("3 : Changer de Mecha\n");
    printf("4 : Fuir\n");
    while(choix < 1 || choix > 4){
        printf("\nSaisir l'action à faire : ");
        scanf("%d", &choix);
        printf("\n");
        if(choix < 1 || choix > 4){
            printf("Choix incorrect\n");
        }
    }

    switch(choix){
        case 1 : attaque_joueur(&joueur.mechas_joueur[0], &joueur.mechas_joueur[1], attaque, mecha);
        return 1;
        break;
        case 2 : utilisation_objet(&joueur, i);
        return 2;
        break;
        case 3 : changer_mecha(&joueur, mecha, nom, etat);
        return 3;
        break;
        case 4 : return 0;
        break;
    }
    return 0;

}
int apprentissage_attaque(mechas_joueur_t *mecha_joueur){
    int i;
    int choix = -1;
    printf("FONCTION ATTAQUE :\n");
    for(i = 0;attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1 ].niveau != mecha_joueur->niveau && i<5 ;i++){
        printf("%d\n",i);
    }
    if(attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1 ].niveau == mecha_joueur->niveau){
        printf("Votre mechas souhaite apprendre une nouvelle attaque: %s\nDégats:%d\nPrecision:%d\n",
        attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].nom,attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].degats,
        attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i]-1].precision);
        printf("choisiseez l'attaques à remplacer ou appuyer sur 0 pour pas l'apprendre\n");
        printf("attaque 1:%s\nDégats:%d\nPrecision:%d\n",
        attaque[mecha_joueur->attaque_1 -1].nom,attaque[mecha_joueur->attaque_1 -1].degats,attaque[mecha_joueur->attaque_1 -1].precision);
        printf("attaque 2:%s\nDégats:%d\nPrecision:%d\nAttaque à remplacer :",
        attaque[mecha_joueur->attaque_2 -1].nom,attaque[mecha_joueur->attaque_2 -1].degats,attaque[mecha_joueur->attaque_2 -1].precision);
        do{
            scanf("%d",&choix);
            if(choix < 0 || choix > 2)
                printf("Veuillez choisir entre 0 et 2 : \n");
        } while(choix < 0 || choix > 2);
        switch(choix){
            case 1:
                printf("Vous avez remplacer votre attaque %s par %s\n",
                attaque[mecha_joueur->attaque_1-1].nom,attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i]-1].nom);
                mecha_joueur->attaque_1 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].id_attaques;
                mecha_joueur->utilisation_1 = attaque[mecha[mecha_joueur->id_mechas -1].liste_attaque[i] -1].utilisations;
                break;
            case 2:
                printf("Vous avez remplacer votre attaque %s par %s\n",
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
    int nouv_level, k, i;

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
    int lvlup, xp_gagne, nouv_level;

   
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


/*
void combat_init(){

    int choix1, choix2;

    mechas_joueur_t tab_mecha[N];
    mechas_t mecha[N];
    
    affichage_mecha(tab_mecha, mecha, nom);
    //choix1 = choix_mecha();
    //choix2 = choix_mecha();
    
    int gagnant = tour_jeu(choix1, choix2, tab_mecha, mecha);
    int perdant;
    
    
    if(gagnant == 1){
        printf("%s a gagné !\n", mecha[choix1].nom);
        perdant = 2;
    }
    else{
        printf("%s a gagné !\n", mecha[choix2].nom);
        perdant = 1;
    }

    level_mechas(gagnant-1, perdant-1, tab_mecha, mecha);

}
*/

int tour_jeu(joueur_t *joueur, joueur_t *mecha_joueur, joueur_t *mecha_ordi){
    int tour = 1;
    int i = 0;

   /* if(mecha_joueur->vitesse >= mecha_ordi->vitesse){
        tour = 1;
    }
    else{
        tour = 0;
    }*/

    while(mecha_joueur->mechas_joueur[i].pv > 0 && mecha_ordi->mechas_joueur[i].pv > 0){
        if(tour){
            choix_action(nom, i);
            //tour--;
        }
        else{

        }
    }
    level_mechas(mecha_joueur, mecha_ordi);
    return OK;
}

void combat_sauvage(joueur_t *joueur, mechas_joueur_t *mecha_sauvage, game_t *game) {
    SDL_Event event;
    Uint32 frameStart;  
    int frameTime;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    int running =1;
    int save_map_active = game->mat_active;
    game->mat_active = 6;
    rectangle_t test;
    char texte[50]= "test";
    creer_rectangle(&test, 50, 50, 50, 50, 255, 255, 255, 150, texte);
    while(running) {
        frameStart = SDL_GetTicks();    //obtien heure

        SDL_PumpEvents();  // Met à jour l'état des événements (telles que les touches pressées)
        keys = SDL_GetKeyboardState(NULL);  // Met à jour l'état des touches

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {       //si on appuie sur fermer la fenetre running = 0
                running = 0;
            }
            if(event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                }
            }

        }

        SDL_RenderClear(game->renderer);     //efface l'ecran

        draw_background(game);
        draw_rect(game, &test);
        SDL_RenderPresent(game->renderer);      //affiche rendu
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAYS > frameTime) {
            SDL_Delay(FRAME_DELAYS - frameTime); // Attend le temps restant
        }
    }
    game->mat_active = save_map_active;
}

/*
int main(){
    
    //combat_init();
    int i = 1;
    

    
    tour_jeu(&joueur, &joueur, &joueur);
    

    recuperation_joueur(&joueur, "player1");
    recuperation_mechas(mecha);
    recuperation_attaques(attaque);

    choix_action(nom, i);
    
    montee_niveau(&joueur.mechas_joueur[0],10,20);
    sauvegarde_partie(&joueur, "player1");
}*/