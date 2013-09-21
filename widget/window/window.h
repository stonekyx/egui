/**
 * @file window.h
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

# ifndef _WINDOW_H_
# define _WINDOW_H_ 1

# include <base_type.h>
# include <comm.h> 

# include "../widget/widget.h"

/**
 * 窗口结构体
 *
 * 请注意
 *     请不要赋予宽度成员和高度成员任意的值。
 *     窗口的宽度必须大于等于最小宽度，小于等于最大宽度。
 *     窗口的高度必须大于等于最小高度，小于等于最大高度。
 **/
# define WINDOW_DEFINITION \
    /**
     * 从 widget 继承
     **/ \
    WIDGET_DEFINITION \
    \
    /**
     * 窗口描述符
     **/ \
    si_t descriptor; \
    \
    /**
     * 是否是模态窗口
     **/ \
    si_t modal; \
    \
    /**
     * 标题
     **/ \
    char * title; \
    \
    /**
     * icon path
     **/ \
    char* icon_path; \
    \
    /**
     * 是否可以最大化
     **/ \
    si_t maximize_enable; \
    \
    /**
     * 是否可以最小化
     **/ \
    si_t minimize_enable; \
    \
    /**
     * 当前是否是全屏
     **/ \
    si_t is_full_screen; \
    \
    /**
     * is activated or not
     **/ \
    si_t is_activated;  \
    \
    /**
     * status: see __window_status__
     **/ \
    si_t status; \
    \
    /**
     * 窗口还原
     **/ \
    struct rectangle restore_area;

struct window
{
    WINDOW_DEFINITION
};

/*  窗口样式结构体 */
struct window_style
{
    struct widget_style common;

    /* 是否可以最大化/最小化/全屏 */
    si_t maximize_enable;
    si_t minimize_enable;
    si_t is_full_screen;

    /* 边框/标题栏尺寸 */
    si_t frame_size;
    si_t title_bar_size;

    /** 窗口边框颜色 **/
    struct color frame_color;

    /** 菜单栏背景色 **/
    struct color menu_bar_backcolor;

    /** 菜单栏边框色 **/
    struct color menu_bar_frame_color;

    /** 标题栏字体颜色 **/
    struct color title_bar_font_color;

    /** 标题栏字体大小 **/
    si_t title_font;
};

extern struct window_style window_default_style;

/**
 * 将地址强制转换成 struct window *
 **/
# define WINDOW_POINTER(addr) ((struct window *)(addr))

enum __window_status__
{
    /**
     * maximized window
     **/
    WINDOW_STATUS_MAXIMIZED = 0,
    /**
     * normal window
     **/
    WINDOW_STATUS_NORMAL,
    /**
     * minimized normal window 
     **/
    WINDOW_STATUS_MINMIZED_NORMAL,
    /**
     * minimized maximized window
     * */
    WINDOW_STATUS_MINIMIZED_MAXIMIZED,
    /* quantity */
    WINDOW_STATUS_QUANTITY
};

extern si_t window_default_callback(addr_t w, addr_t m);

/**
 * @brief 重绘窗口
 **/
extern void window_repaint(struct window* w);

/**
 * @brief 显示窗口
 **/
extern void window_show(struct window* w);

/**
 * @brief 使用前设置窗口部件结构体
 *
 * @param title 窗口标题
 *
 * @return 0
 **/
extern struct window* window_init(char* title);

/**
 * @brief 使用后清理窗口部件结构体
 *
 * @param w 窗口部件结构体指针
 *
 * @return 0
 **/
extern si_t window_exit(struct window * w);

/**
 * @brief 设置控件区域
 * 参数若小于0则对应的位置不发生改变
 *
 * @param window 组件
 * @param x  组件左上角 x 
 * @param y  组件左上角 y
 * @param width  组件宽度
 * @param height 组件高度
 *
 **/ 
extern void window_set_bounds(struct window* w, si_t x, si_t y, si_t width , si_t height);

/**
 * @brief 设置基本颜色
 * 参数若为空则对应的位置不发生改变
 *
 * @param w 控件
 * @param fcolor 前景色
 * @param bcolor 背景色
 **/
extern void window_set_color(struct window* w, struct color* fcolor, struct color* bcolor);

# endif

