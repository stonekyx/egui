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

# ifndef _MY_WIDGET_H_
# define _MY_WIDGET_H_ 1

# include <linux/limits.h>

# include "base_type.h"
# include "widget.h"

/* 工作目录 */
extern char working_directory[PATH_MAX];
/* 存放工作目录的所有项 */
/* 元素是 struct directory_item */
extern struct vector file_list;

extern struct label * l;

extern struct scroll_bar* s;

struct my_widget
{
    WIDGET_DEFINITION

    /* 字体 */
    si_t font;

    /* 显示的起始项 */
    si_t start;

    /* 能显示多少个项 */
    si_t count;

    /**
     * frame color
     **/
    struct color frame_color;
};

extern
si_t
my_widget_default_widget_show
(struct my_widget * mw,
 union message * msg);

extern
si_t
my_widget_default_widget_repaint
(struct my_widget * mw,
 union message * msg);

extern
si_t
my_widget_default_mouse_press
(struct my_widget * mw,
 union message * msg);

extern
si_t
my_widget_default_mouse_release
(struct my_widget * mw,
 union message * msg);

extern
si_t
my_widget_default_callback
(void * self,
 void * msg);

/**
 * @brief 绘制窗口部件
 *
 * @details
 *
 * 该函数构造一个消息
 * 然后将这个消息作为参数调用 my_widget_default_repaint
 *
 * @param mw 窗口部件的地址
 *
 * @return 0
**/
extern
si_t
my_widget_repaint
(struct my_widget * mw);

/**
 * @brief 显示窗口部件
 *
 * @details
 *
 * 该函数构造一个消息
 * 然后将这个消息作为参数调用 my_widget_default_repaint
 *
 * @param mw 窗口部件的地址
 *
 * @return 0
**/
extern
si_t
my_widget_show
(struct my_widget * mw);

/**
 * @brief 使用前设置窗口部件结构体
 *
 * @details
 *
 * @param id 窗口部件的标识符
 *
 * @return 0
**/
extern
void *
my_widget_init
(si_t id);

/**
 * @brief 使用后清理窗口部件结构体
 *
 * @details
 *
 * @param mw 窗口部件结构体指针
 *
 * @return 0
**/
extern
si_t
my_widget_exit
(struct my_widget * mw);

# endif
