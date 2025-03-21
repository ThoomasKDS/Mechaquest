/**
 * \file player.c
 * \brief fichier contenant les fonctions de déplacement
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/player.h"
#include "../lib/affichage.h"
#include "../lib/son.h"
#include <pointDePassage.h>


const int FRAME = 15 ;      //Nombre d'image dans l'animation


/**
 * @brief Gère le déplacement du joueur sur la carte en fonction des touches pressées.
 *
 * Cette fonction détermine la direction du déplacement en fonction des touches fléchées pressées par l'utilisateur
 * (gauche, droite, haut, bas). Elle vérifie si la case cible est valide (libre, accessible ou portail vers une autre carte).
 * Si le déplacement est valide, elle met à jour la position logique et graphique du joueur, et gère éventuellement le changement de carte.
 *
 * La fonction empêche explicitement le déplacement en diagonale et bloque tout déplacement si le joueur est déjà en animation.
 *
 * @param game        Pointeur vers la structure principale du jeu contenant l'état actuel du jeu et des cartes.
 * @param taille_x    Largeur de la carte en nombre de cases.
 * @param taille_y    Hauteur de la carte en nombre de cases.
 * @param keys        Tableau des touches pressées (`Uint8` SDL) pour déterminer le déplacement.
 * @param j           Pointeur vers la structure du joueur (position, état d’animation).
 * @param last_case   Pointeur vers un entier stockant l'identifiant de la dernière case occupée avant le déplacement du joueur.
 * @param sprite_p    Rectangle SDL définissant la position graphique du sprite du joueur.
 *
 * @return int  
 *         - La valeur de la case vers laquelle le joueur se déplace (`obj_case`), qui peut être :
 *           - `RIEN` si la case est vide ou traversable.
 *           - Un identifiant spécial (par exemple : changement de carte).
 *         - `0` si aucun déplacement valide n'est effectué (mouvement diagonal ou joueur en déplacement).
 *
 * @pre Les pointeurs (`game`, `j`, `keys`, `last_case`, `sprite_p`) doivent être valides et correctement initialisés.
 *      La carte et ses dimensions doivent être cohérentes avec `taille_x` et `taille_y`.
 *
 * @post La position du joueur (`j->x`, `j->y`) est mise à jour ainsi que la matrice du jeu (`game->mat`) selon le déplacement effectué.
 *       La gestion du changement de carte est faite si la case cible est un portail (`TPMAP1`, etc.).
 *
 * @note La fonction gère uniquement la logique du déplacement initial. L'animation réelle du joueur est traitée dans la boucle principale en fonction des paramètres `j->moving`, `j->move_dx` et `j->move_dy`.
 *
 * @warning Cette fonction ne doit pas être appelée si le joueur est déjà en animation (`j->moving != 0`).
 *          Assurez-vous que les constantes (`RIEN`, `JOUEUR`, `TPMAP1`, etc.) soient définies avant utilisation.
 */
