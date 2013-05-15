/**
 * @file event_listener.h
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

#ifndef _SELECTER_H_
#define _SELECTER_H_

#include <sys/select.h>

#include "egui_uds.h"
#include "data_structures.h"

/**
 * 监听事件类型
 **/
enum SELECTER_EVENT_TYPE
{
	SELECTER_EVENT_TYPE_READ = 0,
	SELECTER_EVENT_TYPE_WRITE
};

/**
 * 事件的回调函数的返回值
 **/
enum SELECTER_RETURN_TYPE
{
	/** 正常返回，继续下一轮监听 **/
	SELECTER_RETURN_TYPE_CONTINUE = 0,
	/** 删除对应的监听事件 **/
	SELECTER_RETURN_TYPE_REMOVE_HANDLER,
	/** 结束监听 **/
	SELECTER_RETURN_TYPE_END
};

struct events_handler
{
	/** 待监听事件的sockfd **/
	struct egui_uds uds;
	/** 订阅事件的类型：读或写或exception **/
	si_t event_type;
	/** 订阅事件的回调函数， 第二个参数是可选的，返回值若不为0则结束监听 **/
	si_t (*handler)(struct egui_uds*, addr_t arg);
	/** 销毁事件的析构函数， 第二个参数是可选的 **/
	void (*destructor)(struct egui_uds*, addr_t arg);
	/** 回调函数的第二个参数 **/
	addr_t arg;
};

struct event_listener
{
	/** 所有event_handler的集合 **/
	struct list event_handler_set;
	/** 实际select的所有等待读的fd的集合 **/
	fd_set read_set;
	/** 实际select的所有等待写的fd的集合 **/
	fd_set write_set;
};

/**
 * 初始化event_listener
 *
 * @param l 待初始化的event_listener
 **/
extern void event_listener_init(struct event_listener* l);

/**
 * 清理event_listener
 *
 * @param l 待清理的event_listener
 **/
extern void event_listener_exit(struct event_listener* l);

/**
 * 订阅一个句柄的读事件
 *
 * @param l 待操作的event_listener
 * @param uds 待监听的句柄
 * @param arg 回调函数的额外参数
 * @param handler 回调函数 若回调函数返回值非0则退出监听事件循环
 * @param destructor 析构函数 当event_listener退出或对方关闭连接时将被调用
 **/
extern void event_listener_add_read_event(struct event_listener* l, struct egui_uds* uds, addr_t arg,
	si_t (*handler)(struct egui_uds*, addr_t), void (*destructor)(struct egui_uds*, addr_t arg));

/**
 * 订阅一个句柄的写事件
 *
 * @param l 待操作的event_listener
 * @param uds 待监听的句柄
 * @param arg 回调函数的额外参数
 * @param handler 回调函数 返回值为enum SELECTER_RETURN_TYPE 决定event_listener的行为
 * @param destructor 析构函数 当event_listener退出或对方关闭连接时将被调用
 **/
extern void event_listener_add_write_event(struct event_listener* l, struct egui_uds* uds, addr_t arg,
	si_t (*handler)(struct egui_uds*, addr_t), void (*destructor)(struct egui_uds*, addr_t arg));

/**
 * 取消一个读事件的订阅
 *
 * @param s 待操作的event_listener
 * @param uds 待监听的句柄
 **/
extern void event_listener_del_read_handler(struct event_listener* l, struct egui_uds* uds);

/**
 * 取消一个写事件的订阅
 *
 * @param l 待操作的event_listener
 * @param uds 待监听的句柄
 **/
extern void event_listener_del_write_handler(struct event_listener* l, struct egui_uds* uds);

/**
 * 运行event_listener，监听事件并处理
 * event_listener会自动清理对方关闭连接的通信句柄，也会在所有通信句柄都关闭后推出循环
 *
 * @param l 待操作的event_listener
 *
 * @return 成功返回0 否则返回-1
 **/
extern si_t event_listener_exec(struct event_listener* l);

#endif
