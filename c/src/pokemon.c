#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "pokemon.h"

Pokemon *load_pokemon_data(const char *file_path, int *count) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("无法打开 JSON 文件");
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
        printf("解析 JSON 文件错误: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    if (!cJSON_IsArray(json)) {
        printf("JSON 文件的顶级元素不是数组。\n");
        cJSON_Delete(json);
        return NULL;
    }

    int pokemon_count = cJSON_GetArraySize(json);
    Pokemon *pokemon_list = (Pokemon *)malloc(pokemon_count * sizeof(Pokemon));
    if (!pokemon_list) {
        printf("内存分配失败。\n");
        cJSON_Delete(json);
        return NULL;
    }

    for (int i = 0; i < pokemon_count; i++) {
        cJSON *pokemon_json = cJSON_GetArrayItem(json, i);
        if (!pokemon_json || !cJSON_IsObject(pokemon_json)) {
            printf("宝可梦 %d 不是一个有效的对象。\n", i);
            pokemon_list[i].name = NULL;
            continue;
        }

        cJSON *name_object = cJSON_GetObjectItem(pokemon_json, "name");
        if (!name_object || !cJSON_IsObject(name_object)) {
            printf("宝可梦 %d 缺少 'name' 对象或其类型不正确。\n", i);
            pokemon_list[i].name = NULL;
            continue;
        }

        cJSON *name_item = cJSON_GetObjectItem(name_object, "en");
        if (!name_item || !cJSON_IsString(name_item)) {
            printf("宝可梦 %d 缺少英文名称或其类型不正确。\n", i);
            pokemon_list[i].name = NULL;
            continue;
        }
        pokemon_list[i].name = strdup(name_item->valuestring);

        cJSON *slug_item = cJSON_GetObjectItem(pokemon_json, "slug");
        if (!slug_item || !cJSON_IsString(slug_item)) {
            printf("宝可梦 %s 缺少 'slug' 字段或其类型不正确。\n", pokemon_list[i].name);
            pokemon_list[i].slug = NULL;
            continue;
        }
        pokemon_list[i].slug = strdup(slug_item->valuestring);

        cJSON *forms = cJSON_GetObjectItem(pokemon_json, "forms");
        if (!forms || !cJSON_IsArray(forms)) {
            printf("宝可梦 %s 缺少有效的 'forms' 数组。\n", pokemon_list[i].name);
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
                printf("宝可梦 %s 的形态 %d 无效。\n", pokemon_list[i].name, j);
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
        for (int j = 0; j < pokemon_list[i].form_count; j++) {
            if (pokemon_list[i].forms[j]) {
                free(pokemon_list[i].forms[j]);
            }
        }
    }
    free(pokemon_list);
}
