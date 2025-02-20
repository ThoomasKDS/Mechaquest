#include "../lib/pointDePassage.h"
#include "../lib/combat.h"

const int FPS_LIMIT = 60;
const int FRAME_DELAY = 1000 / FPS_LIMIT; // Temps entre chaque frame (16 ms)

int soigner(joueur_t *j){
    j->inventaire->mechaball = 5;
    j->inventaire->carburant = 5;
    j->inventaire->rappel = 5;
    j->inventaire->repousse = 5;
    for(int i = 0;i < j->nb_mechas && i < 4;i++){
        j->mechas_joueur[i].pv = j->mechas_joueur[i].pv_max;
        j->mechas_joueur[i].utilisation_1 = attaque[j->mechas_joueur[i].attaque_1-1].utilisations;
        j->mechas_joueur[i].utilisation_2 = attaque[j->mechas_joueur[i].attaque_2-1].utilisations;
    }
    sauvegarde_partie(j,j->pseudo);
    return OK;
}

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

int choix_starter(joueur_t *j,pnj_t *vinGazole){
    int choix = 0;
    SDL_Event event;
    printf("Choisissez Le mechas que vous souhaité: \n");
    printf("1: Tournicoton \n");
    printf("2: Rasetout \n");
    printf("3: Tikart\n");
    while(!choix){
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) 
                     return 0;   
                if (event.key.keysym.sym == SDLK_1 || event.key.keysym.sym == SDLK_KP_1){
                    choix = copie_mechas(j,&vinGazole->mechas_joueur[0]);
                    printf("Vous avez choisit Tournicoton\n");
                }
                else if (event.key.keysym.sym == SDLK_2 || event.key.keysym.sym == SDLK_KP_2){
                    choix = copie_mechas(j,&vinGazole->mechas_joueur[1]); 
                    printf("Vous avez choisit Rasetout\n");
                }
                else if (event.key.keysym.sym == SDLK_3 || event.key.keysym.sym == SDLK_KP_3){
                    choix = copie_mechas(j,&vinGazole->mechas_joueur[2]); 
                    printf("Vous avez choisit Tikart\n");
                }
            }
        }
        if(choix){
            j->inventaire->mechaball = vinGazole->inventaire->mechaball;
            j->inventaire->carburant = vinGazole->inventaire->carburant;
            j->inventaire->rappel = vinGazole->inventaire->rappel;
            j->inventaire->repousse = vinGazole->inventaire->repousse;
        }
    }
    j->pointSauvegarde = 1;
    sauvegarde_partie(j,j->pseudo);
    return OK;
}


int parler_a_vin_gazole(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p, mechas_t *mecha, zone_t *zone){
    int obj_case;
    game->mat[0][0][15] = 2;
    game->mat[0][0][16] = 2;
    game->mat[0][0][17] = 2;
    pnj_t vinGazole;
    recuperation_pnj(&vinGazole,18);
    SDL_Event event;
    Uint32 frameStart;  
    int frameTime;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    int last_case = RIEN;
    int taille_x_mat = game->img_w/PX;
    int taille_y_mat = game->img_h/PX;
    mechas_joueur_t mecha_sauvage;
    int running =1;
    while(j->pointSauvegarde == 0 && running){
        frameStart = SDL_GetTicks();    //obtien heure

        SDL_PumpEvents();  // Met à jour l'état des événements (telles que les touches pressées)
        keys = SDL_GetKeyboardState(NULL);  // Met à jour l'état des touches

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {       //si on appuie sur fermer la fenetre running = 0
                running = 0;
            }
            if(event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) 
                     running = 0;    
                if (event.key.keysym.sym == SDLK_p){
                    if(j->numMap == 0 && j->x+1 == vinGazole.x && j->y == vinGazole.y && j->derniere_touche == 2 && vinGazole.etat == 0){
                        printf("%s\n",vinGazole.dialogueDebut);
                        choix_starter(j,&vinGazole);
                        printf("%s\n",vinGazole.dialogueFin);
                        vinGazole.etat = 1;
                        sauvegarde_pnj(&vinGazole,18);
                        game->mat[0][0][15] = -12;
                        game->mat[0][0][16] = -12;
                        game->mat[0][0][17] = -12;
                    }
                }
                if(event.key.keysym.sym == SDLK_UP){
                    if(j->numMap == 0 && (  j->x == 15 || j->x == 16 ||j->x == 17) && j->y == 1 ){
                        printf("Allez d'abord parler à Vin Gazole\n");
                    }
                }
            }
        }
        //deplacement du joueur 
        obj_case = deplacement(game,taille_x_mat, taille_y_mat, keys, j, &last_case, sprite_p);
        if(spawn_mecha(j, obj_case, zone, mecha, &mecha_sauvage)) {
            combat_sauvage(j, &mecha_sauvage, game);
        }
        animation(j, sprite_p);

        SDL_RenderClear(game->renderer);     //efface l'ecran

        draw_background(game);
        draw_player(game, sprite_p, sprite_playerH, j);           //dessine le joueur
    

        SDL_RenderPresent(game->renderer);      //affiche rendu
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }
    destruction_pnj(&vinGazole);
    return OK;
}

