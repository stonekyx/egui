/**
 * @file request.c
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

# include "request.h"
# include <stdarg.h>
# include <string.h>

char* request_type_to_str(si_t request_type)
{
	switch(request_type)
	{
	case REQUEST_TYPE_REGISTER_APPLICATION:
		return "REQUEST_TYPE_REGISTER_APPLICATION";
	case REQUEST_TYPE_CANCEL_APPLICATION:
		return "REQUEST_TYPE_CANCEL_APPLICATION";
	case REQUEST_TYPE_GRAPHICS_DEVICE_INIT:
		return "REQUEST_TYPE_GRAPHICS_DEVICE_INIT";
	case REQUEST_TYPE_GRAPHICS_DEVICE_EXIT:
		return "REQUEST_TYPE_GRAPHICS_DEVICE_EXIT";
	case REQUEST_TYPE_WINDOW_MANAGER_QUIT:
		return "REQUEST_TYPE_WINDOW_MANAGER_QUIT";
	case REQUEST_TYPE_UPDATE:
		return "REQUEST_TYPE_UPDATE";
	case REQUEST_TYPE_DESKTOP_DIRTY:
		return "REQUEST_TYPE_DESKTOP_DIRTY";
	case REQUEST_TYPE_REGISTER_WINDOW:
		return "REQUEST_TYPE_REGISTER_WINDOW";
	case REQUEST_TYPE_CANCEL_WINDOW:
		return "REQUEST_TYPE_CANCEL_WINDOW";
	case REQUEST_TYPE_MOVE_WINDOW:
		return "REQUEST_TYPE_MOVE_WINDOW";
	case REQUEST_TYPE_RESIZE_WINDOW:
		return "REQUEST_TYPE_RESIZE_WINDOW";
	case REQUEST_TYPE_MINIMIZE_WINDOW:
		return "REQUEST_TYPE_MINIMIZE_WINDOW";
	case REQUEST_TYPE_MAXIMIZE_WINDOW:
		return "REQUEST_TYPE_MAXIMIZE_WINDOW";
	case REQUEST_TYPE_RESTORE_WINDOW:
		return "REQUEST_TYPE_RESTORE_WINDOW";
	case REQUEST_TYPE_ACTIVATE_WINDOW:
		return "REQUEST_TYPE_ACTIVATE_WINDOW";
	case REQUEST_TYPE_CLEAR:
		return "REQUEST_TYPE_CLEAR";
	case REQUEST_TYPE_CLEAR_WHITE:
		return "REQUEST_TYPE_CLEAR_WHITE";
	case REQUEST_TYPE_CLEAR_BLACK:
		return "REQUEST_TYPE_CLEAR_BLACK";
	case REQUEST_TYPE_DRAW_LINE:
		return "REQUEST_TYPE_DRAW_LINE";
	case REQUEST_TYPE_DRAW_POINT:
		return "REQUEST_TYPE_DRAW_POINT";
	case REQUEST_TYPE_DRAW_RECTANGLE:
		return "REQUEST_TYPE_DRAW_RECTANGLE";
	case REQUEST_TYPE_DRAW_POLYGON:
		return "REQUEST_TYPE_DRAW_POLYGON";
	case REQUEST_TYPE_DRAW_CIRCLE:
		return "REQUEST_TYPE_DRAW_CIRCLE";
	case REQUEST_TYPE_DRAW_ELLIPSE:
		return "REQUEST_TYPE_DRAW_ELLIPSE";
	case REQUEST_TYPE_DRAW_ARROW:
		return "REQUEST_TYPE_DRAW_ARROW";
	case REQUEST_TYPE_DRAW_X:
		return "REQUEST_TYPE_DRAW_X";
	case REQUEST_TYPE_DRAW_BITMAP:
		return "REQUEST_TYPE_DRAW_BITMAP";
	case REQUEST_TYPE_DRAW_IMG:
		return "REQUEST_TYPE_DRAW_IMG";
	case REQUEST_TYPE_FILL_RECTANGLE:
		return "REQUEST_TYPE_FILL_RECTANGLE";
	case REQUEST_TYPE_FILL_CIRCLE:
		return "REQUEST_TYPE_FILL_CIRCLE";
	case REQUEST_TYPE_FILL_ELLIPSE:
		return "REQUEST_TYPE_FILL_ELLIPSE";
	case REQUEST_TYPE_FILL_ARROW:
		return "REQUEST_TYPE_FILL_ARROW";
	case REQUEST_TYPE_FILL_X:
		return "REQUEST_TYPE_FILL_X";
	case REQUEST_TYPE_SHOW_TEXT:
		return "REQUEST_TYPE_SHOW_TEXT";
	case REQUEST_TYPE_CPY_AREA:
		return "REQUEST_TYPE_CPY_AREA";
	case REQUEST_TYPE_SET_COLOR:
		return "REQUEST_TYPE_SET_COLOR";
	case REQUEST_TYPE_SET_AREA:
		return "REQUEST_TYPE_SET_AREA";
	case REQUEST_TYPE_SET_FONT:
		return "REQUEST_TYPE_SET_FONT";
	case REQUEST_TYPE_GET_COLOR_LIMIT:
		return "REQUEST_TYPE_GET_COLOR_LIMIT";
	case REQUEST_TYPE_GET_SCREEN_WIDTH:
		return "REQUEST_TYPE_GET_SCREEN_WIDTH";
	case REQUEST_TYPE_GET_SCREEN_HEIGHT:
		return "REQUEST_TYPE_GET_SCREEN_HEIGHT";
	case REQUEST_TYPE_GET_AREA:
		return "REQUEST_TYPE_GET_AREA";
	case REQUEST_TYPE_GET_COLOR:
		return "REQUEST_TYPE_GET_COLOR";
	case REQUEST_TYPE_GET_FONT_WIDTH:
		return "REQUEST_TYPE_GET_FONT_WIDTH";
	case REQUEST_TYPE_GET_FONT_HEIGHT:
		return "REQUEST_TYPE_GET_FONT_HEIGHT";
	case REQUEST_TYPE_GET_WINDOW_TITLE:
		return "REQUEST_TYPE_GET_WINDOW_TITLE";
	default:
		return NULL;
	}
}

/**
 * 使用va_arg将函数的参数依次写入结构体中
 * 函数参数/结构体变量必须为si_t类型或指针
 * 这样每次将下一个参数写入结构体的下一个字长中即可
 *
 * @param struct_ptr 待写入的结构体指针，所有成员变量都从该指针指向的地址开始写起
 * @param param_num 参数个数
 **/
