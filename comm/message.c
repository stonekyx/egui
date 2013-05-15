/**
 * @file message.c
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

#include <string.h>
#include "message.h"

extern char* message_type_to_str(si_t message_type)
{
	switch(message_type)
	{
	case MESSAGE_TYPE_KEYBD_ENTER:
		return "MESSAGE_TYPE_KEYBD_ENTER";
	case MESSAGE_TYPE_KEYBD_LEAVE:
		return "MESSAGE_TYPE_KEYBD_LEAVE";
	case MESSAGE_TYPE_KEYBD_PRESS:
		return "MESSAGE_TYPE_KEYBD_PRESS";
	case MESSAGE_TYPE_KEYBD_RELEASE:
		return "MESSAGE_TYPE_KEYBD_RELEASE";
	case MESSAGE_TYPE_KEYBD_CLICK:
		return "MESSAGE_TYPE_KEYBD_CLICK";
	case MESSAGE_TYPE_MOUSE_MOVE:
		return "MESSAGE_TYPE_MOUSE_MOVE";
	case MESSAGE_TYPE_MOUSE_ENTER:
		return "MESSAGE_TYPE_MOUSE_ENTER";
	case MESSAGE_TYPE_MOUSE_LEAVE:
		return "MESSAGE_TYPE_MOUSE_LEAVE";
	case MESSAGE_TYPE_MOUSE_PRESS:
		return "MESSAGE_TYPE_MOUSE_PRESS";
	case MESSAGE_TYPE_MOUSE_RELEASE:
		return "MESSAGE_TYPE_MOUSE_RELEASE";
	case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
		return "MESSAGE_TYPE_MOUSE_SINGLE_CLICK";
	case MESSAGE_TYPE_MOUSE_DOUBLE_CLICK:
		return "MESSAGE_TYPE_MOUSE_DOUBLE_CLICK";
	case MESSAGE_TYPE_WIDGET_REPAINT:
		return "MESSAGE_TYPE_WIDGET_REPAINT";
	case MESSAGE_TYPE_WIDGET_SHOW:
		return "MESSAGE_TYPE_WIDGET_SHOW";
	case MESSAGE_TYPE_WIDGET_MOVE:
		return "MESSAGE_TYPE_WIDGET_MOVE";
	case MESSAGE_TYPE_WIDGET_RESIZE:
		return "MESSAGE_TYPE_WIDGET_RESIZE";
	case MESSAGE_TYPE_WINDOW_ACTIVATE:
		return "MESSAGE_TYPE_WINDOW_ACTIVATE";
	case MESSAGE_TYPE_WINDOW_DEACTIVATE:
		return "MESSAGE_TYPE_WINDOW_DEACTIVATE";
	case MESSAGE_TYPE_WINDOW_STATE_CHANGE:
		return "MESSAGE_TYPE_WINDOW_STATE_CHANGE";
	case MESSAGE_TYPE_WINDOW_CLOSE:
		return "MESSAGE_TYPE_WINDOW_CLOSE";
	case MESSAGE_TYPE_WINDOW_MAXIMIZE:
		return "MESSAGE_TYPE_WINDOW_MAXIMIZE";
	case MESSAGE_TYPE_WINDOW_MINIMIZE:
		return "MESSAGE_TYPE_WINDOW_MINIMIZE";
	case MESSAGE_TYPE_WINDOW_RESTORE:
		return "MESSAGE_TYPE_WINDOW_RESTORE";
	case MESSAGE_TYPE_WINDOW_REGISTER:
		return "MESSAGE_TYPE_WINDOW_REGISTER";
	case MESSAGE_TYPE_WINDOW_CANCEL:
		return "MESSAGE_TYPE_WINDOW_CANCEL";
	case MESSAGE_TYPE_APPLICATION_QUIT:
		return "MESSAGE_TYPE_APPLICATION_QUIT";
	default:
		return NULL;
		break;
	}
}
/**
 * 基本消息
 **/
extern void message_set_window_descriptor(union message* m, si_t w)
{
	m->base.window_descriptor = w;
}
extern si_t message_get_window_descriptor(union message* m)
{
	return m->base.window_descriptor;
}

extern void message_set_type(union message* m, si_t t)
{
	m->base.type = t;
}
extern si_t message_get_type(union message* m)
{
	return m->base.type;
}

