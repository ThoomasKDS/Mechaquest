#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/menu.h"
#include "../lib/initGame.h"
#include "../lib/affichage.h"

SDL_Texture* charger_texture(const char *chemin, game_t *game){
    SDL_Surface *surfaceChargee = IMG_Load(chemin);
    if (!surfaceChargee) {
        printf("Erreur chargement image: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, surfaceChargee);
    SDL_FreeSurface(surfaceChargee);
    return texture;
}

int afficher_choix_sexe(game_t* game, joueur_t* j,char* pseudo){
    rectangle_t rect_h, rect_f;
    int largeurEcran, hauteurEcran, frameTime;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart; //obtien l'heure    
    rectangle_t btn_retour;

    SDL_Texture* sprite_h = charger_texture("img/skin/skin_player_homme/bas1.png", game);
    SDL_Texture* sprite_f = charger_texture("img/skin/skin_player_femme/bas1.png", game);
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
            SDL_RenderClear(game->renderer);
            draw_background(game);
            draw_all_rect(game, 3, &btn_retour, &rect_h, &rect_f);
            SDL_RenderCopy(game->renderer, sprite_h, NULL, &rect_h.rect);
            SDL_RenderCopy(game->renderer, sprite_f, NULL, &rect_f.rect);

            // Affichage des valeurs
            char buffer[50];
            SDL_Color couleurTexte = {255, 255, 255, 255};

            // Affichage du texte : Choisir son sexe
            sprintf(buffer, "Choisir son sexe");
            SDL_Surface* surfaceVolume = TTF_RenderText_Solid(game->police, buffer, couleurTexte);
            SDL_Texture* textureVolume = SDL_CreateTextureFromSurface(game->renderer, surfaceVolume);
            SDL_Rect rectVolume = {(largeurEcran - surfaceVolume->w) / 2, hauteurEcran / 2 - 100, surfaceVolume->w, surfaceVolume->h};
            SDL_RenderCopy(game->renderer, textureVolume, NULL, &rectVolume);
            SDL_FreeSurface(surfaceVolume);
            SDL_DestroyTexture(textureVolume);

            SDL_RenderPresent(game->renderer);
            
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

int afficher_choix_suppression(game_t* game, joueur_t* j,char* pseudo){
    int largeurEcran, hauteurEcran,frameTime;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
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
                    if(afficher_choix_sexe(game, j, pseudo)) {
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
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            SDL_RenderClear(game->renderer);
            
            draw_background(game);
            draw_all_rect(game, 3, &btn_retour, &btn_reprendre, &btn_recommencer);
            SDL_RenderPresent(game->renderer);
            
            frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

            if (FRAME_DELAY > frameTime) {
                SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
            }
        }
     }    
    return 0;
}

//affiche quand le joueur tape son pseudo
int aff_saisie_pseudo(game_t* game, joueur_t* j, char* pseudo) {
    int largeurEcran, hauteurEcran,frameTime;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
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
                        if(afficher_choix_sexe(game, j, pseudo)) {
                            init_partie(j,pseudo,j->sexe);
                            return 1;
                        }
                        
                    }
                    else{
                        if(afficher_choix_suppression(game,j,pseudo)){       //retoune 1 si le joueur veut commence le jeu 0 et donc quitte le menu
                            return 1;
                        }
                    }
                }
            }
        }

        if(running){
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            SDL_RenderClear(game->renderer);

            draw_background(game);
            draw_all_rect(game, 2, &btn_retour, &btn_commencer);

            
            if (strlen(pseudo) >= 0) {  
                
                sprintf(texteComplet, "Pseudo : %s", pseudo);  

                SDL_Surface* surfaceTexte = TTF_RenderUTF8_Solid(game->police, texteComplet, couleurTexte);
                if (!surfaceTexte) {
                    printf("Erreur de rendu texte : %s\n", TTF_GetError());
                } else {
                    SDL_Texture* textureTexte = SDL_CreateTextureFromSurface(game->renderer, surfaceTexte);
                    if (textureTexte) {
                        SDL_Rect rectTexte = {
                            (largeurEcran - surfaceTexte->w) / 2,
                            hauteurEcran / 2 - 50,
                            surfaceTexte->w,
                            surfaceTexte->h
                        };
                        SDL_RenderCopy(game->renderer, textureTexte, NULL, &rectTexte);
                        SDL_DestroyTexture(textureTexte);
                    }
                    SDL_FreeSurface(surfaceTexte);
                }
            }      
            SDL_RenderPresent(game->renderer);
            frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

            if (FRAME_DELAY > frameTime) {
                SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
            }
        }
            
        
    }
    SDL_StopTextInput();  
    return 0;
}

