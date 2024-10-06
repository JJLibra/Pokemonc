#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <argp.h>
#include "cJSON.h"

#define MAX_FORMS 10
#define JSON_FILE_PATH "../assets/pokemon.json" // Adjusted path for your project structure

// Pokemon structure
typedef struct {
    char *name;
    char *forms[MAX_FORMS];
    int form_count;
} Pokemon;

// Load Pokemon data from JSON file
Pokemon *load_pokemon_data(const char *file_path, int *count) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Unable to open JSON file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_data = (char *)malloc(file_size + 1);
    fread(json_data, 1, file_size, file);
    fclose(file);
    json_data[file_size] = '\0';

    cJSON *json = cJSON_Parse(json_data);
    free(json_data);
    if (!json) {
        printf("Error parsing JSON file: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    int pokemon_count = cJSON_GetArraySize(json);
    Pokemon *pokemon_list = (Pokemon *)malloc(pokemon_count * sizeof(Pokemon));

    for (int i = 0; i < pokemon_count; i++) {

        cJSON *name_item = cJSON_GetObjectItem(pokemon_json, "name");
        if (!name_item) {
            printf("Error: Missing 'name' field in JSON.\n");
            continue;
        }
        pokemon_list[i].name = strdup(name_item->valuestring);

        cJSON *forms = cJSON_GetObjectItem(pokemon_json, "forms");
        int form_count = cJSON_GetArraySize(forms);
        pokemon_list[i].form_count = form_count;
        for (int j = 0; j < form_count; j++) {
            pokemon_list[i].forms[j] = strdup(cJSON_GetArrayItem(forms, j)->valuestring);
        }
    }

    cJSON_Delete(json);
    *count = pokemon_count;
    return pokemon_list;
}

// List all Pokemon names
void list_all_pokemon(Pokemon *pokemon_list, int count) {
    for (int i = 0; i < count; i++) {
        printf("%s\n", pokemon_list[i].name);
    }
}

// Display a specific Pokemon and its form
void display_pokemon(Pokemon *pokemon_list, int count, const char *name, const char *form) {
    for (int i = 0; i < count; i++) {
        if (strcmp(pokemon_list[i].name, name) == 0) {
            for (int j = 0; j < pokemon_list[i].form_count; j++) {
                if (strcmp(pokemon_list[i].forms[j], form) == 0) {
                    printf("Displaying %s (%s):\n", name, form);
                    // Insert code to display Unicode image here
                    return;
                }
            }
            printf("Form %s not found for %s.\n", form, name);
            return;
        }
    }
    printf("Pokemon %s not found.\n", name);
}

// Display a random Pokemon
void display_random_pokemon(Pokemon *pokemon_list, int count) {
    srand(time(NULL));
    int index = rand() % count;
    const char *name = pokemon_list[index].name;
    const char *form = pokemon_list[index].forms[0];
    printf("Displaying random Pokemon: %s (%s)\n", name, form);
    // Insert code to display Unicode image here
}

// Argument parsing
struct arguments {
    char *pokemon_name;
    char *form;
    int list;
    int random;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
        case 'l':
            arguments->list = 1;
            break;
        case 'r':
            arguments->random = 1;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num == 0) {
                arguments->pokemon_name = arg;
            } else if (state->arg_num == 1) {
                arguments->form = arg;
            } else {
                argp_usage(state);
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

const char *argp_program_version = "pokemon-display 1.0";
const char *argp_program_bug_address = "<example@example.com>";
static char doc[] = "A command-line tool to display Pokémon Unicode images.";
static char args_doc[] = "[pokemon_name] [form]";
static struct argp_option options[] = {
    {"list", 'l', 0, 0, "List all Pokémon"},
    {"random", 'r', 0, 0, "Display a random Pokémon"},
    {0}
};
static struct argp argp = {options, parse_opt, args_doc, doc};

// Main function
int main(int argc, char **argv) {
    struct arguments arguments = {NULL, "regular", 0, 0};
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    int pokemon_count;
    Pokemon *pokemon_list = load_pokemon_data(JSON_FILE_PATH, &pokemon_count);
    if (!pokemon_list) {
        return 1;
    }

    if (arguments.list) {
        list_all_pokemon(pokemon_list, pokemon_count);
    } else if (arguments.random) {
        display_random_pokemon(pokemon_list, pokemon_count);
    } else if (arguments.pokemon_name) {
        display_pokemon(pokemon_list, pokemon_count, arguments.pokemon_name, arguments.form);
    } else {
        printf("No command specified. Use --help for more information.\n");
    }

    // Free allocated memory
    for (int i = 0; i < pokemon_count; i++) {
        free(pokemon_list[i].name);
        for (int j = 0; j < pokemon_list[i].form_count; j++) {
            free(pokemon_list[i].forms[j]);
        }
    }
    free(pokemon_list);

    return 0;
}
