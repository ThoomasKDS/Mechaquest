/**
 * \file combat.c
 * \brief fichier contenant la fonctions de comba
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../lib/combat.h"
#include "../lib/sauv.h"

#define N 2
#define Carb 1
#define Elec 2
#define Renouv 3
#define Nucl 4

char nom[50] = "noaha";

typedef struct{
    char nom[50];
    int mecha1;
    int mecha2;
    int mecha3;
    int mecha4;
}mecha_equipe_t;


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

int util_objet(joueur_t *joueur, int i){
    int choix = 0;
    while(choix < 1 || choix > 3){
        printf("Que voulez-vous utiliser ?\n");
        printf("1 : Carburant (reste %d)\n", joueur->inventaire->carburant);
        printf("2 : Rappel (reste %d)\n", joueur->inventaire->rappel);
        printf("3 : Retour\n");
        scanf("%d", &choix);
        if(choix == 1){
            if(joueur->mechas_joueur[i].pv == joueur->mechas_joueur[i].pv_max){    //Si les PV sont au Max, on ne peut pas utiliser de carburant
                printf("Les PV sont déjà au maximun !\n");
                choix = 0;
            }
            else if(joueur->mechas_joueur[i].pv + 20 > joueur->mechas_joueur[i].pv_max){ //Si les PV + 20 sont sup aux PV Max, les PV deviennent les PV Max
                printf("Ancien PV : %d\n", joueur->mechas_joueur[i].pv);
                joueur->mechas_joueur[i].pv = joueur->mechas_joueur[i].pv_max;
                joueur->inventaire->carburant--;
                printf("Nouveau PV : %d\n", joueur->mechas_joueur[i].pv);
            }
            else if(joueur->mechas_joueur[i].pv == 0){  //Si les PV sont a 0, impossible d'utiliser du carburant
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
            if(joueur->mechas_joueur[i].pv == 0){
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
    
    int choix = 0;
    int i;
    int max;
    if(etat){
        i = 1;
        max = 4;
    }
    else{
        i = 0;
        max = joueur->nb_mechas;
    }
    char nom_mecha[50];
    char type_mecha[50];
    while(choix < 1 || choix > 3){
        printf("Quel Mecha échanger ?\n");
        for(i = 0; i < joueur->nb_mechas && i < max; i++){
            printf("Nom : %s\n", mecha[joueur->mechas_joueur[i].id_mechas-1].nom);
            printf("Mecha %d : \n", i);
            printf("Niveau : %d\n", joueur->mechas_joueur[i].niveau);
            printf("PV : %d\n", joueur->mechas_joueur[i].pv);
            printf("PV Max: %d\n", joueur->mechas_joueur[i].pv_max);
            printf("Vitesse : %d\n", joueur->mechas_joueur[i].vitesse);
            printf("Attaque 1 : %d\n", joueur->mechas_joueur[i].attaque_1);
            printf("Attaque 2 : %d\n", joueur->mechas_joueur[i].attaque_2);
            printf("Attaque générale : %d\n", joueur->mechas_joueur[i].attaque);
            printf("Défense : %d\n", joueur->mechas_joueur[i].defense);
            printf("\n");
        }

        printf("Choix : ");
        scanf("%d", &choix);
        //A terminer
    }
    return 1;
}

int choix_action(int i, char nom[]){

    int choix;
    joueur_t joueur;
    recuperation_joueur(&joueur, nom);
    mechas_t mecha[20];
    recuperation_mechas(mecha);


    printf("Quelle action faire ?\n");
    printf("1 : Attaquer\n");
    printf("2 : Utiliser un objet\n");
    printf("3 : Changer de Mecha\n");
    printf("4 : Fuir\n");
    scanf("%d", &choix);

    switch(choix){
        case 1 : return 1;
        break;
        case 2 : util_objet(&joueur, i);
        return 2;
        break;
        case 3 : changer_mecha(&joueur, mecha, nom);
        return 3;
        break;
        case 4 : return 0;
        break;
    }
    return 0;

}

int tour_jeu(int i, int j, mechas_joueur_t tab_mecha[], mechas_t mecha[]){
    int tour = 1;
    int att, pv1, pv2;
    
    while(tab_mecha[i].pv > 0 && tab_mecha[j].pv > 0){  //Tant que les mechas ont des PV
        if(tour){
            printf("\nAu tour de %s\n", mecha[i].nom);
            choix_action(i, nom);
            printf("Choisissez l'attaque : \n");
            printf("Attaque 1 : %d dégats\n", tab_mecha[i].attaque_1);
            printf("Attaque 2 : %d dégats\n", tab_mecha[i].attaque_2);
            scanf("%d", &att);

            pv2 = tab_mecha[j].pv;

            if(att == 1){   //Attaque 1
                if(tab_mecha[i].attaque_1 + tab_mecha[i].attaque > tab_mecha[j].defense){    //Vérifier que l'attaque est sup à la défense
                    tab_mecha[j].pv -= tab_mecha[i].attaque_1 + tab_mecha[i].attaque - tab_mecha[j].defense;
                }
                else{   //Ne rien faire si la défense est sup à l'attaque
                }
            }
            else{   //Attaque 2
                if(tab_mecha[i].attaque_2 + tab_mecha[i].attaque > tab_mecha[j].defense){    //Vérifier que l'attaque est sup à la défense
                    tab_mecha[j].pv -= tab_mecha[i].attaque_2 + tab_mecha[i].attaque - tab_mecha[j].defense;
                }
                else{   //Ne rien faire si la défense est sup à l'attaque
                }
            }
            printf("PV de %s : %d --> %d\n", mecha[j].nom, pv2, tab_mecha[j].pv);
            tour--;
        }
        else{
            printf("\nAu tour de %s\n", mecha[j].nom);
            printf("Choisissez l'attaque : \n");
            printf("Attaque 1 : %d dégats\n", tab_mecha[j].attaque_1);
            printf("Attaque 2 : %d dégats\n", tab_mecha[j].attaque_2);
            scanf("%d", &att);

            pv1 = tab_mecha[i].pv;

            if(att == 1){   //Attaque 1
                if(tab_mecha[j].attaque_1 + tab_mecha[j].attaque > tab_mecha[i].defense){    //Vérifier que l'attaque est sup à la défense
                    tab_mecha[i].pv -= tab_mecha[j].attaque_1 + tab_mecha[j].attaque - tab_mecha[i].defense;
                }
                else{   //Ne rien faire si la défense est sup à l'attaque
                }
            }
            else{   //Attaque 2
                if(tab_mecha[j].attaque_2 + tab_mecha[j].attaque > tab_mecha[i].defense){    //Vérifier que l'attaque est sup à la défense
                    tab_mecha[i].pv -= tab_mecha[j].attaque_2 + tab_mecha[j].attaque - tab_mecha[i].defense;
                }
                else{   //Ne rien faire si la défense est sup à l'attaque
                }
            }
            printf("PV de %s : %d --> %d\n", mecha[i].nom, pv1, tab_mecha[i].pv);
            tour++;
        }
    }
    if(tab_mecha[i].pv <= 0) return 2;
    else return 1;
}

void level_mechas(int gagnant, int perdant, mechas_joueur_t tab_mecha[], mechas_t mecha[]){
    int i, j, k;
    int lvlup, xp_gagne, nouv_level;
    for(i = 1; i <= tab_mecha[gagnant].niveau; i++){   //Calculer l'XP necessaire pour lvlup
        lvlup = (int)(15 * exp(0.05 * i));
    }
    for(j = 1; j <= tab_mecha[perdant].niveau; j++){   //Calculer l'XP recu en tuant l'adversaire
        xp_gagne = (int)(20 * exp(0.02 * j));
    }

    nouv_level = tab_mecha[gagnant].xp + xp_gagne;
    while(nouv_level >= lvlup){
        tab_mecha[gagnant].niveau++;
        nouv_level -= lvlup;
        tab_mecha[gagnant].xp = 0;
        tab_mecha[gagnant].xp += nouv_level;
        for(k = 1; k <= tab_mecha[gagnant].niveau; k++){   //Calculer le nouveau nombre d'XP necessaire pour lvlup
            lvlup = (int)(15 * exp(0.05 * i));
        }
    }
    printf("LVL %s : %d\n", mecha[gagnant].nom, tab_mecha[gagnant].niveau);
}


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

int main(){
    //combat_init();
    joueur_t joueur;
    int i = 1;

    choix_action(i, nom);
}