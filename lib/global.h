/**
 * @file global.h
 * @brief Définition des constantes, structures et variables globales du jeu.
 * 
 * Ce fichier contient les constantes, structures et variables nécessaires au fonctionnement du jeu,
 * notamment celles utilisées pour la gestion de l'affichage, des combats et des interactions.
 */
#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//*********DEFINE********//
#define LONGUEUR_MAX_PSEUDO 50
#define LONGUEUR_LIGNE 256
#define LONGUEUR_LIGNE_DESC 1070
#define LONGUEUR_DIALOGUE 500
#define LONGUEUR_TYPE 20
#define LONGUEUR_DESC 200

//TAILLE TAB
#define NB_MECHAS_MAX 54
#define NB_MECHAS 24
#define NB_ATTAQUES 64
#define NB_ZONES 10
#define NB_PNJ 24

//INVENTAIRE
#define NB_MECHAS_INVENTAIRE 4
#define NB_OBJET 5

//PNJ IMPORTANT 
#define VIN_GAZOLE_1 17
#define VIN_GAZOLE_2 18
#define VIN_GAZOLE_3 20
#define VIN_GAZOLE_4 21
#define VIN_GAZOLE_5 23

#define IRON_MUSK_DEB 19
#define IRON_MUSK_FIN 22



//RETURN 
#define ERREUR_OUVERTURE -1
#define OK 1
#define ERR 0
#define KO 0
#define RETOUR -1
#define VRAI 1
#define FAUX -2
#define CAPTURE -3
#define FUITE -4

//TAILLE D'UNE CASE
#define PX 32

//COLLISION
#define RIEN 0
#define JOUEUR 1
#define BARRIERE 2
#define BAT 3
#define DECHET 5

//INTERACTION
#define PNJ 6
#define PC 7
#define PORTE 4

//CHANGEMENT MAP

#define TPMAP1 -11
#define TPMAP2 -12
#define TPMAP3 -13
#define TPMAP4 -14
#define TPMAP5 -15
#define TPMAP6 -16

//ZONE SPAWN MECHA
#define Z1 -1
#define Z2 -2
#define Z3 -3
#define Z4 -4
#define Z5 -5
#define Z6 -6
#define Z7 -7
#define Z8 -8
#define Z9 -9
#define Z10 -10 

//DIMENSIONS
#define H 768
#define L 1280

//DIMENSIONS BOUTON
#define LARGEUR_BOUTON 200
#define HAUTEUR_BOUTON 50

//*********CONSTANTES********//
extern const int FRAME_DELAY;


//*********STRUCTURES********//
// Structure qui contient les initialisation de rendu du jeu 


//Structure qui contient les images animés du joueur
/**
 * @struct img_player_t
 * @brief Structure contenant les différentes textures du joueur selon les directions de déplacement.
 *
 * Cette structure regroupe les textures utilisées pour afficher les animations du personnage joueur lorsqu'il se déplace dans différentes directions. Chaque tableau contient quatre textures correspondant aux différentes étapes d'une animation dans la direction spécifiée.
 *
 * @var img_player_t::gauche
 * Tableau de 4 textures SDL représentant les étapes d'animation du joueur se déplaçant vers la gauche.
 *
 * @var img_player_t::droite
 * Tableau de 4 textures SDL représentant les étapes d'animation du joueur se déplaçant vers la droite.
 *
 * @var img_player_t::haut
 * Tableau de 4 textures SDL représentant les étapes d'animation du joueur se déplaçant vers le haut.
 *
 * @var img_player_t::bas
 * Tableau de 4 textures SDL représentant les étapes d'animation du joueur se déplaçant vers le bas.
 *
 * @note Les textures doivent être chargées au préalable à partir des fichiers images appropriés avant leur utilisation.
 *
 * @warning Il est nécessaire de libérer explicitement chacune de ces textures à la fermeture du jeu (via `SDL_DestroyTexture`) pour éviter toute fuite mémoire.
 */
typedef struct{
    SDL_Texture * gauche[4];
    SDL_Texture * droite[4];
    SDL_Texture * haut[4];
    SDL_Texture * bas[4];

}img_player_t;