//on modifie
void afficher_reglage(game_t* game, parametre_t* parametres) {
    int largeurEcran, hauteurEcran,frameTime;;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
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
            }
        }
        if(running) {
        SDL_RenderClear(game->renderer);
        draw_background(game);
        draw_all_rect(game, 3, &btn_retour, &btn_moins_volume, &btn_plus_volume);
        
        
        // Volume
        sprintf(buffer, "Volume: %d", parametres->volume);
        SDL_Surface* surfaceVolume = TTF_RenderText_Solid(game->police, buffer, couleurTexte);
        SDL_Texture* textureVolume = SDL_CreateTextureFromSurface(game->renderer, surfaceVolume);
        SDL_Rect rectVolume ={(largeurEcran - surfaceVolume->w) / 2, hauteurEcran / 2 - 50, surfaceVolume->w, surfaceVolume->h};
        SDL_RenderCopy(game->renderer, textureVolume, NULL, &rectVolume);
        SDL_FreeSurface(surfaceVolume);
        SDL_DestroyTexture(textureVolume);
        
        SDL_RenderPresent(game->renderer);
        
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
        }
        
    }
}

void afficher_informations(game_t* game){
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart = SDL_GetTicks(); //obtien l'heure
    int frameTime;

    rectangle_t Info_1,Info_2,Info_3,Info_4,Info_5,Info_6,btn_retour;
    creer_rectangle(&Info_1,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 100 ,255, 255, 255, 100,"Pour aller en haut : fleche du haut");
    creer_rectangle(&Info_2,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 150 ,255, 255, 255, 100,"Pour aller en bas : fleche du bas");
    creer_rectangle(&Info_3,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 200 ,255, 255, 255, 100,"Pour aller a droite : fleche de droite");
    creer_rectangle(&Info_4,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 250 ,255, 255, 255, 100,"Pour aller a gauche : fleche de gauche");
    creer_rectangle(&Info_5,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 300 ,255, 255, 255, 100,"Pour faire pause : echap");
    creer_rectangle(&Info_6,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 350,255, 255, 255, 100,"Pour interagire : A");
    creer_rectangle(&btn_retour,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, 400 ,98, 23, 8, 255,"Retour");

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
            SDL_RenderClear(game->renderer);
        draw_background(game);
        draw_all_rect(game, 7, &Info_1, &Info_2, &Info_3, &Info_4, &Info_5, &Info_6, &btn_retour);
        SDL_RenderPresent(game->renderer);

        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
        }
        
    }
}

void aff_parametre(game_t* game, parametre_t* parametres){
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
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
                    afficher_informations(game);
                }
                if (x >= btn_reglage.rect.x && x <= btn_reglage.rect.x + btn_reglage.rect.w &&
                    y >= btn_reglage.rect.y && y <= btn_reglage.rect.y + btn_reglage.rect.h) {
                    afficher_reglage(game, parametres);
                }

                if (x >= btn_retour.rect.x && x <= btn_retour.rect.x + btn_retour.rect.w &&
                    y >= btn_retour.rect.y && y <= btn_retour.rect.y + btn_retour.rect.h) {
                        running = 0;
                }
            }
        }
        if(running) {
            //AFFICHAGE
            SDL_RenderClear(game->renderer);

            draw_background(game);
            draw_all_rect(game, 3, &btn_informations, &btn_reglage, &btn_retour);
            SDL_RenderPresent(game->renderer);

            frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

            if (FRAME_DELAY > frameTime) {
                SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
            }
        }

        
    }
}

void afficher_menu(game_t* game, parametre_t* parametres, joueur_t* j, char* pseudo) {
    game->mat_active = 7;
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
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
                    if(aff_saisie_pseudo(game, j, pseudo)){    //si la partie s'est lancé
                        running = 0;
                    }
                }
                if (x >= btn_parametres.rect.x && x <= btn_parametres.rect.x + btn_parametres.rect.w &&
                    y >= btn_parametres.rect.y && y <= btn_parametres.rect.y + btn_parametres.rect.h) {
                    aff_parametre(game, parametres);
                }

                if (x >= btn_quitter.rect.x && x <= btn_quitter.rect.x + btn_quitter.rect.w &&
                    y >= btn_quitter.rect.y && y <= btn_quitter.rect.y + btn_quitter.rect.h) {
                        running = 0;
                }
            }
        }

        if(running) {

            //AFFICHAGE
            SDL_RenderClear(game->renderer);

            SDL_RenderCopy(game->renderer, game->backgroundTexture[7], NULL, &game->dms_win);
            draw_all_rect(game, 3, &btn_jouer, &btn_parametres, &btn_quitter);
            SDL_RenderPresent(game->renderer);

            frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame
            if (FRAME_DELAY > frameTime) {
                SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
            }
        }
    }
}

