#include "players.h"

void printMatrix(Player *player) { // Affichage de la matrice du joueur player
    printf("\nMatrice de %s\n\t",player->name); // Nom du joueur
    for(int a=0; a<SIZE_MATRIX; a++) printf("%c\t",'A'+a); // Caractère en x
    printf("\n\n");
    for(int i=0; i<SIZE_MATRIX; i++) {
        printf("%d\t",i+1); // Chiffre en y
        for(int j=0; j<SIZE_MATRIX; j++) printf("%c\t",player->matrix[i][j]); // Contenu de la case dans la matrice du joueur
        printf("\n\n");
    }
}

int charToInt(char c) { // Retourne la valeur entière d'un caractère fourni
    return toupper(c)-64;
}

void choseCoord(Coordonnees *coordonnees) { // Choix des coordonnées pour le joueur humain
    int correct=0; // Si les coordonnées sont valides compte tenu de la matrice et de sa taille
    char coord[3];
    int xAxis;
    int yAxis;
    while(!correct) {
        printf("Choisissez des coordonnées où faire feu : ");
        scanf("%s",coord); // Choix du joueur de la forme 'A6'
        correct=1;
        if(strlen(coord)<2 || !isalpha(coord[0]) || !isdigit(coord[1]) || (strlen(coord)>2 && !isdigit(coord[2]))) { // Problème de format
            printf("Le format de coordonnées est invalide\n");
            correct=0;
        } else if(toupper(coord[0]) > 64+SIZE_MATRIX) { // Trop grand
            printf("Coordonnées qui dépassent la taille de la matrice\n");
            correct=0;
        } else {
            xAxis=coord[1] - '0';
            if(xAxis<1 || strlen(coord)>2 && (xAxis=xAxis*10+coord[2] - '0') > SIZE_MATRIX) { // Trop petit
                printf("Coordonnées qui dépassent la taille de la matrice\n");
                correct=0;
            }
        }
    }
    yAxis=charToInt(coord[0]);
    coordonnees->x=--xAxis;
    coordonnees->y=--yAxis;
}

int selectionTir(Player *player1, Player *player2, int tir) {
    switch(tir) { // Vérification que le chiffre concerne un tir spécial
        case TIR_LIGNE:
            if(!player2->fleet->boats[3].isDown && player1->special[TIR_LIGNE]) { // Bateau non coulé et tir spécial en question disponible
                player1->special[TIR_LIGNE]=0; // Ne pourra plus utiliser ce tir spécial
                player1->hasUsedSpecial=1; // Vient d'utiliser un tir spécial
                return tir;
            }
            break;
        case TIR_CROIX:
            if(!player2->fleet->boats[1].isDown && player1->special[TIR_CROIX]) {
                player1->special[TIR_CROIX]=0;
                player1->hasUsedSpecial=1;
                return tir;
            }
            break;
        case TIR_PLUS:
            if(!player2->fleet->boats[1].isDown && player1->special[TIR_PLUS]) {
                player1->special[TIR_PLUS]=0;
                player1->hasUsedSpecial=1;
                return tir;
            }
            break;
        case TIR_CARRE:
            if(!player2->fleet->boats[0].isDown && player1->special[TIR_CARRE]) {
                player1->special[TIR_CARRE]=0;
                player1->hasUsedSpecial=1;
                return tir;
            }
            break;
        default :
            break;
    }
    player1->hasUsedSpecial=0; // N'a pas utilisé un tir spécial
    return TIR_NORMAL; // Tir normal utilisé
}

int choseTir(Player *player1, Player *player2) { // Choix du tir
    if(!player1->hasHit || player1->hasUsedSpecial) { // Consigne -> N'a pas touché un bateau au tour d'avant et/ou a utilisé un tir spécial
        player1->hasUsedSpecial=0; // N'a pas utilisé un tir spécial
        return TIR_NORMAL; // Tir normal utilisé
    }
    int haveSpecials=0; // S'il a encore des tirs spéciaux à dépenser
    for(int i=0; i<NB_TIRS; i++) { // Pour chaque tir spécial
        if(player1->special[i]) { // Si le joueur ne l'a pas encore utilisé
            switch(i) {
                case TIR_LIGNE:
                    if(!player2->fleet->boats[3].isDown) { // Si le bateau concerné flotte toujours
                        printf("Tapez 0 pour effectuer un tir en ligne\n");
                        haveSpecials=1; // A encore un tir spécial à utiliser
                    }
                    break;
                case TIR_CROIX:
                    if(!player2->fleet->boats[1].isDown) {
                        printf("Tapez 1 pour effectuer un tir en croix\n");
                        haveSpecials=1;
                    }
                    break;
                case TIR_PLUS:
                    if(!player2->fleet->boats[1].isDown) {
                        printf("Tapez 2 pour effectuer un tir en plus\n");
                        haveSpecials=1;
                    }
                    break;
                case TIR_CARRE:
                    if(!player2->fleet->boats[0].isDown) {
                        printf("Tapez 3 pour effectuer un tir en carré\n");
                        haveSpecials=1;
                    }
                    break;
                default :
                    break;
            }
        }
    }
    if(!haveSpecials) return TIR_NORMAL; // N'a plus de tir spécial en réserve
    printf("Autre pour effectuer un tir normal\n");
    int tir=TIR_NORMAL;
    printf("Choix du tir : ");
    scanf("%d",&tir); // Choix du tir selon le numéro
    return selectionTir(player1,player2,tir);
}

void freeCoordsBoats(Player *players) { // On libère l'espace prit par les coordonnées assignées à chaque bateau de chaque joueur
    for(int i=0; i<2; i++) {
        for(int j=0; j<NUMBER_OF_BOATS; j++) {
            free(players[i].fleet->boats[j].coord);
        }
    }
}

void freeFleets(Player *players) { // On libère l'espace prit par les flottes de navires des joueurs
    for(int i=0; i<2; i++) {
        free(players[i].fleet);
    }
}