/**
 * @file egui_comm.h
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

# ifndef _EGUI_COMM_H_
# define _EGUI_COMM_H_ 1

# include "message.h"
# include "respond.h"
# include "egui_uds.h"
# include "data_structures.h"

/**
 * 发送一个请求
 *
 * @param uds_ptr 通信句柄
 * @param detail_type 请求类型
 * @param body 请求负载包
 * @param body_size 请求负载包+额外负载包的总大小
 *
 * @return 成功返回0，否则返回-1打印错误日志
 **/
extern si_t comm_send_request(const struct egui_uds* uds_ptr, si_t detail_type, const_addr_t body, ui_t body_size);

/**
 * 发送一个包含附加信息的请求
 *
 * @param uds_ptr 通信句柄
 * @param detail_type 请求类型
 * @param body 请求负载包
 * @param body_size 请求负载包+额外负载包的总大小
 * @param extra 附加信息包
 * @param extra_size 附加信息的大小
 *
 * @return 成功返回0，否则返回-1打印错误日志
 **/
extern si_t comm_send_request_with_extra(const struct egui_uds* uds_ptr, si_t detail_type, const_addr_t body, ui_t body_size, const_addr_t extra, ui_t extra_size);

/**
 * 发送一个回应
 *
 * @param uds_ptr 通信句柄
 * @param respond_ptr 回应包指针
 *
 * @return 成功返回0，否则返回-1打印错误日志
 **/
extern si_t comm_send_respond(const struct egui_uds* uds_ptr, const union respond* respond_ptr);

/**
 * 发送一个消息
 *
 * @param uds_ptr 通信句柄
 * @param message_ptr 消息内容
 *
 * @return 成功返回0，否则返回-1打印错误日志
 **/
extern si_t comm_send_message(const struct egui_uds* uds_ptr, const union message* message_ptr);

/**
 * 接受一个请求并发送回应
 *
 * @param uds_ptr 通信句柄
 * @param arg 额外参数
 * @param callback 对请求内容处理的回调函数
 *
 * @return 成功返回callback的返回值，否则返回-1打印错误日志
 **/
extern si_t comm_recv_request_and_send_respond(struct egui_uds* uds_ptr, addr_t arg,
	si_t(* callback)(addr_t, si_t, union respond*, addr_t));

/**
 * 接受一个回应
 *
 * @param uds_ptr 通信句柄
 * @param expected_detail_type 回应类型
 * @param result 回应返回值
 * @param message_queue 存放message的队列
 *
 * @return 成功返回0，否则返回-1打印错误日志
 **/
extern si_t comm_recv_respond(struct egui_uds* uds_ptr, si_t expected_detail_type, addr_t result, struct queue* message_queue);

/**
 * 新开辟一段空间用于接受一个消息 并将这个空间的地址填入消息指针中
 *
 * @param uds_ptr 通信句柄
 * @param expected_detail_type 回应类型
 * @param result 指向一个指针，该指针为返回的回应包的地址
 * @param message_queue 存放message的队列
 *
 * @return 成功返回0，否则返回-1打印错误日志
 **/
extern si_t comm_recv_respond_in_new_buffer(struct egui_uds* uds_ptr, si_t expected_detail_type, addr_t result, struct queue* message_queue);

/**
 * 接受一个消息
 *
 * @param uds_ptr 通信句柄
 * @param msg_ptr 读取的消息
 *
 * @return 成功返回0，否则返回-1打印错误日志
 **/
extern si_t comm_recv_msg(struct egui_uds* uds_ptr, union message* msg_ptr);


# endif
