#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "config.h"
#include "cJSON.h"

Config *load_config(const char *config_file_path) {
    FILE *file = fopen(config_file_path, "r");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *data = (char *)malloc(length + 1);
    if (!data) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';

    fclose(file);

    cJSON *json = cJSON_Parse(data);
    free(data);

    if (!json) {
        fprintf(stderr, "Error parsing configuration file.\n");
        return NULL;
    }

    Config *config = (Config *)malloc(sizeof(Config));
    if (!config) {
        fprintf(stderr, "Memory allocation failed.\n");
        cJSON_Delete(json);
        return NULL;
    }

    cJSON *language = cJSON_GetObjectItemCaseSensitive(json, "language");
    cJSON *shiny_probability = cJSON_GetObjectItemCaseSensitive(json, "shiny_probability");

    config->language = language && cJSON_IsString(language) ? strdup(language->valuestring) : strdup("en");
    config->shiny_probability = shiny_probability && cJSON_IsNumber(shiny_probability) ? shiny_probability->valuedouble : 1.0;

    cJSON_Delete(json);
    return config;
}

Config *create_default_config(const char *config_file_path) {
    char *config_dir = strdup(config_file_path);
    char *last_slash = strrchr(config_dir, '/');
    if (last_slash) {
        *last_slash = '\0';
        mkdir(config_dir, 0755);
    }
    free(config_dir);

    Config *config = (Config *)malloc(sizeof(Config));
    if (!config) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    config->language = strdup("en");
    config->shiny_probability = 0.01;

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "language", config->language);
    cJSON_AddNumberToObject(json, "shiny_probability", config->shiny_probability);

    char *json_data = cJSON_Print(json);

    FILE *file = fopen(config_file_path, "w");
    if (!file) {
        fprintf(stderr, "Unable to create configuration file at %s\n", config_file_path);
        cJSON_Delete(json);
        free(json_data);
        free_config(config);
        return NULL;
    }

    fprintf(file, "%s", json_data);
    fclose(file);

    cJSON_Delete(json);
    free(json_data);

    return config;
}

void free_config(Config *config) {
    if (config) {
        free(config->language);
        free(config);
    }
}