static void write_func_arg_to_struct_var(addr_t struct_ptr, int param_num, ...)
{
    va_list ap;	/* 存放可变长参数们 */
	ui_t i = 0; /* 参数计数器 */
	si_t* cur_ptr = (si_t*)struct_ptr; /* 写入的偏移位置，从结构体指针开始，每次写sizeof(si_t)，然后增加偏移 */
	va_start(ap, param_num);
	while(i ++ != param_num)
	{
		/** 获得下一个参数 **/
		si_t param = va_arg(ap, si_t); 
		/** 指针指向结构体的下一个偏移量 **/
		* (cur_ptr ++) = param;
	}
	va_end(ap);
}

/**
 * 使用va_arg将结构体的变量依次写入函数参数所指向的内容
 * 函数参数/结构体变量必须为si_t类型或指针
 * 这样每次将结构体的下一个字长写入下一个参数指向内容即可
 * 如果函数参数指针为空，则不写入
 *
 * @param struct_ptr 待读取的结构体指针，所有成员变量都从该指针指向的地址开始读取
 * @param param_num 参数个数
 **/
static void write_struct_var_to_func_arg(addr_t struct_ptr, int param_num, ...)
{
    va_list ap;	/* 存放可变长参数们 */
	ui_t i = 0; /* 参数计数器 */
	si_t* cur_ptr = (si_t*)struct_ptr; /* 写入的偏移位置，从结构体指针开始，每次写sizeof(si_t)，然后增加偏移 */
	va_start(ap, param_num);
	while(i ++ != param_num)
	{
		/** 获得下一个参数 **/
		si_t* param = va_arg(ap, si_t*); 
		if(NULL != param)
		{
			*param = *cur_ptr;
		};
		/** 指针指向结构体的下一个偏移量 **/
		++ cur_ptr;
	}
	va_end(ap);
}

