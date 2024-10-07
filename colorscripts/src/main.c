#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "pokemon.h"
#include "display.h"
#include "args.h"
#include "config.h"

#define CONFIG_FILE_PATH "config.json"

#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"

void print_usage(Config *config) {
    printf(COLOR_GREEN "pokemonc" COLOR_RESET " %s\n", config->version);
    printf(COLOR_BLUE "Author: %s <%s>\n" COLOR_RESET, config->author, config->email);
    printf("%s\n\n", config->description);

    printf(COLOR_YELLOW "USAGE:\n" COLOR_RESET);
    printf("    pokemonc <SUBCOMMAND>\n\n");
    
    printf(COLOR_YELLOW "OPTIONS:\n" COLOR_RESET);
    printf(COLOR_GREEN "    -h, --help" COLOR_RESET "       Print help information\n");
    printf(COLOR_GREEN "    -v, --version" COLOR_RESET "    Print version information\n\n");
    
    printf(COLOR_YELLOW "SUBCOMMANDS:\n" COLOR_RESET);
    printf(COLOR_GREEN "    help" COLOR_RESET "             Print this message or the help of the given subcommand(s)\n");
    printf(COLOR_GREEN "    list" COLOR_RESET "             Print list of all pokemon\n");
    printf(COLOR_GREEN "    name" COLOR_RESET "             Select pokemon by name. Generally spelled like in the games.\n");
    printf(COLOR_GREEN "    random" COLOR_RESET "           Show a random pokemon.\n\n");
}

int main(int argc, char **argv) {
    Config *config = load_config(CONFIG_FILE_PATH);
    if (!config) {
        printf("The configuration file could not be loaded. Exited.\n");
        return 1;
    }

    struct arguments arguments = {NULL, "regular", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, 0, config->version};
    if (argc == 1) {
        print_usage(config);
        free_config(config);
        return 0;
    }

    parse_arguments(argc, argv, &arguments);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec * 1000000 + tv.tv_usec);

    int pokemon_count;
    Pokemon *pokemon_list = load_pokemon_data(JSON_FILE_PATH, &pokemon_count, config->language);
    if (!pokemon_list) {
        free_config(config);
        return 1;
    }

    if (arguments.list) {
        list_all_pokemon(pokemon_list, pokemon_count);
    } else if (arguments.random) {
        double shiny_random_value = (rand() % 10000) / 100.0;
        int shiny = shiny_random_value < config->shiny_probability;
        display_random_pokemon(pokemon_list, pokemon_count, shiny, arguments.no_title, arguments.no_mega, arguments.no_gmax, arguments.no_regional, arguments.info, arguments.gen_min, arguments.gen_max, arguments.gen_list, arguments.gen_count);
    } else if (arguments.pokemon_name) {
        display_pokemon(pokemon_list, pokemon_count, arguments.pokemon_name, arguments.form, arguments.shiny);
    } else {
        printf("No command specified. Use --help to see more information.\n");
    }

    free_pokemon_data(pokemon_list, pokemon_count);
    free_config(config);
    return 0;
}
