CFLAGS	= -O2 
OBJ		= "../../bin/game-xsos"

all: $(OBJ)

$(OBJ): main.o interface.o ai.o game.o
	gcc $(CFLAGS) main.o interface.o ai.o game.o `pkg-config --cflags --libs sdl2` -o $(OBJ)

main.o: main.c
	gcc -c $(CFLAGS) main.c -o main.o

interface.o:
	gcc	-c $(CFLAGS) interface.c -o interface.o

ai.o:
	gcc	-c $(CFLAGS) ai.c -o ai.o

game.o:
	gcc	-c $(CFLAGS) game.c -o game.o
