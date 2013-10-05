/**
 * @file application.c
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

# include "application.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <termios.h>
# include <fcntl.h>
# include <sys/stat.h>

# include "comm.h"
# include "client_lib.h"
# include "config_parser.h"

# include "exec.h"
# include "log.h"

static struct termios term;

static si_t term_init(void)
{
	struct termios new_term;
	si_t fd = STDIN_FILENO;
	if(fd < 0) {
		EGUI_PRINT_SYS_ERROR("failed to open termios. open()");
		return -1;
	}

	if(tcgetattr(fd, &term) < 0) {
		EGUI_PRINT_SYS_ERROR("failed to get termios attribute. tcgetattr()");
		return -1;
	}

    new_term = term;
	new_term.c_lflag = new_term.c_lflag & ~(ECHO | ECHOE | ECHOK | ECHONL);
    new_term.c_cflag = new_term.c_cflag | (ICANON);

	if(tcsetattr(fd, TCSAFLUSH, &new_term) < 0) {
		EGUI_PRINT_SYS_ERROR("failed to set termios attribute. tcgetattr()");
		return -1;
	}

	return 0;
}

static void term_restore(void)
{
	si_t fd = STDIN_FILENO;
	if(fd < 0) {
		EGUI_PRINT_SYS_ERROR("failed to open termios. open()");
	}
	if(tcsetattr(fd, TCSAFLUSH, &term) < 0) {
		EGUI_PRINT_SYS_ERROR("failed to set termios attribute. tcgetattr()");
	}
}

struct application global_application;

static si_t application_load_config()
{
	/**
	 * TODO load config from file
	 **/
	global_application.icon_root_path = get_config_path("");
	global_application.server_path = "/tmp/server";
	return 0;
}

si_t application_init(si_t video_access_mode, si_t application_type, char* name)
{
	/**
	 * 加载配置项
	 **/
	if(0 != application_load_config())
	{
		EGUI_PRINT_ERROR("failed to load config.");
		return -1;
	}

	/**
	 * 初始化客户端通信句柄
	 **/
	if(0 != uds_init(&global_application.uds, SOCK_STREAM, global_application.server_path, PEER_ROLE_CLIENT))
	{
		EGUI_PRINT_ERROR("Failed to init client uds");
		return -1;
	}
	/**
	 * 初始化消息队列
	 **/
	queue_init(&global_application.message_queue);
	client_lib_init(&global_application.uds, &global_application.message_queue);

	/**
	 * 初始化event_listener
	 **/
	event_listener_init(&global_application.app_event_listener);
	global_application.exec_flag = 0;

    /**
     * init focus list
     **/
    list_init(&(global_application.focus_list));

    /* 初始化向量 */
    vector_init(&(global_application.window_vector));

	/**
	 * 应用程序名称
	 **/
	global_application.name = malloc(strlen(name) + 1);
	if(NULL == global_application.name)
	{
		EGUI_PRINT_SYS_ERROR("failed to malloc for application name %s. malloc()", name);
		return -1;
	}
	strncpy(global_application.name, name, strlen(name) + 1);
	global_application.application_type = application_type;

	if(0 != register_application(video_access_mode, application_type, name))
	{
		EGUI_PRINT_ERROR("failed to register application %s.", name);
		return -1;
	}

    global_application.focus = NULL;
    global_application.main_window = NULL;

	global_application.desktop_ptr = NULL;
	global_application.desktop_msg_handler = NULL;

    term_init();

    return 0;
}

si_t application_exit()
{
    struct object * addr;
    si_t i, n;

    /**
     * release focus list
     **/
    list_exit(&(global_application.focus_list));

    /* 有多少个窗口 */
    n = vector_size(&(global_application.window_vector));

    /* 依次删除窗口 */
    for(i = n - 1; i >= 0; -- i)
    {
        addr = vector_at(&(global_application.window_vector), i);

        /*
            这里没有逐个注销窗口类对象或者它的派生类对象。
            因为窗口管理程序收到用户应用程序的退出请求后，
            会释放关于这个用户应用程序的所有资源。
        */

        /* 释放这棵树 */
        object_tree_free(addr->parent);

        /* 从窗口向量中删除这个节点 */
        vector_erase(&(global_application.window_vector), i);
    }

    vector_exit(&(global_application.window_vector));

	if(0 != cancel_application())
	{
		EGUI_PRINT_ERROR("failed to cancle application");
	}

	event_listener_exit(&global_application.app_event_listener);
	queue_exit(&global_application.message_queue);
    uds_exit(&global_application.uds);

	free(global_application.name);
    free(global_application.icon_root_path);

    term_restore();

    return 0;
}

/* XXX Not using */
si_t application_quit()
{
    si_t i, n;
    struct object * addr;

    /* 有多少个窗口 */
    n = vector_size(&(global_application.window_vector));

    /* 依次删除窗口 */
    for(i = n - 1; i >= 0; -- i)
    {
        addr = vector_at(&global_application.window_vector, i);

        /*
           这里没有逐个注销窗口类对象或者它的派生类对象。
           因为窗口管理程序收到用户应用程序的退出请求后，
           会释放关于这个用户应用程序的所有资源。
           */

        /* 释放这棵树 */
        object_tree_free(addr->parent);

        /* 从窗口向量中删除这个节点 */
        vector_erase(&global_application.window_vector, i);
    }

    vector_exit(&global_application.window_vector);

	event_listener_exit(&global_application.app_event_listener);
    uds_exit(&global_application.uds);

	free(global_application.name);

    return 0;
}

void application_desktop_set(addr_t desktop_ptr, void (*desktop_handler)(addr_t, union message*))
{
	global_application.desktop_ptr = desktop_ptr;
	global_application.desktop_msg_handler = desktop_handler;
}
