#include "coords.h"
#define NUMBER_OF_BOATS 5
#define PORTE_AVION 5
#define CROISEUR 4
#define DESTROYER 3
#define SOUS_MARIN 3
#define TORPILLEUR 2

typedef struct boats { // Structure pour les bateaux
    int type; // Nom du bateau
    int size;
    Coordonnees *coord; // Où il se situe
    int isDown; // S'il a été abattu
    int isVertical;
} Boat;

typedef struct fleets { // Flotte de navires
    Boat boats[NUMBER_OF_BOATS]; // Tableau de bateaux
    int numberOfBoats;
} Fleet;