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
            sscanf(ligne, "%49[^,], %c,%d,%d,%d\n",joueur->pseudo,&joueur->sexe,&joueur->x,&joueur->y,&joueur->pointSauvegarde);
            joueur->inventaire = (Inventaire *)malloc(sizeof(Inventaire));
            if (!joueur->inventaire) {
                perror("Erreur d'allocation mémoire");
                fclose(file);
                return ERREUR_OUVERTURE;
            }

            // Récupération de l'inventaire
            RecuperationInventaire(joueur->inventaire, joueur->pseudo);
            joueur->nb_mechas = RecuperationMechasJoueur(joueur->mechas_joueur,joueur->pseudo);
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
int SauvegardeInventaire(Inventaire *inventaire, char pseudo[50]) { //Recuperation de l'inventaire dans la structure inventaire avec le pseudo associer
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
int SauvegardeMechasJoueur(Mechas_Joueur * mechas_joueur,char pseudo[50],int nb_mechas) { //Recuperation de l'inventaire dans la structure inventaire avec le pseudo associer
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

//TEST

/*
 int main(void){
    
    Attaque attaques[50];
    int nb = RecuperationAttaques(attaques);
    char pseudo[50] = "noaha";
    
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
    Joueur joueur;
    char pseudo[50] = "Noah";
    RecuperationJoueur(&joueur,pseudo);
    printf("pseudo: %s   sexe: %c\n",joueur.pseudo,joueur.sexe);
    for(int i = 0;i<joueur.nb_mechas;i++){
        printf("num: %d    id: %d   niveau: %d   xp: %d\n",joueur.mechas_joueur[i].numero,joueur.mechas_joueur[i].id_mechas,joueur.mechas_joueur[i].niveau, joueur.mechas_joueur[i].xp);
    }
    Joueur j1;
    char nom[50] = "noaha";
    int  n = RecuperationJoueur(&j1,nom);
    Mechas_Joueur mechas5 ={5,12,12,14,54,54,5,6,2,1,2};
    printf("%d",n);
    j1.mechas_joueur[4] = mechas5;
    j1.nb_mechas++;
    for(int i = 0; i< j1.nb_mechas;i++){
        printf("num: %d    id: %d   pv: %d   xp: %d\n",j1.mechas_joueur[i].numero,j1.mechas_joueur[i].id_mechas,j1.mechas_joueur[i].pv, j1.mechas_joueur[i].xp);
        j1.mechas_joueur[i].pv = -1;
    }
    SauvegardeMechasJoueur(j1.mechas_joueur,nom,j1.nb_mechas);
}
*/