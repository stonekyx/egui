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

# include <ctype.h>
# include <string.h>

# include <data_structures.h>

# include "../graph_engine.h"

extern ui08_t font_matrix_08[][1 *  16];
extern ui08_t font_matrix_10[][2 *  20];
extern ui08_t font_matrix_12[][2 *  24];
extern ui08_t font_matrix_14[][2 *  28];
extern ui08_t font_matrix_16[][2 *  32];
extern ui08_t font_matrix_20[][3 *  40];
extern ui08_t font_matrix_24[][3 *  48];
extern ui08_t font_matrix_28[][4 *  56];
extern ui08_t font_matrix_32[][4 *  64];
extern ui08_t font_matrix_40[][5 *  80];
extern ui08_t font_matrix_48[][6 *  96];
extern ui08_t font_matrix_56[][7 * 112];
extern ui08_t font_matrix_64[][8 * 128];

/**
 * 获得文字的图像数据
 *
 * 根据字体类型获得文字图像数据的起始地址
 *
 * @param type 字体类型
 *
 * @return 文字图像数据的起始地址
**/
static void *
__font_matrix_addr__
(si_t type,
 char c)
{
    switch(type)
    {
        case FONT_MATRIX_08:
            return font_matrix_08[c - 32];

        case FONT_MATRIX_10:
            return font_matrix_10[c - 32];

        case FONT_MATRIX_12:
            return font_matrix_12[c - 32];

        case FONT_MATRIX_14:
            return font_matrix_14[c - 32];

        case FONT_MATRIX_16:
            return font_matrix_16[c - 32];

        case FONT_MATRIX_20:
            return font_matrix_20[c - 32];

        case FONT_MATRIX_24:
            return font_matrix_24[c - 32];

        case FONT_MATRIX_28:
            return font_matrix_28[c - 32];

        case FONT_MATRIX_32:
            return font_matrix_32[c - 32];

        case FONT_MATRIX_40:
            return font_matrix_40[c - 32];

        case FONT_MATRIX_48:
            return font_matrix_48[c - 32];

        case FONT_MATRIX_56:
            return font_matrix_56[c - 32];

        case FONT_MATRIX_64:
            return font_matrix_64[c - 32];

        default:
            break;
    }

    return NULL;
}

/**
 * 显示矩阵字体
 *
 * @param gd 图形设备指针
 * @param x 文字左上角的横坐标
 * @param y 文字左上角的纵坐标
 * @param ch 文字
 *
 * @return 0
**/
static si_t
__matrix_text__
(si_t graphics_device_handle,
 si_t x,
 si_t y,
 char c)
{
    si_t i, j, w, h, linebyte;
    byte_t * addr;
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;


    /* 文字可以显示 */
    if(isprint(c))
    {
        /* 获得宽度 */
        /* 多少像素 */
        w = engine_get_font_width(graphics_device_handle);
        /* 获得高度 */
        /* 多少像素 */
        h = engine_get_font_height(graphics_device_handle);

        /* 每一行数据所占的字节数 */
        linebyte = w >> 3;
        /* 有余数 */
        if(w & 0X7)
        {
            ++ linebyte;
        }

        /* 获得文字图形数据的地址 */
        /* addr 指向文字左上角的字节 */
        addr = __font_matrix_addr__(gd->font, c);

        /* 依次绘制每个点 */
        for(j = 0; j < h; ++ j)
        {
            for(i = 0; i < w; ++ i)
            {
                if(GET_BIT(addr, i) == 1)
                {
                    screen_set_pixel(&(gd->screen), &(gd->rectangle), &(gd->color), x + i, y + j);
                }
            }

            addr += linebyte;
        }
    }

    return 0;
}

# if 0

/**
 * 显示向量字体
 *
 * @param gd 图形设备指针
 * @param x 文字左上角的横坐标
 * @param y 文字左上角的纵坐标
 * @param ch 文字
 *
 * @return 0
**/
static si_t
__vector_text__
(si_t graphics_device_handle,
 si_t x,
 si_t y,
 char ch)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    return 0;
}

# endif

si_t
engine_show_text
(si_t graphics_device_handle,
 si_t x,
 si_t y,
 char * str,
 si_t length)
{
    si_t i, w;

    /* 文字的宽度 */
    w = engine_get_font_width(graphics_device_handle);

    /* 依次显示文字 */
    for(i = 0; i < length; ++ i)
    {
        __matrix_text__(graphics_device_handle, x, y, str[i]);
        x += w;
    }

    return 0;
}
