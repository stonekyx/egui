/**
 * @file respond.h
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

# ifndef _RESPOND_H_
# define _RESPOND_H_ 1

# include <limits.h>

# include "color.h"
# include "geometry.h"

enum __respond_type__
{
    /** 用户应用程序初始化 **/
    RESPOND_TYPE_REGISTER_APPLICATION = 0,
	/** 清理用户应用程序 **/
    RESPOND_TYPE_CANCEL_APPLICATION, 
    
	/** 申请图形设备 **/
    RESPOND_TYPE_GRAPHICS_DEVICE_INIT,    
	/** 释放图形设备 **/
    RESPOND_TYPE_GRAPHICS_DEVICE_EXIT,

    /** 关闭窗口管理程序 **/
    RESPOND_TYPE_WINDOW_MANAGER_QUIT,

    /** 更新屏幕的某个区域 **/
    RESPOND_TYPE_UPDATE,
    RESPOND_TYPE_DESKTOP_DIRTY,

    /** 注册窗口 **/
    RESPOND_TYPE_REGISTER_WINDOW,
    /** 移动窗口 **/
    RESPOND_TYPE_MOVE_WINDOW,
    /** 改变窗口大小 **/
    RESPOND_TYPE_RESIZE_WINDOW,
    /** 最小化窗口 **/
    RESPOND_TYPE_MINIMIZE_WINDOW,
    /** 最大化窗口 **/
    RESPOND_TYPE_MAXIMIZE_WINDOW,
    /** 还原窗口 **/
    RESPOND_TYPE_RESTORE_WINDOW,
    /** 注销窗口 **/
    RESPOND_TYPE_CANCEL_WINDOW,
	/** 激活窗口 **/
	RESPOND_TYPE_ACTIVATE_WINDOW,


    /** 清空屏幕 **/
    RESPOND_TYPE_CLEAR,
    /** 清白 **/
    RESPOND_TYPE_CLEAR_WHITE,
    /** 清黑 **/
    RESPOND_TYPE_CLEAR_BLACK,

   
	/** 绘制直线 **/
    RESPOND_TYPE_DRAW_LINE,
    /** 绘制点 **/
    RESPOND_TYPE_DRAW_POINT,
    /** 绘制矩形 **/
    RESPOND_TYPE_DRAW_RECTANGLE,
    /** 绘制多边形 **/
    RESPOND_TYPE_DRAW_POLYGON,
    /** 绘制圆 **/
    RESPOND_TYPE_DRAW_CIRCLE,
    /** 绘制椭圆 **/
    RESPOND_TYPE_DRAW_ELLIPSE,
    /** 绘制箭头 **/
    RESPOND_TYPE_DRAW_ARROW,
    /** 绘制叉 **/
    RESPOND_TYPE_DRAW_X,
    /** draw bitmap **/
    RESPOND_TYPE_DRAW_BITMAP,
	/* 绘图 */
	RESPOND_TYPE_DRAW_IMG,

    /** 填充矩形 **/
    RESPOND_TYPE_FILL_RECTANGLE,
    /** 填充圆 **/
    RESPOND_TYPE_FILL_CIRCLE,
    /** 填充椭圆 **/
    RESPOND_TYPE_FILL_ELLIPSE,
    /** 填充箭头 **/
    RESPOND_TYPE_FILL_ARROW,
    /** 填充叉 **/
    RESPOND_TYPE_FILL_X,

    /** 显示文字 **/
    RESPOND_TYPE_SHOW_TEXT,

    /** 复制区域 **/
    RESPOND_TYPE_CPY_AREA,
 
    /** 设置图新设备颜色 **/
    RESPOND_TYPE_SET_COLOR,
    /** 设置图新设备区域 **/
    RESPOND_TYPE_SET_AREA,
    /** 设置图新设备字体 **/
    RESPOND_TYPE_SET_FONT,

    /** 获得颜色的限制 **/
    RESPOND_TYPE_GET_COLOR_LIMIT,
    /** 获得屏幕宽度 **/
    RESPOND_TYPE_GET_SCREEN_WIDTH,
    /** 获得屏幕高度 **/
    RESPOND_TYPE_GET_SCREEN_HEIGHT,
    /** 获得图形设备工作区域 **/
    RESPOND_TYPE_GET_AREA,
    /** 获得图形设备的颜色 **/
    RESPOND_TYPE_GET_COLOR,
    /** 获得文字宽度 **/
    RESPOND_TYPE_GET_FONT_WIDTH,
    /** 获得文字高度 **/
    RESPOND_TYPE_GET_FONT_HEIGHT,

	/** 获得窗口信息 **/
	RESPOND_TYPE_GET_WINDOW_TITLE,

    /** 回应种类的数目 **/
    RESPOND_TYPE_QUANTITY,

    /** 回应种类的最大数目 **/
    /** ISO C restricts enumerator values to range of 'int' **/
    RESPOND_TYPE_MAX = INT_MAX
};

