#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/menu.h"
#include "../lib/initGame.h"
#include "../lib/affichage.h"

SDL_Texture* chargerTexture(const char *chemin, game_t *game){
    SDL_Surface *surfaceChargee = IMG_Load(chemin);
    if (!surfaceChargee) {
        printf("Erreur chargement image: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, surfaceChargee);
    SDL_FreeSurface(surfaceChargee);
    return texture;
}

BoutonImage creerBoutonImage(int x, int y, int largeur, int hauteur, char *cheminImage, game_t* game){
    BoutonImage bouton;
    bouton.rect.x = x;
    bouton.rect.y = y;
    bouton.rect.w = largeur;
    bouton.rect.h = hauteur;
    bouton.image = chargerTexture(cheminImage,game);  // Attention : texte doit être une chaîne statique ou allouée dynamiquement
    if (!bouton.image) {
        printf("Erreur lors du chargement de l'image du bouton.\n");
    }
    return bouton;
}

int afficherChoixSexe(game_t* game, joueur_t* j,char* pseudo){
    int largeurEcran, hauteurEcran, frameTime;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart = SDL_GetTicks(); //obtien l'heure

    BoutonImage Homme = creerBoutonImage(largeurEcran / 2 - 350, hauteurEcran / 2 , 200, 250,"img/skin/skin_player_homme/bas1.png",game);
    BoutonImage Femme = creerBoutonImage(largeurEcran / 2 + 150, hauteurEcran / 2 , 200, 250,"img/skin/skin_player_homme/haut1.png",game);
     
    if (!Homme.image || !Femme.image) {
        return -1;
    }
    rectangle_t boutonRetour;
    creer_rectangle(game,&boutonRetour,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2,98, 23, 8, 255,"Retour");
    int enCours = 1, action;

    SDL_Event evenement;
    frameStart = SDL_GetTicks();
    while(enCours){
        while(SDL_PollEvent(&evenement)){
            if (evenement.type == SDL_QUIT) {
                enCours = 0;
            }
            else if(evenement.type == SDL_MOUSEBUTTONDOWN){
                int x = evenement.button.x, y = evenement.button.y;
                
                if (x >= Homme.rect.x && x <= Homme.rect.x + Homme.rect.w &&
                    y >= Homme.rect.y && y <= Homme.rect.y + Homme.rect.h) {
                    action = 1;
                    enCours = 0;
                    j->sexe = 'M';
                }
                else if (x >= Femme.rect.x && x <= Femme.rect.x + Femme.rect.w &&
                    y >= Femme.rect.y && y <= Femme.rect.y + Femme.rect.h) {
                    action = 2;
                    enCours = 0;
                    j->sexe = 'F';
                }
                else if (x >= boutonRetour.rect.x && x <= boutonRetour.rect.x + boutonRetour.rect.w &&
                    y >= boutonRetour.rect.y && y <= boutonRetour.rect.y + boutonRetour.rect.h) {
                    action = 0;
                    enCours = 0;
                    pseudo[0] = '\0';
                }
            }
        }
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);

        SDL_RenderCopy(game->renderer, game->backgroundTexture[7], NULL, &game->dms_win);
        draw_rect(game, &boutonRetour);

        SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(game->renderer, &Homme.rect);
        SDL_RenderFillRect(game->renderer, &Femme.rect);

        // Définir la zone pour afficher l'image centrée dans le bouton
        SDL_Rect zoneImage1 = {
            .x = Homme.rect.x + (Homme.rect.w - Homme.rect.w * 0.8) / 2,
            .y = Homme.rect.y + (Homme.rect.h - Homme.rect.h * 0.8) / 2,
            .w = Homme.rect.w * 0.8,
            .h = Homme.rect.h * 0.8
        };
        SDL_Rect zoneImage2 = {
            .x = Femme.rect.x + (Femme.rect.w - Femme.rect.w * 0.8) / 2,
            .y = Femme.rect.y + (Femme.rect.h - Femme.rect.h * 0.8) / 2,
            .w = Femme.rect.w * 0.8,
            .h = Femme.rect.h * 0.8
        };

        SDL_RenderCopy(game->renderer, Homme.image, NULL, &zoneImage1);
        SDL_RenderCopy(game->renderer, Femme.image, NULL, &zoneImage2);

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
    SDL_DestroyTexture(Homme.image);
    SDL_DestroyTexture(Femme.image);
    return action;
}

int afficherChoixSuppression(game_t* game, joueur_t* j,char* pseudo){
    int largeurEcran, hauteurEcran,action = 0,frameTime;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart = SDL_GetTicks(); //obtien l'heure

    rectangle_t boutonRetour,boutonOUI,boutonNON;
    creer_rectangle(game,&boutonOUI,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran - HAUTEUR_BOUTON * 2) / 2,58, 90, 64, 255,"Reprendre");
    creer_rectangle(game,&boutonNON,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran + HAUTEUR_BOUTON) / 2,77, 144, 142, 255,"Recommencer");
    creer_rectangle(game,&boutonRetour,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2,98, 23, 8, 255,"Retour");
    
    int enCours = 1;
    SDL_Event evenement;
    
    while (enCours) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&evenement)) {
            if (evenement.type == SDL_QUIT) {
                enCours = 0;
            } else if (evenement.type == SDL_MOUSEBUTTONDOWN) {
                int x = evenement.button.x, y = evenement.button.y;

                // Bouton Retour
                if (x >= boutonRetour.rect.x && x <= boutonRetour.rect.x + boutonRetour.rect.w &&
                    y >= boutonRetour.rect.y && y <= boutonRetour.rect.y + boutonRetour.rect.h) {
                    pseudo[0] = '\0';
                    return 0;
                }

                // Bouton Oui
                if (x >= boutonOUI.rect.x && x <= boutonOUI.rect.x + boutonOUI.rect.w &&
                    y >= boutonOUI.rect.y && y <= boutonOUI.rect.y + boutonOUI.rect.h) {
                    return 1;
                }
                // Bouton Non
                if (x >= boutonNON.rect.x && x <= boutonNON.rect.x + boutonNON.rect.w &&
                    y >= boutonNON.rect.y && y <= boutonNON.rect.y + boutonNON.rect.h) {
                    return 0;
                }
            }
        }

        // Affichage
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);

        SDL_RenderCopy(game->renderer, game->backgroundTexture[7], NULL, &game->dms_win);
        draw_rect(game, &boutonRetour);
        draw_rect(game, &boutonOUI);
        draw_rect(game, &boutonNON);

        SDL_RenderPresent(game->renderer);
        
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
     }    
    return action;
}

