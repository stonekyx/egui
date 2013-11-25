/*
 * @file   client_lib.c
 *
 * @author Dalian University of Technology
 *
 * @version 2.0
 *
 * @data 2012.02.15
 *
 * @section LICENSE
 *
 * Copyright (C) 2010 Dalian University of Technology
 *
 * This file is part of Egui.
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

# include "client_lib.h"

# include <stdlib.h>
# include "comm.h"
# include "log.h"

/**
 * 通信句柄
 **/
static struct egui_uds* lib_uds = NULL;
/**
 * 消息队列，在等待回应过程中若收到消息则放入消息队列等待收到回应之后再处理
 **/
static struct queue* message_queue = NULL;

extern void client_lib_init(struct egui_uds* uds_ptr, struct queue* queue_ptr)
{
	lib_uds = uds_ptr;
	message_queue = queue_ptr;
}

/**
 * 发送请求负载，读取返回内容。如果发送或接受过程出错，返回-1，否则返回0
 **/
static si_t send_request_and_recv_respond(si_t request_type, si_t respond_type,
        const_addr_t body, ui_t body_size,
        addr_t result)
{
	if(0 != comm_send_request(lib_uds, request_type, body, body_size))
	{
		EGUI_PRINT_ERROR("failed to send request[%s]", request_type_to_str(request_type));
		return -1;
	}

	if(0 != comm_recv_respond(lib_uds, respond_type, result, message_queue))
	{
		EGUI_PRINT_ERROR("failed to receive respond[%s]", respond_type_to_str(respond_type));
		return -1;
	}
	return 0;
}

static si_t send_request_and_malloc_for_respond(si_t request_type, si_t respond_type,
        const_addr_t body, ui_t body_size,
        addr_t* result)
{
	if(0 != comm_send_request(lib_uds, request_type, body, body_size))
	{
		EGUI_PRINT_ERROR("failed to send request[%s]", request_type_to_str(request_type));
		return -1;
	}

	if(0 != comm_recv_respond_in_new_buffer(lib_uds, respond_type, result, message_queue))
	{
		EGUI_PRINT_ERROR("failed to receive respond[%s]", respond_type_to_str(respond_type));
		return -1;
	}
	return 0;
}

/**
 * 发送请求负载，读取返回的retval。如果发送或接受过错出错，返回-1，否则返回回应包的retval
 **/
static si_t send_request_and_return_respond(si_t request_type, si_t respond_type,
        const_addr_t body, ui_t body_size)
{
	si_t respond_retval = 0, retval = 0;
	retval = send_request_and_recv_respond(request_type, respond_type, body, body_size, &respond_retval);
	return retval == 0 ? respond_retval : retval;
}

/**
 * 发送带有附加内容的负载，读取返回的retval。如果发送或接受过程出错或malloc失败返回-1，否则返回回应包的retval
 *
 * @param request_type 请求类型
 * @param respond_type 回应类型
 * @param body 标准负载指针
 * @param body_len 标准负载的长度
 * @param extra_context 附加内容指针
 * @param extra_context_len 附加内容长度
 **/
static si_t send_body_with_extra_context_and_return_respond(si_t request_type, si_t respond_type,
        const_addr_t body         , ui_t body_len,
        const_addr_t extra_context, ui_t extra_context_len)
{
	si_t result = 0;
	if(0 != comm_send_request_with_extra(lib_uds, request_type, body, body_len, extra_context, extra_context_len))
	{
		EGUI_PRINT_ERROR("failed to send request[%s] with extra content", request_type_to_str(request_type));
		return -1;
	}

	if(0 != comm_recv_respond(lib_uds, respond_type, &result, message_queue))
	{
		EGUI_PRINT_ERROR("failed to receive respond[%s]", respond_type_to_str(respond_type));
		return -1;
	}
	return result;
}

