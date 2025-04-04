/**
 * \file pointDePassage.c
 * \brief Fichier contenant les fonctions de gestion des points de passage, de soin des Mechas,
 *        du choix du starter, des interactions avec les PNJ importants et des événements clés du jeu.
 */
#include "../lib/pointDePassage.h"
#include "../lib/combat.h"

/**
 * @brief Restaure entièrement la santé et les objets du joueur et sauvegarde la partie.
 *
 * Cette fonction réinitialise les objets de l'inventaire du joueur (mechaballs, carburant, rappel, repousse) 
 * à leur quantité maximale prédéfinie (`NB_OBJET`). Elle restaure également entièrement les points de vie (PV) 
 * et le nombre d'utilisations des attaques des Mechas présents dans l'équipe du joueur.
 *
 * Après avoir restauré les statistiques des Mechas et les objets du joueur, elle sauvegarde automatiquement
 * l’état actuel de la partie en appelant `sauvegarde_partie()`.
 *
 * @param j Pointeur vers la structure du joueur dont les Mechas et l'inventaire seront restaurés.
 *
 * @return int
 *         - `OK` : si les opérations de soin et de sauvegarde ont été effectuées avec succès.
 *
 * @pre Le pointeur `j`, ainsi que ses sous-structures (`inventaire`, `mechas_joueur`) doivent être initialisés correctement.
 *      Les tableaux globaux (notamment `attaque[]`) doivent également être initialisés et valides.
 *
 * @post Tous les objets de l'inventaire du joueur sont restaurés à leur quantité maximale.
 *       Les PV et les utilisations des attaques des Mechas du joueur sont entièrement restaurés.
 *       L’état de la partie est sauvegardé.
 *
 * @note Cette fonction est typiquement utilisée après un soin complet (comme dans un centre de soin).
 *
 * @warning Vérifier la validité des constantes utilisées (`NB_OBJET`, `NB_MECHAS_INVENTAIRE`, et `OK`) et
 *          que la fonction `sauvegarde_partie` soit bien implémentée.
 */
int soigner(joueur_t *j){
    j->inventaire->mechaball = NB_OBJET;
    j->inventaire->carburant = NB_OBJET;
    j->inventaire->rappel = NB_OBJET;
    j->inventaire->repousse = NB_OBJET;
    for(int i = 0;i < j->nb_mechas && i < NB_MECHAS_INVENTAIRE;i++){
        j->mechas_joueur[i].pv = j->mechas_joueur[i].pv_max;
        j->mechas_joueur[i].utilisation_1 = attaque[j->mechas_joueur[i].attaque_1-1].utilisations;
        j->mechas_joueur[i].utilisation_2 = attaque[j->mechas_joueur[i].attaque_2-1].utilisations;
    }
    sauvegarde_partie(j,j->pseudo);
    return OK;
}

/**
 * @brief Copie intégralement les attributs d'un Mecha vers l'équipe du joueur.
 *
 * Cette fonction ajoute un nouveau Mecha à l'équipe du joueur en copiant intégralement les attributs
 * d'un Mecha passé en paramètre (`mecha`) vers la prochaine position disponible dans l'équipe du joueur (`j->mechas_joueur[]`).
 *
 * Après la copie, le compteur du nombre de Mechas du joueur (`j->nb_mechas`) est incrémenté automatiquement.
 *
 * @param j      Pointeur vers la structure du joueur dont l'équipe va recevoir le nouveau Mecha.
 * @param mecha  Pointeur vers la structure du Mecha à copier dans l'équipe du joueur.
 *
 * @return int
 *         - `OK` si la copie du Mecha s'est effectuée correctement.
 *
 * @pre Les pointeurs (`j` et `mecha`) doivent être valides et correctement initialisés.
 *      L'équipe du joueur ne doit pas avoir atteint la limite maximale de Mechas autorisés (à vérifier avant appel).
 *
 * @post Le Mecha est copié intégralement dans l'équipe du joueur, et le nombre total de Mechas (`j->nb_mechas`) est mis à jour.
 *
 * @note Assurez-vous avant d'appeler cette fonction que le joueur dispose d'un emplacement libre dans son équipe.
 *
 * @warning Vérifiez que les constantes (`OK` et la taille maximale du tableau `j->mechas_joueur`) soient correctement définies pour éviter tout débordement.
 */
