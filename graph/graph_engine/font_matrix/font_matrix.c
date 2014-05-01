/**
 * @file
 *
 * @author Dalian University of Technology
 *
 * @section LICENSE
 *
 * Copyright (C) 2010 Dalian University of Technology
 *
 * This file is part of EDUGUI.
 *
 * EDUGUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * EDUGUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EDUGUI; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * All rights reserved.
**/

# include <stdio.h>
# include <unistd.h>

# include "../../graph_engine.h"

/*
   NOTE: Keeping all the arrays as-is, without turning them to pointers and
   malloc'ing. The space efficiency is almost the same, as these uninitialized
   data will be kept on BSS. Besides, these arrays can document themselves. */

si_t font_matrix_inited;

/**
 * 如果文字的宽度不能被 8 整除，那么每行填充一些位。
 *
 * 起始地址是文字的左上角的点。
 * 先行后列，先从左向右遍历行，后从上往下遍历列。
**/

/* 008 * 016 字体 */
ui08_t font_matrix_08[95][1 * 16];

/* 010 * 020 字体 */
ui08_t font_matrix_10[95][2 * 20];

/* 012 * 024 字体 */
ui08_t font_matrix_12[95][2 * 24];

/* 014 * 028 字体 */
ui08_t font_matrix_14[95][2 * 28];

/* 016 * 032 字体 */
ui08_t font_matrix_16[95][2 * 32];

/* 020 * 040 字体 */
ui08_t font_matrix_20[95][3 * 40];

/* 024 * 048 字体 */
ui08_t font_matrix_24[95][3 * 48];

/* 028 * 056 字体 */
ui08_t font_matrix_28[95][4 * 56];

/* 032 * 064 字体 */
ui08_t font_matrix_32[95][4 * 64];

/* 040 * 080 字体 */
ui08_t font_matrix_40[95][5 * 80];

/* 048 * 096 字体 */
ui08_t font_matrix_48[95][6 * 96];

/* 056 * 112 字体 */
ui08_t font_matrix_56[95][7 * 112];

/* 064 * 128 字体 */
ui08_t font_matrix_64[95][8 * 128];

/* 两种可能的字体位置，分别对应安装后和安装前 */
#define DEFAULT_FONT_MATRIX PACKAGE_DATA_PATH"/resource/font_matrix/"
#define NONINST_FONT_MATRIX PACKAGE_SRC_PATH"/resource/font_matrix/"

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

struct m2f_rec_t {
    ui08_t *font_matrix;
    size_t length;
    const char *filename;
};

/* font_matrix数组和字体文件名之间的对应关系 */
static const struct m2f_rec_t m2f[] = {
    {(ui08_t*)font_matrix_08, sizeof(ui08_t)*95*1* 16, "font_matrix_08.dat"},
    {(ui08_t*)font_matrix_10, sizeof(ui08_t)*95*2* 20, "font_matrix_10.dat"},
    {(ui08_t*)font_matrix_12, sizeof(ui08_t)*95*2* 24, "font_matrix_12.dat"},
    {(ui08_t*)font_matrix_14, sizeof(ui08_t)*95*2* 28, "font_matrix_14.dat"},
    {(ui08_t*)font_matrix_16, sizeof(ui08_t)*95*2* 32, "font_matrix_16.dat"},
    {(ui08_t*)font_matrix_20, sizeof(ui08_t)*95*3* 40, "font_matrix_20.dat"},
    {(ui08_t*)font_matrix_24, sizeof(ui08_t)*95*3* 48, "font_matrix_24.dat"},
    {(ui08_t*)font_matrix_28, sizeof(ui08_t)*95*4* 56, "font_matrix_28.dat"},
    {(ui08_t*)font_matrix_32, sizeof(ui08_t)*95*4* 64, "font_matrix_32.dat"},
    {(ui08_t*)font_matrix_40, sizeof(ui08_t)*95*5* 80, "font_matrix_40.dat"},
    {(ui08_t*)font_matrix_48, sizeof(ui08_t)*95*6* 96, "font_matrix_48.dat"},
    {(ui08_t*)font_matrix_56, sizeof(ui08_t)*95*7*112, "font_matrix_56.dat"},
    {(ui08_t*)font_matrix_64, sizeof(ui08_t)*95*8*128, "font_matrix_64.dat"},
};

/* 按顺序读取字体文件，填充到font_matrix里 */
static si_t engine_font_init_prefix(const char *prefix)
{
    ui_t i;
    for(i=0; i<sizeof(m2f)/sizeof(m2f[0]); i++) {
        static char path[PATH_MAX];
        FILE *fin;
        sprintf(path, "%s%s", prefix, m2f[i].filename);
        if(-1==access(path, R_OK)) {
            return -1;
        }
        fin = fopen(path, "rb");
        fread(m2f[i].font_matrix, 1, m2f[i].length, fin);
        fclose(fin);
    }
    font_matrix_inited = 1;
    return 0;
}

/* 查找可用的字体位置，并调用初始化函数 */
si_t engine_font_init()
{
    if(0==access(DEFAULT_FONT_MATRIX, F_OK)) {
        return engine_font_init_prefix(DEFAULT_FONT_MATRIX);
    } else if(0==access(NONINST_FONT_MATRIX, F_OK)) {
        return engine_font_init_prefix(NONINST_FONT_MATRIX);
    } else {
        return -1;
    }
}
