CFLAGS	= -O2 
OBJ		= "../../bin/game-lines"

all: $(OBJ)

$(OBJ): main.o interface.o game.o func.o
	gcc $(CFLAGS) main.o interface.o game.o func.o `pkg-config --cflags --libs sdl2 SDL2_ttf SDL2_image` -o $(OBJ)

main.o: main.c
	gcc -c $(CFLAGS) main.c -o main.o

interface.o: interface.c
	gcc	-c $(CFLAGS) interface.c -o interface.o

game.o: game.c
	gcc	-c $(CFLAGS) game.c -o game.o
	
func.o: func.c
	gcc	-c $(CFLAGS) func.c -o func.o

clean:
	rm -f *.o