//on modifie
int afficherSaisiePseudo(game_t* game, joueur_t* j, char* pseudo) {
    int largeurEcran, hauteurEcran,resultatFonction,frameTime;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart = SDL_GetTicks(); //obtien l'heure

    rectangle_t boutonRetour,boutonCommencer;
    creer_rectangle(game,&boutonRetour,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2,98, 23, 8, 255,"Retour");
    creer_rectangle(game,&boutonCommencer,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran + HAUTEUR_BOUTON) / 2,58, 90, 64, 255,"Commencer");
    int enCours = 1, action;
    SDL_Event evenement;
    int longueurPseudo = strlen(pseudo);

    SDL_StartTextInput();  
    while (enCours) {
        while (SDL_PollEvent(&evenement)) {
            if (evenement.type == SDL_QUIT) {
                enCours = 0;
            }
            else if (evenement.type == SDL_KEYDOWN) {
                if (evenement.key.keysym.sym == SDLK_BACKSPACE && longueurPseudo > 0) {
                    pseudo[--longueurPseudo] = '\0';
                }
            }
            else if (evenement.type == SDL_TEXTINPUT) {
                if ((longueurPseudo < LONGUEUR_MAX_PSEUDO - 1) && evenement.text.text[0]) {
                    strncat(pseudo, evenement.text.text, 1);
                    longueurPseudo++;
                }
            }
            else if (evenement.type == SDL_MOUSEBUTTONDOWN) {
                int x = evenement.button.x, y = evenement.button.y;
                if (x >= boutonRetour.rect.x && x <= boutonRetour.rect.x + boutonRetour.rect.w &&
                    y >= boutonRetour.rect.y && y <= boutonRetour.rect.y + boutonRetour.rect.h) {
                    enCours = 0;
                    action = 0;
                }
                if (x >= boutonCommencer.rect.x && x <= boutonCommencer.rect.x + boutonCommencer.rect.w &&
                    y >= boutonCommencer.rect.y && y <= boutonCommencer.rect.y + boutonCommencer.rect.h && 
                    strlen(pseudo) > 0){
                    enCours = 0;
                    action = 1;
                    //si elle n'as pas été retrouvé
                    if(!recuperation_joueur(j,pseudo)){
                        afficherChoixSexe(game, j, pseudo);
                        init_partie(j,pseudo,j->sexe);
                    }
                    //si on a appuié sur le bouton retour
                    else if(pseudo[0] == '\0')
                        enCours = 1;
                    //si on l'a retrouvé
                    else{
                        resultatFonction = afficherChoixSuppression(game,j,pseudo);
                        //bouton retour
                        if(!resultatFonction && pseudo[0] == '\0')
                            enCours = 1;
                        //bouton continuer
                        else if(resultatFonction);
                        //bouton recommencer
                        else{
                            suppression_partie(j,pseudo);
                            afficherChoixSexe(game, j, pseudo);
                            init_partie(j,pseudo,j->sexe);
                        }
                    }
                }
            }

            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            SDL_RenderClear(game->renderer);

            SDL_RenderCopy(game->renderer, game->backgroundTexture[7], NULL, &game->dms_win);
            draw_rect(game, &boutonRetour);
            draw_rect(game, &boutonCommencer);

            SDL_Color couleurTexte = {255, 255, 255, 255};
            if (strlen(pseudo) >= 0) {  
                char texteComplet[LONGUEUR_MAX_PSEUDO + 10];  
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
    if (strlen(pseudo) >= 0 && !action)
                pseudo[0] = '\0';
    SDL_StopTextInput();  
    return action;
}

//on modifie
void afficherReglage(game_t* game, parametre_t* parametres) {
    int largeurEcran, hauteurEcran,frameTime;;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart = SDL_GetTicks(); //obtien l'heure
    rectangle_t boutonRetour,boutonMoinsVolume,boutonPlusVolume;

    creer_rectangle(game, &boutonRetour,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, hauteurEcran / 2 + 100,98, 23, 8, 255,"Retour");
    creer_rectangle(game, &boutonMoinsVolume, 50, 50, largeurEcran / 2 - 150, hauteurEcran / 2 - 60, 0, 255, 0, 255,"+");
    creer_rectangle(game, &boutonPlusVolume, 50, 50, largeurEcran / 2 + 100, hauteurEcran / 2 - 60, 255, 0, 0, 255,"-");
    
    int enCours = 1;
    SDL_Event evenement;
    
    while (enCours) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&evenement)) {
            if (evenement.type == SDL_QUIT) {
                enCours = 0;
            } else if (evenement.type == SDL_MOUSEBUTTONDOWN) {
                int x = evenement.button.x, y = evenement.button.y;

                // Bouton Retour
                if (x >= boutonRetour.rect.x && x <= boutonRetour.rect.x + boutonRetour.rect.w &&
                    y >= boutonRetour.rect.y && y <= boutonRetour.rect.y + boutonRetour.rect.h) {
                    return;
                }

                // Ajuster le volume
                if (x >= boutonMoinsVolume.rect.x && x <= boutonMoinsVolume.rect.x + boutonMoinsVolume.rect.w &&
                    y >= boutonMoinsVolume.rect.y && y <= boutonMoinsVolume.rect.y + boutonMoinsVolume.rect.h) {
                    if (parametres->volume > 0) parametres->volume -= 10;
                }
                if (x >= boutonPlusVolume.rect.x && x <= boutonPlusVolume.rect.x + boutonPlusVolume.rect.w &&
                    y >= boutonPlusVolume.rect.y && y <= boutonPlusVolume.rect.y + boutonPlusVolume.rect.h) {
                    if (parametres->volume < 100) parametres->volume += 10;
                }
            }
        }

        // Affichage
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        SDL_RenderCopy(game->renderer, game->backgroundTexture[7], NULL, &game->dms_win);
        draw_rect(game, &boutonRetour);
        draw_rect(game, &boutonMoinsVolume);
        draw_rect(game, &boutonPlusVolume);
        
        // Affichage des valeurs
        char buffer[50];
        SDL_Color couleurTexte = {255, 255, 255, 255};
        
        // Volume
        sprintf(buffer, "Volume: %d", parametres->volume);
        SDL_Surface* surfaceVolume = TTF_RenderText_Solid(game->police, buffer, couleurTexte);
        SDL_Texture* textureVolume = SDL_CreateTextureFromSurface(game->renderer, surfaceVolume);
        SDL_Rect rectVolume = {(largeurEcran - surfaceVolume->w) / 2, hauteurEcran / 2 - 50, surfaceVolume->w, surfaceVolume->h};
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

void afficherInformations(game_t* game){
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart = SDL_GetTicks(); //obtien l'heure
    int frameTime;

    rectangle_t Info_1,Info_2,Info_3,Info_4,Info_5,Info_6,boutonRetour;
    creer_rectangle(game,&Info_1,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 100 ,255, 255, 255, 100,"Pour aller en haut : fleche du haut");
    creer_rectangle(game,&Info_2,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 150 ,255, 255, 255, 100,"Pour aller en bas : fleche du bas");
    creer_rectangle(game,&Info_3,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 200 ,255, 255, 255, 100,"Pour aller a droite : fleche de droite");
    creer_rectangle(game,&Info_4,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 250 ,255, 255, 255, 100,"Pour aller a gauche : fleche de gauche");
    creer_rectangle(game,&Info_5,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 300 ,255, 255, 255, 100,"Pour faire pause : echap");
    creer_rectangle(game,&Info_6,750,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2 - 275, 350,255, 255, 255, 100,"Pour interagire : I");
    creer_rectangle(game,&boutonRetour,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, 400 ,98, 23, 8, 255,"Retour");

    int running = 1, action;
    SDL_Event event;

    while (running) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x, y = event.button.y;
                if (x >= boutonRetour.rect.x && x <= boutonRetour.rect.x + boutonRetour.rect.w &&
                    y >= boutonRetour.rect.y && y <= boutonRetour.rect.y + boutonRetour.rect.h) {
                    running = 0;
                } 
            }
        }
        SDL_RenderClear(game->renderer);

        SDL_RenderCopy(game->renderer, game->backgroundTexture[7], NULL, &game->dms_win);
        draw_rect(game, &Info_1);
        draw_rect(game, &Info_2);
        draw_rect(game, &Info_3);
        draw_rect(game, &Info_4);
        draw_rect(game, &Info_5);
        draw_rect(game, &Info_6);
        draw_rect(game, &boutonRetour);
        SDL_RenderPresent(game->renderer);

        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }
    // INITIALISATION DES BOUTONS
}