int premier_combat_musk(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p, mechas_t *mecha, zone_t *zone){
    printf("\nDEBUT PARTIE 2\n");
    SDL_Event event;
    Uint32 frameStart;  
    int frameTime;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    int last_case = RIEN;
    int taille_x_mat = game->img_w/PX;
    int taille_y_mat = game->img_h/PX;

    pnj_t vinGazole;
    recuperation_pnj(&vinGazole,19);

    pnj_t ironMusk;
    recuperation_pnj(&ironMusk,20);

    pnj_t pnj[8];
    for(int i = 0;i < 8;i++){
        recuperation_pnj(&pnj[i],i+1);
    }

    int soin = 0;
    int obj_case;

    game->mat[2][0][4] = 2;
    game->mat[2][0][5] = 2;
    game->mat[2][0][6] = 2;

    mechas_joueur_t mecha_sauvage;
    int running =1;
    while(j->pointSauvegarde == 1 && running){
        frameStart = SDL_GetTicks();    //obtien heure

        SDL_PumpEvents();  // Met à jour l'état des événements (telles que les touches pressées)
        keys = SDL_GetKeyboardState(NULL);  // Met à jour l'état des touches

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {       //si on appuie sur fermer la fenetre running = 0
                running = 0;
            }
            if(event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) 
                     running = 0;    
                if (event.key.keysym.sym == SDLK_p){
                    if(j->numMap == 0 && j->x+1 == vinGazole.x && j->y == vinGazole.y && j->derniere_touche == 2){
                        if(j->inventaire->mechaball < 5 || j->inventaire->carburant < 5|| j->inventaire->repousse <5 || j->inventaire->rappel< 5)
                            soin = 1;
                        for(int i = 0;i < 4 && soin == 0;i++){
                            if(j->mechas_joueur[0].pv < j->mechas_joueur[0].pv_max)
                                soin = 1;       
                        }
                        if(soin){
                            printf("%s\n",vinGazole.dialogueFin);
                            soigner(j);
                            soin = 0;
                        }
                        else{
                            printf("%s\n",vinGazole.dialogueDebut);
                        }
                        
                    }
                    if(j->numMap == 2 && j->x+1 == ironMusk.x && j->y == ironMusk.y && j->derniere_touche == 2){
                        printf("%s\n",ironMusk.dialogueDebut);
                        /*
                        combat(j,&ironMUsk);
                        */
                        printf("%s\n",ironMusk.dialogueFin);
                        ironMusk.etat = 1;
                        sauvegarde_pnj(&ironMusk,20);
                        game->mat[2][0][4] = -14;
                        game->mat[2][0][5] = -14;
                        game->mat[2][0][6] = -14;
                        game->mat[2][9][19] = 0;

                        game->mat[2][8][0] = -16;
                        game->mat[2][9][0] = -16;
                        game->mat[2][10][0] = -16;
                        j->x = 23;
                        j->y = 8;
                        j->numMap = 0;
                        j->pointSauvegarde = 2;
                        j->derniere_touche = 4;
                        game->mat_active = j->numMap;
                        sauvegarde_partie(j,j->pseudo);
                        j->screen_x = (float)(game->dms_win.x + (j->x * PX * game->scale));      //position du joueur en px
                        j->screen_y = (float)(game->dms_win.y + (j->y * PX * game->scale));
                        if(!init_player_h(game, sprite_playerH)){
                            return -1;
                        }
                        
                    }
                }
                if(event.key.keysym.sym == SDLK_UP){
                    if(j->numMap == 2 && (j->x == 4 || j->x == 5 ||j->x == 6) && j->y == 1){
                        printf("Allez combattre Iron Musk avant\n");
                    }
                }
            }
        }
        //deplacement du joueur 
        obj_case = deplacement(game,taille_x_mat, taille_y_mat, keys, j, &last_case, sprite_p);
        if(spawn_mecha(j, obj_case, zone, mecha, &mecha_sauvage)) {
            combat_sauvage(j, &mecha_sauvage, game);
        }

        animation(j, sprite_p);

        SDL_RenderClear(game->renderer);     //efface l'ecran

        draw_background(game);
        draw_player(game, sprite_p, sprite_playerH, j);           //dessine le joueur
    

        SDL_RenderPresent(game->renderer);      //affiche rendu
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }
    destruction_pnj(&vinGazole);
    destruction_pnj(&ironMusk);
    for(int i = 0;i < 8;i++){
        destruction_pnj(&pnj[i]);
    }
    return OK;
}

