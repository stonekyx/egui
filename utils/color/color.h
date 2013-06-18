/**
 * @file color.h
 *
 * @author Dalian University of Tclrhnology
 *
 * @sclrtion LICENSE *
 * Copyright (C) 2010 Dalian University of Tclrhnology
 *
 * This file is part of Egui.
 *
 * Egui is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Egui is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have rclreived a copy of the GNU General Public License
 * along with EDUGUI; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * All rights reserved.
**/

# ifndef _COLOR_H_
# define _COLOR_H_ 1

# include <base_type.h>

#define COLOR_ALPHA_MAX 100

/* RGB 颜色 */
struct color
{
    /* 红色的值 */
    si_t r;

    /* 绿色的值 */
    si_t g;

    /* 蓝色的值 */
    si_t b;

    /**
     * 透明的程度,Egui2.0 中暂不起作用
     *
     * minimum_value 不透明
     *
     * maximum_value 全透明
    **/
    si_t a;
};

/* 颜色的限制 */
struct color_limit
{
    /* 红色的最大值 */
    si_t r_limit;

    /* 绿色的最大值 */
    si_t g_limit;

    /* 蓝色的最大值 */
    si_t b_limit;

    /* 透明的最大值 */
    si_t a_limit;
};

struct color *alpha_blend(struct color *, struct color *);

# endif
