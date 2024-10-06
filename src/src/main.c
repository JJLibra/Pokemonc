#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "pokemon.h"
#include "display.h"
#include "args.h"
#include "config.h"

#define CONFIG_FILE_PATH "../config.json"

#define COLOR_YELLOW "\033[33m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RESET "\033[0m"

void print_usage(Config *config) {
    printf(COLOR_GREEN "pokemonc" COLOR_RESET " %s\n", config->version);
    printf("%s <%s>\n", config->author, config->email);
    printf("%s\n\n", config->description);

    printf(COLOR_YELLOW "USAGE:\n" COLOR_RESET);
    printf("    pokemonc <SUBCOMMAND>\n\n");
    
    printf(COLOR_YELLOW "OPTIONS:\n" COLOR_RESET);
    printf(COLOR_GREEN "    -h, --help" COLOR_RESET "       Print help information\n");
    printf(COLOR_GREEN "    -V, --version" COLOR_RESET "    Print version information\n\n");
    
    printf(COLOR_YELLOW "SUBCOMMANDS:\n" COLOR_RESET);
    printf(COLOR_GREEN "    help" COLOR_RESET "      Print this message or the help of the given subcommand(s)\n");
    printf(COLOR_GREEN "    list" COLOR_RESET "      Print list of all pokemon\n");
    printf(COLOR_GREEN "    name" COLOR_RESET "      Select pokemon by name. Generally spelled like in the games.\n");
    printf(COLOR_GREEN "    random" COLOR_RESET "    Show a random pokemon.\n\n");
}

int main(int argc, char **argv) {
    // 加载配置文件
    Config *config = load_config(CONFIG_FILE_PATH);
    if (!config) {
        printf("无法加载配置文件。程序退出。\n");
        return 1;
    }

    struct arguments arguments = {NULL, "regular", 0, 0, 0};
    if (argc == 1) {
        // No arguments provided, print the usage information
        print_usage(config);
        free_config(config);
        return 0;
    }

    parse_arguments(argc, argv, &arguments);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec * 1000000 + tv.tv_usec);

    int pokemon_count;
    Pokemon *pokemon_list = load_pokemon_data(JSON_FILE_PATH, &pokemon_count);
    if (!pokemon_list) {
        free_config(config);
        return 1;
    }

    if (arguments.list) {
        list_all_pokemon(pokemon_list, pokemon_count);
    } else if (arguments.random) {
        display_random_pokemon(pokemon_list, pokemon_count, arguments.shiny);
    } else if (arguments.pokemon_name) {
        display_pokemon(pokemon_list, pokemon_count, arguments.pokemon_name, arguments.form, arguments.shiny);
    } else {
        printf("未指定命令。使用 --help 查看更多信息。\n");
    }

    free_pokemon_data(pokemon_list, pokemon_count);
    free_config(config);
    return 0;
}