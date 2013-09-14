/**
 * @file
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
# include "cursor.h"

# include <string.h>
# include <unistd.h>

# include "window_manager.h"
# include "graph.h"
# include "data_structures.h"
# include "comm.h"
# include "log.h"
# include "event.h"
# include "config.h"

#define DEFAULT_CURSOR PACKAGE_DATA_PATH"/resource/default.cur"
#define NONINST_DEFAULT_CURSOR "../"PACKAGE_SRC_PATH"/resource/default.cur"
static struct graphics_device cursor_gd;
static si_t cursor_gd_handler = (si_t)&cursor_gd;
static struct cursor cursor;

si_t cursor_init(char* cursor_type)
{
	global_wm.old_cursor.x = global_screen.width  >> 1;
	global_wm.old_cursor.y = global_screen.height >> 1;
	global_wm.new_cursor.x = global_screen.width  >> 1;
	global_wm.new_cursor.y = global_screen.height >> 1;

	if(0 == strcmp(cursor_type, "CURSOR_SHAPE_X"))
	{
		global_wm.cursor_shape = CURSOR_SHAPE_X;
	}
	else
	{
		EGUI_PRINT_ERROR("failed to init cursor: cursor_type %s is not supported!", cursor_type);
		return -1;
	}

	cursor_gd.screen = global_screen;
	cursor_gd.screen.video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	cursor_gd.screen.buffer_addr = malloc(global_screen.size);

	engine_set_area(cursor_gd_handler, 0, 0, 15, 15);

	/* 初始化光标信息 */
	if(0 == access(DEFAULT_CURSOR, F_OK)) {
		init_cursor(DEFAULT_CURSOR, &cursor);
	} else {
		init_cursor(NONINST_DEFAULT_CURSOR, &cursor);
	}

	/* 白色 */
	engine_set_color(cursor_gd_handler, global_color_limit.r_limit, global_color_limit.g_limit, global_color_limit.b_limit, 0);

	/* 字体 */
	engine_set_font(cursor_gd_handler, 0);

	cursor_paint();

	return 0;
}

static si_t cursor_get_refresh_area(struct rectangle *result)
{
    struct rectangle old, new;
    old.x = global_wm.old_cursor.x;
    old.y = global_wm.old_cursor.y;
    new.x = global_wm.new_cursor.x;
    new.y = global_wm.new_cursor.y;
    old.width = new.width = cursor.ciHeader[0].ciWidth;
    old.height = new.height = cursor.ciHeader[0].ciHeight;
    return area_union(&old, &new, result); /* real result is not returned! */
}

si_t cursor_paint()
{
    struct rectangle refarea;
    if(-1 == cursor_get_refresh_area(&refarea)) {
        EGUI_PRINT_ERROR("Error in area union code");
        return -1;
    }
    screen_cpy_area(cursor_gd.screen.buffer_addr, global_screen.buffer_addr,
            refarea.x, refarea.y,
            refarea.x, refarea.y,
            refarea.width, refarea.height);
    /* 设置工作区域 */
	rectangle_set(&cursor_gd.rectangle, global_wm.new_cursor.x, global_wm.new_cursor.y , cursor.ciHeader[0].ciWidth, cursor.ciHeader[0].ciHeight);
    switch(global_wm.cursor_shape)
    {
        case CURSOR_SHAPE_X:
                /* 黑色 */
			engine_draw_cursor(cursor_gd_handler, global_wm.new_cursor.x, global_wm.new_cursor.y, cursor.ciHeader[0].ciWidth, cursor.ciHeader[0].ciHeight, &cursor);
            
            break;

        case CURSOR_SHAPE_HAND:
            break;

        default:
            break;
    }
    screen_cpy_area(global_screen.memory_addr, cursor_gd.screen.buffer_addr,
            refarea.x, refarea.y,
            refarea.x, refarea.y,
            refarea.width, refarea.height);

    return 0;
}

si_t cursor_show()
{
    union message * message;

    /* 消息队列非空 */
    if(list_empty(&(global_wm.message_list)) == 0)
    {
        /* 获得第一个消息 */
        message = list_front(&(global_wm.message_list));

        /* 绘制鼠标并且更新光标的坐标 */
        if(message->base.type == MESSAGE_TYPE_MOUSE_MOVE)
        {
            if(message->mouse.code == INPUT_CODE_MOUSE_X_OFFSET ||
               message->mouse.code == INPUT_CODE_MOUSE_Y_OFFSET)
            {
                cursor_paint();

                global_wm.old_cursor.x = global_wm.new_cursor.x;
                global_wm.old_cursor.y = global_wm.new_cursor.y;
            }
        }
    }

    return 0;
}

si_t cursor_exit()
{
    memset(&cursor_gd, 0, sizeof(struct graphics_device));

    return 0;
}
