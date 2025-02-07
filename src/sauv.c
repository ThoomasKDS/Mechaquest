#include "../lib/sauv.h"

//--------FONCTIONS DE RECUPERATION-----------
int recuperation_joueur(joueur_t *joueur, char pseudo[50]) {   //Recuperation de la sauvegarde joueur dans la structure joueur_t
    //Ouverture du fichier
    FILE *file = fopen("../save/joueur.csv", "r");          
    if (file == NULL) {                                     
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[50];
    char ligne[256];
    //traitement du csv
    fgets(ligne, sizeof(ligne), file);                       // Lire la ligne d'en-tête
    while(fgets(ligne, sizeof(ligne), file) != NULL){        //lecture de chaque ligne
        sscanf(ligne, "%[^,]", nom);
        if(!strcmp(nom,pseudo)){                             //verification du pseudo
            sscanf(ligne, "%49[^,], %c,%d,%d,%d\n",joueur->pseudo,&joueur->sexe,&joueur->x,&joueur->y,&joueur->pointSauvegarde);
            //création du pointeur sur la structure inventaire
            joueur->inventaire = (inventaire_t *)malloc(sizeof(inventaire_t));
            if (!joueur->inventaire) {
                perror("Erreur d'allocation mémoire");
                fclose(file);
                return ERREUR_OUVERTURE;
            }

            // Récupération de l'inventaire
            recuperation_inventaire(joueur->inventaire, joueur->pseudo);
            //Récupérer les mechas du joueur
            joueur->nb_mechas = recuperation_mechas_joueur(joueur->mechas_joueur,joueur->pseudo);
            fclose(file);
            return OK;
        }
    }
    fclose(file);       //Aucune correspondance
    return ERR;
}

int recuperation_inventaire(inventaire_t *inventaire, char pseudo[50]) { //Recuperation de l'inventaire dans la structure inventaire_t 
    //Ouverture du fichier
    FILE *file = fopen("../save/inventaire.csv", "r");      
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }

    char nom[50];
    char ligne[256];
    //traitement du csv
    fgets(ligne, sizeof(ligne), file);                      // Lire la ligne d'en-tête
    while(fgets(ligne, sizeof(ligne), file) != NULL){       //Lecture de chaque ligne
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

int recuperation_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[50]) { //Recuperation desmechas du joueur
    int nb_mechas = 0;  
    //Ouverture du fichier
    FILE *file = fopen("../save/infomechas.csv", "r");      
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    int num;
    char nom[50];
    char ligne[256];
    //traitement du csv
    fgets(ligne, sizeof(ligne), file);                      // Lire la ligne d'en-tête
    while(fgets(ligne, sizeof(ligne), file) != NULL){       // lecture de chaque ligne
        sscanf(ligne, "%[^,],%d", nom,&num);
        if(!strcmp(nom,pseudo)){
            sscanf(ligne, "%49[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",     //récupère les bonnes ligne et les rangent au bon endroit du tableau de mechas
                nom,&mechas_joueur[num-1].numero,&mechas_joueur[num-1].id_mechas,
                &mechas_joueur[num-1].niveau,&mechas_joueur[num-1].pv,&mechas_joueur[num-1].pv_max,
                &mechas_joueur[num-1].xp,&mechas_joueur[num-1].attaque,&mechas_joueur[num-1].defense,
                &mechas_joueur[num-1].vitesse,&mechas_joueur[num-1].attaque_1,&mechas_joueur[num-1].attaque_2);
            nb_mechas++;
        }
    }
    fclose(file);           //Aucune correspondance
    return nb_mechas;       //renvoies le nombres de mechas du joueur
}

int recuperation_attaques(attaque_t *attaques) {           //Recuperation des attaques
    //Ouverture du fichier
    FILE *file = fopen("../save/attaques.csv", "r");        
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }

    int count = 0;
    char ligne[256];
    //traitement du csv
    fgets(ligne, sizeof(ligne), file);                      // Lire la première ligne (en-têtes)

    while (fgets(ligne, sizeof(ligne), file) != NULL){      //Lecture de chaque ligne
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

int recuperation_mechas(mechas_t *mechas_l) {           //Recuperation de tous les mechas
    int taille;
    //Ouverture du fichier
    FILE *file = fopen("../save/listeMechas.csv", "r");       
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }

   
    int count = 0;
    char attaques[30];
    char ligne[256];
    //traitement du csv
    fgets(ligne, sizeof(ligne), file);                      // Lire la première ligne (en-têtes)

    while (fgets(ligne, sizeof(ligne), file) != NULL){      //Lecture de chaque ligne
        sscanf(ligne, "%d,%49[^,],%19[^,],%d,%199[^,],%[^\n]",
                                 &mechas_l[count].id_mechas,
                                 mechas_l[count].nom,
                                 mechas_l[count].type,
                                 &mechas_l[count].evolution,
                                 mechas_l[count].description,
                                 attaques);                         //recupere les numero des attaques mais en chaines de caractere
        taille = 0;                                                 // Initialisation du compteur
        char *token = strtok(attaques, ";");                        // Découpe la première partie des attaques

        while (token != NULL) {
            mechas_l[count].liste_attaque[taille] = atoi(token);    // Convertit en entier et stocke
            taille++;                                               // Incrémente le compteur
            token = strtok(NULL, ";");                              // Passe au nombre suivant
        }
        mechas_l[count].nb_attaques = taille;                       
        count++;
    }
        
    fclose(file);
    return count;                                                   //renvoies le nombre de mechas
}
//--------FONCTIONS DE SAUVEGARDE-----------

int sauvegarde_inventaire(inventaire_t *inventaire, char pseudo[50]) { //Sauvegarde de l'inventaire
    int trouver = 0;
    FILE *file = fopen("../save/inventaire.csv", "r");      //Ouverture du fichier inventaire
    FILE *temp = fopen("../save/temporaire.csv", "w");      //Ouverture du futur fichier
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[50];
    char ligne[256];
    //traitement du csv / recopie chaque ligne dans le nouveau fichier
    fgets(ligne, sizeof(ligne), file);          // Lire la ligne d'en-tête
    fprintf(temp,"%s",ligne);                   //mettre la ligne dans le nouveau fichier
    while(fgets(ligne, sizeof(ligne), file) != NULL){ 
        sscanf(ligne, "%[^,]", nom);
        //verifie si la ligne doit etre modifier
        if(!strcmp(nom,pseudo)){
            fprintf(temp,"%s,%d,%d,%d,%d\n",nom,inventaire->mechaball,inventaire->carburant,
                                            inventaire->repousse,inventaire->rappel);
            trouver = OK;           //Détermine si la sauvegarde existe
        }
        else{
            fprintf(temp,"%s",ligne);       //recopie simplement la ligne
        }
    }
    if(!trouver){       //si pas trouver alors écrire une nouvelle ligne
        fprintf(temp,"%s,%d,%d,%d,%d\n",nom,inventaire->mechaball,inventaire->carburant,
                                            inventaire->repousse,inventaire->rappel);
    }
    //fermeture des fichiers
    fclose(file);   
    fclose(temp);

    //supprime et renomme le nouveau fichier
    remove("../save/inventaire.csv");
    rename("../save/temporaire.csv", "../save/inventaire.csv");
    return OK;
}
int sauvegarde_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[50],int nb_mechas) { //Sauvegarde les mechas du joueur
    int indice = 0;
    FILE *file = fopen("../save/infomechas.csv", "r");      //Ouverture du fichier infomechas
    FILE *temp = fopen("../save/temporaire.csv", "w");      //Ouverture du futur fichier
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[50];
    int num;
    char ligne[256];
    //traitement du csv - recopie ligne par ligne du csv
    fgets(ligne, sizeof(ligne), file);                      // Lire la ligne d'en-tête
    fprintf(temp,"%s",ligne);
    while(fgets(ligne, sizeof(ligne), file) != NULL){       
        sscanf(ligne, "%[^,],%d", nom,&num);
        //regarde si la ligne et a changer et la change
        if(!strcmp(nom,pseudo) && num == mechas_joueur[indice].numero){
            fprintf(temp, "%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                nom,mechas_joueur[indice].numero,mechas_joueur[indice].id_mechas,
                mechas_joueur[indice].niveau,mechas_joueur[indice].xp,mechas_joueur[indice].pv,
                mechas_joueur[indice].pv_max,mechas_joueur[indice].attaque,mechas_joueur[indice].defense,
                mechas_joueur[indice].vitesse,mechas_joueur[indice].attaque_1,mechas_joueur[indice].attaque_2);
            indice++;
        }
        else{
            fprintf(temp,"%s",ligne);       //sinon recopie simplement 
        }
        
    }
    while(indice < nb_mechas){  //si les mechas sont pas trouvé créé la ligne de sauvegarde
        fprintf(temp, "%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
            nom,mechas_joueur[indice].numero,mechas_joueur[indice].id_mechas,
            mechas_joueur[indice].niveau,mechas_joueur[indice].xp,mechas_joueur[indice].pv,
            mechas_joueur[indice].pv_max,mechas_joueur[indice].attaque,mechas_joueur[indice].defense,
            mechas_joueur[indice].vitesse,mechas_joueur[indice].attaque_1,mechas_joueur[indice].attaque_2);
        indice++;
    }
    //fermeture des fichiers
    fclose(file);   
    fclose(temp);
    //suppression de l'ancien et renomme le nouveau
    remove("../save/infomechas.csv");
    rename("../save/temporaire.csv", "../save/infomechas.csv");
    return OK;
}