/**
 * 以下为client端对各种类型的请求的发送和接受回应过程，类似与远程过程调用
 *
 * 使用上述所有static方法的帮助，可以使每个函数最多由三步组成(也是三行)
 * 1. 声明对应负载类型(see request.h)
 * 2. 通过对应的setter将参数填充进负载(see request.h)
 * 3. 发送负载获得回应(借助上面的static方法)
 *
 * 第三步分为以下几种情况：
 * 1. 若获得的回应为si_t的retval，可以直接调用send_request_and_return_respond()将返回值作为函数返回值
 * 2. 若获得的回应为其他类型，可以将读取回应的指针传入send_request_and_recv_respond()将返回值作为函数返回值
 * 3. 若发送的负载包含附加内容，可以调用send_body_with_extra_context_and_return_respond()
 *
 * 还有就是发送请求头部但需要接受回应包，此时可以用对应的static函数，将负载部分传为NULL长度为0
 **/
extern si_t register_application(si_t video_access_mode, si_t application_type, const char* name)
{
	struct packet_body_register_application body;
	request_set_register_application(&body, video_access_mode, application_type, name, strlen(name) + 1);
	return send_body_with_extra_context_and_return_respond(REQUEST_TYPE_REGISTER_APPLICATION, RESPOND_TYPE_REGISTER_APPLICATION,
            (const_addr_t)&body, sizeof(body),
            (const_addr_t)name , strlen(name) + 1);
}

extern si_t cancel_application()
{
	return send_request_and_return_respond(REQUEST_TYPE_CANCEL_APPLICATION, RESPOND_TYPE_CANCEL_APPLICATION, NULL, 0);
}

extern si_t graphics_device_init(si_t x_axis, si_t y_axis, si_t x_size, si_t y_size, si_t r, si_t g, si_t b, si_t a, si_t font)
{
	struct packet_body_graphics_device_init body;
	request_set_graphics_device_init(&body, x_axis, y_axis, x_size, y_size, r, g, b, a, font);
	return send_request_and_return_respond(REQUEST_TYPE_GRAPHICS_DEVICE_INIT, RESPOND_TYPE_GRAPHICS_DEVICE_INIT,
            (const_addr_t)&body, sizeof(body));
}

extern si_t graphics_device_exit(si_t gd)
{
	struct packet_body_graphics_device_exit body;
	request_set_graphics_device_exit(&body, gd);
	return send_request_and_return_respond(REQUEST_TYPE_GRAPHICS_DEVICE_EXIT, RESPOND_TYPE_GRAPHICS_DEVICE_EXIT,
            (const_addr_t)&body, sizeof(body));
}

extern si_t window_manager_quit()
{
	return send_request_and_return_respond(REQUEST_TYPE_WINDOW_MANAGER_QUIT, RESPOND_TYPE_WINDOW_MANAGER_QUIT,
            NULL, 0);
}

extern si_t update(si_t gd)
{
	struct packet_body_update body;
	request_set_update(&body, gd);
	return send_request_and_return_respond(REQUEST_TYPE_UPDATE, RESPOND_TYPE_UPDATE,
            (const_addr_t)&body, sizeof(body));
}

extern si_t desktop_dirty()
{
	return send_request_and_return_respond(REQUEST_TYPE_DESKTOP_DIRTY, RESPOND_TYPE_DESKTOP_DIRTY,
            NULL, 0);
}

extern si_t register_window(si_t parent_window_descriptor, const char* title, si_t x, si_t y, si_t width, si_t height, si_t minimize_enable, si_t maximize_enable, si_t modal)
{
	struct packet_body_register_window body;
	request_set_register_window(&body, parent_window_descriptor, title, strlen(title) + 1, x, y, width, height, minimize_enable, maximize_enable, modal);
	/**
	 * 注册窗口较为复杂的请求包：标准请求包外加窗口标题字符串
	 **/
	return send_body_with_extra_context_and_return_respond(REQUEST_TYPE_REGISTER_WINDOW, RESPOND_TYPE_REGISTER_WINDOW,
            (const_addr_t)&body, sizeof(body),
            (const_addr_t)title, strlen(title) + 1);
}

