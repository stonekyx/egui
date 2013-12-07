/**
 * @file event_listener.c
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

#include "event_listener.h"

#include <sys/times.h>

#include "log.h"

static void event_handler_init(struct events_handler* e, struct egui_uds* u, si_t t, addr_t a,
	si_t (*h)(struct egui_uds*, addr_t arg), void (*d)(struct egui_uds*, addr_t arg))
{
	e->uds = *u;
	e->event_type = t;
	e->handler = h;
	e->destructor = d;
	e->arg = a;
}

static void event_handler_exit(struct events_handler* e)
{
	if(NULL != e->destructor)
	{
		e->destructor(&e->uds, e->arg);
	}
	else
	{
		uds_exit(&e->uds);
	}
}

static si_t reload_fd_set(struct event_listener* s)
{
	struct list_node* n = NULL;
	si_t maxfd = 0;

	FD_ZERO(&s->read_set);
	FD_ZERO(&s->write_set);

	for(n = s->event_handler_set.node.next; n != &s->event_handler_set.node; n = n->next)
	{
		struct events_handler* e = (struct events_handler*)(n->data);
		if(SELECTER_EVENT_TYPE_READ == e->event_type)
		{
			FD_SET(e->uds.sock_fd, &s->read_set);
			if(e->uds.sock_fd + 1 > maxfd)
				maxfd = e->uds.sock_fd + 1;
		}
		else if(SELECTER_EVENT_TYPE_WRITE == e->event_type)
		{
			FD_SET(e->uds.sock_fd, &s->write_set);
			if(e->uds.sock_fd + 1 > maxfd)
				maxfd = e->uds.sock_fd + 1;
		}
	}

	return maxfd;
}

extern void event_listener_init(struct event_listener* l)
{
	list_init(&l->event_handler_set);
	FD_ZERO(&l->read_set);
	FD_ZERO(&l->write_set);
	l->callback = NULL;
}

extern void event_listener_add_read_event(struct event_listener* l, struct egui_uds* uds, addr_t arg,
	si_t (*handler)(struct egui_uds*, addr_t), void (*destructor)(struct egui_uds*, addr_t arg))
{
	struct events_handler e;
	event_handler_init(&e, uds, SELECTER_EVENT_TYPE_READ, arg, handler, destructor);
	list_push_back(&l->event_handler_set, &e, sizeof(struct events_handler));
}

extern void event_listener_add_write_event(struct event_listener* l, struct egui_uds* uds, addr_t arg,
	si_t (*handler)(struct egui_uds*, addr_t), void (*destructor)(struct egui_uds*, addr_t arg))
{
	struct events_handler e;
	event_handler_init(&e, uds, SELECTER_EVENT_TYPE_WRITE, arg, handler, destructor);
	list_push_back(&l->event_handler_set, &e, sizeof(struct events_handler));
}

static struct list_node* del_handler_while_loop(struct event_listener* l, struct list_node* n)
{
	struct events_handler* e = (struct events_handler*)(n->data);
	struct list_node* retval = NULL;
	event_handler_exit(e);

	/** 将此时节点指向当前的上一个节点，这样在删除当前节点后可以继续遍历至下一个节点 **/
	retval = n->prev;
	list_erase(&l->event_handler_set, retval->next);
	return retval;
}

static void event_listener_del_handler(struct event_listener* s, struct egui_uds* uds, si_t type)
{
	struct list_node* n = NULL;
	for(n = s->event_handler_set.node.next; n != &s->event_handler_set.node; n = n->next)
	{
		struct events_handler* e = (struct events_handler*)(n->data);
		if(e->event_type == type && e->uds.sock_fd == uds->sock_fd)
		{
			n = del_handler_while_loop(s, n);
		}
	}
}

extern void event_listener_del_read_event(struct event_listener* l, struct egui_uds* uds)
{
	event_listener_del_handler(l, uds, SELECTER_EVENT_TYPE_READ);
}

extern void event_listener_del_write_event(struct event_listener* l, struct egui_uds* uds)
{
	event_listener_del_handler(l, uds, SELECTER_EVENT_TYPE_WRITE);
}

extern si_t event_listener_exec(struct event_listener* l)
{
	while(1)
	{
		si_t select_ret = 0;
		si_t maxfd = 0;

		/**
		 * 将数组里面的fd放入对应的fd_set
		 **/
		maxfd = reload_fd_set(l);

		if(0 == maxfd)
		{
			EGUI_PRINT_INFO("event_listener listener set is empty; quit waiting.");
			return 0;
		}

		/**
		 * 监听等待事件
		 **/
		select_ret = select(maxfd, &l->read_set, &l->write_set, NULL, NULL);
		if(l->callback && l->callback()>0) {
			continue;
		}

		if(select_ret > 0)
		{
			struct list_node* n = NULL;
			/**
			 * 遍历event_handler列表 若命中则调用相应处理函数
			 **/
			for(n = l->event_handler_set.node.next; n != &l->event_handler_set.node; n = n->next)
			{
				struct events_handler* e = (struct events_handler*)(n->data);
				/** 发生读事件 **/
				if((SELECTER_EVENT_TYPE_READ == e->event_type && FD_ISSET(e->uds.sock_fd, &l->read_set))
					/** 发生写事件 **/
					|| (SELECTER_EVENT_TYPE_WRITE == e->event_type && FD_ISSET(e->uds.sock_fd, &l->write_set)))
				{
					int handler_ret = 0;

					/**
					 * 首先判断是否因为已建立连接的对方套结字关闭引起的select返回
					 **/
					if(is_socket_closed(&e->uds))
					{
						n = del_handler_while_loop(l, n);
						continue;
					}

					if(NULL == e->handler)
					{
						continue;
					}
					handler_ret = e->handler(&e->uds, e->arg);

					switch(handler_ret)
					{
					case SELECTER_RETURN_TYPE_CONTINUE:
						break;
					case SELECTER_RETURN_TYPE_REMOVE_HANDLER:
						n = del_handler_while_loop(l, n);
						break;
					case SELECTER_RETURN_TYPE_END:
						/** 关闭所有的通信句柄 **/
						for(n = l->event_handler_set.node.next; n != &l->event_handler_set.node; n = n->next)
						{
							struct events_handler* e = (struct events_handler*)(n->data);
							event_handler_exit(e);
						}
						return 0;
					default:
						EGUI_PRINT_ERROR("event handler return unknown type: %d", handler_ret);
						break;
					}
				}
			}
		}
		else if(select_ret < 0)
		{
			EGUI_PRINT_SYS_ERROR("Failed to get select result in selector: select()");
			return -1;
		}
	}
	return 0;
}

extern void event_listener_exit(struct event_listener* l)
{
	list_exit(&l->event_handler_set);
	FD_ZERO(&l->read_set);
	FD_ZERO(&l->write_set);
}
