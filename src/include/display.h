#ifndef DISPLAY_H
#define DISPLAY_H

#include "pokemon.h"

void list_all_pokemon(Pokemon *pokemon_list, int count);
void display_pokemon(Pokemon *pokemon_list, int count, const char *name, const char *form, int shiny);
void display_random_pokemon(Pokemon *pokemon_list, int count, int shiny, int no_title, int no_mega, int no_gmax);

#endif
