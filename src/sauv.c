/**
 * \file sauv.c
 * \brief fichier contenant les fonctions de sauvegarde et récupération
*/

#include "../lib/sauv.h"

//--------FONCTIONS D'INITIALISAION-----------
/**
 * @brief Initialise une nouvelle partie avec les paramètres par défaut pour un joueur.
 *
 * Cette fonction configure une nouvelle partie en initialisant la structure du joueur :
 * - Définition du pseudo.
 * - Position initiale (`x`, `y`) sur la carte de départ.
 * - Sexe du personnage (utilisé pour l'apparence graphique).
 * - Numéro de la carte initiale (`numMap`).
 * - Point de sauvegarde initialisé à 0.
 * - Initialisation à zéro du nombre de Mechas possédés.
 * - Allocation dynamique de l'inventaire et initialisation des objets à zéro.
 *
 * La fonction crée également un nouveau fichier de sauvegarde spécifique aux PNJs liés au pseudo du joueur,
 * puis sauvegarde automatiquement l'état initial de la partie.
 *
 * @param joueur Pointeur vers la structure du joueur à initialiser.
 * @param pseudo Chaîne de caractères représentant le pseudo du joueur (longueur maximale définie par `LONGUEUR_MAX_PSEUDO`).
 * @param sexe   Caractère représentant le sexe du personnage ('M' ou 'F'), utilisé pour l'apparence du joueur.
 *
 * @return int
 *         - `OK` si l'initialisation s'est correctement déroulée.
 *         - `ERREUR_OUVERTURE` en cas d'échec lors de l'allocation dynamique de l'inventaire.
 *
 * @pre Le pointeur `joueur` doit être valide et non-null.
 *      Les constantes `LONGUEUR_MAX_PSEUDO`, `OK`, et `ERREUR_OUVERTURE` doivent être définies.
 *
 * @post Une nouvelle partie est initialisée, les ressources sont allouées dynamiquement, 
 *       et l'état du jeu est sauvegardé.
 *
 * @note Assure-toi que les fonctions appelées (`nouveau_fichier_pnj`, `sauvegarde_partie`) soient implémentées correctement.
 *
 * @warning L'appel à cette fonction entraîne une allocation dynamique (`malloc`) pour l'inventaire.
 *          La mémoire ainsi allouée devra être libérée (`free`) en fin d'utilisation pour éviter toute fuite mémoire.
 */