extern si_t cancel_window(si_t window_descriptor)
{
	struct packet_body_cancel_window body;
	request_set_cancel_window(&body, window_descriptor);
	return send_request_and_return_respond(REQUEST_TYPE_CANCEL_WINDOW, RESPOND_TYPE_CANCEL_WINDOW,
            (const_addr_t)&body, sizeof(body));
}

extern si_t move_window(si_t window_descriptor, si_t x_axis, si_t y_axis)
{
	struct packet_body_move_window body;
	request_set_move_window(&body, window_descriptor, x_axis, y_axis);
	return send_request_and_return_respond(REQUEST_TYPE_MOVE_WINDOW, RESPOND_TYPE_MOVE_WINDOW,
            (const_addr_t)&body, sizeof(body));
}

extern si_t resize_window(si_t window_descriptor, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size)
{
	struct packet_body_resize_window body;
	request_set_resize_window(&body, window_descriptor, x_axis, y_axis, x_size, y_size);
	return send_request_and_return_respond(REQUEST_TYPE_RESIZE_WINDOW, RESPOND_TYPE_RESIZE_WINDOW,
            (const_addr_t)&body, sizeof(body));
}

extern si_t minimize_window(si_t window_descriptor)
{
	struct packet_body_minimize_window body;
	request_set_minimize_window(&body, window_descriptor);
	return send_request_and_return_respond(REQUEST_TYPE_MINIMIZE_WINDOW, RESPOND_TYPE_MINIMIZE_WINDOW,
            (const_addr_t)&body, sizeof(body));
}

extern si_t maximize_window(si_t window_descriptor, struct rectangle* result)
{
	struct packet_body_maximize_window body;
	request_set_maximize_window(&body, window_descriptor);
	return send_request_and_recv_respond(REQUEST_TYPE_MAXIMIZE_WINDOW, RESPOND_TYPE_MAXIMIZE_WINDOW,
            (const_addr_t)&body, sizeof(body), result);
}

extern si_t restore_window(si_t window_descriptor, struct rectangle* area)
{
	struct packet_body_restore_window body;
	request_set_restore_window(&body, window_descriptor, area->x, area->y, area->width, area->height);
	return send_request_and_return_respond(REQUEST_TYPE_RESTORE_WINDOW, RESPOND_TYPE_RESTORE_WINDOW,
            (const_addr_t)&body, sizeof(body));
}

extern si_t activate_window(si_t window_descriptor)
{
	struct packet_body_activate_window body;
	request_set_activate_window(&body, window_descriptor);
	return send_request_and_return_respond(REQUEST_TYPE_ACTIVATE_WINDOW, RESPOND_TYPE_ACTIVATE_WINDOW,
            (const_addr_t)&body, sizeof(body));
}

extern si_t clear(si_t gd)
{
	struct packet_body_clear body;
	request_set_clear(&body, gd);
	return send_request_and_return_respond(REQUEST_TYPE_CLEAR, RESPOND_TYPE_CLEAR,
            (const_addr_t)&body, sizeof(body));
}

extern si_t clear_black(si_t gd)
{
	struct packet_body_clear_black body;
	request_set_clear_black(&body, gd);
	return send_request_and_return_respond(REQUEST_TYPE_CLEAR_BLACK, RESPOND_TYPE_CLEAR_BLACK,
            (const_addr_t)&body, sizeof(body));
}

extern si_t clear_white(si_t gd)
{
	struct packet_body_clear_white body;
	request_set_clear_white(&body, gd);
	return send_request_and_return_respond(REQUEST_TYPE_CLEAR_WHITE, RESPOND_TYPE_CLEAR_WHITE,
            (const_addr_t)&body, sizeof(body));
}