void afficherParametre(game_t* game, parametre_t* parametres){
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart = SDL_GetTicks(); //obtien l'heure
    int frameTime;

    rectangle_t boutonInformations,boutonReglage,boutonRetour;
    creer_rectangle(game,&boutonInformations,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran - HAUTEUR_BOUTON * 2) / 2,58, 90, 64, 255,"Informations");
    creer_rectangle(game,&boutonReglage,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran + HAUTEUR_BOUTON) / 2,77, 144, 142, 255,"Reglage");
    creer_rectangle(game,&boutonRetour,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2,98, 23, 8, 255,"Retour");
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
                if (x >= boutonInformations.rect.x && x <= boutonInformations.rect.x + boutonInformations.rect.w &&
                    y >= boutonInformations.rect.y && y <= boutonInformations.rect.y + boutonInformations.rect.h) {
                    afficherInformations(game);
                }
                if (x >= boutonReglage.rect.x && x <= boutonReglage.rect.x + boutonReglage.rect.w &&
                    y >= boutonReglage.rect.y && y <= boutonReglage.rect.y + boutonReglage.rect.h) {
                    afficherReglage(game, parametres);
                }

                if (x >= boutonRetour.rect.x && x <= boutonRetour.rect.x + boutonRetour.rect.w &&
                    y >= boutonRetour.rect.y && y <= boutonRetour.rect.y + boutonRetour.rect.h) {
                        running = 0;
                }
            }
        }

        //AFFICHAGE
        SDL_RenderClear(game->renderer);

        SDL_RenderCopy(game->renderer, game->backgroundTexture[7], NULL, &game->dms_win);
        draw_rect(game, &boutonInformations);
        draw_rect(game, &boutonReglage);
        draw_rect(game, &boutonRetour);
        SDL_RenderPresent(game->renderer);

        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }
}

