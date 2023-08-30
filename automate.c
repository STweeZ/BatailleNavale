#include "automate.h"

void searchForCoords(Coordonnees *coordonnees, Player *player) { // L'IA va chercher des coordonnées où faire feu
    int correct=0;
    int toMuchOccupateCases=0; // Quand il ne reste plus de case qui n'est pas entourée par aucune case touchée
    int xAxis;
    int yAxis;
    while(!correct) { // Tant que les coordonnées choisies ne sont pas valides
        toMuchOccupateCases++;
        xAxis=rand()%SIZE_MATRIX; // Choix d'un x random
        yAxis=rand()%SIZE_MATRIX; // Choix d'un y random
        correct=1;
        if(player->matrix[xAxis][yAxis] == '.' || player->matrix[xAxis][yAxis] == '0') { // Si la case choisie n'a pas encore été touchée
            if(toMuchOccupateCases < 200) { // >= 200 si il ne reste plus de case qui n'est pas entouré par d'autres cases -> L'automate a touché beaucoup trop de cases dans l'eau
                if((xAxis-1)>=0 && (player->matrix[xAxis-1][yAxis] != '.' && player->matrix[xAxis-1][yAxis] != '0')) { // Vérifications si la case n'est pas entourée par une case déjà touchée
                    correct=0;
                }
                if((xAxis+1)<SIZE_MATRIX && (player->matrix[xAxis+1][yAxis] != '.' && player->matrix[xAxis+1][yAxis] != '0')) {
                    correct=0;
                }
                if((yAxis-1)>=0 && (player->matrix[xAxis][yAxis-1] != '.' && player->matrix[xAxis][yAxis-1] != '0')) {
                    correct=0;
                }
                if((yAxis+1)<SIZE_MATRIX && (player->matrix[xAxis][yAxis+1] != '.' && player->matrix[xAxis][yAxis+1] != '0')) {
                    correct=0;
                }
            }
        } else {
            correct=0;
        }
    }
    coordonnees->x=xAxis;
    coordonnees->y=yAxis;
}

int verticalMovement(Coordonnees *coordonnees, Player *player1, Player *player2) { // Mouvement vertical de l'IA
    int xAxis=player1->playerType.ia.lastCoord->x;
    int yAxis=player1->playerType.ia.lastCoord->y;
    while(yAxis>0 && player2->matrix[xAxis][yAxis] == '#') { // Tant qu'elle a déjà touché les cases vers le haut
        yAxis--;
    }
    if(player2->matrix[xAxis][yAxis] != 'X' && player2->matrix[xAxis][yAxis] != '#'); // Si elle s'arrête sur une case qui n'est pas une case d'eau et qui n'est pas case de bateau touché on ne fait rien
    else {
        yAxis++;
        while(yAxis<SIZE_MATRIX-1 && player2->matrix[xAxis][yAxis] == '#') { // On parcours le bas à présent à la recherche d'une case non étudiée
            yAxis++;
        }
    }
    coordonnees->x=xAxis;
    coordonnees->y=yAxis;
    if(player2->matrix[xAxis][yAxis] == 'X' || yAxis == (SIZE_MATRIX-1)) return 0; // Il n'y a plus rien à la vertical alors on va voir à l'horizontale
    return 1;
}

void horizontalMovement(Coordonnees *coordonnees, Player *player1, Player *player2) { // Mouvement horizontal de l'IA
    int xAxis=player1->playerType.ia.lastCoord->x;
    int yAxis=player1->playerType.ia.lastCoord->y;
    while(xAxis>0 && player2->matrix[xAxis][yAxis] == '#') { // On parcours la gauche tant qu'on est sur une case de bateau touché
        xAxis--;
    }
    if(player2->matrix[xAxis][yAxis] != 'X' && player2->matrix[xAxis][yAxis] != '#'); // Si elle s'arrête sur une case qui n'est pas une case d'eau et qui n'est pas case de bateau touché on ne fait rien
    else {
        xAxis++;
        while(xAxis<SIZE_MATRIX-1 && player2->matrix[xAxis][yAxis] == '#') { // On parcours l'autre côté du bateau à la recherche d'une case non étudiée
            xAxis++;
        }
    }
    coordonnees->x=xAxis;
    coordonnees->y=yAxis;
}

