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

# ifndef _MANIPULATE_WINDOW_H_
# define _MANIPULATE_WINDOW_H_ 1

# include "widget.h"

/**
 * 设置用户应用程序的主窗口
 *
 * 用户可以用该函数指定对话框或者消息框作为主窗口
 *
 * @param window 主窗口指针
 *
 * @return 0
**/
extern si_t application_set_main_window(struct window * window);

/**
 * 用户应用程序添加顶层窗口或者孩子窗口
 *
 * parent == NULL 时，添加顶层窗口；parent != NULL 时，添加孩子窗口。
 * 当添加的是孩子窗口时，孩子窗口只能在父窗口的区域内显示。
 * 对于窗口、对话框、消息框都适用。
 * window 不能是主窗口。
 *
 * @param parent 父窗口指针
 * @param window 窗口指针
 *
 * @return 0
**/
extern si_t application_add_window(struct window * parent, struct window * window);

/**
 * 用户应用程序删除顶层窗口或者孩子窗口
 *
 * 这个部件以下的所有的后代部件将会被删除，包括窗口、对话框、消息框。
 * 如果主窗口被删除，那么其它所有的窗口都会被删除。
 * 此时用户应用程序将退出
 *
 * 对于窗口、对话框、消息框都适用
 *
 * @param window 窗口指针
 *
 * @return 0
**/
extern si_t application_del_window(struct window * window);

# endif
