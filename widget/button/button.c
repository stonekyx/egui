/**
 * @file button.c
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

#define _POSIX_C_SOURCE 200809L /* for strdup() */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <base_type.h>
# include <config_parser.h>
# include <log.h>
# include <comm.h>
# include <client_lib.h>

# include "button.h"

# define TMP_ARRAY_SIZE 256

/* 按钮样式全局对象 */
static struct button_style button_default_style =
{
    {
        /* 初始化，默认未访问 */
        0,  /* .flag */

        /* 默认工作区域 */
        0,  /* .area_x */
        0,  /* .area_y */
        0,  /* .area_width */
        0,  /* .area_height */

        /* 默认边界宽度 */
        1,  /* .border_size */

        /* 默认宽度&高度 */
        0,  /* .maximum_width */
        0,  /* .minimum_width */
        0,  /* .maximum_height */
        0,  /* .minimum_height */

        /* 默认鼠标形状 */
        CURSOR_SHAPE_X, /* .cursor */

        /* 默认背景色 */
        /* FIXME:不应该hard code */
        0,    /* .back_color_r */
        255,    /* .back_color_g */
        0,  /* .back_color_b */
        0,  /* .back_color_a */

        /* 默认前景色 */
        255,  /* .fore_color_r */
        0,  /* .fore_color_g */
        0,  /* .fore_color_b */
        0,  /* .fore_color_a */
    },
    /* 默认字体 */
    FONT_MATRIX_08, /* .font */
};

/**
 * @brief 用button全局样式对象初始化button对象
 *
 * @param style button样式对象指针
 * @param b button指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t button_init_with_default_style(struct button * b)
{
    char tmp_str[TMP_ARRAY_SIZE] = {'\0'};
    si_t tmp_int;
    struct widget_style_entry extra[] = {
        {.key="font", .type=WIDGET_STYLE_TYPE_STR, .val=tmp_str},
    };
    char *config_path = get_config_path("button.cfg");

    /* warnings are expected when no configuration file is available */
    si_t res = widget_init_with_default_style(config_path,
            WIDGET_POINTER(b), &button_default_style.common,
            extra, sizeof(extra)/sizeof(extra[0]));
    free(config_path);

    if((tmp_int = get_font_enum_from_str(tmp_str)) >= 0) {
        button_default_style.font = tmp_int;
    }

    b->font = button_default_style.font;

    return res;
}

static si_t button_default_widget_show(struct button * b, union message * msg) 
{
    struct rectangle area;

    widget_absolute_area(WIDGET_POINTER(b), &area);

    /* 设置区域 */
    set_area(b->gd, area.x, area.y, area.width, area.height);

    /* 更新 */
    update(b->gd);

    return 0;
}

static void _paint_button(struct button* b, si_t is_pressed, struct rectangle* area, int x, int y)
{
    /* 设置区域 */
    set_area(b->gd, area->x, area->y, area->width, area->height);

	/* 背景色 */
	set_color(b->gd, b->back_color.r, b->back_color.g, b->back_color.b, b->back_color.a);
	/* button 背景 */
	fill_rectangle(b->gd, area->x, area->y, area->width, area->height);

	if(is_pressed)
	{
		/* 前景色 */
		set_color(b->gd, b->fore_color.r, b->fore_color.g, b->fore_color.b, b->fore_color.a);
	}

	/* button 右边框和下边框 */
	draw_line(b->gd, x + b->area.width - 1, y, x + b->area.width - 1, y + b->area.height - 1);
	draw_line(b->gd, x, y + b->area.height - 1, x + b->area.width - 1, y + b->area.height - 1);

	/* 前景色 */
	set_color(b->gd, b->fore_color.r, b->fore_color.g, b->fore_color.b, b->fore_color.a);
	/* button 显示的字 */
	set_font(b->gd, b->font);
	show_text(b->gd, x + b->border_size, y + b->border_size, b->text, strlen(b->text));

	if(is_pressed)
	{
		/* 背景色 */
		set_color(b->gd, b->back_color.r, b->back_color.g, b->back_color.b, b->back_color.a);
	}

	/* button 上边框和左边框 */
	draw_line(b->gd, x, y, x + b->area.width - 1, y);
	draw_line(b->gd, x, y, x, y + b->area.height - 1);
}

