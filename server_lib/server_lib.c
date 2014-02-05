/**
 * @file server_lib.c
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

# include "server_lib.h"

# include <string.h>
# include "graph.h"
# include "log.h"
# include "compiler.h"

static struct event_listener* server_lib_event_listener_ptr = NULL;

/** 处理函数的指针声明 {{{ **/
static void (*client_exit_handler)(addr_t) = NULL;
static si_t(*register_application_handler)(addr_t, si_t, si_t, char*) = NULL;
static si_t(*cancel_application_handler)(addr_t) = NULL;
static si_t(*graphics_device_init_handler)(addr_t, si_t) = NULL;
static si_t(*graphics_device_exit_handler)(addr_t, si_t) = NULL;
static si_t(*window_manager_exit_handler)(addr_t) = NULL;
static si_t(*desktop_dirty_handler)(addr_t) = NULL;
static si_t(*update_handler)(addr_t, si_t) = NULL;
static si_t(*activate_window_handler)(addr_t, si_t) = NULL;
static si_t(*register_window_handler)(addr_t, si_t, char*, si_t, si_t, si_t, si_t, si_t, si_t, si_t) = NULL;
static si_t(*cancel_window_handler)(addr_t, si_t) = NULL;
static si_t(*move_window_handler)(addr_t, si_t, si_t, si_t) = NULL;
static si_t(*resize_window_handler)(addr_t, si_t, si_t, si_t, si_t, si_t) = NULL;
static si_t(*minimize_window_handler)(addr_t, si_t) = NULL;
static si_t(*maximize_window_handler)(addr_t, si_t, struct rectangle*) = NULL;
static si_t(*restore_window_handler)(addr_t, si_t, struct rectangle*) = NULL;
static si_t(*get_window_title)(addr_t, si_t, char**) = NULL;
/** 处理函数的指针声明 }}} **/

/** 静态函数声明 {{{ **/
static si_t _event_listener_handler(struct egui_uds* uds_ptr, addr_t app_addr);
static si_t _server_lib_handler(addr_t body, si_t detail_type, union respond* respond_ptr, addr_t app_addr);
static si_t _server_lib_handle_request(addr_t body, si_t detail_type, union respond* respond_ptr, addr_t app_addr);
static si_t _server_lib_handle_graphics_ops(addr_t body, si_t detail_type, union respond* respond_ptr, addr_t app_addr);
static void _server_lib_exit_handler(struct egui_uds* uds_ptr, addr_t app_addr);
/** 静态函数声明 }}} **/

/**
 * 令event_listener退出的返回值
 **/
static const si_t EXIT_NUMBER = 255;

extern void server_lib_init(struct event_listener* upper_event_listener, void(*handler)(addr_t))
{
	server_lib_event_listener_ptr = upper_event_listener;
	client_exit_handler = handler;
}

extern void server_lib_add_new_client(struct egui_uds* uds_ptr, addr_t app_addr)
{
	event_listener_add_read_event(server_lib_event_listener_ptr, uds_ptr, app_addr, _event_listener_handler, _server_lib_exit_handler);
}

extern void add_register_application_handler(si_t(*handler)(addr_t, si_t, si_t, char*))
{
	register_application_handler = handler;
}

extern void add_cancel_application_handler(si_t(*handler)(addr_t))
{
	cancel_application_handler = handler;
}

extern void add_graphics_device_init_handler(si_t(*handler)(addr_t, si_t))
{
	graphics_device_init_handler = handler;
}

extern void add_graphics_device_exit_handler(si_t(*handler)(addr_t, si_t))
{
	graphics_device_exit_handler = handler;
}

extern void add_window_manager_exit_handler(si_t(*handler)(addr_t))
{
	window_manager_exit_handler = handler;
}

extern void add_desktop_dirty_handler(si_t(*handler)(addr_t))
{
	desktop_dirty_handler = handler;
}

extern void add_update_handler(si_t(*handler)(addr_t, si_t))
{
	update_handler = handler;
}

extern void add_activate_window_handler(si_t(*handler)(addr_t, si_t))
{
	activate_window_handler = handler;
}

extern void add_register_window_handler(si_t(*handler)(addr_t, si_t, char*, si_t, si_t, si_t, si_t, si_t, si_t, si_t))
{
	register_window_handler = handler;
}

extern void add_cancel_window_handler(si_t(*handler)(addr_t, si_t))
{
	cancel_window_handler = handler;
}

