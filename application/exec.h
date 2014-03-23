/**
 * @file exec.h
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

# ifndef _EXEC_H_
# define _EXEC_H_ 1

/* We only need the `widget' definition here, not all the widgets. */
# include "../widget/widget/widget.h"

# include "application.h"

extern si_t application_event_listener_handler(struct egui_uds* uds_ptr, addr_t arg);

extern struct widget* application_widgets_for_each_decreament_public(struct widget*(*do_for_each_widget)(struct widget*, union message*), union message* msg);

/**
 * @brief 获得窗口部件的绝对坐标区域
 *
 * @param w 窗口部件结构体指针
 * @param result 存放绝对区域的地址
 *
 * @return 成功返回 0，失败返回 -1。
**/
extern si_t application_widget_absolute_area (struct widget * w, struct rectangle * result);
# endif
