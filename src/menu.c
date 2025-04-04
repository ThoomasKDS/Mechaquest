/**
 * \file menu.c
 * \brief Fichier contenant les fonctions liées à l'affichage et la gestion du menu principal et des sous-menus du jeu.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/menu.h"
#include "../lib/initGame.h"
#include "../lib/affichage.h"
#include "../lib/son.h"

/**
 * @brief Charge une image depuis un fichier et la convertit en texture SDL.
 *
 * Cette fonction prend en charge le chargement d'une image depuis un chemin spécifié, en utilisant SDL_image.
 * L'image chargée est convertie en une texture compatible avec le renderer SDL spécifié dans `game`.
 * Si une erreur survient pendant le chargement ou la création de la texture, la fonction renvoie `NULL`.
 *
 * @param chemin  Chemin relatif ou absolu vers le fichier image à charger.
 * @param game    Pointeur vers la structure `game_t` contenant le renderer SDL initialisé.
 *
 * @return SDL_Texture*
 *         - La texture SDL créée à partir de l'image chargée en cas de succès.
 *         - `NULL` si le chargement ou la création de texture échoue. Un message d'erreur est affiché sur la sortie standard.
 *
 * @pre Le renderer de `game` doit être initialisé et valide avant l'appel à cette fonction.
 * @post L'image chargée est libérée après conversion en texture ; seule la texture persiste en mémoire.
 *
 * @warning La texture retournée doit être libérée avec `SDL_DestroyTexture()` après utilisation pour éviter les fuites mémoire.
 */
