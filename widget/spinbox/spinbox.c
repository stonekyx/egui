/**
 * @file spinbox.c
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

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <base_type.h>
# include <config_parser.h>
# include <log.h>
# include <comm.h>
# include <client_lib.h>
# include <graph.h>

# include "spinbox.h"

# define TMP_ARRAY_SIZE 256

/* spinbox样式全局对象 */
static struct spinbox_style spinbox_default_style =
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

    /* 默认字体 */
    FONT_MATRIX_08, /* .font */
};

/**
 * @brief 用spinbox全局样式对象初始化spinbox对象
 *
 * @param style spinbox样式对象指针
 * @param b spinbox指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t spinbox_init_with_default_style(struct spinbox * b)
{
    char tmp_str[TMP_ARRAY_SIZE] = {'\0'};
    si_t tmp_int = -1;
    /* spinbox全局样式对象指针 */
    struct spinbox_style * style = &spinbox_default_style;

    /* 如果spinbox全局样式对象尚未加载配置 */
    if(!style->flag)
    {
        struct config_parser parser;
        const char *SPINBOX_STYLE_FILE = get_config_path("spinbox.cfg");

        /* 初始化配置处理器 */
        if(config_parser_init(SPINBOX_STYLE_FILE, &parser) != 0)
        {
            EGUI_PRINT_ERROR("fail to init spinbox style from config file %s.", SPINBOX_STYLE_FILE);

            return -1;
        }

        /* 从配置读取各项配置,赋予style指针 */
        config_parser_get_int(&parser, "area_x", &(style->area_x));
        config_parser_get_int(&parser, "area_y", &(style->area_y));
        config_parser_get_int(&parser, "area_width", &(style->area_width));
        config_parser_get_int(&parser, "area_height", &(style->area_height));

        config_parser_get_int(&parser, "border_size", &(style->border_size));

        config_parser_get_int(&parser, "maximum_width", &(style->maximum_width));
        config_parser_get_int(&parser, "minimum_width", &(style->minimum_width));
        config_parser_get_int(&parser, "maximum_height", &(style->maximum_height));
        config_parser_get_int(&parser, "minimum_height", &(style->minimum_height));

        config_parser_get_str(&parser, "cursor", tmp_str);
        if((tmp_int = get_cursor_enum_from_str(tmp_str)) >= 0)
        {
            style->cursor = tmp_int;
        }

        config_parser_get_int(&parser, "back_color_r", &(style->back_color_r));
        config_parser_get_int(&parser, "back_color_g", &(style->back_color_g));
        config_parser_get_int(&parser, "back_color_b", &(style->back_color_b));
        config_parser_get_int(&parser, "back_color_a", &(style->back_color_a));

        config_parser_get_int(&parser, "fore_color_r", &(style->fore_color_r));
        config_parser_get_int(&parser, "fore_color_g", &(style->fore_color_g));
        config_parser_get_int(&parser, "fore_color_b", &(style->fore_color_b));
        config_parser_get_int(&parser, "fore_color_a", &(style->fore_color_a));

        tmp_int = -1;
        memset(tmp_str, 0, TMP_ARRAY_SIZE);
        config_parser_get_str(&parser, "font", tmp_str);
        if((tmp_int = get_font_enum_from_str(tmp_str)) >= 0)
        {
            style->font = tmp_int;
        }

        /* 退出配置处理器 */
        config_parser_exit(&parser);
        style->flag = 1;
    }

    /* 用spinbox默认样式初始化spinbox各样式属性 */
    b->area.x = style->area_x;
    b->area.y = style->area_y;
    b->area.width = style->area_width;
    b->area.height = style->area_height;

    b->border_size = style->border_size;

    b->maximum_width = style->maximum_width;
    b->minimum_width = style->minimum_width;
    b->maximum_height = style->maximum_height;
    b->minimum_height = style->minimum_height;

    b->cursor = style->cursor;

    b->back_color.r = style->back_color_r;
    b->back_color.g = style->back_color_g;
    b->back_color.b = style->back_color_b;
    b->back_color.a = style->back_color_a;

    b->fore_color.r = style->fore_color_r;
    b->fore_color.g = style->fore_color_g;
    b->fore_color.b = style->fore_color_b;
    b->fore_color.a = style->fore_color_a;

    b->font = style->font;

    return 0;
}

static si_t spinbox_default_widget_show(struct spinbox * b, union message * msg) 
{
    struct rectangle area;

    widget_absolute_area(WIDGET_POINTER(b), &area);

    /* 设置区域 */
    set_area(b->gd, area.x, area.y, area.width, area.height);

    /* 更新 */
    update(b->gd);

    return 0;
}