/**
 * 以下的setter和getter都是使用上面的函数用va_arg写入/读出的
 *
 * 对应的call_by_xx都是将request_body做为参数依次传入callable函数的
 *
 * client端发送request body通常只需要setter
 * server端接受request body之后通常只需要getter获得部分成员变量
 * 或者直接将body作为参数传入另一个实际工作的函数(通常是engine_xx, see graph/graph_engine.h)
 *
 **/

extern void request_set_register_application(struct packet_body_register_application* body, si_t video_access_mode, si_t application_type, const char* name, si_t length)
{
	write_func_arg_to_struct_var((addr_t)body, 4, video_access_mode, application_type, name, length);
}

extern void request_get_register_application(addr_t arg, si_t* video_access_mode, si_t* application_type, char** name, si_t* length)
{
	struct packet_body_register_application* body = (struct packet_body_register_application*)arg;
	body->name = (char*)((byte_t*)arg + sizeof(struct packet_body_register_application));
	write_struct_var_to_func_arg(body, 4, video_access_mode, application_type, name, length);
}

extern void request_set_graphics_device_init(struct packet_body_graphics_device_init* body, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size, si_t r, si_t g, si_t b, si_t a, si_t font)
{
	write_func_arg_to_struct_var((addr_t)body, 9, x_axis, y_axis, x_size, y_size, r, g, b, a, font);
}
 
extern si_t request_call_by_graphics_device_init(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t, si_t, si_t, si_t, si_t))
{
	struct packet_body_graphics_device_init* body = (struct packet_body_graphics_device_init*)arg;
	return callable(body->x_axis, body->y_axis, body->x_size, body->y_size, body->r, body->g, body->b, body->a, body->font);
}

extern void request_set_graphics_device_exit(struct packet_body_graphics_device_exit* body, si_t gd)
{
	write_func_arg_to_struct_var((addr_t)body, 1, gd);
}

extern void request_get_graphics_device_exit(addr_t body, si_t* gd)
{
	write_struct_var_to_func_arg((addr_t)body, 1, gd);
}

extern si_t request_call_by_graphics_device_exit(addr_t arg, si_t (*callable)(si_t))
{
	struct packet_body_graphics_device_exit* body = (struct packet_body_graphics_device_exit*)arg;
	return callable(body->gd);
}

extern void request_set_update(struct packet_body_update* body, si_t gd)
{
	write_func_arg_to_struct_var((addr_t)body, 1, gd);
}

extern void request_get_update(addr_t body, si_t* gd)
{
	write_struct_var_to_func_arg(body, 1, gd);
}

extern void request_set_register_window(struct packet_body_register_window* body, si_t parent_window_descriptor, const char* title, si_t title_length, si_t x, si_t y, si_t width, si_t height, si_t minimize_enable, si_t maximize_enable, si_t modal)
{
	write_func_arg_to_struct_var((addr_t)body, 10, parent_window_descriptor, title, title_length, x, y, width, height, minimize_enable, maximize_enable, modal);
}

extern void request_get_register_window(addr_t arg, si_t* parent_window_descriptor, char** title, si_t* title_length, si_t* x, si_t* y, si_t* width, si_t* height, si_t* minimize_enable, si_t* maximize_enable, si_t* modal)
{
	struct packet_body_register_window* body = (struct packet_body_register_window*)arg;
	body->title = (char*)((byte_t*)arg + sizeof(struct packet_body_register_window));
	write_struct_var_to_func_arg(body, 10, parent_window_descriptor, title, title_length, x, y, width, height, minimize_enable, maximize_enable, modal);
}

extern void request_set_cancel_window(struct packet_body_cancel_window* body, si_t window_id)
{
	write_func_arg_to_struct_var((addr_t)body, 1, window_id);
}

