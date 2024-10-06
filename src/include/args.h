#ifndef ARGS_H
#define ARGS_H

struct arguments {
    char *pokemon_name;
    char *form;
    int list;
    int random;
    int shiny;
    int no_title;
    int no_mega;
    int no_gmax;
    const char *version;
};

void parse_arguments(int argc, char **argv, struct arguments *arguments);

#endif
