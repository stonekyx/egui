/**
 * @file window_manager.h
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

# ifndef _WINDOW_MANAGER_H_
# define _WINDOW_MANAGER_H_ 1

#include "base_type.h"
#include "comm.h"
#include "graph.h"
#include "data_structures.h"
#include "geometry.h"

/* 
 * @brief 服务器信息  
 */
struct window_manager
{
	/**
	 * 通信句柄
	 **/
    struct egui_uds uds;	
	/**
	 * 监听事件句柄
	 **/
	struct event_listener server_listener;

	/**
	 * 图形设备句柄
	 **/
    si_t gd_handler;

	/**
	 * 背景色
	 **/
	struct color backcolor;

	/**
	 * 窗口区域
	 **/
	struct rectangle work_area;

	/**
	 * 所有输入设备数组 元素为struct input_device
	 **/
    struct vector input_device_vector;
	
    /**
	 * 保存由输入事件转换而来的消息 元素为union message
    **/
    struct list message_list;

	/**
	 * 所有应用程序数组 元素为struct application_info
	 **/
    struct vector application_info_vector;
 
	/**
	 * 桌面程序
	 **/
	struct application_info* desktop_app_ptr;

    /**
     * 当前活动的用户应用程序
    **/
    struct application_info * active_app_info_ptr;

    /**
     * 当前活动的窗口
    **/
    struct window_info * active_win_info_ptr;

	/* 光标 */
	struct point old_cursor;
	struct point new_cursor;
    si_t cursor_shape;
};

extern struct window_manager global_wm;

/**
 * 初始化窗口管理器
 **/
extern si_t window_manager_init();

/**
 * 清理窗口管理器
 **/
extern si_t window_manager_exit();

/**
 * 运行窗口管理器
 **/
extern si_t window_manager_run();

/**
 * @brief 获得应用程序在窗口管理器数组下标
 *
 * @param ptr 应用程序指针
 *
 * @return 找到返回下标，否则返回-1
 **/
extern si_t window_manager_get_app_index(struct application_info* ptr);

/**
 * @brief 窗口管理器接受客户端连接的处理函数
 *
 * @param uds_ptr 窗口管理器的通信句柄
 * @param arg 空
 *
 * @return 0
 **/
extern si_t window_manager_accept_handler(struct egui_uds* uds_ptr, addr_t arg);

/**
 * @brief 窗口管理器接受输入事件的处理函数
 *
 * @param uds_ptr 窗口管理器的通信句柄
 * @param arg 空
 *
 * @return 0
 **/
extern si_t window_manager_input_handler(struct egui_uds* uds_ptr, addr_t arg);

/**
 * @brief 窗口管理器输入设备清理函数
 *
 * @param uds_ptr 窗口管理器的通信句柄
 * @param arg 输入设备指针
 **/
extern void window_manager_input_destructor(struct egui_uds* uds, addr_t arg);

/**
 * @brief 窗口管理器设置来自客户端请求的处理函数
 **/
extern int window_manager_set_request_handler();

#endif
