/**
 * @file egui_uds.h
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

# ifndef _EGUI_UDS_H_
# define _EGUI_UDS_H_ 1

# include "base_type.h"
# include <sys/socket.h>

/**
 * 获得结构中成员的偏移量
 *
 * @param struct_name 结构的名字
 * @param member_name 成员的名字
 *
 * @return 成员的偏移量
**/
# define OFFSET_OF(struct_name, member_name) ((ui_t)(&(((struct_name *)0)->member_name)))

/**
 * 系统的最多监听连接数
 **/
# define MAX_CONNECTION_QUANTITY	1024

enum __ipc_uds_role__
{
    /** play as server **/
    PEER_ROLE_SERVER = 0,
    /** play as client **/
    PEER_ROLE_CLIENT,
	/** play as lister **/
	PEER_ROLE_LISTENER
};

/**
    unix domain socket
**/
struct egui_uds
{
	/** 文件名 */
	char * file_path;
	/** 文件描述符 */
	si_t sock_fd;
	/** 端的角色：0 服务器，1 客户机 **/
	si_t peer_role;
};

/**
 * 申请 UNIX 域套接字资源
 *
 * @param uds  UNIX 域套接字结构体的指针
 * @param sock_type 套接字种类：SOCK_STREAM，SOCK_DGRAM，SOCK_SEQPACKET
 * @param file_path socket对应文件名
 * @param peer_role 端的角色
 *
 * @return 成功返回0，失败返回-1。
**/
extern si_t uds_init(struct egui_uds* uds, si_t sock_type, char* file_path, si_t peer_role);

/**
 * 释放 UNIX 域套接字资源
 *
 * @param uds  UNIX 域套接字结构体的指针
 *
 * @return 成功返回0，失败返回-1。
**/
extern si_t uds_exit(struct egui_uds* uds);

/**
 * 获得试图建立连接的客户端的句柄
 *
 * @param server_uds 服务器的句柄
 * @param new_uds 新建立连接的客户端的句柄
 *
 * @return 成功返回0，失败返回-1。
 **/
extern si_t accept_as_uds(struct egui_uds* server_uds, struct egui_uds* new_uds);

/**
 * 将一个裸文件描述符转换成句柄；
 * 此操作用于输入模块，由于输入模块多路复用也需要使用event_listener，因此需要将输入设备的裸文件描述符包装成egui_uds
 *
 * @param uds 包装后的句柄
 * @param fd 裸文件描述符
 **/
extern void uds_init_from_naked_socket(struct egui_uds* uds, si_t sockfd);

/**
 * 对系统函数read()的封装
 *
 * @param uds 待读取的句柄
 * @param content 读取内容buffer
 * @param bytes 最多读取字节
 *
 * @return 成功返回读取字数，失败返回-1。
 **/
extern ssize_t uds_read(const struct egui_uds* uds, addr_t content, size_t bytes);

/** 
 * 对系统函数write()的封装 
 *
 * @param uds 待读取的句柄
 * @param content 写入内容的buffer
 * @param bytes 最多写入字节	
 *
 * @return 成功返回写入字数，失败返回-1。
 **/
extern ssize_t uds_write(const struct egui_uds* uds, const_addr_t content, size_t bytes);

/**
 * 判断对方socket是否已关闭
 *
 * @param uds 待判断的句柄
 *
 * @return 已关闭返回1，未关闭返回0
 **/
extern si_t is_socket_closed(struct egui_uds* uds_ptr);

# endif
