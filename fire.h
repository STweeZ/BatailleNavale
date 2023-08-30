#include "initialize.h"

int boatIsDown(Player *, Boat);
int shotOnBoat(Player *, Coordonnees *);
int fire(Player *, Coordonnees *);
int tir_en_croix(Coordonnees **, Coordonnees *, int);
int tir_en_plus(Coordonnees **, Coordonnees *, int);
Coordonnees **getCoordFire(Coordonnees *, int);
int multipleFires(Coordonnees **, Player *);
void freeTabOfCibles(Coordonnees **);
void freePotientialShot(Player *);