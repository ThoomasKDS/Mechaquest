/**
 * \file sauv.c
 * \brief fichier contenant les fonctions de sauvegarde et récupération
*/

#include "../lib/sauv.h"

//--------FONCTIONS D'INITIALISAION-----------

int init_partie(joueur_t *joueur, char pseudo[50],char sexe){
    strcpy(joueur->pseudo,pseudo);
    joueur->x = 23;
    joueur->y = 8;
    joueur->sexe = sexe;
    joueur->numMap = 0;
    joueur->pointSauvegarde = 0;
    joueur->nb_mechas = 0;
    joueur->inventaire = (inventaire_t *)malloc(sizeof(inventaire_t));
    if (!joueur->inventaire) {
        perror("Erreur d'allocation mémoire");
        return ERREUR_OUVERTURE;
    }
    joueur->inventaire->mechaball = 0;
    joueur->inventaire->carburant = 0;
    joueur->inventaire->repousse = 0;
    joueur->inventaire->rappel = 0;
    nouveau_fichier_pnj(pseudo);
    sauvegarde_partie(joueur,pseudo);
    return OK;
}

int nouveau_fichier_pnj(char pseudo[50]){
    
    char nom_fichier[60] = "save/pnj_";
    char ext[5] = ".csv";
    strcat(nom_fichier,pseudo);
    strcat(nom_fichier,ext);
    FILE *file = fopen("save/pnj.csv", "r");
    FILE *nouv = fopen(nom_fichier, "w");
    if (file == NULL || nouv == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }

    char ligne[1070];

    while (fgets(ligne, sizeof(ligne), file)) {
        fprintf(nouv, "%s", ligne);
    }

    fclose(file);
    fclose(nouv);

    // Remplace l'ancien fichier par le nouveau

    return OK;
}


//--------FONCTIONS DE RECUPERATION-----------

/**
 * \fn int recuperation_joueur
 * \brief fonction qui permet de récupérer tous les informations du joueur dans une structure du type joueur_t à partir de son pseudo. 
 * Si le pseudo n'existe pas dans le fichier joueur.csv, on en créra une nouvelle
 * \return retourne OK si tous marche, ERREUR_OUVERTURE si le fichier joueur.csv n'est pas trouver et ERR pour un autre problème.
*/
int recuperation_joueur(joueur_t *joueur, char pseudo[50]) {   //Recuperation de la sauvegarde joueur dans la structure joueur_t
    //Ouverture du fichier
    FILE *file = fopen("save/joueur.csv", "r");          
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
            sscanf(ligne, "%49[^,], %c,%d,%d,%d,%d\n",joueur->pseudo,&joueur->sexe,&joueur->numMap,&joueur->x,&joueur->y,&joueur->pointSauvegarde);
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

int recuperation_pnj(pnj_t *pnj,char pseudo[50]) {   //Recuperation de la sauvegarde joueur dans la structure joueur_t
    //Ouverture du fichier
    char nom_fichier[60] = "save/pnj_";
    char ext[5] = ".csv";
    strcat(nom_fichier,pseudo);
    strcat(nom_fichier,ext);
    FILE *file = fopen(nom_fichier, "r");
        
    if (file == NULL) {                                     
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    int count = 0;
    char ligne[1070];
    //traitement du csv
    fgets(ligne, sizeof(ligne), file);                       // Lire la ligne d'en-tête
    while(fgets(ligne, sizeof(ligne), file) != NULL){        //lecture de chaque ligne
            sscanf(ligne, "%d,%d,%49[^,],%d,%499[^,],%499[^,],%d,%d,%d\n",&pnj[count].id_pnj,&pnj[count].id_map,
            pnj[count].pseudo,&pnj[count].etat,pnj[count].dialogueDebut,pnj[count].dialogueFin,&pnj[count].x,&pnj[count].y,&pnj[count].orientation);
            //création du pointeur sur la structure inventaire
            pnj[count].inventaire = (inventaire_t *)malloc(sizeof(inventaire_t));
            if (!pnj[count].inventaire) {
                perror("Erreur d'allocation mémoire");
                return ERREUR_OUVERTURE;
            }

            // Récupération de l'inventaire
            recuperation_inventaire(pnj[count].inventaire, pnj[count].pseudo);
            //Récupérer les mechas du joueur
            pnj[count].nb_mechas = recuperation_mechas_joueur(pnj[count].mechas_joueur,pnj[count].pseudo);
            count++;
    }
    fclose(file);       //Aucune correspondance
    return OK;
}

int recuperation_inventaire(inventaire_t *inventaire, char pseudo[50]) { //Recuperation de l'inventaire dans la structure inventaire_t 
    //Ouverture du fichier
    FILE *file = fopen("save/inventaire.csv", "r");      
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
    FILE *file = fopen("save/infomechas.csv", "r");      
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
            sscanf(ligne, "%49[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",     //récupère les bonnes ligne et les rangent au bon endroit du tableau de mechas
                nom,&mechas_joueur[num-1].numero,&mechas_joueur[num-1].id_mechas,
                &mechas_joueur[num-1].niveau,&mechas_joueur[num-1].pv,&mechas_joueur[num-1].pv_max,
                &mechas_joueur[num-1].xp,&mechas_joueur[num-1].attaque,&mechas_joueur[num-1].defense,
                &mechas_joueur[num-1].vitesse,&mechas_joueur[num-1].attaque_1,&mechas_joueur[num-1].attaque_2,
                &mechas_joueur[num-1].utilisation_1,&mechas_joueur[num-1].utilisation_2);
            nb_mechas++;
        }
    }
    fclose(file);           //Aucune correspondance
    return nb_mechas;       //renvoies le nombres de mechas du joueur
}