void nextCoords(Coordonnees *coordonnees, Player *player1, Player *player2) { // Automatisme du choix de tir -> Tirer verticalement ou horizontalement
    if(player1->playerType.ia.vertical == 1) { // Si l'IA sait que le bateau se trouve verticalement
        verticalMovement(coordonnees,player1,player2); // Tir à la verticale
    } else if(player1->playerType.ia.vertical == 0) { // Elle sait qu'il est à l'horizontale
        horizontalMovement(coordonnees,player1,player2); // Tir horizontal
    } else {
        if(!verticalMovement(coordonnees,player1,player2)) { // L'IA ne sait pas encore -> Recherche verticale tout d'abord
            horizontalMovement(coordonnees,player1,player2); // Sinon elle sait à présent qu'il est à l'horizontale
            player1->playerType.ia.vertical=0;
        }
    }
}

void printIACoords(int y, int x) { // Affichage des coordonnées de l'IA selon les nomres d'affichage -> 'A6'
    char c=x+17+'0';
    printf("%c%d\n",c,y+1);
}

int choseAutomateTir(Player *player1, Player *player2) { // Choix du tir
    if(!player1->hasHit || player1->hasUsedSpecial) { // Consigne -> N'a pas touché un bateau au tour d'avant et/ou a utilisé un tir spécial
        player1->hasUsedSpecial=0; // N'a pas utilisé un tir spécial
        return TIR_NORMAL; // Tir normal utilisé
    }
    int tir=rand()%(NB_TIRS+1); // Choix du type de tir random
    return selectionTir(player1,player2,tir);
}

void automate(Player *player1, Player *player2) { // Fonction principale de l'automate
    int stateOfShot=0; // Si c'est touché, coulé, touché coulé, ou autre
    Coordonnees *c=malloc(sizeof(Coordonnees));
    if(!player1->playerType.ia.hitBoat) { // Si l'IA est à la recherche d'un bateau
        searchForCoords(c,player2); // Choix des coordonnées où faire feu
    } else {
        nextCoords(c,player1,player2); // L'IA s'occupe actuellement d'un bateau et elle cherche les prochaines coordonnées où faire feu
    }
    printf("Choix des coordonnées où faire feu : ");
    printIACoords(c->x,c->y);
    int typeOfTir=choseAutomateTir(player1,player2);
    if(typeOfTir != 4) {
        switch(typeOfTir) {
            case TIR_LIGNE:
            printf("TIR EN LIGNE\n");
            break;
        case TIR_CROIX:
            printf("TIR EN CROIX\n");
            break;
        case TIR_PLUS:
            printf("TIR EN PLUS\n");
            break;
        case TIR_CARRE:
            printf("TIR CARRE\n");
            break;
        default :
            break;
        }
    }
    Coordonnees **cibles=getCoordFire(c,typeOfTir);
    stateOfShot=multipleFires(cibles,player2); // Feu sur les cases choisies
    if(stateOfShot==2) { // Si l'IA a touché coulé
        player1->playerType.ia.vertical=-1; // Réinitialisation de l'état vertical
        player1->hasHit=1; // L'IA a touché le bateau
        player1->playerType.ia.hitBoat=0; // L'IA ne s'occupe plus actuellement d'un bateau
        free(player1->playerType.ia.lastCoord); // Libération des coordonnées de la case enregistrée
        player1->playerType.ia.lastCoord=NULL;
    } else if(stateOfShot==1) { // L'IA a touché
        if(!player1->playerType.ia.lastCoord) { // Elle ne s'occupait pas déjà d'un bateau
            player1->playerType.ia.lastCoord=malloc(sizeof(Coordonnees *)); // Enregistrement de la case initiale
            player1->playerType.ia.lastCoord->x=c->x;
            player1->playerType.ia.lastCoord->y=c->y;
            player1->playerType.ia.hitBoat=1;
        } else if(player1->playerType.ia.vertical == -1 && !player1->hasUsedSpecial){ // L'IA ne savait pas encore le sens du bateau
            if(player1->playerType.ia.lastCoord->x==c->x) { // Si la case touché à le même x que la case initiale alors le bateau est à la verticale
                player1->playerType.ia.vertical=1;
            } else player1->playerType.ia.vertical=0; // Si non, horizontal
        }
        player1->hasHit=1; // L'IA a touché
    } else {
        player1->hasHit=0;
    }
    free(c); // Libération des coordonnées choisis
    freeTabOfCibles(cibles);
}