int copie_mechas(joueur_t *j,mechas_joueur_t *mecha){
    j->mechas_joueur[j->nb_mechas].id_mechas = mecha->id_mechas;
    j->mechas_joueur[j->nb_mechas].numero = j->nb_mechas+1;
    j->mechas_joueur[j->nb_mechas].niveau = mecha->niveau;
    j->mechas_joueur[j->nb_mechas].xp = mecha->xp;
    j->mechas_joueur[j->nb_mechas].pv = mecha->pv;
    j->mechas_joueur[j->nb_mechas].pv_max = mecha->pv_max;
    j->mechas_joueur[j->nb_mechas].attaque = mecha->attaque;
    j->mechas_joueur[j->nb_mechas].defense = mecha->defense;
    j->mechas_joueur[j->nb_mechas].vitesse = mecha->vitesse;
    j->mechas_joueur[j->nb_mechas].attaque_1 = mecha->attaque_1;
    j->mechas_joueur[j->nb_mechas].attaque_2 = mecha->attaque_2;
    j->mechas_joueur[j->nb_mechas].utilisation_1 = mecha->utilisation_1;
    j->mechas_joueur[j->nb_mechas].utilisation_2 = mecha->utilisation_2;
    j->nb_mechas+=1;

    return OK;
}

/**
 * @brief Permet au joueur de choisir son Mecha de départ parmi trois options disponibles.
 *
 * Cette fonction affiche un dialogue interactif demandant au joueur de choisir son Mecha de départ ("starter") parmi trois possibilités :
 * - Tournicoton (option 1)
 * - Rasetout (option 2)
 * - Tikart (option 3)
 *
 * Selon la réponse du joueur, le Mecha choisi est copié depuis les Mechas disponibles du PNJ `vinGazole` vers l'équipe du joueur.
 * Un dialogue de confirmation du choix effectué est ensuite affiché au joueur.
 *
 * @param game            Pointeur vers la structure principale du jeu (affichage, événements, etc.).
 * @param j               Pointeur vers la structure du joueur où le Mecha choisi sera ajouté.
 * @param vinGazole       Pointeur vers la structure du PNJ possédant les Mechas disponibles à la sélection.
 * @param sprite_p        Rectangle définissant la position graphique du sprite joueur.
 * @param pnj_sprite      Rectangle définissant la position graphique du sprite du PNJ.
 * @param sprite_pnj      Pointeur vers les textures associées au PNJ.
 * @param sprite_playerH  Pointeur vers les textures associées au joueur.
 *
 * @return int
 *         - `OK` si le choix du Mecha est effectué et copié avec succès dans l'équipe du joueur.
 *         - `ERR` en cas de choix invalide ou de problème lors de la copie.
 *
 * @pre Toutes les structures et ressources graphiques doivent être initialisées correctement.
 *      Le PNJ (`vinGazole`) doit disposer d'au moins trois Mechas valides dans son inventaire.
 *
 * @post Le Mecha choisi est ajouté à l'équipe du joueur.
 *
 * @note Cette fonction est interactive et bloque jusqu'à ce que le joueur sélectionne un choix valide (1, 2 ou 3).
 *
 * @warning Vérifie que les constantes `OK` et `ERR` soient définies dans ton projet.
 *          Pense à gérer le cas où la copie du Mecha échoue (par exemple, équipe pleine).
 */
int choix_starter(joueur_t *j, pnj_t *vinGazole, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite){
    int choix = 0;
    int resultat;
    //SDL_Event event;
    resultat = afficher_dialogue(j, sprite_p, pnj_sprite, vinGazole->pseudo, " Choisissez Le mechas que vous souhaité: \n 1: Tournicoton \n 2: Rasetout \n 3: Tikart.",true);
            
    switch(resultat){
        case 1: choix = copie_mechas(j,&vinGazole->mechas_joueur[0]);
                afficher_dialogue(j, sprite_p, pnj_sprite, vinGazole->pseudo, " Vous avez choisit Tournicoton.",false);
                break;
        case 2: choix = copie_mechas(j,&vinGazole->mechas_joueur[1]);
                afficher_dialogue(j, sprite_p, pnj_sprite, vinGazole->pseudo, " Vous avez choisit Rasetout.",false);
                break;
        case 3: choix = copie_mechas(j,&vinGazole->mechas_joueur[2]);
                afficher_dialogue(j, sprite_p, pnj_sprite,vinGazole->pseudo, " Vous avez choisit Tikart.",false);
                break;
        default: return ERR;break;
    }

    if(choix){
        j->inventaire->mechaball = vinGazole->inventaire->mechaball;
        j->inventaire->carburant = vinGazole->inventaire->carburant;
        j->inventaire->rappel = vinGazole->inventaire->rappel;
        j->inventaire->repousse = vinGazole->inventaire->repousse;
    }

    j->pointSauvegarde = 1;
    sauvegarde_partie(j,j->pseudo);
    return OK;
}

