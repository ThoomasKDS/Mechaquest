#include "../lib/menu.h"

void afficherBouton(SDL_Renderer* renderer, TTF_Font* police, Bouton bouton) {
    SDL_SetRenderDrawColor(renderer, bouton.couleur.r, bouton.couleur.g, bouton.couleur.b, 255);
    SDL_RenderFillRect(renderer, &bouton.rect);
    
    SDL_Color couleurTexte = {255, 255, 255, 255};
    SDL_Surface* surfaceTexte = TTF_RenderUTF8_Solid(police, bouton.texte, couleurTexte);
    SDL_Texture* textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    
    SDL_Rect rectTexte = {bouton.rect.x + (bouton.rect.w - surfaceTexte->w) / 2, 
                         bouton.rect.y + (bouton.rect.h - surfaceTexte->h) / 2, 
                         surfaceTexte->w, surfaceTexte->h};
    SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
    
    SDL_FreeSurface(surfaceTexte);
    SDL_DestroyTexture(textureTexte);
}

int afficherSaisiePseudo(SDL_Renderer* renderer, TTF_Font* police, char* pseudo) {
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(renderer, &largeurEcran, &hauteurEcran);

    Bouton boutonRetour = {{(largeurEcran - LARGEUR_BOUTON) / 2, hauteurEcran / 2 + 150, LARGEUR_BOUTON, HAUTEUR_BOUTON}, {200, 0, 0, 255}, "Retour"};
    Bouton boutonCommencer = {{(largeurEcran - LARGEUR_BOUTON) / 2, hauteurEcran / 2 + 50, LARGEUR_BOUTON, HAUTEUR_BOUTON}, {0, 255, 0, 255}, "Commencer"};

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
                if (longueurPseudo < LONGUEUR_MAX_PSEUDO && evenement.text.text[0] != '\0') {
                    strcat(pseudo, evenement.text.text);
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

            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderClear(renderer);
            afficherBouton(renderer, police, boutonRetour);
            afficherBouton(renderer, police, boutonCommencer);

            SDL_Color couleurTexte = {255, 255, 255, 255};
            if (strlen(pseudo) >= 0) {  
                char texteComplet[LONGUEUR_MAX_PSEUDO + 10];  
                sprintf(texteComplet, "Pseudo : %s", pseudo);  

                SDL_Surface* surfaceTexte = TTF_RenderUTF8_Solid(police, texteComplet, couleurTexte);
                if (!surfaceTexte) {
                    printf("Erreur de rendu texte : %s\n", TTF_GetError());
                } else {
                    SDL_Texture* textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
                    if (textureTexte) {
                        SDL_Rect rectTexte = {
                            (largeurEcran - surfaceTexte->w) / 2,
                            hauteurEcran / 2 - 50,
                            surfaceTexte->w,
                            surfaceTexte->h
                        };
                        SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
                        SDL_DestroyTexture(textureTexte);
                    }
                    SDL_FreeSurface(surfaceTexte);
                }
            }            

            SDL_RenderPresent(renderer);
            SDL_Delay(5);
        }
    }
    SDL_StopTextInput();  
    return action;
}


void afficherParametres(SDL_Renderer* renderer, TTF_Font* police, parametre_t* parametres) {
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(renderer, &largeurEcran, &hauteurEcran);
    
    Bouton boutonRetour = {{(largeurEcran - LARGEUR_BOUTON) / 2, hauteurEcran / 2 + 100, LARGEUR_BOUTON, HAUTEUR_BOUTON}, {200, 0, 0, 255}, "Retour"};
    
    // Boutons de réglage du volume
    Bouton boutonMoinsVolume = {{largeurEcran / 2 - 150, hauteurEcran / 2 - 60, 50, 50}, {255, 0, 0, 255}, "-"};
    Bouton boutonPlusVolume = {{largeurEcran / 2 + 100, hauteurEcran / 2 - 60, 50, 50}, {0, 255, 0, 255}, "+"};

    // Boutons de réglage de la difficulté
    Bouton boutonMoinsDifficulte = {{largeurEcran / 2 - 150, hauteurEcran / 2, 50, 50}, {255, 0, 0, 255}, "-"};
    Bouton boutonPlusDifficulte = {{largeurEcran / 2 + 100, hauteurEcran / 2, 50, 50}, {0, 255, 0, 255}, "+"};

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

                // Ajuster la difficulté
                if (x >= boutonMoinsDifficulte.rect.x && x <= boutonMoinsDifficulte.rect.x + boutonMoinsDifficulte.rect.w &&
                    y >= boutonMoinsDifficulte.rect.y && y <= boutonMoinsDifficulte.rect.y + boutonMoinsDifficulte.rect.h) {
                    if (parametres->difficulte > 1) parametres->difficulte -= 1;
                }
                if (x >= boutonPlusDifficulte.rect.x && x <= boutonPlusDifficulte.rect.x + boutonPlusDifficulte.rect.w &&
                    y >= boutonPlusDifficulte.rect.y && y <= boutonPlusDifficulte.rect.y + boutonPlusDifficulte.rect.h) {
                    if (parametres->difficulte < 3) parametres->difficulte += 1;
                }
            }
        }

        // Affichage
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);
        afficherBouton(renderer, police, boutonRetour);
        afficherBouton(renderer, police, boutonMoinsVolume);
        afficherBouton(renderer, police, boutonPlusVolume);
        afficherBouton(renderer, police, boutonMoinsDifficulte);
        afficherBouton(renderer, police, boutonPlusDifficulte);
        
        // Affichage des valeurs
        char buffer[50];
        SDL_Color couleurTexte = {255, 255, 255, 255};
        
        // Volume
        sprintf(buffer, "Volume: %d", parametres->volume);
        SDL_Surface* surfaceVolume = TTF_RenderText_Solid(police, buffer, couleurTexte);
        SDL_Texture* textureVolume = SDL_CreateTextureFromSurface(renderer, surfaceVolume);
        SDL_Rect rectVolume = {(largeurEcran - surfaceVolume->w) / 2, hauteurEcran / 2 - 50, surfaceVolume->w, surfaceVolume->h};
        SDL_RenderCopy(renderer, textureVolume, NULL, &rectVolume);
        SDL_FreeSurface(surfaceVolume);
        SDL_DestroyTexture(textureVolume);
        
        // Conversion de la difficulté en texte
        const char* texteDifficulte;
        switch (parametres->difficulte) {
            case 1: texteDifficulte = "Facile"; break;
            case 2: texteDifficulte = "Moyen"; break;
            case 3: texteDifficulte = "Difficile"; break;
            default: texteDifficulte = "Inconnu"; break;
        }

        // Affichage de la difficulté
        SDL_Surface* surfaceDifficulte = TTF_RenderText_Solid(police, texteDifficulte, couleurTexte);
        SDL_Texture* textureDifficulte = SDL_CreateTextureFromSurface(renderer, surfaceDifficulte);
        SDL_Rect rectDifficulte = {(largeurEcran - surfaceDifficulte->w) / 2, hauteurEcran / 2, surfaceDifficulte->w, surfaceDifficulte->h};
        SDL_RenderCopy(renderer, textureDifficulte, NULL, &rectDifficulte);
        SDL_FreeSurface(surfaceDifficulte);
        SDL_DestroyTexture(textureDifficulte);
        
        SDL_RenderPresent(renderer);
    }
}


