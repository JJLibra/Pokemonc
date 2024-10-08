#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "args.h"
#include "config.h"
#include <getopt.h>

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

void print_name_help() {
    printf(COLOR_BOLD_PINK "pokemonc name\n" COLOR_RESET);
    printf(COLOR_YELLOW "USAGE:\n" COLOR_RESET);
    printf("    pokemonc name [OPTIONS] <POKEMON_NAME> [FORM]\n\n");

    printf(COLOR_YELLOW "ARGS:\n" COLOR_RESET);
    printf(COLOR_GREEN "    <POKEMON_NAME>" COLOR_RESET "    Name of the Pokémon\n");
    printf(COLOR_GREEN "    [FORM]" COLOR_RESET "           Form of the Pokémon (optional)\n\n");

    printf(COLOR_YELLOW "OPTIONS:\n" COLOR_RESET);
    printf(COLOR_GREEN "    -h, --help" COLOR_RESET "           Print help information\n");
    printf(COLOR_GREEN "    -s, --shiny" COLOR_RESET "          Show shiny version\n");
    printf(COLOR_GREEN "    -f, --form" COLOR_RESET "           Specify the form\n\n");
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

void parse_arguments(int argc, char **argv, struct arguments *arguments) {
    arguments->gen_min = 0;
    arguments->gen_max = 0;
    arguments->name_subcommand = 0; // Initialize name subcommand flag
    arguments->form = NULL; // Initialize form to NULL

    // Handle general `--help`
    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        print_general_help();
        exit(0);
    }

    // Subcommand handling
    if (argc > 1) {
        if (strcmp(argv[1], "list") == 0) {
            arguments->list = 1;
            argc--;
            argv++;
        } else if (strcmp(argv[1], "name") == 0) {
            arguments->name_subcommand = 1;
            argc--;
            argv++;
        } else if (strcmp(argv[1], "random") == 0) {
            arguments->random = 1;
            argc--;
            argv++;
        } else if (strcmp(argv[1], "help") == 0) {
            print_general_help();
            exit(0);
        }
    }

    int opt;
    int option_index = 0;

    static struct option long_options[] = {
        {"list", no_argument, 0, 'l'},
        {"random", no_argument, 0, 'r'},
        {"name", required_argument, 0, 'n'},
        {"shiny", no_argument, 0, 's'},
        {"version", no_argument, 0, 'v'},
        {"no-title", no_argument, 0, 1001},
        {"no-mega", no_argument, 0, 1002},
        {"no-gmax", no_argument, 0, 1003},
        {"no-regional", no_argument, 0, 1004},
        {"info", no_argument, 0, 'i'},
        {"form", required_argument, 0, 'f'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    // Reset getopt internal variables
    optind = 1;
    opterr = 0;

    while ((opt = getopt_long(argc, argv, "lrn:svihf:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'l':
                arguments->list = 1;
                break;
            case 'r':
                arguments->random = 1;
                break;
            case 'n':
                arguments->pokemon_name = optarg;
                break;
            case 's':
                arguments->shiny = 1;
                break;
            case 'v':
                printf("pokemonc %s\n", arguments->version);
                exit(0);
            case 'i':
                arguments->info = 1;
                break;
            case 'f':
                arguments->form = optarg;
                break;
            case 'h':
                if (arguments->random) {
                    print_random_help();
                } else if (arguments->name_subcommand) {
                    print_name_help();
                } else {
                    print_general_help();
                }
                exit(0);
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
            default:
                print_general_help();
                exit(1);
        }
    }

    // Positional arguments
    if (arguments->random) {
        if (optind < argc) {
            char *arg = argv[optind];
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
        } else {
            // Default to all generations if no specific input is given
            arguments->gen_min = MIN_GEN;
            arguments->gen_max = MAX_GEN;
        }
    } else if (arguments->name_subcommand) {
        if (optind < argc) {
            arguments->pokemon_name = argv[optind++];
        } else {
            printf(COLOR_YELLOW "Error: No Pokémon name provided.\n" COLOR_RESET);
            exit(1);
        }
        // If form is not specified via -f, check for positional form argument
        if (optind < argc && arguments->form == NULL) {
            arguments->form = argv[optind++];
        }
    } else {
        // Default action if no subcommand is provided
        if (optind < argc) {
            arguments->pokemon_name = argv[optind++];
            if (optind < argc) {
                arguments->form = argv[optind++];
            }
        }
    }

    // Set default form if not specified
    if (arguments->form == NULL) {
        arguments->form = "regular";
    }
}
