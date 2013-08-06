/**
 * @file request.h
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

# ifndef _REQUEST_H_
# define _REQUEST_H_ 1

# include "base_type.h"
# include "geometry.h"
# include "color.h"
# include <limits.h>

enum __request_type__
{
    /** 用户应用程序初始化 **/
    REQUEST_TYPE_REGISTER_APPLICATION = 0,
	/** 清理用户应用程序 **/
    REQUEST_TYPE_CANCEL_APPLICATION, 
    
	/** 申请图形设备 **/
    REQUEST_TYPE_GRAPHICS_DEVICE_INIT,    
	/** 释放图形设备 **/
    REQUEST_TYPE_GRAPHICS_DEVICE_EXIT,

    /** 关闭窗口管理程序 **/
    REQUEST_TYPE_WINDOW_MANAGER_QUIT,

    /** 更新屏幕的某个区域 **/
    REQUEST_TYPE_UPDATE,
    REQUEST_TYPE_DESKTOP_DIRTY,

    /** 注册窗口 **/
    REQUEST_TYPE_REGISTER_WINDOW,
    /** 注销窗口 **/
    REQUEST_TYPE_CANCEL_WINDOW,
    /** 移动窗口 **/
    REQUEST_TYPE_MOVE_WINDOW,
    /** 改变窗口大小 **/
    REQUEST_TYPE_RESIZE_WINDOW,
    /** 最小化窗口 **/
    REQUEST_TYPE_MINIMIZE_WINDOW,
    /** 最大化窗口 **/
    REQUEST_TYPE_MAXIMIZE_WINDOW,
    /** 还原窗口 **/
    REQUEST_TYPE_RESTORE_WINDOW,
	/** 激活窗口 **/
	REQUEST_TYPE_ACTIVATE_WINDOW,


    /** 清空屏幕 **/
    REQUEST_TYPE_CLEAR,
    /** 清白 **/
    REQUEST_TYPE_CLEAR_WHITE,
    /** 清黑 **/
    REQUEST_TYPE_CLEAR_BLACK,

   
	/** 绘制直线 **/
    REQUEST_TYPE_DRAW_LINE,
    /** 绘制点 **/
    REQUEST_TYPE_DRAW_POINT,
    /** 绘制矩形 **/
    REQUEST_TYPE_DRAW_RECTANGLE,
    /** 绘制多边形 **/
    REQUEST_TYPE_DRAW_POLYGON,
    /** 绘制圆 **/
    REQUEST_TYPE_DRAW_CIRCLE,
    /** 绘制椭圆 **/
    REQUEST_TYPE_DRAW_ELLIPSE,
    /** 绘制箭头 **/
    REQUEST_TYPE_DRAW_ARROW,
    /** 绘制叉 **/
    REQUEST_TYPE_DRAW_X,
    /** draw bitmap **/
    REQUEST_TYPE_DRAW_BITMAP,
	/* 绘图 */
	REQUEST_TYPE_DRAW_IMG,

    /** 填充矩形 **/
    REQUEST_TYPE_FILL_RECTANGLE,
    /** 填充圆 **/
    REQUEST_TYPE_FILL_CIRCLE,
    /** 填充椭圆 **/
    REQUEST_TYPE_FILL_ELLIPSE,
    /** 填充箭头 **/
    REQUEST_TYPE_FILL_ARROW,
    /** 填充叉 **/
    REQUEST_TYPE_FILL_X,

    /** 显示文字 **/
    REQUEST_TYPE_SHOW_TEXT,

    /** 复制区域 **/
    REQUEST_TYPE_CPY_AREA,
 
    /** 设置图新设备颜色 **/
    REQUEST_TYPE_SET_COLOR,
    /** 设置图新设备区域 **/
    REQUEST_TYPE_SET_AREA,
    /** 设置图新设备字体 **/
    REQUEST_TYPE_SET_FONT,

    /** 获得颜色的限制 **/
    REQUEST_TYPE_GET_COLOR_LIMIT,
    /** 获得屏幕宽度 **/
    REQUEST_TYPE_GET_SCREEN_WIDTH,
    /** 获得屏幕高度 **/
    REQUEST_TYPE_GET_SCREEN_HEIGHT,
    /** 获得图形设备工作区域 **/
    REQUEST_TYPE_GET_AREA,
    /** 获得图形设备的颜色 **/
    REQUEST_TYPE_GET_COLOR,
    /** 获得文字宽度 **/
    REQUEST_TYPE_GET_FONT_WIDTH,
    /** 获得文字高度 **/
    REQUEST_TYPE_GET_FONT_HEIGHT,

	/** 获得窗口信息 **/
	REQUEST_TYPE_GET_WINDOW_TITLE,

    /** 请求种类的数目 **/
    REQUEST_TYPE_QUANTITY,

    /** 请求种类的最大数目 **/
    /** ISO C restricts enumerator values to range of 'int' **/
    REQUEST_TYPE_MAX = INT_MAX
};