int deplacement(int taille_x, int taille_y, const Uint8 *keys, joueur_t * j, int * last_case, SDL_Rect *sprite_p) {
    int obj_case = RIEN;
    if (!j->moving){  // si joueur deja entrain de se deplacer on ne fait rien

        int dx = 0, dy = 0;

        //direction du deplacement
        if (keys[SDL_SCANCODE_LEFT]) {
            dx = -1;
            j->derniere_touche = 1;
        }  
        if (keys[SDL_SCANCODE_RIGHT]) {
            dx = 1;
            j->derniere_touche = 2;

        } 
        if (keys[SDL_SCANCODE_UP]){
            dy = -1;
            j->derniere_touche = 3;
        }   
        if (keys[SDL_SCANCODE_DOWN]){
            j->derniere_touche = 4;
            dy = 1;
        }  
        if (dx != 0 && dy != 0) return 0; //empeche deplacement en diagonal
        
        //stocke la nouvelle case 
        int new_x = j->x + dx;
        int new_y = j->y + dy;

        //verifie qu'on peut se deplacer
        if (new_x >= 0 && new_x <= taille_x && new_y >= 0 && new_y <= taille_y && game.mat[game.mat_active][new_y][new_x] <= RIEN) {   
            obj_case = game.mat[game.mat_active][new_y][new_x];
            if(game.mat[game.mat_active][new_y][new_x] <= TPMAP1) { //si on change de map
                game.mat[game.mat_active][j->y][j->x] = RIEN;
                game.mat_active = -(game.mat[game.mat_active][new_y][new_x] + 11); //converti avec le bonne indice de la map
                

                //place au bonne endroit le joueur en fonction de ou le tp se trouve
                if(new_y == 0) j->y = taille_y - 2;
                else if(new_y == taille_y-1) j->y = 1;
                if(new_x == taille_x - 1) j->x = 1;
                else if(new_x == 0) j->x = taille_x - 2;



                j->screen_x = game.dms_win.x + (PX * j->x * game.scale);
                j->screen_y = game.dms_win.y + (PX * j->y * game.scale);
                sprite_p->x = j->screen_x;
                sprite_p->y = j->screen_y;
                game.mat[game.mat_active][j->y][j->x] = JOUEUR;
                j->numMap = game.mat_active;
                
            }

            else {
                // met à jour la mat            
                game.mat[game.mat_active][j->y][j->x] = *last_case;
                *last_case = game.mat[game.mat_active][new_y][new_x];
                game.mat[game.mat_active][new_y][new_x] = JOUEUR;

                // initialise l'animation
                j->x = new_x;
                j->y = new_y;
                j->move_dx = dx * (PX * game.scale) / FRAME;  // divise le déplacement en 15 étapes
                j->move_dy = dy * (PX * game.scale) / FRAME;
                j->moving = FRAME;  // animation sur 16 frames
                //vérification sapwn mechas
                jouer_bruit("son/pas.wav");
                
            }
        }
    }
    return obj_case;
}


/**
 * @brief Met à jour l'animation du joueur lors de son déplacement.
 *
 * Cette fonction actualise progressivement la position graphique du joueur (`sprite_p`)
 * sur l'écran, selon les incréments définis (`j->move_dx` et `j->move_dy`).
 * L'animation se déroule sur un nombre prédéfini de frames (`j->moving`), décrémenté
 * à chaque appel de la fonction jusqu’à atteindre zéro (fin de l’animation).
 *
 * Lorsque l'animation se termine (`j->moving == 0`), les valeurs de déplacement (`move_dx`, `move_dy`)
 * sont remises à zéro pour stopper tout mouvement résiduel.
 *
 * @param j         Pointeur vers la structure du joueur contenant les informations sur l'animation en cours.
 * @param sprite_p  Pointeur vers la structure SDL_Rect définissant la position du sprite du joueur à l’écran.
 *
 * @pre Les valeurs de déplacement (`j->move_dx` et `j->move_dy`) doivent être définies avant le début de l’animation.
 *      Le pointeur `sprite_p` doit être valide et initialisé correctement avec la position actuelle du joueur.
 *
 * @post La position graphique du joueur (`sprite_p->x`, `sprite_p->y`) est mise à jour progressivement à chaque appel.
 *       Les valeurs de déplacement sont réinitialisées à zéro lorsque l'animation se termine.
 *
 * @note Cette fonction doit être appelée régulièrement dans la boucle principale (généralement chaque frame)
 *       pour garantir une animation fluide du déplacement du joueur.
 *
 * @warning Veiller à ce que `j->screen_x` et `j->screen_y` soient initialisées correctement pour éviter tout décalage du sprite.
 */
void animation(joueur_t *j, SDL_Rect *sprite_p) {
    if (j->moving > 0) {
        j->screen_x += j->move_dx;
        j->screen_y += j->move_dy;
        sprite_p->x = (int)j->screen_x;
        sprite_p->y = (int)j->screen_y - 38;
        j->moving--;
    }

    if (j->moving == 0) {
        j->move_dx = 0;
        j->move_dy = 0;
    }

    
}

