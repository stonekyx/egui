/**
 * @file window_manager.c
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
# define _GNU_SOURCE
#include "window_manager.h"

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>

#include "application_info.h"
#include "window_info.h"
#include "event.h"
#include "log.h"
#include "cursor.h"

struct window_manager global_wm;
struct termios term;

/**
 * @brief 初始化终端
 *
 * @return 成功返回0 否则返回-1
 **/
si_t terminal_init()
{
	struct termios new_term = term;
	si_t fd = open((char*)ctermid(NULL), O_RDONLY);
	if(fd < 0)
	{
		EGUI_PRINT_SYS_ERROR("failed to open termios. open()");
		return -1;
	}

	if(tcgetattr(fd, &term) < 0)
	{
		EGUI_PRINT_SYS_ERROR("failed to get termios attribute. tcgetattr()");
		return -1;
	}

	new_term.c_lflag = term.c_lflag & ~( ECHO | ECHOE | ECHOK | ECHONL);

	if(tcsetattr(fd, TCSAFLUSH, &new_term) < 0)
	{
		EGUI_PRINT_SYS_ERROR("failed to set termios attribute. tcgetattr()");
		return -1;
	}

	return 0;
}

/**
 * @brief 清理终端
 **/
void terminal_exit()
{
	si_t fd = open((char*)ctermid(NULL), O_RDONLY);
	if(fd < 0)
	{
		EGUI_PRINT_SYS_ERROR("failed to open termios. open()");
	}

	if(tcsetattr(fd, TCSAFLUSH, &term) < 0)
	{
		EGUI_PRINT_SYS_ERROR("failed to set termios attribute. tcgetattr()");
	}
}

/**
 * @brief 初始化配置文件
 **/
static si_t config_init()
{
	global_wm.backcolor.r = 0;
	global_wm.backcolor.g = 0;
	global_wm.backcolor.b = 0;
	global_wm.backcolor.a = 0;
	return 0;
}

/**
 * 清理配置文件
 **/
static si_t config_exit()
{
	return 0;
}

/**
 * @brief 初始化通信模块
 *
 * @param server_path 配置项 窗口管理器的socket路径
 *
 * @return 成功返回0 否则返回-1
 **/
static si_t comm_init(char* server_path)
{
	int i = 0;
	/**
	 * 初始化窗口管理器的通信句柄 用来接受用户应用程序的连接请求
	 **/
	if(0 != uds_init(&global_wm.uds, SOCK_STREAM, server_path, PEER_ROLE_SERVER))
	{
		EGUI_PRINT_ERROR("Failed to init server uds");
		return -1;
	}

	/**
	 * 初始化监听事件的event_listener
	 **/
	event_listener_init(&global_wm.server_listener);

	/**
	 * 添加event_listener监听来自窗口管理器通信句柄的读请求 若有读请求发生则调用window_manager_accept_handler处理
	 * 析构函数为简单关闭通信句柄
	 **/
	event_listener_add_read_event(&global_wm.server_listener, &global_wm.uds, NULL, window_manager_accept_handler, NULL);

	/**
	 * 设置对来自客户端的请求处理函数
	 *
	 * 当客户端建立连接后 在window_manager_accept_handler中 将调用server_lib的接口添加一个客户端
	 * server_lib添加客户端的具体实现是 将这个客户端句柄添加到event_listener里面监听读请求 并使用server_lib内部的事件处理函数
	 * server_lib内部的事件处理函数 将来自客户端的绘图请求直接调用绘图模块的函数处理 然后将函数返回值发回回应包
	 * 而来自客户端需要窗口管理器处理的窗口/应用程序管理类的请求 则通过窗口管理器在window_manger_set_request_handler里面设置的处理函数处理
	 **/
	window_manager_set_request_handler();

	/**
	 * 设置对来自输入设备的请求处理函数
	 * 事件处理函数为window_manager_input_handler 析构函数为input_exit对应的函数
	 **/
	for(i = 0; i != vector_size(&global_wm.input_device_vector); ++ i)
	{
		struct input_device* device_ptr = vector_at(&global_wm.input_device_vector, i);
		event_listener_add_read_event(&global_wm.server_listener, &device_ptr->uds, device_ptr, window_manager_input_handler, window_manager_input_destructor);
	}
	return 0;
}

