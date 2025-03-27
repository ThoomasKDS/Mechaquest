#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "global.h"

#define OK 1
#define KO 0
#define RETOUR -1

// === STUBS ===
void afficher_dialogue_combat(mechas_joueur_t *a, mechas_joueur_t *b, const char *source, const char *msg) {
    (void)a; (void)b;
    printf("[%s] %s\n", source, msg);
}

int aff_mechas_combat(joueur_t *joueur) {
    // Pour les tests, on retourne toujours le mecha n°0
    (void)joueur;
    return 0;
}

// === FONCTION À TESTER ===
int utilisation_carburant(joueur_t *joueur, mechas_joueur_t *ordi, int *actif){
    if(joueur->inventaire->carburant <= 0){
        afficher_dialogue_combat(&(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Vous n'avez pas de carburant.");
        return KO;
    } else {
        int i = aff_mechas_combat(joueur); // récupère le numéro du mecha sélectionné
        if(i == -1 || i == 4){
            return RETOUR;
        } else if(joueur->mechas_joueur[i].pv == joueur->mechas_joueur[i].pv_max){
            afficher_dialogue_combat(&(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Les PV sont déjà au maximum !");
            return RETOUR;
        } else if(joueur->mechas_joueur[i].pv <= 0){
            afficher_dialogue_combat(&(joueur->mechas_joueur[*actif]), ordi, "Systeme", "Vous ne pouvez pas utiliser de carburant (PV à 0).");
            return RETOUR;
        } else {
            joueur->mechas_joueur[i].pv += 20;
            if(joueur->mechas_joueur[i].pv > joueur->mechas_joueur[i].pv_max){
                joueur->mechas_joueur[i].pv = joueur->mechas_joueur[i].pv_max;
            }
            joueur->inventaire->carburant--;
            char message[200] = "Utilisation carburant, nouveau PV : ";
            char buffer[10];
            sprintf(buffer, "%d", joueur->mechas_joueur[i].pv);
            strcat(message, buffer);
            strcat(message, ".");
            afficher_dialogue_combat(&(joueur->mechas_joueur[*actif]), ordi, "Systeme", message);
        }
    }

    return OK;
}

// === TESTS ===

void test_pas_de_carburant() {
    inventaire_t inv = {.carburant = 0};
    joueur_t joueur = {.inventaire = &inv};
    mechas_joueur_t ordi = {.pv = 100};
    int actif = 0;

    int res = utilisation_carburant(&joueur, &ordi, &actif);
    assert(res == KO);
    printf("✅ Test 1 (pas de carburant) OK\n");
}

void test_pv_au_max() {
    inventaire_t inv = {.carburant = 1};
    mechas_joueur_t mecha = {.pv = 100, .pv_max = 100};
    joueur_t joueur = {.inventaire = &inv};
    joueur.mechas_joueur[0] = mecha;
    mechas_joueur_t ordi = {.pv = 100};
    int actif = 0;

    int res = utilisation_carburant(&joueur, &ordi, &actif);
    assert(res == RETOUR);
    assert(joueur.mechas_joueur[0].pv == 100);
    assert(joueur.inventaire->carburant == 1);
    printf("✅ Test 2 (PV déjà au max) OK\n");
}

void test_pv_zero() {
    inventaire_t inv = {.carburant = 1};
    mechas_joueur_t mecha = {.pv = 0, .pv_max = 100};
    joueur_t joueur = {.inventaire = &inv};
    joueur.mechas_joueur[0] = mecha;
    mechas_joueur_t ordi = {.pv = 100};
    int actif = 0;

    int res = utilisation_carburant(&joueur, &ordi, &actif);
    assert(res == RETOUR);
    assert(joueur.mechas_joueur[0].pv == 0);
    assert(joueur.inventaire->carburant == 1);
    printf("✅ Test 3 (PV à 0) OK\n");
}

void test_soin_reussi() {
    inventaire_t inv = {.carburant = 2};
    mechas_joueur_t mecha = {.pv = 50, .pv_max = 100};
    joueur_t joueur = {.inventaire = &inv};
    joueur.mechas_joueur[0] = mecha;
    mechas_joueur_t ordi = {.pv = 100};
    int actif = 0;

    int res = utilisation_carburant(&joueur, &ordi, &actif);
    assert(res == OK);
    assert(joueur.mechas_joueur[0].pv == 70);
    assert(joueur.inventaire->carburant == 1);
    printf("✅ Test 4 (soin classique) OK\n");
}

int main() {
    test_pas_de_carburant();
    test_pv_au_max();
    test_pv_zero();
    test_soin_reussi();
    printf("\nTous les tests utilisation_carburant sont passés avec succès !\n");
    return 0;
}
