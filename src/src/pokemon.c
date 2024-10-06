#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "pokemon.h"

Pokemon *load_pokemon_data(const char *file_path, int *count, const char *language) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Unable to open JSON file");
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
        printf("Parsing JSON file error: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    if (!cJSON_IsArray(json)) {
        printf("The top-level element of a JSON file is not an array.\n");
        cJSON_Delete(json);
        return NULL;
    }

    int pokemon_count = cJSON_GetArraySize(json);
    Pokemon *pokemon_list = (Pokemon *)malloc(pokemon_count * sizeof(Pokemon));
    if (!pokemon_list) {
        printf("Memory allocation failed.\n");
        cJSON_Delete(json);
        return NULL;
    }

    for (int i = 0; i < pokemon_count; i++) {
        cJSON *pokemon_json = cJSON_GetArrayItem(json, i);
        if (!pokemon_json || !cJSON_IsObject(pokemon_json)) {
            pokemon_list[i].name = NULL;
            continue;
        }

        cJSON *name_object = cJSON_GetObjectItem(pokemon_json, "name");
        if (!name_object || !cJSON_IsObject(name_object)) {
            pokemon_list[i].name = NULL;
            continue;
        }

        cJSON *name_item = cJSON_GetObjectItem(name_object, "en");
        if (!name_item || !cJSON_IsString(name_item)) {
            pokemon_list[i].name = NULL;
            continue;
        }
        pokemon_list[i].name = strdup(name_item->valuestring);

        cJSON *slug_item = cJSON_GetObjectItem(pokemon_json, "slug");
        if (!slug_item || !cJSON_IsString(slug_item)) {
            pokemon_list[i].slug = NULL;
            continue;
        }
        pokemon_list[i].slug = strdup(slug_item->valuestring);

        cJSON *desc_object = cJSON_GetObjectItem(pokemon_json, "desc");
        if (desc_object && cJSON_IsObject(desc_object)) {
            cJSON *desc_item = cJSON_GetObjectItem(desc_object, language);
            if (desc_item && cJSON_IsString(desc_item)) {
                pokemon_list[i].desc = strdup(desc_item->valuestring);
            } else {
                pokemon_list[i].desc = NULL;
            }
        } else {
            pokemon_list[i].desc = NULL;
        }

        cJSON *forms = cJSON_GetObjectItem(pokemon_json, "forms");
        if (!forms || !cJSON_IsArray(forms)) {
            pokemon_list[i].form_count = 0;
            continue;
        }

        int form_count = cJSON_GetArraySize(forms);
        pokemon_list[i].form_count = form_count > MAX_FORMS ? MAX_FORMS : form_count;

        for (int j = 0; j < pokemon_list[i].form_count; j++) {
            cJSON *form_item = cJSON_GetArrayItem(forms, j);
            if (form_item && cJSON_IsString(form_item)) {
                pokemon_list[i].forms[j] = strdup(form_item->valuestring);
            } else {
                pokemon_list[i].forms[j] = NULL;
            }
        }
    }

    cJSON_Delete(json);
    *count = pokemon_count;
    return pokemon_list;
}

void free_pokemon_data(Pokemon *pokemon_list, int count) {
    for (int i = 0; i < count; i++) {
        if (pokemon_list[i].name) {
            free(pokemon_list[i].name);
        }
        if (pokemon_list[i].slug) {
            free(pokemon_list[i].slug);
        }
        if (pokemon_list[i].desc) {
            free(pokemon_list[i].desc);
        }
        for (int j = 0; j < pokemon_list[i].form_count; j++) {
            if (pokemon_list[i].forms[j]) {
                free(pokemon_list[i].forms[j]);
            }
        }
    }
    free(pokemon_list);
}