/**
 * 将respond_type转换为字符串，便于打印日志信息
 **/
extern char* respond_type_to_str(si_t respond_type);

#define RESPOND_BASE_DEFINATION \
	si_t body_len; \
	si_t type; \
	si_t respond_data_type;

enum __respond_data_type__
{
    /** 固定大小的回**/
	RESPOND_DATA_TYPE_FIX = 0,
    /** 变长的回应 **/
	RESPOND_DATA_TYPE_VARIABLE,
    /** 回应种类的数目 **/
	RESPOND_DATA_TYPE_QUANTITY,
    /** 回应种类的最大数目 **/
    /** ISO C restricts enumerator values to range of 'int' **/
	RESPOND_DATA_TYPE_MAX = INT_MAX
};

/**
 * 回应一般是一个si_t的返回值，表示实际调用函数的返回值
 * 有的时候会返回struct rectange(如get_area)，struct color(如get_color)，struct color_limit(如get_color_limit)
 **/
union respond;

/**
 * 多数情况下返回执行情况，一个si_t
 **/
struct respond_body_normal
{
	RESPOND_BASE_DEFINATION
	si_t retval;
};
extern void respond_set_normal(union respond* respond_ptr, si_t type, si_t retval);

/**
 * 有时回返回一个区域，如maximize_window
 **/
struct respond_body_rectangle
{
	RESPOND_BASE_DEFINATION
	struct rectangle retval;
};
extern void respond_set_rectangle(union respond* respond_ptr, si_t type, struct rectangle* retval);

/**
 * 有时回返回颜色最大值，如get_color_limit
 **/
struct respond_body_color_limit
{
	RESPOND_BASE_DEFINATION
	struct color_limit retval;
};
extern void respond_set_color_limit(union respond* respond_ptr, si_t type, struct color_limit* retval);

/**
 * 有时会返回一个颜色，如get_color
 **/
struct respond_body_color
{
	RESPOND_BASE_DEFINATION
	struct color retval;
};
extern void respond_set_color(union respond* respond_ptr, si_t type, struct color* retval);

/**
 * 有时返回不定长的非临时变量 例如get_window_title返回窗口标题
 **/
struct respond_body_variable
{
	RESPOND_BASE_DEFINATION
	addr_t retval;
};
extern void respond_set_va(union respond* respond_ptr, si_t type, addr_t va_arg, si_t len);

union respond
{
	struct respond_body_normal normal;
	struct respond_body_rectangle rectangle;
	struct respond_body_color_limit color_limit;
	struct respond_body_color color;
	struct respond_body_variable va;
};
extern void respond_set_empty(union respond* respond_ptr);
extern si_t respond_get_type(union respond* respond_ptr);
extern si_t respond_get_len(union respond* respond_ptr);
extern addr_t respond_get_value(union respond* respond_ptr);

# endif

