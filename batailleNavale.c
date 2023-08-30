#include "batailleNavale.h"

/*
    Delacroix Grégoire
    Rose Matthieu
    L3
*/

Player playerOne;
Player playerTwo;
Player players[2];

void initializeGame() { // Initialisation de la partie
    initializePlayers(players, &playerOne, &playerTwo); // Des deux joueurs
    for(int i=0; i<2; i++) {
        initializeMatrix(&players[i]); // De la matrice de chacun
        initializeFleet(&players[i]); // Des flottes de chacun
        placeFleet(&players[i]); // Placement des navires pour chacun
    }
}

void showMaj() { // Affichage de la partie avec modifications prises en compte
    for(int i=0; i<2; i++) printMatrix(&players[i]);
}

void myTurn(Player *players) { // Tour du joueur 1
    printf("\nA votre tour :\n");
    int tir=choseTir(&players[0], &players[1]); // Choix du tir normal ou spécial
    Coordonnees *c=malloc(sizeof(Coordonnees));
    choseCoord(c); // Choix des coordonnées où faire feu
    Coordonnees **cibles=getCoordFire(c,tir); // Coordonnées des différentes cases cibles selon la consigne
    if(multipleFires(cibles,&players[1])>=1) (&players[0])->hasHit=1; // Réalisation des différents tirs
    else (&players[0])->hasHit=0;
    freeTabOfCibles(cibles); // Libération de l'espace alloué pour les coordonnées
    free(c);
}

void AITurn(Player *players) { // Tour de l'IA
    printf("\nAu tour de l'IA :\n");
    automate(&players[1], &players[0]);
}

void newGame() { // Nouvelle partie
    initializeGame(); // Initialisation
    while(players[0].fleet->numberOfBoats != 0 && players[1].fleet->numberOfBoats != 0) { // Tant qu'une des deux flottes n'a pas entièrement coulée
        myTurn(players); // Chacun joue à son tour
        AITurn(players);
        showMaj(); // Affichage des modifications
    }
    freePotientialShot(&players[1]); // Libération de l'espace occupé par les coordonnées si l'IA n'a pas fini de détruire un bateau prit pour cible
    if(players[1].fleet->numberOfBoats == 0 && players[0].fleet->numberOfBoats == 0) printf("WOW ! Vous êtes à égalité !\n"); // Cas où il y a égalité
    else if(players[1].fleet->numberOfBoats == 0) printf("Vous avez gagné, félicitations !\n"); // Cas où l'humain a gagné
    else printf("Vous avez perdu, dommage...\n"); // Cas où l'IA a gagné
    freeCoordsBoats(players); // Libération des navires et des flottes des deux joueurs
    freeFleets(players);
}

void main() {
    int wantToPlay=1; // Tant que l'utilisateur veut jouer
    while(wantToPlay) {
        wantToPlay=0;
        printf("Nouvelle partie !\n");
        newGame(); // Nouvelle partie
        printf("\nVoulez-vous rejouer ? (Oui ou Non)\n");
        char c[3];
        scanf("%s",c); // Oui ou Non
        if(strcmp("Oui",c)==0 || strcmp("oui",c)==0) wantToPlay=1;
        printf("\n");
    }
}