/**
 * @brief Gère l'interaction du joueur avec le PNJ "Vin Gazole", incluant dialogues, choix du starter et modification de la carte.
 *
 * Cette fonction vérifie si le joueur se trouve à côté du PNJ "Vin Gazole" dans la carte initiale (map 0) et qu'il presse la touche `P` pour initier l'interaction. 
 * Si l'interaction est valide (position, orientation et état initial du PNJ), un dialogue initial s'affiche, suivi du choix d'un Mecha starter. 
 * Ensuite, un dialogue final est affiché, l'état du PNJ est modifié (indiquant que l'interaction a eu lieu), une sauvegarde du PNJ est effectuée, 
 * et certaines cases spécifiques de la carte sont modifiées pour permettre la progression du joueur.
 *
 * De plus, la fonction empêche le joueur d'accéder à une autre zone de la carte avant d'avoir parlé à Vin Gazole, affichant alors un message d'avertissement.
 *
 * @param game            Pointeur vers la structure du jeu contenant les informations graphiques et l'état des cartes.
 * @param j               Pointeur vers la structure du joueur.
 * @param sprite_p        Rectangle définissant la position graphique du sprite joueur.
 * @param pnj_sprite      Rectangle définissant la position graphique du sprite du PNJ.
 * @param sprite_pnj      Pointeur vers les textures associées au PNJ.
 * @param sprite_playerH  Pointeur vers les textures associées au joueur.
 * @param keys            Tableau des touches pressées (`Uint8` SDL) pour gérer l'interaction.
 *
 * @return int
 *         - `OK` si la fonction s'exécute correctement.
 *
 * @pre Toutes les ressources graphiques (sprites, textures), ainsi que les structures (`game`, `joueur`, `pnj`), doivent être initialisées correctement.
 *      La matrice du jeu (`game->mat`) doit être définie et cohérente avec les positions du PNJ.
 *
 * @post Le PNJ "Vin Gazole" a changé d'état (`etat = 1`), une sauvegarde est effectuée, le joueur possède un Mecha starter, et certaines cases de la carte sont modifiées pour permettre une progression ultérieure.
 *
 * @note L'interaction se déclenche spécifiquement avec la touche `P`. Le message d'avertissement est affiché lorsque le joueur essaie de progresser prématurément vers une autre zone.
 *
 * @warning Vérifier que les constantes (`VIN_GAZOLE_1`, `OK`, `TPMAP2`) soient définies et valides, et que les coordonnées d'interaction soient correctes pour éviter les incohérences.
 */
int parler_a_vin_gazole( joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, const Uint8 *keys){
                if (keys[SDL_SCANCODE_P]){
                    if(j->numMap == 0 && j->x+1 == pnj[VIN_GAZOLE_1].x && j->y == pnj[VIN_GAZOLE_1].y && j->derniere_touche == 2 && pnj[VIN_GAZOLE_1].etat == 0){
                        printf("%s\n",pnj[VIN_GAZOLE_1].dialogueDebut);
                        afficher_dialogue(j, sprite_p, pnj_sprite, pnj[VIN_GAZOLE_1].pseudo, pnj[VIN_GAZOLE_1].dialogueDebut,false);
                        choix_starter(j, &pnj[VIN_GAZOLE_1], sprite_p, pnj_sprite);
                        printf("%s\n",pnj[VIN_GAZOLE_1].dialogueFin);
                        afficher_dialogue(j, sprite_p, pnj_sprite,pnj[VIN_GAZOLE_1].pseudo, pnj[VIN_GAZOLE_1].dialogueFin,false);
                        
                        pnj[VIN_GAZOLE_1].etat = 1;
                        sauvegarde_pnj(&pnj[VIN_GAZOLE_1],pnj[VIN_GAZOLE_1].id_pnj,j->pseudo);
                        game.mat[0][0][15] = TPMAP2;
                        game.mat[0][0][16] = TPMAP2;
                        game.mat[0][0][17] = TPMAP2;
                        j->pointSauvegarde = 1;
                    }
                }
                if(keys[SDL_SCANCODE_UP]){
                    if(j->numMap == 0 && (  j->x == 15 || j->x == 16 ||j->x == 17) && j->y == 1 ){
                        afficher_dialogue(j, sprite_p, pnj_sprite, "indice", " Allez d'abord parler à Vin Gazole.",false);
                        printf("Allez d'abord parler à Vin Gazole\n");
                    }
                }
    return OK;
}

