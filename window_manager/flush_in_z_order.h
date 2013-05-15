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

# ifndef _FLUSH_IN_Z_ORDER_H_
# define _FLUSH_IN_Z_ORDER_H_ 1

# include <base_type.h>

# include "window_manager.h"

/**
 * 按照 Z 序刷新一个用户应用程序的所有窗口
 *
 * @param app_info_ptr
 * @param area 更新区域
 *
 * @return 0
**/
extern si_t flush_window_in_z_order(struct application_info * app_info_ptr, struct rectangle* area);

/**
 * 按照 Z 序更新用户应用程序某个窗口以下的所有窗口
 *
 * 更新在 app_info_ptr 中 win_info_ptr 下面的所有窗口
 *
 * @param app_info_ptr
 * @param win_info_ptr
 * @param area 更新区域
 *
 * @return 0
**/
extern si_t flush_below_in_z_order(struct application_info * app_info_ptr, struct window_info * win_info_ptr, struct rectangle* area);

/**
 * 按照 Z 序更新用户应用程序某个窗口以上的所有窗口
 *
 * 更新在 app_info_ptr 中 win_info_ptr 上面的所有窗口
 *
 * @param app_info_ptr
 * @param win_info_ptr
 * @param area 更新区域
 *
 * @return 0
**/
extern si_t flush_above_in_z_order(struct application_info * app_info_ptr, struct window_info * win_info_ptr, struct rectangle* area);

extern si_t flush_above_in_z_order_including(struct application_info * app_info_ptr, struct window_info * win_info_ptr, struct rectangle* area);

/**
 * 按照 Z 序更新所有用户应用程序的窗口
 *
 * @param x 要更新区域左上角的横坐标
 * @param y 要更新区域左上角的纵坐标
 * @param width 要更新区域的宽度
 * @param height 要更新区域的高度
 *
 * @return 0
**/
extern si_t flush_in_z_order(si_t x, si_t y, si_t width, si_t height);

# endif
