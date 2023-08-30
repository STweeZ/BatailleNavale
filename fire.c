#include "fire.h"

int boatIsDown(Player *p, Boat b) { // Si le bateau b du joueur p a coulé
    for(int i=0; i<b.size; i++) { // Parcours de sa taille
        if(b.isVertical) { // S'il est vertical -> Parcours vers le haut
            if(p->matrix[b.coord->x][b.coord->y+i] != '#') return 0; // N'a pas coulé car autre que #
        } else {
            if(p->matrix[b.coord->x+i][b.coord->y] != '#') return 0;
        }
    }
    return 1;
}

int shotOnBoat(Player *player, Coordonnees *c) { // Tir simple sur une case bateau
    int isShot; // Type de touché
    for(int i=0; i<NUMBER_OF_BOATS; i++) { // On cherche de quel bateau il s'agit
        if(!player->fleet->boats[i].isDown) { // Qui n'a pas encore coulé
            isShot=0;
            for(int j=0; j<player->fleet->boats[i].size; j++) { // On le parcours de gauche à droite ou de haut en bas
                if(player->fleet->boats[i].isVertical) {
                    if(player->fleet->boats[i].coord->x == c->x && (player->fleet->boats[i].coord->y + j) == c->y) { // C'est le bateau-là qu'on a touché
                        isShot=1;
                        break;
                    }
                } else {
                    if((player->fleet->boats[i].coord->x + j) == c->x && (player->fleet->boats[i].coord->y) == c->y) {
                        isShot=1;
                        break;
                    }
                }
            }
            if(isShot) { // Le bateau actuel a été touché
                player->matrix[c->x][c->y]='#'; // On change l'état de la case pour 'Bateau touché'
                if(boatIsDown(player, player->fleet->boats[i])) { // Vérification si le bateau a de ce fait coulé ou non
                    player->fleet->boats[i].isDown = 1; // A coulé
                    player->fleet->numberOfBoats--; // Un bateau en moins
                    return TOUCHE_COULE;
                }
                return TOUCHE;
            }
        }
    }
}

int fire(Player *player, Coordonnees *c) { // Feu sur une case de coordonnée c sur la matrice du joueur player
    int state=0; // Etat du tir
    switch(player->matrix[c->x][c->y]) { // Pour la case concernée
        case '0': // Case bateau donc tir sur celle-ci
            state=shotOnBoat(player,c);
            break;
        case '.': // Case eau
            state=DANS_EAU;
            player->matrix[c->x][c->y]='X';
            break;
        case 'X': // Case eau
            state=DANS_EAU;
            break;
        case '#': // Case de bateau déjà touché
            state=DEJA_TOUCHE;
            break;
        default :
            break;
    }
    return state;
}

int tir_en_croix(Coordonnees **cibles, Coordonnees *coord, int nbCases) { // Tir spécial en croix sur les coordonnées initiales coord
    if(coord->x-1 >= 0) { // Pour ne pas dépasser la taille de la matrice sur la gauche
        if(coord->y-1 >= 0) { // Pas de dépassement de taille en y
            cibles[nbCases]=malloc(sizeof(Coordonnees)); // Création d'une cellule cible suivant le schéma en croix et ajout de la cellule cible au pointeur sur le tableau de cellules cibles
            cibles[nbCases]->x=coord->x-1;
            cibles[nbCases]->y=coord->y-1;
            nbCases++; // Calcul du nombre de cases cibles
        }
        if(coord->y+1 < SIZE_MATRIX) {
            cibles[nbCases]=malloc(sizeof(Coordonnees));
            cibles[nbCases]->x=coord->x-1;
            cibles[nbCases]->y=coord->y+1;
            nbCases++;
        }
    }
    if(coord->x+1 < SIZE_MATRIX) { // Idem par symétrie
        if(coord->y-1 >= 0) {
            cibles[nbCases]=malloc(sizeof(Coordonnees));
            cibles[nbCases]->x=coord->x+1;
            cibles[nbCases]->y=coord->y-1;
            nbCases++;
        }
        if(coord->y+1 < SIZE_MATRIX) {
            cibles[nbCases]=malloc(sizeof(Coordonnees));
            cibles[nbCases]->x=coord->x+1;
            cibles[nbCases]->y=coord->y+1;
            nbCases++;
        }
    }
    return nbCases;
}