/**
 * @brief Gère l'interaction du joueur avec les PNJs "Vin Gazole 2" et "Iron Musk", incluant dialogues, soins et combats.
 *
 * Cette fonction permet d'interagir avec deux PNJs spécifiques :
 * 
 * - **Vin Gazole 2 (map 0)** : Si le joueur interagit avec ce PNJ et que ses objets ou Mechas ne sont pas au maximum de leur capacité, 
 *   ses Mechas et son inventaire sont automatiquement soignés, et un dialogue confirmant le soin est affiché.
 *   Sinon, un simple dialogue initial est affiché.
 * 
 * - **Iron Musk (map 2)** : Lors de l'interaction, un dialogue de début s'affiche, un combat se déroule (actuellement commenté), 
 *   suivi d'un dialogue de fin. Après le combat, l'état du PNJ est mis à jour, une sauvegarde est effectuée, 
 *   des accès à d'autres cartes sont débloqués, et le joueur est téléporté à une position spécifique sur la carte initiale (map 0).
 *
 * De plus, la fonction empêche explicitement le joueur de quitter la zone d'Iron Musk avant d'avoir complété ce combat obligatoire, 
 * affichant alors un message d'avertissement.
 *
 * @param game            Pointeur vers la structure principale du jeu contenant les informations sur les cartes et le renderer.
 * @param j               Pointeur vers la structure du joueur (inventaire, équipe de Mechas, position).
 * @param sprite_p        Rectangle définissant la position graphique du sprite joueur.
 * @param pnj_sprite      Rectangle définissant la position graphique du sprite du PNJ.
 * @param sprite_pnj      Pointeur vers les textures associées au PNJ.
 * @param sprite_playerH  Pointeur vers les textures associées au joueur.
 * @param keys            Tableau des touches pressées (`Uint8` SDL) utilisé pour gérer l'interaction (`P` et flèches directionnelles).
 *
 * @return int
 *         - `OK` lorsque la fonction s'est correctement déroulée.
 *
 * @pre Toutes les structures du jeu (`game`, `joueur`, `pnj`) et les ressources graphiques doivent être initialisées.
 *      La fonction `soigner` ainsi que les fonctions de sauvegarde (`sauvegarde_pnj`, `sauvegarde_partie`) doivent être valides.
 *
 * @post Les états des PNJs sont modifiés après interaction, le joueur est éventuellement soigné, et des modifications de carte sont effectuées pour permettre la progression du joueur.
 *
 * @note La fonction déclenche l'interaction uniquement lors de l'appui sur la touche `P`. 
 *       Le message d'avertissement apparaît lorsque le joueur tente de quitter la zone sans avoir effectué le combat requis.
 *
 * @warning Vérifie que toutes les constantes (`VIN_GAZOLE_2`, `IRON_MUSK_DEB`, `OK`, `TPMAP4`, `TPMAP6`, `NB_OBJET`) soient correctement définies et initialisées.
 */
