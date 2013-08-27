/**
 * @file application_info.h
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

#ifndef _SERVER_APP_H_
#define _SERVER_APP_H_ 1

#include "base_type.h"
#include "graph.h"
#include "comm.h"
#include "data_structures.h"
#include "window_info.h"

/* 服务器模块保存的客户端的信息  */
struct application_info
{
    /**
	 * 通信句柄
	 **/
	struct egui_uds uds;

	/**
	 * 应用程序类型：
	 * APPLICATION_TYPE_NORMAL 普通应用程序
	 * APPLICATION_TYPE_DESKTOP 桌面应用程序
	 * APPLICATION_TYPE_WINDOW_MANAGER_EXIT 可关闭窗口管理器的程序
	 **/
	si_t application_type;

    /**
	 * 视频
	 **/
    struct screen screen;

    /**
     * 用户应用程序拥有所有窗口向量
     *
     * vector中的元素是以 struct object node 为根的二叉树
     * XXX: 二叉树只有一个根节点，保存三个一样的指针，指向一个
     * struct window_info
     *
     * 二叉树的成员的类型是 struct window_info
    **/
    struct vector window_info_vector;

    /**
     * 保存用户应用程序申请的所有绘图设备.
     *
     * 成员的类型是 struct graphics_device *
    **/
    struct vector gd_ptr_vector;

    /**
     * application name
     * of which icon will be painted
     **/
    char* name;
};

/**
 * 设置应用程序
 *
 * @param app_info_ptr 应用程序指针
 * @param video_access_mode 视频访问模式
 * @param application_type 应用程序类型
 * @param app_name 应用程序名称
 *
 * @return 成功返回0 否则返回-1
 **/
extern si_t application_info_set(struct application_info* app_info_ptr, si_t video_access_mode, si_t application_type, char* app_name);

/**
 * @brief 初始化应用程序
 *
 * @param app_info_ptr 应用程序指针
 **/
extern void application_info_init(struct application_info* app_info_ptr);

/**
 * @brief 清理应用程序
 *
 * @param app_info_ptr 应用程序指针
 **/
extern void application_info_exit(struct application_info* app_info_ptr);

/**
 * @brief 活动窗口在应用程序中的下标
 *
 * @param app_info_ptr 应用程序指针
 * @param ptr 窗口指针
 *
 * @return 找到返回下标，否则返回-1
 **/
extern si_t application_info_get_win_index(struct application_info* app_info_ptr, struct window_info* ptr);

#endif 