extern void add_move_window_handler(si_t(*handler)(addr_t, si_t, si_t, si_t))
{
	move_window_handler = handler;
}

extern void add_resize_window_handler(si_t(*handler)(addr_t, si_t, si_t, si_t, si_t, si_t))
{
	resize_window_handler = handler;
}

extern void add_minimize_window_handler(si_t(*handler)(addr_t, si_t))
{
	minimize_window_handler = handler;
}

extern void add_maximize_window_handler(si_t(*handler)(addr_t, si_t, struct rectangle*))
{
	maximize_window_handler = handler;
}

extern void add_restore_window_handler(si_t(*handler)(addr_t, si_t, struct rectangle*))
{
	restore_window_handler = handler;
}

extern void add_get_window_title_handler(si_t(*handler)(addr_t, si_t, char**))
{
	get_window_title = handler;
}

static void copy_message(union message* base_msg, union message* msg_to_be_sent)
{
	if(base_msg)
		memcpy(msg_to_be_sent, base_msg, sizeof(union message));
	else
		memset(msg_to_be_sent, 0, sizeof(union message));
}

extern si_t send_input_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter)
{
	message_set_window_descriptor(msg, window_descripter);
	return comm_send_message(uds_ptr, msg);
}

extern si_t send_widget_move_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter, si_t x_offset, si_t y_offset)
{
	union message msg_to_be_sent;
	struct point p = {0};
	point_set(&p, x_offset, y_offset);
	copy_message(msg, &msg_to_be_sent);
	message_set_widget_move(&msg_to_be_sent, window_descripter, &p);
	return comm_send_message(uds_ptr, &msg_to_be_sent);
}

extern si_t send_widget_resize_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size)
{
	union message msg_to_be_sent;
	struct rectangle area = {0};
	copy_message(msg, &msg_to_be_sent);
	rectangle_set(&area, x_axis, y_axis, x_size, y_size);
	message_set_widget_resize(&msg_to_be_sent, window_descripter, &area);
	return comm_send_message(uds_ptr, &msg_to_be_sent);
}

extern si_t send_window_activate_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter)
{
	union message msg_to_be_sent;
	copy_message(msg, &msg_to_be_sent);
	message_set_window_activate(&msg_to_be_sent, window_descripter);
	return comm_send_message(uds_ptr, &msg_to_be_sent);
}

extern si_t send_window_deactivate_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter)
{
	union message msg_to_be_sent;
	copy_message(msg, &msg_to_be_sent);
	message_set_window_deactivate(&msg_to_be_sent, window_descripter);
	return comm_send_message(uds_ptr, &msg_to_be_sent);
}

extern si_t send_window_minimize_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter)
{
	union message msg_to_be_sent;
	copy_message(msg, &msg_to_be_sent);
	message_set_window_minimize(&msg_to_be_sent, window_descripter);
	return comm_send_message(uds_ptr, &msg_to_be_sent);
}

extern si_t send_window_maximize_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter)
{
	union message msg_to_be_sent;
	copy_message(msg, &msg_to_be_sent);
	message_set_window_maximize(&msg_to_be_sent, window_descripter);
	return comm_send_message(uds_ptr, &msg_to_be_sent);
}

extern si_t send_window_restore_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter)
{
	union message msg_to_be_sent;
	copy_message(msg, &msg_to_be_sent);
	message_set_window_restore(&msg_to_be_sent, window_descripter);
	return comm_send_message(uds_ptr, &msg_to_be_sent);
}

extern si_t send_window_close_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter)
{
	union message msg_to_be_sent;
	copy_message(msg, &msg_to_be_sent);
	message_set_window_close(&msg_to_be_sent, window_descripter);
	return comm_send_message(uds_ptr, &msg_to_be_sent);
}

extern si_t send_window_register_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter)
{
	union message msg_to_be_sent;
	copy_message(msg, &msg_to_be_sent);
	message_set_window_register(&msg_to_be_sent, window_descripter);
	return comm_send_message(uds_ptr, &msg_to_be_sent);
}

extern si_t send_window_cancel_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter)
{
	union message msg_to_be_sent;
	copy_message(msg, &msg_to_be_sent);
	message_set_window_cancel(&msg_to_be_sent, window_descripter);
	return comm_send_message(uds_ptr, &msg_to_be_sent);
}

