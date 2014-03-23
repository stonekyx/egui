/**
 * @file application.h
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

# ifndef _APPLICATION_H_
# define _APPLICATION_H_ 1

# include "color.h"
# include "data_structures.h"
# include "comm.h"
# include "client_lib.h"

/**
 * 用户应用程序结构体
**/
struct application
{
    /**
     * name of the application
     * of which icon will be paint
     **/
    char* name;

	/**
	 * 应用程序类型
	 **/
	si_t application_type;

	/**
	 * 通信句柄
	 **/
	struct egui_uds uds;

	/**
	 * 事件监听句柄
	 **/
	struct event_listener app_event_listener;

	/**
	 * 消息队列
	 **/
	struct queue message_queue;

	/** 运行标志位 **/
	si_t exec_flag;

    /**
     * 指向主窗口
    **/
    struct window * main_window;

    /**
     * 拥有焦点的窗口部件，可以为空。
    **/
    struct window * focus;

    /**
     * focus_list
     * all the wigets that require for focus
     **/
    struct list focus_list;

    /**
     * 保存所有窗口
     *
     * 该向量的元素是 struct object
     * 每个元素是以该窗口为根结点的二叉树的附加节点
     * 它与 struct window 互为父子节点
    **/
    struct vector window_vector;

	/**
	 * 桌面应用程序对消息的处理函数
	 **/
	void (*desktop_msg_handler)(addr_t, union message*);
	addr_t desktop_ptr;

	/**
	 * icon路径 就是窗口的左上角的图案所在路径
	 **/
	char* icon_root_path;
	/**
	 * server uds path
	 **/
	char* server_path;
};

/**
 * 将地址强制转换成 struct application *
**/
# define APPLICATION_POINTER(addr) ((struct application *)(addr))


/**
 * 用户通过这个全局的指针能够访问用户应用程序结构体
**/
extern struct application global_application;

/**
 * @brief 使用前设置设置用户应用程序结构体
 *
 * @details 初始化 global_application
 *
 * @param video_access_mode 视频访问模式
 * @param application_type 应用程序类型
 * @param name: name of the application
 *
 * @return 0
**/
extern si_t application_init(si_t video_access_mode, si_t application_type, char* name);

/**
 * @brief 使用后清理用户应用程序结构体
 *
 * @details
 * 该函数会发送应用程序退出的请求，然后进行清理工作。
 * 也就是说调用这个函数表示主动要求退出。
 *
 * @return 0
**/
extern si_t application_exit();

/**
 * @brief 使用后清理用户应用程序结构体
 *
 * @details
 * 收到退出消息，调用该函数进行清理工作。
 * 也就是说这是被动的，不再发退出请求给窗口管理程序。
 *
 * @return 0
**/
extern si_t application_quit();

/**
 * @brief 应用程序主循环
 *
 * @return 0
 **/
extern si_t application_exec();

/**
 * @brief 设置桌面应用程序的桌面结构体和消息回调函数
 *
 * @param desktop_ptr 桌面结构体
 * @param desktop_handler 桌面消息回调函数 接受窗口激死激活注册注销等消息
 **/
extern void application_desktop_set(addr_t desktop_ptr, void (*desktop_handler)(addr_t, union message*));

# endif