SDL_Texture* charger_texture(const char *chemin){
    SDL_Surface *surfaceChargee = IMG_Load(chemin);
    if (!surfaceChargee) {
        printf("Erreur chargement image: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(game.renderer, surfaceChargee);
    SDL_FreeSurface(surfaceChargee);
    return texture;
}

/**
 * @brief Affiche un écran permettant au joueur de choisir son sexe (Masculin ou Féminin).
 *
 * Cette fonction affiche deux rectangles interactifs représentant respectivement les sexes masculin et féminin,
 * chacun associé à une image. Le joueur peut cliquer sur l'un des rectangles pour sélectionner son sexe,
 * ou utiliser un bouton de retour pour annuler son choix.
 *
 * Le choix effectué est stocké dans la structure joueur (`j->sexe`). Si le joueur choisit le bouton retour,
 * le pseudo sera remis à zéro (`pseudo[0] = '\0'`).
 *
 * @param game   Pointeur vers la structure du jeu contenant le renderer et les informations d'affichage.
 * @param j      Pointeur vers la structure du joueur, où le sexe choisi sera enregistré.
 * @param pseudo Chaîne de caractères du pseudo du joueur, réinitialisée à vide si le joueur revient en arrière.
 *
 * @return `1` si le joueur a sélectionné son sexe (Masculin ou Féminin),
 *         `0` si le joueur choisit de retourner en arrière (bouton retour).
 *
 * @pre Le renderer (`game->renderer`) doit être initialisé avant l'appel.
 *      Les pointeurs `game`, `j` et `pseudo` doivent être valides.
 * @post La valeur du sexe du joueur est enregistrée ('M' ou 'F'), et le pseudo est éventuellement modifié.
 *
 * @note Cette fonction gère sa propre boucle d'événements et assure le rendu continu de l'écran jusqu'à la sélection du joueur.
 *       Elle libère automatiquement les textures créées avant de quitter.
 */
int afficher_choix_sexe(joueur_t* j,char* pseudo){
    rectangle_t rect_h, rect_f;
    int largeurEcran, hauteurEcran, frameTime;
    SDL_GetRendererOutputSize(game.renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart; //obtien l'heure    
    rectangle_t btn_retour;

    SDL_Texture* sprite_h = charger_texture("img/skin/skin_player_homme/bas1.png");
    SDL_Texture* sprite_f = charger_texture("img/skin/skin_player_femme/bas1.png");
    creer_rectangle(&btn_retour,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2,98, 23, 8, 255,"Retour");
    creer_rectangle(&rect_h, 200, 250, largeurEcran / 2 - 350, hauteurEcran / 2, 255, 255, 255, 150, NULL);
    creer_rectangle(&rect_f, 200, 250, largeurEcran / 2 + 150, hauteurEcran / 2, 255, 255, 255, 150, NULL);
    int running = 1, res;

    SDL_Event event;
    while(running){
        frameStart = SDL_GetTicks();
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) {
                running = 0;
                res = 0;
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN){
                int x = event.button.x, y = event.button.y;
                
                if (x >= rect_h.rect.x && x <= rect_h.rect.x + rect_h.rect.w &&
                    y >= rect_h.rect.y && y <= rect_h.rect.y + rect_h.rect.h) {
                    running = 0;
                    j->sexe = 'M';
                    res = 1;
                }
                else if (x >= rect_f.rect.x && x <= rect_f.rect.x + rect_f.rect.w &&
                    y >= rect_f.rect.y && y <= rect_f.rect.y + rect_f.rect.h) {
                    running = 0;
                    j->sexe = 'F';
                    res = 1;
                }
                else if (x >= btn_retour.rect.x && x <= btn_retour.rect.x + btn_retour.rect.w &&
                    y >= btn_retour.rect.y && y <= btn_retour.rect.y + btn_retour.rect.h) {
                    running = 0;
                    pseudo[0] = '\0';
                    res = 0;
                }
            }
        }
        if(running) {
            SDL_RenderClear(game.renderer);
            draw_background();
            draw_all_rect(3, &btn_retour, &rect_h, &rect_f);
            SDL_RenderCopy(game.renderer, sprite_h, NULL, &rect_h.rect);
            SDL_RenderCopy(game.renderer, sprite_f, NULL, &rect_f.rect);

            // Affichage des valeurs
            char buffer[50];
            SDL_Color couleurTexte = {255, 255, 255, 255};

            // Affichage du texte : Choisir son sexe
            sprintf(buffer, "Choisir son sexe");
            SDL_Surface* surfaceVolume = TTF_RenderText_Solid(game.police, buffer, couleurTexte);
            SDL_Texture* textureVolume = SDL_CreateTextureFromSurface(game.renderer, surfaceVolume);
            SDL_Rect rectVolume = {(largeurEcran - surfaceVolume->w) / 2, hauteurEcran / 2 - 100, surfaceVolume->w, surfaceVolume->h};
            SDL_RenderCopy(game.renderer, textureVolume, NULL, &rectVolume);
            SDL_FreeSurface(surfaceVolume);
            SDL_DestroyTexture(textureVolume);

            SDL_RenderPresent(game.renderer);
            
            frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

            if (FRAME_DELAY > frameTime) {
                SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
            }
        }
     }    
    
    SDL_DestroyTexture(sprite_h);
    SDL_DestroyTexture(sprite_f);
    return res;
}

/**
 * @brief Affiche un écran permettant au joueur de choisir entre reprendre, recommencer ou retourner en arrière avant une suppression de sauvegarde.
 *
 * Cette fonction affiche trois boutons distincts à l'écran : 
 * - "Reprendre" pour continuer avec la sauvegarde existante.
 * - "Recommencer" pour supprimer la sauvegarde actuelle et démarrer une nouvelle partie (le joueur devra choisir son sexe à nouveau).
 * - "Retour" pour annuler l’action en cours et revenir à l'écran précédent.
 *
 * La fonction attend la réponse du joueur via un clic souris sur un des trois boutons affichés et exécute les actions associées :
 * mise à jour ou suppression des données du joueur selon la sélection.
 *
 * @param game   Pointeur vers la structure du jeu (renderer, fenêtre, etc.).
 * @param j      Pointeur vers la structure du joueur dont la sauvegarde est concernée.
 * @param pseudo Pointeur vers le tableau de caractères contenant le pseudo du joueur.
 *
 * @return Un entier indiquant le résultat de l’action du joueur :
 *         - `1` : Le joueur choisit "Reprendre" ou "Recommencer" (dans ce cas, la sauvegarde est supprimée puis réinitialisée).
 *         - `0` : Le joueur choisit "Retour" ou ferme la fenêtre, aucune action effectuée.
 *
 * @pre Le renderer (`game->renderer`) doit être initialisé. Les pointeurs `game`, `j`, et `pseudo` doivent être valides.
 * @post Selon l’action choisie, la sauvegarde du joueur peut être supprimée et réinitialisée. 
 *       Le pseudo peut être vidé (`pseudo[0] = '\0'`) si l'utilisateur annule son choix.
 *
 * @note Cette fonction gère elle-même les événements SDL et le rendu graphique associé aux boutons.
 *       Elle contrôle aussi le framerate (temps d'attente entre chaque affichage) pour fluidifier l'affichage.
 */
int afficher_choix_suppression(joueur_t* j,char* pseudo){
    int largeurEcran, hauteurEcran,frameTime;
    SDL_GetRendererOutputSize(game.renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart; //obtien l'heure

    rectangle_t btn_retour,btn_reprendre,btn_recommencer;
    creer_rectangle(&btn_reprendre,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran - HAUTEUR_BOUTON * 2) / 2,58, 90, 64, 255,"Reprendre");
    creer_rectangle(&btn_recommencer,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran + HAUTEUR_BOUTON) / 2,77, 144, 142, 255,"Recommencer");
    creer_rectangle(&btn_retour,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2,98, 23, 8, 255,"Retour");
    
    int x, y;
    int running = 1;
    SDL_Event event;
    
    while (running) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                x = event.button.x; 
                y = event.button.y;

                // Bouton Retour
                if (x >= btn_retour.rect.x && x <= btn_retour.rect.x + btn_retour.rect.w && y >= btn_retour.rect.y && y <= btn_retour.rect.y + btn_retour.rect.h) {
                    pseudo[0] = '\0';
                    return 0;
                }

                // Bouton Oui
                if (x >= btn_reprendre.rect.x && x <= btn_reprendre.rect.x + btn_reprendre.rect.w && y >= btn_reprendre.rect.y && y <= btn_reprendre.rect.y + btn_reprendre.rect.h) {
                    return 1;
                }
                // Bouton Non
                if (x >= btn_recommencer.rect.x && x <= btn_recommencer.rect.x + btn_recommencer.rect.w && y >= btn_recommencer.rect.y && y <= btn_recommencer.rect.y + btn_recommencer.rect.h) {
                    if(afficher_choix_sexe(j, pseudo)) {
                        suppression_partie(j,pseudo);
                        init_partie(j,pseudo,j->sexe);
                        return 1;
                    }
                    else {
                        return 0;
                    }
                }
            }
        }

        if(running) {
            // Affichage
            SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
            SDL_RenderClear(game.renderer);
            
            draw_background();
            draw_all_rect(3, &btn_retour, &btn_reprendre, &btn_recommencer);
            SDL_RenderPresent(game.renderer);
            
            frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

            if (FRAME_DELAY > frameTime) {
                SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
            }
        }
     }    
    return 0;
}