int recuperation_attaques(attaque_t *attaques) {           //Recuperation des attaques
    //Ouverture du fichier
    FILE *file = fopen("save/attaques.csv", "r");        
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
    FILE *file = fopen("save/listeMechas.csv", "r");       
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
        sscanf(ligne, "%d,%49[^,],%19[^,],%d,%d,%199[^,],%[^\n]",
                                 &mechas_l[count].id_mechas,
                                 mechas_l[count].nom,
                                 mechas_l[count].type,
                                 &mechas_l[count].evolution,
                                 &mechas_l[count].niveau_evolution,
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

int recuperation_zone(zone_t *zone_l){           //Recuperation de tous les mechas
    int taille;
    //Ouverture du fichier
    FILE *file = fopen("save/zone.csv", "r");       
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier ");
        return ERREUR_OUVERTURE;
    }

   
    int count = 0;
    char MechasZone[30];
    char ligne[256];
    //traitement du csv
    fgets(ligne, sizeof(ligne), file);                      // Lire la première ligne (en-têtes)

    while (fgets(ligne, sizeof(ligne), file) != NULL){      //Lecture de chaque ligne
        sscanf(ligne, "%d,%d,%d,%d,%d,%d,%[^\n]",
                                 &zone_l[count].id_zone,
                                 &zone_l[count].NiveauMoyenApparition,
                                 &zone_l[count].PvMoyen,
                                 &zone_l[count].VitesseMoyenne,
                                 &zone_l[count].Attaque,
                                 &zone_l[count].Defense,
                                 MechasZone);                         //recupere les numero des attaques mais en chaines de caractere

        taille = 0;                                                 // Initialisation du compteur
        char *token = strtok(MechasZone, ";");                        // Découpe la première partie des attaques

        while (token != NULL) {
            zone_l[count].listeMechasZone[taille] = atoi(token);    // Convertit en entier et stocke
            taille++;                                               // Incrémente le compteur
            token = strtok(NULL, ";");                              // Passe au nombre suivant
        }
        zone_l[count].nb_mechas = taille;                       
        count++;
    }
        
    fclose(file);
    return count;                                                   //renvoies le nombre de mechas
}
//--------FONCTIONS DE SAUVEGARDE-----------

int sauvegarde_inventaire(inventaire_t *inventaire, char pseudo[50]) { //Sauvegarde de l'inventaire
    int trouver = 0;
    FILE *file = fopen("save/inventaire.csv", "r");      //Ouverture du fichier inventaire
    FILE *temp = fopen("save/temporaire.csv", "w");      //Ouverture du futur fichier
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
        fprintf(temp,"%s,%d,%d,%d,%d\n",pseudo,inventaire->mechaball,inventaire->carburant,
                                            inventaire->repousse,inventaire->rappel);
    }
    //fermeture des fichiers
    fclose(file);   
    fclose(temp);

    //supprime et renomme le nouveau fichier
    remove("save/inventaire.csv");
    rename("save/temporaire.csv", "save/inventaire.csv");
    return OK;
}
int sauvegarde_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[50],int nb_mechas) { //Sauvegarde les mechas du joueur
    int indice = 0;
    FILE *file = fopen("save/infomechas.csv", "r");      //Ouverture du fichier infomechas
    FILE *temp = fopen("save/temporaire.csv", "w");      //Ouverture du futur fichier
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
            fprintf(temp, "%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                nom,mechas_joueur[indice].numero,mechas_joueur[indice].id_mechas,
                mechas_joueur[indice].niveau,mechas_joueur[indice].pv,mechas_joueur[indice].pv_max,
                mechas_joueur[indice].xp,mechas_joueur[indice].attaque,mechas_joueur[indice].defense,
                mechas_joueur[indice].vitesse,mechas_joueur[indice].attaque_1,mechas_joueur[indice].attaque_2,
                mechas_joueur[indice].utilisation_1,mechas_joueur[indice].utilisation_2);

            indice++;
        }
        else{
            fprintf(temp,"%s",ligne);       //sinon recopie simplement 
        }
        
    }
    while(indice < nb_mechas){  //si les mechas sont pas trouvé créé la ligne de sauvegarde
        fprintf(temp, "%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
            pseudo,mechas_joueur[indice].numero,mechas_joueur[indice].id_mechas,
                mechas_joueur[indice].niveau,mechas_joueur[indice].pv,mechas_joueur[indice].pv_max,
                mechas_joueur[indice].xp,mechas_joueur[indice].attaque,mechas_joueur[indice].defense,
                mechas_joueur[indice].vitesse,mechas_joueur[indice].attaque_1,mechas_joueur[indice].attaque_2,
                mechas_joueur[indice].utilisation_1,mechas_joueur[indice].utilisation_2);
        indice++;
    }
    //fermeture des fichiers
    fclose(file);   
    fclose(temp);
    //suppression de l'ancien et renomme le nouveau
    remove("save/infomechas.csv");
    rename("save/temporaire.csv", "save/infomechas.csv");
    return OK;
}

