#include "fleets.h"
#define SIZE_MATRIX 10
#define NB_TIRS 4
#define TIR_LIGNE 0
#define TIR_CROIX 1
#define TIR_PLUS 2
#define TIR_CARRE 3
#define TIR_NORMAL 4
#define DANS_EAU 0
#define DEJA_TOUCHE 1
#define TOUCHE 2
#define TOUCHE_COULE 3

typedef struct human { // Structure réservé au joueur humain
} Human;

typedef struct ia { // Structure réservé à l'IA
    Coordonnees *lastCoord; // Coordonnées où l'IA a touchée
    int vertical; // Si le bateau est vertical, horizontal, ou pas encore trouvé
    int hitBoat;
} IA;

typedef union playerType { // Tir spécial si joueur humain, coordonnée de touche si IA
    Human human;
    IA ia;
} PlayerType;

typedef struct players { // Structure d'un joueur
    char *name;
    char matrix[SIZE_MATRIX][SIZE_MATRIX]; // Matrice de jeu
    Fleet *fleet; // Sa flotte de navires
    int hasHit; // S'il a touché un navire ennemi
    int special[NB_TIRS]; // Tableau de tirs spéciaux
    int hasUsedSpecial; // S'il a utilisé un tir spécial lors de son dernier tir
    PlayerType playerType; // Type de joueur (IA ou humain)
} Player;

void printMatrix(Player *);
int charToInt(char);
void choseCoord(Coordonnees *);
int selectionTir(Player *, Player *, int);
int choseTir(Player *, Player *);
void freeCoordsBoats(Player *);
void freeFleets(Player *);