extern void request_get_cancel_window(addr_t body, si_t* window_id)
{
	write_struct_var_to_func_arg(body, 1, window_id);
}
 
extern void request_set_move_window(struct packet_body_move_window* body, si_t window_id, si_t x_axis, si_t y_axis)
{
	write_func_arg_to_struct_var((addr_t)body, 3, window_id, x_axis, y_axis);
}

extern void request_get_move_window(addr_t body, si_t* window_id, si_t* x_axis, si_t* y_axis)
{
	write_struct_var_to_func_arg(body, 3, window_id, x_axis, y_axis);
}

extern void request_set_resize_window(struct packet_body_resize_window* body, si_t window_id, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size)
{
	write_func_arg_to_struct_var((addr_t)body, 5, window_id, x_axis, y_axis, x_size, y_size);
}

extern void request_get_resize_window(addr_t body, si_t* window_id, si_t* x_axis, si_t* y_axis, si_t* x_size, si_t* y_size)
{
	write_struct_var_to_func_arg(body, 5, window_id, x_axis, y_axis, x_size, y_size);
}

extern void request_set_maximize_window(struct packet_body_maximize_window* body, si_t window_id)
{
	write_func_arg_to_struct_var((addr_t)body, 1, window_id);
}

extern void request_get_maximize_window(addr_t body, si_t* window_id)
{
	write_struct_var_to_func_arg(body, 1, window_id);
}

extern void request_set_minimize_window(struct packet_body_minimize_window* body, si_t window_id)
{
	write_func_arg_to_struct_var((addr_t)body, 1, window_id);
}

extern void request_get_minimize_window(addr_t body, si_t* window_id)
{
	write_struct_var_to_func_arg(body, 1, window_id);
}

extern void request_set_restore_window(struct packet_body_restore_window* body, si_t window_id, si_t x, si_t y, si_t width, si_t height)
{
	write_func_arg_to_struct_var((addr_t)body, 5, window_id, x, y, width, height);
}

extern void request_get_restore_window(addr_t body, si_t* window_id, si_t* x, si_t* y, si_t* width, si_t* height)
{
	write_struct_var_to_func_arg(body, 5, window_id, x, y, width, height);
}

extern void request_set_activate_window(struct packet_body_activate_window* body, si_t window_id)
{
	write_func_arg_to_struct_var((addr_t)body, 1, window_id);
}

extern void request_get_activate_window(addr_t body, si_t* window_id)
{
	write_struct_var_to_func_arg(body, 1, window_id);
}

extern void request_set_clear(struct packet_body_clear* body, si_t gd)
{
	write_func_arg_to_struct_var((addr_t)body, 1, gd);
}

extern si_t request_call_by_clear(addr_t arg, si_t (*callable)(si_t))
{
	struct packet_body_clear* body = (struct packet_body_clear*)arg;
	return callable(body->gd);
}

extern void request_set_clear_black(struct packet_body_clear_black* body, si_t gd)
{
	write_func_arg_to_struct_var((addr_t)body, 1, gd);
}

extern si_t request_call_by_clear_black(addr_t arg, si_t (* callable)(si_t))
{
	struct packet_body_clear_black* body = (struct packet_body_clear_black*)arg;
	return callable(body->gd);
}

extern void request_set_clear_white(struct packet_body_clear_white* body, si_t gd)
{
	write_func_arg_to_struct_var((addr_t)body, 1, gd);
}

extern si_t request_call_by_clear_white(addr_t arg, si_t (*callable)(si_t))
{
	struct packet_body_clear_white* body = (struct packet_body_clear_white*)arg;
	return callable(body->gd);
}

extern void request_set_cpy_area(struct packet_body_cpy_area* body, si_t gd, si_t mode, si_t dst_x, si_t dst_y, si_t src_x, si_t src_y, si_t x_size, si_t y_size)
{
	write_func_arg_to_struct_var((addr_t)body, 8, gd, mode, dst_x, dst_y, src_x, src_y, x_size, y_size);
}