int sauvegarde_partie(joueur_t *joueur, char pseudo[50]) { //Sauvegarde de la partie globale
    int trouver = 0;
    FILE *file = fopen("save/joueur.csv", "r");          //Ouverture du joueur
    FILE *temp = fopen("save/temporaire.csv", "w");      //Ouverture du futur fichier
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
            fprintf(temp,"%s,%c,%d,%d,%d,%d\n",nom,joueur->sexe,joueur->numMap,joueur->x,
                                            joueur->y,joueur->pointSauvegarde);
            trouver = OK;       //indique que la ligne existe et a été modifié
        }
        else{
            fprintf(temp,"%s",ligne);       //recopie simplement
        }
    }
    if(!trouver){       //si pas trouver alors créé une nouvelle ligne de sauvegarde
        fprintf(temp,"%s,%c,%d,%d,%d,%d\n",pseudo,joueur->sexe,joueur->numMap,joueur->x,
                                            joueur->y,joueur->pointSauvegarde);
    }

    //fermeture des fichiers
    fclose(file);   
    fclose(temp);
    //suppression de l'ancien fichier et renomme le nouveau
    remove("save/joueur.csv");
    rename("save/temporaire.csv", "save/joueur.csv");

    sauvegarde_inventaire(joueur->inventaire,pseudo);                           //appel la sauvegarde de l'inventaire
    sauvegarde_mechas_joueur(joueur->mechas_joueur,pseudo,joueur->nb_mechas);    //appel la sauvegarde des mechas
    return OK;
}

