#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "pokemon.h"
#include "display.h"
#include "args.h"

int main(int argc, char **argv) {
    struct arguments arguments = {NULL, "regular", 0, 0, 0};
    parse_arguments(argc, argv, &arguments);

    // 使用 gettimeofday() 初始化随机种子
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec * 1000000 + tv.tv_usec);  // 微秒级时间作为种子

    // 加载宝可梦数据
    int pokemon_count;
    Pokemon *pokemon_list = load_pokemon_data("../assets/pokemon.json", &pokemon_count);
    if (!pokemon_list) {
        return 1;  // 加载数据失败
    }

    // 根据命令行参数执行相应的操作
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
    free_pokemon_data(pokemon_list, pokemon_count);

    return 0;
}
