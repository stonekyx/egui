/**
 * @file tab.c
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
# include <widget.h>

# include "tab.h"

/* tab样式全局对象 */
static struct tab_style tab_default_style =
{
    {
        /* 初始化，默认未访问 */
        0,  /* .flag */

        /* 默认工作区域 */
        0,  /* .area_x */
        0,  /* .area_y */
        100,  /* .area_width */
        30,  /* .area_height */

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
        0,    /* .back_color_r */
        255,    /* .back_color_g */
        0,  /* .back_color_b */
        0,  /* .back_color_a */

        /* 默认前景色 */
        255,  /* .fore_color_r */
        0,  /* .fore_color_g */
        0,  /* .fore_color_b */
        0,  /* .fore_color_a */
    }
};

/**
 * @brief 用tab全局样式对象初始化tab对象
 *
 * @param style tab样式对象指针
 * @param b tab指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t tab_init_with_default_style(struct tab * b)
{
    char *config_path = get_config_path("tab.cfg");

    si_t res = widget_init_with_default_style(config_path,
            WIDGET_POINTER(b), &tab_default_style.common,
            NULL, 0);
    free(config_path);

    return res;
}

static si_t tab_default_widget_show(struct tab * b, union message * msg) 
{
    struct rectangle area;

    widget_absolute_area(WIDGET_POINTER(b), &area);

    /* 设置区域 */
    set_area(b->gd, area.x, area.y, area.width, area.height);

    /* 更新 */
    update(b->gd);

    return 0;
}

static si_t tab_button_callback(addr_t self, addr_t msg)
{
    struct button *b = self;
    union message *m = msg;
    si_t delta = 0;
    if(!strcmp(b->text, "v")) {
        delta = -1;
    } else if(!strcmp(b->text, "^")) {
        delta = 1;
    }
    switch(m->base.type) {
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
            tab_value_change(TAB_POINTER(b->custom_data), delta);
            break;
        default:
            return button_default_callback(self, msg);
    }
    return 0;
}

si_t tab_default_callback(addr_t self, addr_t msg)
{
    struct tab * b = self;
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_WIDGET_REPAINT:
            tab_default_widget_show(b, m);
            break;

        case MESSAGE_TYPE_WIDGET_SHOW:
            tab_default_widget_show(b, m);
            break;

        default:
            break;
    }

    return 0;
}

void tab_repaint(struct tab* b)
{
	widget_repaint(WIDGET_POINTER(b));
}

void tab_show(struct tab* b)
{
	widget_show(WIDGET_POINTER(b));
}

struct tab* tab_init(si_t minval, si_t maxval, si_t initval)
{
    struct tab * addr;

    /* 分配存储空间 */
    addr = (struct tab *)malloc(sizeof(struct tab));

    if(addr == NULL) {
        EGUI_PRINT_SYS_ERROR("fail to malloc");
        return NULL;
    }

    if(NULL == (addr = widget_init_common(WIDGET_POINTER(addr), 0))) {
        return NULL;
    }
    /* struct tab 的成员 */
    addr->name = "struct tab";

    /* 用全局样式对象初始化tab样式 */
    tab_init_with_default_style(addr);
	
    /* 默认的回调函数 */
    addr->callback = tab_default_callback;

    list_init(&addr->pages);

    return addr;
}

/*
    一般用户并不需要调用这个
    用户应用程序退出之后，application 会释放这个控件所占的空间。
    而窗口管理程序会释放所有资源，包括申请的每个图形设备。
*/
si_t tab_exit(struct tab * b)
{
    return widget_exit(WIDGET_POINTER(b));
}

void tab_set_bounds(struct tab * b, si_t x, si_t y, si_t width , si_t height)
{
	widget_set_bounds(WIDGET_POINTER(b), x, y, width, height);
}

void tab_set_color(struct tab* b, struct color* fcolor, struct color* bcolor)
{
	widget_set_color(WIDGET_POINTER(b), fcolor, bcolor);
}

void tab_set_font(struct tab* b, si_t font)
{
}
