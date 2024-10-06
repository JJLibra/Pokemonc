#ifndef POKEMON_H
#define POKEMON_H

#define MAX_FORMS 10

typedef struct {
    char *name;
    char *slug;
    char *forms[MAX_FORMS];
    int form_count;
} Pokemon;

Pokemon *load_pokemon_data(const char *file_path, int *count);
void free_pokemon_data(Pokemon *pokemon_list, int count);

#endif
