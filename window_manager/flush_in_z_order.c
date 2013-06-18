/**
 * @file flush_in_z_order.c
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

# include "flush_in_z_order.h"
# include "cursor.h"
# include "window_info_iterator.h"

static void flush_area_to_screen(struct screen* s, struct rectangle* area)
{
	screen_cpy_area_alpha(s->memory_addr, s->buffer_addr,
		area->x, area->y, area->x, area->y, area->width, area->height);
}

static void paint_desktop(struct rectangle* area)
{
	struct rectangle result = {0};
	area_intersection(&global_wm.work_area, area, &result);
	if(NULL == global_wm.desktop_app_ptr)
	{
		screen_set_area(&global_screen, &global_wm.work_area, &global_wm.backcolor, result.x, result.y, result.width, result.height);
	}
	else
	{
		flush_area_to_screen(&global_wm.desktop_app_ptr->screen, &result);
	}
}

static si_t _do_flush_window(struct window_info_iterator* iter, addr_t arg)
{
	struct rectangle* area = (struct rectangle*)arg;
	struct rectangle flush_area = {0};
	if(area_intersection(&iter->win_info_ptr->area, area, &flush_area) != -1)
	{
		flush_area_to_screen(&iter->app_info_ptr->screen, &flush_area);
	}
	return 0;
}

si_t flush_window_in_z_order(struct application_info * app_info_ptr, struct rectangle* area)
{
	struct window_info_iterator iter;
	window_info_iterator_clear(&iter);

	/**
	 * 顺序遍历该应用程序的所有顶层窗口数组的所有窗口树的所有窗口，
	 * 将每个窗口与该区域交集的地方依次刷新
	 **/
	iter.app_info_ptr = app_info_ptr;
	app_traversal_increment(&iter, _do_flush_window, area);

	return 0;
}

static si_t _do_flush_until(struct window_info_iterator* iter, addr_t arg)
{
	struct temp_arg
	{
		struct rectangle* area;
		struct window_info* win_info_ptr;
	};
	struct temp_arg* tmp = (struct temp_arg*)arg;

	/* 这是临界条件 */
	if(tmp->win_info_ptr == iter->win_info_ptr)
	{
		return 1;
	}

	_do_flush_window(iter, tmp->area);
	return 0;
}
	
si_t flush_below_in_z_order(struct application_info * app_info_ptr, struct window_info * win_info_ptr, struct rectangle* area)
{
	struct temp_arg
	{
		struct rectangle* area;
		struct window_info* win_info_ptr;
	}tmp;
	struct window_info_iterator iter;
	window_info_iterator_clear(&iter);

	paint_desktop(area);

	if(app_info_ptr == NULL || win_info_ptr == NULL)
	{
		return 0;
	}

	iter.app_info_ptr = app_info_ptr;
	tmp.win_info_ptr = win_info_ptr;
	tmp.area = area;

	/**
	 * 顺序遍历所有应用程序的所有顶层窗口数组的所有窗口树的所有窗口直到遇到某个窗口为止，
	 * 将每个窗口与该区域交集的地方依次刷新
	 **/
	all_app_traversal_increment(&iter, _do_flush_until, &tmp);

	return 0;
}

static si_t _do_find_window(struct window_info_iterator* iter, addr_t arg)
{
	struct window_info* win_info_ptr = (struct window_info*)arg;
	if(win_info_ptr == iter->win_info_ptr)
		return 1;
	return 0;
}

si_t flush_above_in_z_order(struct application_info * app_info_ptr, struct window_info * win_info_ptr, struct rectangle* area)
{
	struct window_info_iterator iter;
	window_info_iterator_clear(&iter);

	if(app_info_ptr == NULL || win_info_ptr == NULL)
	{
		return 0;
	}

	/**
	 * 顺序遍历所有应用程序的所有顶层窗口数组的所有窗口树的所有窗口直到找到某个窗口为止
	 **/ 
	all_app_traversal_increment(&iter, _do_find_window, win_info_ptr);

	/**
	 * 从窗口所在的下一个窗口开始 遍历这棵窗口树
	 * 将每个窗口与该区域交集的地方依次刷新
	 **/
	iter.win_info_ptr = object_tree_iterator_increment(iter.top_win_info_ptr->parent, OBJECT_POINTER(iter.win_info_ptr));
	win_tree_traversal_increment_from(&iter, iter.win_info_ptr, _do_flush_window, area);

	/**
	 * 从窗口所在的下一个顶层窗口开始 遍历这个应用程序
	 * 将每个窗口与该区域交集的地方依次刷新
	 **/
	app_traversal_increment_from(&iter, iter.win_index + 1, _do_flush_window, area);

	/**
	 * 从窗口所在的下一个应用程序开始 遍历所有应用程序
	 * 将每个窗口与该区域交集的地方依次刷新
	 **/
	all_app_traversal_increment_from(&iter, iter.app_index + 1, _do_flush_window, area);
	return 0;
}

si_t flush_above_in_z_order_including(struct application_info * app_info_ptr, struct window_info * win_info_ptr, struct rectangle* area)
{
	struct window_info_iterator iter;
	window_info_iterator_clear(&iter);

	if(app_info_ptr == NULL || win_info_ptr == NULL)
	{
		return 0;
	}

	/**
	 * 顺序遍历所有应用程序的所有顶层窗口数组的所有窗口树的所有窗口直到找到某个窗口为止
	 **/ 
	all_app_traversal_increment(&iter, _do_find_window, win_info_ptr);

	/**
	 * 从窗口所在的下一个窗口开始 遍历这棵窗口树
	 * 将每个窗口与该区域交集的地方依次刷新
	 **/
	win_tree_traversal_increment_from(&iter, iter.win_info_ptr, _do_flush_window, area);

	/**
	 * 从窗口所在的下一个顶层窗口开始 遍历这个应用程序
	 * 将每个窗口与该区域交集的地方依次刷新
	 **/
	app_traversal_increment_from(&iter, iter.win_index + 1, _do_flush_window, area);

	/**
	 * 从窗口所在的下一个应用程序开始 遍历所有应用程序
	 * 将每个窗口与该区域交集的地方依次刷新
	 **/
	all_app_traversal_increment_from(&iter, iter.app_index + 1, _do_flush_window, area);
	return 0;
}

si_t flush_in_z_order(si_t x, si_t y, si_t width, si_t height)
{
	struct window_info_iterator iter;
	struct rectangle temp_area = {0};
	window_info_iterator_clear(&iter);

	rectangle_set(&temp_area, x, y, width, height);

	paint_desktop(&temp_area);

	/**
	 * 顺序遍历所有应用程序的所有顶层窗口数组的所有窗口树的所有窗口，
	 * 将每个窗口与该区域交集的地方依次刷新
	 **/
	all_app_traversal_increment(&iter, _do_flush_window, &temp_area);

	/* 将缓冲区的内容复制到存储区 */
	screen_flush
		(x,
		 y,
		 width,
		 height);

	/* 更新光标 */
	cursor_paint();
	return 0;
}