int tir_en_plus(Coordonnees **cibles, Coordonnees *coord, int nbCases) { // Tir spécial en plus sur les coordonnées initiales coord
    if(coord->y-1 >= 0) { // Pas de dépassement de taille en y
        cibles[nbCases]=malloc(sizeof(Coordonnees)); // Création d'une cellule cible suivant le schéma en plus et ajout de la cellule cible au pointeur sur le tableau de cellules cibles
        cibles[nbCases]->x=coord->x;
        cibles[nbCases]->y=coord->y-1;
        nbCases++;
    }
    if(coord->y+1 < SIZE_MATRIX) {
        cibles[nbCases]=malloc(sizeof(Coordonnees));
        cibles[nbCases]->x=coord->x;
        cibles[nbCases]->y=coord->y+1;
        nbCases++;
    }
    if(coord->x-1 >= 0) { // Pas de dépassement de taille en x
        cibles[nbCases]=malloc(sizeof(Coordonnees));
        cibles[nbCases]->x=coord->x-1;
        cibles[nbCases]->y=coord->y;
        nbCases++;
    }
    if(coord->x+1 < SIZE_MATRIX) {
        cibles[nbCases]=malloc(sizeof(Coordonnees));
        cibles[nbCases]->x=coord->x+1;
        cibles[nbCases]->y=coord->y;
        nbCases++;
    }
    return nbCases;
}

Coordonnees **getCoordFire(Coordonnees *coord, int tir) { // Contrainte du projet -> Retour d'un pointeur sur un tableau de cellules cibles, avec pour paramètre d'entrée les coordonnées centrales
    int nbCases=1; // Nombre de cases cibles
    Coordonnees **cibles=malloc((SIZE_MATRIX+1)*sizeof(Coordonnees *)); // Création du pointeur sur le tableau de cellules cibles
    cibles[0]=malloc(sizeof(Coordonnees)); // Initialisation de la cellule cible centrale
    cibles[0]->x=coord->x;
    cibles[0]->y=coord->y;
    switch(tir) { // En fonction du tir -> Spécial ou normal
        case TIR_LIGNE:
            for(int i=0; i<SIZE_MATRIX; i++) { // Ajout des cellules présentes sur la même ligne que la cellule centrale
                if(i!=coord->y) {
                    cibles[nbCases]=malloc(sizeof(Coordonnees));
                    cibles[nbCases]->x=coord->x;
                    cibles[nbCases]->y=i;
                    nbCases++;
                }
            }
            break;
        case TIR_CROIX:
            nbCases=tir_en_croix(cibles,coord,nbCases); // Ajout des cellules suivants le schéma en croix
            break;
        case TIR_PLUS:
            nbCases=tir_en_plus(cibles,coord,nbCases); // Ajout des cellules suivants le schéma en plus
            break;
        case TIR_CARRE: // Ajout des cellules suivants le schéma en carré -> Tir en croix + Tir en plus = Tir carré
            nbCases=tir_en_croix(cibles,coord,nbCases);
            nbCases=tir_en_plus(cibles,coord,nbCases);
            break;
        default :
            break;
    }
    cibles[nbCases]=NULL; // Dernière cellule étant une cellule vide pour faciliter la lecture du tableau
    cibles=realloc(cibles,(nbCases+1)*sizeof(Coordonnees *)); // Réallocation
    return cibles;
}

int multipleFires(Coordonnees **cibles, Player *player) { // Tir multiple ou simple selon le tableau de cellules cibles pointé par le pointeur cibles
    int i=0;
    int state=DANS_EAU; // Etat général du tir ou des tirs
    while(cibles[i]) { // Pour chaque cellule cible
        int etatTir=fire(player,cibles[i]); // Feu sur la cellule et lecture de l'état
        if(etatTir==TOUCHE_COULE) state=etatTir;
        else if(etatTir==TOUCHE && state!=TOUCHE_COULE) state=etatTir;
        else if(etatTir==DEJA_TOUCHE && state!=TOUCHE && state!=TOUCHE_COULE) state=etatTir;
        i++;
    }
    switch(state) { // Information transmmise quant à l'état dur tir
        case TOUCHE_COULE:
            printf("Touché, coulé !\n");
            return 2;
            break;
        case TOUCHE:
            printf("Touché !\n");
            return 1;
            break;
        case DEJA_TOUCHE:
            printf("Déjà touché !\n");
            return 0;
            break;
        case DANS_EAU:
            printf("Dans l'eau !\n");
            return 0;
            break;
        default :
            break;
    }
}

void freeTabOfCibles(Coordonnees **cibles) { // Libération de l'espace allouée pour le pointeur vers le tableau de cellules cibles
    int i=0;
    while(cibles[i]) { // Dans un premier temps libération de l'espace alloué pour chaque cellule cible
        free(cibles[i]);
        i++;
    }
    free(cibles); // Libération de l'espace occupé par le pointeur du tableau
}

void freePotientialShot(Player *player) { // Libération potentielle des coordonnées sauvegardées par l'IA fournie en paramètre d'entrée
    if(player->playerType.ia.lastCoord) {
        free(player->playerType.ia.lastCoord);
    }
}