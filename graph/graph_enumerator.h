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

# ifndef _GRAPH_ENUMATER_H_
# define _GRAPH_ENUMATER_H_ 1

# include <string.h>

# include <limits.h>

/**
 * 图片对齐格式
 **/
enum ALIGN_TYPE
{
    /**
     * horizontally on the left side
     **/
    ALIGN_HORIZONTAL_TYPE_LEFT = ((ui_t)1) << 0,
    /**
     * horizontally in center
     **/
    ALIGN_HORIZONTAL_TYPE_CENTER = ((ui_t)1) << 1,
    /**
     * horizontally on the right side
     **/
    ALIGN_HORIZONTAL_TYPE_RIGHT = ((ui_t)1) << 2,
    /**
     * horizontally mask
     **/
    ALIGN_HORIZONTAL_TYPE_MASK = 0x7,

    /**
     * vertically on the upper side
     **/
    ALIGN_VERTICAL_TYPE_UP = ((ui_t)1) << 3,
    /**
     * vertically in center
     **/
    ALIGN_VERTICAL_TYPE_CENTER = ((ui_t)1) << 4,
    /**
     * vertically on the downer side
     **/
    ALIGN_VERTICAL_TYPE_DOWN = ((ui_t)1) << 5,
    /**
     * vertically mask
     **/
    ALIGN_VERTICAL_TYPE_MASK = 0x38
};

/**
 * 区域复制的模式
**/
enum __cpy_area_mode__
{
    /** 从缓冲区到存储区 **/
    CPY_AREA_MODE_BUFFER_TO_MEMORY = 0,
    /** 从存储区到缓冲区 **/
    CPY_AREA_MODE_MEMORY_TO_BUFFER,
    /** 从缓冲区到缓冲区 **/
    CPY_AREA_MODE_BUFFER_TO_BUFFER,
    /** 从存储区到存储区 **/
    CPY_AREA_MODE_MEMORY_TO_MEMORY,

    /** 区域复制模式的数目 **/
    CPY_AREA_MODE_QUANTITY,

    /** 区域复制模式的最大数目 **/
    /** ISO C restricts enumerator values to range of 'int' **/
    CPY_AREA_MODE_MAX = INT_MAX
};

enum __video_access_mode__
{
    /**
     * 直接模式
    **/
    VIDEO_ACCESS_MODE_DIRECT = 0,

    /**
     * 缓冲模式
    **/
    VIDEO_ACCESS_MODE_BUFFER
};

/**
 * 字体
**/
enum __font__
{
    FONT_MATIX_MINIMUM = 0,

    /** 点阵字体 宽度为 08 个像素 **/
    FONT_MATRIX_08 = 0,

    /** 点阵字体 宽度为 10 个像素 **/
    FONT_MATRIX_10,

    /** 点阵字体 宽度为 12 个像素 **/
    FONT_MATRIX_12,

    /** 点阵字体 宽度为 14 个像素 **/
    FONT_MATRIX_14,

    /** 点阵字体 宽度为 16 个像素 **/
    FONT_MATRIX_16,

    /** 点阵字体 宽度为 20 个像素 **/
    FONT_MATRIX_20,

    /** 点阵字体 宽度为 24 个像素 **/
    FONT_MATRIX_24,

    /** 点阵字体 宽度为 28 个像素 **/
    FONT_MATRIX_28,

    /** 点阵字体 宽度为 32 个像素 **/
    FONT_MATRIX_32,

    /** 点阵字体 宽度为 40 个像素 **/
    FONT_MATRIX_40,

    /** 点阵字体 宽度为 48 个像素 **/
    FONT_MATRIX_48,

    /** 点阵字体 宽度为 56 个像素 **/
    FONT_MATRIX_56,

    /** 点阵字体 宽度为 64 个像素 **/
    FONT_MATRIX_64,

    FONT_MATRIX_MAXMUM,

    /** 矢量字体 **/
    FONT_VECTOR,

    /** 字体种类数目 **/
    FONT_QUANTITY,

    /** 字体种类最大数目 **/
    /** ISO C restricts enumerator values to range of 'int' **/
    FONT_MAX = INT_MAX
};

enum __cursor_shape__
{
    /** X **/
    CURSOR_SHAPE_X = 0,