void afficherMenu(game_t* game, parametre_t* parametres, joueur_t* j, char* pseudo) {
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart = SDL_GetTicks(); //obtien l'heure
    int frameTime;

    rectangle_t boutonJouer,boutonParametres,boutonQuitter;
    creer_rectangle(game,&boutonJouer,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran - HAUTEUR_BOUTON * 2) / 2,58, 90, 64, 255,"Jouer");
    creer_rectangle(game,&boutonParametres,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran + HAUTEUR_BOUTON) / 2,77, 144, 142, 255,"Parametres");
    creer_rectangle(game,&boutonQuitter,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2,98, 23, 8, 255,"Quitter");
    // INITIALISATION DES BOUTONS
    
    int running = 1, action;
    SDL_Event event;

    while (running) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x, y = event.button.y;
                if (x >= boutonJouer.rect.x && x <= boutonJouer.rect.x + boutonJouer.rect.w &&
                    y >= boutonJouer.rect.y && y <= boutonJouer.rect.y + boutonJouer.rect.h) {
                    action = afficherSaisiePseudo(game, j, pseudo);
                    if ((strlen(pseudo) > 0) && action) {
                        running = 0;
                        printf("Pseudo saisi : %s\n", pseudo);
                    }
                }
                if (x >= boutonParametres.rect.x && x <= boutonParametres.rect.x + boutonParametres.rect.w &&
                    y >= boutonParametres.rect.y && y <= boutonParametres.rect.y + boutonParametres.rect.h) {
                    afficherParametre(game, parametres);
                }

                if (x >= boutonQuitter.rect.x && x <= boutonQuitter.rect.x + boutonQuitter.rect.w &&
                    y >= boutonQuitter.rect.y && y <= boutonQuitter.rect.y + boutonQuitter.rect.h) {
                        running = 0;
                }
            }
        }

        //AFFICHAGE
        SDL_RenderClear(game->renderer);

        SDL_RenderCopy(game->renderer, game->backgroundTexture[7], NULL, &game->dms_win);
        draw_rect(game, &boutonJouer);
        draw_rect(game, &boutonParametres);
        draw_rect(game, &boutonQuitter);
        SDL_RenderPresent(game->renderer);

        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }
}