/**
 * 把request_type转换成字符串便于打印日志信息
 **/
extern char* request_type_to_str(si_t request_type);
/**
 * 以下为client发送给server的各种request的标准负载以及对应的getter和setter
 * 主要负载声明为一个个结构体packet_body_请求类型
 * 而负载的成员变量均为si_t或指针，即长度均为一个字长
 * 这样读写速度都可以加快，并且可以用tricky的方法(va_arg)快速读取
 * 下面的setter和getter,call_by分别被client_lib和server_lib调用
 **/

/**
 * 需要窗口管理器处理的请求
 * setter是客户端发送时包装使用的
 * getter是窗口管理器收到后读取内容传递给上层作为参数
 * 
 * graph_device_init/exit需要先掉用graph模块的engine_graph_device_init/exit
 * 然后再交给上层处理
 * 因此有call_by函数用于直接将负载内容作为函数参数调用
 **/
/**
 * 注册应用程序
 **/
struct packet_body_register_application
{
	si_t video_access_mode;
	si_t application_type;
	char* name;
	si_t length;
};

enum APPLICATION_TYPE
{
	APPLICATION_TYPE_NORMAL = 1 << 1,
	APPLICATION_TYPE_DESKTOP = 1 << 2,
	APPLICATION_TYPE_WINDOW_MANAGER_EXIT = 1 << 3
};
extern void request_set_register_application(struct packet_body_register_application* body, si_t video_access_mode, si_t application_type, const char* name, si_t length);
extern void request_get_register_application(addr_t arg, si_t* video_access_mode, si_t* application_type, char** name, si_t* length);

/**
 * 初始化图形设备的请求
 **/
struct packet_body_graphics_device_init
{
	si_t x_axis;
	si_t y_axis;
	si_t x_size;
	si_t y_size;
	si_t r;
	si_t g;
	si_t b;
	si_t a;
	si_t font;
};
extern void request_set_graphics_device_init(struct packet_body_graphics_device_init* body, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size, si_t r, si_t g, si_t b, si_t a, si_t font);
extern si_t request_call_by_graphics_device_init(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t, si_t, si_t, si_t, si_t));

/**
 * 清理图形设备的请求
 **/
struct packet_body_graphics_device_exit
{
	si_t gd;
};
extern void request_set_graphics_device_exit(struct packet_body_graphics_device_exit* body, si_t gd);
extern void request_get_graphics_device_exit(addr_t body, si_t* gd);
extern si_t request_call_by_graphics_device_exit(addr_t arg, si_t (*callable)(si_t));

/**
 * 更新图形设备区域请求
 **/
struct packet_body_update
{
    si_t gd;
};
extern void request_set_update(struct packet_body_update* body, si_t gd);
extern void request_get_update(addr_t body, si_t* gd);

/**
 * 注册窗口的请求
 **/
struct packet_body_register_window
{
    si_t parent_window_descriptor;
    char* title;
	si_t title_length;
    si_t x;
    si_t y;
    si_t width;
    si_t height;
    si_t minimize_enable;
    si_t maximize_enable;
    si_t modal;
};
extern void request_set_register_window(struct packet_body_register_window* body, si_t parent_window_descriptor, const char* title, si_t title_length, si_t x, si_t y, si_t width, si_t height, si_t minimize_enable, si_t maximize_enable, si_t modal);
extern void request_get_register_window(addr_t body, si_t* parent_window_descriptor, char** title, si_t* title_length, si_t* x, si_t* y, si_t* width, si_t* height, si_t* minimize_enable, si_t* maximize_enable, si_t* modal);

/**
 * 取消窗口的请求
**/
struct packet_body_cancel_window
{
	si_t window_id;
};
extern void request_set_cancel_window(struct packet_body_cancel_window* body, si_t window_id);
extern void request_get_cancel_window(addr_t body, si_t* window_id);