int init_partie(joueur_t *joueur, char pseudo[LONGUEUR_MAX_PSEUDO],char sexe){
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

/**
 * @brief Crée un nouveau fichier personnalisé des PNJs basé sur un modèle initial.
 *
 * Cette fonction génère un nouveau fichier de sauvegarde personnalisé pour les PNJs en copiant intégralement
 * le contenu du fichier modèle (`save/pnj.csv`). Le nouveau fichier créé est spécifique au joueur et nommé
 * selon le format suivant : `save/pnj_[pseudo].csv`.
 *
 * Ce mécanisme permet à chaque joueur de disposer de sa propre sauvegarde des interactions et des états des PNJs,
 * indépendamment des autres parties ou profils existants.
 *
 * @param pseudo Chaîne de caractères représentant le pseudo du joueur (longueur maximale définie par `LONGUEUR_MAX_PSEUDO`).
 *
 * @return int
 *         - `OK` si la création et la copie du fichier se sont déroulées avec succès.
 *         - `ERREUR_OUVERTURE` en cas de problème d'ouverture des fichiers (lecture ou écriture).
 *
 * @pre Le dossier `save` doit exister et être accessible en écriture.
 *      Le fichier source `save/pnj.csv` doit exister et être lisible.
 *      Les constantes (`LONGUEUR_MAX_PSEUDO`, `LONGUEUR_LIGNE_DESC`, `OK`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post Un nouveau fichier personnalisé pour le joueur est créé dans le dossier `save`.
 *
 * @note Assure-toi de vérifier que le pseudo ne contient pas de caractères spéciaux pouvant perturber le nom du fichier.
 *
 * @warning Pense à gérer correctement les droits d'accès au dossier de sauvegarde et aux fichiers, ainsi qu'à gérer les erreurs éventuelles.
 */
int nouveau_fichier_pnj(char pseudo[LONGUEUR_MAX_PSEUDO]){
    
    char nom_fichier[LONGUEUR_MAX_PSEUDO+10] = "save/pnj_";
    char ext[5] = ".csv";
    strcat(nom_fichier,pseudo);
    strcat(nom_fichier,ext);
    FILE *file = fopen("save/pnj.csv", "r");
    FILE *nouv = fopen(nom_fichier, "w");
    if (file == NULL || nouv == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }

    char ligne[LONGUEUR_LIGNE_DESC];

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
 * @brief Récupère les données d'une partie sauvegardée pour un joueur donné depuis un fichier CSV.
 *
 * Cette fonction recherche dans le fichier de sauvegarde `save/joueur.csv` les données associées au pseudo spécifié.
 * Si le pseudo est trouvé, la fonction charge automatiquement :
 * - Les informations principales du joueur (pseudo, sexe, carte actuelle, position, point de sauvegarde).
 * - L'inventaire du joueur (objets possédés).
 * - Les Mechas du joueur (équipe actuelle).
 *
 * L'inventaire est alloué dynamiquement lors de la récupération. Les fonctions `recuperation_inventaire` et
 * `recuperation_mechas_joueur` sont appelées pour charger les données détaillées correspondantes.
 *
 * @param joueur Pointeur vers la structure joueur_t qui sera remplie avec les données récupérées.
 * @param pseudo Chaîne de caractères indiquant le pseudo du joueur dont les données doivent être récupérées.
 *
 * @return int
 *         - `OK` si les données ont été récupérées avec succès.
 *         - `ERREUR_OUVERTURE` si une erreur d'ouverture du fichier ou d'allocation mémoire survient.
 *         - `ERR` si le pseudo n'est pas trouvé dans le fichier.
 *
 * @pre Le fichier `save/joueur.csv` doit exister et être lisible.
 *      Les fonctions `recuperation_inventaire` et `recuperation_mechas_joueur` doivent être implémentées et fonctionnelles.
 *      Les constantes (`LONGUEUR_MAX_PSEUDO`, `LONGUEUR_LIGNE`, `OK`, `ERR`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post La structure `joueur` est initialisée avec les données récupérées en cas de succès.
 *
 * @note La mémoire allouée dynamiquement pour l'inventaire devra être libérée explicitement à la fin de son utilisation pour éviter les fuites mémoire.
 *
 * @warning Vérifie les fichiers CSV et les fonctions annexes pour éviter les incohérences de données.
 */
int recuperation_joueur(joueur_t *joueur, char pseudo[LONGUEUR_MAX_PSEUDO]) { 
    //Ouverture du fichier
    FILE *file = fopen("save/joueur.csv", "r");          
    if (file == NULL) {                                     
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[LONGUEUR_MAX_PSEUDO];
    char ligne[LONGUEUR_LIGNE];
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

/**
 * @brief Récupère l'état et les données détaillées des PNJs à partir d'un fichier CSV personnalisé.
 *
 * Cette fonction lit un fichier CSV spécifique aux PNJs, généré précédemment pour un joueur particulier, 
 * dont le nom est formaté ainsi : `save/pnj_[pseudo].csv`.
 * Elle charge dans un tableau de structures `pnj_t` :
 * - L'identifiant unique (`id_pnj`).
 * - La carte sur laquelle se situe le PNJ (`id_map`).
 * - Le pseudo ou nom affiché du PNJ.
 * - L'état actuel du PNJ (interaction réalisée ou non).
 * - Les dialogues de début et de fin d'interaction.
 * - La position (`x`, `y`) et l'orientation sur la carte.
 * - L'inventaire du PNJ.
 * - L'équipe de Mechas du PNJ.
 *
 * L'inventaire de chaque PNJ est alloué dynamiquement, et les Mechas ainsi que les objets sont chargés à l'aide 
 * des fonctions `recuperation_inventaire` et `recuperation_mechas_joueur`.
 *
 * @param pnj    Tableau de structures PNJ (`pnj_t`) dans lequel les données seront chargées.
 * @param pseudo Chaîne de caractères représentant le pseudo du joueur utilisé pour charger son fichier personnalisé.
 *
 * @return int
 *         - `OK` si la récupération s'effectue correctement.
 *         - `ERREUR_OUVERTURE` si une erreur d'ouverture du fichier ou une erreur d'allocation mémoire survient.
 *
 * @pre Le fichier personnalisé (`save/pnj_[pseudo].csv`) doit exister et être lisible.
 *      Les fonctions annexes (`recuperation_inventaire`, `recuperation_mechas_joueur`) doivent être valides et fonctionnelles.
 *      Les constantes (`LONGUEUR_MAX_PSEUDO`, `LONGUEUR_LIGNE_DESC`, `OK`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post Le tableau `pnj` est rempli avec toutes les informations récupérées.
 *
 * @note La mémoire dynamiquement allouée pour l'inventaire de chaque PNJ devra être libérée explicitement en fin d'utilisation pour éviter les fuites mémoire.
 *
 * @warning Vérifie la validité des fichiers CSV et des fonctions associées pour éviter les incohérences et problèmes de chargement.
 */
int recuperation_pnj(pnj_t *pnj,char pseudo[50]) {
    //Ouverture du fichier
    char nom_fichier[LONGUEUR_MAX_PSEUDO+10] = "save/pnj_";
    char ext[5] = ".csv";
    strcat(nom_fichier,pseudo);
    strcat(nom_fichier,ext);
    FILE *file = fopen(nom_fichier, "r");
        
    if (file == NULL) {                                     
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    int count = 0;
    char ligne[LONGUEUR_LIGNE_DESC];
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

/**
 * @brief Récupère les données de l'inventaire d'un joueur ou d'un PNJ depuis un fichier CSV.
 *
 * Cette fonction lit le fichier `save/inventaire.csv` à la recherche de l'entrée correspondant 
 * au pseudo spécifié. Si l'entrée est trouvée, elle charge dans la structure d'inventaire :
 * - Le nombre de mechaballs disponibles.
 * - La quantité de carburant.
 * - Le nombre d'objets repousse disponibles.
 * - Le nombre d'objets rappel disponibles.
 *
 * Les données récupérées sont stockées directement dans la structure inventaire passée en paramètre.
 *
 * @param inventaire Pointeur vers la structure `inventaire_t` qui sera remplie avec les données récupérées.
 * @param pseudo     Chaîne de caractères représentant le pseudo du joueur ou du PNJ concerné par la récupération.
 *
 * @return int
 *         - `OK` si l'inventaire a été récupéré avec succès.
 *         - `ERREUR_OUVERTURE` si une erreur d'ouverture du fichier survient.
 *         - `ERR` si le pseudo recherché n'est pas trouvé dans le fichier.
 *
 * @pre Le fichier `save/inventaire.csv` doit exister, être correctement formaté, et être lisible.
 *      Les constantes (`LONGUEUR_MAX_PSEUDO`, `LONGUEUR_LIGNE`, `OK`, `ERR`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post La structure inventaire est initialisée avec les quantités des objets récupérées en cas de succès.
 *
 * @note Cette fonction peut être utilisée aussi bien pour le joueur que pour les PNJs.
 *
 * @warning Vérifie la cohérence des données stockées dans le fichier CSV pour éviter les erreurs d’initialisation.
 */
int recuperation_inventaire(inventaire_t *inventaire, char pseudo[LONGUEUR_MAX_PSEUDO]) {
    //Ouverture du fichier
    FILE *file = fopen("save/inventaire.csv", "r");      
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }

    char nom[LONGUEUR_MAX_PSEUDO];
    char ligne[LONGUEUR_LIGNE];
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

/**
 * @brief Récupère l'équipe de Mechas d'un joueur à partir d'un fichier CSV.
 *
 * Cette fonction lit le fichier `save/infomechas.csv` et récupère tous les Mechas appartenant au joueur spécifié.
 * Chaque ligne correspondant au pseudo recherché est analysée et les données des Mechas suivants sont chargées :
 * - Numéro du Mecha dans l'équipe du joueur.
 * - Identifiant du Mecha.
 * - Niveau actuel.
 * - Points de vie actuels et maximum.
 * - Expérience actuelle.
 * - Statistiques d'attaque, de défense et de vitesse.
 * - Identifiants des attaques disponibles (attaque 1 et attaque 2).
 * - Nombre d'utilisations restantes pour chaque attaque.
 *
 * Les Mechas récupérés sont placés directement dans le tableau fourni en paramètre, à leur position spécifique selon leur numéro d'équipe.
 *
 * @param mechas_joueur Tableau de structures `mechas_joueur_t` à remplir avec les données récupérées.
 * @param pseudo        Chaîne de caractères représentant le pseudo du joueur concerné par la récupération.
 *
 * @return int
 *         - Nombre de Mechas récupérés avec succès (≥ 0).
 *         - `ERREUR_OUVERTURE` en cas d'échec lors de l'ouverture du fichier.
 *
 * @pre Le fichier `save/infomechas.csv` doit exister, être correctement formaté, et lisible.
 *      Les constantes (`LONGUEUR_MAX_PSEUDO`, `LONGUEUR_LIGNE`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post Le tableau `mechas_joueur` est initialisé avec les Mechas du joueur. La fonction retourne leur nombre.
 *
 * @note Vérifie que les indices (`num`) utilisés dans le fichier CSV respectent bien les limites du tableau `mechas_joueur`.
 *
 * @warning Assure-toi de gérer les éventuelles incohérences des données (par exemple, index hors tableau).
 */
int recuperation_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[50]) {
    int nb_mechas = 0;  
    //Ouverture du fichier
    FILE *file = fopen("save/infomechas.csv", "r");      
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    int num;
    char nom[LONGUEUR_MAX_PSEUDO];
    char ligne[LONGUEUR_LIGNE];
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

/**
 * @brief Récupère les informations complètes des attaques disponibles depuis un fichier CSV.
 *
 * Cette fonction lit le fichier `save/attaques.csv` contenant la liste des attaques disponibles dans le jeu.
 * Chaque attaque récupérée comprend les données suivantes :
 * - Identifiant unique de l'attaque (`id_attaques`).
 * - Nom de l'attaque.
 * - Type de l'attaque (par exemple : feu, eau, etc.).
 * - Niveau requis pour apprendre/utiliser l'attaque.
 * - Dégâts infligés par l'attaque.
 * - Pourcentage de précision de l'attaque.
 * - Nombre d'utilisations disponibles pour l'attaque (PP).
 *
 * Les attaques sont stockées directement dans le tableau passé en paramètre.
 *
 * @param attaques Tableau de structures `attaque_t` à remplir avec les données récupérées.
 *
 * @return int
 *         - Nombre total d'attaques récupérées avec succès (≥ 0).
 *         - `ERREUR_OUVERTURE` en cas d'échec lors de l'ouverture du fichier.
 *
 * @pre Le fichier `save/attaques.csv` doit exister, être correctement formaté, et être lisible.
 *      Les constantes (`LONGUEUR_LIGNE`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post Le tableau `attaques` est initialisé avec toutes les données récupérées.
 *
 * @note Vérifie la capacité maximale du tableau `attaques` afin d’éviter un débordement en cas d'un grand nombre d'attaques.
 *
 * @warning Assure-toi de gérer explicitement les erreurs liées à la lecture du fichier (format incorrect, dépassement du tableau, etc.).
 */
int recuperation_attaques(attaque_t *attaques) {
    //Ouverture du fichier
    FILE *file = fopen("save/attaques.csv", "r");        
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    int count = 0;
    char ligne[LONGUEUR_LIGNE_DESC];
    //traitement du csv
    fgets(ligne, sizeof(ligne), file);                      // Lire la première ligne (en-têtes)

    while (fgets(ligne, sizeof(ligne), file) != NULL){      //Lecture de chaque ligne
        sscanf(ligne, "%d,%49[^,],%19[^,],%d,%d,%d,%d,%199[^,]\n",
                                 &attaques[count].id_attaques,
                                 attaques[count].nom,
                                 attaques[count].type,
                                 &attaques[count].niveau,
                                 &attaques[count].degats,
                                 &attaques[count].precision,
                                 &attaques[count].utilisations,
                                 attaques[count].description) ;
        count++;
    }
        

    fclose(file);
    return count;       //renvoies le nombre d'attqaues
}

/**
 * @brief Récupère la liste complète des Mechas disponibles et leurs caractéristiques à partir d'un fichier CSV.
 *
 * Cette fonction lit le fichier `save/listeMechas.csv` qui contient toutes les informations relatives aux Mechas existants dans le jeu.
 * Pour chaque Mecha, les données suivantes sont récupérées et stockées :
 * - Identifiant unique (`id_mechas`).
 * - Nom du Mecha.
 * - Type du Mecha.
 * - État d'évolution (identifiant du Mecha évolué, ou 0 si aucune évolution).
 * - Niveau requis pour évoluer (si applicable).
 * - Description textuelle du Mecha.
 * - Liste des identifiants d'attaques apprises par le Mecha, récupérés sous forme de chaîne puis convertis en entiers.
 * - Nombre total d'attaques disponibles pour ce Mecha.
 *
 * Chaque attaque est stockée dans un tableau spécifique à chaque Mecha.
 *
 * @param mechas_l Tableau de structures `mechas_t` à remplir avec les données récupérées.
 *
 * @return int
 *         - Nombre total de Mechas récupérés avec succès (≥ 0).
 *         - `ERREUR_OUVERTURE` en cas d'échec lors de l'ouverture du fichier.
 *
 * @pre Le fichier `save/listeMechas.csv` doit exister, être correctement formaté et lisible.
 *      Les constantes (`LONGUEUR_LIGNE`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post Le tableau `mechas_l` est initialisé avec les données de chaque Mecha récupéré.
 *
 * @note Assure-toi que le tableau `liste_attaque` de chaque Mecha est suffisamment grand pour accueillir toutes les attaques listées.
 *
 * @warning Vérifie le format des attaques (séparées par des points-virgules) dans le CSV afin d'éviter toute erreur de conversion ou dépassement.
 */
int recuperation_mechas(mechas_t *mechas_l) {
    int taille;
    //Ouverture du fichier
    FILE *file = fopen("save/listeMechas.csv", "r");       
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }

   
    int count = 0;
    char attaques[20];
    char ligne[LONGUEUR_LIGNE];
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

/**
 * @brief Récupère les données détaillées des zones de jeu depuis un fichier CSV.
 *
 * Cette fonction lit le fichier `save/zone.csv` contenant les informations sur les différentes zones du jeu.  
 * Pour chaque zone, les données suivantes sont chargées :
 * - Identifiant unique de la zone (`id_zone`).
 * - Niveau moyen d'apparition des Mechas sauvages.
 * - Points de vie (PV) moyens des Mechas sauvages.
 * - Vitesse moyenne des Mechas sauvages.
 * - Attaque moyenne des Mechas sauvages.
 * - Défense moyenne des Mechas sauvages.
 * - Liste des identifiants des Mechas pouvant apparaître dans cette zone, récupérée sous forme de chaîne puis convertie en entiers.
 * - Nombre total de Mechas différents pouvant apparaître dans cette zone.
 *
 * Chaque identifiant de Mecha est stocké dans un tableau spécifique à chaque zone.
 *
 * @param zone_l Tableau de structures `zone_t` à remplir avec les données récupérées.
 *
 * @return int
 *         - Nombre total de zones récupérées avec succès (≥ 0).
 *         - `ERREUR_OUVERTURE` en cas d'échec lors de l'ouverture du fichier.
 *
 * @pre Le fichier `save/zone.csv` doit exister, être correctement formaté et lisible.
 *      Les constantes (`LONGUEUR_LIGNE`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post Le tableau `zone_l` est initialisé avec les données de chaque zone récupérée.
 *
 * @note Vérifie que le tableau `listeMechasZone` est suffisamment grand pour accueillir tous les identifiants des Mechas listés.
 *
 * @warning Vérifie le format des identifiants de Mechas (séparés par des points-virgules) dans le CSV afin d'éviter des erreurs de conversion ou débordement.
 */
int recuperation_zone(zone_t *zone_l){          
    int taille;
    //Ouverture du fichier
    FILE *file = fopen("save/zone.csv", "r");       
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier ");
        return ERREUR_OUVERTURE;
    }

   
    int count = 0;
    char MechasZone[20];
    char ligne[LONGUEUR_LIGNE];
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

/**
 * @brief Sauvegarde les données de l'inventaire d'un joueur (ou PNJ) dans un fichier CSV.
 *
 * Cette fonction met à jour ou ajoute les informations d'un inventaire spécifique (quantités d'objets)
 * dans le fichier `save/inventaire.csv`. Si une entrée correspondante au pseudo spécifié existe déjà,
 * elle est mise à jour avec les nouvelles valeurs fournies. Si aucune entrée n'existe, elle sera ajoutée
 * à la fin du fichier.
 *
 * Les objets sauvegardés incluent :
 * - Mechaballs.
 * - Carburant.
 * - Repousses.
 * - Rappels.
 *
 * @param inventaire Pointeur vers la structure d'inventaire (`inventaire_t`) à sauvegarder.
 * @param pseudo     Chaîne de caractères représentant le pseudo du joueur ou du PNJ associé à cet inventaire.
 *
 * @return int
 *         - `OK` si l'inventaire est sauvegardé avec succès.
 *         - `ERREUR_OUVERTURE` en cas d'erreur d'ouverture des fichiers.
 *
 * @pre Les fichiers doivent être accessibles en lecture/écriture dans le dossier `save`.
 *      Les constantes (`LONGUEUR_MAX_PSEUDO`, `LONGUEUR_LIGNE`, `OK`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post Le fichier CSV est mis à jour avec les données actuelles de l'inventaire fourni.
 *
 * @note Cette fonction utilise un fichier temporaire pour éviter la corruption des données en cas de problème pendant la sauvegarde.
 *
 * @warning Assure-toi que le dossier `save` existe et que les fichiers sont accessibles en lecture et écriture.
 */
int sauvegarde_inventaire(inventaire_t *inventaire, char pseudo[LONGUEUR_MAX_PSEUDO]) {
    int trouver = 0;
    FILE *file = fopen("save/inventaire.csv", "r");      //Ouverture du fichier inventaire
    FILE *temp = fopen("save/temporaire.csv", "w");      //Ouverture du futur fichier
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[LONGUEUR_MAX_PSEUDO];
    char ligne[LONGUEUR_LIGNE];
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

/**
 * @brief Sauvegarde l'équipe de Mechas d'un joueur dans un fichier CSV.
 *
 * Cette fonction met à jour ou ajoute les informations détaillées des Mechas appartenant à un joueur
 * dans le fichier `save/infomechas.csv`. Pour chaque Mecha, les informations sauvegardées incluent :
 * - Numéro du Mecha dans l'équipe.
 * - Identifiant du Mecha.
 * - Niveau actuel.
 * - Points de vie actuels et maximum.
 * - Expérience accumulée.
 * - Statistiques : attaque, défense et vitesse.
 * - Identifiants des deux attaques disponibles.
 * - Nombre d'utilisations restantes pour chaque attaque.
 *
 * Si une entrée correspondante à un Mecha précis existe déjà, elle est mise à jour avec les nouvelles valeurs.
 * Sinon, une nouvelle entrée sera créée pour chaque Mecha manquant.
 *
 * @param mechas_joueur Tableau de structures `mechas_joueur_t` contenant les données des Mechas à sauvegarder.
 * @param pseudo        Chaîne de caractères représentant le pseudo du joueur concerné par cette sauvegarde.
 * @param nb_mechas     Nombre de Mechas actuellement dans l'équipe du joueur.
 *
 * @return int
 *         - `OK` si les Mechas ont été sauvegardés avec succès.
 *         - `ERREUR_OUVERTURE` en cas d'erreur d'ouverture des fichiers.
 *
 * @pre Les fichiers doivent être accessibles en lecture et écriture dans le dossier `save`.
 *      Les constantes (`LONGUEUR_MAX_PSEUDO`, `LONGUEUR_LIGNE`, `OK`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post Le fichier CSV est mis à jour avec les données actuelles de chaque Mecha de l'équipe du joueur.
 *
 * @note Cette fonction crée un fichier temporaire pour garantir l'intégrité des données lors de la sauvegarde.
 *
 * @warning Vérifie que le dossier `save` existe et que les fichiers nécessaires sont accessibles.
 */
int sauvegarde_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[LONGUEUR_MAX_PSEUDO],int nb_mechas) {
    int indice = 0;
    FILE *file = fopen("save/infomechas.csv", "r");      //Ouverture du fichier infomechas
    FILE *temp = fopen("save/temporaire.csv", "w");      //Ouverture du futur fichier
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[LONGUEUR_MAX_PSEUDO];
    int num;
    char ligne[LONGUEUR_LIGNE];
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

/**
 * @brief Sauvegarde l'état global d'une partie d'un joueur dans plusieurs fichiers CSV.
 *
 * Cette fonction sauvegarde les informations principales du joueur, telles que :
 * - Pseudo du joueur.
 * - Sexe du personnage.
 * - Carte actuelle (`numMap`).
 * - Position (`x`, `y`) actuelle du joueur sur la carte.
 * - Dernier point de sauvegarde atteint.
 *
 * Les informations sont sauvegardées dans le fichier `save/joueur.csv`.  
 * Si une sauvegarde correspondant au pseudo du joueur existe déjà, elle est mise à jour ;  
 * sinon, une nouvelle entrée est créée.
 *
 * La fonction sauvegarde également automatiquement :
 * - L'inventaire du joueur (objets possédés) via la fonction `sauvegarde_inventaire`.
 * - L'équipe actuelle de Mechas du joueur via la fonction `sauvegarde_mechas_joueur`.
 *
 * Ces informations sont respectivement enregistrées dans les fichiers :
 * - `save/inventaire.csv`
 * - `save/infomechas.csv`
 *
 * @param joueur Pointeur vers la structure joueur_t contenant toutes les données à sauvegarder.
 * @param pseudo Chaîne de caractères représentant le pseudo du joueur concerné par cette sauvegarde.
 *
 * @return int
 *         - `OK` si la sauvegarde complète s'est déroulée correctement.
 *         - `ERREUR_OUVERTURE` en cas d'erreur lors de l'ouverture des fichiers.
 *
 * @pre Les fichiers CSV doivent être accessibles en lecture et écriture dans le dossier `save`.
 *      Les fonctions `sauvegarde_inventaire` et `sauvegarde_mechas_joueur` doivent être valides.
 *      Les constantes (`LONGUEUR_MAX_PSEUDO`, `LONGUEUR_LIGNE`, `OK`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post Tous les fichiers CSV concernés sont mis à jour avec les informations actuelles du joueur.
 *
 * @note Un fichier temporaire est utilisé pour garantir l'intégrité des données pendant la sauvegarde.
 *
 * @warning Vérifie la disponibilité des fichiers CSV nécessaires et le bon fonctionnement des fonctions annexes pour éviter toute perte de données.
 */
int sauvegarde_partie(joueur_t *joueur, char pseudo[LONGUEUR_MAX_PSEUDO]) { 
    int trouver = 0;
    FILE *file = fopen("save/joueur.csv", "r");          //Ouverture du joueur
    FILE *temp = fopen("save/temporaire.csv", "w");      //Ouverture du futur fichier
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[LONGUEUR_MAX_PSEUDO];
    char ligne[LONGUEUR_LIGNE];
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

/**
 * @brief Sauvegarde ou met à jour les données d'un PNJ spécifique pour un joueur dans un fichier CSV personnalisé.
 *
 * Cette fonction sauvegarde les données associées à un PNJ particulier (identifié par son ID) dans un fichier CSV dédié
 * au joueur concerné (`save/pnj_[pseudo].csv`). Si l'entrée correspondant à l'ID du PNJ existe déjà, les informations suivantes
 * seront mises à jour :
 * - ID du PNJ.
 * - Identifiant de la carte où se trouve le PNJ.
 * - Pseudo du PNJ.
 * - État d'interaction (non rencontré, rencontré, combat terminé, etc.).
 * - Dialogues de début et de fin d'interaction.
 * - Position (`x`, `y`) et orientation du PNJ sur la carte.
 *
 * Si l'entrée du PNJ n'existe pas, elle sera ajoutée au fichier.
 *
 * @param pnj     Pointeur vers la structure `pnj_t` contenant les données actuelles du PNJ à sauvegarder.
 * @param id_pnj  Identifiant unique du PNJ à sauvegarder ou à mettre à jour.
 * @param pseudo  Chaîne de caractères représentant le pseudo du joueur associé à cette sauvegarde.
 *
 * @return int
 *         - `OK` si la sauvegarde ou la mise à jour a été réalisée avec succès.
 *         - `ERREUR_OUVERTURE` si une erreur survient lors de l'ouverture des fichiers.
 *
 * @pre Le dossier `save` doit exister et être accessible en lecture/écriture.
 *      Le fichier CSV personnalisé (`save/pnj_[pseudo].csv`) doit être accessible (ou sera créé si inexistant).
 *      Les constantes (`LONGUEUR_MAX_PSEUDO`, `LONGUEUR_LIGNE_DESC`, `OK`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post Le fichier CSV spécifique au joueur est mis à jour ou créé avec les informations actuelles du PNJ.
 *
 * @note Utilise un fichier temporaire pour assurer l'intégrité des données lors de la sauvegarde.
 *
 * @warning Vérifie que le pseudo du joueur ne comporte aucun caractère spécial incompatible avec le système de fichiers.
 */
int sauvegarde_pnj(pnj_t *pnj, int id_pnj, char pseudo[LONGUEUR_MAX_PSEUDO]) {
    int trouver = 0;  // Indique si la ligne a été trouvée et modifiée
    
    char nom_fichier[LONGUEUR_MAX_PSEUDO+10] = "save/pnj_";
    char ext[5] = ".csv";
    strcat(nom_fichier,pseudo);
    strcat(nom_fichier,ext);
    FILE *file = fopen(nom_fichier, "r");
    FILE *temp = fopen("save/temp.csv", "w");
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }

    char ligne[LONGUEUR_LIGNE_DESC];
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
/**
 * @brief Supprime l'entrée d'un inventaire associé à un joueur (ou PNJ) d'un fichier CSV.
 *
 * Cette fonction supprime définitivement les données d'inventaire correspondant au pseudo spécifié
 * du fichier `save/inventaire.csv`. Elle crée un fichier temporaire, recopie toutes les entrées sauf
 * celle associée au pseudo fourni, puis remplace l'ancien fichier par ce fichier temporaire.
 *
 * Cette opération est utile pour nettoyer les données associées à un joueur supprimé ou réinitialisé.
 *
 * @param inventaire Pointeur vers une structure d'inventaire (non utilisée ici, conservée pour compatibilité).
 * @param pseudo     Chaîne de caractères représentant le pseudo du joueur ou du PNJ à supprimer de l'inventaire.
 *
 * @return int
 *         - `OK` si l'entrée de l'inventaire a été supprimée avec succès.
 *         - `ERREUR_OUVERTURE` en cas d'erreur lors de l'ouverture des fichiers.
 *
 * @pre Les fichiers CSV doivent être accessibles en lecture et écriture dans le dossier `save`.
 *      Les constantes (`LONGUEUR_MAX_PSEUDO`, `LONGUEUR_LIGNE`, `OK`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post L'entrée correspondant au pseudo spécifié est supprimée du fichier `inventaire.csv`.
 *
 * @note La structure `inventaire` en paramètre est conservée pour compatibilité mais n'est pas utilisée par cette fonction.
 *
 * @warning Cette opération est irréversible. Assure-toi que les données ne seront plus nécessaires avant d'appeler cette fonction.
 */
int suppression_inventaire(inventaire_t *inventaire, char pseudo[LONGUEUR_MAX_PSEUDO]) {
    FILE *file = fopen("save/inventaire.csv", "r");      //Ouverture du fichier inventaire
    FILE *temp = fopen("save/temporaire.csv", "w");      //Ouverture du futur fichier
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[LONGUEUR_MAX_PSEUDO];
    char ligne[LONGUEUR_LIGNE];
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

/**
 * @brief Supprime toutes les entrées associées aux Mechas d'un joueur dans un fichier CSV.
 *
 * Cette fonction supprime définitivement les informations des Mechas appartenant à un joueur spécifique
 * depuis le fichier `save/infomechas.csv`. Un fichier temporaire est créé, puis toutes les entrées sont
 * recopiées sauf celles associées au pseudo spécifié. Finalement, l'ancien fichier est remplacé par le fichier temporaire.
 *
 * Cette fonction est utile lorsqu'une partie est supprimée ou réinitialisée afin d'assurer qu'il ne reste
 * aucune donnée associée aux Mechas du joueur.
 *
 * @param mechas_joueur Tableau de structures `mechas_joueur_t` (non utilisé dans cette fonction, conservé pour compatibilité).
 * @param pseudo        Chaîne de caractères représentant le pseudo du joueur dont les données Mechas doivent être supprimées.
 * @param nb_mechas     Nombre de Mechas dans l'équipe (paramètre non utilisé ici, conservé pour compatibilité).
 *
 * @return int
 *         - `OK` si les entrées ont été supprimées avec succès.
 *         - `ERREUR_OUVERTURE` en cas d'erreur lors de l'ouverture des fichiers.
 *
 * @pre Les fichiers CSV doivent être accessibles en lecture et écriture dans le dossier `save`.
 *      Les constantes (`LONGUEUR_MAX_PSEUDO`, `LONGUEUR_LIGNE`, `OK`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post Toutes les entrées correspondant aux Mechas du joueur sont supprimées du fichier `infomechas.csv`.
 *
 * @note Les paramètres `mechas_joueur` et `nb_mechas` ne sont pas utilisés directement ici, mais sont conservés pour compatibilité avec d'autres fonctions.
 *
 * @warning Cette opération est irréversible. Assure-toi que les données supprimées ne seront plus nécessaires.
 */
int suppression_mechas_joueur(mechas_joueur_t * mechas_joueur,char pseudo[LONGUEUR_MAX_PSEUDO],int nb_mechas) {
    FILE *file = fopen("save/infomechas.csv", "r");      //Ouverture du fichier infomechas
    FILE *temp = fopen("save/temporaire.csv", "w");      //Ouverture du futur fichier
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[LONGUEUR_MAX_PSEUDO];
    char ligne[LONGUEUR_LIGNE];
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

/**
 * @brief Supprime définitivement toutes les données d'une partie d'un joueur depuis les fichiers CSV.
 *
 * Cette fonction supprime intégralement les informations associées à un joueur spécifique des fichiers suivants :
 * - `save/joueur.csv` : informations globales du joueur (position, carte, etc.).
 * - `save/inventaire.csv` : inventaire du joueur (objets possédés).
 * - `save/infomechas.csv` : Mechas possédés par le joueur.
 * - `save/pnj_[pseudo].csv` : informations des interactions avec les PNJs propres à ce joueur.
 *
 * La fonction procède par recopie des données dans des fichiers temporaires, en omettant les entrées correspondant au pseudo spécifié,
 * puis remplace les fichiers originaux par les temporaires pour garantir la sécurité des données.
 *
 * @param joueur Pointeur vers la structure joueur_t contenant les informations du joueur (utilisé uniquement pour l'accès aux autres fonctions).
 * @param pseudo Chaîne de caractères représentant le pseudo du joueur à supprimer.
 *
 * @return int
 *         - `OK` si toutes les données associées au joueur ont été supprimées avec succès.
 *         - `ERREUR_OUVERTURE` en cas d'erreur lors de l'ouverture des fichiers.
 *
 * @pre Les fichiers CSV doivent être accessibles en lecture et écriture dans le dossier `save`.
 *      Les fonctions `suppression_inventaire` et `suppression_mechas_joueur` doivent être définies et valides.
 *      Les constantes (`LONGUEUR_MAX_PSEUDO`, `LONGUEUR_LIGNE`, `OK`, `ERREUR_OUVERTURE`) doivent être définies.
 *
 * @post Toutes les entrées correspondant au joueur sont supprimées définitivement des fichiers concernés.
 *
 * @note Utilise des fichiers temporaires pour garantir l'intégrité des données durant la suppression.
 *
 * @warning Cette opération est irréversible. Assure-toi que les données du joueur ne seront plus nécessaires avant de lancer cette opération.
 */
int suppression_partie(joueur_t *joueur, char pseudo[LONGUEUR_MAX_PSEUDO]) { 
    FILE *file = fopen("save/joueur.csv", "r");          //Ouverture du joueur
    FILE *temp = fopen("save/temporaire.csv", "w");      //Ouverture du futur fichier
    if (file == NULL || temp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ERREUR_OUVERTURE;
    }
    char nom[LONGUEUR_MAX_PSEUDO];
    char ligne[LONGUEUR_LIGNE];
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
    char nom_fichier[LONGUEUR_MAX_PSEUDO+10] = "save/pnj_";
    char ext[5] = ".csv";
    strcat(nom_fichier,pseudo);
    strcat(nom_fichier,ext);
    remove(nom_fichier);
    return OK;
}


//DESTRUCTION ALLOCATION DYNAMIQUE

/**
 * @brief Libère la mémoire dynamiquement allouée à un joueur.
 *
 * Cette fonction libère explicitement la mémoire allouée à l'inventaire d'un joueur
 * en appelant `free()`. Elle s'assure également de mettre le pointeur d'inventaire à `NULL`
 * pour éviter les références invalides (dangling pointers).
 *
 * À appeler impérativement avant de supprimer ou de réinitialiser définitivement une structure joueur.
 *
 * @param joueur Pointeur vers la structure joueur_t dont l'inventaire sera libéré.
 *
 * @return int
 *         - `OK` après la libération réussie de l'inventaire.
 *
 * @pre Le pointeur `joueur` doit être valide et l'inventaire doit avoir été alloué dynamiquement au préalable.
 *
 * @post Le pointeur `inventaire` de la structure joueur sera mis à `NULL`.
 *
 * @note Après appel de cette fonction, toute tentative d'accès à l'inventaire libéré doit être évitée.
 *
 * @warning Ne pas appeler cette fonction sur une structure joueur dont l'inventaire n'est pas alloué dynamiquement (risque d'erreur de segmentation).
 */
int destruction_joueur(joueur_t *joueur){
    joueur->nb_mechas = 0;
    free(joueur->inventaire);
    joueur->inventaire = NULL;
    return OK;
}

/**
 * @brief Libère la mémoire dynamiquement allouée à un PNJ.
 *
 * Cette fonction libère explicitement la mémoire allouée à l'inventaire d'un PNJ 
 * en appelant la fonction `free()`. Le pointeur vers l'inventaire du PNJ est ensuite mis à `NULL`
 * afin d’éviter les références invalides (dangling pointers).
 *
 * Cette opération est à réaliser avant la suppression définitive ou la réinitialisation complète d'une structure PNJ.
 *
 * @param pnj Pointeur vers la structure `pnj_t` dont l'inventaire sera libéré.
 *
 * @return int
 *         - `OK` après la libération réussie de l'inventaire.
 *
 * @pre Le pointeur `pnj` doit être valide et son inventaire doit avoir été alloué dynamiquement auparavant.
 *
 * @post Le pointeur `inventaire` de la structure `pnj` sera mis à `NULL`.
 *
 * @note Après l'appel à cette fonction, toute tentative d'accès à l'inventaire libéré doit être évitée.
 *
 * @warning Ne pas appeler cette fonction si l'inventaire du PNJ n'a pas été dynamiquement alloué, car cela provoquerait une erreur de segmentation.
 */
int destruction_pnj(pnj_t *pnj){
    free(pnj->inventaire);
    pnj->inventaire = NULL;
    return OK;
}
