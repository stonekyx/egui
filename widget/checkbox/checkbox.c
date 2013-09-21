/**
 * @file checkbox.c
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

# include "checkbox.h"

# define TMP_ARRAY_SIZE 256

/* checkbox样式全局对象 */
static struct checkbox_style checkbox_default_style =
{
    {
        /* 初始化，默认未访问 */
        0,  /* .flag */

        /* 默认工作区域 */
        0,  /* .area_x */
        0,  /* .area_y */
        13,  /* .area_width */
        13,  /* .area_height */

        /* 默认边界宽度 */
        1,  /* .border_size */

        /* 默认宽度&高度 */
        13,  /* .maximum_width */
        13,  /* .minimum_width */
        13,  /* .maximum_height */
        13,  /* .minimum_height */

        /* 默认鼠标形状 */
        CURSOR_SHAPE_X, /* .cursor */

        /* 默认背景色 */
        255,    /* .back_color_r */
        255,    /* .back_color_g */
        255,  /* .back_color_b */
        0,  /* .back_color_a */

        /* 默认前景色 */
        0,  /* .fore_color_r */
        0,  /* .fore_color_g */
        0,  /* .fore_color_b */
        0,  /* .fore_color_a */
    }
};

/**
 * @brief 用checkbox全局样式对象初始化checkbox对象
 *
 * @param style checkbox样式对象指针
 * @param b checkbox指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t checkbox_init_with_default_style(struct checkbox * b)
{
    char *config_path = get_config_path("checkbox.cfg");
    si_t res;
    res = widget_init_with_default_style(config_path,
            WIDGET_POINTER(b), &checkbox_default_style.common, NULL, 0);
    free(config_path);
    return res;
}

static si_t checkbox_default_widget_show(struct checkbox * b, union message * msg) 
{
    struct rectangle area;

    widget_absolute_area(WIDGET_POINTER(b), &area);

    /* 设置区域 */
    set_area(b->gd, area.x, area.y, area.width, area.height);

    /* 更新 */
    update(b->gd);

    return 0;
}

static void _paint_checkbox(struct checkbox* c, si_t is_checked, struct rectangle* area, int x, int y)
{
    /* 背景色 */
    set_color(c->gd, c->back_color.r, c->back_color.g, c->back_color.b, c->back_color.a);
    /* checkbox 背景 */
    fill_rectangle(c->gd, area->x, area->y, area->width, area->height);

    /* 前景色 */
    set_color(c->gd, c->fore_color.r, c->fore_color.g, c->fore_color.b, c->fore_color.a);
    /* checkbox 右边框和下边框 */
    draw_line(c->gd, x + c->area.width - 1, y, x + c->area.width - 1, y + c->area.height - 1);
    draw_line(c->gd, x, y + c->area.height - 1, x + c->area.width - 1, y + c->area.height - 1);
    /* checkbox 上边框和左边框 */
    draw_line(c->gd, x, y, x + c->area.width - 1, y);
    draw_line(c->gd, x, y, x, y + c->area.height - 1);

    if(is_checked) {
        draw_line(c->gd, x, y, x+c->area.width-1, y+c->area.height-1);
        draw_line(c->gd, x, y+c->area.height-1, x+c->area.width-1, y);
    }
}

static si_t checkbox_default_widget_repaint(struct checkbox *c, union message *msg)
{
    struct rectangle area;
    si_t x, y;

    /* 获得左上角的绝对坐标 */
    widget_absolute_coordinate(WIDGET_POINTER(c), &x, &y);
    /* 获得绝对的工作区域 */
    /* 将会舍弃不在父控件内的部分*/
    widget_absolute_area(WIDGET_POINTER(c), &area);
    /* 设置区域 */
    set_area(c->gd, area.x, area.y, area.width, area.height);

    _paint_checkbox(c, c->checked, &area, x, y);
    return 0;
}

si_t checkbox_default_callback(addr_t self, addr_t msg)
{
    struct checkbox * b = self;
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_WIDGET_REPAINT:
            checkbox_default_widget_repaint(b, m);
            checkbox_default_widget_show(b, m);
            break;
        case MESSAGE_TYPE_WIDGET_SHOW:
            checkbox_default_widget_show(b, m);
            break;
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
            checkbox_toggle(b);
            checkbox_default_widget_repaint(b, m);
            checkbox_default_widget_show(b, m);
            if(b->click_callback) {
                b->click_callback(b->user_data);
            }
            break;
        default:
            break;
    }
    return 0;
}

void checkbox_repaint(struct checkbox* c)
{
    widget_repaint(WIDGET_POINTER(c));
}

void checkbox_show(struct checkbox* c)
{
    widget_show(WIDGET_POINTER(c));
}

struct checkbox* checkbox_init(int checked)
{
    struct checkbox * addr;

    /* 分配存储空间 */
    addr = (struct checkbox *)malloc(sizeof(struct checkbox));
    if(addr == NULL) {
        EGUI_PRINT_SYS_ERROR("fail to malloc");
        return NULL;
    }

    if(!(addr=widget_init_common(WIDGET_POINTER(addr), 0))) {
        return NULL;
    }

    addr->name = "struct checkbox";

    /* 用全局样式对象初始化checkbox样式 */
    checkbox_init_with_default_style(addr);

    /* checkbox 子类成员 */
    addr->callback = checkbox_default_callback;
    addr->checked = checked;
    addr->click_callback = NULL;
    addr->user_data = NULL;

    return addr;
}

/*
   一般用户并不需要调用这个
   用户应用程序退出之后，application 会释放这个控件所占的空间。
   而窗口管理程序会释放所有资源，包括申请的每个图形设备。
   */
si_t checkbox_exit(struct checkbox * c)
{
    return widget_exit(WIDGET_POINTER(c));
}

void checkbox_set_bounds(struct checkbox *c, si_t x, si_t y, si_t width , si_t height)
{
    widget_set_bounds(WIDGET_POINTER(c), x, y, width, height);
}

void checkbox_set_color(struct checkbox *c, struct color *fcolor, struct color *bcolor)
{
    widget_set_color(WIDGET_POINTER(c), fcolor, bcolor);
}

void checkbox_toggle(struct checkbox* c)
{
    if(c->checked)
        c->checked = 0;
    else
        c->checked = 1;
}