extern si_t request_call_by_cpy_area(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t, si_t, si_t, si_t))
{
	struct packet_body_cpy_area* body = (struct packet_body_cpy_area*)arg;
	return callable(body->gd, body->mode, body->dst_x, body->dst_y, body->src_x, body->src_y, body->x_size, body->y_size);
}

extern void request_set_draw_arrow(struct packet_body_draw_arrow* body, si_t gd, si_t x, si_t y, si_t size, si_t direction)
{
	write_func_arg_to_struct_var((addr_t)body, 5, gd, x, y, size, direction);
}

extern si_t request_call_by_draw_arrow(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t))
{
	struct packet_body_draw_arrow* body = (struct packet_body_draw_arrow*)arg;
	return callable(body->gd, body->x, body->y, body->size, body->direction);
}

extern void request_set_draw_bitmap(struct packet_body_draw_bitmap* body, si_t gd, const char* name, si_t length, si_t align)
{
	write_func_arg_to_struct_var((addr_t)body, 4, gd, name, length, align);
}

extern si_t request_call_by_draw_bitmap(addr_t arg, si_t (*callable)(si_t, char*, si_t))
{
	struct packet_body_draw_bitmap* body = (struct packet_body_draw_bitmap*)arg;
	body->name = (char*)((byte_t*)arg + sizeof(struct packet_body_draw_bitmap));
	return callable(body->gd, body->name, body->align);
}

extern void request_set_draw_circle(struct packet_body_draw_circle* body, si_t gd, si_t x, si_t y, si_t radius)
{
	write_func_arg_to_struct_var((addr_t)body, 4, gd, x, y, radius);
}

extern si_t request_call_by_draw_circle(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t))
{
	struct packet_body_draw_circle* body = (struct packet_body_draw_circle*)arg;
	return callable(body->gd, body->x, body->y, body->radius);
}

extern void request_set_draw_ellipse(struct packet_body_draw_ellipse* body, si_t gd, si_t x, si_t y, si_t a, si_t b)
{
	write_func_arg_to_struct_var((addr_t)body, 5, gd, x, y, a, b);
}

extern si_t request_call_by_draw_ellipse(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t))
{
	struct packet_body_draw_ellipse* body = (struct packet_body_draw_ellipse*)arg;
	return callable(body->gd, body->x, body->y, body->a, body->b);
}

extern void request_set_draw_img(struct packet_body_draw_img* body, si_t gd, const char* name, si_t length, si_t align)
{
	write_func_arg_to_struct_var((addr_t)body, 4, gd, name, length, align);
}

extern si_t request_call_by_draw_img(addr_t arg, si_t (*callable)(si_t, char*, si_t))
{
	struct packet_body_draw_img* body = (struct packet_body_draw_img*)arg;
	body->name = (char*)((byte_t*)arg + sizeof(struct packet_body_draw_img));
	return callable(body->gd, body->name, body->align);
}

extern void request_set_draw_line(struct packet_body_draw_line* body, si_t gd, si_t x1, si_t y1, si_t x2, si_t y2)
{
	write_func_arg_to_struct_var((addr_t)body, 5, gd, x1, y1, x2, y2);
}

extern si_t request_call_by_draw_line(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t))
{
	struct packet_body_draw_line* body = (struct packet_body_draw_line*)arg;
	return callable(body->gd, body->x1, body->y1, body->x2, body->y2);
}

extern void request_set_draw_point(struct packet_body_draw_point* body, si_t gd, si_t x, si_t y)
{
	write_func_arg_to_struct_var((addr_t)body, 3, gd, x, y);
}

extern si_t request_call_by_draw_point(addr_t arg, si_t (*callable)(si_t, si_t, si_t))
{
	struct packet_body_draw_point* body = (struct packet_body_draw_point*)arg;
	return callable(body->gd, body->x, body->y);
}

extern void request_set_draw_polygon(struct packet_body_draw_polygon* body, si_t gd, const struct point* point, si_t point_count)
{
	write_func_arg_to_struct_var((addr_t)body, 3, gd, point, point_count);
}