int premier_combat_musk(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, const Uint8 *keys){
   int soin = 0;
    if (keys[SDL_SCANCODE_P]){
        if(j->numMap == 0 && j->x+1 == pnj[VIN_GAZOLE_2].x && j->y == pnj[VIN_GAZOLE_2].y && j->derniere_touche == 2){
            if(j->inventaire->mechaball < NB_OBJET || j->inventaire->carburant < NB_OBJET|| j->inventaire->repousse < NB_OBJET || j->inventaire->rappel< NB_OBJET)
                soin = 1;
            for(int i = 0;i < NB_MECHAS_INVENTAIRE && soin == 0;i++){
                if(j->mechas_joueur[0].pv < j->mechas_joueur[0].pv_max)
                    soin = 1;       
            }
            if(soin){
                afficher_dialogue(j, sprite_p, pnj_sprite, pnj[VIN_GAZOLE_2].pseudo, pnj[VIN_GAZOLE_2].dialogueFin,false);
                printf("%s\n",pnj[VIN_GAZOLE_2].dialogueFin);
                soigner(j);
                soin = 0;
            }
            else{
                afficher_dialogue(j, sprite_p, pnj_sprite,pnj[VIN_GAZOLE_2].pseudo, pnj[VIN_GAZOLE_2].dialogueDebut,false);
                printf("%s\n",pnj[VIN_GAZOLE_2].dialogueDebut);
            }
            
        }
        if(j->numMap == 2 && j->x+1 == pnj[IRON_MUSK_DEB].x && j->y == pnj[IRON_MUSK_DEB].y && j->derniere_touche == 2){
            afficher_dialogue(j, sprite_p, pnj_sprite, pnj[IRON_MUSK_DEB].pseudo, pnj[IRON_MUSK_DEB].dialogueDebut,false);
            combat_pnj(j, &pnj[IRON_MUSK_DEB]);
            afficher_dialogue(j, sprite_p, pnj_sprite, pnj[IRON_MUSK_DEB].pseudo, pnj[IRON_MUSK_DEB].dialogueFin,false);
            pnj[IRON_MUSK_DEB].etat = 1;
            sauvegarde_pnj(&pnj[IRON_MUSK_DEB],pnj[IRON_MUSK_DEB].id_pnj,j->pseudo);
            game.mat[2][0][4] = TPMAP4;
            game.mat[2][0][5] = TPMAP4;
            game.mat[2][0][6] = TPMAP4;
            game.mat[2][9][19] = 0;

            game.mat[2][8][0] = TPMAP6;
            game.mat[2][9][0] = TPMAP6;
            game.mat[2][10][0] = TPMAP6;

            j->x = 23;
            j->y = 8;
            j->numMap = 0;
            j->pointSauvegarde = 2;
            j->derniere_touche = 4;
            game.mat_active = j->numMap;
            sauvegarde_partie(j,j->pseudo);
            j->screen_x = (float)(game.dms_win.x + (j->x * PX * game.scale));      //position du joueur en px
            j->screen_y = (float)(game.dms_win.y + (j->y * PX * game.scale));
        }
    }
    if(keys[SDL_SCANCODE_UP]){
        if(j->numMap == 2 && (j->x == 4 || j->x == 5 ||j->x == 6) && j->y == 1){
            afficher_dialogue(j, sprite_p, pnj_sprite,"indice", " Allez combattre Iron Musk avant.\n",false);
            printf("Allez combattre Iron Musk avant\n");
        }
    }
    return OK;
}

                /**
 * @brief Gère la seconde interaction du joueur avec le PNJ "Vin Gazole", incluant dialogues, soin du joueur et déblocage de la carte.
 *
 * Cette fonction permet au joueur de retourner parler au PNJ "Vin Gazole" après avoir progressé dans l'histoire.  
 * Lors de cette interaction, deux dialogues successifs sont affichés au joueur (début et fin). Ensuite,  
 * le joueur et ses Mechas sont complètement soignés (inventaire et PV), l'état du PNJ est modifié pour indiquer  
 * que l'interaction a eu lieu, une sauvegarde du PNJ est effectuée, et les cases permettant la progression ultérieure  
 * du joueur sont débloquées. De plus, un nouveau point de sauvegarde est défini pour le joueur.
 *
 * Si le joueur tente de progresser avant cette interaction obligatoire, un message d'avertissement lui indique  
 * clairement qu'il doit aller se soigner au préalable.
 *
 * @param game            Pointeur vers la structure principale du jeu (état actuel, renderer, cartes).
 * @param j               Pointeur vers la structure du joueur (position, inventaire, état des Mechas).
 * @param sprite_p        Rectangle définissant la position graphique du sprite joueur.
 * @param pnj_sprite      Rectangle définissant la position graphique du sprite du PNJ.
 * @param sprite_pnj      Pointeur vers les textures associées au PNJ.
 * @param sprite_playerH  Pointeur vers les textures associées au joueur.
 * @param keys            Tableau des touches pressées (`Uint8` SDL) pour gérer l'interaction.
 *
 * @return int
 *         - `OK` si la fonction s'exécute correctement.
 *
 * @pre Toutes les structures du jeu (`game`, `joueur`, `pnj`) et ressources graphiques doivent être initialisées correctement.
 *      Les fonctions appelées (`afficher_dialogue`, `soigner`, `sauvegarde_pnj`) doivent être implémentées et valides.
 *
 * @post L'état du PNJ "Vin Gazole" est mis à jour (`etat = 1`), le joueur est intégralement soigné, une sauvegarde est effectuée,
 *       et la carte est modifiée pour permettre au joueur de progresser. Un point de sauvegarde est enregistré.
 *
 * @note L'interaction se déclenche uniquement en pressant la touche `P`. 
 *       Un message d'avertissement est affiché en cas de tentative prématurée d'avancer.
 *
 * @warning Vérifier que toutes les constantes (`VIN_GAZOLE_3`, `OK`, `TPMAP2`) soient correctement définies et initialisées.
 */
