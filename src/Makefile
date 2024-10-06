CC = gcc
CFLAGS = -Wall -Iinclude
OBJ = src/main.o src/args.o src/display.o src/pokemon.o libs/cJSON.o
TARGET = pokemon_display

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

src/main.o: src/main.c include/args.h include/display.h include/pokemon.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/args.o: src/args.c include/args.h
	$(CC) $(CFLAGS) -c src/args.c -o src/args.o

src/display.o: src/display.c include/display.h include/pokemon.h
	$(CC) $(CFLAGS) -c src/display.c -o src/display.o

src/pokemon.o: src/pokemon.c include/pokemon.h include/cJSON.h
	$(CC) $(CFLAGS) -c src/pokemon.c -o src/pokemon.o

libs/cJSON.o: libs/cJSON.c include/cJSON.h
	$(CC) $(CFLAGS) -c libs/cJSON.c -o libs/cJSON.o

clean:
	rm -f $(OBJ) $(TARGET)