/**
 * @brief Affiche un écran permettant au joueur de saisir son pseudo et de démarrer une partie.
 *
 * Cette fonction présente une interface de saisie de texte où le joueur peut entrer son pseudo. 
 * Deux boutons sont affichés à l'écran :
 * - "Commencer" : vérifie si une sauvegarde existe sous le pseudo saisi et propose de continuer ou recommencer.
 * - "Retour" : permet au joueur d'annuler la saisie et revenir en arrière (réinitialisant le pseudo).
 *
 * Elle gère l'entrée utilisateur via le clavier (y compris la gestion de la suppression avec BACKSPACE) 
 * et les interactions par clic souris sur les boutons.
 *
 * @param game    Pointeur vers la structure du jeu contenant le renderer et les ressources nécessaires à l'affichage.
 * @param j       Pointeur vers la structure du joueur où les données chargées ou initialisées seront stockées.
 * @param pseudo  Chaîne de caractères où sera stocké le pseudo saisi par le joueur. Réinitialisé en cas de retour.
 *
 * @return int
 *         - `1` : si le joueur a terminé la saisie et validé en appuyant sur "Commencer" après avoir choisi son sexe ou confirmé une sauvegarde existante.
 *         - `0` : si le joueur quitte ou utilise le bouton "Retour".
 *
 * @pre Le renderer (`game->renderer`) et la police de caractères (`game->police`) doivent être initialisés.
 *      Les pointeurs `game`, `j` et `pseudo` doivent être valides.
 * @post Le pseudo du joueur est stocké ou réinitialisé selon l'action choisie. La partie peut être initialisée si le joueur valide.
 *
 * @note La saisie du texte utilise SDL pour gérer les événements clavier. 
 *       La fonction inclut une régulation de framerate pour un affichage fluide.
 *
 * @warning La longueur maximale du pseudo est définie par la constante `LONGUEUR_MAX_PSEUDO`. 
 *          Veiller à ce que cette constante soit cohérente avec la taille réelle du tableau `pseudo`.
 */