/**
 * @brief 通信模块清理
 **/
static void comm_exit()
{
	/**
	 * 清理event_listener 清理的同时将会close所有的通信句柄
	 **/
	event_listener_exit(&global_wm.server_listener);
}

/**
 * @brief 输入事件模块初始化
 *
 * @param keybd_device_path 鼠标路径
 * @param mouse_device_path 键盘路径
 * @param double_click_delay 双击时间间隔
 *
 * @return 成功返回0 否则返回-1
 **/
static si_t event_init(char* keybd_device_path, char* mouse_device_path, si_t double_click_delay)
{
	struct input_device device;

	/**
	 * 初始化输入设备数组
	 **/
	vector_init(&global_wm.input_device_vector);

	/**
	 * 初始化键盘输入设备
	 **/
	if(keybd_init(&device, keybd_device_path) < 0)
	{
		EGUI_PRINT_ERROR("failed to init keybd device");
		return -1;
	}
	/**
	 * 将键盘输入设备添加到输入设备队列中
	 **/
	vector_push_back(&global_wm.input_device_vector, &device, sizeof(device));

	/**
	 * 初始化鼠标输入设备
	 **/
	if(mouse_init(&device, mouse_device_path, double_click_delay) < 0)
	{
		EGUI_PRINT_ERROR("failed to init mouse device");
		return -1;
	}
	/**
	 * 将键盘输入设备添加到输入设备队列中
	 **/
	vector_push_back(&global_wm.input_device_vector, &device, sizeof(device));

	/**
	 * 初始化消息队列
	 **/
	list_init(&global_wm.message_list);

	return 0;
}

/**
 * @brief 清理输入事件模块
 **/
static void event_exit()
{
	/**
	 * 清理输入设备数组 注意输入设备句柄由于已经添加到event_listener中 因此会自行close
	 **/
	vector_exit(&global_wm.input_device_vector);

	/**
	 * 清理消息队列
	 **/
	list_exit(&global_wm.message_list);
}

/**
 * @brief 初始化窗口管理器图形相关
 *
 * @param framebuffer_path framebuffer设备路径
 * @param top_margin 配置项 屏幕上方留白的像素
 * @param down_margin 配置项 屏幕下方留白的像素
 * @param left_margin 配置项 屏幕左方留白的像素
 * @param right_margin 配置项 屏幕右方留白的像素
 *
 * @return 成功返回0 否则返回-1
 **/
static si_t graph_init(char* framebuffer_path, si_t top_margin, si_t down_margin, si_t left_margin, si_t right_margin)
{       
	struct graphics_device* gd_ptr = NULL;

	/* 初始化屏幕 */
	screen_init(framebuffer_path);
	/* 设置颜色的限制 */
	screen_color_limit();

	/** 
	 * 初始化窗口管理器的图形设备
	 **/
	global_wm.gd_handler = engine_graphics_device_init(0, 0, global_screen.width, global_screen.height, 0,0,0,0,0);
	if(0 == global_wm.gd_handler)
	{
		EGUI_PRINT_ERROR("failed to init window manager graph device.");
		return -1;
	}
	gd_ptr = (struct graphics_device*)global_wm.gd_handler;
	gd_ptr->screen.memory_addr = global_screen.memory_addr;
	gd_ptr->screen.buffer_addr = NULL;
	gd_ptr->screen.video_access_mode = VIDEO_ACCESS_MODE_DIRECT;

	/**
	 * 初始化窗口区域
	 **/
	rectangle_set(&global_wm.work_area,
		0 + left_margin, 0 + top_margin, global_screen.width - left_margin - right_margin, global_screen.height - top_margin - down_margin);

	/** 绘制桌面 **/
	engine_set_color(global_wm.gd_handler, global_wm.backcolor.r, global_wm.backcolor.g, global_wm.backcolor.b, global_wm.backcolor.a);
	engine_fill_rectangle(global_wm.gd_handler, global_wm.work_area.x, global_wm.work_area.y, global_wm.work_area.width, global_wm.work_area.height);

	return 0;
}