extern si_t draw_point(si_t gd, si_t x, si_t y)
{
	struct packet_body_draw_point body;
	request_set_draw_point(&body, gd, x, y);
	return send_request_and_return_respond(REQUEST_TYPE_DRAW_POINT, RESPOND_TYPE_DRAW_POINT,
            (const_addr_t)&body, sizeof(body));
}

extern si_t draw_line(si_t gd, si_t x1, si_t y1, si_t x2, si_t y2)
{
	struct packet_body_draw_line body;
	request_set_draw_line(&body, gd, x1, y1, x2, y2);
	return send_request_and_return_respond(REQUEST_TYPE_DRAW_LINE, RESPOND_TYPE_DRAW_LINE,
            (const_addr_t)&body, sizeof(body));
}

extern si_t draw_rectangle(si_t gd, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size)
{
	struct packet_body_draw_rectangle body;
	request_set_draw_rectangle(&body, gd, x_axis, y_axis, x_size, y_size);
	return send_request_and_return_respond(REQUEST_TYPE_DRAW_RECTANGLE, RESPOND_TYPE_DRAW_RECTANGLE,
            (const_addr_t)&body, sizeof(body));
}

extern si_t draw_polygon(si_t gd, const struct point * p, ui_t point_count)
{
	struct packet_body_draw_polygon body;
	request_set_draw_polygon(&body, gd, p, point_count);
	/**
	 * 画多边型需要较为复杂的请求包：标准请求包外加每个点的信息
	 **/
	return send_body_with_extra_context_and_return_respond(REQUEST_TYPE_DRAW_POLYGON, RESPOND_TYPE_DRAW_POLYGON,
            (const_addr_t)&body, sizeof(body),
            (const_addr_t)p    , point_count * sizeof(struct point));
}

extern si_t draw_circle(si_t gd, si_t x, si_t y, si_t radius)
{
	struct packet_body_draw_circle body;
	request_set_draw_circle(&body, gd, x, y, radius);
	return send_request_and_return_respond(REQUEST_TYPE_DRAW_CIRCLE, RESPOND_TYPE_DRAW_CIRCLE,
            (const_addr_t)&body, sizeof(body));
}

extern si_t draw_ellipse(si_t gd, si_t x, si_t y, si_t a, si_t b)
{
	struct packet_body_draw_ellipse body;
	request_set_draw_ellipse(&body, gd, x, y, a, b);
	return send_request_and_return_respond(REQUEST_TYPE_DRAW_ELLIPSE, RESPOND_TYPE_DRAW_ELLIPSE,
            (const_addr_t)&body, sizeof(body));
}

extern si_t draw_x(si_t gd, si_t x, si_t y, si_t a, si_t b)
{
	struct packet_body_draw_x body;
	request_set_draw_x(&body, gd, x, y, a, b);
	return send_request_and_return_respond(REQUEST_TYPE_DRAW_X, RESPOND_TYPE_DRAW_X,
            (const_addr_t)&body, sizeof(body));
}

extern si_t draw_arrow(si_t gd, si_t x, si_t y, si_t size, si_t direction)
{
	struct packet_body_draw_arrow body;
	request_set_draw_arrow(&body, gd, x, y, size, direction);
	return send_request_and_return_respond(REQUEST_TYPE_DRAW_ARROW, RESPOND_TYPE_DRAW_ARROW,
            (const_addr_t)&body, sizeof(body));
}

extern si_t draw_img(si_t gd, const char* img_path, si_t align)
{
	struct packet_body_draw_img body;
	request_set_draw_img(&body, gd, img_path, strlen(img_path) + 1, align);
	/**
	 * 画图需要较为复杂的请求包：标准请求包外加路径字符串内容
	 **/
	return send_body_with_extra_context_and_return_respond(REQUEST_TYPE_DRAW_IMG, RESPOND_TYPE_DRAW_IMG,
            (const_addr_t)&body   , sizeof(body),
            (const_addr_t)img_path, strlen(img_path) + 1);
}