int retourner_parler_a_vin_gazole(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p, mechas_t *mecha, zone_t *zone){
    printf("\nDEBUT PARTIE 3\n");
    game->mat[0][0][15] = 2;
    game->mat[0][0][16] = 2;
    game->mat[0][0][17] = 2;
    int obj_case;
    pnj_t vinGazole;
    recuperation_pnj(&vinGazole,21);

    SDL_Event event;
    Uint32 frameStart;  
    int frameTime;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    int last_case = RIEN;
    int taille_x_mat = game->img_w/PX;
    int taille_y_mat = game->img_h/PX;
    mechas_joueur_t mecha_sauvage;
    int running =1;
    while(j->pointSauvegarde == 2 && running){
        frameStart = SDL_GetTicks();    //obtien heure

        SDL_PumpEvents();  // Met à jour l'état des événements (telles que les touches pressées)
        keys = SDL_GetKeyboardState(NULL);  // Met à jour l'état des touches

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {       //si on appuie sur fermer la fenetre running = 0
                running = 0;
            }
            if(event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) 
                     running = 0;    
                if (event.key.keysym.sym == SDLK_p){
                    if(j->x+1 == vinGazole.x && j->y == vinGazole.y && j->derniere_touche == 2 && vinGazole.etat == 0){
                        printf("%s\n",vinGazole.dialogueDebut);
                        printf("%s\n",vinGazole.dialogueFin);
                        soigner(j);
                        vinGazole.etat = 1;
                        sauvegarde_pnj(&vinGazole,21);
                        game->mat[0][0][15] = -12;
                        game->mat[0][0][16] = -12;
                        game->mat[0][0][17] = -12;
                        j->pointSauvegarde = 3;
                    }
                }
                if(event.key.keysym.sym == SDLK_UP){
                    if((j->x == 15 || j->x == 16 ||j->x == 17) && j->y == 1){
                        printf("Allez vous soigner\n");
                    }
                }
            }
        }
        //deplacement du joueur 
        obj_case = deplacement(game,taille_x_mat, taille_y_mat, keys, j, &last_case, sprite_p);
        if(spawn_mecha(j, obj_case, zone, mecha, &mecha_sauvage)) {
            combat_sauvage(j, &mecha_sauvage, game);
        }

        animation(j, sprite_p);

        SDL_RenderClear(game->renderer);     //efface l'ecran

        draw_background(game);
        draw_player(game, sprite_p, sprite_playerH, j);           //dessine le joueur
    

        SDL_RenderPresent(game->renderer);      //affiche rendu
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }
    destruction_pnj(&vinGazole);
    return OK;
}

int combat_final(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p, mechas_t *mecha, zone_t *zone){
    printf("\nDEBUT PARTIE 4\n");
    SDL_Event event;
    Uint32 frameStart;  
    int frameTime;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    int last_case = RIEN;
    int taille_x_mat = game->img_w/PX;
    int taille_y_mat = game->img_h/PX;
    int obj_case;
    pnj_t vinGazole;
    recuperation_pnj(&vinGazole,22);

    pnj_t ironMusk;
    recuperation_pnj(&ironMusk,23);

    pnj_t pnj[18];
    for(int i = 0;i < 18;i++){
        recuperation_pnj(&pnj[i],i+1);
    }

    int soin = 0;

    mechas_joueur_t mecha_sauvage;
    int running = 1;
    while(j->pointSauvegarde == 3 && running){
        frameStart = SDL_GetTicks();    //obtien heure

        SDL_PumpEvents();  // Met à jour l'état des événements (telles que les touches pressées)
        keys = SDL_GetKeyboardState(NULL);  // Met à jour l'état des touches

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {       //si on appuie sur fermer la fenetre running = 0
                running = 0;
            }
            if(event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) 
                     running = 0;    
                if (event.key.keysym.sym == SDLK_p){
                    if(j->numMap == 0 && j->x+1 == vinGazole.x && j->y == vinGazole.y && j->derniere_touche == 2){
                        if(j->inventaire->mechaball < 5 || j->inventaire->carburant < 5|| j->inventaire->repousse <5 || j->inventaire->rappel< 5)
                            soin = 1;
                        for(int i = 0;i < 4 && soin == 0;i++){
                            if(j->mechas_joueur[0].pv < j->mechas_joueur[0].pv_max)
                                soin = 1;       
                        }
                        if(soin){
                            printf("%s\n",vinGazole.dialogueFin);
                            soigner(j);
                            soin = 0;
                        }
                        else{
                            printf("%s\n",vinGazole.dialogueDebut);
                        }
                        
                    }
                    if(j->numMap == 4 && j->x == ironMusk.x && j->y-1 == ironMusk.y && j->derniere_touche == 3 && ironMusk.etat == 0){
                        printf("%s\n",ironMusk.dialogueDebut);
                        /*
                        combat avec iron musk
                        */
                        printf("%s\n",ironMusk.dialogueFin);
                        copie_mechas(j,&ironMusk.mechas_joueur[0]);
                        sauvegarde_partie(j,j->pseudo);
                        ironMusk.etat = 1;
                        sauvegarde_pnj(&ironMusk,23);

                        j->pointSauvegarde = 4;

                    }
                }

            }
        }
        //deplacement du joueur 
        obj_case = deplacement(game,taille_x_mat, taille_y_mat, keys, j, &last_case, sprite_p);
        if(spawn_mecha(j, obj_case, zone, mecha, &mecha_sauvage)) {
            printf("oui");
            combat_sauvage(j, &mecha_sauvage, game);
        }
        animation(j, sprite_p);

        SDL_RenderClear(game->renderer);     //efface l'ecran

        draw_background(game);
        draw_player(game, sprite_p, sprite_playerH, j);           //dessine le joueur
    

        SDL_RenderPresent(game->renderer);      //affiche rendu
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }
    destruction_pnj(&vinGazole);
    destruction_pnj(&ironMusk);
    for(int i = 0;i < 18;i++){
        destruction_pnj(&pnj[i]);
    }
    return OK;
}

