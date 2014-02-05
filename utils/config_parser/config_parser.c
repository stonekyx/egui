#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <log.h>

#include "config_parser.h"
#include "compiler.h"

#define TMP_ARRAY_LENTH 1024
#define DILIMETER '='

/**
 * @brief 判断一行字符串是否以某个字符开头
 *
 * @param line 要判断的行
 * @param ch 要匹配的字符
 *
 * @return 是返回1，否则返回0
 **/
static si_t starts_with(char line[], const char ch)
{
    if (NULL == line || strlen(line) < 1)
    {
        return 0;
    }

    if (ch != line[0])
    {

        return 0;
    }

    return 1;
}

/**
 * @brief 除去字符串两端的空白符
 *
 * @param str 要处理的字符串
 *
 * @return 成功返回0,否则(即全部由空白符组成的)返回-1
 **/
static si_t strip(char str[])
{
    char *p = str;
    char *q = NULL;
    si_t tmp_len;
    si_t len;

    tmp_len = strlen(str);

    if(p)
    {
        q = p + strlen(str) - 1;

        while(*p && isspace(*p))
        {
            p++;
        }

        while(q >= p && isspace(*q))
        {
            *q-- = '\0';
        }
    }

    if(q - p < 0)
    {
        return -1;
    }

    len = q - p + 1;
    strncpy(str, p, len);
    if(len < tmp_len)
    {
        str[len] = '\0';
    }

    return 0;
}

/**
 * @brief 将一行配置分析为key/value
 *
 * @param line 配置项字符串
 * @param dilimeter 配置项分隔符，如“=”
 * @param config_item 配置项结构体指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t analyze_line(char line[], const char dilimeter, struct config_item * item)
{
    char *pos;
    char key[TMP_ARRAY_LENTH] = {'\0'};
    char value[TMP_ARRAY_LENTH] = {'\0'};

    if (NULL == line || strlen(line) < 1)
    {
        return -1;
    }

    pos = strchr(line, dilimeter);
    if (NULL == pos)
    {
        return -1;
    }

    /* dilimeter左边为key,并去除空格 */
    strncpy(key, line, pos - line);
    if(strip(key) != 0)
    {
        return -1;
    }
    item->key = (char *)malloc(strlen(key)+1);
    memset(item->key, 0, strlen(key)+1);
    strcpy(item->key, key);

    /* dilimeter右边为value,并去除空格 */
    strncpy(value, pos+1, line + strlen(line) - 1 - pos);
    if(strip(value) != 0)
    {
        return -1;
    }
    item->value = (char *)malloc(strlen(value)+1);
    memset(item->value, 0, strlen(value)+1);
    strcpy(item->value, value);

    return 0;
}

/* 初始化配置处理器 */
si_t config_parser_init(const char * path, struct config_parser * parser)
{
    FILE *fp;
    struct config_item item;
    char line[TMP_ARRAY_LENTH] = {'\0'};

    if (NULL == path)
    {
        EGUI_PRINT_ERROR("invalid config file path.");

        return -1;
    }

    memset(parser->config_file_path, 0, sizeof(parser->config_file_path));

    /* 初始化配置文件路径和配置存储vector */
    strcpy(parser->config_file_path, path);
    vector_init(&(parser->config_vector));

    /* 将配置文件读入配置存储vector */
    if ((fp = fopen(parser->config_file_path, "r")) == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to open config file fail.");

        return -1;
    }

    /* 一行一行处理 */
    while(fgets(line, TMP_ARRAY_LENTH, fp) != NULL)
    {
        /* 忽略空行 */
        if(strip(line) == -1)
        {
            continue;
        }

        /* 忽略注释行(此时已经去除了行首空白符) */
        if(starts_with(line, '#'))
        {
            continue;
        }

        /* 配置项行 */
        if(analyze_line(line, DILIMETER, &item) == -1)
        {
            EGUI_PRINT_ERROR("fail to analyze line: %s.", line);

            return -1;
        }

        vector_push_back(&(parser->config_vector), &item, sizeof(item));
    }

    fclose(fp);

    return 0;
}

/**
 * @brief 比较配置项的key值是否相等
 *
 * @param data 配置项对象指针
 *
 * @return 相等返回1，否则返回0
 **/
static si_t key_compare(void * data0, void * data1)
{
    struct config_item * item = (struct config_item *)data0;
    char * key = (char *)data1;

    if(strncmp(key, item->key, strlen(key)) == 0)
    {
        return 1;
    }

    return 0;
}

