/**
 * @file flowbox.h
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

# ifndef _FLOWBOX_H_
# define _FLOWBOX_H_ 1

# include <base_type.h>
# include <comm.h>

# include "../widget/widget.h"

struct flowbox
{
    WIDGET_DEFINITION
    si_t used_width;
    si_t used_height;
    si_t max_height_of_row;
    si_t seam;
};

/**
 * 将地址强制转换成 struct flowbox *
 **/
# define FLOWBOX_POINTER(addr) ((struct flowbox *)(addr))

/**
 * @brief 使用前设置窗口部件结构体
 *
 * @details
 *
 * @param text 显示在flowbox上的内容
 *
 * @return 0
**/
extern struct flowbox* flowbox_init(si_t id);

/**
 * @brief 使用后清理窗口部件结构体
 *
 * @details
 *
 * @param b 窗口部件结构体指针
 *
 * @return 0
**/
extern si_t flowbox_exit(struct flowbox * b);

void flowbox_set_bounds(struct flowbox * b, si_t x, si_t y, si_t width , si_t height);

si_t flowbox_add_widget(struct flowbox *f, struct widget *w);

# endif