int jeu_libre(game_t *game, img_player_t *sprite_playerH, joueur_t *j,SDL_Rect *sprite_p, mechas_t *mecha, zone_t *zone){
    int obj_case;
    pnj_t vinGazole;
    recuperation_pnj(&vinGazole,24);
    pnj_t pnj[18];
    for(int i = 0;i < 18;i++){
        recuperation_pnj(&pnj[i],i+1);
    }
    int soin = 0;
    SDL_Event event;
    Uint32 frameStart;  
    int frameTime;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    int last_case = RIEN;
    int taille_x_mat = game->img_w/PX;
    int taille_y_mat = game->img_h/PX;
    mechas_joueur_t mecha_sauvage;
    int running =1;
    while(j->pointSauvegarde == 4 && running){
        frameStart = SDL_GetTicks();    //obtien heure

        SDL_PumpEvents();  // Met à jour l'état des événements (telles que les touches pressées)
        keys = SDL_GetKeyboardState(NULL);  // Met à jour l'état des touches

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {       //si on appuie sur fermer la fenetre running = 0
                running = 0;
            }
            if(event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) 
                     running = 0;    
                if (event.key.keysym.sym == SDLK_p){
                    printf("\n%d\n",vinGazole.id_pnj);
                    if(j->numMap == 0 && j->x+1 == vinGazole.x && j->y == vinGazole.y && j->derniere_touche == 2 && vinGazole.etat == 0){
                        if(vinGazole.id_pnj == 24){
                            printf("%s\n",vinGazole.dialogueDebut);
                            printf("%s\n",vinGazole.dialogueFin);
                            soigner(j);
                            vinGazole.etat = 1;
                            sauvegarde_pnj(&vinGazole,24);
                            recuperation_pnj(&vinGazole,22);
                        }
                        else if(vinGazole.id_pnj == 22){
                            if(j->inventaire->mechaball < 5 || j->inventaire->carburant < 5|| j->inventaire->repousse <5 || j->inventaire->rappel< 5)
                                soin = 1;
                            for(int i = 0;i < 4 && soin == 0;i++){
                                if(j->mechas_joueur[0].pv < j->mechas_joueur[0].pv_max)
                                    soin = 1;       
                                }
                                if(soin){
                                    printf("%s\n",vinGazole.dialogueFin);
                                    soigner(j);
                                    soin = 0;
                                }
                                else{
                                    printf("%s\n",vinGazole.dialogueDebut);
                                }
                            }
                       
                    }
                }
            }
        }
        //deplacement du joueur 
        obj_case = deplacement(game,taille_x_mat, taille_y_mat, keys, j, &last_case, sprite_p);
        if(spawn_mecha(j, obj_case, zone, mecha, &mecha_sauvage)) {
            combat_sauvage(j, &mecha_sauvage, game);
        }

        animation(j, sprite_p);

        SDL_RenderClear(game->renderer);     //efface l'ecran

        draw_background(game);
        draw_player(game, sprite_p, sprite_playerH, j);           //dessine le joueur
    

        SDL_RenderPresent(game->renderer);      //affiche rendu
        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé pour la frame

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime); // Attend le temps restant
        }
    }
    destruction_pnj(&vinGazole);
    for(int i = 0;i < 18;i++){
        destruction_pnj(&pnj[i]);
    }
    return OK;
}