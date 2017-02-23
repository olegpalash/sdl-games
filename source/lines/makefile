CFLAGS	= -O2 
OBJ		= "../../bin/game-lines"

all: $(OBJ)

$(OBJ): main.o interface.o game.o
	gcc $(CFLAGS) main.o interface.o game.o `pkg-config --cflags --libs sdl2 SDL2_ttf ` -o $(OBJ)

main.o: main.c
	gcc -c $(CFLAGS) main.c -o main.o

interface.o: interface.c
	gcc	-c $(CFLAGS) interface.c -o interface.o

game.o: game.c
	gcc	-c $(CFLAGS) game.c -o game.o

clean:
	rm -f *.o
