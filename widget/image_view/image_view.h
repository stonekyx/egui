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

# ifndef _IMAGE_VIEW_H_
# define _IMAGE_VIEW_H_ 1

# include <base_type.h>
# include <comm.h>

# include "../widget/widget.h"

/**
 * 窗口部件结构体
**/
# define IMAGE_VIEW_DEFINITION \
    /**
     * 从 widget 继承
    **/ \
    WIDGET_DEFINITION \
    \
    /**
     * path of the picture
    **/ \
    char * path; \
    \
    /**
     * align
     **/ \
    int align;

struct image_view
{
    IMAGE_VIEW_DEFINITION
};

/* image_view样式结构体 */
struct image_view_style
{
    struct widget_style common;
};

/**
 * 将地址强制转换成 struct image_view*
**/
# define IMAGE_VIEW_POINTER(addr) ((struct image_view*)(addr))

/**
 * @brief image_view的默认回调函数
 * 处理重绘事件，显示事件
 *
 * @param self
 * @param msg
 *
 * @return 0
 **/
extern si_t image_view_default_callback(addr_t self, addr_t msg);

/**
 * @brief 使用前设置窗口部件结构体
 *
 * @param path 图片位置
 *
 * @return 0
**/
extern struct image_view* image_view_init(char* path);

/**
 * @brief 使用后清理窗口部件结构体
 *
 * @param b 窗口部件结构体指针
 *
 * @return 0
**/
extern si_t image_view_exit(struct image_view * i);

/**
 * @brief 设置image_view的位置
 * 需要保留原值的将对应参数设为负数
 *
 * @param i image_view控件指针
 * @param x 相对父控件的横坐标
 * @param y 相对父控件的纵坐标
 * @param width 宽度
 * @param height 长度
 **/
extern void image_view_set_bounds(struct image_view* i, si_t x, si_t y, si_t width, si_t height);

/**
 * @brief 设置image_view的颜色
 * 需要保留原值的将对应参数指针设置为空
 *
 * @param i 控件
 * @param fcolor 前景色
 * @param bcolor 背景色
 **/
extern void image_view_set_color(struct image_view* i, struct color* fcolor, struct color* bcolor);

#endif