/**
 * @brief Détermine et génère l'apparition aléatoire d'un Mecha sauvage dans une zone définie.
 *
 * Cette fonction gère l'apparition aléatoire d'un Mecha sauvage en fonction d'une probabilité cumulative. 
 * À chaque déplacement du joueur dans une zone propice (identifiée par `obj_case`), la probabilité 
 * de rencontrer un Mecha augmente. Lorsqu'un Mecha apparaît, ses attributs (niveau, PV, attaque, défense, vitesse, attaques disponibles) 
 * sont définis aléatoirement en fonction des valeurs moyennes prédéfinies pour cette zone.
 *
 * @param j              Pointeur vers la structure du joueur (pour la gestion de la probabilité d'apparition).
 * @param obj_case       Valeur représentant la case où se situe le joueur (détermine la zone de spawn éventuel).
 * @param mecha_sauvage  Pointeur vers une structure Mecha qui sera remplie avec les attributs du Mecha sauvage généré.
 *
 * @return int
 *         - `1` : Un Mecha sauvage est apparu, et ses attributs ont été initialisés.
 *         - `0` : Aucun Mecha sauvage n’est apparu (probabilité insuffisante).
 *
 * @pre La structure `zone[]`, ainsi que les tableaux globaux (`attaque[]`, `mecha[]`), doivent être initialisés 
 *      et contenir des données valides.
 *      Les constantes telles que `Z1` et `Z10` doivent être correctement définies.
 *      Le générateur aléatoire (`rand()`) doit être initialisé préalablement (par exemple via `srand()`).
 *
 * @post Si un Mecha apparaît, ses attributs sont initialisés dans la structure pointée par `mecha_sauvage`.
 *       La probabilité de rencontre est réinitialisée (`j->proba_combat = 0`) en cas de succès.
 *       Sinon, la probabilité cumulative (`j->proba_combat`) est augmentée.
 *
 * @note Des logs détaillés sur les attributs générés sont affichés sur la sortie standard (utile pour le debug).
 *
 * @warning Vérifier soigneusement les bornes des tableaux (`zone`, `mecha`, `attaque`) pour éviter tout accès hors limites.
 */
int spawn_mecha(joueur_t * j, int obj_case, mechas_joueur_t * mecha_sauvage) {
    if(obj_case <= Z1 && obj_case >= Z10) {     //Z1 => Z10 nombres negatifs
        int i;
        j->proba_combat += 100;
        int n = rand() % 100;
        if(n < j->proba_combat) {
            j->proba_combat = 0;
            obj_case *= -1;
            obj_case--;               //donne l'indice de la zone
            int indice_liste = (rand() % zone[obj_case].nb_mechas );
            int indice_mechas =  zone[obj_case].listeMechasZone[indice_liste];      
            mecha_sauvage->niveau = (rand() % 5) + (zone[obj_case].NiveauMoyenApparition - 2);
            mecha_sauvage->pv_max = (rand() % 5) + (zone[obj_case].PvMoyen - 2);
            mecha_sauvage->pv = mecha_sauvage->pv_max;
            mecha_sauvage->id_mechas = indice_mechas;
            mecha_sauvage->attaque = (rand() % 10) + (zone[obj_case].Attaque - 4);
            mecha_sauvage->defense = (rand() % 10) + (zone[obj_case].Defense - 4);
            mecha_sauvage->vitesse = (rand() % 6) + (zone[obj_case].VitesseMoyenne  - 3);
            for(i = 0; i < 5 && attaque[mecha[mecha_sauvage->id_mechas].liste_attaque[i]-1].niveau <= mecha_sauvage->niveau; i++);
            mecha_sauvage->attaque_1 = attaque[mecha[indice_mechas].liste_attaque[i-1]-1].id_attaques;
            mecha_sauvage->attaque_2 = attaque[mecha[indice_mechas].liste_attaque[i-2]-1].id_attaques;
            mecha_sauvage->utilisation_1 = attaque[mecha[indice_mechas].liste_attaque[i-1]-1].utilisations;
            mecha_sauvage->utilisation_2 = attaque[mecha[indice_mechas].liste_attaque[i-2]-1].utilisations;
            /*printf("Mecha Sauvage:\n");
            printf(" Niveau: %d\n", mecha_sauvage->niveau);
            printf(" PV: %d/%d\n", mecha_sauvage->pv, mecha_sauvage->pv_max);
            printf(" ID: %d\n", mecha_sauvage->id_mechas);
            printf(" Attaque: %d\n", mecha_sauvage->attaque);
            printf(" Défense: %d\n", mecha_sauvage->defense);
            printf(" Vitesse: %d\n", mecha_sauvage->vitesse);
            printf(" Attaque 1: %d, Utilisations %d\n", mecha_sauvage->attaque_1, mecha_sauvage->utilisation_1);
            printf(" Attaque 2: %d, Utilisations %d\n", mecha_sauvage->attaque_2, mecha_sauvage->utilisation_2);*/

            return 1;
        }
    }
    return 0;
}