void afficherMenu(SDL_Renderer* renderer, TTF_Font* police, parametre_t* parametres, char* pseudo) {
    int largeurEcran, hauteurEcran;
    SDL_GetRendererOutputSize(renderer, &largeurEcran, &hauteurEcran);

    // INITIALISATION DES BOUTONS
    Bouton boutonJouer = {{(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran - HAUTEUR_BOUTON * 2) / 2, LARGEUR_BOUTON, HAUTEUR_BOUTON}, {0, 255, 0, 255}, "Jouer"};
    Bouton boutonParametres = {{(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON) / 2, LARGEUR_BOUTON, HAUTEUR_BOUTON}, {0, 0, 0, 255}, "Paramètres"};
    Bouton boutonQuitter = {{(largeurEcran - LARGEUR_BOUTON) / 2, (hauteurEcran + HAUTEUR_BOUTON * 4) / 2, LARGEUR_BOUTON, HAUTEUR_BOUTON}, {255, 0, 0, 255}, "Quitter"};
    
    // CHARGEMENT DU LOGO
    SDL_Surface* surfaceLogo = SDL_LoadBMP("img/Mechaquest_logo.bmp");
    SDL_Texture* textureLogo = NULL;
    if (surfaceLogo) {
        textureLogo = SDL_CreateTextureFromSurface(renderer, surfaceLogo);
        SDL_FreeSurface(surfaceLogo);
    } else {
        printf("Erreur de chargement du logo : %s\n", SDL_GetError());
    }

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
                    action = afficherSaisiePseudo(renderer, police, pseudo);
                    if ((strlen(pseudo) > 0) && action) {
                        enCours = 0;
                        printf("Pseudo saisi : %s\n", pseudo);
                    }
                }
                if (x >= boutonParametres.rect.x && x <= boutonParametres.rect.x + boutonParametres.rect.w &&
                    y >= boutonParametres.rect.y && y <= boutonParametres.rect.y + boutonParametres.rect.h) {
                    afficherParametres(renderer, police, parametres);
                }

                if (x >= boutonQuitter.rect.x && x <= boutonQuitter.rect.x + boutonQuitter.rect.w &&
                    y >= boutonQuitter.rect.y && y <= boutonQuitter.rect.y + boutonQuitter.rect.h) {
                    enCours = 0;
                }
            }
        }

        //AFFICHAGE
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);

        // AFFICHAGE DU LOGO
        if (textureLogo) {
            SDL_Rect rectLogo = { (largeurEcran - 300) / 2, 100, 300, 200 }; // Position et taille du logo
            SDL_RenderCopy(renderer, textureLogo, NULL, &rectLogo);
        }

        afficherBouton(renderer, police, boutonJouer);
        afficherBouton(renderer, police, boutonParametres);
        afficherBouton(renderer, police, boutonQuitter);
        SDL_RenderPresent(renderer);
    }

    // LIBÉRATION DE LA TEXTURE DU LOGO
    if (textureLogo) {
        SDL_DestroyTexture(textureLogo);
    }
}


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Menu SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_FULLSCREEN);
    if (!window) {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur de création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("img/police_temporaire.ttf", 24);
    if (!font) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    parametre_t parametres = {50, 1};  // Volume à 50, difficulté facile
    char pseudo[LONGUEUR_MAX_PSEUDO + 1] = "";  // On initialise le pseudo à une chaîne vide

    afficherMenu(renderer, font, &parametres, pseudo);

    printf("Pseudo final : %s\n", pseudo);  // Affichage du pseudo final

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
