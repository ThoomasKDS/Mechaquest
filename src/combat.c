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

int utilisation_objet(joueur_t *joueur, mechas_joueur_t *ordi, int i){
    int choix = 0;
    while(choix < 1 || choix > 4){
        printf("Que voulez-vous utiliser ?\n");
        printf("1 : Carburant (reste %d)\n", joueur->inventaire->carburant);
        printf("2 : Rappel (reste %d)\n", joueur->inventaire->rappel);
        printf("3 : Mechaball (reste %d)\n", joueur->inventaire->mechaball);
        printf("4 : Retour\n");
        scanf("%d", &choix);
        if(choix == 1){
            if(joueur->inventaire->carburant <= 0){
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
            if(joueur->inventaire->rappel <= 0){
                printf("Vous n'avez pas de rappel\n");
                choix = 0;
            }
            else if(joueur->mechas_joueur[i].pv <= 0){
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
            if(joueur->inventaire->mechaball <= 0){
                printf("Vous n'avez pas de mechaball\n");
                choix = 0;
            }
            else{
                int taux_capture;   //Un taux de capture different en fonction du type de mecha (nucleaire + difficile a attraper)
                if((strcmp(mecha[ordi->id_mechas-1].type, "Nucleaire") == 0)){
                    taux_capture = 0.3;
                }
                else{
                    taux_capture = 0.8;
                }
                int proba_capture = (1 - ordi->pv / ordi->pv_max) * taux_capture;   //Calcul les chances d'attraper le mecha

                srand(time(NULL));
                int nbr_rand_capture = (rand() % 100) + 1;

                if(proba_capture <= nbr_rand_capture){  //Si le mecha est attrapé
                    copie_mechas(joueur, ordi);
                }
                joueur->inventaire->mechaball--;
            }
        }
        else if(choix == 4){
            return 3;
        }
        else{
            printf("Choix indisponible\n");
        }
    }
    return 0;

}



int changer_mecha(joueur_t *joueur, char nom[], int etat){
    
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


int attaque_joueur(mechas_joueur_t *mecha_joueur, mechas_joueur_t *mecha_ordi){

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
        case 1 : attaque_joueur(&joueur.mechas_joueur[0], &joueur.mechas_joueur[1]);
        return 1;
        break;
        case 2 : utilisation_objet(&joueur, &ordi, i);
        return 2;
        break;
        case 3 : changer_mecha(&joueur, nom, etat);
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


int tour_jeu(joueur_t *joueur, pnj_t *ordi, int pnj){
    int tour = 0;
    int i;
    int som_pv = 0;

    if(joueur->mechas_joueur[0].vitesse >= ordi->mechas_joueur[0].vitesse){
        tour = 1;
    }

    while(joueur->mechas_joueur[0].pv >= 0 && ordi->mechas_joueur[0].pv >= 0){
        if(tour){
            for(i = 0; i < 3 || i < joueur->nb_mechas; i++){
                som_pv += joueur->mechas_joueur[i].pv;
            }
            if(joueur->mechas_joueur[0].pv >= 0 && som_pv > 0){ //Si le mecha n'a plus de PV mais que les autres en ont, changer mecha
                changer_mecha(&joueur, nom, 1);
            }
            else{
                choix_action(nom, i);
            }
            tour--;
        }
        else if(pnj){
            attaque_ordi_pnj(ordi, &joueur->mechas_joueur[0]);
            tour++;
        }
        else{
            attaque_ordi_sauvage(&ordi->mechas_joueur[0], &joueur->mechas_joueur[0]);
            tour++;
        }
    }
    return 1;
}

void combat_sauvage(joueur_t *joueur, mechas_joueur_t *mecha_sauvage, game_t *game) {
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    int running = 1;
    int save_map_active = game->mat_active;
    game->mat_active = 6;
    int win_w, win_h;
    SDL_GetRendererOutputSize(game->renderer, &win_w, &win_h);       //dimensions ecran
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
    
    // Création des rectangles
    rectangle_t rect_bas, rect_attaque, rect_objet, rect_changer_mecha, rect_fuite;
    rectangle_t border_attaque, border_objet, border_mecha, border_fuite;
    
    creer_rectangle(game, &rect_bas, rect_bas_w, rect_bas_h, rect_bas_x, rect_bas_y, 0, 0, 0, 150, NULL);
    creer_rectangle(game, &rect_attaque, btn_w, btn_h, btn_center_x, btn_center_y, 165, 36, 8, 250, "Attaquer");
    creer_rectangle(game, &rect_objet, btn_w, btn_h, btn_left_x, btn_center_y, 204, 116, 16, 250, "Objet");
    creer_rectangle(game, &rect_changer_mecha, btn_w, btn_h, btn_right_x, btn_center_y, 69, 150, 54, 250, "Mechas");
    creer_rectangle(game, &rect_fuite, btn_w, btn_h / 1.5, btn_center_x, btn_fuite_y, 7, 103, 160, 250, "Fuite");
    
    // Création des bordures
    creer_rectangle(game, &border_attaque, btn_w + 2 * border_size, btn_h + 2 * border_size, btn_center_x - border_size, btn_center_y - border_size, 210, 145, 132, 255, NULL);
    creer_rectangle(game, &border_objet, btn_w + 2 * border_size, btn_h + 2 * border_size, btn_left_x - border_size, btn_center_y - border_size, 94, 99, 102, 255, NULL);
    creer_rectangle(game, &border_mecha, btn_w + 2 * border_size, btn_h + 2 * border_size, btn_right_x - border_size, btn_center_y - border_size, 94, 99, 102, 255, NULL);
    creer_rectangle(game, &border_fuite, btn_w + 2 * border_size, (btn_h / 1.5) + 2 * border_size, btn_center_x - border_size, btn_fuite_y - border_size, 94, 99, 102, 255, NULL);
    
    while (running) {
        frameStart = SDL_GetTicks();
        
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
            }
            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_LEFT){
                    choix = (choix - 1 + 3) % 3;
                }
                if(event.key.keysym.sym == SDLK_RIGHT){
                    choix = (choix + 1) % 3;
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
        draw_rect(game, &rect_bas);
        draw_rect(game, &border_attaque);
        draw_rect(game, &border_objet);
        draw_rect(game, &border_mecha);
        draw_rect(game, &border_fuite);
        draw_rect(game, &rect_attaque);
        draw_rect(game, &rect_objet);
        draw_rect(game, &rect_changer_mecha);
        draw_rect(game, &rect_fuite);
        SDL_RenderPresent(game->renderer);
        
        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAYS > frameTime) {
            SDL_Delay(FRAME_DELAYS - frameTime);
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