/**
 * 移动窗口的请求
 **/
struct packet_body_move_window
{
	si_t window_id;
	si_t x_axis;
	si_t y_axis;
};
extern void request_set_move_window(struct packet_body_move_window* body, si_t window_id, si_t x_axis, si_t y_axis);
extern void request_get_move_window(addr_t body, si_t* window_id, si_t* x_axis, si_t* y_axis);

/**
 * 调整窗口大小的请求
 **/
struct packet_body_resize_window
{
    si_t window_id;
    si_t x_axis;
    si_t y_axis;
    si_t x_size;
    si_t y_size;
};
extern void request_set_resize_window(struct packet_body_resize_window* body, si_t window_id, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size);
extern void request_get_resize_window(addr_t body, si_t* window_id, si_t* x_axis, si_t* y_axis, si_t* x_size, si_t* y_size);

/**
 * 最大化窗口的请求
 **/
struct packet_body_maximize_window
{
	si_t window_id;
};
extern void request_set_maximize_window(struct packet_body_maximize_window* body, si_t window_id);
extern void request_get_maximize_window(addr_t body, si_t* window_id);

/**
 * 最小化窗口的请求
 **/
struct packet_body_minimize_window
{
	si_t window_id;
};
extern void request_set_minimize_window(struct packet_body_minimize_window* body, si_t window_id);
extern void request_get_minimize_window(addr_t body, si_t* window_id);

/**
 * 还原窗口的请求
 **/
struct packet_body_restore_window
{
    si_t window_id;
    si_t x;
    si_t y;
    si_t width;
    si_t height;
};
extern void request_set_restore_window(struct packet_body_restore_window* body, si_t window_id, si_t x, si_t y, si_t width, si_t height);
extern void request_get_restore_window(addr_t body, si_t* window_id, si_t* x, si_t* y, si_t* width, si_t* height);

/**
 * 激活窗口的请求（只有桌面应用程序有)
 **/
struct packet_body_activate_window
{
	si_t window_id;
};
extern void request_set_activate_window(struct packet_body_activate_window* body, si_t window_id);
extern void request_get_activate_window(addr_t body, si_t* window_id);

/**
 * 直接调用graph模块的函数，不需要窗口管理器处理的请求
 * 客户端将参数打包发给服务器端，
 * 服务器将参数一个个放入实际调用的图形模块的函数，
 * 然后将函数返回值打包发回客户端，
 * 客户端函数返回回应值
 *
 * setter是客户端发送时包装使用的
 * call_by函数用于直接将负载内容作为参数调用
 **/
/**
 * 清屏请求
 **/
struct packet_body_clear
{
	si_t gd;
};
extern void request_set_clear(struct packet_body_clear* body, si_t gd);
extern si_t request_call_by_clear(addr_t arg, si_t (*callable)(si_t));

/**
 * 清屏请求，颜色为黑
 **/
struct packet_body_clear_black
{
	si_t gd;
};
extern void request_set_clear_black(struct packet_body_clear_black* body, si_t gd);
extern si_t request_call_by_clear_black(addr_t arg, si_t (* callable)(si_t));

/**
 * 清屏请求，颜色为白
 **/
struct packet_body_clear_white
{
	si_t gd;
};
extern void request_set_clear_white(struct packet_body_clear_white* body, si_t gd);
extern si_t request_call_by_clear_white(addr_t arg, si_t (*callable)(si_t));

/**
 * 拷贝区域请求
 **/
struct packet_body_cpy_area
{
    si_t gd;
    si_t mode;
    si_t dst_x;
    si_t dst_y;
    si_t src_x;
    si_t src_y;
    si_t x_size;
    si_t y_size;
};
extern void request_set_cpy_area(struct packet_body_cpy_area* body, si_t gd, si_t mode, si_t dst_x, si_t dst_y, si_t src_x, si_t src_y, si_t x_size, si_t y_size);
extern si_t request_call_by_cpy_area(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t, si_t, si_t, si_t));

/**
 * 绘制箭头请求
 **/
struct packet_body_draw_arrow
{
    si_t gd;
    si_t x;
    si_t y;
    si_t size;
    si_t direction;
};
extern void request_set_draw_arrow(struct packet_body_draw_arrow* body, si_t gd, si_t x, si_t y, si_t size, si_t direction);
extern si_t request_call_by_draw_arrow(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t));

