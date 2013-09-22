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

typedef void (*spinbox_event_handler)(struct widget *subscriber, struct widget *publisher, si_t event);

enum spinbox_event_type
{
    SPINBOX_CURRENT_CHANGE,
    SPINBOX_EVENT_ALL
};

struct spinbox_subscribe_info
{
    struct widget *subscriber;
    struct widget *publisher;
    si_t event;
    spinbox_event_handler handler;
};

/* spinbox样式结构体 */
struct spinbox_style
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
extern struct spinbox* spinbox_init(si_t maxval, si_t minval, si_t initval);

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

# endif
