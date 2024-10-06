#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pokemon.h"
#include "display.h"

void list_all_pokemon(Pokemon *pokemon_list, int count) {
    for (int i = 0; i < count; i++) {
        if (pokemon_list[i].name) {
            printf("%s\n", pokemon_list[i].name);
        } else {
            printf("The name of the Pokémon was not found.\n");
        }
    }
}

void display_pokemon(Pokemon *pokemon_list, int count, const char *name, const char *form, int shiny) {
    for (int i = 0; i < count; i++) {
        if (pokemon_list[i].name && strcmp(pokemon_list[i].name, name) == 0) {
            char art_path[256];
            if (strcmp(form, "regular") == 0) {
                snprintf(art_path, sizeof(art_path), "../assets/colorscripts/%s/%s", shiny ? "shiny" : "regular", pokemon_list[i].slug);
            } else {
                snprintf(art_path, sizeof(art_path), "../assets/colorscripts/%s/%s-%s", shiny ? "shiny" : "regular", pokemon_list[i].slug, form);
            }

            FILE *art_file = fopen(art_path, "r");
            if (!art_file) {
                printf("Unable to read art file for Pokémon '%s'. Path: %s\n", name, art_path);
                return;
            }

            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), art_file) != NULL) {
                printf("%s", buffer);
            }

            fclose(art_file);
            return;
        }
    }
    printf("Pokémon %s not found.\n", name);
}

void display_random_pokemon(Pokemon *pokemon_list, int count, int shiny, int no_title, int no_mega, int no_gmax, int no_regional) {
    int index;
    const char *form;

    do {
        index = rand() % count;
        form = pokemon_list[index].form_count > 0 && pokemon_list[index].forms[0] ? pokemon_list[index].forms[0] : "regular";
    } while ((no_mega && (strstr(form, "mega") != NULL || strstr(form, "mega-X") != NULL || strstr(form, "mega-Y") != NULL)) || 
             (no_gmax && strstr(form, "gmax") != NULL) || 
             (no_regional && (strstr(form, "alola") != NULL || strstr(form, "galar") != NULL || strstr(form, "hisui") != NULL || strstr(form, "paldea") != NULL)););

    const char *name = pokemon_list[index].name;

    if (!no_title) {
        printf("%s\n", name);
    }
    
    display_pokemon(pokemon_list, count, name, form, shiny);
}
