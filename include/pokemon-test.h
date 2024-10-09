#ifndef POKEMON_H
#define POKEMON_H

#define MAX_FORMS 10

typedef struct {
    char *name;
    char *slug;
    char *desc;
    char *forms[MAX_FORMS];
    int form_count;
    int gen;
} Pokemon;

Pokemon *load_pokemon_data_from_memory(const char *json_data, size_t data_len, int *count, const char *language);
void free_pokemon_data(Pokemon *pokemon_list, int count);

#endif