extern si_t send_application_quit_message(struct egui_uds* uds_ptr)
{
	/**
	 * 其实只需要发送头部 但是这样为了一个application quit还要增加一种特殊的msg 所以就只填负载的type了
	 **/
	union message msg_to_be_sent;
	memset(&msg_to_be_sent, 0, sizeof(msg_to_be_sent));
	message_set_type(&msg_to_be_sent, MESSAGE_TYPE_APPLICATION_QUIT);
	return comm_send_message(uds_ptr, &msg_to_be_sent);
}

static si_t _event_listener_handler(struct egui_uds* uds_ptr, addr_t app_addr)
{
	si_t ret = comm_recv_request_and_send_respond(uds_ptr, app_addr, _server_lib_handler);
	if(-1 == ret)
	{
		EGUI_PRINT_ERROR("failed to recv request or send respond");
		return SELECTER_RETURN_TYPE_CONTINUE;
	}
	return ret;
}

static si_t _server_lib_handler(addr_t body, si_t detail_type, union respond* respond_ptr, addr_t app_addr)
{
	si_t ret = _server_lib_handle_graphics_ops(body, detail_type, respond_ptr, app_addr);
	if(ret == -1)
		ret = _server_lib_handle_request(body, detail_type, respond_ptr, app_addr);
	return ret;
}