int aff_saisie_pseudo(joueur_t* j, char* pseudo) {
    int largeurEcran, hauteurEcran,frameTime;
    SDL_GetRendererOutputSize(game.renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart;

    rectangle_t btn_retour,btn_commencer;
    creer_rectangle(&btn_retour,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2,98, 23, 8, 255,"Retour");
    creer_rectangle(&btn_commencer,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran + HAUTEUR_BOUTON) / 2,58, 90, 64, 255,"Commencer");
    int running = 1;
    SDL_Event event;
    int longueurPseudo = 0;
    SDL_Color couleurTexte = {255, 255, 255, 255};
    char texteComplet[LONGUEUR_MAX_PSEUDO + 10]; 

    SDL_StartTextInput();  
    while (running) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && longueurPseudo > 0) {
                    pseudo[--longueurPseudo] = '\0';
                }
            }
            else if (event.type == SDL_TEXTINPUT) {
                if ((longueurPseudo < LONGUEUR_MAX_PSEUDO - 1) && event.text.text[0]) {
                    strncat(pseudo, event.text.text, 1);
                    longueurPseudo++;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x, y = event.button.y;
                if (x >= btn_retour.rect.x && x <= btn_retour.rect.x + btn_retour.rect.w && y >= btn_retour.rect.y && y <= btn_retour.rect.y + btn_retour.rect.h) {
                    pseudo[0] = '\0';
                    running = 0;
                }
                if (x >= btn_commencer.rect.x && x <= btn_commencer.rect.x + btn_commencer.rect.w && y >= btn_commencer.rect.y && y <= btn_commencer.rect.y + btn_commencer.rect.h && strlen(pseudo) > 0){
                    //si la sauv n'existe pas
                    if(!recuperation_joueur(j,pseudo)){
                        if(afficher_choix_sexe(j, pseudo)) {
                            init_partie(j,pseudo,j->sexe);
                            return 1;
                        }
                        
                    }
                    else{
                        if(afficher_choix_suppression(j,pseudo)){       //retoune 1 si le joueur veut commence le jeu 0 et donc quitte le menu
                            return 1;
                        }
                    }
                }
            }
        }

        if(running){
            SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
            SDL_RenderClear(game.renderer);

            draw_background();
            draw_all_rect(2, &btn_retour, &btn_commencer);

            
            if (strlen(pseudo) >= 0) {  
                
                sprintf(texteComplet, "Pseudo : %s", pseudo);  

                SDL_Surface* surfaceTexte = TTF_RenderUTF8_Solid(game.police, texteComplet, couleurTexte);
                if (!surfaceTexte) {
                    printf("Erreur de rendu texte : %s\n", TTF_GetError());
                } else {
                    SDL_Texture* textureTexte = SDL_CreateTextureFromSurface(game.renderer, surfaceTexte);
                    if (textureTexte) {
                        SDL_Rect rectTexte = {
                            (largeurEcran - surfaceTexte->w) / 2,
                            hauteurEcran / 2 - 50,
                            surfaceTexte->w,
                            surfaceTexte->h
                        };
                        SDL_RenderCopy(game.renderer, textureTexte, NULL, &rectTexte);
                        SDL_DestroyTexture(textureTexte);
                    }
                    SDL_FreeSurface(surfaceTexte);
                }
            }      
            SDL_RenderPresent(game.renderer);
            frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

            if (FRAME_DELAY > frameTime) {
                SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
            }
        }
            
        
    }
    SDL_StopTextInput();  
    return 0;
}

/**
 * @brief Affiche un menu permettant de régler le volume sonore du jeu (Pour l'instant).
 *
 * Cette fonction présente une interface graphique avec les éléments suivants :
 * - Un bouton pour diminuer le volume ("+").
 * - Un bouton pour augmenter le volume ("+").
 * - Un bouton "Retour" permettant de quitter le menu des réglages.
 *
 * L'utilisateur peut cliquer sur ces boutons pour augmenter ou diminuer le volume par palier de 10 unités,
 * dans une plage comprise entre 0 et 100. Le volume modifié est directement enregistré dans la structure `parametres`.
 *
 * @param game        Pointeur vers la structure du jeu contenant le renderer SDL.
 * @param parametres  Pointeur vers une structure contenant les paramètres du jeu, notamment le volume.
 *
 * @pre Le renderer (`game->renderer`) et la police (`game->police`) doivent être initialisés avant l'appel.
 *      Les pointeurs `game` et `parametres` doivent être valides.
 * @post Le volume sonore du jeu est mis à jour selon l'interaction utilisateur.
 *
 * @note Cette fonction gère intégralement sa propre boucle d'événements et réalise l'affichage en continu.
 *       Le bouton "Retour" permet de quitter le menu et revenir à l'écran précédent.
 *
 * @warning Assurez-vous que les textures générées durant l'affichage (`SDL_Surface` et `SDL_Texture`) soient correctement libérées après utilisation pour éviter les fuites mémoire.
 */