/**
 * @struct img_pnj_t
 * @brief Structure regroupant les textures utilisées pour afficher les PNJs du jeu.
 *
 * Cette structure centralise les textures représentant les personnages non-joueurs (PNJs) dans différentes orientations et pour certains PNJs spécifiques :
 *
 * @var img_pnj_t::gauche
 * Texture SDL représentant l'apparence générique des PNJs lorsqu'ils sont orientés vers la gauche.
 *
 * @var img_pnj_t::droite
 * Texture SDL représentant l'apparence générique des PNJs lorsqu'ils sont orientés vers la droite.
 *
 * @var img_pnj_t::haut
 * Texture SDL représentant l'apparence générique des PNJs lorsqu'ils sont orientés vers le haut.
 *
 * @var img_pnj_t::bas
 * Texture SDL représentant l'apparence générique des PNJs lorsqu'ils sont orientés vers le bas.
 *
 * @var img_pnj_t::vin_gazole
 * Texture SDL spécifique représentant le PNJ nommé "Vin Gazole".
 *
 * @var img_pnj_t::iron_musk_bas
 * Texture SDL spécifique représentant le PNJ "Iron Musk" orienté vers le bas.
 *
 * @var img_pnj_t::iron_musk_droite
 * Texture SDL spécifique représentant le PNJ "Iron Musk" orienté vers la droite.
 *
 * @note Chaque texture doit être chargée avant utilisation via une fonction dédiée (`charger_texture`) depuis les fichiers d'images correspondants.
 *
 * @warning Il est impératif de libérer explicitement chacune de ces textures (via `SDL_DestroyTexture`) lors de la fermeture du jeu afin d'éviter toute fuite mémoire.
 */
typedef struct{
    SDL_Texture * gauche;
    SDL_Texture * droite;
    SDL_Texture * haut;
    SDL_Texture * bas;
    SDL_Texture * vin_gazole;
    SDL_Texture * iron_musk_bas;
    SDL_Texture * iron_musk_droite;
}img_pnj_t;

/**
 * @struct game_t
 * @brief Structure principale contenant les ressources et paramètres liés à la gestion du jeu.
 *
 * Cette structure regroupe l'ensemble des éléments nécessaires à l'affichage graphique et à la gestion des cartes et ressources :
 *
 * @var game_t::window
 * Pointeur vers la fenêtre SDL principale du jeu.
 *
 * @var game_t::renderer
 * Pointeur vers le renderer SDL utilisé pour dessiner les éléments à l'écran.
 *
 * @var game_t::backgroundTexture
 * Tableau de textures SDL pour les arrière-plans des différentes cartes ou menus (8 textures).
 *
 * @var game_t::calqueTexture
 * Tableau de textures SDL pour gérer différents calques visuels sur les cartes (6 textures).
 *
 * @var game_t::police
 * Pointeur vers une police TTF utilisée pour afficher du texte à l'écran.
 *
 * @var game_t::mat
 * Matrice à trois dimensions représentant les données logiques des cartes du jeu (id objets, collisions, états, etc.).
 *
 * @var game_t::scale
 * Facteur d'échelle utilisé pour adapter dynamiquement les éléments graphiques à la taille de la fenêtre.
 *
 * @var game_t::dms_win
 * Rectangle SDL (`SDL_Rect`) décrivant les dimensions et la position de la fenêtre d'affichage active.
 *
 * @var game_t::img_w
 * Largeur originale (en pixels) des images représentant les cartes du jeu.
 *
 * @var game_t::img_h
 * Hauteur originale (en pixels) des images représentant les cartes du jeu.
 *
 * @var game_t::mat_active
 * Indice de la matrice active actuellement utilisée pour afficher et gérer les interactions avec la carte en cours.
 *
 * @note Cette structure centralise les ressources clés utilisées par la plupart des fonctions du jeu.
 *       Elle permet d'éviter les accès multiples ou inutiles à des ressources partagées.
 *
 * @warning Les ressources SDL associées (textures, renderer, fenêtre, polices) doivent impérativement être libérées via SDL_Destroy* et TTF_CloseFont lorsque le jeu se termine, afin d’éviter toute fuite mémoire.
 */
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture[8];
    SDL_Texture* calqueTexture[6];
    img_player_t sprite_joueur;
    img_pnj_t sprite_pnj;
    TTF_Font* police;
    int *** mat ;
    float scale;                        //echelle qui adapte l'affichage à l'ecran et à la taille de l'image
    SDL_Rect dms_win;
    int img_w;
    int img_h;
    int mat_active;
} game_t;