/**
 * 绘制bmp图请求
 **/
struct packet_body_draw_bitmap
{
    si_t gd;
    char* name;
	si_t length;
    si_t align;
};
extern void request_set_draw_bitmap(struct packet_body_draw_bitmap* body, si_t gd, const char* name, si_t length, si_t align);
extern si_t request_call_by_draw_bitmap(addr_t arg, si_t (*callable)(si_t, char*, si_t));

/**
 * 绘制圆请求
 **/
struct packet_body_draw_circle
{
    si_t gd;
    si_t x;
    si_t y;
    si_t radius;
};
extern void request_set_draw_circle(struct packet_body_draw_circle* body, si_t gd, si_t x, si_t y, si_t radius);
extern si_t request_call_by_draw_circle(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t));

/**
 * 绘制椭圆请求
 **/
struct packet_body_draw_ellipse
{
	si_t gd;
	si_t x;
	si_t y;
	si_t a;
	si_t b;
};
extern void request_set_draw_ellipse(struct packet_body_draw_ellipse* body, si_t gd, si_t x, si_t y, si_t a, si_t b);
extern si_t request_call_by_draw_ellipse(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t));

/**
 * 绘制图像请求
 **/
struct packet_body_draw_img
{
	si_t gd;
	char* name;
	si_t length;
	si_t align;
};
extern void request_set_draw_img(struct packet_body_draw_img* body, si_t gd, const char* name, si_t length, si_t align);
extern si_t request_call_by_draw_img(addr_t arg, si_t (*callable)(si_t, char*, si_t));

/**
 * 绘制直线请求
 **/
struct packet_body_draw_line
{
	si_t gd;
	si_t x1;
	si_t y1;
	si_t x2;
	si_t y2;
};
extern void request_set_draw_line(struct packet_body_draw_line* body, si_t gd, si_t x1, si_t y1, si_t x2, si_t y2);
extern si_t request_call_by_draw_line(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t));

/**
 * 绘制点请求
 **/
struct packet_body_draw_point
{
	si_t gd;
	si_t x;
	si_t y;
};
extern void request_set_draw_point(struct packet_body_draw_point* body, si_t gd, si_t x, si_t y);
extern si_t request_call_by_draw_point(addr_t arg, si_t (*callable)(si_t, si_t, si_t));

/**
 * 绘制多边形请求
 **/
struct packet_body_draw_polygon
{
	si_t gd;
	struct point * points;
	si_t point_count;
};
extern void request_set_draw_polygon(struct packet_body_draw_polygon* body, si_t gd, const struct point* point, si_t point_count);
extern si_t request_call_by_draw_polygon(addr_t arg, si_t (*callable)(si_t, struct point*, si_t));

/**
 * 绘制矩形请求
 **/
struct packet_body_draw_rectangle
{
	si_t gd;
	si_t x_axis;
	si_t y_axis;
	si_t x_size;
	si_t y_size;
};
extern void request_set_draw_rectangle(struct packet_body_draw_rectangle* body, si_t gd, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size);
extern si_t request_call_by_draw_rectangle(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t));

/**
 * 绘制x型请求(光标)
 **/
struct packet_body_draw_x
{
	si_t gd;
	si_t x;
	si_t y;
	si_t a;
	si_t b;
};
extern void request_set_draw_x(struct packet_body_draw_x* body, si_t gd, si_t x, si_t y, si_t a, si_t b);
extern si_t request_call_by_draw_x(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t));

/**
 * 填充箭头请求
 **/
struct packet_body_fill_arrow
{
	si_t gd;
	si_t x;
	si_t y;
	si_t size;
	si_t direction;
};
extern void request_set_fill_arrow(struct packet_body_fill_arrow* body, si_t gd, si_t x, si_t y, si_t size, si_t direction);
extern si_t request_call_by_fill_arrow(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t));

/**
 * 填充圆形的请求
 **/
struct packet_body_fill_circle
{
	si_t gd;
	si_t x;
	si_t y;
	si_t radius;
};
extern void request_set_fill_circle(struct packet_body_fill_circle* body, si_t gd, si_t x, si_t y, si_t radius);
extern si_t request_call_by_fill_circle(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t));

/**
 * 填充椭圆形的请求
 **/