void afficher_reglage(parametre_t* parametres) {
    int largeurEcran, hauteurEcran,frameTime;;
    SDL_GetRendererOutputSize(game.renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart; //obtien l'heure
    rectangle_t btn_retour,btn_moins_volume,btn_plus_volume;
    char buffer[50];
    SDL_Color couleurTexte = {255, 255, 255, 255};
   

    creer_rectangle(&btn_retour,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, hauteurEcran / 2 + 100,98, 23, 8, 255,"Retour");
    creer_rectangle(&btn_moins_volume, 50, 50, largeurEcran / 2 - 150, hauteurEcran / 2 - 60, 255, 0, 0, 255,"-");
    creer_rectangle(&btn_plus_volume, 50, 50, largeurEcran / 2 + 100, hauteurEcran / 2 - 60, 0, 255, 0, 255,"+");
    
    int running = 1;
    SDL_Event event;
    
    while (running) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x, y = event.button.y;

                // Bouton Retour
                if (x >= btn_retour.rect.x && x <= btn_retour.rect.x + btn_retour.rect.w &&
                    y >= btn_retour.rect.y && y <= btn_retour.rect.y + btn_retour.rect.h) {
                    running = 0;
                }

                // Ajuster le volume
                if (x >= btn_moins_volume.rect.x && x <= btn_moins_volume.rect.x + btn_moins_volume.rect.w &&
                    y >= btn_moins_volume.rect.y && y <= btn_moins_volume.rect.y + btn_moins_volume.rect.h) {
                    if (parametres->volume > 0) parametres->volume -= 10;
                }
                if (x >= btn_plus_volume.rect.x && x <= btn_plus_volume.rect.x + btn_plus_volume.rect.w &&
                    y >= btn_plus_volume.rect.y && y <= btn_plus_volume.rect.y + btn_plus_volume.rect.h) {
                    if (parametres->volume < 100) parametres->volume += 10;
                }

                int dernier_volume = -1;
                if (parametres->volume != dernier_volume) {  
                    dernier_volume = parametres->volume;  
                    regler_volume(parametres);  // Applique le nouveau volume pour les musiques
                }
            }
        }
        if(running) {
        SDL_RenderClear(game.renderer);
        draw_background();
        draw_all_rect(3, &btn_retour, &btn_moins_volume, &btn_plus_volume);
        
        
        // Volume
        sprintf(buffer, "Volume: %d", parametres->volume);
        SDL_Surface* surfaceVolume = TTF_RenderText_Solid(game.police, buffer, couleurTexte);
        SDL_Texture* textureVolume = SDL_CreateTextureFromSurface(game.renderer, surfaceVolume);
        SDL_Rect rectVolume ={(largeurEcran - surfaceVolume->w) / 2, hauteurEcran / 2 - 50, surfaceVolume->w, surfaceVolume->h};
        SDL_RenderCopy(game.renderer, textureVolume, NULL, &rectVolume);
        SDL_FreeSurface(surfaceVolume);
        SDL_DestroyTexture(textureVolume);
        
        SDL_RenderPresent(game.renderer);
        
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
        }
        
    }
}

/**
 * @brief Affiche un écran d'informations sur les contrôles du jeu.
 *
 * Cette fonction présente un écran listant les touches de contrôle disponibles pour le joueur.
 * Chaque contrôle est affiché dans un rectangle spécifique avec son explication correspondante.
 * Un bouton « Retour » permet au joueur de quitter l'écran d'informations et revenir à l'écran précédent.
 *
 * Les informations affichées comprennent :
 *  - Déplacement vers le haut, bas, gauche, droite.
 *  - Pause du jeu.
 *  - Interaction avec des éléments du jeu.
 *
 * L’écran reste affiché jusqu’à ce que le joueur clique sur "Retour" ou ferme la fenêtre.
 *
 * @param game Pointeur vers la structure du jeu contenant le renderer SDL utilisé pour l’affichage.
 *
 * @pre Le renderer (`game->renderer`) et la police (`game->police`) doivent être initialisés.
 *      Le pointeur `game` doit être valide.
 *
 * @post Aucun changement d’état du jeu n’est effectué en dehors de l’affichage des informations.
 *
 * @note Cette fonction gère elle-même la boucle d'événements et l’affichage continu de l’écran d’information.
 */