//STRUCTURES D'AFFICHAGE
/**
 * @struct rectangle_t
 * @brief Structure représentant un rectangle graphique avec du texte à afficher dans une interface SDL.
 *
 * Cette structure permet de définir facilement un élément graphique composé d'un rectangle coloré, 
 * accompagné éventuellement d'un texte centré ou positionné à l'intérieur. 
 * Elle est principalement utilisée pour les boutons, les menus ou les dialogues dans une interface graphique.
 *
 * @var rectangle_t::rect
 * Structure `SDL_Rect` définissant la position (coordonnées x, y) et les dimensions (largeur, hauteur) du rectangle à afficher.
 *
 * @var rectangle_t::couleur
 * Structure `SDL_Color` définissant la couleur de fond du rectangle (RGBA).
 *
 * @var rectangle_t::text
 * Tableau de caractères (jusqu'à 500 caractères) contenant le texte affiché à l'intérieur du rectangle.
 *
 * @var rectangle_t::couleur_text
 * Structure `SDL_Color` définissant la couleur du texte à afficher (RGBA).
 *
 * @note La longueur maximale du texte est limitée à 500 caractères (modifiable selon besoin).
 * @note Le rendu du texte nécessite une police (`TTF_Font`) préalablement chargée via SDL_ttf.
 *
 * @warning Assure-toi que la police (`TTF_Font`) utilisée pour afficher le texte est correctement chargée et accessible.
 */
typedef struct {
    SDL_Rect rect;
    SDL_Color couleur;
    char text[500];
    SDL_Color couleur_text;
} rectangle_t;

/**
 * @struct parametre_t
 * @brief Structure contenant les paramètres réglables du jeu.
 *
 * Cette structure stocke divers paramètres de configuration du jeu modifiables par l'utilisateur.
 * Elle permet de centraliser et gérer facilement les préférences utilisateur, comme le niveau sonore du jeu.
 *
 * @var parametre_t::volume
 * Entier représentant le niveau du volume sonore général du jeu (typiquement de 0 à 100).
 *
 * @note Cette structure peut être étendue pour inclure d'autres paramètres utilisateur (luminosité, contrôles, langue, etc.).
 *
 * @warning Veille à sauvegarder et charger ces paramètres à partir d'un fichier de configuration pour conserver les préférences utilisateur entre les sessions.
 */
typedef struct {
    int volume;
} parametre_t;

//STRUCTURES DE DONNEES
/**
 * @struct attaque_t
 * @brief Structure représentant les caractéristiques d'une attaque d'un Mecha.
 *
 * Cette structure décrit toutes les caractéristiques d'une attaque disponible pour les Mechas du jeu.
 * Chaque attaque est identifiée par un ID unique et possède plusieurs attributs déterminant son efficacité et son utilisation.
 *
 * @var attaque_t::id_attaques
 * Entier représentant l'identifiant unique de l'attaque.
 *
 * @var attaque_t::nom
 * Chaîne de caractères (longueur `LONGUEUR_MAX_PSEUDO`) contenant le nom de l'attaque.
 *
 * @var attaque_t::type
 * Chaîne de caractères (longueur `LONGUEUR_TYPE`) indiquant le type de l'attaque (ex : feu, eau, électrique).
 *
 * @var attaque_t::niveau
 * Niveau minimal requis pour que le Mecha puisse apprendre/utiliser cette attaque.
 *
 * @var attaque_t::degats
 * Valeur représentant les dégâts potentiels infligés à l'adversaire.
 *
 * @var attaque_t::precision
 * Valeur (en pourcentage) indiquant la probabilité de réussite de l'attaque.
 *
 * @var attaque_t::utilisations
 * Nombre maximal d'utilisations possibles de cette attaque avant épuisement (points de pouvoir ou PP).
 *
 * @note Les constantes `LONGUEUR_MAX_PSEUDO` et `LONGUEUR_TYPE` doivent être définies dans les fichiers d'en-tête.
 *
 * @warning Assure-toi que les valeurs (dégâts, précision, utilisations) sont correctement validées afin de maintenir l'équilibre du jeu.
 */
