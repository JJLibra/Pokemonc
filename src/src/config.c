#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "config.h"

Config *load_config(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Unable to open configuration file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_data = (char *)malloc(file_size + 1);
    fread(json_data, 1, file_size, file);
    fclose(file);
    json_data[file_size] = '\0';

    cJSON *json = cJSON_Parse(json_data);
    free(json_data);
    if (!json) {
        printf("Error parsing configuration file: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    Config *config = (Config *)malloc(sizeof(Config));
    if (!config) {
        printf("Memory allocation failed.\n");
        cJSON_Delete(json);
        return NULL;
    }

    cJSON *version_item = cJSON_GetObjectItem(json, "version");
    config->version = version_item && cJSON_IsString(version_item) ? strdup(version_item->valuestring) : strdup("unknown");

    cJSON *author_item = cJSON_GetObjectItem(json, "author");
    config->author = author_item && cJSON_IsString(author_item) ? strdup(author_item->valuestring) : strdup("unknown");

    cJSON *email_item = cJSON_GetObjectItem(json, "email");
    config->email = email_item && cJSON_IsString(email_item) ? strdup(email_item->valuestring) : strdup("unknown");

    cJSON *description_item = cJSON_GetObjectItem(json, "description");
    config->description = description_item && cJSON_IsString(description_item) ? strdup(description_item->valuestring) : strdup("No description provided.");

    cJSON *language_item = cJSON_GetObjectItem(json, "language");
    config->language = language_item && cJSON_IsString(language_item) ? strdup(language_item->valuestring) : strdup("en");

    cJSON *shiny_prob_item = cJSON_GetObjectItem(json, "shiny_probability");
    config->shiny_probability = shiny_prob_item && cJSON_IsNumber(shiny_prob_item) ? shiny_prob_item->valuedouble : 0.0;

    cJSON_Delete(json);
    return config;
}

void free_config(Config *config) {
    if (config) {
        free(config->version);
        free(config->author);
        free(config->email);
        free(config->description);
        free(config->language);
        free(config);
    }
}