void afficher_informations(){
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game.renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart = SDL_GetTicks(); //obtien l'heure
    int frameTime;

    rectangle_t Info_1,Info_2,Info_3,Info_4,Info_5,Info_6, Info_7 ,btn_retour;
    creer_rectangle(&Info_1,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 100 ,255, 255, 255, 100,"Pour aller en haut : fleche du haut");
    creer_rectangle(&Info_2,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 150 ,255, 255, 255, 100,"Pour aller en bas : fleche du bas");
    creer_rectangle(&Info_3,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 200 ,255, 255, 255, 100,"Pour aller a droite : fleche de droite");
    creer_rectangle(&Info_4,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 250 ,255, 255, 255, 100,"Pour aller a gauche : fleche de gauche");
    creer_rectangle(&Info_5,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 300 ,255, 255, 255, 100,"Pour faire pause : echap");
    creer_rectangle(&Info_6,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 350,255, 255, 255, 100,"Pour interagire : A");
    creer_rectangle(&Info_7,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 400,255, 255, 255, 100,"Pour parler a un pnj : P");
    creer_rectangle(&btn_retour,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, 450 ,98, 23, 8, 255,"Retour");

    int running = 1;
    SDL_Event event;

    while (running) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x, y = event.button.y;
                if (x >= btn_retour.rect.x && x <= btn_retour.rect.x + btn_retour.rect.w &&
                    y >= btn_retour.rect.y && y <= btn_retour.rect.y + btn_retour.rect.h) {
                    running = 0;
                } 
            }
        }
        if(running) {
            SDL_RenderClear(game.renderer);
        draw_background();
        draw_all_rect(8, &Info_1, &Info_2, &Info_3, &Info_4, &Info_5, &Info_6, &Info_7 ,&btn_retour);
        SDL_RenderPresent(game.renderer);

        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
        }
        
    }
}

/**
 * @brief Affiche l'écran des paramètres généraux du jeu avec les options Informations, Réglage et Retour.
 *
 * Cette fonction présente trois boutons interactifs :
 * - "Informations" : affiche l'écran expliquant les contrôles du jeu.
 * - "Réglage" : affiche l'écran permettant d'ajuster des paramètres (comme le volume sonore).
 * - "Retour" : permet de quitter l'écran des paramètres et revenir au menu précédent.
 *
 * L'utilisateur interagit avec ces boutons via la souris. Chaque clic entraîne l'affichage de l'écran correspondant,
 * sauf "Retour" qui termine immédiatement la fonction en cours.
 *
 * @param game        Pointeur vers la structure du jeu contenant le renderer SDL.
 * @param parametres  Pointeur vers une structure contenant les paramètres modifiables du jeu.
 *
 * @pre Le renderer (`game->renderer`) et la police (`game->police`) doivent être initialisés avant l'appel.
 *      Les pointeurs `game` et `parametres` doivent être valides.
 *
 * @post Aucun paramètre n’est directement modifié par cette fonction, mais les sous-menus appelés peuvent changer les valeurs de la structure `parametres`.
 *
 * @note La fonction gère une boucle d'événements SDL et effectue le rendu graphique en continu tant que l'écran des paramètres est actif.
 *       Le framerate est régulé afin d’assurer un affichage fluide.
 */
