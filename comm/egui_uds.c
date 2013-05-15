/**
 * @file egui_uds.c
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

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/un.h>
# include <sys/socket.h>
#include <signal.h>

# include "egui_uds.h"
# include "log.h"

si_t uds_init(struct egui_uds* uds, si_t sock_type, char* file_path, si_t peer_role)
{
    si32_t temp = 1;
    struct sockaddr_un sun;

	uds->file_path = malloc(strlen(file_path) + 1);
	if(NULL == uds->file_path)
	{
		EGUI_PRINT_SYS_ERROR("Failed to malloc for file path: malloc()");
		uds->sock_fd = -1;
		return -1;
	}
	strncpy(uds->file_path, file_path, strlen(file_path) + 1);
	uds->peer_role = peer_role;

    /* 创建套接字 */
    uds->sock_fd = socket(PF_UNIX, sock_type, 0);

    if(uds->sock_fd < 0)
    {
		EGUI_PRINT_SYS_ERROR("Failed to create socket: socket()");
        return -1;
    }

    /* 套接字地址可重用 */
    if(setsockopt(uds->sock_fd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(si32_t)) == -1)
    {
		EGUI_PRINT_SYS_ERROR("Failed to set socket address reusable: setsockopt()");
		close(uds->sock_fd);
		uds->sock_fd = -1;
        return -1;
    }

    if(uds->peer_role == PEER_ROLE_SERVER)
    {
        /* 防止已经用过 */
        unlink(uds->file_path);
        /* 清空 */
        memset(&sun, 0, sizeof(struct sockaddr_un));
        /* 协议族 */
        sun.sun_family = AF_UNIX;
        /* 路径 */
        strncpy(sun.sun_path, uds->file_path, strlen(uds->file_path));

        /* 将地址绑定到套接字 */
        if(bind(uds->sock_fd, (struct sockaddr *)(&sun), strlen(sun.sun_path) + OFFSET_OF(struct sockaddr_un, sun_path)) == -1)
        {
			EGUI_PRINT_SYS_ERROR("Failed to bind address to socket: bind()");
            close(uds->sock_fd);
			uds->sock_fd = -1;
            unlink(sun.sun_path);

            return -1;
        }

        /* 让所有用户都能使用这个文件 */
        if(chmod(uds->file_path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH) < 0)
        {
			EGUI_PRINT_SYS_ERROR("Failed to change mode of socket file: chmod()");
            close(uds->sock_fd);
			uds->sock_fd = -1;
            unlink(sun.sun_path);

            return -1;
        }

        /* 告诉内核可以进行监听 */
        if(listen(uds->sock_fd, MAX_CONNECTION_QUANTITY) == -1)
        {
			EGUI_PRINT_SYS_ERROR("Failed to listen socket: listen()");
            close(uds->sock_fd);
			uds->sock_fd = -1;
            unlink(sun.sun_path);

            return -1;
        }
    }
    else if(uds->peer_role == PEER_ROLE_CLIENT)
    {
        memset(&sun, 0, sizeof(struct sockaddr_un));
        /* 协议族 */
        sun.sun_family = AF_UNIX;

        /* 客户机绑定的地址 */
        sprintf(sun.sun_path, "/tmp/socket_client_%d", getpid());
        /* 防止已经用过 */
        unlink(sun.sun_path);

        /* 将地址绑定到套接字 */
        if(bind(uds->sock_fd, (struct sockaddr *)(&sun), strlen(sun.sun_path) + OFFSET_OF(struct sockaddr_un, sun_path)) == -1)
        {
			EGUI_PRINT_SYS_ERROR("Failed to bind address to socket: bind()");
            close(uds->sock_fd);
			uds->sock_fd = -1;
            unlink(sun.sun_path);

            return -1;
        }

        memset(&sun, 0, sizeof(struct sockaddr_un));
        /* 协议族 */
        sun.sun_family = AF_UNIX;
        /* 路径 */
        strncpy(sun.sun_path, uds->file_path, strlen(uds->file_path));

        /* 连接 */
        if(connect(uds->sock_fd, (struct sockaddr *)(&sun), strlen(sun.sun_path) + OFFSET_OF(struct sockaddr_un, sun_path)) == -1)
        {
			EGUI_PRINT_SYS_ERROR("Failed to connect to server: connect()");
            close(uds->sock_fd);
			uds->sock_fd = -1;
            unlink(sun.sun_path);

            return -1;
        }
    }

    return 0;
}