int afficherMenuPause(game_t* game, parametre_t* parametres) {
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
    Uint32 frameStart = SDL_GetTicks(); //obtien l'heure
    int frameTime;

    // INITIALISATION DES BOUTONS
    rectangle_t boutonAcceuil,boutonReprendre,boutonParametres;
    creer_rectangle(game,&boutonReprendre,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran - HAUTEUR_BOUTON * 2) / 2,58, 90, 64, 255,"Reprendre");
    creer_rectangle(game,&boutonParametres,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2,(hauteurEcran + HAUTEUR_BOUTON) / 2,77, 144, 142, 255,"Parametres");
    creer_rectangle(game,&boutonAcceuil,LARGEUR_BOUTON,HAUTEUR_BOUTON,(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2,98, 23, 8, 255,"Menu d'acceuil");
    
    int enCours = 1, Pause = 1;
    SDL_Event evenement;

    while (enCours) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&evenement)) {
            if (evenement.type == SDL_QUIT) {
                enCours = 0;
            } else if (evenement.type == SDL_MOUSEBUTTONDOWN) {
                int x = evenement.button.x, y = evenement.button.y;
                if (x >= boutonReprendre.rect.x && x <= boutonReprendre.rect.x + boutonReprendre.rect.w &&
                    y >= boutonReprendre.rect.y && y <= boutonReprendre.rect.y + boutonReprendre.rect.h) {
                    return 1;
                }
                if (x >= boutonParametres.rect.x && x <= boutonParametres.rect.x + boutonParametres.rect.w &&
                    y >= boutonParametres.rect.y && y <= boutonParametres.rect.y + boutonParametres.rect.h) {
                    afficherParametre(game, parametres);
                    Pause = 0;
                }

                if (x >= boutonAcceuil.rect.x && x <= boutonAcceuil.rect.x + boutonAcceuil.rect.w &&
                    y >= boutonAcceuil.rect.y && y <= boutonAcceuil.rect.y + boutonAcceuil.rect.h) {
                    return 0;
                }
            }
        }

        //AFFICHAGE
        //SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        if(!Pause){
            SDL_RenderClear(game->renderer);
            SDL_RenderCopy(game->renderer, game->backgroundTexture[7], NULL, &game->dms_win);
        }
        draw_rect(game, &boutonReprendre);
        draw_rect(game, &boutonParametres);
        draw_rect(game, &boutonAcceuil);
        SDL_RenderPresent(game->renderer);
        
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }
    return 0;
}
