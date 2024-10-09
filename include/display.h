#ifndef DISPLAY_H
#define DISPLAY_H

#include "pokemon.h"

void list_all_pokemon(Pokemon *pokemon_list, int count);
void display_pokemon(Pokemon *pokemon_list, int count, const char *name, const char *form, int shiny, int info);
void display_random_pokemon(Pokemon *pokemon_list, int count, int shiny, int no_title, int no_mega, int no_gmax, int no_regional, int info, int gen_min, int gen_max, int *gen_list, int gen_count);

#endif
