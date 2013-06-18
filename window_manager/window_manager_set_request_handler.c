/**
 * @file window_manger_set_request_handler.c
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
#include "window_info.h"
#include "window_manager.h"
#include "server_lib.h"
#include "window_info_iterator.h"
#include "cursor.h"
#include "flush_in_z_order.h"
#include "log.h"

#include <stdlib.h>
/**
 * @brief 处理应用程序注册请求的回调函数
 * 应用程序与窗口管理器建立连接是通过应用程序connect，窗口管理器accept建立的
 * 之后应用程序就应当发送一个注册应用程序的请求 以便完善窗口管理器这边维护的应用程序的信息
 *
 * @param app_ptr 应用程序的指针 应用程序结构体在窗口管理器accept之后就申请空间了 接下来是填充具体信息
 * @param video_access_mode 视频访问模式，每次绘制是直接绘到屏幕上，还是先绘到一个buffer上再一起绘到屏幕上
 * @param application_type 应用程序类型
 * @param app_name 应用程序的名字
 *
 * @return 正常返回0 出错返回-1
 **/
static si_t register_application_handler(addr_t app_ptr, si_t video_access_mode, si_t application_type, char* app_name)
{
    struct application_info * app_info_ptr = (struct application_info*)app_ptr;

	if(APPLICATION_TYPE_DESKTOP == application_type)
	{
		if(NULL != global_wm.desktop_app_ptr)
		{
			EGUI_PRINT_ERROR("failed to register two desktop application; original desktop is %s",
				global_wm.desktop_app_ptr->name);
			return -1;
		}
		global_wm.desktop_app_ptr = app_info_ptr;
	}

	if(0 != application_info_set(app_info_ptr, video_access_mode, application_type, app_name))
	{
		EGUI_PRINT_ERROR("failed to set application info %s", app_name);
		return -1;
	}

	return 0;
}

/**
 * @brief all_app_traversal_decrement配套使用 
 * 寻找倒数第一个未被最小化的窗口 并设置其为活动窗口，对应的应用程序为活动应用程序
 **/
static si_t _do_update_last_window(struct window_info_iterator* iter, addr_t arg)
{
	if(iter->win_info_ptr->area.x >= 0)
	{
		global_wm.active_app_info_ptr = iter->app_info_ptr;
		global_wm.active_win_info_ptr = iter->win_info_ptr;
		return 1;
	}
	return 0;
}
/**
 * @brief 注销应用程序请求处理函数
 *
 * @param app_ptr 应用程序指针
 **/
static si_t cancel_application_handler(addr_t app_ptr)
{
    struct application_info * app_info_ptr = (struct application_info*) app_ptr;
	int n = 0, i = 0;

	/** 在桌面注销窗口 **/
    n = vector_size(&(app_info_ptr->window_info_vector));
    for(i = 0; i < n; ++ i)
    {
        struct object* tree = (struct object*)vector_at(&(app_info_ptr->window_info_vector), i);

		/** 在桌面注销窗口 **/
		if(NULL != global_wm.desktop_app_ptr)
		{
			send_window_cancel_message(&global_wm.desktop_app_ptr->uds, NULL, (si_t)(tree)->parent);
		}
    }

	/** 清理应用程序信息 **/
	application_info_exit(app_info_ptr);

    /**
	 * 删除用户应用程序向量中相应的元素 
	 **/
	i = window_manager_get_app_index(app_info_ptr);
	vector_erase(&(global_wm.application_info_vector), i);

	/** 更新活动应用程序 **/
    if(app_info_ptr == global_wm.active_app_info_ptr)
	{
		struct window_info_iterator iter;
		window_info_iterator_clear(&iter);

		global_wm.active_win_info_ptr = NULL;
		global_wm.active_app_info_ptr = NULL;
		if(all_app_traversal_decrement(&iter, _do_update_last_window, NULL))
		{
			send_window_activate_message(&global_wm.active_app_info_ptr->uds, NULL, (si_t)global_wm.active_win_info_ptr);
			if(NULL != global_wm.desktop_app_ptr)
			{
				send_window_activate_message(&global_wm.desktop_app_ptr->uds, NULL, (si_t)iter.top_win_info_ptr);
			}
		}
	}

    /* 显示窗口前面的内容 */
    flush_in_z_order(0, 0, global_screen.width, global_screen.height);
    screen_flush(0, 0, global_screen.width, global_screen.height);

    /* 更新光标 */
    cursor_paint();
	return 0;
}

