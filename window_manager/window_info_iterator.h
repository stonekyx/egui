/**
 * @file window_info_iterator.h
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

#ifndef WINDOW_INFO_ITERATOR_H
#define WINDOW_INFO_ITERATOR_H 1

#include "window_info.h"
#include "application_info.h"

/**
 * 迭代器 用来遍历窗口管理器的所有应用程序下的所有顶层窗口下的窗口树上的所有窗口
 **/
struct window_info_iterator
{
	/* 当前应用程序指针 */
	struct application_info* app_info_ptr;
	/* 当前顶层窗口指针 */
	struct window_info* top_win_info_ptr;
	/* 当前窗口指针 */
	struct window_info* win_info_ptr;
	/* 当前应用程序所在的应用程序数组下标 */
	si_t app_index;
	/* 当前顶层窗口所在的顶层窗口数组下标 */
	si_t win_index;
};

/* 请空迭代器 */
extern void window_info_iterator_clear(struct window_info_iterator* iter);

/**
 * 从某个窗口开始顺序（从左向右）遍历一棵窗口树
 *
 * @param iter 迭代器
 * @param from 从哪个窗口开始
 * @param action 对每个窗口调用的操作，返回0则继续，返回非0值则中断遍历
 * @param arg action的额外参数
 *
 * @return 返回0则说明遍历了所有的窗口 否则就是当前窗口返回非0值后中断变量
 **/
extern si_t win_tree_traversal_increment_from(struct window_info_iterator* iter, struct window_info* from, si_t(*action)(struct window_info_iterator*, void*), void* arg);

/**
 * 顺序（从左向右）遍历一棵窗口树
 *
 * @param iter 迭代器
 * @param action 对每个窗口调用的操作，返回0则继续，返回非0值则中断遍历
 * @param arg action的额外参数
 *
 * @return 返回0则说明遍历了所有的窗口 否则就是当前窗口返回非0值后中断变量
 **/
extern si_t win_tree_traversal_increment(struct window_info_iterator* iter, si_t(*action)(struct window_info_iterator*, void*), void* arg);

/**
 * 从某个顶层窗口开始顺序遍历一个应用程序（的所有窗口）
 *
 * @param iter 迭代器
 * @param from 从哪个顶层窗口开始
 * @param action 对每个窗口调用的操作，返回0则继续，返回非0值则中断遍历
 * @param arg action的额外参数
 *
 * @return 返回0则说明遍历了所有的窗口 否则就是当前窗口返回非0值后中断变量
 **/
extern si_t app_traversal_increment_from(struct window_info_iterator* iter, si_t from_index, si_t(*action)(struct window_info_iterator*, void*), void* arg);

/**
 * 顺序遍历一个应用程序（的所有窗口）
 *
 * @param iter 迭代器
 * @param action 对每个窗口调用的操作，返回0则继续，返回非0值则中断遍历
 * @param arg action的额外参数
 *
 * @return 返回0则说明遍历了所有的窗口 否则就是当前窗口返回非0值后中断变量
 **/
extern si_t app_traversal_increment(struct window_info_iterator* iter, si_t(*action)(struct window_info_iterator*, void*), void* arg);

/**
 * 从某个应用程序开始顺序遍历所有应用程序（的所有窗口）
 *
 * @param iter 迭代器
 * @param from 从哪个应用程序开始
 * @param action 对每个窗口调用的操作，返回0则继续，返回非0值则中断遍历
 * @param arg action的额外参数
 *
 * @return 返回0则说明遍历了所有的窗口 否则就是当前窗口返回非0值后中断变量
 **/
extern si_t all_app_traversal_increment_from(struct window_info_iterator* iter, si_t from, si_t(*action)(struct window_info_iterator*, void*), void* arg);

/**
 * 顺序遍历所有应用程序（的所有窗口）
 *
 * @param iter 迭代器
 * @param action 对每个窗口调用的操作，返回0则继续，返回非0值则中断遍历
 * @param arg action的额外参数
 *
 * @return 返回0则说明遍历了所有的窗口 否则就是当前窗口返回非0值后中断变量
 **/
extern si_t all_app_traversal_increment(struct window_info_iterator* iter, si_t(*action)(struct window_info_iterator*, void*), void* arg);

/**
 * 逆序遍历所有应用程序（的所有窗口）
 *
 * @param iter 迭代器
 * @param action 对每个窗口调用的操作，返回0则继续，返回非0值则中断遍历
 * @param arg action的额外参数
 *
 * @return 返回0则说明遍历了所有的窗口 否则就是当前窗口返回非0值后中断变量
 **/
extern si_t all_app_traversal_decrement(struct window_info_iterator* iter, si_t(*action)(struct window_info_iterator*, void*), void* arg);

/**
 * 逆序遍历所有应用程序（的所有*顶层*窗口）
 *
 * @param iter 迭代器
 * @param action 对每个窗口调用的操作，返回0则继续，返回非0值则中断遍历
 * @param arg action的额外参数
 *
 * @return 返回0则说明遍历了所有的窗口 否则就是当前窗口返回非0值后中断变量
 **/
extern si_t all_app_traversal_top_window_decrement(struct window_info_iterator* iter, si_t(*action)(struct window_info_iterator*, void*), void* arg);

#endif