typedef struct{
    int id_attaques;
    char nom[LONGUEUR_MAX_PSEUDO];
    char type[LONGUEUR_TYPE];
    int niveau;
    int degats;
    int precision;
    int utilisations;
} attaque_t;

/**
 * @struct mechas_t
 * @brief Représente un mecha disponible dans le jeu avec ses caractéristiques.
 * 
 * Cette structure contient toutes les informations statiques d’un mecha, 
 * telles que son type, ses attaques disponibles, sa description et ses 
 * critères d’évolution.
 * 
 * @var mechas_t::id_mechas
 * Identifiant unique du mecha.
 * 
 * @var mechas_t::nom
 * Nom du mecha.
 * 
 * @var mechas_t::type
 * Type du mecha (ex: Carburant, Electrique, Renouvelable, Nucleaire...).
 * 
 * @var mechas_t::evolution
 * Identifiant du mecha en lequel celui-ci évolue (0 si aucune évolution).
 * 
 * @var mechas_t::niveau_evolution
 * Niveau requis pour que le mecha évolue.
 * 
 * @var mechas_t::description
 * Description détaillée du mecha.
 * 
 * @var mechas_t::liste_attaque
 * Tableau des identifiants des attaques que le mecha peut apprendre (taille maximale : 5).
 * 
 * @var mechas_t::nb_attaques
 * Nombre total d’attaques dans la liste que ce mecha peut apprendre.
 */
typedef struct{
    SDL_Texture * texture[2];
    int id_mechas;
    char nom[LONGUEUR_MAX_PSEUDO];
    char type[LONGUEUR_TYPE];
    int evolution;
    int niveau_evolution;
    char description[LONGUEUR_DESC];
    int liste_attaque[5];
    int nb_attaques;
} mechas_t;

/**
 * @struct inventaire_t
 * @brief Représente l’inventaire d’un joueur contenant les objets utilisables.
 * 
 * Cette structure stocke la quantité de chaque type d’objet que le joueur possède.
 * Les objets sont utilisés principalement pendant les combats ou lors des déplacements.
 * 
 * @var inventaire_t::mechaball
 * Nombre de Mechaballs, utilisées pour capturer des mechas sauvages.
 * 
 * @var inventaire_t::carburant
 * Nombre de carburants, utilisés pour soigner un mecha (récupérer des PV).
 * 
 * @var inventaire_t::repousse
 * Nombre de repousses, utilisées pour éviter les combats sauvages pendant un temps.
 * 
 * @var inventaire_t::rappel
 * Nombre de rappels, utilisés pour réanimer un mecha KO (PV à 0).
 */
typedef struct{
    int mechaball;
    int carburant;
    int repousse;
    int rappel;
} inventaire_t;

/**
 * @struct mechas_joueur_t
 * @brief Représente un mecha appartenant à un joueur, avec ses statistiques et attaques.
 * 
 * Cette structure contient les informations dynamiques d’un mecha possédé par un joueur, 
 * notamment son niveau, ses points de vie, ses statistiques de combat et les attaques 
 * actuellement apprises.
 * 
 * @var mechas_joueur_t::numero
 * Numéro du mecha dans l’équipe du joueur (position dans l’inventaire/combat).
 * 
 * @var mechas_joueur_t::id_mechas
 * Identifiant du modèle de mecha (référence à la structure `mechas_t`).
 * 
 * @var mechas_joueur_t::niveau
 * Niveau actuel du mecha.
 * 
 * @var mechas_joueur_t::xp
 * Expérience accumulée vers le niveau suivant.
 * 
 * @var mechas_joueur_t::pv
 * Points de vie actuels du mecha.
 * 
 * @var mechas_joueur_t::pv_max
 * Points de vie maximum du mecha.
 * 
 * @var mechas_joueur_t::attaque
 * Statistique d’attaque du mecha (influence les dégâts infligés).
 * 
 * @var mechas_joueur_t::defense
 * Statistique de défense du mecha (réduit les dégâts reçus).
 * 
 * @var mechas_joueur_t::vitesse
 * Statistique de vitesse (peut influencer l’ordre des tours).
 * 
 * @var mechas_joueur_t::attaque_1
 * Identifiant de la première attaque apprise (référence à `attaque_t`).
 * 
 * @var mechas_joueur_t::attaque_2
 * Identifiant de la seconde attaque apprise (référence à `attaque_t`).
 * 
 * @var mechas_joueur_t::utilisation_1
 * Nombre d’utilisations restantes pour l’attaque 1.
 * 
 * @var mechas_joueur_t::utilisation_2
 * Nombre d’utilisations restantes pour l’attaque 2.
 */