/**
 * @brief 
 * 对初始化图形设备的处理函数
 * 将图形设备的屏幕设置同应用程序的屏幕
 * 添加这个图形设备句柄到应用程序的图形设备数组中
 * 调用这个处理函数之前已经使用engine_graphics_device_init()函数初始化图形设备了
 *
 * @param app_ptr 应用程序指针
 * @param gd_handler 图形设备描述符
 *
 * @return 0
 **/
static si_t graphics_device_init_handler(addr_t app_ptr, si_t gd_handler)
{
    struct application_info * app_info_ptr = (struct application_info*)app_ptr;
	struct graphics_device* gd = (struct graphics_device*)gd_handler;

    if(gd != NULL)
    {
		gd->screen = app_info_ptr->screen;
        vector_push_back(&app_info_ptr->gd_ptr_vector, &gd_handler, sizeof(si_t));
    }
	return 0;
}

/**
 * @brief 
 * 对清理图形设备的处理函数 从应用程序的图形设备数组中清除这个图形设备句柄
 * 调用这个处理函数之前已经使用engine_graphics_device_exit()清理图形设备了
 *
 * @param app_ptr 应用程序指针
 * @param gd_handler 图形设备描述符
 *
 * @return 成功返回0 否则返回-1
 **/
static si_t graphics_device_exit_handler(addr_t app_ptr, si_t gd_handler)
{ 
    struct application_info * app_info_ptr = (struct application_info*)app_ptr;
	int i = 0, n = 0;

    n = vector_size(&(app_info_ptr->gd_ptr_vector));
	for(i = 0; i < n; ++ i)
    {
        si_t* gd_2_ptr = vector_at(&(app_info_ptr->gd_ptr_vector), i);
        if(*gd_2_ptr == gd_handler)
        {
			/**
			 * 找到图形设备指针并清理
			 **/
			vector_erase(&(app_info_ptr->gd_ptr_vector), i);
			return 0;
        }
    }
	return -1;
}

/**
 * @brief 对退出窗口管理器命令的处理函数(需要关闭窗口管理器的权限)
 *
 * @param app_ptr 应用程序指针
 * 
 * @return 成功返回0，否则返回-1
 **/
static si_t window_manager_exit_handler(addr_t app_ptr)
{
    struct application_info * app_info_ptr = (struct application_info*)app_ptr;
	/**
	 * 是否为关机程序
	 **/
	if(APPLICATION_TYPE_WINDOW_MANAGER_EXIT == app_info_ptr->application_type)
	{
		int n = vector_size(&global_wm.application_info_vector);
		int i = 0;

		/**
		 * 通知所有程序推出
		 **/
		for(i = 0; i != n; ++ i)
		{
			struct application_info * app_info_ptr = vector_at(&(global_wm.application_info_vector), i);
			if(0 != send_application_quit_message(&app_info_ptr->uds))
			{
				EGUI_PRINT_ERROR("failed to send application quit msg to %s", app_info_ptr->name);
			}
		}
		return 0;
	}
	return -1;
}

/**
 * @brief 对重写桌面的处理函数(需要桌面权限)
 *
 * @param app_ptr 应用程序指针
 * @return 成功返回0，否则返回-1
 **/
