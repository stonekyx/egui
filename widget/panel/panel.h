/**
 * @file panel.h
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

# ifndef _PANEL_H_
# define _PANEL_H_ 1

# include <base_type.h>
# include <comm.h>

# include "../widget/widget.h"

/**
 * 窗口部件结构体
**/
# define PANEL_DEFINITION \
    /**
     * 从 widget 继承
    **/ \
    WIDGET_DEFINITION

struct panel
{
    PANEL_DEFINITION
	/*
	 struct vector widget_vector;
	*/
};

/* 按钮样式结构体 */
struct panel_style
{
    /* 一个标志 */
    si_t flag;

    /* 工作区域 */
    si_t area_x;
    si_t area_y;
    si_t area_width;
    si_t area_height;

    /* 边界宽度 */
    si_t border_size;

    /* 高度&宽度 */
    si_t maximum_width;
    si_t minimum_width;
    si_t maximum_height;
    si_t minimum_height;

    /* 鼠标形状 */
    si_t cursor;

    /* 背景色 */
    si_t back_color_r;
    si_t back_color_g;
    si_t back_color_b;
    si_t back_color_a;

    /* 前景色 */
    si_t fore_color_r;
    si_t fore_color_g;
    si_t fore_color_b;
    si_t fore_color_a;
};

/**
 * 将地址强制转换成 struct panel *
**/
# define PANEL_POINTER(addr) ((struct panel *)(addr))

extern si_t panel_default_callback(void* self , void* msg );

/**
 * @brief 使用前,设置窗口部件结构体
 *
 * @details
 *
 * @param id 窗口部件的标识符
 *
 * @return 0
**/
extern void * panel_init(si_t id);


/**
 * @brief 使用后清理窗口部件结构体
 *
 * @details
 *
 * @param i 窗口部件结构体指针
 *
 * @return 0
**/
extern si_t panel_exit (struct panel * ic);

/**
 * 绘制窗口部件
 *
 * @param i 窗口部件的地址
 *
 * @return 0
**/


extern si_t panel_repaint(struct panel * i);
extern si_t panel_default_widget_repaint(struct panel* ic , union message * msg);


extern si_t panel_show(struct panel * i);
extern si_t panel_default_widget_show(struct panel* ic , union message * msg);



extern si_t panel_default_mouse_press(struct panel* ic , union message * msg);



extern si_t panel_default_mouse_release(struct panel* ic , union message * msg);

extern void panel_set_bounds(struct panel* panel, si_t x, si_t y, si_t width , si_t height);

# endif
