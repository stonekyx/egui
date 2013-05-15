/**
 * @file respond.c
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

#include "respond.h"
#include <string.h>

char* respond_type_to_str(si_t respond_type)
{
	switch(respond_type)
	{
	case RESPOND_TYPE_REGISTER_APPLICATION:
		return "RESPOND_TYPE_REGISTER_APPLICATION";
	case RESPOND_TYPE_CANCEL_APPLICATION:
		return "RESPOND_TYPE_CANCEL_APPLICATION";
	case RESPOND_TYPE_GRAPHICS_DEVICE_INIT:
		return "RESPOND_TYPE_GRAPHICS_DEVICE_INIT";
	case RESPOND_TYPE_GRAPHICS_DEVICE_EXIT:
		return "RESPOND_TYPE_GRAPHICS_DEVICE_EXIT";
	case RESPOND_TYPE_WINDOW_MANAGER_QUIT:
		return "RESPOND_TYPE_WINDOW_MANAGER_QUIT";
	case RESPOND_TYPE_UPDATE:
		return "RESPOND_TYPE_UPDATE";
	case RESPOND_TYPE_DESKTOP_DIRTY:
		return "RESPOND_TYPE_DESKTOP_DIRTY";
	case RESPOND_TYPE_REGISTER_WINDOW:
		return "RESPOND_TYPE_REGISTER_WINDOW";
	case RESPOND_TYPE_MOVE_WINDOW:
		return "RESPOND_TYPE_MOVE_WINDOW";
	case RESPOND_TYPE_RESIZE_WINDOW:
		return "RESPOND_TYPE_RESIZE_WINDOW";
	case RESPOND_TYPE_MINIMIZE_WINDOW:
		return "RESPOND_TYPE_MINIMIZE_WINDOW";
	case RESPOND_TYPE_MAXIMIZE_WINDOW:
		return "RESPOND_TYPE_MAXIMIZE_WINDOW";
	case RESPOND_TYPE_RESTORE_WINDOW:
		return "RESPOND_TYPE_RESTORE_WINDOW";
	case RESPOND_TYPE_CANCEL_WINDOW:
		return "RESPOND_TYPE_CANCEL_WINDOW";
	case RESPOND_TYPE_ACTIVATE_WINDOW:
		return "RESPOND_TYPE_ACTIVATE_WINDOW";
	case RESPOND_TYPE_CLEAR:
		return "RESPOND_TYPE_CLEAR";
	case RESPOND_TYPE_CLEAR_WHITE:
		return "RESPOND_TYPE_CLEAR_WHITE";
	case RESPOND_TYPE_CLEAR_BLACK:
		return "RESPOND_TYPE_CLEAR_BLACK";
	case RESPOND_TYPE_DRAW_LINE:
		return "RESPOND_TYPE_DRAW_LINE";
	case RESPOND_TYPE_DRAW_POINT:
		return "RESPOND_TYPE_DRAW_POINT";
	case RESPOND_TYPE_DRAW_RECTANGLE:
		return "RESPOND_TYPE_DRAW_RECTANGLE";
	case RESPOND_TYPE_DRAW_POLYGON:
		return "RESPOND_TYPE_DRAW_POLYGON";
	case RESPOND_TYPE_DRAW_CIRCLE:
		return "RESPOND_TYPE_DRAW_CIRCLE";
	case RESPOND_TYPE_DRAW_ELLIPSE:
		return "RESPOND_TYPE_DRAW_ELLIPSE";
	case RESPOND_TYPE_DRAW_ARROW:
		return "RESPOND_TYPE_DRAW_ARROW";
	case RESPOND_TYPE_DRAW_X:
		return "RESPOND_TYPE_DRAW_X";
	case RESPOND_TYPE_DRAW_BITMAP:
		return "RESPOND_TYPE_DRAW_BITMAP";
	case RESPOND_TYPE_DRAW_IMG:
		return "RESPOND_TYPE_DRAW_IMG";
	case RESPOND_TYPE_FILL_RECTANGLE:
		return "RESPOND_TYPE_FILL_RECTANGLE";
	case RESPOND_TYPE_FILL_CIRCLE:
		return "RESPOND_TYPE_FILL_CIRCLE";
	case RESPOND_TYPE_FILL_ELLIPSE:
		return "RESPOND_TYPE_FILL_ELLIPSE";
	case RESPOND_TYPE_FILL_ARROW:
		return "RESPOND_TYPE_FILL_ARROW";
	case RESPOND_TYPE_FILL_X:
		return "RESPOND_TYPE_FILL_X";
	case RESPOND_TYPE_SHOW_TEXT:
		return "RESPOND_TYPE_SHOW_TEXT";
	case RESPOND_TYPE_CPY_AREA:
		return "RESPOND_TYPE_CPY_AREA";
	case RESPOND_TYPE_SET_COLOR:
		return "RESPOND_TYPE_SET_COLOR";
	case RESPOND_TYPE_SET_AREA:
		return "RESPOND_TYPE_SET_AREA";
	case RESPOND_TYPE_SET_FONT:
		return "RESPOND_TYPE_SET_FONT";
	case RESPOND_TYPE_GET_COLOR_LIMIT:
		return "RESPOND_TYPE_GET_COLOR_LIMIT";
	case RESPOND_TYPE_GET_SCREEN_WIDTH:
		return "RESPOND_TYPE_GET_SCREEN_WIDTH";
	case RESPOND_TYPE_GET_SCREEN_HEIGHT:
		return "RESPOND_TYPE_GET_SCREEN_HEIGHT";
	case RESPOND_TYPE_GET_AREA:
		return "RESPOND_TYPE_GET_AREA";
	case RESPOND_TYPE_GET_COLOR:
		return "RESPOND_TYPE_GET_COLOR";
	case RESPOND_TYPE_GET_FONT_WIDTH:
		return "RESPOND_TYPE_GET_FONT_WIDTH";
	case RESPOND_TYPE_GET_FONT_HEIGHT:
		return "RESPOND_TYPE_GET_FONT_HEIGHT";
	case RESPOND_TYPE_GET_WINDOW_TITLE:
		return "RESPOND_TYPE_GET_WINDOW_TITLE";
	default:
		return NULL;
	}
}


static void respond_set_type_and_size(union respond* respond_ptr, si_t type, si_t data_type, si_t size)
{
	respond_ptr->normal.respond_data_type = data_type;
	respond_ptr->normal.type = type;
	respond_ptr->normal.body_len = size;
}

extern void respond_set_normal(union respond* respond_ptr, si_t type, si_t retval)
{
	respond_set_type_and_size(respond_ptr, type, RESPOND_DATA_TYPE_FIX, sizeof(si_t));
	respond_ptr->normal.retval = retval;
}

extern void respond_set_rectangle(union respond* respond_ptr, si_t type, struct rectangle* retval)
{
	respond_set_type_and_size(respond_ptr, type, RESPOND_DATA_TYPE_FIX, sizeof(struct rectangle));
	respond_ptr->rectangle.retval = *retval;
}

extern void respond_set_color_limit(union respond* respond_ptr, si_t type, struct color_limit* retval)
{
	respond_set_type_and_size(respond_ptr, type, RESPOND_DATA_TYPE_FIX, sizeof(struct color_limit));
	respond_ptr->color_limit.retval = *retval;
}

extern void respond_set_color(union respond* respond_ptr, si_t type, struct color* retval)
{
	respond_set_type_and_size(respond_ptr, type, RESPOND_DATA_TYPE_FIX, sizeof(struct color));
	respond_ptr->color.retval = *retval;
}

extern void respond_set_va(union respond* respond_ptr, si_t type, addr_t va_arg, si_t len)
{
	respond_set_type_and_size(respond_ptr, type, RESPOND_DATA_TYPE_VARIABLE, len);
	respond_ptr->va.retval = va_arg;
}

extern void respond_set_empty(union respond* respond_ptr)
{
	respond_ptr->normal.body_len = 0;
}

extern si_t respond_get_type(union respond* respond_ptr)
{
	return respond_ptr->normal.type;
}

extern si_t respond_get_len(union respond* respond_ptr)
{
	return respond_ptr->normal.body_len;
}

extern addr_t respond_get_value(union respond* respond_ptr)
{
	/**
	 * 可变长的回应包的retval是一个地址，直接返回即可
	 **/
	if(respond_ptr->normal.respond_data_type == RESPOND_DATA_TYPE_VARIABLE)
		return (addr_t)respond_ptr->normal.retval;
	else
		return &respond_ptr->normal.retval;
}
