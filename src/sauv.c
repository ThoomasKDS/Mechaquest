#include "../lib/sauv.h"


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
            joueur->inventaire = malloc(1 * sizeof(Inventaire));    //alloue la memoire de la structure inventaire
            RecuperationInventaire(joueur->inventaire,joueur->pseudo);      //Appel la fonction de récupération de l'inventaire
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
            sscanf(ligne, "%49[^,],%d,%d,%d,%d\n",nom,&inventaire->mechaball,&inventaire->carburant,
                                                    &inventaire->repousse,&inventaire->rappel);
            fclose(file);
            return OK;
        }
    }
    fclose(file);   //Aucune correspondance
    return ERR;
}

int RecuperationMechasJoueur(Mechas_Joueur * mechas_joueur,char pseudo[50]) { //Recuperation de l'inventaire dans la structure inventaire avec le pseudo associer
    int nb_mechas = 0;
    FILE *file = fopen("../save/infomechas.csv", "r");      //Ouverture du fichier
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
            sscanf(ligne, "%49[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                nom,&mechas_joueur[nb_mechas].numero,&mechas_joueur[nb_mechas].id_mechas,
                &mechas_joueur[nb_mechas].niveau,&mechas_joueur[nb_mechas].xp,&mechas_joueur[nb_mechas].pv,
                &mechas_joueur[nb_mechas].pv_max,&mechas_joueur[nb_mechas].attaque,&mechas_joueur[nb_mechas].defense,
                &mechas_joueur[nb_mechas].vitesse,&mechas_joueur[nb_mechas].attaque_1,&mechas_joueur[nb_mechas].attaque_2);
            nb_mechas++;
        }
    }
    fclose(file);   //Aucune correspondance
    return nb_mechas;
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
        sscanf(ligne, "%d,%49[^,],%19[^,],%d,%d,%d,%d\n",
                                 &attaques[count].id_attaques,
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

int RecuperationMechas(Mechas *mechas_l) {           //Recuperation des attaques dans le tableau de structures d'Attaque
    int taille;
    FILE *file = fopen("../save/listeMechas.csv", "r");        //Ouverture du fichier
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }

    char ligne[256];
    fgets(ligne, sizeof(ligne), file); // Lire la première ligne (en-têtes)

    int count = 0;
    char attaques[30];
    while (fgets(ligne, sizeof(ligne), file) != NULL){      //Prends chaque ligne et l'enregistre
        sscanf(ligne, "%d,%49[^,],%19[^,],%d,%199[^,],%[^\n]",
                                 &mechas_l[count].id_mechas,
                                 mechas_l[count].nom,
                                 mechas_l[count].type,
                                 &mechas_l[count].evolution,
                                 mechas_l[count].description,
                                 attaques);     //recupere les numero des attaques mais en chaines de caractere
        taille = 0;  // Initialisation du compteur
        char *token = strtok(attaques, ";");  // Découpe la première partie des attaques

        while (token != NULL) {
            mechas_l[count].liste_attaque[taille] = atoi(token);  // Convertit en entier et stocke
            taille++;  // Incrémente le compteur
            token = strtok(NULL, ";");  // Passe au nombre suivant
        }
        mechas_l[count].nb_attaques = taille;
        count++;
    }
        
    fclose(file);
    return count;       //renvoies le nombre d'attqaues
}


//TEST

/*
 int main(void){
 
    Joueur joueur;

    Attaque attaques[50];
    int nb = RecuperationAttaques(attaques);
    char pseudo[50] = "noaha";
    RecuperationJoueur(&joueur,pseudo);
    printf("pseudo: %s   sexe: %c\n",joueur.pseudo,joueur.sexe);
    printf("pseudo: %s   mechaball: %d   carburant: %d\n",joueur.pseudo,joueur.inventaire->mechaball,joueur.inventaire->carburant);
    for(int i = 0;i<nb;i++){
        printf("id: %d    nom: %s   type: %s   niveau: %d\n",attaques[i].id_attaques,attaques[i].nom,attaques[i].type, attaques[i].niveau);
    }
    Mechas mechas_l[50];
    int nb = RecuperationMechas(mechas_l);
    for(int i = 0; i < nb;i++){
         printf("\n-----------------------------\n");
        printf("ID: %d\n", mechas_l[i].id_mechas);
        printf("Nom: %s\n", mechas_l[i].nom);
        printf("Type: %s\n", mechas_l[i].type);
        printf("Évolution: %d\n", mechas_l[i].evolution);
        printf("Description: %s\n", mechas_l[i].description);
        printf("listes attaques : ");
        for (int j = 0; j < mechas_l[i].nb_attaques; j++) {
            printf("%d ", mechas_l[i].liste_attaque[j]);
        }
        printf("\n");

    }
    
    Mechas_Joueur mechas_joueur[50];
    char pseudo[50] = "Noahb";
    int nb = RecuperationMechasJoueur(mechas_joueur,pseudo);
    printf("%d\n",nb);
    for(int i = 0;i<nb;i++){
        printf("num: %d    id: %d   niveau: %d   xp: %d\n",mechas_joueur[i].numero,mechas_joueur[i].id_mechas,mechas_joueur[i].niveau, mechas_joueur[i].xp);
    }
}
*/