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

# include <string.h>

# include "client_lib.h"
# include "paint_window_decoration.h"


/**
 * 整数判断奇偶
 *
 * @param i 要判断的整数
 *
 * @return 偶数返回零，奇数返回一
**/
# define PARITY(i) \
    (0X1 & (i))


/**
 * set proper color to window
 *
 * @param w: pointer to window
 * @param c: orignial color
 * @reutrn: 0
 **/
static si_t
set_window_color
(struct window* w,
 struct color* c)
{
    int r = c->r;
    int g = c->g;
    int b = c->b;
    int a = c->a;

    if(w->is_activated)
    {
        set_color(w->gd, r, g, b, a);
    }
    /**
     * deactivate: fade out color
     **/
    else
    {
		struct color_limit limit = {0};
		get_color_limit(&limit);
        set_color(w->gd,
            (r >= (limit.r_limit >> 1)) ? (r) : (r + (limit.r_limit >> 1)),
            (g >= (limit.g_limit >> 1)) ? (g) : (g + (limit.g_limit >> 1)),
            (b >= (limit.b_limit >> 1)) ? (b) : (b + (limit.b_limit >> 1)),
            a);
    }
    return 0;
}

/**
 * 绘制窗口的边框
 *
 * @param w 窗口指针
 *
 * @return 0
**/
static si_t
paint_window_frame
(struct window * w)
{
    si_t i, x, y, width, height;

    x = w->area.x - window_default_style.frame_size;
    y = w->area.y - window_default_style.frame_size - window_default_style.title_bar_size;
    width = w->area.width + window_default_style.frame_size + window_default_style.frame_size;
    height = w->area.height + window_default_style.frame_size + window_default_style.frame_size + window_default_style.title_bar_size;

    set_area(w->gd, x, y, width, height);

    /* 边框 */
    set_window_color(w, &window_default_style.frame_color);
    for(i = 0; i < window_default_style.frame_size; ++ i)
    {
        draw_rectangle (w->gd, x + i, y + i, width - i - i, height - i - i);
    }

    return 0;
}

/**
 * 绘制窗口的菜单按钮
 *
 * @param w 窗口指针
 *
 * @return 0
**/
static si_t
paint_window_menu_button
(struct window * w)
{
    si_t i, x, y, width, height;

    x = w->area.x;
    y = w->area.y - window_default_style.title_bar_size;
    width = window_default_style.title_bar_size;
    height = window_default_style.title_bar_size;

    set_area(w->gd, x, y, width, height);

    /**
     * paint icon
     **/
    if(w->icon_path != NULL)
    {
        draw_img(w->gd, w->icon_path, ALIGN_HORIZONTAL_TYPE_CENTER | ALIGN_VERTICAL_TYPE_CENTER);
    }
    else
    {
        /* 背景 */
        set_window_color(w, &window_default_style.menu_bar_backcolor);
        fill_rectangle(w->gd, x, y, width, height);
    }

    /* 菜单按钮边框 */
    set_window_color(w, &window_default_style.menu_bar_frame_color);
    for(i = 0; i < window_default_style.frame_size; ++ i)
    {
        draw_rectangle(w->gd, x + i, y + i, width - i - i, height - i - i);
    }

    /* 设置为用户工作区域 */
    set_area(w->gd, w->area.x, w->area.y, w->area.width, w->area.height);

    return 0;
}

/**
 * 绘制窗口的标题栏
 *
 * @param w 窗口指针
 *
 * @return 0
**/
static si_t
paint_window_title_bar
(struct window * w)
{
    si_t i, x, y, width, height;

    x = w->area.x + window_default_style.title_bar_size;
    y = w->area.y - window_default_style.title_bar_size;
    width = w->area.width - window_default_style.title_bar_size - window_default_style.title_bar_size;
    height = window_default_style.title_bar_size;

    if(w->minimize_enable == 1)
    {
        width -= window_default_style.title_bar_size;
    }
    if(w->maximize_enable == 1)
    {
        width -= window_default_style.title_bar_size;
    }

    set_area(w->gd, x, y, width, height);

    /* 背景 */
    set_window_color(w, &window_default_style.menu_bar_backcolor);
    fill_rectangle(w->gd, x, y, width, height);

    /* 字体 */
    set_window_color(w, &window_default_style.title_bar_font_color);
    set_font(w->gd, FONT_MATRIX_12);
    /* 标题 */
    show_text(w->gd, x + window_default_style.frame_size, y + window_default_style.frame_size, w->title, strlen(w->title));

    /* 标题栏边框 */
    set_window_color(w, &window_default_style.menu_bar_frame_color);
    for(i = 0; i < window_default_style.frame_size; ++ i)
    {
        draw_rectangle(w->gd, x + i, y + i, width - i - i, height - i - i);
    }

    return 0;
}