int retourner_parler_a_vin_gazole(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, const Uint8 *keys){
    if (keys[SDL_SCANCODE_P]){
        if(j->x+1 == pnj[VIN_GAZOLE_3].x && j->y == pnj[VIN_GAZOLE_3].y && j->derniere_touche == 2 && pnj[VIN_GAZOLE_3].etat == 0){
            afficher_dialogue(j, sprite_p, pnj_sprite,pnj[VIN_GAZOLE_3].pseudo, pnj[VIN_GAZOLE_3].dialogueDebut,false);
            printf("%s\n",pnj[VIN_GAZOLE_3].dialogueDebut);
            afficher_dialogue(j, sprite_p, pnj_sprite,pnj[VIN_GAZOLE_3].pseudo, pnj[VIN_GAZOLE_3].dialogueFin,false);
            printf("%s\n",pnj[VIN_GAZOLE_3].dialogueFin);
            soigner(j);
            pnj[VIN_GAZOLE_3].etat = 1;
            sauvegarde_pnj(&pnj[VIN_GAZOLE_3],pnj[VIN_GAZOLE_3].id_pnj,j->pseudo);
            game.mat[0][0][15] = TPMAP2;
            game.mat[0][0][16] = TPMAP2;
            game.mat[0][0][17] = TPMAP2;
            j->pointSauvegarde = 3;
        }
    }
    if(keys[SDL_SCANCODE_UP]){
        if((j->x == 15 || j->x == 16 ||j->x == 17) && j->y == 1){
            afficher_dialogue(j, sprite_p, pnj_sprite, "indice", " Allez vous soigner.",false);
            printf("Allez vous soigner\n");
        }
    } 
    return OK;
}

                /**
 * @brief Gère les interactions finales avec les PNJs "Vin Gazole" et "Iron Musk", incluant dialogues, soin automatique et combat final.
 *
 * Cette fonction gère deux interactions distinctes :
 *
 * - **Interaction avec Vin Gazole (map 0)** :  
 *   Le joueur peut interagir pour se faire entièrement soigner (inventaire et Mechas) si ses objets ou ses Mechas ne sont pas au maximum.  
 *   Un dialogue spécifique est alors affiché pour indiquer que le soin a eu lieu. Sinon, un simple dialogue de début est affiché.
 *
 * - **Combat final contre Iron Musk (map 4)** :  
 *   Cette interaction déclenche un dialogue initial, un combat (actuellement commenté), puis un dialogue final.  
 *   Après la victoire, le joueur reçoit un Mecha spécifique de la part d'Iron Musk, le jeu sauvegarde automatiquement l'état  
 *   du joueur et du PNJ, et un nouveau point de sauvegarde est enregistré.
 *
 * @param game            Pointeur vers la structure du jeu (état des cartes, renderer, etc.).
 * @param j               Pointeur vers la structure du joueur (inventaire, Mechas, position, état).
 * @param sprite_p        Rectangle définissant la position graphique du sprite joueur.
 * @param pnj_sprite      Rectangle définissant la position graphique du sprite du PNJ.
 * @param sprite_pnj      Pointeur vers les textures associées au PNJ.
 * @param sprite_playerH  Pointeur vers les textures associées au joueur.
 * @param keys            Tableau des touches pressées (`Uint8` SDL) pour gérer les interactions (touche `P`).
 *
 * @return int
 *         - `OK` lorsque la fonction s'exécute correctement.
 *
 * @pre Toutes les structures et ressources graphiques nécessaires doivent être initialisées et valides.
 *      Les fonctions appelées (`afficher_dialogue`, `soigner`, `copie_mechas`, `sauvegarde_partie`, `sauvegarde_pnj`) doivent être implémentées.
 *
 * @post L'interaction avec "Vin Gazole" entraîne le soin automatique du joueur si nécessaire.
 *       L'interaction avec "Iron Musk" déclenche le combat final, l'ajout d'un nouveau Mecha à l'équipe du joueur, la sauvegarde automatique, et met à jour l'état du PNJ.
 *
 * @note Le combat contre Iron Musk doit être implémenté explicitement (actuellement commenté).
 *       L'interaction s'active en pressant la touche `P`.
 *
 * @warning Vérifie bien que toutes les constantes (`VIN_GAZOLE_4`, `IRON_MUSK_FIN`, `NB_OBJET`, `OK`) soient correctement définies et initialisées.
 */
