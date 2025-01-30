#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../lib/combat.h"
#include "../lib/mecha.h"

#define N 2


void liste_mecha(mecha_t tab_mecha[]){
    int i;
    for(i = 0; i < N; i++){
        printf("Mecha %d : \n", i+1);
        printf("%s\n", tab_mecha[i].nom);
        printf("Niveau : %d\n", tab_mecha[i].lvl);
        printf("PV : %d\n", tab_mecha[i].pv);
        printf("Attaque 1 : %d\n", tab_mecha[i].att1);
        printf("Attaque 2 : %d\n", tab_mecha[i].att2);
        printf("Attaque générale : %d\n", tab_mecha[i].att_gen);
        printf("Défense : %d\n", tab_mecha[i].def);
        printf("\n");
    }
}

int choix_mecha(){
    int choix;
    printf("Choisissez le mecha : ");
    scanf("%d", &choix);
    
    switch(choix){
        case 1 : return 0;
        break;
        case 2 : return 1;
        break;
        case 3 : return 2;
        break;
        case 4 : return 3;
        break;
        default : return 4;
        break;
    }
}

int tour_jeu(int i, int j, mecha_t tab_mecha[]){
    int tour = 1;
    int att, pv1, pv2;
    
    while(tab_mecha[i].pv > 0 && tab_mecha[j].pv > 0){  //Tant que les mechas ont des PV
        if(tour){
            printf("\nAu tour de %s\n", tab_mecha[i].nom);
            printf("Choisissez l'attaque : \n");
            printf("Attaque 1 : %d dégats\n", tab_mecha[i].att1);
            printf("Attaque 2 : %d dégats\n", tab_mecha[i].att2);
            scanf("%d", &att);

            pv2 = tab_mecha[j].pv;

            if(att == 1){   //Attaque 1
                if(tab_mecha[i].att1 + tab_mecha[i].att_gen > tab_mecha[j].def){    //Vérifier que l'attaque est sup à la défense
                    tab_mecha[j].pv -= tab_mecha[i].att1 + tab_mecha[i].att_gen - tab_mecha[j].def;
                }
                else{   //Ne rien faire si la défense est sup à l'attaque
                }
            }
            else{   //Attaque 2
                if(tab_mecha[i].att2 + tab_mecha[i].att_gen > tab_mecha[j].def){    //Vérifier que l'attaque est sup à la défense
                    tab_mecha[j].pv -= tab_mecha[i].att2 + tab_mecha[i].att_gen - tab_mecha[j].def;
                }
                else{   //Ne rien faire si la défense est sup à l'attaque
                }
            }
            printf("PV de %s : %d --> %d\n", tab_mecha[j].nom, pv2, tab_mecha[j].pv);
            tour--;
        }
        else{
            printf("\nAu tour de %s\n", tab_mecha[j].nom);
            printf("Choisissez l'attaque : \n");
            printf("Attaque 1 : %d dégats\n", tab_mecha[j].att1);
            printf("Attaque 2 : %d dégats\n", tab_mecha[j].att2);
            scanf("%d", &att);

            pv1 = tab_mecha[i].pv;

            if(att == 1){   //Attaque 1
                if(tab_mecha[j].att1 + tab_mecha[j].att_gen > tab_mecha[i].def){    //Vérifier que l'attaque est sup à la défense
                    tab_mecha[i].pv -= tab_mecha[j].att1 + tab_mecha[j].att_gen - tab_mecha[i].def;
                }
                else{   //Ne rien faire si la défense est sup à l'attaque
                }
            }
            else{   //Attaque 2
                if(tab_mecha[j].att2 + tab_mecha[j].att_gen > tab_mecha[i].def){    //Vérifier que l'attaque est sup à la défense
                    tab_mecha[i].pv -= tab_mecha[j].att2 + tab_mecha[j].att_gen - tab_mecha[i].def;
                }
                else{   //Ne rien faire si la défense est sup à l'attaque
                }
            }
            printf("PV de %s : %d --> %d\n", tab_mecha[i].nom, pv1, tab_mecha[i].pv);
            tour++;
        }
    }
    if(tab_mecha[i].pv <= 0) return 2;
    else return 1;
}

void level_mechas(int gagnant, int perdant, mecha_t tab_mecha[]){
    int i, j, k;
    int lvlup, xp_gagne, nouv_level;
    for(i = 1; i <= tab_mecha[gagnant].lvl; i++){   //Calculer l'XP necessaire pour lvlup
        lvlup = (int)(15 * exp(0.05 * i));
    }
    for(j = 1; j <= tab_mecha[perdant].lvl; j++){   //Calculer l'XP recu en tuant l'adversaire
        xp_gagne = (int)(20 * exp(0.02 * j));
    }

    nouv_level = tab_mecha[gagnant].lvl_avance + xp_gagne;
    while(nouv_level >= lvlup){
        if(nouv_level >= lvlup){
            tab_mecha[gagnant].lvl++;
            nouv_level -= lvlup;
            tab_mecha[gagnant].lvl_avance = 0;
            tab_mecha[gagnant].lvl_avance += nouv_level;
            for(k = 1; k <= tab_mecha[gagnant].lvl; k++){   //Calculer le nouveau nombre d'XP necessaire pour lvlup
                lvlup = (int)(15 * exp(0.05 * i));
            }
        }
    }
    printf("LVL %s : %d\n", tab_mecha[gagnant].nom, tab_mecha[gagnant].lvl);
}


void combat_init(){

    int choix1, choix2;

    mecha_t tab_mecha[N];
    
    strcpy(tab_mecha[0].nom, "Robot");
    tab_mecha[0].lvl = 1;
    tab_mecha[0].lvl_avance = 0;
    tab_mecha[0].pv = 40;
    tab_mecha[0].att1 = 20;
    tab_mecha[0].att2 = 10;
    tab_mecha[0].att_gen = 10;
    tab_mecha[0].def = 10;
    
    strcpy(tab_mecha[1].nom, "Humain");
    tab_mecha[1].lvl = 1;
    tab_mecha[1].lvl_avance = 0;
    tab_mecha[1].pv = 60;
    tab_mecha[1].att1 = 20;
    tab_mecha[1].att2 = 5;
    tab_mecha[1].att_gen = 10;
    tab_mecha[1].def = 15;

    liste_mecha(tab_mecha);
    choix1 = choix_mecha();
    choix2 = choix_mecha();
    
    int gagnant = tour_jeu(choix1, choix2, tab_mecha);
    int perdant;
    
    
    if(gagnant == 1){
        printf("%s a gagné !\n", tab_mecha[choix1].nom);
        perdant = 2;
    }
    else{
        printf("%s a gagné !\n", tab_mecha[choix2].nom);
        perdant = 1;
    }

    level_mechas(gagnant-1, perdant-1, tab_mecha);

}