si_t uds_exit(struct egui_uds* uds)
{
    close(uds->sock_fd);

    if(uds->peer_role == PEER_ROLE_SERVER)
    {
        /* 删除文件 */

        /*
            WARNING 不能删除
            因为窗口管理程序会创建新的进程并且执行新的程序。
            在调用 fork 之后，子进程会释放从父进程（窗口管理程序）继承的资源。
            也就是说，它会调用这个函数（ ipc_uds_exit ），这样就将文件删除了。
            没有了这个文件，子进程就不能和父进程（窗口管理程序）通信。
        */
        /*
            unlink(uds->file_path);
        */
    }
    else if(uds->peer_role == PEER_ROLE_CLIENT)
    {
		/** 
		 * 只用其中sun_path位，但是由于不能得知sun_path数组大小，只好浪费一下栈空间了
		 * 绝非最好的方法，因此如果有机会还是要改进一下
		 **/
		struct sockaddr_un sun;
        memset(sun.sun_path, 0, sizeof(sun.sun_path));

        /* 获得文件名 */
        sprintf(sun.sun_path, "/tmp/socket_client_%d", getpid());

        /* 删除文件 */
        unlink(sun.sun_path);
    }

	if(uds->file_path != NULL)
	{
		free(uds->file_path);
	}

    memset(uds, 0, sizeof(struct egui_uds));

    return 0;
}

si_t accept_as_uds(struct egui_uds* server_uds, struct egui_uds* new_uds)
{
	si_t new_fd = accept(server_uds->sock_fd, NULL, NULL);
	if(new_fd < 0)
	{
		EGUI_PRINT_SYS_ERROR("Failed to accept new socket: accept()");
		return -1;
	}

	uds_init_from_naked_socket(new_uds, new_fd);
	return 0;
}

void uds_init_from_naked_socket(struct egui_uds* uds, si_t sockfd)
{
	uds->file_path = NULL;
	uds->sock_fd = sockfd;
	uds->peer_role = PEER_ROLE_LISTENER;
}

si_t uds_read(struct egui_uds* uds, addr_t content, ui_t bytes)
{
	si_t nread = read(uds->sock_fd, content, bytes);
	if(nread < 0)
	{
		EGUI_PRINT_SYS_ERROR("Failed to read from egui unix domain socket: read()");
		return -1;
	}
	return nread;
}

si_t uds_write(struct egui_uds* uds, addr_t content, ui_t bytes)
{
	si_t nwrite = write(uds->sock_fd, content, bytes);
	if(nwrite < 0)
	{
		EGUI_PRINT_SYS_ERROR("Failed to write from egui unix domain socket: write()");
		return -1;
	}
	return nwrite;
}

static si_t signal_pipe_recv = 0;
static void sigpipe_handler(int signo)
{
	signal_pipe_recv = 1;
}

/**
 * 如何判断一个socket已关闭？对其进行写操作，如果引发sigpipe信号，则说明对方socket已关闭
 **/
si_t is_socket_closed(struct egui_uds* uds_ptr)
{
	/**
	 * 未建立连接
	 **/
	if(PEER_ROLE_SERVER == uds_ptr->peer_role)
		return 0;

	/** 设置sigpipe信号处理函数 **/
	signal(SIGPIPE, sigpipe_handler);

	if(-1 == write(uds_ptr->sock_fd, NULL, 0))
	{
		/** 收到sigpipe: 说明对方已关闭socket **/
		if(1 == signal_pipe_recv)
		{
			EGUI_PRINT_ERROR("detect client socket close");
			return 1;
		}
	}

	/** 还原sigpipe信号处理函数 **/
	signal(SIGPIPE, SIG_DFL);
	return 0;
}

