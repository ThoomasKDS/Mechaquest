#include "../lib/sauv.h"
#include <stdio.h>
#include <string.h>

int RecuperationJoueur(Joueur *joueur, char pseudo[50]) {   //Recuperation de la sauvegarde joueur dans la structure Joueur
    FILE *file = fopen("../save/joueur.csv", "r");          //Ouverture du fichier
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[50];
    char ligne[256];
    fgets(ligne, sizeof(ligne), file); // Lire la ligne d'en-tête
    while(fgets(ligne, sizeof(ligne), file) != NULL){         //Prendre chaque ligne, verifie que le pseudo corresponds et copie les données si correspondance
        sscanf(ligne, "%[^,]", nom);
        if(!strcmp(nom,pseudo)){
            sscanf(ligne, "%49[^,],%c,%d,%d,%d\n",joueur->pseudo,&joueur->sexe,&joueur->x,&joueur->y,&joueur->pointSauvegarde);
            fclose(file);
            return OK;
        }
    }
    fclose(file);       //Aucune correspondance
    return ERR;
}

int RecuperationInventaire(Inventaire *inventaire, char pseudo[50]) { //Recuperation de l'inventaire dans la structure inventaire avec le pseudo associer
    FILE *file = fopen("../save/inventaire.csv", "r");      //Ouverture du fichier
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }

    char nom[50];
    char ligne[256];
    fgets(ligne, sizeof(ligne), file); // Lire la ligne d'en-tête
    while(fgets(ligne, sizeof(ligne), file) != NULL){       //Prendre chaque ligne, verifie que le pseudo corresponds et copie les données si correspondance
        sscanf(ligne, "%[^,]", nom);
        if(!strcmp(nom,pseudo)){
            sscanf(ligne, "%49[^,],%d,%d,%d,%d\n",inventaire->pseudo,&inventaire->mechaball,
                                                  &inventaire->carburant,&inventaire->repousse,&inventaire->rappel);
            fclose(file);
            return OK;
        }
    }
    fclose(file);   //Aucune correspondance
    return ERR;
}

int RecuperationAttaques(Attaque *attaques) {           //Recuperation des attaques dans le tableau de structures d'Attaque
    FILE *file = fopen("../save/attaques.csv", "r");        //Ouverture du fichier
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }

    char ligne[256];
    fgets(ligne, sizeof(ligne), file); // Lire la première ligne (en-têtes)

    int count = 0;

    while (fgets(ligne, sizeof(ligne), file) != NULL){      //Prends chaque ligne et l'enregistre
        sscanf(ligne, "%49[^,],%19[^,],%d,%d,%d,%d\n",
                                 attaques[count].nom,
                                 attaques[count].type,
                                 &attaques[count].niveau,
                                 &attaques[count].degats,
                                 &attaques[count].precision,
                                 &attaques[count].utilisations) ;
        count++;
    }
    fclose(file);
    return count;       //renvoies le nombre d'attqaues
}


//TEST
 /*int main(void){
    Joueur joueur;
    Inventaire inventaire;
    Attaque attaques[50];
    int nb = RecuperationAttaques(attaques);
    char pseudo[50] = "noaha";
    RecuperationJoueur(&joueur,pseudo);
    RecuperationInventaire(&inventaire,pseudo);
    printf("pseudo: %s   sexe: %c\n",joueur.pseudo,joueur.sexe);
    printf("pseudo: %s   mechaball: %d   carburant: %d\n",inventaire.pseudo,inventaire.mechaball,inventaire.carburant);
    for(int i = 0;i<nb;i++){
        printf("nom: %s   type: %s   niveau: %d\n",attaques[i].nom,attaques[i].type, attaques[i].niveau);
    }
}*/