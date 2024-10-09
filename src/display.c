#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pokemon.h"
#include "display.h"

#define COLORSCRIPTS_PATH "/usr/local/share/pokemonc/assets/colorscripts/"

char *get_colorscripts_path(const char *slug, const char *form, int shiny) {
    // Calculate the required buffer size
    size_t path_size = strlen(COLORSCRIPTS_PATH) + strlen(shiny ? "shiny" : "regular") + strlen(slug) + strlen(form) + 10;
    char *art_path = malloc(path_size);
    if (!art_path) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    // Construct the path
    if (strcmp(form, "regular") == 0) {
        snprintf(art_path, path_size, "%s%s/%s", COLORSCRIPTS_PATH, shiny ? "shiny" : "regular", slug);
    } else {
        snprintf(art_path, path_size, "%s%s/%s-%s", COLORSCRIPTS_PATH, shiny ? "shiny" : "regular", slug, form);
    }

    if (access(art_path, F_OK) != 0) {
        // Try default form
        snprintf(art_path, path_size, "%s%s/%s", COLORSCRIPTS_PATH, shiny ? "shiny" : "regular", slug);
        if (access(art_path, F_OK) != 0) {
            // Art file not found
            free(art_path);
            return NULL;
        }
    }

    return art_path;
}

void list_all_pokemon(Pokemon *pokemon_list, int count) {
    for (int i = 0; i < count; i++) {
        if (pokemon_list[i].slug) {
            printf("%s\n", pokemon_list[i].slug);
        } else {
            printf("The name of the Pokémon was not found.\n");
        }
    }
}

void display_pokemon(Pokemon *pokemon_list, int count, const char *name, const char *form, int shiny, int info) {
    for (int i = 0; i < count; i++) {
        if (pokemon_list[i].slug && strcmp(pokemon_list[i].slug, name) == 0) {
            char *art_path = get_colorscripts_path(pokemon_list[i].slug, form, shiny);
            if (!art_path) {
                fprintf(stderr, "Unable to read art file for Pokémon '%s'.\n", name);
                return;
            }

            FILE *art_file = fopen(art_path, "r");
            free(art_path);
            if (!art_file) {
                fprintf(stderr, "Unable to open art file for Pokémon '%s'.\n", name);
                return;
            }

            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), art_file) != NULL) {
                printf("%s", buffer);
            }

            fclose(art_file);

            // Print description if info flag is set
            if (info && pokemon_list[i].desc) {
                printf("\n%s\n", pokemon_list[i].desc);
            }

            return;
        }
    }
    fprintf(stderr, "Pokémon %s not found.\n", name);
}

void display_random_pokemon(Pokemon *pokemon_list, int count, int shiny, int no_title, int no_mega, int no_gmax, int no_regional, int info, int gen_min, int gen_max, int *gen_list, int gen_count) {
    Pokemon **filtered_pokemon = malloc(count * sizeof(Pokemon *));
    if (!filtered_pokemon) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    int filtered_count = 0;
    for (int i = 0; i < count; i++) {
        Pokemon *p = &pokemon_list[i];
        const char *form = p->form_count > 0 && p->forms[0] ? p->forms[0] : "regular";
        int pokemon_gen = p->gen;

        int in_gen_list = 0;

        // Check if the Pokémon's generation is in the specified gen_list
        if (gen_count > 0) {
            for (int j = 0; j < gen_count; j++) {
                if (pokemon_gen == gen_list[j]) {
                    in_gen_list = 1;
                    break;
                }
            }
        } else {
            // If no gen_list is provided, use gen_min and gen_max
            in_gen_list = (pokemon_gen >= gen_min && pokemon_gen <= gen_max);
        }

        // Ensure the Pokémon meets all filter criteria
        if (in_gen_list &&
            !(no_mega && (strstr(form, "mega") != NULL || strstr(form, "mega-X") != NULL || strstr(form, "mega-Y") != NULL)) &&
            !(no_gmax && strstr(form, "gmax") != NULL) &&
            !(no_regional && (strstr(form, "alola") != NULL || strstr(form, "galar") != NULL || strstr(form, "hisui") != NULL || strstr(form, "paldea") != NULL))) {
            filtered_pokemon[filtered_count++] = p;
        }
    }

    if (filtered_count == 0) {
        fprintf(stderr, "No Pokémon found matching the specified criteria.\n");
        free(filtered_pokemon);
        return;
    }

    int index = rand() % filtered_count;
    Pokemon *selected_pokemon = filtered_pokemon[index];
    const char *form = selected_pokemon->form_count > 0 && selected_pokemon->forms[0] ? selected_pokemon->forms[0] : "regular";
    const char *name = selected_pokemon->slug;

    if (!no_title) {
        printf("%s (%s)\n", name, form);
    }

    display_pokemon(pokemon_list, count, name, form, shiny, info);

    if (info && selected_pokemon->desc) {
        printf("\n%s\n", selected_pokemon->desc);
    }

    free(filtered_pokemon);
}
