/**
 * @file label.h
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

# ifndef _LABEL_H_
# define _LABEL_H_ 1

# include <base_type.h>
# include <comm.h>

# include "../widget/widget.h"

/**
 * 窗口部件结构体
**/
# define LABEL_DEFINITION \
    /**
     * 从 widget 继承
    **/ \
    WIDGET_DEFINITION \
    \
    /**
     * 文本
    **/ \
    char * text; \
    \
    /**
     * 字体
    **/ \
    si_t font;

struct label
{
    LABEL_DEFINITION
};

/* 按钮样式结构体 */
struct label_style
{
    /* 一个标志 */
    si_t flag;

    /* 工作区域 */
    si_t area_x;
    si_t area_y;
    si_t area_width;
    si_t area_height;

    /* 边界宽度 */
    si_t border_size;

    /* 高度&宽度 */
    si_t maximum_width;
    si_t minimum_width;
    si_t maximum_height;
    si_t minimum_height;

    /* 鼠标形状 */
    si_t cursor;

    /* 背景色 */
    si_t back_color_r;
    si_t back_color_g;
    si_t back_color_b;
    si_t back_color_a;

    /* 前景色 */
    si_t fore_color_r;
    si_t fore_color_g;
    si_t fore_color_b;
    si_t fore_color_a;

    /* 字体 */
    si_t font;
};

/**
 * 将地址强制转换成 struct label *
 **/
# define LABEL_POINTER(addr) ((struct label *)(addr))

/**
 * @brief label的默认回调函数
 * 处理重绘事件，显示事件
 *
 * @param self
 * @param msg
 *
 * @return 0
 **/
extern si_t label_default_callback(addr_t self, addr_t msg);

/**
 * @brief 重绘窗口
 **/
extern void label_repaint(struct label* l);

/**
 * @brief 显示窗口
 **/
extern void label_show(struct label* l);

/**
 * @brief 使用前设置窗口部件结构体
 *
 * @param text 标签显示的字符串
 *
 * @return 0
**/
extern struct label* label_init(char* text);

/**
 * @brief 使用后清理窗口部件结构体
 *
 * @param l 窗口部件结构体指针
 *
 * @return 0
**/
extern si_t label_exit(struct label * l);

/**
 * @brief 设置label的位置
 *
 * @param l label控件指针
 * @param x 相对父控件的横坐标
 * @param y 相对父控件的纵坐标
 * @param width 宽度
 * @param height 长度
 **/
extern void label_set_bounds(struct label* label, si_t x, si_t y, si_t width , si_t height);

/**
 * @brief 设置基本颜色
 * 参数若为空则对应的位置不发生改变
 *
 * @param l 控件
 * @param fcolor 前景色
 * @param bcolor 背景色
 **/
extern void label_set_color(struct label* l, struct color* fcolor, struct color* bcolor);

/**
 * @brief 设置字体
 *
 * @param l 控件
 * @param font 字体
 **/
extern void label_set_font(struct label* l, si_t font);

# endif
