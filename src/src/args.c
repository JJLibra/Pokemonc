#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"
#include "config.h"
#include <argp.h>

#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_RESET "\033[0m"

void print_random_help() {
    printf(COLOR_YELLOW "USAGE:\n" COLOR_RESET);
    printf("    pokemonc random [OPTIONS] [GENERATIONS]\n\n");

    printf(COLOR_YELLOW "ARGS:\n" COLOR_RESET);
    printf(COLOR_GREEN "    <GENERATIONS>" COLOR_RESET "    Generation number, range (1-8), or list of generations (1,3,6) [default: 1-8]\n\n");

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
        case 1001:
            arguments->no_title = 1;
            break;
        case 1002:
            arguments->no_mega = 1;
            break;
        case 1003:
            arguments->no_gmax = 1;
            break;
        case 'v':
            printf("pokemonc %s\n", arguments->version);
            exit(0);
        case ARGP_KEY_ARG:
            if (arguments->random) {
                // Handle random subcommand arguments (e.g., generations)
                // Add additional logic here if needed
            } else if (state->arg_num == 0) {
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

void parse_arguments(int argc, char **argv, struct arguments *arguments) {
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
        {"list", 'l', 0, 0, "列出所有宝可梦"},
        {"random", 'r', 0, 0, "显示随机宝可梦"},
        {"shiny", 's', 0, 0, "显示闪光版宝可梦"},
        {"version", 'v', 0, 0, "显示版本信息"},
        {"no-title", 1001, 0, 0, "Do not display pokemon name"},
        {"no-mega", 1002, 0, 0, "Do not show mega pokemon"},
        {"no-gmax", 1003, 0, 0, "Do not show gigantamax pokemon"},
        {0}
    };

    static char args_doc[] = "[pokemon_name] [form]";
    static char doc[] = "Pokemonc is mostly a C rewrite of phoney badger's pokemon-colorscripts with some extra features.";
    static struct argp argp = {options, parse_opt, args_doc, doc};

    argp_parse(&argp, argc, argv, 0, 0, arguments);
}