static si_t _server_lib_handle_request(addr_t body, si_t detail_type, union respond* respond_ptr, addr_t app_addr)
{
	/** 存储调用实际操作函数结果的变量 **/
	si_t func_ret = 0;
	/** 存储_server_lib_handle_request()返回值的变量 **/
	si_t handler_ret = SELECTER_RETURN_TYPE_CONTINUE;
	switch(detail_type)
	{
	case REQUEST_TYPE_REGISTER_APPLICATION:
		{
			si_t video_access_mode = 0, application_type = 0;
			char* app_name = NULL;
			request_get_register_application(body, &video_access_mode, &application_type, &app_name, NULL);
			func_ret = register_application_handler(app_addr, video_access_mode, application_type, app_name);
			respond_set_normal(respond_ptr, RESPOND_TYPE_REGISTER_APPLICATION, func_ret);
		}
		break;
	case REQUEST_TYPE_CANCEL_APPLICATION:
		respond_set_normal(respond_ptr, RESPOND_TYPE_CANCEL_APPLICATION, 0);
		handler_ret = SELECTER_RETURN_TYPE_REMOVE_HANDLER;
		break;
		/**
		 * 处理初始化/清理图形设备的请求：
		 * 首先从请求包中获取必要参数，
		 * 接着调用graph模块的函数对图形设备初始化/清理，
		 * 接着将获得的参数和应用程序指针传递给上层的回调函数进行处理
		 * 最后将上层回调函数的返回值作为回应包的返回值
		 **/
	case REQUEST_TYPE_GRAPHICS_DEVICE_INIT:
		{
			si_t gd = request_call_by_graphics_device_init(body, engine_graphics_device_init);
			func_ret = graphics_device_init_handler(app_addr, gd);
			respond_set_normal(respond_ptr, RESPOND_TYPE_GRAPHICS_DEVICE_INIT, gd);
		}
		break;
	case REQUEST_TYPE_GRAPHICS_DEVICE_EXIT:
		{
			si_t gd = 0;
			request_get_graphics_device_exit(body, &gd);
			func_ret = request_call_by_graphics_device_exit(body, engine_graphics_device_exit);
			func_ret = graphics_device_exit_handler(app_addr, gd);
			respond_set_normal(respond_ptr, RESPOND_TYPE_GRAPHICS_DEVICE_EXIT, func_ret);
		}
		break;
		/**
		 * 接到window_manager_quit命令 若成功执行则返回退出循环的
		 **/
	case REQUEST_TYPE_WINDOW_MANAGER_QUIT:
		func_ret = window_manager_exit_handler(app_addr);
		respond_set_normal(respond_ptr, RESPOND_TYPE_WINDOW_MANAGER_QUIT, func_ret);
		if(0 == func_ret)
		{
			handler_ret = SELECTER_RETURN_TYPE_END;
		}
		break;
	case REQUEST_TYPE_DESKTOP_DIRTY:
		func_ret = desktop_dirty_handler(app_addr);
		respond_set_normal(respond_ptr, RESPOND_TYPE_DESKTOP_DIRTY, func_ret);
		break;
	case REQUEST_TYPE_UPDATE:
		{
			si_t gd = 0;
			request_get_update(body, &gd);
			func_ret = update_handler(app_addr, gd);
			respond_set_normal(respond_ptr, RESPOND_TYPE_UPDATE, func_ret);
		}
		break;
	case REQUEST_TYPE_ACTIVATE_WINDOW:
		{
			si_t window_descripter = 0;
			request_get_activate_window(body, &window_descripter);
			func_ret = activate_window_handler(app_addr, window_descripter);
			respond_set_normal(respond_ptr, RESPOND_TYPE_ACTIVATE_WINDOW, func_ret);
		}
		break;
	case REQUEST_TYPE_REGISTER_WINDOW:
		{
			si_t window_descripter = 0, x_axis = 0, y_axis = 0, x_size = 0, y_size = 0, maxable = 0, minable = 0, model = 0;
			char* title = NULL;
			request_get_register_window(body, &window_descripter, &title, NULL, &x_axis, &y_axis, &x_size, &y_size, &minable, &maxable, &model);
			func_ret = register_window_handler(app_addr, window_descripter, title, x_axis, y_axis, x_size, y_size, minable, maxable, model);
			respond_set_normal(respond_ptr, RESPOND_TYPE_REGISTER_WINDOW, func_ret);
		}
		break;
	case REQUEST_TYPE_CANCEL_WINDOW:
		{
			si_t window_descripter = 0;
			request_get_cancel_window(body, &window_descripter);
			func_ret = cancel_window_handler(app_addr, window_descripter);
			respond_set_normal(respond_ptr, RESPOND_TYPE_CANCEL_WINDOW, func_ret);
		}
		break;
	case REQUEST_TYPE_MOVE_WINDOW:
		{
			si_t window_descripter = 0, x_axis = 0, y_axis = 0;
			request_get_move_window(body, &window_descripter, &x_axis, &y_axis);
			func_ret = move_window_handler(app_addr, window_descripter, x_axis, y_axis);
			respond_set_normal(respond_ptr, RESPOND_TYPE_MOVE_WINDOW, func_ret);
		}
		break;
	case REQUEST_TYPE_RESIZE_WINDOW:
		{
			si_t window_descripter = 0, x_axis = 0, y_axis = 0, x_size = 0, y_size = 0;
			request_get_resize_window(body, &window_descripter, &x_axis, &y_axis, &x_size, &y_size);
			func_ret = resize_window_handler(app_addr, window_descripter, x_axis, y_axis, x_size, y_size);
			respond_set_normal(respond_ptr, RESPOND_TYPE_RESIZE_WINDOW, func_ret);
		}
		break;
	case REQUEST_TYPE_MINIMIZE_WINDOW:
		{
			si_t window_descripter = 0;
			request_get_minimize_window(body, &window_descripter);
			func_ret = minimize_window_handler(app_addr, window_descripter);
			respond_set_normal(respond_ptr, RESPOND_TYPE_MINIMIZE_WINDOW, func_ret);
		}
		break;
	case REQUEST_TYPE_MAXIMIZE_WINDOW:
		{
			si_t window_descripter = 0;
			struct rectangle area;
			request_get_maximize_window(body, &window_descripter);
			func_ret = maximize_window_handler(app_addr, window_descripter, &area);
			respond_set_rectangle(respond_ptr, RESPOND_TYPE_MAXIMIZE_WINDOW, &area);
		}
		break;
	case REQUEST_TYPE_RESTORE_WINDOW:
		{
			si_t window_descripter = 0;
			struct rectangle area;
			request_get_restore_window(body, &window_descripter, &area.x, &area.y, &area.width, &area.height);
			func_ret = restore_window_handler(app_addr, window_descripter, &area);
			respond_set_normal(respond_ptr, RESPOND_TYPE_RESTORE_WINDOW, func_ret);
		}
		break;
	case REQUEST_TYPE_GET_WINDOW_TITLE:
		{
			si_t window_descripter = 0;
			char* title = NULL;
			request_get_window_title(body, &window_descripter);
			get_window_title(app_addr, window_descripter, &title);
			respond_set_va(respond_ptr, RESPOND_TYPE_GET_WINDOW_TITLE, title, strlen(title) + 1);
		}
		break;
	}
	return handler_ret;
}

