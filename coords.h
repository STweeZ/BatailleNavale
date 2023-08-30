#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define isdigit(a) ((a)>='0' && (a)<='9')
#define isalpha(a) (isupper(a) || islower(a))
#define isupper(a) ((a)>='A' && (a)<='Z')
#define islower(a) ((a)>='a' && (a)<='z')
#define toupper(a) (islower(a) ? (a-32) : a)
#define tolower(a) (isupper(a) ? (a+32) : a)
#define alphatodigit(a) (toupper(a)-16)

typedef struct coords { // Structure pour les coordonnées
    int x;
    int y;
} Coordonnees;