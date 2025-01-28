//CONSTANTES
const int ERREUR_OUVERTURE = -1;
const int OK = 1;
const int ERR = 0;


//STRUCTURE DE DONNEES//

typedef struct {
    int id_attaques;
    char nom[50];
    char type[20];
    int niveau;
    int degats;
    int precision;
    int utilisations;
} Attaque;

typedef struct {
    int mechaball;
    int carburant;
    int repousse;
    int rappel;
} Inventaire;

typedef struct {
    char pseudo[50];
    char sexe;
    int x;
    int y;
    int pointSauvegarde;
    Inventaire *inventaire;
} Joueur;



//FONCTIONS

int RecuperationJoueur(Joueur *joueur, char pseudo[50]);

int RecuperationInventaire(Inventaire *inventaire, char pseudo[50]);

int RecuperationAttaques(Attaque *attaques);
