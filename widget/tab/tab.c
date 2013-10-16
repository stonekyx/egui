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

    list_init(&addr->pages);

    addr->panel = panel_init(0);
    addr->border_size = 0;
    addr->panel->back_color = addr->back_color;
    addr->panel->fore_color = addr->fore_color;
    panel_set_bounds(addr->panel,
            0, 0,
            addr->area.width, addr->area.height);
    object_attach_child(OBJECT_POINTER(addr), OBJECT_POINTER(addr->panel));

    addr->page_titles = flowbox_init(0);
    flowbox_set_bounds(addr->page_titles, 0, 0,
            /* page titles can actually fill up the whole tab widget */
            addr->area.width, addr->area.height);
    object_attach_child(OBJECT_POINTER(addr),
            OBJECT_POINTER(addr->page_titles));

    return addr;
}

/*
    一般用户并不需要调用这个
    用户应用程序退出之后，application 会释放这个控件所占的空间。
    而窗口管理程序会释放所有资源，包括申请的每个图形设备。
*/
si_t tab_exit(struct tab * b)
{
    flowbox_exit(b->page_titles);
    panel_exit(b->panel);
    list_exit(&b->pages);
    return widget_exit(WIDGET_POINTER(b));
}

static void tab_refresh_internal_bounds(struct tab *t)
{
    ui_t used_height;
    flowbox_set_bounds(t->page_titles, 0, 0,
            t->area.width, t->area.height);
    flowbox_reorder_widgets(t->page_titles);
    used_height = flowbox_get_actual_height(t->page_titles);
    panel_set_bounds(t->panel,
            0, used_height+1,
            t->area.width, t->area.height - used_height-1);
}

void tab_set_bounds(struct tab * b, si_t x, si_t y, si_t width , si_t height)
{
    widget_set_bounds(WIDGET_POINTER(b), x, y, width, height);
    tab_refresh_internal_bounds(b);
}

void tab_set_color(struct tab* b, struct color* fcolor, struct color* bcolor)
{
    struct list_node *pos;
    list_for_each_macro(pos, &b->pages) {
        tab_page_set_color(*(struct tab_page **)pos->data,
                fcolor, bcolor);
    }
    panel_set_color(b->panel, fcolor, bcolor);
}

void tab_set_font(struct tab* b, si_t font)
{
    struct list_node *pos;
    list_for_each_macro(pos, &b->pages) {
        tab_page_set_font(*(struct tab_page **)pos->data, font);
    }
}

void tab_add_page(struct tab *t, struct tab_page *tp)
{
    list_push_back(&t->pages, &tp, sizeof(tp));
    object_attach_child(OBJECT_POINTER(t->page_titles),
            OBJECT_POINTER(tp->page_head));
    flowbox_add_widget(t->page_titles, WIDGET_POINTER(tp->page_head));
    tab_refresh_internal_bounds(t);
    if(list_size(&t->pages)==1) {
        tab_set_focus(t, 0);
    }
}

static void tab_object_tree_refresh(struct object *obj)
{
    struct object *tree;
    if(!obj) {
        return;
    }
    /* 找到这棵树根节点的父节点 */
    /* 没有的话 tree == NULL */
	tree = object_get_root(obj)->parent;

    /* obj 在对象树中 */
    /* 必须更新 root->parent 节点的 lchild 成员和 rchild 成员 */
    /* 此时 tree == root->parent */
    if(tree != NULL)
    {
        /* 更新最左边节点 */
        tree->lchild = object_tree_l_most_node(tree->parent);
        /* 更新最右边节点 */
        tree->rchild = object_tree_r_most_node(tree->parent);
    }
}

/* @par not holds a sub-tree of widgets, and these widgets used to
 * belong to @oldpar. We give them back to @oldpar, and move the sub-tree
 * of @newpar to @par. */
static void tab_object_tree_move(struct object *par, struct object *oldpar, struct object *newpar)
{
    if(!par || !newpar) {
        return;
    }
    if(oldpar) {
        oldpar->rchild = par->rchild; /* old sub-tree: parent side */
    }
    if(par->rchild) {
        par->rchild->parent = oldpar; /* old sub-tree: child side */
    }
    par->rchild = newpar->rchild; /* new sub-tree: parent side */
    if(newpar->rchild) {
        newpar->rchild->parent = par; /* new sub-tree: child side */
    }
    newpar->rchild = NULL;

    tab_object_tree_refresh(par);
    tab_object_tree_refresh(oldpar);
    tab_object_tree_refresh(newpar);
}

si_t tab_set_focus(struct tab *t, ui_t page_idx)
{
    struct tab_page *new_focus =
        *(struct tab_page **)list_element_at(&t->pages, page_idx);
    if(!new_focus) {
        return -1;
    }
    tab_object_tree_move(OBJECT_POINTER(t->panel),
            OBJECT_POINTER(t->focus),
            OBJECT_POINTER(new_focus));

    t->focus = new_focus;

    panel_repaint(t->panel);
    return 0;
}