/* 匹配一个配置项的key,获得字符串型的value */
si_t config_parser_get_str(struct config_parser * parser, const char * key, char value[])
{
    si_t index;
    struct config_item * item;

    if(NULL == key)
    {
        EGUI_PRINT_ERROR("bad key to map.");

        return -1;
    }

    index = vector_find(&(parser->config_vector), (void *)key, key_compare);
    if(index < 0)
    {
        EGUI_PRINT_ERROR("fail to map key %s.", key);

        return -1;
    }

    item = (struct config_item *)vector_at(&(parser->config_vector), index);
    if(NULL == item)
    {
        EGUI_PRINT_ERROR("sorry, i donot know why vector_at failed. but it happened :(");

        return -1;
    }

    /* XXX:是否会发生错误? */
    strncpy(value, item->value, strlen(item->value));

    return 0;
}

/* 匹配一个配置项的key,获得int型的value */
si_t config_parser_get_int(struct config_parser * parser, const char * key, si_t * value)
{
    char str[TMP_ARRAY_LENTH] = {'\0'};

    if(config_parser_get_str(parser, key, str) != 0)
    {
        EGUI_PRINT_ERROR("fail to map key %s", key);

        return -1;
    }

    if(sscanf(str, "%d", (int*)value) != 1)
    {
        EGUI_PRINT_SYS_ERROR("fail to transform %s into type int.", str);

        return -1;
    }

    return 0;
}


/* 匹配一个配置项的key,获得int型的value */
si_t config_parser_get_float(struct config_parser * parser, const char * key, float * value)
{
    char str[TMP_ARRAY_LENTH] = {'\0'};

    if(config_parser_get_str(parser, key, str) != 0)
    {
        EGUI_PRINT_ERROR("fail to map key %s.", key);

        return -1;
    }

    if(sscanf(str, "%f", value) != 1)
    {
        EGUI_PRINT_SYS_ERROR("fail to transform %s into type int.", str);

        return -1;
    }

    return 0;

}

/**
 * @brief 将一个配置项写入文件
 *
 * @param item 配置想结构对象指针
 * @param fp 要写入的文件指针
 *
 * @return 0
 **/
static si_t item_save(void * item, void * fp)
{
    FILE * p = (FILE *)fp;
    struct config_item * it = (struct config_item *)item;

    fprintf(p, "%s=%s\n", it->key, it->value);

    return 0;
}

/**
 * @brief 将当前配置结构数据写入文件
 *
 * @param parser 配置处理器对象的指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t save_config_to_file(struct config_parser * parser)
{
    FILE *fp;

    if((fp = fopen(parser->config_file_path, "w")) == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to open config file %s.", parser->config_file_path);

        return -1;
    }

    vector_for_each(&(parser->config_vector), (void *)fp, item_save);

    fclose(fp);

    return 0;
}

/* 修改或者新建一个字符型value的配置项 */
si_t config_parser_set_str(struct config_parser * parser, const char * key, const char * value)
{
    si_t index;

    if(NULL == key || NULL == value)
    {
        EGUI_PRINT_ERROR("bad key or value pointer parameter.");

        return -1;
    }

    index = vector_find(&(parser->config_vector), (void *)key, key_compare);

    /* 查找此key存在,修改对应的value */
    if(index >= 0)
    {
        struct config_item * item_ptr;

        item_ptr = (struct config_item *)vector_at(&(parser->config_vector), index);

        /* 修改vector中此key对应的value的值 */
        strncpy(item_ptr->value, value, strlen(value));
    }

    /* 此key不存在,新建一个配置项 */
    else
    {
        struct config_item item;

        item.key = (char *)malloc(strlen(key)+1);
        item.value = (char *)malloc(strlen(value)+1);

        memset(item.key, 0, strlen(key)+1);
        memset(item.value, 0, strlen(value)+1);

        strcpy(item.key, key);
        strcpy(item.value, value);

        vector_push_back(&(parser->config_vector), &item, sizeof(item));
    }

    /* 写回文件 */
    if(save_config_to_file(parser) != 0)
    {
        EGUI_PRINT_ERROR("fail to save config to file.");

        return -1;
    }

    return 0;
}

/**
 * @brief 释放配置项对象
 *
 * @param data 配置项对象指针
 *
 * @return 0
 **/
static si_t item_exit(void * data0, void * data1)
{
    struct config_item *item = (struct config_item *)data0;
    NOT_USED(data1);

    free(item->key);
    free(item->value);

    return 0;
}

/* 退出配置处理器 */
si_t config_parser_exit(struct config_parser * parser)
{
    /* 遍历vector，删除各个item */
    vector_for_each(&(parser->config_vector), NULL, item_exit);

    vector_exit(&(parser->config_vector));

    return 0;
}

char *get_config_path(const char *filename)
{
    char *res = calloc(strlen(getenv("HOME"))+1+strlen(CONFIG_DIR)
            +strlen(filename)+1, 1);
    return strcat(
            strcat(
                strcat(
                    strcat(res,
                        getenv("HOME")),
                    "/"),
                CONFIG_DIR),
            filename);
}
