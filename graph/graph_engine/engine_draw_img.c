/**
 * @file engine_draw_img.c
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

# include "../graph_engine.h"
# include <string.h>

static const char bmp_type[] = ".bmp";
extern
si_t
engine_draw_img
(si_t graphics_device_handle, 
 char* path, 
 si_t align)
{
	/**
	 * 判断图片类型
	 * 理论上应该根据图片内的魔数来判断
	 * 此处根据文件名判断 之后需要改进
	 **/
	char* type = path + strlen(path);
	while(*type != '.')
	{
		-- type;
		if(type == path)
			return -1;
	}

	/**
	 * 暂时只支持bitmap
	 **/
	if(0 == strncmp(bmp_type, type, sizeof(bmp_type)))
	{
		return engine_draw_bitmap(graphics_device_handle, path, align);
	}
	else
	{
		return -1;
	}
}


