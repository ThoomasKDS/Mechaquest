#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "global.h"

#define OK 1
#define KO 0
#define RETOUR -1

// === TABLEAU GLOBAL ===
mechas_t mecha[NB_MECHAS];

// === STUBS ===
void afficher_dialogue_combat(mechas_joueur_t *a, mechas_joueur_t *b, const char *source, const char *msg) {
    (void)a; (void)b;
    printf("[%s] %s\n", source, msg);
}

int aff_mechas_combat(joueur_t *joueur) {
    (void)joueur;
    return 0; // toujours sélection du premier mecha pour test
}

// === FONCTION À TESTER ===
int utilisation_rappel(joueur_t *joueur, mechas_joueur_t *ordi, int *actif){
    if(joueur->inventaire->rappel <= 0){
        afficher_dialogue_combat(&(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Vous n'avez pas de rappel.");
        return KO;
    } else {
        int i = aff_mechas_combat(joueur);
        if(i == -1 || i == 4){
            return RETOUR;
        } else if(joueur->mechas_joueur[i].pv != 0){
            afficher_dialogue_combat(&(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Vous ne pouvez pas utiliser de rappel sur un mechas encore en vie.");
            return RETOUR;
        } else {
            joueur->mechas_joueur[i].pv = joueur->mechas_joueur[i].pv_max / 2;
            char message[200] = "";
            strcat(message, mecha[joueur->mechas_joueur[i].id_mechas - 1].nom);
            strcat(message, " est de retour !");
            afficher_dialogue_combat(&(joueur->mechas_joueur[*actif]), ordi, "Systeme", message);
            joueur->inventaire->rappel--;
        }
    }
    return OK;
}

// === TESTS ===

void test_pas_de_rappel() {
    inventaire_t inv = {.rappel = 0};
    joueur_t joueur = {.inventaire = &inv};
    int actif = 0;
    mechas_joueur_t ordi = {.pv = 100};

    int res = utilisation_rappel(&joueur, &ordi, &actif);
    assert(res == KO);
    printf("✅ Test 1 (pas de rappel) OK\n");
}

void test_rappel_sur_mecha_vivant() {
    inventaire_t inv = {.rappel = 1};
    mechas_joueur_t mecha_j = {.pv = 50, .pv_max = 100, .id_mechas = 1};
    joueur_t joueur = {.inventaire = &inv};
    joueur.mechas_joueur[0] = mecha_j;
    strcpy(mecha[0].nom, "DieselX");

    int actif = 0;
    mechas_joueur_t ordi = {.pv = 100};

    int res = utilisation_rappel(&joueur, &ordi, &actif);
    assert(res == RETOUR);
    assert(joueur.mechas_joueur[0].pv == 50);
    assert(joueur.inventaire->rappel == 1);
    printf("✅ Test 2 (mecha encore en vie) OK\n");
}

void test_rappel_correct() {
    inventaire_t inv = {.rappel = 2};
    mechas_joueur_t mecha_j = {.pv = 0, .pv_max = 100, .id_mechas = 1};
    joueur_t joueur = {.inventaire = &inv};
    joueur.mechas_joueur[0] = mecha_j;
    strcpy(mecha[0].nom, "DieselX");

    int actif = 0;
    mechas_joueur_t ordi = {.pv = 100};

    int res = utilisation_rappel(&joueur, &ordi, &actif);
    assert(res == OK);
    assert(joueur.mechas_joueur[0].pv == 50);
    assert(joueur.inventaire->rappel == 1);
    printf("✅ Test 3 (rappel fonctionne) OK\n");
}

int main() {
    test_pas_de_rappel();
    test_rappel_sur_mecha_vivant();
    test_rappel_correct();
    printf("\nTous les tests de rappel sont passés avec succès !\n");
    return 0;
}