extern si_t draw_bitmap(si_t gd, const char* name, int align)
{
	struct packet_body_draw_bitmap body;
	request_set_draw_bitmap(&body, gd, name, strlen(name) + 1, align);
	/**
	 * 画图需要较为复杂的请求包：标准请求包外加路径字符串内容
	 **/
	return send_body_with_extra_context_and_return_respond(REQUEST_TYPE_DRAW_BITMAP, RESPOND_TYPE_DRAW_BITMAP,
            (const_addr_t)&body, sizeof(body),
            (const_addr_t)name, strlen(name) + 1);
}

extern si_t fill_rectangle(si_t gd, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size)
{
	struct packet_body_fill_rectangle body;
	request_set_fill_rectangle(&body, gd, x_axis, y_axis, x_size, y_size);
	return send_request_and_return_respond(REQUEST_TYPE_FILL_RECTANGLE, RESPOND_TYPE_FILL_RECTANGLE,
            (const_addr_t)&body, sizeof(body));
}

extern si_t fill_circle(si_t gd, si_t x, si_t y, si_t radius)
{
	struct packet_body_fill_circle body;
	request_set_fill_circle(&body, gd, x, y, radius);
	return send_request_and_return_respond(REQUEST_TYPE_FILL_CIRCLE, RESPOND_TYPE_FILL_CIRCLE,
            (const_addr_t)&body, sizeof(body));
}

extern si_t fill_ellipse(si_t gd, si_t x, si_t y, si_t a, si_t b)
{
	struct packet_body_fill_ellipse body;
	request_set_fill_ellipse(&body, gd, x, y, a, b);
	return send_request_and_return_respond(REQUEST_TYPE_FILL_ELLIPSE, RESPOND_TYPE_FILL_ELLIPSE,
            (const_addr_t)&body, sizeof(body));
}

extern si_t fill_x(si_t gd, si_t x, si_t y, si_t a, si_t b)
{
	struct packet_body_fill_x body;
	request_set_fill_x(&body, gd, x, y, a, b);
	return send_request_and_return_respond(REQUEST_TYPE_FILL_X, RESPOND_TYPE_FILL_X,
            (const_addr_t)&body, sizeof(body));
}

extern si_t fill_arrow(si_t gd, si_t x, si_t y, si_t size, si_t direction)
{
	struct packet_body_fill_arrow body;
	request_set_fill_arrow(&body, gd, x, y, size, direction);
	return send_request_and_return_respond(REQUEST_TYPE_FILL_ARROW, RESPOND_TYPE_FILL_ARROW,
            (const_addr_t)&body, sizeof(body));
}

extern si_t fill_polygon(si_t gd, const struct point * p, ui_t point_count)
{
	struct packet_body_fill_polygon body;
	request_set_fill_polygon(&body, gd, p, point_count);
	/**
	 * 画多边型需要较为复杂的请求包：标准请求包外加每个点的信息
	 **/
	return send_body_with_extra_context_and_return_respond(REQUEST_TYPE_FILL_POLYGON, RESPOND_TYPE_FILL_POLYGON,
            (const_addr_t)&body, sizeof(body),
            (const_addr_t)p    , point_count * sizeof(struct point));
}

extern si_t cpy_area(si_t gd, si_t mode, si_t dst_x, si_t dst_y, si_t src_x, si_t src_y, si_t x_size, si_t y_size)
{
	struct packet_body_cpy_area body;
	request_set_cpy_area(&body, gd, mode, dst_x, dst_y, src_x, src_y, x_size, y_size);
	return send_request_and_return_respond(REQUEST_TYPE_CPY_AREA, RESPOND_TYPE_CPY_AREA,
            (const_addr_t)&body, sizeof(body));
}

