#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/menu.h"
#include "../lib/initGame.h"
#include "../lib/affichage.h"

SDL_Color rouge = {200, 0, 0, 255};
SDL_Color vert = {0, 200, 0, 255};
SDL_Color noir = {0, 0, 0, 255};

// On garde !!!
Bouton creerBouton(int x, int y, int largeur, int hauteur, SDL_Color couleur, const char* texte) {
    Bouton bouton;
    bouton.rect.x = x;
    bouton.rect.y = y;
    bouton.rect.w = largeur;
    bouton.rect.h = hauteur;
    bouton.couleur = couleur;
    bouton.texte = texte;  // Attention : texte doit être une chaîne statique ou allouée dynamiquement
    return bouton;
}

void afficherBouton(game_t* game, Bouton bouton) {
    SDL_SetRenderDrawColor(game->renderer, bouton.couleur.r, bouton.couleur.g, bouton.couleur.b, 255);
    SDL_RenderFillRect(game->renderer, &bouton.rect);
    
    SDL_Color couleurTexte = {255, 255, 255, 255};
    SDL_Surface* surfaceTexte = TTF_RenderUTF8_Solid(game->police, bouton.texte, couleurTexte);
    if (!surfaceTexte) {
        printf("Erreur de création de surface texte\n");
        return;
    }
    SDL_Texture* textureTexte = SDL_CreateTextureFromSurface(game->renderer, surfaceTexte);
    
    SDL_Rect rectTexte = {bouton.rect.x + (bouton.rect.w - surfaceTexte->w) / 2, 
                         bouton.rect.y + (bouton.rect.h - surfaceTexte->h) / 2, 
                         surfaceTexte->w, surfaceTexte->h};
    SDL_RenderCopy(game->renderer, textureTexte, NULL, &rectTexte);
    
    SDL_FreeSurface(surfaceTexte);
    SDL_DestroyTexture(textureTexte);
}

//on modifie
int afficherSaisiePseudo(game_t* game, char* pseudo) {
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);

    Bouton boutonRetour = creerBouton((largeurEcran - LARGEUR_BOUTON) / 2, hauteurEcran / 2 + 150, LARGEUR_BOUTON, HAUTEUR_BOUTON, rouge, "Retour");
    Bouton boutonCommencer = creerBouton((largeurEcran - LARGEUR_BOUTON) / 2, hauteurEcran / 2 + 50, LARGEUR_BOUTON, HAUTEUR_BOUTON, vert, "Commencer");

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
                else if (evenement.key.keysym.sym == SDLK_RETURN) {
                    enCours = 0;
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
                    y >= boutonCommencer.rect.y && y <= boutonCommencer.rect.y + boutonCommencer.rect.h) {
                    enCours = 0;
                    action = 1;
                }
            }

            SDL_RenderClear(game->renderer);
            afficherBouton(game, boutonRetour);
            afficherBouton(game, boutonCommencer);

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
            SDL_Delay(5);
        }
    }
    SDL_StopTextInput();  
    return action;
}

//on modifie
void afficherParametres(game_t* game, parametre_t* parametres) {
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);
    
    Bouton boutonRetour = creerBouton((largeurEcran - LARGEUR_BOUTON) / 2, hauteurEcran / 2 + 100, LARGEUR_BOUTON, HAUTEUR_BOUTON, rouge, "Retour");
    
    // Boutons de réglage du volume
    Bouton boutonMoinsVolume = creerBouton(largeurEcran / 2 - 150, hauteurEcran / 2 - 60, 50, 50, rouge, "-");
    Bouton boutonPlusVolume = creerBouton(largeurEcran / 2 + 100, hauteurEcran / 2 - 60, 50, 50, vert, "+");

    int enCours = 1;
    SDL_Event evenement;
    
    while (enCours) {
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
        SDL_RenderClear(game->renderer);
        afficherBouton(game, boutonRetour);
        afficherBouton(game, boutonMoinsVolume);
        afficherBouton(game, boutonPlusVolume);
        
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
    }
}

//on modifie
void afficherMenu(game_t* game, parametre_t* parametres, char* pseudo) {
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(game->renderer, &largeurEcran, &hauteurEcran);

    // INITIALISATION DES BOUTONS
    Bouton boutonJouer = creerBouton((largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran - HAUTEUR_BOUTON * 2) / 2, LARGEUR_BOUTON, HAUTEUR_BOUTON, vert, "Jouer");
    Bouton boutonParametres = creerBouton((largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON) / 2, LARGEUR_BOUTON, HAUTEUR_BOUTON, noir, "Paramètres");
    Bouton boutonQuitter = creerBouton((largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2, LARGEUR_BOUTON, HAUTEUR_BOUTON, rouge, "Quitter");

    int enCours = 1, action;
    SDL_Event evenement;

    while (enCours) {
        while (SDL_PollEvent(&evenement)) {
            if (evenement.type == SDL_QUIT) {
                enCours = 0;
            } else if (evenement.type == SDL_MOUSEBUTTONDOWN) {
                int x = evenement.button.x, y = evenement.button.y;
                if (x >= boutonJouer.rect.x && x <= boutonJouer.rect.x + boutonJouer.rect.w &&
                    y >= boutonJouer.rect.y && y <= boutonJouer.rect.y + boutonJouer.rect.h) {
                    action = afficherSaisiePseudo(game, pseudo);
                    if ((strlen(pseudo) > 0) && action) {
                        enCours = 0;
                        printf("Pseudo saisi : %s\n", pseudo);
                    }
                }
                if (x >= boutonParametres.rect.x && x <= boutonParametres.rect.x + boutonParametres.rect.w &&
                    y >= boutonParametres.rect.y && y <= boutonParametres.rect.y + boutonParametres.rect.h) {
                    afficherParametres(game, parametres);
                }

                if (x >= boutonQuitter.rect.x && x <= boutonQuitter.rect.x + boutonQuitter.rect.w &&
                    y >= boutonQuitter.rect.y && y <= boutonQuitter.rect.y + boutonQuitter.rect.h) {
                    enCours = 0;
                }
            }
        }

        //AFFICHAGE
        SDL_RenderClear(game->renderer);

        afficherBouton(game, boutonJouer);
        afficherBouton(game, boutonParametres);
        afficherBouton(game, boutonQuitter);
        SDL_RenderPresent(game->renderer);
    }

}