int combat_final(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, const Uint8 *keys){
    int soin = 0;
    if (keys[SDL_SCANCODE_P]){
        if(j->numMap == 0 && j->x+1 == pnj[VIN_GAZOLE_4].x && j->y == pnj[VIN_GAZOLE_4].y && j->derniere_touche == 2){
            if(j->inventaire->mechaball < NB_OBJET || j->inventaire->carburant < NB_OBJET|| j->inventaire->repousse <NB_OBJET || j->inventaire->rappel< NB_OBJET)
                soin = 1;
            for(int i = 0;i < NB_MECHAS_INVENTAIRE && soin == 0;i++){
                if(j->mechas_joueur[0].pv < j->mechas_joueur[0].pv_max)
                    soin = 1;       
            }
            if(soin){
                afficher_dialogue(j, sprite_p, pnj_sprite, pnj[VIN_GAZOLE_4].pseudo, pnj[VIN_GAZOLE_4].dialogueFin,false);
                printf("%s\n",pnj[VIN_GAZOLE_4].dialogueFin);
                soigner(j);
                soin = 0;
            }
            else{
                afficher_dialogue(j, sprite_p, pnj_sprite ,pnj[VIN_GAZOLE_4].pseudo, pnj[VIN_GAZOLE_4].dialogueDebut,false);
                printf("%s\n",pnj[VIN_GAZOLE_4].dialogueDebut);
            }
            
        }
        if(j->numMap == 4 && j->x == pnj[IRON_MUSK_FIN].x && j->y-1 == pnj[IRON_MUSK_FIN].y && j->derniere_touche == 3 && pnj[IRON_MUSK_FIN].etat == 0){
            afficher_dialogue(j, sprite_p, pnj_sprite, pnj[IRON_MUSK_FIN].pseudo, pnj[IRON_MUSK_FIN].dialogueDebut,false);
            printf("%s\n",pnj[IRON_MUSK_FIN].dialogueDebut);
            
            if(combat_pnj(j, &pnj[IRON_MUSK_FIN])){
                afficher_dialogue(j, sprite_p, pnj_sprite, pnj[IRON_MUSK_FIN].pseudo, pnj[IRON_MUSK_FIN].dialogueFin,false);
                printf("%s\n",pnj[IRON_MUSK_FIN].dialogueFin);
                copie_mechas(j,&pnj[IRON_MUSK_FIN].mechas_joueur[0]);
                sauvegarde_partie(j,j->pseudo);
                pnj[IRON_MUSK_FIN].etat = 1;
                sauvegarde_pnj(&pnj[IRON_MUSK_FIN],pnj[IRON_MUSK_FIN].id_pnj,j->pseudo);

                j->pointSauvegarde = 4;
            }
            else {
                game_over(j);
            }
            
            
        }
    }
    return OK;
}

                /**
 * @brief Gère l'interaction avec le PNJ "Vin Gazole" en mode jeu libre, permettant soins et dialogues répétés.
 *
 * Cette fonction permet au joueur d'interagir avec le PNJ "Vin Gazole" après avoir terminé l'histoire principale du jeu.
 * Lors de la toute première interaction dans ce mode (`etat == 0`), deux dialogues (début et fin) sont affichés, puis le joueur est soigné complètement.
 * L'état du PNJ passe ensuite à 1 et une sauvegarde de cet état est réalisée.
 *
 * Pour les interactions suivantes (`etat == 1`), le joueur peut toujours revenir pour être soigné automatiquement en cas de besoin (inventaire ou PV des Mechas non maximaux).
 * Un dialogue adapté selon l'état actuel du joueur est alors affiché (indiquant le soin ou non).
 *
 * @param game            Pointeur vers la structure du jeu (cartes, renderer, état général).
 * @param j               Pointeur vers la structure du joueur (inventaire, Mechas, position).
 * @param sprite_p        Rectangle définissant la position graphique du sprite joueur.
 * @param pnj_sprite      Rectangle définissant la position graphique du sprite du PNJ.
 * @param sprite_pnj      Pointeur vers les textures associées au PNJ.
 * @param sprite_playerH  Pointeur vers les textures associées au joueur.
 * @param keys            Tableau des touches pressées (`Uint8` SDL) permettant l'interaction (`P`).
 *
 * @return int
 *         - `OK` si l'interaction est traitée correctement.
 *
 * @pre Toutes les ressources graphiques et structures (`game`, `joueur`, `pnj`) doivent être initialisées.
 *      Les fonctions `afficher_dialogue`, `soigner`, `sauvegarde_pnj` doivent être implémentées.
 *
 * @post L'état initial du PNJ est modifié à la première interaction (`etat` passe à 1), et le joueur peut revenir indéfiniment pour des soins.
 *       L'inventaire et les Mechas du joueur peuvent être restaurés à chaque interaction si nécessaire.
 *
 * @note La touche `P` déclenche l'interaction avec le PNJ.
 *
 * @warning Assure-toi que toutes les constantes (`VIN_GAZOLE_4`, `VIN_GAZOLE_5`, `OK`, `NB_OBJET`) soient définies et initialisées.
 */
