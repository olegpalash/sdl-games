CFLAGS	= -O2 
OBJ		= "../../bin/game-xsos"

all: $(OBJ)

$(OBJ): main.o interface.o
	gcc $(CFLAGS) main.o interface.o `pkg-config --cflags --libs sdl2` -o $(OBJ)

main.o: main.c
	gcc -c $(CFLAGS) main.c -o main.o

interface.o:
	gcc	-c $(CFLAGS) interface.c -o interface.o
