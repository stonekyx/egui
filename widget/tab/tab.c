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

# include <config_parser.h>
# include <base_type.h>
# include <log.h>

# include "tab.h"
# include "tab_header.h"
# include "tab_page.h"

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
        221,  /* .back_color_r */
        221,  /* .back_color_g */
        255,  /* .back_color_b */
        0,  /* .back_color_a */

        /* 默认前景色 */
        75,  /* .fore_color_r */
        105,  /* .fore_color_g */
        131,  /* .fore_color_b */
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

struct tab* tab_init(void)
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

    addr->border_size = 0;

    addr->header = tab_header_init();
    addr->header->parent_tab = addr;
    object_attach_child(OBJECT_POINTER(addr),
            OBJECT_POINTER(addr->header));

    return addr;
}

/*
    一般用户并不需要调用这个
    用户应用程序退出之后，application 会释放这个控件所占的空间。
    而窗口管理程序会释放所有资源，包括申请的每个图形设备。
*/
si_t tab_exit(struct tab * b)
{
    tab_header_exit(b->header);
    return widget_exit(WIDGET_POINTER(b));
}

void tab_set_bounds(struct tab * b, si_t x, si_t y, si_t width , si_t height)
{
    widget_set_bounds(WIDGET_POINTER(b), x, y, width, height);
}

/* Set color for all pages */
void tab_set_color(struct tab* b, struct color* fcolor, struct color* bcolor)
{
    struct list_node *pos;
    list_for_each_macro(&b->header->units, pos) {
        panel_set_color(
                PANEL_POINTER(((struct tab_header_unit*)pos->data)->page),
                fcolor, bcolor);
    }
}

void tab_set_font(struct tab* b, si_t font)
{
    tab_header_set_font(b->header, font);
}

void tab_add_page(struct tab *t, struct tab_page *tp)
{
    tab_header_add_page(t->header, tp);
}

si_t tab_set_focus(struct tab *t, struct tab_page *new_focus)
{
    struct list_node *pos;
    list_for_each_macro(&t->header->units, pos) {
        if(((struct tab_header_unit*)pos->data)->page == new_focus) {
            tab_header_set_focus(t->header, pos->data);
            return 0;
        }
    }
    return -1;
}

si_t tab_default_callback(void * t, void *m)
{
    struct tab* self = t;
    union message* msg = m;
    
    widget_default_callback(self, msg);
    return 0;
}