static si_t desktop_dirty_handler(addr_t app_ptr)
{
    struct application_info * app_info_ptr = (struct application_info*)app_ptr;
	/**
	 * 是否为桌面程序
	 **/
	if(APPLICATION_TYPE_DESKTOP == app_info_ptr->application_type)
	{
		return flush_in_z_order(0, 0, global_screen.width, global_screen.height);
	}
	return -1;
}

/**
 * @brief 刷新图形设备的处理函数
 *
 * @param app_ptr 应用程序指针
 * @param gd_handler 图形设备句柄
 *
 * @return 0
 **/
static si_t update_handler(addr_t app_ptr, si_t gd_handler)
{
    struct application_info * app_info_ptr = (struct application_info*)app_ptr;
	struct graphics_device* gd_ptr = (struct graphics_device*)gd_handler;
    /* 这个用户应用程序有多少窗口 */
    si_t n = vector_size(&(app_info_ptr->window_info_vector));
    if(n > 0)
    {
        struct object* tree = vector_at(&(app_info_ptr->window_info_vector), 0);
        struct window_info* win_info_ptr = (struct window_info *)(tree->lchild);

        memset(global_screen.buffer_addr, 0, global_screen.size);

        /* 更新光标 */
        flush_above_in_z_order_including(app_info_ptr, win_info_ptr, &gd_ptr->rectangle);
        screen_flush(gd_ptr->rectangle.x, gd_ptr->rectangle.y, gd_ptr->rectangle.width, gd_ptr->rectangle.height);

        /* 更新光标 */
        cursor_paint();
    }
	return 0;
}

/**
 * @brief 被all_app_traversal_top_window_decrement()配套使用
 * 寻找特定的顶层窗口
 **/
static si_t _do_find_top_window(struct window_info_iterator* iter, addr_t arg)
{
	return ((si_t)iter->top_win_info_ptr == (si_t)arg);
}
/**
 * @brief 激活窗口的处理函数 需要桌面权限
 *
 * @param app_ptr 应用程序指针
 * @param window_descripter 窗口描述符
 *
 * @return 
 **/
static si_t activate_window_handler(addr_t app_ptr, si_t window_descripter)
{
	struct application_info* app_info_ptr = (struct application_info*)app_ptr;
	struct window_info_iterator iter;

	if(0 == (app_info_ptr->application_type & APPLICATION_TYPE_DESKTOP))
	{
		EGUI_PRINT_ERROR("none desktop application send activate window request.");
		return -1;
	}

	window_info_iterator_clear(&iter);
	if(0 == all_app_traversal_top_window_decrement(&iter, _do_find_top_window, (addr_t)window_descripter))
    {
        /* 原来有活动窗口 */
        if(global_wm.active_win_info_ptr != NULL)
        {
            /* 发送激死消息给原来的活动窗口 */
			send_window_deactivate_message(&global_wm.active_app_info_ptr->uds, NULL, (si_t)global_wm.active_win_info_ptr);
			/** 通知桌面 **/
			if(NULL != global_wm.desktop_app_ptr)
			{
				send_window_deactivate_message(&global_wm.desktop_app_ptr->uds, NULL,
					(si_t)object_get_root(OBJECT_POINTER(global_wm.active_win_info_ptr)));
			}

            /* 改变活动窗口 */
            global_wm.active_win_info_ptr = NULL;
            global_wm.active_app_info_ptr = NULL;
        }
    }
    else if(global_wm.active_win_info_ptr != iter.top_win_info_ptr)
    {
        /* 原来有活动窗口 */
        if(global_wm.active_win_info_ptr != NULL)
        {
            /* 发送激死消息给原来的活动窗口 */
			send_window_deactivate_message(&global_wm.active_app_info_ptr->uds, NULL, (si_t)global_wm.active_win_info_ptr);
			/** 通知桌面 **/
			if(NULL != global_wm.desktop_app_ptr)
			{
				send_window_deactivate_message(&global_wm.desktop_app_ptr->uds, NULL,
					(si_t)object_get_root(OBJECT_POINTER(global_wm.active_win_info_ptr)));
			}
        }

        /* 将活动窗口移动到该用户应用程序的窗口向量的尾部 */
        vector_move_back(&(app_info_ptr->window_info_vector), iter.win_index);

        /* 将活动程序移动到用户应用程序向量的尾部 */
        vector_move_back(&(global_wm.application_info_vector), iter.app_index);

		object_move_first(OBJECT_POINTER(iter.top_win_info_ptr));

        /* 改变活动窗口 */
        global_wm.active_win_info_ptr = iter.top_win_info_ptr;
        global_wm.active_app_info_ptr = iter.app_info_ptr;

		send_window_activate_message(&global_wm.active_app_info_ptr->uds, NULL, (si_t)iter.top_win_info_ptr);
		if(NULL != global_wm.desktop_app_ptr)
		{
			send_window_activate_message(&global_wm.desktop_app_ptr->uds, NULL,
				(si_t)object_get_root(OBJECT_POINTER(iter.top_win_info_ptr)));
		}
    }

	return 0;
}

