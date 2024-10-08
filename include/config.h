#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    char *language;
    double shiny_probability;
} Config;

Config *load_config(const char *config_file_path);
Config *create_default_config(const char *config_file_path);
void free_config(Config *config);

#endif