int jeu_libre(joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite,const Uint8 *keys){
    int soin = 0;
    if (keys[SDL_SCANCODE_P]){
        if(j->numMap == 0 && j->x+1 == pnj[VIN_GAZOLE_5].x && j->y == pnj[VIN_GAZOLE_5].y && j->derniere_touche == 2){
            if(pnj[VIN_GAZOLE_5].etat == 0){
                afficher_dialogue(j, sprite_p, pnj_sprite, pnj[VIN_GAZOLE_5].pseudo, pnj[VIN_GAZOLE_5].dialogueDebut,false);
                printf("%s\n",pnj[VIN_GAZOLE_5].dialogueDebut);
                afficher_dialogue(j, sprite_p, pnj_sprite,pnj[VIN_GAZOLE_5].pseudo, pnj[VIN_GAZOLE_5].dialogueFin,false);
                printf("%s\n",pnj[VIN_GAZOLE_5].dialogueFin);
                soigner(j);
                pnj[VIN_GAZOLE_5].etat = 1;
                sauvegarde_pnj(&pnj[VIN_GAZOLE_5],pnj[VIN_GAZOLE_5].id_pnj,j->pseudo);
            }
            else if(pnj[VIN_GAZOLE_3].etat == 1){
                game_over(j);
                if(j->inventaire->mechaball < NB_OBJET || j->inventaire->carburant < NB_OBJET|| j->inventaire->repousse < NB_OBJET || j->inventaire->rappel< NB_OBJET)
                    soin = 1;
                for(int i = 0;i < NB_MECHAS_INVENTAIRE && soin == 0;i++){
                    if(j->mechas_joueur[0].pv < j->mechas_joueur[0].pv_max)
                        soin = 1;       
                }
                if(soin){
                    afficher_dialogue(j, sprite_p, pnj_sprite, pnj[VIN_GAZOLE_4].pseudo, pnj[VIN_GAZOLE_4].dialogueFin,false);
                    printf("%s\n",pnj[VIN_GAZOLE_4].dialogueFin);
                    soigner(j);
                    soin = 0;
                }
                else{
                    afficher_dialogue(j, sprite_p, pnj_sprite,pnj[VIN_GAZOLE_4].pseudo, pnj[VIN_GAZOLE_4].dialogueDebut,false);
                    printf("%s\n",pnj[VIN_GAZOLE_4].dialogueDebut);
                }
            }
            
        }
    }
    return OK;
}
