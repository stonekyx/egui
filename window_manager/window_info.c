/**
 * @file window_info.c
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
#include "window_info.h"

#include <stdlib.h>

#include "log.h"

static si_t title_bar_size = 30;
static si_t frame_size = 3;

extern void window_info_config(si_t f, si_t t)
{
	title_bar_size = t;
	frame_size = f;
}

extern struct window_info* window_info_init(char* title, si_t maxable, si_t minable, si_t modal)
{
	struct window_info* win_info_ptr = (struct window_info*)malloc(sizeof(struct window_info));
	if(NULL == win_info_ptr)
	{
		EGUI_PRINT_SYS_ERROR("failed to malloc window info status for window %s. malloc()", title);
		return NULL;
	}

    win_info_ptr->parent = NULL;
    win_info_ptr->lchild = NULL;
    win_info_ptr->rchild = NULL;
    win_info_ptr->name = "struct window_info";

	win_info_ptr->title = malloc(strlen(title) + 1);
	if(NULL == win_info_ptr->title)
	{
		EGUI_PRINT_SYS_ERROR("failed to malloc title for window %s. malloc()", title);
		free(win_info_ptr);
		return NULL; 
	}
    strncpy(win_info_ptr->title, title, strlen(title) + 1);

    /* 调用请求处理函数 */
    win_info_ptr->minimize_enable = minable;
    win_info_ptr->maximize_enable = maxable;
    win_info_ptr->modal = modal;
    win_info_ptr->need_restore = 0;

	return win_info_ptr;
}

void window_info_resize(struct window_info* win_info_ptr, si_t x, si_t y, si_t w, si_t h)
{
    /* 窗口区域 */
	rectangle_set(&win_info_ptr->area, 
		x - frame_size, y - frame_size - title_bar_size, w + 2 * frame_size, h + 2 * frame_size + title_bar_size);

    /* 用户工作区域 */
	rectangle_set(&win_info_ptr->work_area, x, y, w, h);

    /* 菜单按钮区域 */
	rectangle_set(&win_info_ptr->menu_button_area,
		x, y - title_bar_size, title_bar_size, title_bar_size);

    /* 标题栏区域 */
	rectangle_set(&win_info_ptr->title_bar_area,
		x + title_bar_size, y - title_bar_size, w - 2 * title_bar_size, title_bar_size);

    /* 最小化按钮区域 */
    if(win_info_ptr->minimize_enable == 1)
    {
		rectangle_set(&win_info_ptr->minimize_button_area, 
			x + w - 3 * title_bar_size, y - title_bar_size, title_bar_size, title_bar_size);

        win_info_ptr->title_bar_area.width -= title_bar_size;
    }

    /* 最大化按钮区域 */
    if(win_info_ptr->maximize_enable == 1)
    {
		rectangle_set(&win_info_ptr->maximize_button_area, 
			x + w - 2 * title_bar_size, y - title_bar_size, title_bar_size, title_bar_size);

        win_info_ptr->title_bar_area.width -= title_bar_size;
    }

    /* 退出按钮区域 */
	rectangle_set(&win_info_ptr->close_button_area,
		x + w - title_bar_size, y - title_bar_size, title_bar_size, title_bar_size);
}

void window_info_maximum(struct window_info* win_info_ptr, struct rectangle* max_area)
{
	window_info_resize(win_info_ptr, max_area->x + frame_size, max_area->y + frame_size + title_bar_size, 
		max_area->width - 2 * frame_size, max_area->height - 2 * frame_size - title_bar_size);
}

void window_info_move(struct window_info* win_info_ptr, si_t x_offset, si_t y_offset)
{
	/* 改变窗口区域的位置 */
	rectangle_move(&win_info_ptr->area, x_offset, y_offset);

	/* 改变用户工作区域的位置 */
	rectangle_move(&win_info_ptr->work_area, x_offset, y_offset);

	/* 改变菜单按钮区域的位置 */
	rectangle_move(&win_info_ptr->menu_button_area, x_offset, y_offset);

	/* 改变标题栏区域的位置 */
	rectangle_move(&win_info_ptr->title_bar_area, x_offset, y_offset);

	/* 改变最小化按钮区域的位置 */
	if(win_info_ptr->minimize_enable == 1)
	{
		rectangle_move(&win_info_ptr->minimize_button_area, x_offset, y_offset);
	}

	/* 改变最大化按钮区域的位置 */
	if(win_info_ptr->maximize_enable == 1)
	{
		rectangle_move(&win_info_ptr->maximize_button_area, x_offset, y_offset);
	}

	/* 改变退出按钮区域的位置 */
	rectangle_move(&win_info_ptr->close_button_area, x_offset, y_offset);
}


void window_info_exit(struct window_info* w)
{
	free(w->title);
	free(w);
}

void window_info_tree_exit(struct window_info* win_info_ptr)
{
	if(win_info_ptr->lchild != NULL)
	{
		window_info_tree_exit((struct window_info*)win_info_ptr->lchild);
	}
	if(win_info_ptr->rchild != NULL)
	{
		window_info_tree_exit((struct window_info*)win_info_ptr->rchild);
	}
	window_info_exit(win_info_ptr);
}

si_t window_info_is_click_left_frame(struct window_info* win_info_ptr, struct point* p)
{
	if(p->x >= win_info_ptr->area.x &&
		p->x <= win_info_ptr->work_area.x - 1 &&
		p->y >= win_info_ptr->work_area.y &&
		p->y <= win_info_ptr->work_area.y + win_info_ptr->work_area.height - 1)
		return 1;
	return 0;
}

si_t window_info_is_click_right_frame(struct window_info* win_info_ptr, struct point* p)
{
	if(p->x >= win_info_ptr->work_area.x + win_info_ptr->work_area.width &&
		p->x <= win_info_ptr->work_area.x + win_info_ptr->work_area.width + frame_size - 1 &&
		p->y >= win_info_ptr->work_area.y &&
		p->y <= win_info_ptr->work_area.y + win_info_ptr->work_area.height - 1)
		return 1;
	return 0;
}

si_t window_info_is_click_up_frame(struct window_info* win_info_ptr, struct point* p)
{
	if(p->x >= win_info_ptr->work_area.x &&
		p->x <= win_info_ptr->work_area.x + win_info_ptr->work_area.width - 1 &&
		p->y >= win_info_ptr->area.y &&
		p->y <= win_info_ptr->area.y + frame_size - 1)
		return 1;
	return 0;
}

si_t window_info_is_click_down_frame(struct window_info* win_info_ptr, struct point* p)
{
	if(p->x >= win_info_ptr->work_area.x &&
		p->x <= win_info_ptr->work_area.x + win_info_ptr->work_area.width - 1 &&
		p->y >= win_info_ptr->work_area.y + win_info_ptr->work_area.height &&
		p->y <= win_info_ptr->work_area.y + win_info_ptr->work_area.height + frame_size - 1)
		return 1;
	return 0;
}

si_t window_info_least_width(struct window_info* win_info_ptr)
{
	int title_bar_num = 3;
    if(win_info_ptr->minimize_enable == 1)
	{
		title_bar_num += 1;
	}
    if(win_info_ptr->maximize_enable == 1)
	{
		title_bar_num += 1;
	}
	return title_bar_num * title_bar_size;
}

