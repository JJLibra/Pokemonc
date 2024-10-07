CC = gcc
CFLAGS = -Wall -Iinclude
BINDIR = binary
OBJ = $(BINDIR)/main.o $(BINDIR)/args.o $(BINDIR)/display.o $(BINDIR)/pokemon.o $(BINDIR)/config.o $(BINDIR)/cJSON.o
TARGET = pokemonc

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

$(BINDIR)/main.o: src/main.c include/args.h include/display.h include/pokemon.h include/config.h
	$(CC) $(CFLAGS) -c src/main.c -o $(BINDIR)/main.o

$(BINDIR)/args.o: src/args.c include/args.h
	$(CC) $(CFLAGS) -c src/args.c -o $(BINDIR)/args.o

$(BINDIR)/display.o: src/display.c include/display.h include/pokemon.h
	$(CC) $(CFLAGS) -c src/display.c -o $(BINDIR)/display.o

$(BINDIR)/pokemon.o: src/pokemon.c include/pokemon.h include/cJSON.h
	$(CC) $(CFLAGS) -c src/pokemon.c -o $(BINDIR)/pokemon.o

$(BINDIR)/config.o: src/config.c include/config.h include/cJSON.h
	$(CC) $(CFLAGS) -c src/config.c -o $(BINDIR)/config.o

$(BINDIR)/cJSON.o: libs/cJSON.c include/cJSON.h
	$(CC) $(CFLAGS) -c libs/cJSON.c -o $(BINDIR)/cJSON.o

clean:
	rm -f $(BINDIR)/*.o $(TARGET)
