#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include "pokemon.h"
#include "display.h"
#include "args.h"
#include "config.h"

#define JSON_FILE_PATH "/usr/local/share/pokemonc/assets/pokemon.json"

#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"

#define VERSION "0.1.1"
#define AUTHOR "JJLibra"
#define EMAIL "2565474095@qq.com"
#define DESCRIPTION "Print Pokémon sprites in your terminal"

char *get_config_file_path() {
    const char *config_home = getenv("XDG_CONFIG_HOME");
    char *config_file_path;

    if (!config_home || strlen(config_home) == 0) {
        const char *home_dir = getenv("HOME");
        if (!home_dir) {
            fprintf(stderr, "Unable to get user home directory path.\n");
            return NULL;
        }
        size_t path_len = strlen(home_dir) + strlen("/.config/pokemonc/config.json") + 1;
        config_file_path = malloc(path_len);
        if (!config_file_path) {
            fprintf(stderr, "Memory allocation failed.\n");
            return NULL;
        }
        snprintf(config_file_path, path_len, "%s/.config/pokemonc/config.json", home_dir);
    } else {
        size_t path_len = strlen(config_home) + strlen("/pokemonc/config.json") + 1;
        config_file_path = malloc(path_len);
        if (!config_file_path) {
            fprintf(stderr, "Memory allocation failed.\n");
            return NULL;
        }
        snprintf(config_file_path, path_len, "%s/pokemonc/config.json", config_home);
    }

    return config_file_path;
}

char *get_json_file_path() {
    size_t path_len = strlen(JSON_FILE_PATH) + 1;
    char *json_file_path = malloc(path_len);
    if (!json_file_path) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }
    snprintf(json_file_path, path_len, "%s", JSON_FILE_PATH);

    if (access(json_file_path, F_OK) != 0) {
        fprintf(stderr, "JSON file not found: %s\n", json_file_path);
        free(json_file_path);
        return NULL;
    }

    return json_file_path;
}

void print_usage() {
    printf(COLOR_GREEN "pokemonc" COLOR_RESET " %s\n", VERSION);
    printf(COLOR_BLUE "Author: %s <%s>\n" COLOR_RESET, AUTHOR, EMAIL);
    printf("%s\n\n", DESCRIPTION);

    printf(COLOR_YELLOW "USAGE:\n" COLOR_RESET);
    printf("    pokemonc <SUBCOMMAND>\n\n");

    printf(COLOR_YELLOW "OPTIONS:\n" COLOR_RESET);
    printf(COLOR_GREEN "    -h, --help" COLOR_RESET "       Print help information\n");
    printf(COLOR_GREEN "    -v, --version" COLOR_RESET "    Print version information\n\n");

    printf(COLOR_YELLOW "SUBCOMMANDS:\n" COLOR_RESET);
    printf(COLOR_GREEN "    help" COLOR_RESET "             Print this message or the help of the given subcommand(s)\n");
    printf(COLOR_GREEN "    list" COLOR_RESET "             Print list of all Pokémon\n");
    printf(COLOR_GREEN "    name" COLOR_RESET "             Select Pokémon by name. Generally spelled like in the games.\n");
    printf(COLOR_GREEN "    random" COLOR_RESET "           Show a random Pokémon.\n\n");
}

int main(int argc, char **argv) {
    char *config_file_path = get_config_file_path();
    if (!config_file_path) {
        fprintf(stderr, "Unable to determine configuration file path. Exited.\n");
        return EXIT_FAILURE;
    }

    Config *config = load_config(config_file_path);
    if (!config) {
        // If the configuration file does not exist, create a default configuration file
        config = create_default_config(config_file_path);
        if (!config) {
            fprintf(stderr, "Failed to create default configuration file.\n");
            free(config_file_path);
            return EXIT_FAILURE;
        }
        printf("Default configuration file created at %s\n", config_file_path);
    }
    free(config_file_path);

    char *json_file_path = get_json_file_path();
    if (!json_file_path) {
        free_config(config);
        return EXIT_FAILURE;
    }

    struct arguments arguments = {0};
    arguments.version = VERSION;
    arguments.form = "regular";

    if (argc == 1) {
        print_usage();
        free_config(config);
        free(json_file_path);
        return EXIT_SUCCESS;
    }

    if (parse_arguments(argc, argv, &arguments) != 0) {
        free_config(config);
        free(json_file_path);
        return EXIT_FAILURE;
    }

    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand((unsigned int)(tv.tv_sec * 1000000 + tv.tv_usec));

    int pokemon_count;
    Pokemon *pokemon_list = load_pokemon_data(json_file_path, &pokemon_count, config->language);
    free(json_file_path);
    if (!pokemon_list) {
        free_config(config);
        return EXIT_FAILURE;
    }

    if (arguments.list) {
        list_all_pokemon(pokemon_list, pokemon_count);
    } else if (arguments.random) {
        double shiny_random_value = (rand() % 10000) / 100.0;
        int shiny = shiny_random_value < config->shiny_probability;
        display_random_pokemon(pokemon_list, pokemon_count, shiny, arguments.no_title, arguments.no_mega, arguments.no_gmax, arguments.no_regional, arguments.info, arguments.gen_min, arguments.gen_max, arguments.gen_list, arguments.gen_count);
    } else if (arguments.pokemon_name) {
        display_pokemon(pokemon_list, pokemon_count, arguments.pokemon_name, arguments.form, arguments.shiny, arguments.info);
    } else {
        fprintf(stderr, "No command specified. Use --help to see more information.\n");
    }

    free_pokemon_data(pokemon_list, pokemon_count);
    free_config(config);
    return EXIT_SUCCESS;
}
