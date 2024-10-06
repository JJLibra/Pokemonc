#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pokemon.h"
#include "display.h"

void list_all_pokemon(Pokemon *pokemon_list, int count) {
    for (int i = 0; i < count; i++) {
        if (pokemon_list[i].name) {
            printf("%s\n", pokemon_list[i].name);
        } else {
            printf("未找到宝可梦的名称。\n");
        }
    }
}

void display_pokemon(Pokemon *pokemon_list, int count, const char *name, const char *form, int shiny) {
    for (int i = 0; i < count; i++) {
        if (pokemon_list[i].name && strcmp(pokemon_list[i].name, name) == 0) {
            char art_path[256];
            if (strcmp(form, "regular") == 0) {
                snprintf(art_path, sizeof(art_path), "../assets/colorscripts/%s/%s", shiny ? "shiny" : "regular", pokemon_list[i].slug);
            } else {
                snprintf(art_path, sizeof(art_path), "../assets/colorscripts/%s/%s-%s", shiny ? "shiny" : "regular", pokemon_list[i].slug, form);
            }

            FILE *art_file = fopen(art_path, "r");
            if (!art_file) {
                printf("无法读取宝可梦 '%s' 的艺术文件。路径: %s\n", name, art_path);
                return;
            }

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

void display_random_pokemon(Pokemon *pokemon_list, int count, int shiny) {
    int index = rand() % count;
    const char *name = pokemon_list[index].name;
    const char *form = pokemon_list[index].form_count > 0 && pokemon_list[index].forms[0] ? pokemon_list[index].forms[0] : "regular";
    printf("显示随机宝可梦: %s (%s)\n", name, form);
    display_pokemon(pokemon_list, count, name, form, shiny);
}
