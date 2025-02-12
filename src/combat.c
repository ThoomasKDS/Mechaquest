/**
 * \file combat.c
 * \brief fichier contenant la fonctions de combat
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../lib/combat.h"
#include "../lib/sauv.h"

#define N 2

char nom[50] = "noaha";

joueur_t joueur;
mechas_t mecha[20];
attaque_t attaque[10];



void affichage_mecha(mechas_joueur_t tab_mecha[], mechas_t mecha[], char nom[]){

    //char type[20];
    recuperation_mechas(mecha);
    joueur_t joueur;
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

    //Lister les 2 attaques
    printf("%s : %d dégats\n", attaque[mecha_joueur->attaque_1].nom, attaque[mecha_joueur->attaque_1].degats);
    printf("%s : %d dégats\n", attaque[mecha_joueur->attaque_2].nom, attaque[mecha_joueur->attaque_2].degats);


    while(choix_attaque < 1 || choix_attaque > 2){
        printf("Choisir l'attaque : ");
        scanf("%d", &choix_attaque);
        if(choix_attaque < 1 || choix_attaque > 2){
            printf("Attaque indisponible");
        }
    }

    ancien_pv = mecha_ordi->pv;
    att1 = attaque[mecha_joueur->attaque_1].degats;
    att2 = attaque[mecha_joueur->attaque_2].degats;

    printf("type attaque : %s, type mecha : %s\n", attaque[mecha_joueur->attaque_1].type, mecha[mecha_ordi->id_mechas-1].type);

    if(choix_attaque == 1){
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
    }
    else{
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
    }
    printf("PV : %d --> %d\n", ancien_pv, mecha_ordi->pv);
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

void montee_niveau(joueur_t *mecha, int xp_partage, int lvlup){
    int nouv_level, k, i;

    nouv_level = mecha->mechas_joueur->xp + xp_partage;
    while(nouv_level >= lvlup){
        mecha->mechas_joueur->niveau++;
        nouv_level -= lvlup;
        mecha->mechas_joueur->xp = 0;
        mecha->mechas_joueur->xp += nouv_level;
        lvlup = (int)(15 * exp(0.05 * mecha->mechas_joueur->niveau));   //Calculer le nouveau nombre d'XP necessaire
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

}
/*

int main(){
    //combat_init();
    int i = 1;
    //choix_action(nom, i);

    recuperation_joueur(&joueur, nom);
    recuperation_mechas(mecha);
    recuperation_attaques(attaque);

    
    tour_jeu(&joueur, &joueur, &joueur);
}*/