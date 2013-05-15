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

# ifndef _CURSOR_H_
# define _CURSOR_H_ 1

# include <limits.h>
# include "base_type.h"

#if 0 /** commented by padme 2012-05-11 {{{ **/
enum __cursor_shape__
{
    /** X **/
    CURSOR_SHAPE_X = 0,

    /** 箭头 **/
    CURSOR_SHAPE_ARROW,

    /** 手 **/
    CURSOR_SHAPE_HAND,

    /** 输入文本 **/
    CURSOR_SHAPE_TEXT_INPUT,

    /** 选择文本 **/
    CURSOR_SHAPE_TEXT_SELECT,

    /** 水平调整 **/
    CURSOR_SHAPE_H_RESIZE,

    /** 竖直调整 **/
    CURSOR_SHAPE_V_RESIZE

    /** 对角线调整 **/

    /** 对角线调整 **/
};
#endif /** commented by padme 2012-05-11 }}} **/


/**
 * 初始化光标
**/
extern si_t cursor_init();


/**
 * 绘制光标
**/
extern si_t cursor_paint();

extern si_t cursor_show();

/**
 * 清理光标
**/
extern si_t cursor_exit();

# endif