typedef struct{
    int numero;
    int id_mechas;
    int niveau;
    int xp;
    int pv;
    int pv_max;
    int attaque;
    int defense;
    int vitesse;
    int attaque_1;
    int attaque_2;
    int utilisation_1;
    int utilisation_2;
} mechas_joueur_t;

/**
 * @struct joueur_t
 * @brief Représente le joueur dans le jeu, avec sa position, son équipe de mechas et son inventaire.
 * 
 * Cette structure regroupe toutes les informations relatives au joueur, que ce soit pour la 
 * gestion de la position sur la carte, les déplacements, l’état de l’équipe de mechas, ou encore 
 * l’inventaire d’objets.
 * 
 * @var joueur_t::pseudo
 * Pseudonyme du joueur (nom personnalisé).
 * 
 * @var joueur_t::sexe
 * Sexe du joueur (ex : 'M' pour masculin, 'F' pour féminin).
 * 
 * @var joueur_t::numMap
 * Numéro de la carte (map) actuelle sur laquelle se trouve le joueur.
 * 
 * @var joueur_t::x
 * Coordonnée X du joueur sur la carte (en cases).
 * 
 * @var joueur_t::y
 * Coordonnée Y du joueur sur la carte (en cases).
 * 
 * @var joueur_t::screen_x
 * Position X réelle à l’écran (en pixels, pour l’affichage lisse/animé).
 * 
 * @var joueur_t::screen_y
 * Position Y réelle à l’écran (en pixels).
 * 
 * @var joueur_t::moving
 * Indicateur booléen (0/1) indiquant si un déplacement est en cours.
 * 
 * @var joueur_t::move_dx
 * Direction de déplacement sur l’axe X (valeur flottante pour animation fluide).
 * 
 * @var joueur_t::move_dy
 * Direction de déplacement sur l’axe Y.
 * 
 * @var joueur_t::derniere_touche
 * Dernière touche directionnelle pressée (utile pour l’animation du sprite).
 * 
 * @var joueur_t::pointSauvegarde
 * Numéro du dernier point de sauvegarde atteint.
 * 
 * @var joueur_t::nb_mechas
 * Nombre de mechas que possède actuellement le joueur.
 * 
 * @var joueur_t::inventaire
 * Pointeur vers la structure `inventaire_t` contenant les objets du joueur.
 * 
 * @var joueur_t::mechas_joueur
 * Tableau de `NB_MECHAS_MAX` mechas possédés par le joueur (type `mechas_joueur_t`).
 * 
 * @var joueur_t::proba_combat
 * Probabilité (en %) qu’un combat sauvage se déclenche lors des déplacements.
 */
typedef struct{
    char pseudo[LONGUEUR_MAX_PSEUDO];
    char sexe;
    int numMap;
    int x, y;
    float screen_x, screen_y;       // Position réelle à l'écran 
    int moving;                     // Indique si un déplacement est en cours
    float move_dx, move_dy;           // Direction du mouvement en pixels
    int derniere_touche;
    int pointSauvegarde;
    int nb_mechas;
    inventaire_t *inventaire;
    mechas_joueur_t mechas_joueur [NB_MECHAS_MAX];
    int proba_combat;
} joueur_t;

