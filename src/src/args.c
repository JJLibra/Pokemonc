#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"
#include "config.h"
#include <argp.h>

#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BOLD_PINK "\033[1;35m"
#define COLOR_RESET "\033[0m"

#define MAX_GEN 9
#define MIN_GEN 1

void print_random_help() {
    printf(COLOR_BOLD_PINK "pokemonc random\n" COLOR_RESET);
    printf(COLOR_YELLOW "USAGE:\n" COLOR_RESET);
    printf("    pokemonc random [OPTIONS] [GENERATIONS]\n\n");

    printf(COLOR_YELLOW "ARGS:\n" COLOR_RESET);
    printf(COLOR_GREEN "    <GENERATIONS>" COLOR_RESET "    Generation number, range (1-9), or list of generations (1,3,6) [default: 1-9]\n\n");

    printf(COLOR_YELLOW "OPTIONS:\n" COLOR_RESET);
    printf(COLOR_GREEN "    -h, --help" COLOR_RESET "           Print help information\n");
    printf(COLOR_GREEN "    -i, --info" COLOR_RESET "           Print pokedex entry (if it exists)\n");
    printf(COLOR_GREEN "        --no-gmax" COLOR_RESET "        Do not show gigantamax pokemon\n");
    printf(COLOR_GREEN "        --no-mega" COLOR_RESET "        Do not show mega pokemon\n");
    printf(COLOR_GREEN "        --no-regional" COLOR_RESET "    Do not show regional pokemon\n");
    printf(COLOR_GREEN "        --no-title" COLOR_RESET "       Do not display pokemon name\n");
}

void print_general_help() {
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

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
        case 'l':
            arguments->list = 1;
            break;
        case 'r':
            arguments->random = 1;
            break;
        case 'n':
            arguments->pokemon_name = arg;
            break;
        case 1001:
            arguments->no_title = 1;
            break;
        case 1002:
            arguments->no_mega = 1;
            break;
        case 1003:
            arguments->no_gmax = 1;
            break;
        case 1004:
            arguments->no_regional = 1;
            break;
        case 'i':
            arguments->info = 1;
            break;
        case 'v':
            printf("pokemonc %s\n", arguments->version);
            exit(0);
        case ARGP_KEY_ARG:
            if (strcmp(state->argv[1], "list") == 0) { // "list" subcommand
                arguments->list = 1;
            } else if (strcmp(state->argv[1], "name") == 0) { // "name" subcommand
                arguments->pokemon_name = arg;
            } else if (arguments->random) {
                if (strcmp(arg, "random") == 0) break; // Skip the "random" argument itself

                // Handle comma-separated list
                if (strchr(arg, ',')) {
                    char *token = strtok(arg, ",");
                    arguments->gen_count = 0;

                    while (token != NULL && arguments->gen_count < MAX_GEN_LIST) {
                        int gen = atoi(token);
                        if (gen < MIN_GEN || gen > MAX_GEN) {
                            printf(COLOR_YELLOW "Error: Invalid generation number %d. Please specify a valid numeric generation between %d and %d.\n" COLOR_RESET, gen, MIN_GEN, MAX_GEN);
                            exit(1);
                        }
                        arguments->gen_list[arguments->gen_count++] = gen;
                        token = strtok(NULL, ",");
                    }
                } else {
                    // Handle single generation or range
                    char *dash = strchr(arg, '-');
                    if (dash) {
                        *dash = '\0';
                        arguments->gen_min = atoi(arg);
                        arguments->gen_max = atoi(dash + 1);
                    } else {
                        arguments->gen_min = atoi(arg);
                        arguments->gen_max = arguments->gen_min;
                    }

                    // Validate the generation range
                    if (arguments->gen_min < MIN_GEN || arguments->gen_max > MAX_GEN || arguments->gen_min > arguments->gen_max) {
                        printf(COLOR_YELLOW "Error: Invalid generation range %d-%d. Please specify a valid range between %d and %d.\n" COLOR_RESET, arguments->gen_min, arguments->gen_max, MIN_GEN, MAX_GEN);
                        exit(1);
                    }
                }
            } else if (state->arg_num == 0) {
                arguments->pokemon_name = arg;
            } else if (state->arg_num == 1) {
                arguments->form = arg;
            } else {
                argp_usage(state);
            }
            break;
        case ARGP_KEY_END:
            if (arguments->random && arguments->gen_min == 0 && arguments->gen_max == 0 && arguments->gen_count == 0) {
                // Default to all generations if no specific input is given
                arguments->gen_min = MIN_GEN;
                arguments->gen_max = MAX_GEN;
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

void parse_arguments(int argc, char **argv, struct arguments *arguments) {
    arguments->gen_min = 0;
    arguments->gen_max = 0;

    // Handle `random --help` case
    if (argc > 1 && strcmp(argv[1], "random") == 0) {
        arguments->random = 1;
        if (argc == 3 && (strcmp(argv[2], "--help") == 0 || strcmp(argv[2], "-h") == 0)) {
            print_random_help();
            exit(0);
        }
    }

    // Handle general `--help`
    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        print_general_help();
        exit(0);
    }

    static struct argp_option options[] = {
        {"list", 'l', 0, 0, "List all Pokémon"},
        {"random", 'r', 0, 0, "Show random Pokémon"},
        {"name", 'n', "NAME", 0, "Select Pokémon by name"},
        {"shiny", 's', 0, 0, "Show a shiny Pokémon"},
        {"version", 'v', 0, 0, "Displays version information"},
        {"no-title", 1001, 0, 0, "Do not display pokemon name"},
        {"no-mega", 1002, 0, 0, "Do not show mega pokemon"},
        {"no-gmax", 1003, 0, 0, "Do not show gigantamax pokemon"},
        {"no-regional", 1004, 0, 0, "Do not show regional pokemon"},
        {"info", 'i', 0, 0, "Print pokedex entry"},
        {0}
    };

    static char args_doc[] = "[pokemon_name] [form]";
    static char doc[] = "Pokemonc is mostly a C rewrite of phoney badger's pokemon-colorscripts with some extra features.";
    static struct argp argp = {options, parse_opt, args_doc, doc};

    argp_parse(&argp, argc, argv, 0, 0, arguments);
}
