#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "global.h"

#define OK 1
#define KO 0

// Déclaration des tableaux globaux
attaque_t attaque[NB_ATTAQUES];
mechas_t mecha[NB_MECHAS];

// === STUBS ===
// Dans le fichier combat.c, ce sont des fonctions d'affichage SDL mais pour nos test, on le fera sur le terminal.
void afficher_dialogue_combat(mechas_joueur_t *att, mechas_joueur_t *def, const char *source, const char *msg) {
    (void)att;
    (void)def;
    printf("[%s] %s\n", source, msg);
}

void animation_degat(int choix, int pv_old, int pv_new, mechas_joueur_t *m1, mechas_joueur_t *m2) {
    (void)choix;
    (void)m1;
    (void)m2;
    printf("Animation dégâts : %d -> %d\n", pv_old, pv_new);
}

// === Fonction testée (copiée ici directement) ===
int algo_attaque(int choix, mechas_joueur_t *mecha_att, mechas_joueur_t *mecha_def, int mecha_choix) {
    if(choix == 2) return 0;
    if(choix == 0 && mecha_att->utilisation_1 == 0) {
        afficher_dialogue_combat(mecha_att, mecha_def, "Systeme", "Vous ne pouvez plus utiliser cette attaque.");
        return KO;
    }
    if(choix == 1 && mecha_att->utilisation_2 == 0) {
        afficher_dialogue_combat(mecha_att, mecha_def, "Systeme", "Vous ne pouvez plus utiliser cette attaque.");
        return KO;
    }
    //int ancien_pv = mecha_def->pv;   non utile pour le test
    float att_degat[2] = {attaque[mecha_att->attaque_1].degats, attaque[mecha_att->attaque_2].degats};
    float stat_att_mecha = mecha_att->attaque;
    char *mecha_att_type[2] = {attaque[mecha_att->attaque_1].type, attaque[mecha_att->attaque_2].type};
    char *mecha_def_type = mecha[mecha_def->id_mechas - 1].type;
    char *nom_attaque[2] = {attaque[mecha_att->attaque_1].nom, attaque[mecha_att->attaque_2].nom};
    int precision[2] = {attaque[mecha_att->attaque_1].precision, attaque[mecha_att->attaque_2].precision};
    char att_type[4][20] = {"Carburant", "Electrique", "Renouvelable", "Carburant"};
    int nbr_rand = (rand() % 100) + 1;
    char msg[300];
    strcpy(msg, mecha[mecha_att->id_mechas - 1].nom);
    strcat(msg, " utilise ");
    strcat(msg, nom_attaque[choix]);
    strcat(msg, ".");

    if(mecha_choix) 
        afficher_dialogue_combat(mecha_att, mecha_def, "Systeme", msg);
    else 
        afficher_dialogue_combat(mecha_def, mecha_att, "Systeme", msg);

    if(nbr_rand <= precision[choix]) {
        if((strcmp(mecha_att_type[choix], "Uranium"))) {
            for(int i = 0; i < 3; i++) {
                if((!strcmp(mecha_att_type[choix], att_type[i])) && !strcmp(mecha_def_type, att_type[i + 1])) {
                    att_degat[choix] *= 1.5;
                }
            }
        } else if(!strcmp(mecha_att_type[choix], "Uranium") && strcmp(mecha_def_type, "Uranium")) {
            att_degat[choix] *= 1.2;
        }
        if(stat_att_mecha + att_degat[choix] > mecha_def->defense) {
            int pv_old = mecha_def->pv;
            int pv_new = pv_old - (stat_att_mecha + att_degat[choix] - mecha_def->defense);
            if(pv_new < 0) pv_new = 0;

            if(mecha_choix) 
                animation_degat(mecha_choix, pv_old, pv_new, mecha_att, mecha_def);
            else 
                animation_degat(mecha_choix, pv_old, pv_new, mecha_def, mecha_att);

            mecha_def->pv = pv_new;
        }
        if(!choix) mecha_att->utilisation_1--;
        else mecha_att->utilisation_2--;
    } else {
        afficher_dialogue_combat(mecha_att, mecha_def, "Systeme", "L'attaque echoue.");
    }
    return OK;
}

// === Setup des données ===
void setup_donnees_test() {
    strcpy(attaque[0].nom, "Coup de feu");
    strcpy(attaque[0].type, "Carburant");
    attaque[0].degats = 30;
    attaque[0].precision = 100;

    strcpy(attaque[1].nom, "Éclair");
    strcpy(attaque[1].type, "Electrique");
    attaque[1].degats = 40;
    attaque[1].precision = 0;

    strcpy(mecha[0].nom, "SolarBot");
    strcpy(mecha[0].type, "Renouvelable");

    strcpy(mecha[1].nom, "DieselX");
    strcpy(mecha[1].type, "Carburant");
}

// === Test 1 : attaque réussie ===
void test_attaque_reussie() {
    attaque[0].precision = 100;

    mechas_joueur_t att = { .attaque = 50, .utilisation_1 = 1, .attaque_1 = 0, .id_mechas = 1 };
    mechas_joueur_t def = { .defense = 10, .pv = 100, .id_mechas = 2 };

    int res = algo_attaque(0, &att, &def, 1);
    assert(res == OK);
    assert(def.pv < 100);
    assert(att.utilisation_1 == 0);
    printf("✅ Test 1 (attaque réussie) OK\n");
}

// === Test 2 : attaque échoue (précision nulle) ===
void test_attaque_ratee() {
    attaque[1].precision = 0;

    mechas_joueur_t att = { .attaque = 50, .utilisation_2 = 1, .attaque_2 = 1, .id_mechas = 1 };
    mechas_joueur_t def = { .defense = 10, .pv = 100, .id_mechas = 2 };

    int res = algo_attaque(1, &att, &def, 1);
    assert(res == OK);
    assert(def.pv == 100);
    assert(att.utilisation_2 == 1);
    printf("✅ Test 2 (attaque échouée) OK\n");
}

// === Test 3 : plus d'utilisation ===
void test_plus_dutilisation() {
    mechas_joueur_t att = { .utilisation_1 = 0, .attaque_1 = 0, .id_mechas = 1 };
    mechas_joueur_t def = { .pv = 100, .id_mechas = 2 };

    int res = algo_attaque(0, &att, &def, 1);
    assert(res == KO);
    printf("✅ Test 3 (plus d'utilisation) OK\n");
}

int main() {
    setup_donnees_test();
    srand(0);  // Pour résultats reproductibles
    test_attaque_reussie();
    test_attaque_ratee();
    test_plus_dutilisation();
    printf("\nTous les tests sont passés avec succès.\n");
    return 0;
}
