#include "../lib/sauv.h"


int recuperation_joueur(joueur_t *joueur, char pseudo[50]) {   //Recuperation de la sauvegarde joueur dans la structure Joueur
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
            sscanf(ligne, "%49[^,], %c,%d,%d,%d\n",joueur->pseudo,&joueur->sexe,&joueur->x,&joueur->y,&joueur->pointSauvegarde);
            joueur->inventaire = (inventaire_t *)malloc(sizeof(inventaire_t));
            if (!joueur->inventaire) {
                perror("Erreur d'allocation mémoire");
                fclose(file);
                return ERREUR_OUVERTURE;
            }

            // Récupération de l'inventaire
            recuperation_inventaire(joueur->inventaire, joueur->pseudo);
            joueur->nb_mechas = recuperation_mchasJoueur(joueur->mechas_joueur,joueur->pseudo);
            fclose(file);
            return OK;
        }
    }
    fclose(file);       //Aucune correspondance
    return ERR;
}

int recuperation_inventaire(inventaire_t *inventaire, char pseudo[50]) { //Recuperation de l'inventaire dans la structure inventaire avec le pseudo associer
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

int recuperation_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[50]) { //Recuperation de l'inventaire dans la structure inventaire avec le pseudo associer
    int nb_mechas = 0;
    FILE *file = fopen("../save/infomechas.csv", "r");      //Ouverture du fichier
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    int num;
    char nom[50];
    char ligne[256];
    fgets(ligne, sizeof(ligne), file); // Lire la ligne d'en-tête
    while(fgets(ligne, sizeof(ligne), file) != NULL){       //Prendre chaque ligne, verifie que le pseudo corresponds et copie les données si correspondance
        sscanf(ligne, "%[^,],%d", nom,&num);
        if(!strcmp(nom,pseudo)){
            sscanf(ligne, "%49[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                nom,&mechas_joueur[num-1].numero,&mechas_joueur[num-1].id_mechas,
                &mechas_joueur[num-1].niveau,&mechas_joueur[num-1].xp,&mechas_joueur[num-1].pv,
                &mechas_joueur[num-1].pv_max,&mechas_joueur[num-1].attaque,&mechas_joueur[num-1].defense,
                &mechas_joueur[num-1].vitesse,&mechas_joueur[num-1].attaque_1,&mechas_joueur[num-1].attaque_2);
            nb_mechas++;
        }
    }
    fclose(file);   //Aucune correspondance
    return nb_mechas;
}