/**
 * 清理图形模块
 **/
static void graph_exit()
{
	/** 刷黑屏幕 **/
	engine_clear_black(global_wm.gd_handler);
	/** 清理屏幕 **/
	screen_exit();
	/** 清理窗口管理器图形设备 */
	engine_graphics_device_exit(global_wm.gd_handler);
}

/**
 * @brief 应用程序初始化
 *
 * @param frame_size 配置项 窗口边框宽度
 * @param title_bar_size 配置项 窗口标题栏高度
 *
 **/
static void applications_init(si_t frame_size, si_t title_bar_size)
{
	/**
	 * 初始化应用程序数组
	 **/
	vector_init(&global_wm.application_info_vector);

	/** 桌面程序指针初始化 **/
	global_wm.desktop_app_ptr = NULL;

	/** 活动窗口/应用程序 初始化 **/
	global_wm.active_app_info_ptr = NULL;
	global_wm.active_win_info_ptr = NULL;

	/** 窗口配置 **/
	window_info_config(frame_size, title_bar_size);
}

/**
 * @brief 清理应用程序
 **/
static void applications_exit()
{
	si_t n = 0, i = 0;

	n = vector_size(&(global_wm.application_info_vector));
	for(i = 0; i < n; ++ i)
	{
		/* 第 i 个用户应用程序 */
		struct application_info * app_info_ptr = vector_at(&(global_wm.application_info_vector), i);
		application_info_exit(app_info_ptr);
	}

	vector_exit(&(global_wm.application_info_vector));
}

/**
 * 初始化窗口管理器
 * 就不写注释了 函数名写的已经很明白。。
 **/
si_t window_manager_init()
{
	if(0 != terminal_init())
	{
		EGUI_PRINT_ERROR("failed to init terminal");
		return -1;
	}

	if(0 != config_init())
	{
		EGUI_PRINT_ERROR("failed to init config");
		return -1;
	}

	if(0 != event_init("/dev/input/event8", "/dev/input/event10", 1000))
	{
		EGUI_PRINT_ERROR("failed to init event module");
		return -1;
	}

	if(0 != comm_init("/tmp/server"))
	{
		EGUI_PRINT_ERROR("failed to init comm module");
		return -1;
	}

	if(0 != graph_init("/dev/fb0", 0, 0, 0, 0))
	{
		EGUI_PRINT_ERROR("failed to init graph module");
		return -1;
	}

	if(0 != cursor_init("CURSOR_SHAPE_X"))
	{
		EGUI_PRINT_ERROR("failed to init cursor");
		return -1;
	}

	applications_init(3, 30);

	return 0;
}

/**
 * 清理窗口管理器
 * 就不写注释了 函数名写的已经很明白。。
 **/
si_t window_manager_exit()
{
	applications_exit();
	cursor_exit();
	graph_exit();
	comm_exit();
	event_exit();
	config_exit();
	terminal_exit();

	return 0;
}

si_t window_manager_run()
{
	return event_listener_exec(&global_wm.server_listener);
}

si_t window_manager_get_app_index(struct application_info* ptr)
{
	int i;
	i = vector_size(&global_wm.application_info_vector);
	while(--i >= 0)
		if((int)vector_at(&global_wm.application_info_vector, i) == (int)ptr)
			return i;
	return -1;
}

