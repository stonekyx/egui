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

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <base_type.h>
# include <config_parser.h>
# include <log.h>
# include <comm.h>
# include <client_lib.h>
# include <graph.h>

# include "image_view.h"

# define TMP_ARRAY_SIZE 256

/* 按钮样式全局对象 */
static struct image_view_style image_view_default_style =
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
        255,    /* .back_color_r */
        255,    /* .back_color_g */
        0,  /* .back_color_b */
        0,  /* .back_color_a */

        /* 默认前景色 */
        0,  /* .fore_color_r */
        0,  /* .fore_color_g */
        0,  /* .fore_color_b */
        0,  /* .fore_color_a */
    }
};

/**
 * @brief 用image_view全局样式对象初始化image_view对象
 *
 * @param iv image_view指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t image_view_init_with_default_style(struct image_view * iv)
{
    char *config_path = get_config_path("image_view.cfg");

    si_t res = widget_init_with_default_style(config_path,
            WIDGET_POINTER(iv), &image_view_default_style.common,
            NULL, 0);
    free(config_path);

    return res;
}

static si_t image_view_default_widget_show(struct image_view * i, union message * msg)
{
    struct rectangle area;

    widget_absolute_area(WIDGET_POINTER(i), &area);

    /* 设置区域 */
    set_area(i->gd, area.x, area.y, area.width, area.height);

    /* 更新 */
    update(i->gd);

    return 0;
}

static si_t image_view_default_widget_repaint(struct image_view * i, union message * msg)
{
    struct rectangle area;
    si_t x, y;

    /* 获得左上角的绝对坐标 */
    widget_absolute_coordinate(WIDGET_POINTER(i), &x, &y);

    /* 获得绝对的工作区域 */
    /* 将会舍弃不在父控件内的部分*/
    widget_absolute_area(WIDGET_POINTER(i), &area);

    /* 设置区域 */
    set_area(i->gd, area.x, area.y, area.width, area.height);

    set_color(i->gd, i->back_color.r, i->back_color.g, i->back_color.b, i->back_color.a);

    /* 填充矩形 */
    fill_rectangle(i->gd, area.x, area.y, area.width, area.height);

    draw_img(i->gd, i->path, i->align);

    return 0;
}

si_t image_view_default_callback(addr_t self, addr_t msg)
{
    struct image_view * i = self;
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_WIDGET_REPAINT:
            image_view_default_widget_repaint(i, m);
            image_view_default_widget_show(i, m);
            break;

        case MESSAGE_TYPE_WIDGET_SHOW:
            image_view_default_widget_show(i, m);
            break;

        default:
            break;
    }

    return 0;
}

struct image_view* image_view_init(char* path)
{
    struct image_view * addr;

    /* 分配存储空间 */
    addr = (struct image_view *)malloc(sizeof(struct image_view));

    if(addr == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to malloc");
        return NULL;
    }

    if(!(addr=widget_init_common(WIDGET_POINTER(addr), 0))) {
        return NULL;
    }

    addr->name = "struct image_view";

    /* 用全局样式对象初始化image_view样式 */
    image_view_init_with_default_style(addr);

    addr->path = path;
	/* 默认各种居中 */
    addr->align = ALIGN_HORIZONTAL_TYPE_CENTER | ALIGN_VERTICAL_TYPE_CENTER;
    /* 默认的回调函数 */
    addr->callback = image_view_default_callback;

    return addr;
}

/*
    一般用户并不需要调用这个
    用户应用程序退出之后，application 会释放这个控件所占的空间。
    而窗口管理程序会释放所有资源，包括申请的每个图形设备。
*/
si_t image_view_exit(struct image_view * i)
{
    return widget_exit(WIDGET_POINTER(i));
}

void image_view_set_bounds(struct image_view* i, si_t x, si_t y, si_t width, si_t height)
{
	widget_set_bounds(WIDGET_POINTER(i), x, y, width, height);
}

void image_view_set_color(struct image_view* i, struct color* fcolor, struct color* bcolor)
{
	widget_set_color(WIDGET_POINTER(i), fcolor, bcolor);
}

