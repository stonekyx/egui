/**
 * @file tab_page.h
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
 * MERCHANTAB_PAGEILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EDUGUI; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * All rights reserved.
**/

# ifndef _TAB_PAGE_H_
# define _TAB_PAGE_H_ 1

# include <base_type.h>
# include <data_structures.h>

# include "../widget/widget.h"

/**
 * 标签页结构体
**/
struct tab_page
{
    si_t left_offset;
    struct button *page_head;
    struct list widgets;
};

/**
 * 将地址强制转换成 struct tab_page *
 **/
# define TAB_PAGE_POINTER(addr) ((struct tab_page *)(addr))

/**
 * @brief 使用前设置窗口部件结构体
 *
 * @details
 *
 * @param title 显示在tab_page上的内容
 *
 * @return 0
**/
extern struct tab_page* tab_page_init(const char *title);

/**
 * @brief 使用后清理窗口部件结构体
 *
 * @details
 *
 * @param b 窗口部件结构体指针
 *
 * @return 0
**/
extern si_t tab_page_exit(struct tab_page * b);

/**
 * @brief 设置tab_page的颜色，仅设置标签头颜色
 * 需要保留原值的将对应参数指针设置为空
 *
 * @param p 控件
 * @param fore 前景色
 * @param back 背景色
 **/
extern void tab_page_set_color(struct tab_page *p, struct color *fore, struct color *back);

/**
 * @brief 设置tab_page的字体
 *
 * @param b 控件
 * @param font 字体
 **/
extern void tab_page_set_font(struct tab_page *p, si_t font);

/**
 * @brief 设置标签页按钮的宽度
 *
 * @param b 要设置的标签页
 * @param width 目标宽度（仅按钮）
 **/
extern void tab_page_set_title_width(struct tab_page *b, ui_t width);

/**
 * @brief 添加子控件
 *
 * @param p 要添加到的标签页
 * @param w 要添加的控件
 **/
extern void tab_page_add_widget(struct tab_page *p, struct widget *w);

# endif