int sauvegarde_pnj(pnj_t *pnj, int id_pnj, char pseudo[50]) {
    int trouver = 0;  // Indique si la ligne a été trouvée et modifiée
    
    char nom_fichier[60] = "save/pnj_";
    char ext[5] = ".csv";
    strcat(nom_fichier,pseudo);
    strcat(nom_fichier,ext);
    FILE *file = fopen(nom_fichier, "r");
    FILE *temp = fopen("save/temp.csv", "w");
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }

    char ligne[1070];
    int num_pnj;

    // Copie l'en-tête
    if (fgets(ligne, sizeof(ligne), file)) {
        fprintf(temp, "%s", ligne);
    }

    // Parcourt le fichier ligne par ligne
    while (fgets(ligne, sizeof(ligne), file)) {
        if (sscanf(ligne, "%d", &num_pnj) == 1) {
            if (num_pnj == id_pnj) {
                // Modifie la ligne
                fprintf(temp, "%d,%d,%s,%d,%s,%s,%d,%d,%d\n",
                        id_pnj, pnj->id_map, pnj->pseudo, pnj->etat,
                        pnj->dialogueDebut, pnj->dialogueFin, pnj->x, pnj->y, pnj->orientation);
                trouver = 1;
            } else {
                // Recopie les autres lignes
                fprintf(temp, "%s", ligne);
            }
        } else {
            fprintf(temp, "%s", ligne);  // Ligne mal formée, recopiée telle quelle
        }
    }

    // Ajoute une nouvelle ligne si non trouvée
    if (!trouver) {
        fprintf(temp, "%d,%d,%s,%d,%s,%s,%d,%d,%d\n",
                id_pnj, pnj->id_map, pnj->pseudo, pnj->etat,
                pnj->dialogueDebut, pnj->dialogueFin, pnj->x, pnj->y, pnj->orientation);
    }

    fclose(file);
    fclose(temp);

    // Remplace l'ancien fichier par le nouveau
    remove(nom_fichier);
    rename("save/temp.csv", nom_fichier);

    return OK;
}


//FONCTIONS DE SUPPRESSION DE SAUVEGARDES

int suppression_inventaire(inventaire_t *inventaire, char pseudo[50]) { //Sauvegarde de l'inventaire
    FILE *file = fopen("save/inventaire.csv", "r");      //Ouverture du fichier inventaire
    FILE *temp = fopen("save/temporaire.csv", "w");      //Ouverture du futur fichier
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
        if(strcmp(nom,pseudo)){
            fprintf(temp,"%s",ligne);       //recopie simplement la ligne
        }
    }
    //fermeture des fichiers
    fclose(file);   
    fclose(temp);

    //supprime et renomme le nouveau fichier
    remove("save/inventaire.csv");
    rename("save/temporaire.csv", "save/inventaire.csv");
    return OK;
}
int suppression_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[50],int nb_mechas) { //Sauvegarde les mechas du joueur
    FILE *file = fopen("save/infomechas.csv", "r");      //Ouverture du fichier infomechas
    FILE *temp = fopen("save/temporaire.csv", "w");      //Ouverture du futur fichier
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[50];
    char ligne[256];
    //traitement du csv - recopie ligne par ligne du csv
    fgets(ligne, sizeof(ligne), file);                      // Lire la ligne d'en-tête
    fprintf(temp,"%s",ligne);
    while(fgets(ligne, sizeof(ligne), file) != NULL){       
        sscanf(ligne, "%[^,]", nom);
        //regarde si la ligne et a changer et la change
        if(strcmp(nom,pseudo)){
            fprintf(temp,"%s",ligne);       //sinon recopie simplement 
         } 
    }
    //fermeture des fichiers
    fclose(file);   
    fclose(temp);
    //suppression de l'ancien et renomme le nouveau
    remove("save/infomechas.csv");
    rename("save/temporaire.csv", "save/infomechas.csv");
    return OK;
}

int suppression_partie(joueur_t *joueur, char pseudo[50]) { //Sauvegarde de la partie globale
    FILE *file = fopen("save/joueur.csv", "r");          //Ouverture du joueur
    FILE *temp = fopen("save/temporaire.csv", "w");      //Ouverture du futur fichier
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
        if(strcmp(nom,pseudo)){
            fprintf(temp,"%s",ligne);       //recopie simplement
        }
    }
    //fermeture des fichiers
    fclose(file);   
    fclose(temp);
    //suppression de l'ancien fichier et renomme le nouveau
    remove("save/joueur.csv");
    rename("save/temporaire.csv", "save/joueur.csv");

    suppression_inventaire(joueur->inventaire,pseudo);                           //appel la sauvegarde de l'inventaire
    suppression_mechas_joueur(joueur->mechas_joueur,pseudo,joueur->nb_mechas);    //appel la sauvegarde des mechas
    char nom_fichier[60] = "save/pnj_";
    char ext[5] = ".csv";
    strcat(nom_fichier,pseudo);
    strcat(nom_fichier,ext);
    remove(nom_fichier);
    return OK;
}


//DESTRUCTION ALLOCATION DYNAMIQUE

int destruction_joueur(joueur_t *joueur){
    free(joueur->inventaire);
    joueur->inventaire = NULL;
    return OK;
}
int destruction_pnj(pnj_t *pnj){
    free(pnj->inventaire);
    pnj->inventaire = NULL;
    return OK;
}