extern si_t request_call_by_draw_polygon(addr_t arg, si_t (*callable)(si_t, struct point*, si_t))
{
	struct packet_body_draw_polygon* body = (struct packet_body_draw_polygon*)arg;
	body->points = (struct point*)((byte_t*)arg + sizeof(struct packet_body_draw_polygon));
	return callable(body->gd, body->points, body->point_count);
}

extern void request_set_draw_rectangle(struct packet_body_draw_rectangle* body, si_t gd, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size)
{
	write_func_arg_to_struct_var((addr_t)body, 5, gd, x_axis, y_axis, x_size, y_size);
}

extern si_t request_call_by_draw_rectangle(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t))
{
	struct packet_body_draw_rectangle* body = (struct packet_body_draw_rectangle*)arg;
	return callable(body->gd, body->x_axis, body->y_axis, body->x_size, body->y_size);
}

extern void request_set_draw_x(struct packet_body_draw_x* body, si_t gd, si_t x, si_t y, si_t a, si_t b)
{
	write_func_arg_to_struct_var((addr_t)body, 5, gd, x, y, a, b);
}

extern si_t request_call_by_draw_x(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t))
{
	struct packet_body_draw_x* body = (struct packet_body_draw_x*)arg;
	return callable(body->gd, body->x, body->y, body->a, body->b);
}

extern void request_set_fill_arrow(struct packet_body_fill_arrow* body, si_t gd, si_t x, si_t y, si_t size, si_t direction)
{
	write_func_arg_to_struct_var((addr_t)body, 5, gd, x, y, size, direction);
}

extern si_t request_call_by_fill_arrow(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t))
{
	struct packet_body_fill_arrow* body = (struct packet_body_fill_arrow*)arg;
	return callable(body->gd, body->x, body->y, body->size, body->direction);
}

extern void request_set_fill_circle(struct packet_body_fill_circle* body, si_t gd, si_t x, si_t y, si_t radius)
{
	write_func_arg_to_struct_var((addr_t)body, 4, gd, x, y, radius);
}

extern si_t request_call_by_fill_circle(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t))
{
	struct packet_body_fill_circle* body = (struct packet_body_fill_circle*)arg;
	return callable(body->gd, body->x, body->y, body->radius);
}

extern void request_set_fill_ellipse(struct packet_body_fill_ellipse* body, si_t gd, si_t x, si_t y, si_t a, si_t b)
{
	write_func_arg_to_struct_var((addr_t)body, 5, gd, x, y, a, b);
}

extern si_t request_call_by_fill_ellipse(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t))
{
	struct packet_body_fill_ellipse* body = (struct packet_body_fill_ellipse*)arg;
	return callable(body->gd, body->x, body->y, body->a, body->b);
}

extern void request_set_fill_polygon(struct packet_body_fill_polygon* body, si_t gd, struct point* point, si_t point_count)
{
	write_func_arg_to_struct_var((addr_t)body, 3, gd, point, point_count);
}

extern si_t request_call_by_fill_polygon(addr_t arg, si_t (*callable)(si_t, struct point*, si_t))
{
	struct packet_body_fill_polygon* body = (struct packet_body_fill_polygon*)arg;
	body->points = (struct point*)((byte_t*)arg + sizeof(struct packet_body_fill_polygon));
	return callable(body->gd, body->points, body->point_count);
}

extern void request_set_fill_rectangle(struct packet_body_fill_rectangle* body, si_t gd, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size)
{
	write_func_arg_to_struct_var((addr_t)body, 5, gd, x_axis, y_axis, x_size, y_size);
}

extern si_t request_call_by_fill_rectangle(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t))
{
	struct packet_body_fill_rectangle* body = (struct packet_body_fill_rectangle*)arg;
	return callable(body->gd, body->x_axis, body->y_axis, body->x_size, body->y_size);
}

extern void request_set_fill_x(struct packet_body_fill_x* body, si_t gd, si_t x, si_t y, si_t a, si_t b)
{
	write_func_arg_to_struct_var((addr_t)body, 5, gd, x, y, a, b);
}