extern void message_set_cursor_position(union message* m, struct point* c)
{
	m->base.cursor_position = *c;
}
extern struct point* message_get_cursor_position(union message* m)
{
	return &m->base.cursor_position;
}

extern void message_set_time(union message* m, struct timeval* t)
{
	m->base.time = *t;
}
extern struct timeval* message_get_time(union message* m)
{
	return &m->base.time;
}

static void message_set_window(union message* m, si_t w, si_t t)
{
	message_set_window_descriptor(m, w);
	message_set_type(m, t);
}

/**
 * 针对键盘消息
 **/
extern void message_set_keybd(union message* msg, si_t t, si_t c, si_t v, si_t m)
{
	message_set_type(msg, t);
	msg->keybd.code = c;
	msg->keybd.value = v;
	msg->keybd.mask = m;
}

extern si_t message_get_keybd_code(union message* m)
{
	return m->keybd.code;
}

extern si_t message_get_keybd_value(union message* m)
{
	return m->keybd.value;
}

extern si_t message_get_keybd_mask(union message* m)
{
	return m->keybd.mask;
}

extern void message_set_keyboard_leave(union message* m)
{
	message_set_type(m, MESSAGE_TYPE_KEYBD_LEAVE);
}

/**
 * 针对鼠标消息
 **/
extern void message_set_mouse(union message* msg, si_t t, si_t c, si_t v, si_t m)
{
	message_set_type(msg, t);
	msg->mouse.code = c;
	msg->mouse.value = v;
	msg->mouse.mask = m;
}

extern si_t message_get_mouse_code(union message* m)
{
	return m->mouse.code;
}

extern si_t message_get_mouse_value(union message* m)
{
	return m->mouse.value;
}

extern si_t message_get_mouse_mask(union message* m)
{
	return m->mouse.mask;
}

/**
 * 针对窗口的消息
 **/
extern void message_set_widget_show(union message* m, si_t w)
{
	message_set_window(m, w, MESSAGE_TYPE_WIDGET_SHOW);
}

extern void message_set_widget_repaint(union message* m, si_t w, struct rectangle* r)
{
	message_set_window(m, w, MESSAGE_TYPE_WIDGET_REPAINT);
	m->widget_repaint.area = *r;
}

extern struct rectangle* message_get_widget_repaint_area(union message* m)
{
	return &m->widget_repaint.area;
}

extern void message_set_widget_move(union message* m, si_t w, struct point* p)
{
	message_set_window(m, w, MESSAGE_TYPE_WIDGET_MOVE);
	m->widget_move.position = *p;
}

extern struct point* message_get_widget_move_poisition(union message* m)
{
	return &m->widget_move.position;
}

extern void message_set_widget_resize(union message* m, si_t w, struct rectangle* area)
{
	message_set_window(m, w, MESSAGE_TYPE_WIDGET_RESIZE);
	m->widget_resize.area = *area;
}

extern struct rectangle* message_get_widget_resize_area(union message* m)
{
	return &m->widget_resize.area;
}

extern void message_set_window_activate(union message* m, si_t w)
{
	message_set_window(m, w, MESSAGE_TYPE_WINDOW_ACTIVATE);
}

extern void message_set_window_deactivate(union message* m, si_t w)
{
	message_set_window(m, w, MESSAGE_TYPE_WINDOW_DEACTIVATE);
}

extern void message_set_window_minimize(union message* m, si_t w)
{
	message_set_window(m, w, MESSAGE_TYPE_WINDOW_MINIMIZE);
}

extern void message_set_window_maximize(union message* m, si_t w)
{
	message_set_window(m, w, MESSAGE_TYPE_WINDOW_MAXIMIZE);
}

extern void message_set_window_restore(union message* m, si_t w)
{
	message_set_window(m, w, MESSAGE_TYPE_WINDOW_RESTORE);
}

extern void message_set_window_close(union message* m, si_t w)
{
	message_set_window(m, w, MESSAGE_TYPE_WINDOW_CLOSE);
}

extern void message_set_window_register(union message* m, si_t w)
{
	message_set_window(m, w, MESSAGE_TYPE_WINDOW_REGISTER);
}

extern void message_set_window_cancel(union message* m, si_t w)
{
	message_set_window(m, w, MESSAGE_TYPE_WINDOW_CANCEL);
}
