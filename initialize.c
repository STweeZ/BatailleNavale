#include "initialize.h"

void initializeMatrix(Player *player) { // Initialisation de la matrice de player à vide
    for(int i=0; i<SIZE_MATRIX; i++) {
        for(int j=0; j<SIZE_MATRIX; j++) {
            player->matrix[i][j]='.';
        }
    }
}

void initializePlayers(Player *players, Player *playerOne, Player *playerTwo) { // Initialisation des deux joueurs
    players[0]=*playerOne; // Joueur humain
    players[1]=*playerTwo; // Joueur IA
    char *player1="Vous";
    char *player2="IA";
    players[0].name=player1;
    players[1].name=player2;
    for(int i=0; i<2; i++) { // Les deux n'ont encore rien touché
        players[i].hasHit=0;
        players[i].hasUsedSpecial=0; // Les joueurs n'ont pas encore utilisé de tir spécial
        for(int j=0; j<NB_TIRS; j++) { // Initialisation des tirs spéciaux à 1 -> Pas encore utilisés
            players[i].special[j]=1;
        }
    }
    players[1].playerType.ia.lastCoord=NULL; // L'IA n'a pas encore touché un bateau
    players[1].playerType.ia.vertical=-1; // L'IA ne sait donc pas si le bateau est à la verticale ou à l'horizontal
    players[1].playerType.ia.hitBoat=0; // L'IA ne s'occupe pas encore d'un bateau
}

void initializeFleet(Player *player) { // Initialisation de la flotte du joueur player
    Boat porte_avion={0,PORTE_AVION,malloc(sizeof(Coordonnees)),0,0}; // Création des bateaux nécessaires
    Boat croiseur={1,CROISEUR,malloc(sizeof(Coordonnees)),0,0};
    Boat destroyer={2,DESTROYER,malloc(sizeof(Coordonnees)),0,0};
    Boat sous_marin={3,SOUS_MARIN,malloc(sizeof(Coordonnees)),0,0};
    Boat torpilleur={4,TORPILLEUR,malloc(sizeof(Coordonnees)),0,0};

    Fleet *fleet=malloc(sizeof(Fleet)); // Création de la flotte et ajout des bateaux à celle-ci
    fleet->boats[0]=porte_avion;
    fleet->boats[1]=croiseur;
    fleet->boats[2]=destroyer;
    fleet->boats[3]=sous_marin;
    fleet->boats[4]=torpilleur;
    fleet->numberOfBoats=NUMBER_OF_BOATS; // Nombre de bateaux dans la flotte
    player->fleet=fleet;
}

void placeFleet(Player *player) { // Placement des bateaux de la flotte sur le terrain de player
    int isVertical;
    int xCoord;
    int yCoord;
    int canBePlaced; // Si le bateau peut être placé
    for(int i=0; i<NUMBER_OF_BOATS; i++) { // Pour chaque bateau
        isVertical=rand()%2; // Si il est vertical ou horizontal
        canBePlaced=0;
        while(!canBePlaced) {
            canBePlaced=1;
            xCoord=rand()%SIZE_MATRIX; // Coordonnées en x random
            yCoord=rand()%SIZE_MATRIX; // Coordonnées en y random
            if((isVertical && (yCoord + player->fleet->boats[i].size >= SIZE_MATRIX)) || (!isVertical && (xCoord + player->fleet->boats[i].size >= SIZE_MATRIX))) { // Si le bateau dépasse la taille de la matrice
                canBePlaced=0;
            } else {
                for(int j=0; j<player->fleet->boats[i].size; j++) { // Vérification si le bateau ne se met pas à la place d'un autre et s'il n'est pas collé à un autre
                    if(isVertical) {
                        if(player->matrix[xCoord][yCoord+j] != '.') { // Si ne rentre pas dans un autre bateau
                            canBePlaced=0;
                        } else if(xCoord>0 && player->matrix[xCoord-1][yCoord+j] != '.') { // Si le bateau n'est pas collé à un autre par les côtés
                            canBePlaced=0;
                        } else if(xCoord<SIZE_MATRIX-1 && player->matrix[xCoord+1][yCoord+j] != '.') {
                            canBePlaced=0;
                        }
                    } else if(!isVertical) {
                        if(player->matrix[xCoord+j][yCoord] != '.') {
                            canBePlaced=0;
                        } else if(yCoord>0 && player->matrix[xCoord+j][yCoord-1] != '.') {
                            canBePlaced=0;
                        } else if(yCoord<SIZE_MATRIX-1 && player->matrix[xCoord+j][yCoord+1] != '.') {
                            canBePlaced=0;
                        }
                    }
                }
                if(isVertical && (yCoord>0 && player->matrix[xCoord][yCoord-1] != '.') || (yCoord+player->fleet->boats[i].size<SIZE_MATRIX && player->matrix[xCoord][yCoord+player->fleet->boats[i].size] != '.')) canBePlaced=0; // Vérification supplémentaire afin de voir si le bateau n'est pas collé à un autre par le devant ou l'arrière
                else if(!isVertical && (xCoord>0 && player->matrix[xCoord-1][yCoord] != '.') || (xCoord+player->fleet->boats[i].size<SIZE_MATRIX && player->matrix[xCoord+player->fleet->boats[i].size][yCoord] != '.')) canBePlaced=0;
                if(isVertical) { // Dernière vérification afin de vérifier que les coins du bateau ne sont pas collés aux coins d'un autre bateau
                    if(xCoord>0) {
                        if(yCoord>0 && player->matrix[xCoord-1][yCoord-1] != '.') canBePlaced=0;
                        if(yCoord+player->fleet->boats[i].size<SIZE_MATRIX && player->matrix[xCoord-1][yCoord+player->fleet->boats[i].size] != '.') canBePlaced=0;
                    }
                    if(xCoord<SIZE_MATRIX-1) {
                        if(yCoord>0 && player->matrix[xCoord+1][yCoord-1] != '.') canBePlaced=0;
                        if(yCoord+player->fleet->boats[i].size<SIZE_MATRIX && player->matrix[xCoord-1][yCoord+player->fleet->boats[i].size] != '.') canBePlaced=0;
                    }
                } else if(!isVertical) {
                    if(xCoord>0) {
                        if(yCoord>0 && player->matrix[xCoord-1][yCoord-1] != '.') canBePlaced=0;
                        if(yCoord<SIZE_MATRIX-1 && player->matrix[xCoord-1][yCoord+1] != '.') canBePlaced=0;
                    }
                    if(xCoord+player->fleet->boats[i].size<SIZE_MATRIX) {
                        if(yCoord>0 && player->matrix[xCoord+player->fleet->boats[i].size][yCoord-1] != '.') canBePlaced=0;
                        if(yCoord<SIZE_MATRIX && player->matrix[xCoord+player->fleet->boats[i].size][yCoord+1] != '.') canBePlaced=0;
                    }
                }
            }
        }
        for(int k=0; k<player->fleet->boats[i].size; k++) { // Affichage du bateau sur la matrice du joueur
            if(isVertical) {
                player->matrix[xCoord][yCoord+k]='0';
            } else {
                player->matrix[xCoord+k][yCoord]='0';
            }
        }
        player->fleet->boats[i].isVertical=isVertical;
        player->fleet->boats[i].coord->x=xCoord;
        player->fleet->boats[i].coord->y=yCoord;
    }
}