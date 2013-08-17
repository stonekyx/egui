/**
 * @file checkbox.h
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

# ifndef _CHECKBOX_H_
# define _CHECKBOX_H_ 1

# include <base_type.h>
# include <comm.h>

# include "../widget/widget.h"

typedef void (*checkbox_callback_t)(addr_t usr);

struct checkbox
{
    WIDGET_DEFINITION
    int checked;
    checkbox_callback_t click_callback;
    addr_t user_data;
};

/* 按钮样式结构体 */
struct checkbox_style
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
};

/**
 * 将地址强制转换成 struct checkbox *
 **/
# define CHECKBOX_POINTER(addr) ((struct checkbox *)(addr))

/**
 * @brief checkbox的默认回调函数
 * 处理鼠标按下事件，重绘事件，显示事件
 *
 * @param self
 * @param msg
 *
 * @return 0
 **/
extern si_t checkbox_default_callback(addr_t self, addr_t msg);

/**
 * @brief checkbox重绘
 **/
extern void checkbox_repaint(struct checkbox* c);

/**
 * @brief checkbox显示
 **/
extern void checkbox_show(struct checkbox* c);

/**
 * @brief 用户端初始化函数
 *
 * @details
 *
 * @param text 显示在checkbox上的内容
 *
 * @return 0
**/
extern struct checkbox* checkbox_init(int checked);

/**
 * @brief 析构函数
 * FIXME: Never gets called.
 *
 * @details
 *
 * @param c checkbox结构体指针
 *
 * @return 0
**/
extern si_t checkbox_exit(struct checkbox * c);

/**
 * @brief 设置checkbox的位置
 * 需要保留原值的将对应参数设为负数
 *
 * @param c checkbox控件指针
 * @param x 相对父控件的横坐标
 * @param y 相对父控件的纵坐标
 * @param width 宽度
 * @param height 长度
 **/
extern void checkbox_set_bounds(struct checkbox * c, si_t x, si_t y, si_t width , si_t height);

/**
 * @brief 设置按钮的颜色
 * 需要保留原值的将对应参数指针设置为空
 *
 * @param b 控件
 * @param fcolor 前景色
 * @param bcolor 背景色
 **/
extern void checkbox_set_color(struct checkbox * c, struct color* fcolor, struct color* bcolor);

/**
 * @brief 反转checkbox的选中状态
 *
 * @param c 控件
 **/
extern void checkbox_toggle(struct checkbox *c);

# endif
