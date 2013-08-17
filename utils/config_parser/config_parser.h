/*
 * 处理配置文件,格式如下:
   #comments
   key1 = value1
   key2 = value2

 * 每行一个配置项,可以行注释
 * 默认每行配置不超过1024字节
 */

#ifndef _CONFIG_PARSER_H_
#define _CONFIG_PARSER_H_ 1

#include <stdio.h>

#include <data_structures.h>

#define FILE_NAME_MAX_LEN 256

/* 一个配置项*/
struct config_item
{
    char * key;
    char * value;
};

/* 配置处理器结构体 */
struct config_parser
{
    /* 配置文件路径 */
    char config_file_path[FILE_NAME_MAX_LEN];

    /* 配置项存储vector */
    struct vector config_vector;
};

/**
 * @brief 初始化配置处理器
 *
 * @param config_file_path 配置文件路径
 * @param parser 配置处理器对象的指针
 *
 * @return 成功返回0,否则返回-1
 **/
extern si_t config_parser_init(const char * config_file_path, struct config_parser * parser);

/**
 * @brief 匹配一个配置项的key,获得字符串型的value
 *
 * @param config_parser 配置处理器的指针
 * @param key 要匹配的key
 * @param value 对应的value
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t config_parser_get_str(struct config_parser * parser, const char * key, char value[]);

/**
 * @brief 匹配一个配置项的key,获得int型的value
 *
 * @param config_parser 配置处理器的指针
 * @param key 要匹配的key
 * @param value 对应的value
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t config_parser_get_int(struct config_parser * parser, const char * key, si_t * value);

/**
 * @brief 匹配一个配置项的key,获得float型的value
 *
 * @param config_parser 配置处理器的指针
 * @param key 要匹配的key
 * @param value 对应的value
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t config_parser_get_float(struct config_parser * parser, const char * key, float * value);

/**
 * @brief 修改或者新建一个字符型value的配置项
 *
 * @param config_parser 配置处理器的指针
 * @param key 要修改或新建的key
 * @param value 对应的value
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t config_parser_set_str(struct config_parser * parser, const char * key, const char * value);

/**
 * @brief 退出配置处理器
 *
 * @param config_parser 配置处理器的指针
 *
 * @return 0
 **/
extern si_t config_parser_exit(struct config_parser * parser);

#define CONFIG_DIR ".egui/"

/**
 * @brief 获得配置文件的路径
 *
 * @param filename 配置文件名
 *
 * @return 配置文件的路径
 **/
extern char *get_config_path(const char *filename);

#endif