/**
 * @brief 注册窗口的处理函数
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针, 父窗口描述符，窗口标题，初始化窗口的左上角横坐标和纵坐标，初始化窗口的长和宽，是否可以最大化，是否可以最小化，模泰
 * @param app_ptr 应用程序指针
 * @param parent_descripter 父窗口描述符
 * @param title 窗口标题
 * @param x 窗口左上方横坐标
 * @param y 窗口左上方纵坐标
 * @param w 窗口的宽
 * @param h 窗口的高
 * @param maxable  是否可以最大化
 * @param minable 是否可以最小化
 * @param model 摸态
 *
 * @return 成功返回窗口句柄 否则返回0
 **/
static si_t register_window_handler(addr_t app_ptr, si_t parent_descripter, char* title, si_t x, si_t y, si_t w, si_t h, si_t maxable, si_t minable, si_t model)
{
	struct application_info * app_info_ptr = (struct application_info*)app_ptr;
	struct window_info * win_info_ptr = NULL;

	win_info_ptr = window_info_init(title, maxable, minable, model);
	if(NULL == win_info_ptr)
	{
		EGUI_PRINT_ERROR("failed to init window info");
		return -1;
	}

	window_info_resize(win_info_ptr, x, y, w, h);

	/**
	 * 顶层窗口:添加到顶层窗口队列中
	 **/
	if(parent_descripter == 0)
	{
		struct object root = {0};
		root.parent = OBJECT_POINTER(win_info_ptr);
		root.lchild = OBJECT_POINTER(win_info_ptr);
		root.rchild = OBJECT_POINTER(win_info_ptr);

		/* 将窗口的信息添加到向量 */
		vector_push_back(&(app_info_ptr->window_info_vector), &root, sizeof(struct object));
		if(NULL != global_wm.desktop_app_ptr)
		{
			send_window_register_message(&global_wm.desktop_app_ptr->uds, NULL, (si_t)win_info_ptr);
		}

		win_info_ptr->parent = vector_back(&(app_info_ptr->window_info_vector));
	}
	/**
	 * 否则：直接添加子窗口
	 **/
	else
	{
		object_insert_child(OBJECT_POINTER(parent_descripter), OBJECT_POINTER(win_info_ptr));
	}

	if(global_wm.active_win_info_ptr != NULL && global_wm.active_app_info_ptr != NULL)
	{
		send_window_deactivate_message(&global_wm.active_app_info_ptr->uds, NULL, (si_t)global_wm.active_win_info_ptr);
	}

	/* 最近创建的窗口是活动窗口 */
	global_wm.active_win_info_ptr = win_info_ptr;
	/* 最近创建活动窗口的用户应用程序是活动程序 */
	global_wm.active_app_info_ptr = app_info_ptr;

	send_window_activate_message(&global_wm.active_app_info_ptr->uds, NULL, (si_t)global_wm.active_win_info_ptr);

	/* 刷新包括新窗口在内的所有窗口 */
	flush_below_in_z_order(app_info_ptr, win_info_ptr, &win_info_ptr->area);

	/* 刷新包括新窗口在内的所有窗口 */
	flush_above_in_z_order_including(app_info_ptr, win_info_ptr, &win_info_ptr->area);

	/* 将缓冲区的内容复制到存储区 */
	screen_flush(win_info_ptr->area.x, win_info_ptr->area.y, win_info_ptr->area.width, win_info_ptr->area.height);
	/* 更新光标 */
	cursor_paint();

	/* 返回窗口的句柄 */
	return (si_t)win_info_ptr;
}