extern si_t show_text(si_t gd, si_t x, si_t y, const char * str, si_t size)
{
	struct packet_body_show_text body;
	request_set_show_text(&body, gd, x, y, str, size);
	/**
	 * 显示字符串需要较为复杂的请求包：标准请求包外加字符串内容
	 **/
	return send_body_with_extra_context_and_return_respond(REQUEST_TYPE_SHOW_TEXT, RESPOND_TYPE_SHOW_TEXT,
            (const_addr_t)&body, sizeof(body),
            (const_addr_t)str  , strlen(str) + 1);
}

extern si_t get_area(si_t gd, struct rectangle * result)
{
	struct packet_body_get_area body;
	request_set_get_area(&body, gd);
	return send_request_and_recv_respond(REQUEST_TYPE_GET_AREA, RESPOND_TYPE_GET_AREA,
            (const_addr_t)&body, sizeof(body),
            (addr_t)result);
}

extern si_t get_color(si_t gd, struct color * result)
{
	struct packet_body_get_color body;
	request_set_get_color(&body, gd);
	return send_request_and_recv_respond(REQUEST_TYPE_GET_COLOR, RESPOND_TYPE_GET_COLOR,
            (const_addr_t)&body, sizeof(body),
            (addr_t)result);
}

extern si_t get_color_limit(struct color_limit * result)
{
	return send_request_and_recv_respond(REQUEST_TYPE_GET_COLOR_LIMIT, RESPOND_TYPE_GET_COLOR_LIMIT,
            NULL, 0,
            (addr_t)result);
}

extern si_t get_font_width(si_t gd)
{
	struct packet_body_get_font_width body;
	request_set_get_font_width(&body, gd);
	return send_request_and_return_respond(REQUEST_TYPE_GET_FONT_WIDTH, RESPOND_TYPE_GET_FONT_WIDTH,
            (const_addr_t)&body, sizeof(body));
}

extern si_t get_font_height(si_t gd)
{
	struct packet_body_get_font_height body;
	request_set_get_font_height(&body, gd);
	return send_request_and_return_respond(REQUEST_TYPE_GET_FONT_HEIGHT, RESPOND_TYPE_GET_FONT_HEIGHT,
            (const_addr_t)&body, sizeof(body));
}

extern si_t get_screen_width()
{
	return send_request_and_return_respond(REQUEST_TYPE_GET_SCREEN_WIDTH, RESPOND_TYPE_GET_SCREEN_WIDTH,
            NULL, 0);
}

extern si_t get_screen_height()
{
	return send_request_and_return_respond(REQUEST_TYPE_GET_SCREEN_HEIGHT, RESPOND_TYPE_GET_SCREEN_HEIGHT,
            NULL, 0);
}

extern si_t set_area(si_t gd, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size)
{
	struct packet_body_set_area body;
	request_set_set_area(&body, gd,  x_axis,  y_axis,  x_size,  y_size);
	return send_request_and_return_respond(REQUEST_TYPE_SET_AREA, RESPOND_TYPE_SET_AREA,
            (const_addr_t)&body, sizeof(body));
}

extern si_t set_color(si_t gd, ui_t r, ui_t g, ui_t b, ui_t a)
{
	struct packet_body_set_color body;
	request_set_set_color(&body, gd, r, g, b, a);
	return send_request_and_return_respond(REQUEST_TYPE_SET_COLOR, RESPOND_TYPE_SET_COLOR,
            (const_addr_t)&body, sizeof(body));
}

extern si_t set_font(si_t gd, si_t font)
{
	struct packet_body_set_font body;
	request_set_set_font(&body, gd, font);
	return send_request_and_return_respond(REQUEST_TYPE_SET_FONT, RESPOND_TYPE_SET_FONT,
            (const_addr_t)&body, sizeof(body));
}

extern char* get_window_title(si_t window_descriptor)
{
	struct packet_body_get_window_title body;
	char* title = NULL;
	request_set_window_title(&body, window_descriptor);
	send_request_and_malloc_for_respond(REQUEST_TYPE_SET_FONT, RESPOND_TYPE_SET_FONT,
            (const_addr_t)&body, sizeof(body),
            (addr_t)&title);
	return title;
}
