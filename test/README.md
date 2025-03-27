# ✅ Tests unitaires du système de combat — *Projet Mecha Combat*

Ce dossier contient des **tests unitaires manuels** destinés à valider des fonctions critiques liées aux combats dans le jeu : attaques, soins avec objets, et rappels de méchas.

Chaque fichier de test :
- Contient la **fonction testée copiée localement**
- Simule les dépendances (fonctions externes, inventaire, affichage)
- Utilise des **assertions (`assert`)** pour valider les résultats

---

## Fichiers de test & scénarios

---

### `test_fonction_algo_attaque.c`

**Fonction testée :** `int algo_attaque(int choix, mechas_joueur_t *att, mechas_joueur_t *def, int mecha_choix)`

| Test | Phase de préparation | Action | Validation |
|------|----------------------|--------|------------|
| ✅ **Attaque réussie** | Attaquant avec attaque disponible, précision à 100% | Appel à `algo_attaque(0, ...)` | PV du défenseur a diminué, PP décrémenté |
| ✅ **Attaque échoue (précision = 0)** | Attaquant avec attaque disponible, précision forcée à 0 | Appel à `algo_attaque(1, ...)` | PV identique, attaque non consommée |
| ✅ **Plus d’utilisation (PP à 0)** | Attaque 1 déjà épuisée | Appel à `algo_attaque(0, ...)` | Retour `KO`, PV inchangé |

Stubs : `afficher_dialogue_combat`, `animation_degat`  
Variables globales simulées : `attaque[]`, `mecha[]`

---

###  `test_fonction_utilisation_carburant.c`

**Fonction testée :** `int utilisation_carburant(joueur_t *joueur, mechas_joueur_t *ordi, int *actif)`

| Test | Phase de préparation | Action | Validation |
|------|----------------------|--------|------------|
| ❌ **Pas de carburant** | Inventaire avec `carburant = 0` | Appel à `utilisation_carburant(...)` | Retour `KO`, PV inchangé |
| ❌ **PV au maximum** | Mecha avec `pv == pv_max` | Sélection via `aff_mechas_combat()` | Retour `RETOUR`, aucun soin |
| ❌ **Mecha KO** | PV à 0 | Appel à `utilisation_carburant()` | Retour `RETOUR`, pas de soin |
| ✅ **Soin effectif** | Mecha à PV partiel, carburant dispo | Appel à `utilisation_carburant()` | PV +20 (sans dépasser max), -1 carburant |

Stubs : `afficher_dialogue_combat`, `aff_mechas_combat`

---

###  `test_fonction_utilisation_rappel.c`

**Fonction testée :** `int utilisation_rappel(joueur_t *joueur, mechas_joueur_t *ordi, int *actif)`

| Test | Phase de préparation | Action | Validation |
|------|----------------------|--------|------------|
| ❌ **Pas de rappel** | Inventaire avec `rappel = 0` | Appel à `utilisation_rappel()` | Retour `KO`, rien ne change |
| ❌ **Mecha vivant** | Mecha a PV > 0 | Appel à `utilisation_rappel()` | Retour `RETOUR`, rappel non consommé |
| ✅ **Mecha KO** | PV à 0, rappel dispo, `mecha[]` contient un nom | Appel à `utilisation_rappel()` | PV = `pv_max / 2`, -1 rappel, message de retour |

Stubs : `aff_mechas_combat`, `afficher_dialogue_combat`  
Accès à `mecha[]` pour afficher le nom du mécha réanimé

---

## Exécution

**Compiler tous les tests :**

```bash
make