static void _paint_spinbox(struct spinbox* b, si_t is_pressed, struct rectangle* area, int x, int y)
{
	/* 背景色 */
	set_color(b->gd, b->back_color.r, b->back_color.g, b->back_color.b, b->back_color.a);
	/* spinbox 背景 */
	fill_rectangle(b->gd, area->x, area->y, area->width, area->height);

	if(is_pressed)
	{
		/* 前景色 */
		set_color(b->gd, b->fore_color.r, b->fore_color.g, b->fore_color.b, b->fore_color.a);
	}

	/* spinbox 右边框和下边框 */
	draw_line(b->gd, x + b->area.width - 1, y, x + b->area.width - 1, y + b->area.height - 1);
	draw_line(b->gd, x, y + b->area.height - 1, x + b->area.width - 1, y + b->area.height - 1);

	/* 前景色 */
	set_color(b->gd, b->fore_color.r, b->fore_color.g, b->fore_color.b, b->fore_color.a);
	/* spinbox 显示的字 */
	set_font(b->gd, b->font);
	show_text(b->gd, x + b->border_size, y + b->border_size, b->text, strlen(b->text));

	if(is_pressed)
	{
		/* 背景色 */
		set_color(b->gd, b->back_color.r, b->back_color.g, b->back_color.b, b->back_color.a);
	}

	/* spinbox 上边框和左边框 */
	draw_line(b->gd, x, y, x + b->area.width - 1, y);
	draw_line(b->gd, x, y, x, y + b->area.height - 1);
}

static si_t spinbox_default_widget_repaint(struct spinbox * b, union message * msg)
{
    struct rectangle area;
    si_t x, y;

    /* 获得左上角的绝对坐标 */
    widget_absolute_coordinate(WIDGET_POINTER(b), &x, &y);

    /* 获得绝对的工作区域 */
    /* 将会舍弃不在父控件内的部分*/
    widget_absolute_area(WIDGET_POINTER(b), &area);

    /* 设置区域 */
    set_area(b->gd, area.x, area.y, area.width, area.height);

	/* 背景色 */
    set_color(b->gd, b->back_color.r, b->back_color.g, b->back_color.b, b->back_color.a);
    /* spinbox 背景 */
    fill_rectangle(b->gd, area.x, area.y, area.width, area.height);
    /* spinbox 右边框 */
    draw_line(b->gd, x + b->area.width - 1, y, x + b->area.width - 1, y + b->area.height - 1);
    /* spinbox 下边框 */
    draw_line(b->gd, x, y + b->area.height - 1, x + b->area.width - 1, y + b->area.height - 1);

	/* 前景色 */
    set_color(b->gd, b->fore_color.r, b->fore_color.g, b->fore_color.b, b->fore_color.a);
	/* spinbox 显示的字 */
    set_font(b->gd, b->font);
    show_text(b->gd, x + b->border_size, y + b->border_size, b->text, strlen(b->text));
    /* spinbox 上边框 */
    draw_line(b->gd, x, y, x + b->area.width - 1, y);
    /* spinbox 左边框 */
    draw_line(b->gd, x, y, x, y + b->area.height - 1);
	_paint_spinbox(b, 0, &area, x, y);

    return 0;
}

static si_t spinbox_default_mouse_press(struct spinbox * b, union message * msg)
{
    struct rectangle area;
    si_t x, y;

    /* 获得左上角的绝对坐标 */
    widget_absolute_coordinate(WIDGET_POINTER(b), &x, &y);

    /* 获得绝对的工作区域 */
    /* 将会舍弃不在父控件内的部分*/
    widget_absolute_area(WIDGET_POINTER(b), &area);

    /* 设置区域 */
    set_area(b->gd, area.x, area.y, area.width, area.height);

	/* 背景色 */
    set_color(b->gd, b->back_color.r, b->back_color.g, b->back_color.b, b->back_color.a);
    /* 上边框 */
    draw_line(b->gd, x, y, x + b->area.width - 1, y);
    /* 左边框 */
    draw_line(b->gd, x, y, x, y + b->area.height - 1);

	/* 前景色 */
    set_color(b->gd, b->fore_color.r, b->fore_color.g, b->fore_color.b, b->fore_color.a);
    /* 右边框 */
    draw_line(b->gd, x + b->area.width - 1, y, x + b->area.width - 1, y + b->area.height - 1);
    /* 下边框 */
    draw_line(b->gd, x, y + b->area.height - 1, x + b->area.width - 1, y + b->area.height - 1);
	_paint_spinbox(b, 1, &area, x, y);

    return 0;
}

