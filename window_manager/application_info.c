/**
 * @file application_info.c
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

#include "application_info.h"
#include <stdlib.h>

#include "log.h"

extern si_t application_info_set(struct application_info* app_info_ptr, si_t video_access_mode, si_t application_type, char* app_name)
{
	/**
	 * 屏幕初始化
	 **/
	app_info_ptr->screen = global_screen;
    app_info_ptr->screen.video_access_mode = video_access_mode;
    app_info_ptr->screen.memory_addr = global_screen.buffer_addr;
    /* 分配缓冲区 */
    if(video_access_mode == VIDEO_ACCESS_MODE_BUFFER)
    {
        app_info_ptr->screen.buffer_addr = (byte_t *)malloc(app_info_ptr->screen.size);
        if(app_info_ptr->screen.buffer_addr == NULL)
        {
			EGUI_PRINT_SYS_ERROR("failed to malloc for screen buffer. malloc()");
            return -1;
        }
        /* 清零 */
        memset(app_info_ptr->screen.buffer_addr, 0, app_info_ptr->screen.size);
    }
    else if(video_access_mode == VIDEO_ACCESS_MODE_DIRECT)
    {
        app_info_ptr->screen.buffer_addr = NULL;
    }

	app_info_ptr->application_type = application_type;
	/**
	 * 应用程序名称
	 **/
	app_info_ptr->name = malloc(strlen(app_name) + 1);
	if(NULL == app_info_ptr->name)
	{
		EGUI_PRINT_SYS_ERROR("failed to malloc for application name %s. malloc()", app_name);
		return -1;
	}
	strncpy(app_info_ptr->name, app_name, strlen(app_name + 1));

	return 0;
}

extern void application_info_init(struct application_info* app_info_ptr)
{
	/* 设置用户应用程序信息结构 */
	memset(app_info_ptr, 0, sizeof(struct application_info));

	/* 初始化用户应用程序的窗口向量 */
	vector_init(&(app_info_ptr->window_info_vector));

	/* 初始化用户应用程序的图形设备指针向量 */
	vector_init(&app_info_ptr->gd_ptr_vector);
}

extern void application_info_exit(struct application_info* app_info_ptr)
{
	int n = 0, i = 0;

    /* 释放缓冲区 */
    if(VIDEO_ACCESS_MODE_BUFFER == app_info_ptr->screen.video_access_mode)
    {
        free(app_info_ptr->screen.buffer_addr);
    }
	free(app_info_ptr->name);

    /**
	 * 清理所有的图形设备 
	 **/
    n = vector_size(&(app_info_ptr->gd_ptr_vector));
    for(i = 0; i < n; ++ i)
    {
		int* gd_2_ptr = vector_at(&(app_info_ptr->gd_ptr_vector), i);
        engine_graphics_device_exit(* gd_2_ptr);
    }
    vector_exit(&(app_info_ptr->gd_ptr_vector));

	/**
	 * 清理所有的窗口
	 **/
    n = vector_size(&(app_info_ptr->window_info_vector));
    for(i = 0; i < n; ++ i)
    {
        struct object* tree = vector_at(&(app_info_ptr->window_info_vector), i);
        window_info_tree_exit((struct window_info*)tree->parent);
    }
    vector_exit(&(app_info_ptr->window_info_vector));

	if(0 != uds_exit(&app_info_ptr->uds))
	{
		EGUI_PRINT_ERROR("failed to exit uds in application");
	}
}

si_t application_info_get_win_index(struct application_info* app_info_ptr, struct window_info* ptr)
{
    si_t i;
    i = vector_size(&app_info_ptr->window_info_vector);
    while(-- i >= 0)
    {
        struct object* tree = vector_at(&app_info_ptr->window_info_vector, i);
        struct object* node = tree->lchild;
        while(node != NULL)
        {
            if((si_t)node == (si_t)ptr)
                return i;
            node = object_tree_iterator_increment(tree, node);
        }
    }
    return -1;
}