static void __window_info_destructor(void* arg)
{
	struct window_info* w = (struct window_info*)arg;
	window_info_exit(w);
}

/**
 * @brief 与all_app_traversal_decrement()配套使用
 * 删除窗口 如果是顶层窗口还需要清理其在顶层窗口数组的内容
 **/
static si_t _do_delete_window(struct window_info_iterator* iter, addr_t arg)
{
	if((si_t)iter->win_info_ptr == (si_t)arg)
	{
		object_delete(OBJECT_POINTER(iter->win_info_ptr), __window_info_destructor);
		/* 删除窗口信息结构 */
		if(iter->win_info_ptr == iter->top_win_info_ptr)
		{
			vector_erase(&(iter->app_info_ptr->window_info_vector), iter->win_index);
		}
		return 1;
	}
	return 0;
}

/**
 * @brief 对取消窗口请求的处理函数
 *
 * @param app_ptr 应用程序指针
 * @param window_descripter 窗口描述符
 *
 * @return 0
 **/
static si_t cancel_window_handler(addr_t app_ptr, si_t window_descripter)
{
	struct application_info * app_info_ptr = (struct application_info*)app_ptr;
	struct window_info_iterator iter;
	window_info_iterator_clear(&iter);

	/* 清理窗口 */
	if(0 == all_app_traversal_decrement(&iter, _do_delete_window, (addr_t)window_descripter))
		return 0;

	/* 如果是顶层窗口则发送消息给桌面 */
	if(NULL != global_wm.desktop_app_ptr && (si_t)window_descripter == (si_t)iter.top_win_info_ptr)
	{
		send_window_cancel_message(&global_wm.desktop_app_ptr->uds, NULL, window_descripter);
	}

	/* 更新活动用户应用程序和活动窗口 */
	if((si_t)global_wm.active_win_info_ptr == window_descripter || global_wm.active_app_info_ptr == app_info_ptr)
	{
		global_wm.active_win_info_ptr = NULL;
		global_wm.active_app_info_ptr = NULL;

		window_info_iterator_clear(&iter);
		if(all_app_traversal_decrement(&iter, _do_update_last_window, NULL))
		{
			/**
			 * send activate msg
			 **/
			send_window_activate_message(&(global_wm.active_app_info_ptr->uds), NULL, (si_t)global_wm.active_win_info_ptr);
			if(NULL != global_wm.desktop_app_ptr)
			{
				send_window_activate_message(&global_wm.desktop_app_ptr->uds, NULL, (si_t)iter.top_win_info_ptr);
			}
		}
	}

	/* 将缓冲区的内容复制到存储区 */
	flush_in_z_order(0, 0, global_screen.width, global_screen.height);

	screen_flush(0, 0, global_screen.width, global_screen.height);

	/* 更新光标 */
	cursor_paint();

	return 0;
}

/**
 * @brief 对移动窗口请求的处理函数
 *
 * @param app_ptr 应用程序指针
 * @param window_descripter 窗口描述符
 * @param x 新的横坐标
 * @param y 新的纵坐标
 *
 * @return 0
 **/
