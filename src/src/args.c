#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"
#include "config.h"
#include <argp.h>

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
        {0}
    };

    static char args_doc[] = "[pokemon_name] [form]";
    static char doc[] = "一个用于在终端显示宝可梦 ASCII 艺术的命令行工具。";
    static struct argp argp = {options, parse_opt, args_doc, doc};

    argp_parse(&argp, argc, argv, 0, 0, arguments);
}
