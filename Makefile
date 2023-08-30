# Variables 

# But principal

batailleNavale : coords.o fleets.o players.o initialize.o fire.o automate.o batailleNavale.o coords.h fleets.h players.h initialize.h fire.h automate.h batailleNavale.h batailleNavale.c
	gcc coords.o fleets.o players.o initialize.o fire.o automate.o batailleNavale.o -o batailleNavale

coords.o : coords.c coords.h
	gcc -c coords.c

fleets.o : fleets.c fleets.h
	gcc -c fleets.c

players.o : players.c players.h
	gcc -c players.c

initialize.o : initialize.c initialize.h
	gcc -c initialize.c

fire.o : fire.c fire.h
	gcc -c fire.c

automate.o : automate.c automate.h
	gcc -c automate.c

batailleNavale.o : batailleNavale.c batailleNavale.h
	gcc -c batailleNavale.c

# Le menage

clean :
	rm -f *.o *~