static si_t move_window_handler(addr_t app_ptr, si_t window_descripter, si_t x, si_t y)
{
	struct application_info * app_info_ptr = (struct application_info*)app_ptr;
	struct window_info* win_info_ptr = (struct window_info*)window_descripter;
	struct rectangle old_area = {0}, new_area = {0}, union_area = {0};
	si_t x_offset = 0, y_offset = 0;

	/* 更新窗口原来区域后面的内容 */
	flush_below_in_z_order(app_info_ptr, win_info_ptr, &win_info_ptr->area);

	/** 更新窗口区域 **/
	old_area = win_info_ptr->area;
	x_offset = x - win_info_ptr->work_area.x;
	y_offset = y - win_info_ptr->work_area.y;
	window_info_move(win_info_ptr, x_offset, y_offset);
	new_area = win_info_ptr->area;

	/** 刷新 **/
	area_union(&old_area, &new_area, &union_area);
	screen_flush(union_area.x, union_area.y, union_area.width, union_area.height);

	/* 更新光标 */
	cursor_paint();
	return 0;
}

/**
 * @brief 对调整窗口大小的请求处理函数
 *
 * @param app_ptr 应用程序指针
 * @param window_descripter 窗口描述符
 * @param x 窗口的横坐标
 * @param y 窗口的纵坐标
 * @param width 窗口的宽
 * @param height 窗口的高
 *
 * @return 
 **/
static si_t resize_window_handler(addr_t app_ptr, si_t window_descripter, si_t x, si_t y, si_t width, si_t height)
{
	struct application_info * app_info_ptr = (struct application_info*)app_ptr;
	struct window_info* win_info_ptr = (struct window_info*)window_descripter;
	struct rectangle old_area = {0}, new_area = {0}, union_area = {0};

	/* 更新窗口原来区域后面的内容 */
	flush_below_in_z_order(app_info_ptr, win_info_ptr, &(win_info_ptr->area));

	/** 更新窗口区域 **/
	old_area = win_info_ptr->area;
	window_info_resize(win_info_ptr, x, y, width, height);
	new_area = win_info_ptr->area;
	area_union(&old_area, &new_area, &union_area);

	/** 刷新 **/
	screen_flush(union_area.x, union_area.y, union_area.width, union_area.height);

	/* 更新光标 */
	cursor_paint();

	return 0;
}

/**
 * @brief 窗口最小化请求的处理函数
 *
 * @param app_ptr 应用程序指针
 * @param window_descripter 窗口描述符
 *
 * @return 0
 **/
static si_t minimize_window_handler(addr_t app_ptr, si_t window_descripter)
{
	struct window_info* win_info_ptr = (struct window_info*)window_descripter;
	struct rectangle old_area = win_info_ptr->area;

	/** 设置window_info的区域为一个不可能的区域 **/
	window_info_resize(win_info_ptr, -1, -1, 0, 0);

	/** 刷新 **/
	flush_in_z_order(old_area.x, old_area.y, old_area.width, old_area.height);

	/* 更新光标 */
	cursor_paint();

	return 0;
}

/**
 * @brief 窗口最大化请求的处理函数
 *
 * @param app_ptr 应用程序指针
 * @param window_descripter 窗口描述符
 * @param area 最大化之后的窗口区域
 *
 * @return 
 **/
static si_t maximize_window_handler(addr_t app_ptr, si_t window_descripter, struct rectangle* area)
{
	struct application_info * app_info_ptr = (struct application_info*)app_ptr;
	struct window_info* win_info_ptr = (struct window_info*)window_descripter;

	/**
	 * 新的区域为窗口最大工作区
	 **/
	window_info_maximum(win_info_ptr, &global_wm.work_area);

	/* 更新窗口原来区域后面的内容 */
	flush_below_in_z_order(app_info_ptr, win_info_ptr, &win_info_ptr->area);

	screen_flush(win_info_ptr->area.x, win_info_ptr->area.y, win_info_ptr->area.width, win_info_ptr->area.height);

	/* 更新光标 */
	cursor_paint();

	*area = win_info_ptr->work_area;

	return 0;
}