void aff_parametre(parametre_t* parametres){
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game.renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart; //obtien l'heure
    int frameTime;

    rectangle_t btn_informations,btn_reglage,btn_retour;
    creer_rectangle(&btn_informations,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran - HAUTEUR_BOUTON * 2) / 2,58, 90, 64, 255,"Informations");
    creer_rectangle(&btn_reglage,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran + HAUTEUR_BOUTON) / 2,77, 144, 142, 255,"Reglage");
    creer_rectangle(&btn_retour,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2,98, 23, 8, 255,"Retour");
    // INITIALISATION DES BOUTONS
    
    int running = 1;
    SDL_Event event;

    while (running) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x, y = event.button.y;
                if (x >= btn_informations.rect.x && x <= btn_informations.rect.x + btn_informations.rect.w &&
                    y >= btn_informations.rect.y && y <= btn_informations.rect.y + btn_informations.rect.h) {
                    afficher_informations();
                }
                if (x >= btn_reglage.rect.x && x <= btn_reglage.rect.x + btn_reglage.rect.w &&
                    y >= btn_reglage.rect.y && y <= btn_reglage.rect.y + btn_reglage.rect.h) {
                    afficher_reglage(parametres);
                }

                if (x >= btn_retour.rect.x && x <= btn_retour.rect.x + btn_retour.rect.w &&
                    y >= btn_retour.rect.y && y <= btn_retour.rect.y + btn_retour.rect.h) {
                        running = 0;
                }
            }
        }
        if(running) {
            //AFFICHAGE
            SDL_RenderClear(game.renderer);

            draw_background();
            draw_all_rect(3, &btn_informations, &btn_reglage, &btn_retour);
            SDL_RenderPresent(game.renderer);

            frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

            if (FRAME_DELAY > frameTime) {
                SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
            }
        }

        
    }
}

/**
 * @brief Affiche le menu principal du jeu, permettant au joueur de démarrer, régler les paramètres ou quitter.
 *
 * Cette fonction affiche le menu principal du jeu avec trois boutons interactifs :
 * - "Jouer" : pour saisir ou choisir un pseudo et démarrer une partie.
 * - "Parametres" : pour accéder aux réglages du jeu tels que le volume sonore et les contrôles.
 * - "Quitter" : pour fermer immédiatement l'application.
 *
 * La fonction gère la boucle principale des événements (clics souris) jusqu'à ce que le joueur choisisse de quitter,
 * démarre une partie valide ou ferme la fenêtre du jeu.
 *
 * @param game        Pointeur vers la structure principale du jeu (renderer, textures, état actuel du jeu).
 * @param parametres  Pointeur vers la structure contenant les paramètres ajustables du jeu (par exemple, le volume).
 * @param j           Pointeur vers la structure du joueur pour sauvegarder ou charger les données de la partie.
 * @param pseudo      Chaîne de caractères utilisée pour stocker le pseudo saisi par le joueur.
 *
 * @pre Toutes les ressources graphiques (renderer, police, textures) doivent être initialisées. 
 *      Les pointeurs passés (`game`, `parametres`, `j`, `pseudo`) doivent être valides.
 *
 * @post L'état du jeu peut changer en fonction de l'action sélectionnée par l'utilisateur.
 *       Le pseudo peut être saisi ou réinitialisé, les paramètres ajustés, ou le jeu fermé.
 *
 * @note Cette fonction gère intégralement sa propre boucle d'événements SDL et les rendus graphiques associés.
 *       Elle régule également le framerate pour un affichage fluide de l'interface.
 */
void afficher_menu(parametre_t* parametres, joueur_t* j, char* pseudo) {
    game.mat_active = 7;
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game.renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart; //obtien l'heure
    int frameTime;
    int x, y;

    rectangle_t btn_jouer,btn_parametres,btn_quitter;
    creer_rectangle(&btn_jouer,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran - HAUTEUR_BOUTON * 2) / 2,58, 90, 64, 255,"Jouer");
    creer_rectangle(&btn_parametres,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran + HAUTEUR_BOUTON) / 2,77, 144, 142, 255,"Parametres");
    creer_rectangle(&btn_quitter,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2,98, 23, 8, 255,"Quitter");
    // INITIALISATION DES BOUTONS
    
    int running = 1;
    SDL_Event event;

    while (running) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                x = event.button.x; 
                y = event.button.y;
                if (x >= btn_jouer.rect.x && x <= btn_jouer.rect.x + btn_jouer.rect.w && y >= btn_jouer.rect.y && y <= btn_jouer.rect.y + btn_jouer.rect.h) {
                    if(aff_saisie_pseudo(j, pseudo)){    //si la partie s'est lancé
                        running = 0;
                    }
                }
                if (x >= btn_parametres.rect.x && x <= btn_parametres.rect.x + btn_parametres.rect.w &&
                    y >= btn_parametres.rect.y && y <= btn_parametres.rect.y + btn_parametres.rect.h) {
                    aff_parametre(parametres);
                }

                if (x >= btn_quitter.rect.x && x <= btn_quitter.rect.x + btn_quitter.rect.w &&
                    y >= btn_quitter.rect.y && y <= btn_quitter.rect.y + btn_quitter.rect.h) {
                        running = 0;
                }
            }
        }

        if(running) {

            //AFFICHAGE
            SDL_RenderClear(game.renderer);

            SDL_RenderCopy(game.renderer, game.backgroundTexture[7], NULL, &game.dms_win);
            draw_all_rect(3, &btn_jouer, &btn_parametres, &btn_quitter);
            SDL_RenderPresent(game.renderer);

            frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame
            if (FRAME_DELAY > frameTime) {
                SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
            }
        }
    }
}

