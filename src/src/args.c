#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"
#include "config.h"
#include <argp.h>

#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"

// 命令行参数结构体
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
        case 'l':
            arguments->list = 1;
            break;
        case 'r':
            arguments->random = 1;
            break;
        case 's':
            arguments->shiny = 1;
            break;
        case 'v':
            printf("pokemonc %s\n", arguments->version);
            exit(0);
            break;
        case 'h':
        case ARGP_KEY_END:
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
            exit(0);
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

void parse_arguments(int argc, char **argv, struct arguments *arguments) {
    static struct argp_option options[] = {
        {"list", 'l', 0, 0, "列出所有宝可梦"},
        {"random", 'r', 0, 0, "显示随机宝可梦"},
        {"shiny", 's', 0, 0, "显示闪光版宝可梦"},
        {"version", 'v', 0, 0, "显示版本信息"},
        {"help", 'h', 0, 0, "显示帮助信息"},
        {0}
    };

    static char args_doc[] = "[pokemon_name] [form]";
    static char doc[] = "Pokemonc is mostly a C rewrite of phoney badger's pokemon-colorscripts with some extra features.";
    static struct argp argp = {options, parse_opt, args_doc, doc};

    argp_parse(&argp, argc, argv, 0, 0, arguments);
}