extern si_t request_call_by_fill_x(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t))
{
	struct packet_body_fill_x* body = (struct packet_body_fill_x*)arg;
	return callable(body->gd, body->x, body->y, body->a, body->b);
}

extern void request_set_get_area(struct packet_body_get_area* body, si_t gd)
{
	write_func_arg_to_struct_var((addr_t)body, 1, gd);
}

extern struct rectangle* request_call_by_get_area(addr_t arg, struct rectangle* (*callable)(si_t))
{
	struct packet_body_get_area* body = (struct packet_body_get_area*)arg;
	return callable(body->gd);
}

extern void request_set_get_color(struct packet_body_get_color* body, si_t gd)
{
	write_func_arg_to_struct_var((addr_t)body, 1, gd);
}

extern struct color* request_call_by_get_color(addr_t arg, struct color*(*callable)(si_t))
{
	struct packet_body_get_color* body = (struct packet_body_get_color*)arg;
	return callable(body->gd);
}

extern void request_set_get_font_width(struct packet_body_get_font_width* body, si_t gd)
{
	write_func_arg_to_struct_var((addr_t)body, 1, gd);
}

extern si_t request_call_by_get_font_width(addr_t arg, si_t (*callable)(si_t))
{
	struct packet_body_get_font_width* body = (struct packet_body_get_font_width*)arg;
	return callable(body->gd);
}

extern void request_set_get_font_height(struct packet_body_get_font_height* body, si_t gd)
{
	write_func_arg_to_struct_var((addr_t)body, 1, gd);
}

extern si_t request_call_by_get_font_height(addr_t arg, si_t (*callable)(si_t))
{
	struct packet_body_get_font_height* body = (struct packet_body_get_font_height*)arg;
	return callable(body->gd);
}

extern void request_set_set_area(struct packet_body_set_area* body, si_t gd, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size)
{
	write_func_arg_to_struct_var((addr_t)body, 5, gd, x_axis, y_axis, x_size, y_size);
}

extern si_t request_call_by_set_area(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t))
{
	struct packet_body_set_area* body = (struct packet_body_set_area*)arg;
	return callable(body->gd, body->x_axis, body->y_axis, body->x_size, body->y_size);
}

extern void request_set_set_color(struct packet_body_set_color* body, si_t gd, si_t r, si_t g, si_t b, si_t a)
{
	write_func_arg_to_struct_var((addr_t)body, 5, gd, r, g, b, a);
}

extern si_t request_call_by_set_color(addr_t arg, si_t (*callable)(si_t, si_t, si_t, si_t, si_t))
{
	struct packet_body_set_color* body = (struct packet_body_set_color*)arg;
	return callable(body->gd, body->r, body->g, body->b, body->a);
}

extern void request_set_set_font(struct packet_body_set_font* body, si_t gd, si_t font)
{
	write_func_arg_to_struct_var((addr_t)body, 2, gd, font);
}

extern si_t request_call_by_set_font(addr_t arg, si_t (*callable)(si_t, si_t))
{
	struct packet_body_set_font* body = (struct packet_body_set_font*)arg;
	return callable(body->gd, body->font);
}

extern void request_set_show_text(struct packet_body_show_text* body, si_t gd, si_t x, si_t y, const char *str, si_t length)
{
	write_func_arg_to_struct_var((addr_t)body, 5, gd, x, y, str, length);
}

extern si_t request_call_by_show_text(addr_t arg, si_t (*callable)(si_t, si_t, si_t, char *, si_t))
{
	struct packet_body_show_text* body = (struct packet_body_show_text*)arg;
	body->str = (char*)((byte_t*)arg + sizeof(struct packet_body_show_text));
	return callable(body->gd, body->x, body->y, body->str, body->length);
}

extern void request_set_window_title(struct packet_body_get_window_title* body, si_t window_descriptor)
{
	write_func_arg_to_struct_var((addr_t)body, 1, window_descriptor);
}

extern void request_get_window_title(addr_t body, si_t* window_descriptor)
{
	write_struct_var_to_func_arg(body, 1, window_descriptor);
}