/**
 * @brief Affiche le menu pause du jeu avec les options Reprendre, Paramètres et Accueil.
 *
 * Cette fonction affiche un menu de pause permettant au joueur de choisir entre :
 * - **Reprendre** : pour retourner directement à la partie en cours.
 * - **Parametres** : accéder aux réglages du jeu (volume, contrôles, etc.).
 * - "Accueil" : quitter la partie en cours pour revenir au menu principal.
 *
 * L'utilisateur interagit avec le menu en cliquant sur l'un des boutons affichés.
 * L'action associée est immédiatement déclenchée :
 * - Reprendre la partie.
 * - Accéder aux paramètres.
 * - Revenir au menu principal (Accueil).
 *
 * @param game        Pointeur vers la structure du jeu contenant les ressources nécessaires à l'affichage.
 * @param parametres  Pointeur vers la structure contenant les paramètres ajustables du jeu.
 *
 * @return int
 *         - `1` : Si l'utilisateur choisit "Reprendre", la partie continue.
 *         - `0` : Si l'utilisateur clique sur "Accueil" ou ferme la fenêtre.
 *
 * @pre Le renderer (`game->renderer`) et les ressources graphiques doivent être initialisés avant l'appel.
 *      Les pointeurs `game` et `parametres` doivent être valides.
 *
 * @post Le jeu reprend son cours ou revient au menu d'accueil en fonction de l'option sélectionnée par l'utilisateur.
 *
 * @note La fonction gère entièrement l'affichage graphique et la boucle d'événements SDL associée à l'écran de pause.
 *       Elle maintient un framerate constant grâce à un contrôle du temps d'affichage par frame.
 *
 * @warning Veillez à bien définir les constantes telles que `LARGEUR_BOUTON`, `HAUTEUR_BOUTON`, et `FRAME_DELAY`.
 */
int afficher_menu_pause(parametre_t* parametres) {
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game.renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart; //obtien l'heure
    int frameTime;

    // INITIALISATION DES BOUTONS
    rectangle_t btn_accueil,btn_reprendre,btn_parametres;
    creer_rectangle(&btn_reprendre,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran - HAUTEUR_BOUTON * 2) / 2,58, 90, 64, 255,"Reprendre");
    creer_rectangle(&btn_parametres,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran + HAUTEUR_BOUTON) / 2,77, 144, 142, 255,"Parametres");
    creer_rectangle(&btn_accueil,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2,98, 23, 8, 255,"Menu d'acceuil");
    
    int running = 1;
    SDL_Event event;

    while (running) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x, y = event.button.y;
                if (x >= btn_reprendre.rect.x && x <= btn_reprendre.rect.x + btn_reprendre.rect.w &&
                    y >= btn_reprendre.rect.y && y <= btn_reprendre.rect.y + btn_reprendre.rect.h) {
                    return 1;
                }
                if (x >= btn_parametres.rect.x && x <= btn_parametres.rect.x + btn_parametres.rect.w &&
                    y >= btn_parametres.rect.y && y <= btn_parametres.rect.y + btn_parametres.rect.h) {
                    aff_parametre(parametres);
                }

                if (x >= btn_accueil.rect.x && x <= btn_accueil.rect.x + btn_accueil.rect.w &&
                    y >= btn_accueil.rect.y && y <= btn_accueil.rect.y + btn_accueil.rect.h) {
                    return 0;
                }
            }
        }

        //AFFICHAGE
        //SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game.renderer);
        draw_background();      
        draw_calque();  
        draw_all_rect(3, &btn_reprendre, &btn_parametres, &btn_accueil);
        SDL_RenderPresent(game.renderer);
        
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }
    return 0;
}




