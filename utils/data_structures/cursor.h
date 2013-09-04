#ifndef SELF_BASETYPE
#define SELF_BASETYPE _

#include <base_type.h>
#include "color.h"
#include <stdlib.h>

struct cursor_file_header
{
	char cfReserve[2];
	int cfType;
	int cfNum;
};

struct cursor_info_header
{
	short ciWidth;
	short ciHeight;
	short ciColorCount;
	char ciReserve1;
	char ciReserve2[4];
	long ciDataSize;
	long ciDataOffset;
};

struct bmp_info_header
{
	long biSize;
	long biWidth;
	long biHeight;
	int biNum;		/* 位图个数	*/
	int biBitCount;
	long biCompressType;
	long biDataSize;
	char biReserve[16];
};
/*
struct color
{
	unsigned short r;
	unsigned short g;
	unsigned short b;
	unsigned short a;
};
*/

struct cursor
{
	struct cursor_file_header cfHeader;
	struct cursor_info_header *ciHeader;
	struct bmp_info_header *biHeader;
	char **andData;
	char **xorData;
};

struct cursor_pixel
{
	struct color color;
	int x;
	int y;
};

/**
 * @brief 读取光标文件头
 *
 * @param path 光标文件路径
 * @param header 信息存取地址
 *
 **/
void read_cursor_file_header(char *path, struct cursor_file_header *header);

/**
 * @brief 打印光标文件头
 *
 * @param header 目标信息
 *
 **/
void print_cursor_file_header(struct cursor_file_header *header);

/**
 * @brief 读取光标信息头
 *
 * @param path 光标文件路径
 * @param header 信息存取地址
 *
 **/
void read_cursor_info_header(char *path, struct cursor_info_header *header, int order);

/**
 * @brief 打印光标信息头
 *
 * @param header 目标信息
 *
 **/
void print_cursor_info_header(struct cursor_info_header *header);

/**
 * @brief 读取光标位图信息头
 *
 * @param path 光标文件路径
 * @param header 位图信息头存取地址
 * @param order 位图序号(光标文件中可保存多张位图信息)
 *
 **/
void read_bmp_info_header(char *path, struct bmp_info_header *header, int order);

/**
 * @brief 打印光标位图信息头
 *
 * @param header 目标信息
 *
 **/
void print_bmp_info_header(struct bmp_info_header *header);

/**
 * @brief 读取像素块（仅测试用）
 *
 * @param path 目标文件路径
 * @param pale 信息存取地址
 * @param bits 像素个数
 * @param order 序号
 *
 **/
void read_palette(char *path, struct color *pale, int bits, int order);

/**
 * @brief 打印像素块
 *
 * @param pale 目标信息
 * @param bits 像素个数
 *
 **/
void print_palette(struct color *pale, int bits);

/**
 * @brief 读取and xor数据信息
 *
 * @param path 目标文件路径
 * @param cur 信息保存地址
 *
 **/
void read_andxor_data(char *path, struct cursor* cur);

/**
 * @brief 初始化光标文件
 *
 * @param path 目标文件路径
 * @param cur 信息保存地址
 *
 **/
void init_cursor(char *path, struct cursor *cur);


#endif