/** 
* @brief Détecte si un PNJ peut déclencher un combat avec le joueur en fonction de sa position et orientation.
*
* Cette fonction parcourt la liste des PNJ présents sur la carte active, vérifiant leur état, leur position,
* et leur orientation pour déterminer si l'un d'entre eux peut voir directement le joueur. Le PNJ peut détecter 
* le joueur jusqu'à 3 cases devant lui selon son orientation actuelle (haut, droite, bas, gauche).
*
* Dès qu'un PNJ détecte le joueur, la fonction retourne immédiatement l'indice correspondant à ce PNJ dans le tableau global `pnj[]`.
*
* @param game    Pointeur vers la structure contenant l’état du jeu et la carte active.
* @param joueur  Pointeur vers la structure du joueur contenant sa position actuelle sur la carte.
*
* @return int
*         - L'indice (`i`) du PNJ dans le tableau `pnj[]` qui a détecté le joueur.
*         - `0` si aucun PNJ ne détecte le joueur à proximité.
*
* @pre Les tableaux `game->mat` et `pnj[]` doivent être initialisés correctement.
*      Les constantes (`JOUEUR`, `PNJ`, `VIN_GAZOLE_1`) doivent être définies.
*
* @post Aucune modification d’état du jeu n'est réalisée par cette fonction.
*       Elle ne fait que retourner l'indice d'un PNJ pouvant déclencher un combat.
*
* @note La fonction s’assure que les coordonnées testées restent toujours à l'intérieur des limites de la matrice.
*
* @warning Assurez-vous que les valeurs des orientations du PNJ sont correctement définies :
*          - `1` : vers le haut
*          - `2` : vers la droite
*          - `3` : vers le bas
*          - `4` : vers la gauche
*/
int detection_combat_pnj(joueur_t *joueur){
    int test_x , test_y;
    int taille_x_mat = game.img_w / PX;    //taille de la matrice
    int taille_y_mat = game.img_h / PX;
    for(int i = 0; i < VIN_GAZOLE_1; i++){
        if((pnj[i].id_map ) == (game.mat_active+1) && pnj[i].etat == 0){
                test_x = pnj[i].x;
                test_y = pnj[i].y;
            for(int j = 0; j < 3 && (game.mat[game.mat_active][test_y][test_x] <= JOUEUR || game.mat[game.mat_active][test_y][test_x] ==PNJ); j++){
                
                switch(pnj[i].orientation){
                    case 1: //Vers le haut
                        test_y -= 1;
                    break;
                    case 2: //Vers la droite
                        test_x += 1;
                    break;
                    case 3: //Vers le bas
                        test_y += 1;
                    break;
                    case 4: //Vers la gauche
                        test_x -= 1;
                    break;
                }

                if(test_x >= 0 && test_x < taille_x_mat && test_y >= 0 && test_y < taille_y_mat){   //Test si les calculs ne sortent pas de la matrice
                    if(test_x == joueur->x && test_y == joueur->y){
                        return i;
                    }
                }
            }
            
        }
    }
    return 0;
}
