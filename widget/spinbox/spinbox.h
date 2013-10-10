/**
 * @file spinbox.h
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

# ifndef _SPINBOX_H_
# define _SPINBOX_H_ 1

# include <base_type.h>
# include <comm.h>

# include "../widget/widget.h"

/**
 * 窗口部件结构体
**/
# define SPINBOX_DEFINITION \
    /**
     * 从 widget 继承
    **/ \
    WIDGET_DEFINITION \
    \
    /**
     * numeric attributes
     **/ \
    si_t maxval, minval, value; \
    \
    /**
     * textbox
     **/ \
    struct text_line *text_number; \
    \
    /**
     * increase button
     **/ \
    struct button *button_up; \
    \
    /**
     * decrease button
     **/ \
    struct button *button_down; \
    \
    /**
     * listeners of our events
     **/ \
    struct list subscribe_info_list;

struct spinbox
{
    SPINBOX_DEFINITION
};

enum spinbox_event_type
{
    SPINBOX_CURRENT_CHANGE,
    SPINBOX_EVENT_ALL
};

/* spinbox样式结构体 */
struct spinbox_style
{
    struct widget_style common;
};

/**
 * 将地址强制转换成 struct spinbox *
 **/
# define SPINBOX_POINTER(addr) ((struct spinbox *)(addr))

/**
 * @brief spinbox的默认回调函数
 * 处理鼠标按下事件，鼠标抬起事件，重绘事件，显示事件
 *
 * @param self
 * @param msg
 *
 * @return 0
 **/
extern si_t spinbox_default_callback(addr_t self, addr_t msg);

/**
 * @brief spinbox重绘
 **/
extern void spinbox_repaint(struct spinbox* b);

/**
 * @brief spinbox显示
 **/
extern void spinbox_show(struct spinbox* b);

/**
 * @brief 使用前设置窗口部件结构体
 *
 * @details
 *
 * @param text 显示在spinbox上的内容
 *
 * @return 0
**/
extern struct spinbox* spinbox_init(si_t minval, si_t maxval, si_t initval);

/**
 * @brief 使用后清理窗口部件结构体
 *
 * @details
 *
 * @param b 窗口部件结构体指针
 *
 * @return 0
**/
extern si_t spinbox_exit(struct spinbox * b);

/**
 * @brief 设置spinbox的位置
 * 需要保留原值的将对应参数设为负数
 *
 * @param b spinbox控件指针
 * @param x 相对父控件的横坐标
 * @param y 相对父控件的纵坐标
 * @param width 宽度
 * @param height 长度
 **/
extern void spinbox_set_bounds(struct spinbox * widget, si_t x, si_t y, si_t width , si_t height);

/**
 * @brief 设置spinbox的颜色
 * 需要保留原值的将对应参数指针设置为空
 *
 * @param b 控件
 * @param fcolor 前景色
 * @param bcolor 背景色
 **/
extern void spinbox_set_color(struct spinbox* b, struct color* fcolor, struct color* bcolor);

/**
 * @brief 设置spinbox的字体
 *
 * @param b 控件
 * @param font 字体
 **/
extern void spinbox_set_font(struct spinbox* b, si_t font);

/**
 * @brief 注册事件监听函数
 *
 * @param pub 发布者（也就是要监听的spinbox）
 * @param sub 接收者（传给处理函数，用于辨识是哪个控件要接收。
 *            因为处理函数显然也是写在某个控件的源文件中的。）
 * @param event 要监听的事件，全部事件都要的话可以传入某个特别的宏，
 *              见该文件前面部分。
 * @param handler 处理函数，当指定事件发生时会被调用
 **/
extern void spinbox_register_move_handler(struct spinbox *pub, struct widget *sub, si_t event, widget_event_handler handler);
# endif
