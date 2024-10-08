#ifndef ARGS_H
#define ARGS_H

#define MAX_GEN_LIST 9

struct arguments {
    char *pokemon_name;
    char *form;
    int list;
    int random;
    int shiny;
    int no_title;
    int no_mega;
    int no_gmax;
    int no_regional;
    int info;
    int gen_min;
    int gen_max;
    int gen_list[MAX_GEN_LIST];
    int gen_count;
    int name_subcommand;
    const char *version;
};

void parse_arguments(int argc, char **argv, struct arguments *arguments);

#endif