int sauvegarde_partie(joueur_t *joueur, char pseudo[50]) { //Sauvegarde de la partie globale
    int trouver = 0;
    FILE *file = fopen("../save/joueur.csv", "r");          //Ouverture du joueur
    FILE *temp = fopen("../save/temporaire.csv", "w");      //Ouverture du futur fichier
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[50];
    char ligne[256];
    //traitement du csv - recopie chaque ligne dans le nouveau fichier
    fgets(ligne, sizeof(ligne), file);                      // Lire la ligne d'en-tête
    fprintf(temp,"%s",ligne);
    while(fgets(ligne, sizeof(ligne), file) != NULL){       
        sscanf(ligne, "%[^,]", nom);
        //verifie si la ligne est a modifié 
        if(!strcmp(nom,pseudo)){
            fprintf(temp,"%s,%c,%d,%d,%d\n",nom,joueur->sexe,joueur->x,
                                            joueur->y,joueur->pointSauvegarde);
            trouver = OK;       //indique que la ligne existe et a été modifié
        }
        else{
            fprintf(temp,"%s",ligne);       //recopie simplement
        }
    }
    if(!trouver){       //si pas trouver alors créé une nouvelle ligne de sauvegarde
        fprintf(temp,"%s,%c,%d,%d,%d\n",nom,joueur->sexe,joueur->x,
                                            joueur->y,joueur->pointSauvegarde);
    }

    //fermeture des fichiers
    fclose(file);   
    fclose(temp);
    //suppression de l'ancien fichier et renomme le nouveau
    remove("../save/joueur.csv");
    rename("../save/temporaire.csv", "../save/joueur.csv");

    sauvegarde_inventaire(joueur->inventaire,nom);                           //appel la sauvegarde de l'inventaire
    sauvegarde_mechas_joueur(joueur->mechas_joueur,nom,joueur->nb_mechas);    //appel la sauvegarde des mechas
    return OK;
}

//TEST

/*
 int main(void){
    
}*/