/**
 * 绘制窗口的最小化按钮
 *
 * @param w 窗口指针
 *
 * @return 0
**/
static si_t
paint_window_minimize_button
(struct window * w)
{
    si_t i, x, y, width, height;

    if(w->minimize_enable == 1)
    {
        x = w->area.x + w->area.width - window_default_style.title_bar_size - window_default_style.title_bar_size - window_default_style.title_bar_size;
        y = w->area.y - window_default_style.title_bar_size;
        width = window_default_style.title_bar_size;
        height = window_default_style.title_bar_size;

        set_area(w->gd, x, y, width, height);

        /* 背景 */
        set_window_color(w, &window_default_style.menu_bar_backcolor);
        fill_rectangle(w->gd, x, y, width, height);

        /* 最小化按钮边框 */
        set_window_color(w, &window_default_style.menu_bar_frame_color);
        for(i = 0; i < window_default_style.frame_size; ++ i)
        {
            draw_rectangle(w->gd, x + i, y + i, width - i - i, height - i - i);
        }

        /* 偶数 */
        if(PARITY(window_default_style.title_bar_size) == 0)
        {
            draw_rectangle(w->gd, x + (window_default_style.title_bar_size >> 1) - 1, y + (window_default_style.title_bar_size >> 1) - 1, 2, 2);
        }
        /* 奇数 */
        else
        {
            draw_point(w->gd, x + (window_default_style.title_bar_size >> 1), y + (window_default_style.title_bar_size >> 1));
        }
    }

    return 0;
}

/**
 * 绘制窗口的最大化按钮
 *
 * @param w 窗口指针
 *
 * @return 0
**/
static si_t
paint_window_maximize_button
(struct window * w)
{
    si_t i, x, y, width, height;

    if(w->maximize_enable == 1)
    {
        x = w->area.x + w->area.width - window_default_style.title_bar_size - window_default_style.title_bar_size;
        y = w->area.y - window_default_style.title_bar_size;
        width = window_default_style.title_bar_size;
        height = window_default_style.title_bar_size;

        set_area(w->gd, x, y, width, height);

        /* 背景 */
        set_window_color(w, &window_default_style.menu_bar_backcolor);
        fill_rectangle(w->gd, x, y, width, height);

        /* 最大化按钮边框 */
        set_window_color(w, &window_default_style.menu_bar_frame_color);
        for(i = 0; i < window_default_style.frame_size; ++ i)
        {
            draw_rectangle(w->gd, x + i, y + i, width - i - i, height - i - i);
        }

        draw_line(w->gd,
         x + (window_default_style.title_bar_size >> 1),
         y,
         x + (window_default_style.title_bar_size >> 1),
         y + (window_default_style.title_bar_size >> 1));

        draw_line(w->gd,
         x,
         y + (window_default_style.title_bar_size >> 1),
         x + (window_default_style.title_bar_size >> 1),
         y + (window_default_style.title_bar_size >> 1));
    }

    return 0;
}

/**
 * 绘制窗口的关闭按钮
 *
 * @param w 窗口指针
 *
 * @return 0
**/
static si_t
paint_window_close_button
(struct window * w)
{
    si_t i, x, y, width, height;

    x = w->area.x + w->area.width - window_default_style.title_bar_size;
    y = w->area.y - window_default_style.title_bar_size;
    width = window_default_style.title_bar_size;
    height = window_default_style.title_bar_size;

    set_area(w->gd, x, y, width, height);

    /* 背景 */
    set_window_color(w, &window_default_style.menu_bar_backcolor);
    fill_rectangle(w->gd, x, y, width, height);

    /* 最大化按钮边框 */
    set_window_color(w, &window_default_style.menu_bar_frame_color);
    for(i = 0; i < window_default_style.frame_size; ++ i)
    {
        draw_rectangle(w->gd, x + i, y + i, width - i - i, height - i - i);
    }

    draw_line(w->gd, x + 1, y + 1, x + width - 2,  y + height - 2);
    draw_line(w->gd, x + width - 2, y + 1, x + 1,  y + height - 2);

    return 0;
}

si_t paint_window_decoration(struct window * w)
{
    paint_window_frame(w);
    paint_window_menu_button(w);
    paint_window_title_bar(w);
    paint_window_minimize_button(w);
    paint_window_maximize_button(w);
    paint_window_close_button(w);

    return 0;
}