/**
 * @struct pnj_t
 * @brief Représente un PNJ (Personnage Non Joueur) dans le jeu, avec sa position, ses dialogues et ses mechas.
 * 
 * Cette structure contient toutes les informations nécessaires pour gérer les PNJ dans le jeu, 
 * qu’il s’agisse de leur position sur la carte, de leurs dialogues, ou des mechas qu’ils utilisent 
 * lors des combats.
 * 
 * @var pnj_t::id_pnj
 * Identifiant unique du PNJ.
 * 
 * @var pnj_t::id_map
 * Identifiant de la carte (map) sur laquelle le PNJ est présent.
 * 
 * @var pnj_t::pseudo
 * Nom du PNJ affiché dans le jeu.
 * 
 * @var pnj_t::etat
 * État du PNJ (utile pour gérer les événements comme l’activation ou la progression dans l’histoire).
 * 
 * @var pnj_t::dialogueDebut
 * Dialogue affiché avant une interaction ou un combat (début).
 * 
 * @var pnj_t::dialogueFin
 * Dialogue affiché après une interaction ou un combat (fin).
 * 
 * @var pnj_t::x
 * Coordonnée X du PNJ sur la carte.
 * 
 * @var pnj_t::y
 * Coordonnée Y du PNJ sur la carte.
 * 
 * @var pnj_t::orientation
 * Orientation actuelle du PNJ (1 = haut, 2 = droite, 3 = bas, 4 = gauche).
 * 
 * @var pnj_t::nb_mechas
 * Nombre de mechas que possède le PNJ pour le combat.
 * 
 * @var pnj_t::inventaire
 * Pointeur vers la structure `inventaire_t` contenant les objets utilisables par le PNJ.
 * 
 * @var pnj_t::mechas_joueur
 * Tableau de mechas utilisés par le PNJ durant les combats (`mechas_joueur_t`).
 */
typedef struct{
    int id_pnj;
    int id_map;
    char pseudo[LONGUEUR_MAX_PSEUDO];
    int etat;
    char dialogueDebut[LONGUEUR_DIALOGUE];
    char dialogueFin[LONGUEUR_DIALOGUE];
    int x, y;
    int orientation;
    int nb_mechas;
    inventaire_t *inventaire;
    mechas_joueur_t mechas_joueur [NB_MECHAS_MAX];
} pnj_t;

/**
 * @struct zone_t
 * @brief Représente une zone de spawn pour les mechas sauvages dans le jeu.
 * 
 * Cette structure contient les caractéristiques des zones où apparaissent des mechas sauvages, 
 * ainsi que la liste des mechas pouvant y être rencontrés. Elle est utilisée pour déterminer 
 * quels types de mechas peuvent apparaître aléatoirement et avec quelles statistiques moyennes.
 * 
 * @var zone_t::id_zone
 * Identifiant unique de la zone.
 * 
 * @var zone_t::NiveauMoyenApparition
 * Niveau moyen des mechas qui apparaissent dans cette zone.
 * 
 * @var zone_t::PvMoyen
 * Points de vie moyens des mechas sauvages dans la zone.
 * 
 * @var zone_t::VitesseMoyenne
 * Vitesse moyenne des mechas sauvages.
 * 
 * @var zone_t::Attaque
 * Attaque moyenne des mechas dans la zone.
 * 
 * @var zone_t::Defense
 * Défense moyenne des mechas dans la zone.
 * 
 * @var zone_t::listeMechasZone
 * Tableau contenant les identifiants (ID) des mechas pouvant apparaître dans cette zone (maximum 10).
 * 
 * @var zone_t::nb_mechas
 * Nombre total de mechas définis pour cette zone (doit être ≤ 10).
 */
typedef struct{
    int id_zone;
    int NiveauMoyenApparition;
    int PvMoyen;
    int VitesseMoyenne;
    int Attaque;
    int Defense;
    int listeMechasZone[10];
    int nb_mechas;
}zone_t;


//VARIABLES GLOBALE
extern mechas_t mecha[NB_MECHAS];
extern attaque_t attaque[NB_ATTAQUES];
extern zone_t zone[NB_ZONES];
extern pnj_t pnj[NB_PNJ];
extern game_t game;

#endif