/**
 * @brief 窗口还原请求的处理函数
 *
 * @param app_ptr 应用程序指针
 * @param window_descripter 窗口描述符
 * @param area 还原之后的窗口区域
 *
 * @return 0
 **/
static si_t restore_window_handler(addr_t app_ptr, si_t window_descripter, struct rectangle* area)
{
	struct application_info * app_info_ptr = (struct application_info*)app_ptr;
	struct window_info* win_info_ptr = (struct window_info*)window_descripter;
	struct rectangle old_area = {0}, new_area = {0}, union_area = {0};

	/* 更新窗口原来区域后面的内容 */
	flush_below_in_z_order(app_info_ptr, win_info_ptr, &win_info_ptr->area);

	/** 更新窗口大小 **/
	old_area = win_info_ptr->area;
	window_info_resize(win_info_ptr, area->x, area->y, area->width, area->height);
	new_area = win_info_ptr->area;
	area_union(&old_area, &new_area, &union_area);

	/* 更新窗口原来区域后面的内容 */
	flush_below_in_z_order(app_info_ptr, win_info_ptr, &union_area);

	/** 刷新 **/
	screen_flush(union_area.x, union_area.y, union_area.width, union_area.height);

	/* 更新光标 */
	cursor_paint();

	return 0;
}

/**
 * @brief 获取窗口标题请求的处理函数 只有桌面程序可以获取窗口标题
 *
 * @param app_ptr 应用程序指针
 * @param window_descripter 窗口描述符
 * @param title_ptr 函数输出 窗口标题字符串的指针
 *
 * @return 0
 **/

static si_t get_window_title_handler(addr_t app_ptr, si_t window_descripter, char** title_ptr)
{
	struct application_info * app_info_ptr = (struct application_info*)app_ptr;
	struct window_info* win_info_ptr = (struct window_info*)window_descripter;

	if(app_info_ptr->application_type & APPLICATION_TYPE_DESKTOP)
	{
		*title_ptr = win_info_ptr->title;
		return 0;
	}
	*title_ptr = NULL;
	return -1;
}

static void window_manager_request_destructor(addr_t arg)
{
	/**
	 * 若由于应用程序出错导致应用程序退出，event_listener将检测到套结字关闭。
	 * 此时应当调用cancel_application_handler来清理服务器端有关应用程序的信息
	 * 因此需要判断这个应用程序是否还存在于窗口管理器端 
	 * 如果已不存在 则说明之前已经发出cancel_application的请求了
	 * 那么就可以啥也不做了
	 **/
	cancel_application_handler(arg);
}

extern int window_manager_set_request_handler()
{
	/**
	 * 服务器端库初始化
	 **/
	server_lib_init(&global_wm.server_listener, window_manager_request_destructor);

	/**
	 * 添加各个请求的处理函数
	 * 在一个新的应用程序与窗口管理器建立连接之后
	 * 应用程序发出对应请求，窗口管理器即回调用对应的处理函数处理
	 **/
	add_register_application_handler(register_application_handler);
	add_cancel_application_handler(cancel_application_handler);
	add_graphics_device_init_handler(graphics_device_init_handler);
	add_graphics_device_exit_handler(graphics_device_exit_handler);
	add_window_manager_exit_handler(window_manager_exit_handler);
	add_desktop_dirty_handler(desktop_dirty_handler);
	add_update_handler(update_handler);
	add_activate_window_handler(activate_window_handler);
	add_register_window_handler(register_window_handler);
	add_cancel_window_handler(cancel_window_handler);
	add_move_window_handler(move_window_handler);
	add_resize_window_handler(resize_window_handler);
	add_minimize_window_handler(minimize_window_handler);
	add_maximize_window_handler(maximize_window_handler);
	add_restore_window_handler(restore_window_handler);
	add_get_window_title_handler(get_window_title_handler);

	return 0;
}