int recuperation_attaques(attaque_t *attaques) {           //Recuperation des attaques dans le tableau de structures d'Attaque
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

int recuperation_mechas(mechas_t *mechas_l) {           //Recuperation des attaques dans le tableau de structures d'Attaque
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
int sauvegarde_inventaire(inventaire_t *inventaire, char pseudo[50]) { //Recuperation de l'inventaire dans la structure inventaire avec le pseudo associer
    int trouver = 0;
    FILE *file = fopen("../save/inventaire.csv", "r");      //Ouverture du fichier
    FILE *temp = fopen("../save/temporaire.csv", "w");      //Ouverture du fichier
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[50];
    char ligne[256];
    fgets(ligne, sizeof(ligne), file); // Lire la ligne d'en-tête
    fprintf(temp,"%s",ligne);
    while(fgets(ligne, sizeof(ligne), file) != NULL){       //Prendre chaque ligne, verifie que le pseudo corresponds et copie les données si correspondance
        sscanf(ligne, "%[^,]", nom);
        if(!strcmp(nom,pseudo)){
            fprintf(temp,"%s,%d,%d,%d,%d\n",nom,inventaire->mechaball,inventaire->carburant,
                                            inventaire->repousse,inventaire->rappel);
            trouver = OK;
        }
        else{
            fprintf(temp,"%s",ligne);
        }
    }
    if(!trouver){
        fprintf(temp,"%s,%d,%d,%d,%d\n",nom,inventaire->mechaball,inventaire->carburant,
                                            inventaire->repousse,inventaire->rappel);
    }
    fclose(file);   
    fclose(temp);

    remove("../save/inventaire.csv");
    rename("../save/temporaire.csv", "../save/inventaire.csv");
    return OK;
}
int sauvegarde_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[50],int nb_mechas) { //Recuperation de l'inventaire dans la structure inventaire avec le pseudo associer
    int indice = 0;
    FILE *file = fopen("../save/infomechas.csv", "r");      //Ouverture du fichier
    FILE *temp = fopen("../save/temporaire.csv", "w");      //Ouverture du fichier
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[50];
    int num;
    char ligne[256];
    fgets(ligne, sizeof(ligne), file); // Lire la ligne d'en-tête
    fprintf(temp,"%s",ligne);
    while(fgets(ligne, sizeof(ligne), file) != NULL){       //Prendre chaque ligne, verifie que le pseudo corresponds et copie les données si correspondance
        sscanf(ligne, "%[^,],%d", nom,&num);
        if(!strcmp(nom,pseudo) && num == mechas_joueur[indice].numero){
            fprintf(temp, "%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                nom,mechas_joueur[indice].numero,mechas_joueur[indice].id_mechas,
                mechas_joueur[indice].niveau,mechas_joueur[indice].xp,mechas_joueur[indice].pv,
                mechas_joueur[indice].pv_max,mechas_joueur[indice].attaque,mechas_joueur[indice].defense,
                mechas_joueur[indice].vitesse,mechas_joueur[indice].attaque_1,mechas_joueur[indice].attaque_2);
            indice++;
        }
        else{
            fprintf(temp,"%s",ligne);
        }
        
    }
    while(indice < nb_mechas){
            fprintf(temp, "%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                nom,mechas_joueur[indice].numero,mechas_joueur[indice].id_mechas,
                mechas_joueur[indice].niveau,mechas_joueur[indice].xp,mechas_joueur[indice].pv,
                mechas_joueur[indice].pv_max,mechas_joueur[indice].attaque,mechas_joueur[indice].defense,
                mechas_joueur[indice].vitesse,mechas_joueur[indice].attaque_1,mechas_joueur[indice].attaque_2);
            indice++;
        }
    fclose(file);   
    fclose(temp);

    remove("../save/infomechas.csv");
    rename("../save/temporaire.csv", "../save/infomechas.csv");
    return OK;
}

int sauvegarde_partie(joueur_t *joueur, char pseudo[50]) { //Recuperation de l'inventaire dans la structure inventaire avec le pseudo associer
    int trouver = 0;
    FILE *file = fopen("../save/joueur.csv", "r");      //Ouverture du fichier
    FILE *temp = fopen("../save/temporaire.csv", "w");      //Ouverture du fichier
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[50];
    char ligne[256];
    fgets(ligne, sizeof(ligne), file); // Lire la ligne d'en-tête
    fprintf(temp,"%s",ligne);
    while(fgets(ligne, sizeof(ligne), file) != NULL){       //Prendre chaque ligne, verifie que le pseudo corresponds et copie les données si correspondance
        sscanf(ligne, "%[^,]", nom);
        if(!strcmp(nom,pseudo)){
            fprintf(temp,"%s,%c,%d,%d,%d\n",nom,joueur->sexe,joueur->x,
                                            joueur->y,joueur->pointSauvegarde);
            trouver = OK;
        }
        else{
            fprintf(temp,"%s",ligne);
        }
    }
    if(!trouver){
        fprintf(temp,"%s,%c,%d,%d,%d\n",nom,joueur->sexe,joueur->x,
                                            joueur->y,joueur->pointSauvegarde);
    }
    fclose(file);   
    fclose(temp);
    remove("../save/joueur.csv");
    rename("../save/temporaire.csv", "../save/joueur.csv");

    SauvegardeInventaire(joueur->inventaire,nom);
    SauvegardeMechasJoueur(joueur->mechas_joueur,nom,joueur->nb_mechas);
    return OK;
}

//TEST

/*
 int main(void){
    
}*/