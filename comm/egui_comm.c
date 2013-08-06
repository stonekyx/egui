/**
 * @file egui_comm.c
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
# include "egui_comm.h"

# include <unistd.h>
# include <stdlib.h>
# include "packet.h"
# include "request.h"
# include "respond.h"

# include "log.h"

/**
 * send packet head & packet body
 **/
static si_t comm_send_head_and_body(const struct egui_uds* uds_ptr, si_t packet_type, si_t detail_type,
        const_addr_t body,  ui_t body_size,
        const_addr_t extra, ui_t extra_size)
{
	struct packet_head   head;
	head.packet_type = packet_type;
	head.detail_type = detail_type;
	head.size = body_size + extra_size;

	/* 发送头部 */
	if(sizeof(struct packet_head) != uds_write(uds_ptr, &head, sizeof(struct packet_head)))
	{
		EGUI_PRINT_ERROR("failed to write packet head");
		return -1;
	}
	/* 发送负载 */
	if(body_size != uds_write(uds_ptr, body, body_size))
	{
		EGUI_PRINT_ERROR("failed to write packet body");
		return -1;
	}
	/** 发送附加信息 **/
	if(extra_size != 0)
	{
		if(extra_size != uds_write(uds_ptr, extra, extra_size))
		{
			EGUI_PRINT_ERROR("failed to write extra msg");
			return -1;
		}
	}

	return 0;
}

/**
 * send packet request
 **/
extern si_t comm_send_request(const struct egui_uds* uds_ptr, si_t detail_type,
        const_addr_t body, ui_t body_size)
{
	return comm_send_head_and_body(uds_ptr, PACKET_TYPE_REQUEST, detail_type,
            body, body_size,
            NULL, 0);
}

/**
 * send packet request with extra msg
 **/
extern si_t comm_send_request_with_extra(const struct egui_uds* uds_ptr, si_t detail_type,
        const_addr_t body, ui_t body_size,
        const_addr_t extra, ui_t extra_size)
{ 
	return comm_send_head_and_body(uds_ptr, PACKET_TYPE_REQUEST, detail_type,
            body , body_size,
            extra, extra_size);
}

/**
 * send packet respond
 **/
extern si_t comm_send_respond(const struct egui_uds* uds_ptr, const union respond* respond_ptr)
{ 
	return comm_send_head_and_body(uds_ptr, PACKET_TYPE_RESPOND, respond_get_type(respond_ptr),
            respond_get_value(respond_ptr), respond_get_len(respond_ptr),
            NULL                          , 0);
}

/**
 * send packet message
 **/
extern si_t comm_send_message(const struct egui_uds* uds_ptr, const union message* message_ptr)
{ 
	return comm_send_head_and_body(uds_ptr, PACKET_TYPE_MESSAGE, message_get_type(message_ptr),
		message_ptr, sizeof(union message),
        NULL       , 0);
}

/**
 * recv packet head, return packet_type, detail_type, size
 **/
static si_t comm_recv_head(struct egui_uds* uds_ptr, si_t* detail_type, si_t* size)
{
	struct packet_head head;

	/* 接受头部 */
	if(sizeof(struct packet_head) != uds_read(uds_ptr, &head, sizeof(struct packet_head)))
	{
		EGUI_PRINT_ERROR("failed to read packet head");
		return -1;
	}
	*detail_type = head.detail_type;
	*size = head.size;
	return head.packet_type;
}

/**
 * recv request only and handle it with callback
 **/
extern si_t comm_recv_request_and_send_respond(struct egui_uds* uds_ptr, addr_t arg,
	si_t(* callback)(addr_t, si_t, union respond*, addr_t))
{
	si_t detail_type = 0, body_size = 0;
	si_t packet_type = comm_recv_head(uds_ptr, &detail_type, &body_size);
	if(PACKET_TYPE_REQUEST == packet_type)
	{
		union respond respond_body;
		si_t ret = 0;
		byte_t* buf_ptr = NULL;
		byte_t* addr = (byte_t *)malloc(body_size);
		if(NULL == addr)
		{
			EGUI_PRINT_SYS_ERROR("failed to malloc for packet body");
			return -1;
		}
		/** 读取请求，注意如果包含附加请求可能需要读取多次 **/
		buf_ptr = addr;
		while(buf_ptr != addr + body_size)
		{
			si_t nleft = addr + body_size - buf_ptr;
			si_t nread = uds_read(uds_ptr, buf_ptr, nleft);
			if(nread <= 0)
			{
				EGUI_PRINT_ERROR("failed to read request body");
				return -1;
			}
			buf_ptr += nread;
		}
		/** 处理请求，获得回应包 **/
		ret = callback(addr, detail_type, &respond_body, arg);
		free(addr);
		if(respond_get_len(&respond_body) != 0)
		{
			/** 发送回应包 **/
			if(0 != comm_send_respond(uds_ptr, &respond_body))
			{
				EGUI_PRINT_ERROR("failed to send respond");
			}
		}
		return ret;
	}
	else if(-1 == packet_type)
	{
		EGUI_PRINT_ERROR("failed to recv head");
		return -1;
	}
	else
	{
		EGUI_PRINT_ERROR("expect request actaully recv %s[%d]", 
			packet_type == PACKET_TYPE_RESPOND ? "respond" : (packet_type == PACKET_TYPE_MESSAGE ? "message" : "unknown"), (int)packet_type);
		return -1;
	}
}

