#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <argp.h>
#include "cJSON.h"

#define MAX_FORMS 10
#define JSON_FILE_PATH "../assets/pokemon.json"

// 宝可梦结构体
typedef struct {
    char *name;
    char *slug;
    char *forms[MAX_FORMS];
    int form_count;
} Pokemon;

// 从 JSON 文件中加载宝可梦数据
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

    // 检查顶级 JSON 元素类型
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
            pokemon_list[i].name = NULL; // 设置为 NULL，防止后续访问
            continue;
        }

        cJSON *name_object = cJSON_GetObjectItem(pokemon_json, "name");
        if (!name_object || !cJSON_IsObject(name_object)) {
            printf("宝可梦 %d 缺少 'name' 对象或其类型不正确。\n", i);
            pokemon_list[i].name = NULL; // 设置为 NULL，防止后续访问
            continue;
        }

        // 从 'name' 对象中获取英文名称
        cJSON *name_item = cJSON_GetObjectItem(name_object, "en");
        if (!name_item || !cJSON_IsString(name_item)) {
            printf("宝可梦 %d 缺少英文名称或其类型不正确。\n", i);
            pokemon_list[i].name = NULL; // 设置为 NULL，防止后续访问
            continue;
        }
        pokemon_list[i].name = strdup(name_item->valuestring);

        // 获取 'slug' 字段
        cJSON *slug_item = cJSON_GetObjectItem(pokemon_json, "slug");
        if (!slug_item || !cJSON_IsString(slug_item)) {
            printf("宝可梦 %s 缺少 'slug' 字段或其类型不正确。\n", pokemon_list[i].name);
            pokemon_list[i].slug = NULL; // 设置为 NULL，防止后续访问
            continue;
        }
        pokemon_list[i].slug = strdup(slug_item->valuestring);

        cJSON *forms = cJSON_GetObjectItem(pokemon_json, "forms");
        if (!forms || !cJSON_IsArray(forms)) {
            printf("宝可梦 %s 缺少有效的 'forms' 数组。\n", pokemon_list[i].name);
            pokemon_list[i].form_count = 0; // 没有可用的形态
            continue;
        }

        int form_count = cJSON_GetArraySize(forms);
        pokemon_list[i].form_count = form_count > MAX_FORMS ? MAX_FORMS : form_count; // 防止超过 MAX_FORMS

        for (int j = 0; j < pokemon_list[i].form_count; j++) {
            cJSON *form_item = cJSON_GetArrayItem(forms, j);
            if (form_item && cJSON_IsString(form_item)) {
                pokemon_list[i].forms[j] = strdup(form_item->valuestring);
            } else {
                printf("宝可梦 %s 的形态 %d 无效。\n", pokemon_list[i].name, j);
                pokemon_list[i].forms[j] = NULL; // 设置为 NULL，防止后续访问
            }
        }
    }

    cJSON_Delete(json);
    *count = pokemon_count;
    return pokemon_list;
}

// 列出所有宝可梦的名称
void list_all_pokemon(Pokemon *pokemon_list, int count) {
    for (int i = 0; i < count; i++) {
        if (pokemon_list[i].name) { // 确保名称指针有效
            printf("%s\n", pokemon_list[i].name);
        } else {
            printf("未找到宝可梦的名称。\n");
        }
    }
}

// 显示特定宝可梦及其形态
void display_pokemon(Pokemon *pokemon_list, int count, const char *name, const char *form, int shiny) {
    for (int i = 0; i < count; i++) {
        if (pokemon_list[i].name && strcmp(pokemon_list[i].name, name) == 0) {
            // 使用 slug 构建宝可梦艺术文件的路径
            char art_path[256];
            if (strcmp(form, "regular") == 0) {
                snprintf(art_path, sizeof(art_path), "../assets/colorscripts/%s/%s", shiny ? "shiny" : "regular", pokemon_list[i].slug);
            } else {
                snprintf(art_path, sizeof(art_path), "../assets/colorscripts/%s/%s-%s", shiny ? "shiny" : "regular", pokemon_list[i].slug, form);
            }

            // 打开艺术文件
            FILE *art_file = fopen(art_path, "r");
            if (!art_file) {
                printf("无法读取宝可梦 '%s' 的艺术文件。路径: %s\n", name, art_path);
                return;
            }

            // 读取并打印艺术文件内容
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), art_file) != NULL) {
                printf("%s", buffer);
            }

            fclose(art_file);
            return;
        }
    }
    printf("未找到宝可梦 %s。\n", name);
}

// 显示随机宝可梦
void display_random_pokemon(Pokemon *pokemon_list, int count, int shiny) {
    int index = rand() % count;
    const char *name = pokemon_list[index].name;
    const char *form;

    // 检查是否有可用的形态
    if (pokemon_list[index].form_count > 0 && pokemon_list[index].forms[0]) {
        form = pokemon_list[index].forms[0];
    } else {
        form = "regular"; // 默认形态
    }

    printf("显示随机宝可梦: %s (%s)\n", name, form);
    display_pokemon(pokemon_list, count, name, form, shiny);
}

// 命令行参数结构体
struct arguments {
    char *pokemon_name;
    char *form;
    int list;
    int random;
    int shiny; // 用于显示闪光版宝可梦
};

// 解析命令行参数
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

    // 检查输入的名称和形态是否包含非法字符
    if (arguments->pokemon_name && strpbrk(arguments->pokemon_name, "/\\:*?\"<>|")) {
        printf("错误：宝可梦名称包含非法字符。\n");
        exit(1);
    }
    if (arguments->form && strpbrk(arguments->form, "/\\:*?\"<>|")) {
        printf("错误：形态名称包含非法字符。\n");
        exit(1);
    }

    return 0;
}

const char *argp_program_version = "pokemon-display 1.0";
const char *argp_program_bug_address = "<example@example.com>";
static char doc[] = "一个用于在终端显示宝可梦 ASCII 艺术的命令行工具。";
static char args_doc[] = "[pokemon_name] [form]";
static struct argp_option options[] = {
    {"list", 'l', 0, 0, "列出所有宝可梦"},
    {"random", 'r', 0, 0, "显示随机宝可梦"},
    {"shiny", 's', 0, 0, "显示闪光版宝可梦"},
    {0}
};
static struct argp argp = {options, parse_opt, args_doc, doc};

// 主函数
int main(int argc, char **argv) {
    struct arguments arguments = {NULL, "regular", 0, 0, 0};
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // 使用 gettimeofday() 初始化随机种子
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec * 1000000 + tv.tv_usec);  // 微秒级时间作为种子

    int pokemon_count;
    Pokemon *pokemon_list = load_pokemon_data(JSON_FILE_PATH, &pokemon_count);
    if (!pokemon_list) {
        return 1;
    }

    if (arguments.list) {
        list_all_pokemon(pokemon_list, pokemon_count);
    } else if (arguments.random) {
        display_random_pokemon(pokemon_list, pokemon_count, arguments.shiny);
    } else if (arguments.pokemon_name) {
        display_pokemon(pokemon_list, pokemon_count, arguments.pokemon_name, arguments.form, arguments.shiny);
    } else {
        printf("未指定命令。使用 --help 查看更多信息。\n");
    }

    // 释放分配的内存
    for (int i = 0; i < pokemon_count; i++) {
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

    return 0;
}