int afficher_menu_pause(game_t* game, parametre_t* parametres) {
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
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
                    aff_parametre(game, parametres);
                }

                if (x >= btn_accueil.rect.x && x <= btn_accueil.rect.x + btn_accueil.rect.w &&
                    y >= btn_accueil.rect.y && y <= btn_accueil.rect.y + btn_accueil.rect.h) {
                    return 0;
                }
            }
        }

        //AFFICHAGE
        //SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        draw_background(game);        
        draw_all_rect(game, 3, &btn_reprendre, &btn_parametres, &btn_accueil);
        SDL_RenderPresent(game->renderer);
        
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }
    return 0;
}

int afficher_dialogue(game_t *game, joueur_t *j, SDL_Rect *sprite_p, SDL_Rect *pnj_sprite, img_pnj_t *sprite_pnj, img_player_t *sprite_playerH, char *pseudo, char *dialogue, int choix) {
    if (!game || !pseudo || !dialogue) return ERR;
    
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
    
    int dialogueWidth = largeurEcran * 3 / 4;
    int dialogueHeight = hauteurEcran / 4;
    int dialogueX = (largeurEcran - dialogueWidth) / 2;
    int dialogueY = hauteurEcran - dialogueHeight - 20;
    
    rectangle_t fondDialogue, textRect, pseudoRect, infoRect;
    
    creer_rectangle(&fondDialogue, dialogueWidth, dialogueHeight, dialogueX, dialogueY, 0, 0, 0, 180, "");
    creer_rectangle(&pseudoRect, 200, 40, dialogueX + 20, dialogueY + 10, 255, 255, 255, 255, pseudo);
    creer_rectangle(&textRect, dialogueWidth - 40, dialogueHeight - 80, dialogueX + 20, dialogueY + 60, 255, 255, 255, 255, "");
    
    if (!choix)
        creer_rectangle(&infoRect, 300, 30, dialogueX + (dialogueWidth / 2) - 150, dialogueY + dialogueHeight - 35, 255, 255, 255, 255, "A pour continuer");
    else
        creer_rectangle(&infoRect, 300, 30, dialogueX + (dialogueWidth / 2) - 150, dialogueY + dialogueHeight - 35, 255, 255, 255, 255, "Choisir entre 1 et 3");

    int len = strlen(dialogue);
    char displayedText[256] = "";
    int index = 0, textIndex = 0;
    bool waitingForKey = false;
    SDL_Event event;
    Uint32 frameStart, frameTime;
    
    SDL_FlushEvent(SDL_KEYDOWN);  // Vider les touches avant d'afficher
    
    while (index < len) {
        frameStart = SDL_GetTicks();
        displayedText[textIndex] = dialogue[index];
        displayedText[textIndex + 1] = '\0';
        strncpy(textRect.text, displayedText, sizeof(textRect.text) - 1);
        SDL_RenderClear(game->renderer);
        draw_all(game, j, sprite_p, pnj_sprite, sprite_pnj, sprite_playerH);
        draw_rect(game, &fondDialogue, draw_text_center);
        draw_rect(game, &pseudoRect, draw_text_center);
        draw_rect(game, &textRect, draw_text_left_middle);
        draw_rect(game, &infoRect, draw_text_center);
        SDL_RenderPresent(game->renderer);
        
        int delay = (dialogue[index] == '.' || dialogue[index] == '!' || dialogue[index] == '?') ? 500 : 10;
        waitingForKey = (dialogue[index] == '.' || dialogue[index] == '!' || dialogue[index] == '?');
        index++;
        textIndex++;

        if (waitingForKey == true) {
            SDL_FlushEvent(SDL_KEYDOWN);  // Éviter la répétition du dernier appui
            
            while (waitingForKey == true) {
                if (SDL_WaitEvent(&event)) {
                    if (event.type == SDL_QUIT) return OK;
                    if (!choix && event.type == SDL_KEYDOWN && !event.key.repeat) {
                        if (event.key.keysym.sym == SDLK_a) {
                            waitingForKey = false;
                            textIndex = 0;
                            memset(displayedText, 0, sizeof(displayedText));
                        }
                    } else if (choix && event.type == SDL_KEYDOWN && !event.key.repeat) {
                        if (event.key.keysym.sym == SDLK_1 || event.key.keysym.sym == SDLK_KP_1) return 1;
                        if (event.key.keysym.sym == SDLK_2 || event.key.keysym.sym == SDLK_KP_2) return 2;
                        if (event.key.keysym.sym == SDLK_3 || event.key.keysym.sym == SDLK_KP_3) return 3;
                    }
                }
            }
        }
        
        frameTime = SDL_GetTicks() - frameStart;
        if (delay > frameTime) SDL_Delay(delay - frameTime);
    }
    
    return choix ? choix : OK;
}