struct packet_body_fill_ellipse
{
	si_t gd;
	si_t x;
	si_t y;
	si_t a;
	si_t b;
};
extern void request_set_fill_ellipse(struct packet_body_fill_ellipse* body, si_t gd, si_t x, si_t y, si_t a, si_t b);
extern si_t request_call_by_fill_ellipse(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t));

/**
 * 填充多边形的请求
 **/
struct packet_body_fill_polygon
{
	si_t gd;
	struct point * points;
	si_t point_count;
};
extern void request_set_fill_polygon(struct packet_body_fill_polygon* body, si_t gd, struct point* point, si_t point_count);
extern si_t request_call_by_fill_polygon(addr_t arg, si_t (*callable)(si_t, struct point*, si_t));

/**
 * 填充矩形的请求
 **/
struct packet_body_fill_rectangle
{
	si_t gd;
	si_t x_axis;
	si_t y_axis;
	si_t x_size;
	si_t y_size;
};
extern void request_set_fill_rectangle(struct packet_body_fill_rectangle* body, si_t gd, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size);
extern si_t request_call_by_fill_rectangle(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t));

/**
 * 填充x的请求
 **/
struct packet_body_fill_x
{
	si_t gd;
	si_t x;
	si_t y;
	si_t a;
	si_t b;
};
extern void request_set_fill_x(struct packet_body_fill_x* body, si_t gd, si_t x, si_t y, si_t a, si_t b);
extern si_t request_call_by_fill_x(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t));

/**
 * 获得图形设备的区域的请求
 **/
struct packet_body_get_area
{
	si_t gd;
};
extern void request_set_get_area(struct packet_body_get_area* body, si_t gd);
extern struct rectangle* request_call_by_get_area(addr_t arg, struct rectangle* (*callable)(si_t));

/**
 * 获得图形设备的颜色的请求
 **/
struct packet_body_get_color
{
	si_t gd;
};
extern void request_set_get_color(struct packet_body_get_color* body, si_t gd);
extern struct color* request_call_by_get_color(addr_t arg, struct color*(*callable)(si_t));

/**
 * 获得图形设备的字体宽度的请求
 **/
struct packet_body_get_font_width
{
	si_t gd;
};
extern void request_set_get_font_width(struct packet_body_get_font_width* body, si_t gd);
extern si_t request_call_by_get_font_width(addr_t arg, si_t (*callable)(si_t));

/**
 * 获得图形设备字体高度的请求
 **/
struct packet_body_get_font_height
{
	si_t gd;
};
extern void request_set_get_font_height(struct packet_body_get_font_height* body, si_t gd);
extern si_t request_call_by_get_font_height(addr_t arg, si_t (*callable)(si_t));

/**
 * 设置图形设备的区域的请求
 **/
struct packet_body_set_area
{
    si_t gd;
    si_t x_axis;
    si_t y_axis;
    si_t x_size;
    si_t y_size;
};
extern void request_set_set_area(struct packet_body_set_area* body, si_t gd, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size);
extern si_t request_call_by_set_area(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t));

/**
 * 设置图形设备的颜色的请求
 **/
struct packet_body_set_color
{
    si_t gd;
    si_t r;
    si_t g;
    si_t b;
    si_t a;
};
extern void request_set_set_color(struct packet_body_set_color* body, si_t gd, si_t r, si_t g, si_t b, si_t a);
extern si_t request_call_by_set_color(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t));

/**
 * 设置图形设备字体的请求
 **/
struct packet_body_set_font
{
    si_t gd;
    si_t font;
};
extern void request_set_set_font(struct packet_body_set_font* body, si_t gd, si_t font);
extern si_t request_call_by_set_font(addr_t arg, si_t (*callable)(si_t, si_t));

/**
 * 显示字体的请求
 **/
struct packet_body_show_text
{
    si_t gd;
    si_t x;
    si_t y;
    char * str;
    si_t length;
};
extern void request_set_show_text(struct packet_body_show_text* body, si_t gd, si_t x, si_t y, const char *str, si_t length);
extern si_t request_call_by_show_text(addr_t arg, si_t (*callable)(si_t, si_t, si_t, char *, si_t));

struct packet_body_get_window_title
{
	si_t window_descriptor;
};
extern void request_set_window_title(struct packet_body_get_window_title* body, si_t window_descriptor);
extern void request_get_window_title(addr_t body, si_t* window_descriptor);

# endif