static si_t button_default_widget_repaint(struct button * b, union message * msg)
{
    struct rectangle area;
    si_t x, y;

    /* 获得左上角的绝对坐标 */
    widget_absolute_coordinate(WIDGET_POINTER(b), &x, &y);

    /* 获得绝对的工作区域 */
    /* 将会舍弃不在父控件内的部分*/
    widget_absolute_area(WIDGET_POINTER(b), &area);

	_paint_button(b, 0, &area, x, y);

    return 0;
}

static si_t button_default_mouse_press(struct button * b, union message * msg)
{
    struct rectangle area;
    si_t x, y;

    /* 获得左上角的绝对坐标 */
    widget_absolute_coordinate(WIDGET_POINTER(b), &x, &y);

    /* 获得绝对的工作区域 */
    /* 将会舍弃不在父控件内的部分*/
    widget_absolute_area(WIDGET_POINTER(b), &area);

	_paint_button(b, 1, &area, x, y);

    return 0;
}

static si_t button_default_mouse_release(struct button * b, union message * msg)
{
    struct rectangle area;
    si_t x, y;

    /* 获得左上角的绝对坐标 */
    widget_absolute_coordinate(WIDGET_POINTER(b), &x, &y);

    /* 获得绝对的工作区域 */
    /* 将会舍弃不在父控件内的部分*/
    widget_absolute_area(WIDGET_POINTER(b), &area);

	_paint_button(b, 0, &area, x, y);

    return 0;
}

si_t button_default_callback(addr_t self, addr_t msg)
{
    struct button * b = self;
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_WIDGET_REPAINT:
            button_default_widget_repaint(b, m);
            button_default_widget_show(b, m);
            break;

        case MESSAGE_TYPE_WIDGET_SHOW:
            button_default_widget_show(b, m);
            break;

        case MESSAGE_TYPE_MOUSE_PRESS:
            button_default_mouse_press(b, m);
            button_default_widget_show(b, m);
            break;

        case MESSAGE_TYPE_MOUSE_RELEASE:
            button_default_mouse_release(b, m);
            button_default_widget_show(b, m);
            break;

        default:
            break;
    }

    return 0;
}

void button_repaint(struct button* b)
{
	widget_repaint(WIDGET_POINTER(b));
}

void button_show(struct button* b)
{
	widget_show(WIDGET_POINTER(b));
}

struct button* button_init(const char* text)
{
    struct button * addr;

    /* 分配存储空间 */
    addr = (struct button *)malloc(sizeof(struct button));

    if(addr == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to malloc");
        return NULL;
    }

    if(!(addr=widget_init_common(WIDGET_POINTER(addr), 0))) {
        return NULL;
    }

    addr->name = "struct button";

    /* 用全局样式对象初始化button样式 */
    button_init_with_default_style(addr);
	
    /* button 子类成员 */
    addr->callback = button_default_callback;
	addr->text = strdup(text);

    addr->custom_data = NULL;

    return addr;
}

/*
    一般用户并不需要调用这个
    用户应用程序退出之后，application 会释放这个控件所占的空间。
    而窗口管理程序会释放所有资源，包括申请的每个图形设备。
*/
si_t button_exit(struct button * b)
{
    free(b->text);
    return widget_exit(WIDGET_POINTER(b));
}

void button_set_bounds(struct button * b, si_t x, si_t y, si_t width , si_t height)
{
	widget_set_bounds(WIDGET_POINTER(b), x, y, width, height);
}

void button_set_bounds_auto(struct button *b)
{
    button_set_bounds(b,
            -1, -1,
            b->border_size*2 +
            get_font_width(b->gd)*strlen(b->text),
            b->border_size*2 +
            get_font_height(b->gd));
}

void button_set_color(struct button* b, struct color* fcolor, struct color* bcolor)
{
	widget_set_color(WIDGET_POINTER(b), fcolor, bcolor);
}

void button_set_font(struct button* b, si_t font)
{
	b->font = font;
}
