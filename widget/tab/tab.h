/**
 * @file tab.h
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

# ifndef _TAB_H_
# define _TAB_H_ 1

# include <base_type.h>
# include <data_structures.h>

# include "../widget/widget.h"

# include "tab_page.h"
# include "../panel/panel.h"
# include "../flowbox/flowbox.h"

/**
 * 窗口部件结构体
**/
struct tab
{
    WIDGET_DEFINITION
    struct list pages;
    struct panel *panel;
    struct tab_page *focus;
    struct tab_header *header;
};

/* tab样式结构体 */
struct tab_style
{
    struct widget_style common;
};

/**
 * 将地址强制转换成 struct tab *
 **/
# define TAB_POINTER(addr) ((struct tab *)(addr))

/**
 * @brief 使用前设置窗口部件结构体
 *
 * @details
 *
 * @return 0
**/
extern struct tab* tab_init(void);

/**
 * @brief 使用后清理窗口部件结构体
 *
 * @details
 *
 * @param b 窗口部件结构体指针
 *
 * @return 0
**/
extern si_t tab_exit(struct tab * b);

/**
 * @brief 设置tab的位置
 * 需要保留原值的将对应参数设为负数
 *
 * @param b tab控件指针
 * @param x 相对父控件的横坐标
 * @param y 相对父控件的纵坐标
 * @param width 宽度
 * @param height 长度
 **/
extern void tab_set_bounds(struct tab * widget, si_t x, si_t y, si_t width , si_t height);

/**
 * @brief 设置tab的颜色
 * 需要保留原值的将对应参数指针设置为空
 *
 * @param b 控件
 * @param fcolor 前景色
 * @param bcolor 背景色
 **/
extern void tab_set_color(struct tab* b, struct color* fcolor, struct color* bcolor);

/**
 * @brief 设置tab的字体
 *
 * @param b 控件
 * @param font 字体
 **/
extern void tab_set_font(struct tab* b, si_t font);

extern void tab_add_page(struct tab *t, struct tab_page *tp);

extern si_t tab_set_focus(struct tab *t, struct tab_page *new_focus);

extern si_t tab_set_focus_index(struct tab *t, ui_t page_idx);

# endif