static si_t spinbox_default_mouse_release(struct spinbox * b, union message * msg)
{
    struct rectangle area;
    si_t x, y;

    /* 获得左上角的绝对坐标 */
    widget_absolute_coordinate(WIDGET_POINTER(b), &x, &y);

    /* 获得绝对的工作区域 */
    /* 将会舍弃不在父控件内的部分*/
    widget_absolute_area(WIDGET_POINTER(b), &area);

    /* 设置区域 */
    set_area(b->gd, area.x, area.y, area.width, area.height);

	/* 背景色 */
    set_color(b->gd, b->back_color.r, b->back_color.g, b->back_color.b, b->back_color.a);
    /* spinbox 右边框 */
    draw_line(b->gd, x + b->area.width - 1, y, x + b->area.width - 1, y + b->area.height - 1);
    /* spinbox 下边框 */
    draw_line(b->gd, x, y + b->area.height - 1, x + b->area.width - 1, y + b->area.height - 1);

	/* 前景色 */
    set_color(b->gd, b->fore_color.r, b->fore_color.g, b->fore_color.b, b->fore_color.a);
    /* spinbox 上边框 */
    draw_line(b->gd, x, y, x + b->area.width - 1, y);
    /* spinbox 左边框 */
    draw_line(b->gd, x, y, x, y + b->area.height - 1);
	_paint_spinbox(b, 0, &area, x, y);

    return 0;
}

si_t spinbox_default_callback(addr_t self, addr_t msg)
{
    struct spinbox * b = self;
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_WIDGET_REPAINT:
            spinbox_default_widget_repaint(b, m);
            spinbox_default_widget_show(b, m);
            break;

        case MESSAGE_TYPE_WIDGET_SHOW:
            spinbox_default_widget_show(b, m);
            break;

        case MESSAGE_TYPE_MOUSE_PRESS:
            spinbox_default_mouse_press(b, m);
            spinbox_default_widget_show(b, m);
            break;

        case MESSAGE_TYPE_MOUSE_RELEASE:
            spinbox_default_mouse_release(b, m);
            spinbox_default_widget_show(b, m);
            break;

        default:
            break;
    }

    return 0;
}

void spinbox_repaint(struct spinbox* b)
{
	widget_repaint(WIDGET_POINTER(b));
}

void spinbox_show(struct spinbox* b)
{
	widget_show(WIDGET_POINTER(b));
}

struct spinbox* spinbox_init(char* text)
{
    struct spinbox * addr;

    /* 分配存储空间 */
    addr = (struct spinbox *)malloc(sizeof(struct spinbox));

    if(addr == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to malloc");

        return NULL;
    }

    /* 申请图形设备 */
    addr->gd = graphics_device_init(0, 0, 0, 0, 0, 0 ,0 ,0 ,0);

    /* 申请失败 */
    if(addr->gd == 0)
    {
        /* 释放存储空间 */
        free(addr);

        return NULL;
    }

    /* struct spinbox 的成员 */
    addr->parent = NULL;
    addr->lchild = NULL;
    addr->rchild = NULL;
    addr->name = "struct spinbox";
    addr->id = 0;

    /* 默认是否能处理键盘输入消息 */
    addr->input_enable = 0;

    /* 默认是否可以刷新 */
    addr->update_enable = 1;

    /* 默认是否可见 */
    addr->visible = 1;

    /* 默认是否拥有键盘焦点*/
    addr->keybd_focus = 0;

    /* 默认是否是窗口 */
    addr->is_window = 0;

    /* 用全局样式对象初始化spinbox样式 */
    spinbox_init_with_default_style(addr);
	
    /* 默认的回调函数 */
    addr->callback = spinbox_default_callback;

	addr->text = text;

    return addr;
}

/*
    一般用户并不需要调用这个
    用户应用程序退出之后，application 会释放这个控件所占的空间。
    而窗口管理程序会释放所有资源，包括申请的每个图形设备。
*/
si_t spinbox_exit(struct spinbox * b)
{
    graphics_device_exit(b->gd);

    free(b);

    return 0;
}

void spinbox_set_bounds(struct spinbox * b, si_t x, si_t y, si_t width , si_t height)
{
	widget_set_bounds(WIDGET_POINTER(b), x, y, width, height);
}

void spinbox_set_color(struct spinbox* b, struct color* fcolor, struct color* bcolor)
{
	widget_set_color(WIDGET_POINTER(b), fcolor, bcolor);
}

void spinbox_set_font(struct spinbox* b, si_t font)
{
	b->font = font;
}
