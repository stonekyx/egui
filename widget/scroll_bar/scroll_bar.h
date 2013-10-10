/**
 * @file scrool_bar.h
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

#ifndef _SCROLL_BAR_H_
#define _SCROLL_BAR_H_ 1

# include "base_type.h"
# include "comm.h"

# include "../widget/widget.h"

/**
 * widget structure
 **/
# define SCROLL_BAR_DEFINITION \
    /**
     * inherited from widget
     **/ \
    WIDGET_DEFINITION \
    \
    /**
     * is vertical 垂直
     **/ \
    si_t is_vertical; \
    \
    /**
     * total length of the scroll area
     **/ \
    si_t total_len; \
    \
    /**
     * height of a line; specified by the corresponding scroll area
     * the distance that elevator move when user pressed the arow
     **/ \
    si_t line_height; \
    \
    /**
     * offset relative to the scroll_bar, from the end of the upper/leffer arrow
     **/ \
    si_t elevator_offset; \
    \
    /**
     * height of the elevator
     **/ \
    si_t elevator_height; \
    \
    /**
     * offset of first line of the current view
     * relative to first line of the scroll area
     **/ \
    si_t cur_offset; \
    \
    /**
     * position of last mouse click, relative to left-top point of widget
     **/ \
    si_t click_pos; \
    \
    /**
     * color of frame of the scroll bar
     **/ \
    struct color outterframe_color; \
    \
    /**
     * color of frame of the elevator
     **/ \
    struct color innerframe_color; \
    \
    /**
     * list of subscriber infomation
     **/ \
    struct list subscribe_info_list;


struct scroll_bar
{
    SCROLL_BAR_DEFINITION
};

/* 滚动条样式结构体 */
struct scroll_bar_style
{
    struct widget_style common;

    /* outterframe颜色 */
    si_t outterframe_color_r;
    si_t outterframe_color_g;
    si_t outterframe_color_b;
    si_t outterframe_color_a;

    /* innerframe颜色 */
    si_t innerframe_color_r;
    si_t innerframe_color_g;
    si_t innerframe_color_b;
    si_t innerframe_color_a;
};

/**
 * subscriber is the widget that observe specific event occurs on specific widget(publisher)
 **/
struct subscribe_info
{
    /**
     * the widget that subscribe events
     **/
    struct widget* subscriber;
    /**
     * the widget that publish event
     **/
    struct widget* publisher;
    /**
     * the event to be observed
     **/
    si_t event;
    /**
     * the function that would be called when event happen
     * the three parameters are: the subscriber, the publisher, the event
     **/
    widget_event_handler handler;
};


/**
 * cast address to struct text_line pointer
 **/
# define SCROLL_BAR_POINTER(addr) ((struct scroll_bar*)(addr))

enum SCROLL_BAR_EVENT
{
    /**
     * press up arrow
     **/
    SCROLL_BAR_EVENT_LINE_UP = 0,
    /**
     * press down arow
     **/
    SCROLL_BAR_EVENT_LINE_DOWN,
    /**
     * press spaces above elevator
     **/
    SCROLL_BAR_EVENT_PAGE_UP,
    /**
     * press spaces below elevator
     **/
    SCROLL_BAR_EVENT_PAGE_DOWN,
    /**
     * move elevator
     **/
    SCROLL_BAR_EVENT_CHANGE,
    /**
     * all the events, only happen when register the handler
     **/
    SCROLL_BAR_EVENT_ALL,

    SCROLL_BAR_EVENT_MAX
};

/**
 * @brief scroll_bar的默认回调函数
 * 处理鼠标抬起事件，按下事件，重绘事件，显示事件。
 *
 * @param self
 * @param msg
 *
 * @return 0
 **/
extern si_t scroll_bar_default_callback(addr_t s, addr_t m);

/**
 * @brief scroll_bar重绘
 **/
extern void scroll_bar_repaint(struct scroll_bar* s);

/**
 * @brief scroll_bar显示
 **/
extern void scroll_bar_show(struct scroll_bar* s);

/**
 * @brief 使用前设置窗口部件结构体
 *
 * @param is_vertical 是否是垂直的 一旦定了就不要改啦%>_<%
 * @param total_len 与之关联的区域的总长度
 * @param line_height 与之关联的区域单行长度
 *
 * @return scroll_bar指针
**/
extern struct scroll_bar* scroll_bar_init(si_t is_vertical, si_t total_len, si_t line_height);

/**
 * @brief 使用后清理窗口部件结构体
 *
 * @param s 窗口部件结构体指针
 *
 * @return 0
**/
extern si_t scroll_bar_exit(struct scroll_bar* s);

#if 0 /** commented by padme 2012-05-31 {{{ **/
/**
 * @brief 设置为垂直的滚动条
 **/
extern void scroll_bar_set_horizonal(struct scroll_bar* s);

/**
 * @brief 设置为水平的滚动条
 **/
extern void scroll_bar_set_vertical(struct scroll_bar* s);
#endif /** commented by padme 2012-05-31 }}} **/

/**
 * @brief 设置一行的长度，即按下向下箭头时下移的位移 或按下向上箭头事上移的位移
 *
 * @param s scroll_bar指针
 * @param line_height 行的长度
 **/
extern void scroll_bar_set_line_height(struct scroll_bar* s, si_t line_height);

/**
 * @brief 设置scroll_bar的显示模式，包括总长度，当前偏移
 *
 * @param total_len 总长度
 * @param current_offset 当前偏移
 **/
extern void scroll_bar_set_view(struct scroll_bar* s, si_t total_len, si_t current_offset);

/**
 * @brief 更新当前偏移
 *
 * @param offset 当前偏移
 **/
extern void scroll_bar_update_offset(struct scroll_bar* s, si_t offset);

/**
 * @brief 添加与scroll_bar关联的控件和其对scroll_bar移动事件的处理函数
 *
 * @param w 控件
 * @param event 移动事件
 * @param handler 处理函数
 **/
extern void scroll_bar_register_move_handler(struct scroll_bar* s, struct widget* w, si_t event, widget_event_handler handler);

/**
 * @brief 设置scroll_bar的位置
 * 需要保留原值的将对应参数设为负数
 *
 * @param s scroll_bar控件指针
 * @param x 相对父控件的横坐标
 * @param y 相对父控件的纵坐标
 * @param width 宽度
 * @param height 长度
 **/
extern void scroll_bar_set_bounds(struct scroll_bar* s, si_t x, si_t y, si_t width , si_t height);

/**
 * @brief 设置scrool_bar的颜色
 * 需要保留原值的将对应参数指针设置为空
 *
 * @param s scroll_bar控件指针
 * @param fcolor 前景色
 * @param bcolor 背景色
 * @param ocolor 外边框颜色
 * @param icolor 内边框颜色
 **/
extern void scroll_bar_set_colors(struct scroll_bar* s, struct color* fcolor, struct color* bcolor, struct color* ocolor, struct color* icolor);

#endif
