/**
 * @file flowbox.c
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
# include <log.h>

# include "flowbox.h"

static si_t flowbox_subwidget_adjust_bounds(
        struct flowbox *f,
        struct widget *w
        )
{
    if(w->area.width > f->area.width) {
        return -1; /* widget too wide */
    }

    /* Check if a new line is needed */
    /* Not checking for height overflow */
    if(f->used_width + w->area.width + f->seam > f->area.width) {
        if(f->used_height) {
            f->used_height += f->seam;
        }
        f->used_height += f->max_height_of_row;
        f->max_height_of_row = 0;
        f->used_width = 0;
    }

    /* update position of sub-widget */
    if(0 == f->used_width) {
        widget_set_bounds(w, 0, f->used_height + f->seam, -1, -1);
    } else {
        widget_set_bounds(w,
                f->used_width + f->seam, f->used_height + f->seam,
                -1, -1);
    }

    if(f->used_width) {
        f->used_width += f->seam;
    }
    f->used_width += w->area.width;

    if(f->max_height_of_row < w->area.height) {
        f->max_height_of_row = w->area.height;
    }

    return 0;
}

struct flowbox* flowbox_init(si_t id)
{
    struct flowbox * addr;

    /* 分配存储空间 */
    addr = calloc(1, sizeof(struct flowbox));

    if(addr == NULL) {
        EGUI_PRINT_SYS_ERROR("fail to malloc");
        return NULL;
    }

    if(NULL == (addr = widget_init_common(WIDGET_POINTER(addr), id))) {
        return NULL;
    }
    /* struct flowbox 的成员 */
    addr->name = "struct flowbox";
    addr->back_color.a = 100;
    addr->fore_color.a = 100;

    addr->seam = 0;
    addr->used_width = addr->used_height = 0;
    addr->max_height_of_row = 0;

    list_init(&addr->subwidgets);

    return addr;
}

/*
    一般用户并不需要调用这个
    用户应用程序退出之后，application 会释放这个控件所占的空间。
    而窗口管理程序会释放所有资源，包括申请的每个图形设备。
*/
si_t flowbox_exit(struct flowbox * b)
{
    list_exit(&b->subwidgets);
    return widget_exit(WIDGET_POINTER(b));
}

void flowbox_set_bounds(struct flowbox * b, si_t x, si_t y, si_t width , si_t height)
{
    widget_set_bounds(WIDGET_POINTER(b), x, y, width, height);
}

si_t flowbox_add_widget(struct flowbox *f, struct widget *w)
{
    if(flowbox_subwidget_adjust_bounds(f, w) == -1) {
        return -1;
    }
    if(list_push_back(&f->subwidgets, &w, sizeof(w)) == -1) {
        return -1;
    }
    return 0;
}

si_t flowbox_reorder_widgets(struct flowbox *f)
{
    struct list_node *pos;
    f->used_width = f->used_height = 0;
    f->max_height_of_row = 0;
    list_for_each_macro(pos, &f->subwidgets) {
        if(flowbox_subwidget_adjust_bounds(f, pos->data) == -1) {
            return -1;
        }
    }
    return 0;
}

si_t flowbox_get_actual_height(struct flowbox *f)
{
    return f->used_height + f->max_height_of_row +
        (f->used_height ? f->seam : 0);
}
