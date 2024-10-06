#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    char *version;
    char *author;
    char *email;
    char *description;
} Config;

Config *load_config(const char *file_path);
void free_config(Config *config);

#endif