/**
 * recv specific type of respond
 **/
static si_t _recv_respond(struct egui_uds* uds_ptr, si_t expected_detail_type, addr_t result, si_t is_result_pointer, struct queue* message_queue)
{
	while(1)
	{
		si_t detail_type = 0, body_size = 0;
		si_t packet_type = comm_recv_head(uds_ptr, &detail_type, &body_size);
		if(PACKET_TYPE_RESPOND == packet_type)
		{
			if(expected_detail_type == detail_type)
			{
				addr_t buff = NULL;
				/**
				 * result的值是一个指针 即result是指针的指针
				 * 将回应包接受到一个新开辟的内存中 并将result的值指向这个地址
				 **/
				if(is_result_pointer)
				{
					addr_t* result_ptr = (addr_t*)result;
					*result_ptr = malloc(body_size);
					if(NULL == *result_ptr)
					{
						EGUI_PRINT_SYS_ERROR("failed to malloc for respond");
						return -1;
					}
					buff = *result_ptr;
				}
				else
				{
					buff = result;
				}
				/* 读取负载 */
				if(body_size != uds_read(uds_ptr, buff, body_size))
				{
					EGUI_PRINT_ERROR("failed to read respond body");
					return -1;
				}
				break;
			}
			else
			{
				EGUI_PRINT_ERROR("expect respond type[%s], actaully recv [%s]", respond_type_to_str(expected_detail_type), respond_type_to_str(detail_type));
			}
		}
		/**
		 * 若收到消息，则放入消息队列
		 **/
		else if(PACKET_TYPE_MESSAGE == packet_type)
		{
			union message msg;
			if(body_size != uds_read(uds_ptr, &msg, body_size))
			{
				EGUI_PRINT_ERROR("failed to read msg, type %s", message_type_to_str(detail_type));
				return -1;
			}
			queue_push(message_queue, &msg, sizeof(msg));
		}
		else if(-1 == packet_type)
		{
			EGUI_PRINT_ERROR("failed to recv head");
			return -1;
		}
		else
		{
			EGUI_PRINT_ERROR("expect respond, actaully recv %s[%d]", 
				packet_type == PACKET_TYPE_REQUEST ? "request" : "unknown", (int)packet_type);
		}
	}
	return 0;
}

extern si_t comm_recv_respond(struct egui_uds* uds_ptr, si_t expected_detail_type, addr_t result, struct queue* message_queue)
{
	return _recv_respond(uds_ptr, expected_detail_type, result, 0, message_queue);
}

extern si_t comm_recv_respond_in_new_buffer(struct egui_uds* uds_ptr, si_t expected_detail_type, addr_t result, struct queue* message_queue)
{
	return _recv_respond(uds_ptr, expected_detail_type, result, 1, message_queue);
}

/**
 * recv message
 **/
extern si_t comm_recv_msg(struct egui_uds* uds_ptr, union message* msg_ptr)
{
	si_t detail_type = 0, body_size = 0;
	si_t packet_type = comm_recv_head(uds_ptr, &detail_type, &body_size);
	if(PACKET_TYPE_MESSAGE == packet_type)
	{
		if(body_size != uds_read(uds_ptr, msg_ptr, sizeof(union message)))
		{
			EGUI_PRINT_ERROR("failed to read message body");
			return -1;
		}
		return 0;
	}
	else if(-1 == packet_type)
	{
		EGUI_PRINT_ERROR("failed to recv head");
		return -1;
	}
	else
	{
		EGUI_PRINT_ERROR("expect message actaully recv %s[%d]", 
			packet_type == PACKET_TYPE_RESPOND ? "respond" : (packet_type == PACKET_TYPE_REQUEST ? "request" : "unknown"), (int)packet_type);
		return -1;
	}
}
