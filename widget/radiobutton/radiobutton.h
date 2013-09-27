/**
 * @file radiobutton.h
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

# ifndef _RADIOBUTTON_H_
# define _RADIOBUTTON_H_ 1

# include <base_type.h>
# include <comm.h>

# include "../widget/widget.h"

typedef void (*radiobutton_callback_t)(const char *group_name, addr_t usr);

struct radiobutton
{
    WIDGET_DEFINITION
    si_t selected;
    const char *group_name;
    radiobutton_callback_t click_callback;
    addr_t user_data;
};

/* radiobutton样式结构体 */
struct radiobutton_style
{
    struct widget_style common;
};

/**
 * 将地址强制转换成 struct radiobutton *
 **/
# define RADIOBUTTON_POINTER(addr) ((struct radiobutton *)(addr))

/**
 * @brief radiobutton的默认回调函数
 * 处理鼠标按下事件，重绘事件，显示事件
 *
 * @param self
 * @param msg
 *
 * @return 0
 **/
extern si_t radiobutton_default_callback(addr_t self, addr_t msg);

/**
 * @brief radiobutton重绘
 **/
extern void radiobutton_repaint(struct radiobutton* c);

/**
 * @brief radiobutton显示
 **/
extern void radiobutton_show(struct radiobutton* c);

/**
 * @brief 用户端初始化函数
 *
 * @details
 *
 * @param text 显示在radiobutton上的内容
 *
 * @return 0
**/
extern struct radiobutton* radiobutton_init(const char *group_name, int selected);

/**
 * @brief 析构函数
 * FIXME: Never gets called.
 *
 * @details
 *
 * @param c radiobutton结构体指针
 *
 * @return 0
**/
extern si_t radiobutton_exit(struct radiobutton * c);

/**
 * @brief 设置radiobutton的位置
 * 需要保留原值的将对应参数设为负数
 *
 * @param c radiobutton控件指针
 * @param x 相对父控件的横坐标
 * @param y 相对父控件的纵坐标
 * @param width 宽度
 * @param height 长度
 **/
extern void radiobutton_set_bounds(struct radiobutton * c, si_t x, si_t y, si_t width , si_t height);

/**
 * @brief 设置radiobutton的颜色
 * 需要保留原值的将对应参数指针设置为空
 *
 * @param b 控件
 * @param fcolor 前景色
 * @param bcolor 背景色
 **/
extern void radiobutton_set_color(struct radiobutton * c, struct color* fcolor, struct color* bcolor);

/**
 * @brief 选中指定的radiobutton，并取消选中其他同名的radiobutton
 *
 * @param c 控件
 * @param m Placeholder
 **/
extern void radiobutton_select(struct radiobutton *c, union message *m);

# endif