static si_t _server_lib_handle_graphics_ops(addr_t body, si_t detail_type, union respond* respond_ptr, addr_t app_addr)
{
	/** 存储调用实际操作函数结果的变量 **/
	si_t func_ret = 0;
	/** 存储_server_lib_handle_request()返回值的变量 **/
	si_t handler_ret = SELECTER_RETURN_TYPE_CONTINUE;

	NOT_USED(app_addr);
	switch(detail_type)
	{
	case REQUEST_TYPE_CLEAR:
		func_ret = request_call_by_clear(body, engine_clear);
		respond_set_normal(respond_ptr, RESPOND_TYPE_CLEAR, func_ret);
		break;
	case REQUEST_TYPE_CLEAR_WHITE:
		func_ret = request_call_by_clear_white(body, engine_clear_white);
		respond_set_normal(respond_ptr, RESPOND_TYPE_CLEAR_WHITE, func_ret);
		break;
	case REQUEST_TYPE_CLEAR_BLACK:
		func_ret = request_call_by_clear_black(body, engine_clear_black);
		respond_set_normal(respond_ptr, RESPOND_TYPE_CLEAR_BLACK, func_ret);
		break;
	case REQUEST_TYPE_CPY_AREA:
		func_ret = request_call_by_cpy_area(body, engine_cpy_area);
		respond_set_normal(respond_ptr, RESPOND_TYPE_CPY_AREA, func_ret);
		break;
	case REQUEST_TYPE_DRAW_ARROW:
		func_ret = request_call_by_draw_arrow(body, engine_draw_arrow);
		respond_set_normal(respond_ptr, RESPOND_TYPE_DRAW_ARROW, func_ret);
		break;
	case REQUEST_TYPE_DRAW_BITMAP:
		func_ret = request_call_by_draw_bitmap(body, engine_draw_bitmap);
		respond_set_normal(respond_ptr, RESPOND_TYPE_DRAW_BITMAP, func_ret);
		break;
	case REQUEST_TYPE_DRAW_CIRCLE:
		func_ret = request_call_by_draw_circle(body, engine_draw_circle);
		respond_set_normal(respond_ptr, RESPOND_TYPE_DRAW_CIRCLE, func_ret);
		break;
	case REQUEST_TYPE_DRAW_ELLIPSE:
		func_ret = request_call_by_draw_ellipse(body, engine_draw_ellipse);
		respond_set_normal(respond_ptr, RESPOND_TYPE_DRAW_ELLIPSE, func_ret);
		break;
	case REQUEST_TYPE_DRAW_IMG:
		func_ret = request_call_by_draw_img(body, engine_draw_img);
		respond_set_normal(respond_ptr, RESPOND_TYPE_DRAW_IMG, func_ret);
		break;
	case REQUEST_TYPE_DRAW_LINE:
		func_ret = request_call_by_draw_line(body, engine_draw_line);
		respond_set_normal(respond_ptr, RESPOND_TYPE_DRAW_LINE, func_ret);
		break;
	case REQUEST_TYPE_DRAW_POINT:
		func_ret = request_call_by_draw_point(body, engine_draw_point);
		respond_set_normal(respond_ptr, RESPOND_TYPE_DRAW_POINT, func_ret);
		break;
	case REQUEST_TYPE_DRAW_POLYGON:
		func_ret = request_call_by_draw_polygon(body, engine_draw_polygon);
		respond_set_normal(respond_ptr, RESPOND_TYPE_DRAW_POLYGON, func_ret);
		break;
	case REQUEST_TYPE_DRAW_RECTANGLE:
		func_ret = request_call_by_draw_rectangle(body, engine_draw_rectangle);
		respond_set_normal(respond_ptr, RESPOND_TYPE_DRAW_RECTANGLE, func_ret);
		break;
	case REQUEST_TYPE_DRAW_X:
		func_ret = request_call_by_draw_x(body, engine_draw_x);
		respond_set_normal(respond_ptr, RESPOND_TYPE_DRAW_X, func_ret);
		break;
	case REQUEST_TYPE_FILL_ARROW:
		func_ret = request_call_by_fill_arrow(body, engine_fill_arrow);
		respond_set_normal(respond_ptr, RESPOND_TYPE_FILL_ARROW, func_ret);
		break;
	case REQUEST_TYPE_FILL_CIRCLE:
		func_ret = request_call_by_fill_circle(body, engine_fill_circle);
		respond_set_normal(respond_ptr, RESPOND_TYPE_FILL_CIRCLE, func_ret);
		break;
	case REQUEST_TYPE_FILL_ELLIPSE:
		func_ret = request_call_by_fill_ellipse(body, engine_fill_ellipse);
		respond_set_normal(respond_ptr, RESPOND_TYPE_FILL_ELLIPSE, func_ret);
		break;
	case REQUEST_TYPE_FILL_RECTANGLE:
		func_ret = request_call_by_fill_rectangle(body, engine_fill_rectangle);
		respond_set_normal(respond_ptr, RESPOND_TYPE_FILL_RECTANGLE, func_ret);
		break;
	case REQUEST_TYPE_FILL_X:
		func_ret = request_call_by_fill_x(body, engine_fill_x);
		respond_set_normal(respond_ptr, RESPOND_TYPE_FILL_X, func_ret);
		break;
	case REQUEST_TYPE_FILL_POLYGON:
		func_ret = request_call_by_fill_polygon(body, engine_fill_polygon);
		respond_set_normal(respond_ptr, RESPOND_TYPE_FILL_POLYGON, func_ret);
		break;
	case REQUEST_TYPE_GET_COLOR_LIMIT:
		{
			struct color_limit func_ret = {0};
			engine_get_color_limit(&func_ret);
			respond_set_color_limit(respond_ptr, RESPOND_TYPE_GET_COLOR_LIMIT, &func_ret);
		}
		break;
	case REQUEST_TYPE_GET_SCREEN_WIDTH:
		func_ret = screen_get_width();
		respond_set_normal(respond_ptr, RESPOND_TYPE_GET_SCREEN_WIDTH, func_ret);
		break;
	case REQUEST_TYPE_GET_SCREEN_HEIGHT:
		func_ret = screen_get_height();
		respond_set_normal(respond_ptr, RESPOND_TYPE_GET_SCREEN_HEIGHT, func_ret);
		break;
	case REQUEST_TYPE_GET_COLOR:
		{
			struct color* func_ret_ptr = request_call_by_get_color(body, engine_get_color);
			respond_set_color(respond_ptr, RESPOND_TYPE_GET_COLOR, func_ret_ptr);
		}
		break;
	case REQUEST_TYPE_GET_AREA:
		{
			struct rectangle* func_ret_ptr = request_call_by_get_area(body, engine_get_area);
			respond_set_rectangle(respond_ptr, RESPOND_TYPE_GET_AREA, func_ret_ptr);
		}
		break;
	case REQUEST_TYPE_GET_FONT_WIDTH:
		func_ret = request_call_by_get_font_width(body, engine_get_font_width);
		respond_set_normal(respond_ptr, RESPOND_TYPE_GET_FONT_WIDTH, func_ret);
		break;
	case REQUEST_TYPE_GET_FONT_HEIGHT:
		func_ret = request_call_by_get_font_height(body, engine_get_font_height);
		respond_set_normal(respond_ptr, RESPOND_TYPE_GET_FONT_HEIGHT, func_ret);
		break;
	case REQUEST_TYPE_SET_AREA:
		func_ret = request_call_by_set_area(body, engine_set_area);
		respond_set_normal(respond_ptr, RESPOND_TYPE_SET_AREA, func_ret);
		break;
	case REQUEST_TYPE_SET_COLOR:
		func_ret = request_call_by_set_color(body, engine_set_color);
		respond_set_normal(respond_ptr, RESPOND_TYPE_SET_COLOR, func_ret);
		break;
	case REQUEST_TYPE_SET_FONT:
		func_ret = request_call_by_set_font(body, engine_set_font);
		respond_set_normal(respond_ptr, RESPOND_TYPE_SET_FONT, func_ret);
		break;
	case REQUEST_TYPE_SHOW_TEXT:
		func_ret = request_call_by_show_text(body, engine_show_text);
		respond_set_normal(respond_ptr, RESPOND_TYPE_SHOW_TEXT, func_ret);
		break;
	default:
		/**
		 * 不是绘制图形类的请求，接下来查看是否为窗口管理类请求
		 **/
		handler_ret = -1;
		break;
	}
	return handler_ret;
}

static void _server_lib_exit_handler(struct egui_uds* uds_ptr, addr_t app_addr)
{
	NOT_USED(uds_ptr);
	if(NULL != client_exit_handler)
	{
		client_exit_handler(app_addr);
	}
}
