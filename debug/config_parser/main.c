#include <stdio.h>
#include <string.h>

#include <config_parser.h>

#define TMP_ARRAY_LENTH 1024

int main()
{
    struct config_parser parser;
    char key[TMP_ARRAY_LENTH] = {'\0'};
    char value[TMP_ARRAY_LENTH] = {'\0'};
    int int_value;
    float float_value;

    if(config_parser_init("./test.cfg", &parser) != 0)
    {
        printf("fp is NULL.\n");

        return -1;
    }

    printf("------------------------------------\n");

    strcpy(key, "liu");
    if(config_parser_get_str(&parser, key, value) != 0)
    {
        printf("map %s faild!\n", key);

        return -1;
    }
    printf("value of key [%s] is [%s]\n", key, value);

    strcpy(key, "lenth");
    if(config_parser_get_int(&parser, key, (si_t*)&int_value) != 0)
    {
        printf("map %s faild!\n", key);

        return -1;
    }
    printf("value of key [%s] is [%d]\n", key, int_value);

    strcpy(key, "height");
    if(config_parser_get_float(&parser, key, &float_value) != 0)
    {
        printf("map %s faild!\n", key);

        return -1;
    }
    printf("value of key [%s] is [%f]\n", key, float_value);

    printf("------------------------------------\n");

    if(config_parser_set_str(&parser, "liu", "yanping") != 0)
    {
        printf("set key liu failed.\n");

        return -1;
    }

    if(config_parser_set_str(&parser, "qu", "shisong") != 0)
    {
        printf("add item qu=shisong failed.\n");

        return -1;
    }

    config_parser_exit(&parser);

    return 0;
}