    /** 箭头 **/
    CURSOR_SHAPE_ARROW,

    /** 手 **/
    CURSOR_SHAPE_HAND,

    /** 输入文本 **/
    CURSOR_SHAPE_TEXT_INPUT,

    /** 选择文本 **/
    CURSOR_SHAPE_TEXT_SELECT,

    /** 水平调整 **/
    CURSOR_SHAPE_H_RESIZE,

    /** 竖直调整 **/
    CURSOR_SHAPE_V_RESIZE

    /** 对角线调整 **/

    /** 对角线调整 **/
};

#if 0 /** commented by padme 2012-05-10 {{{ **/
/**
 * @brief 从字符串得到字体枚举值
 *
 * @param str 要匹配的字体类型字符串
 *
 * @return 成功返回str对应字体的枚举值,否则返回-1
 **/
extern si_t get_font_enum_from_str(const char * str)
{
    if(NULL == str)
    {
        return -1;
    }

    if(0 == strcmp(str, "FONT_MATRIX_08"))
    {
        return FONT_MATRIX_08;
    }
    if(0 == strcmp(str, "FONT_MATRIX_10"))
    {
        return FONT_MATRIX_10;
    }
    if(0 == strcmp(str, "FONT_MATRIX_12"))
    {
        return FONT_MATRIX_12;
    }
    if(0 == strcmp(str, "FONT_MATRIX_14"))
    {
        return FONT_MATRIX_14;
    }
    if(0 == strcmp(str, "FONT_MATRIX_16"))
    {
        return FONT_MATRIX_16;
    }
    if(0 == strcmp(str, "FONT_MATRIX_20"))
    {
        return FONT_MATRIX_20;
    }
    if(0 == strcmp(str, "FONT_MATRIX_24"))
    {
        return FONT_MATRIX_24;
    }
    if(0 == strcmp(str, "FONT_MATRIX_28"))
    {
        return FONT_MATRIX_28;
    }
    if(0 == strcmp(str, "FONT_MATRIX_32"))
    {
        return FONT_MATRIX_32;
    }
    if(0 == strcmp(str, "FONT_MATRIX_40"))
    {
        return FONT_MATRIX_40;
    }
    if(0 == strcmp(str, "FONT_MATRIX_48"))
    {
        return FONT_MATRIX_48;
    }
    if(0 == strcmp(str, "FONT_MATRIX_56"))
    {
        return FONT_MATRIX_56;
    }
    if(0 == strcmp(str, "FONT_MATRIX_64"))
    {
        return FONT_MATRIX_64;
    }
    if(0 == strcmp(str, "FONT_VECTOR"))
    {
        return FONT_VECTOR;
    }

    return -1;
}

/**
 * @brief 从字符串得到鼠标形状枚举值
 *
 * @param str 要匹配的鼠标形状类型字符串
 *
 * @return 成功返回str对应鼠标形状的枚举值,否则返回-1
 **/
extern si_t get_cursor_enum_from_str(const char * str)
{
    if(NULL == str)
    {
        return -1;
    }

    if(0 == strcmp(str, "CURSOR_SHAPE_X"))
    {
        return CURSOR_SHAPE_X;
    }
    if(0 == strcmp(str, "CURSOR_SHAPE_ARROW"))
    {
        return CURSOR_SHAPE_ARROW;
    }
    if(0 == strcmp(str, "CURSOR_SHAPE_HAND"))
    {
        return CURSOR_SHAPE_HAND;
    }
    if(0 == strcmp(str, "CURSOR_SHAPE_TEXT_INPUT"))
    {
        return CURSOR_SHAPE_TEXT_INPUT;
    }
    if(0 == strcmp(str, "CURSOR_SHAPE_TEXT_SELECT"))
    {
        return CURSOR_SHAPE_TEXT_SELECT;
    }
    if(0 == strcmp(str, "CURSOR_SHAPE_H_RESIZE"))
    {
        return CURSOR_SHAPE_H_RESIZE;
    }
    if(0 == strcmp(str, "CURSOR_SHAPE_V_RESIZE"))
    {
        return CURSOR_SHAPE_V_RESIZE;
    }

    return -1;
}
#endif /** commented by padme 2012-05-10 }}} **/

#endif

