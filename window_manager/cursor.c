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

# include "window_manager.h"
# include "graph.h"
# include "data_structures.h"
# include "comm.h"
# include "log.h"
# include "event.h"

static struct graphics_device cursor_gd;
static si_t cursor_gd_handler = (si_t)&cursor_gd;

si_t cursor_init(char* cursor_type)
{
    FILE *ftmp = fopen("/tmp/egui.debug.out", "a");
    fprintf(ftmp, "%d\n", global_fix_screen_info.line_length);
    fclose(ftmp);
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
	cursor_gd.screen.video_access_mode = VIDEO_ACCESS_MODE_DIRECT;
	cursor_gd.screen.buffer_addr = NULL;

	engine_set_area(cursor_gd_handler, 0, 0, 15, 15);

	/* 白色 */
	engine_set_color(cursor_gd_handler, global_color_limit.r_limit, global_color_limit.g_limit, global_color_limit.b_limit, 0);

	/* 字体 */
	engine_set_font(cursor_gd_handler, 0);

	cursor_paint();

	return 0;
}

si_t cursor_paint()
{
    /* 设置工作区域 */
	rectangle_set(&cursor_gd.rectangle, global_wm.new_cursor.x - 7, global_wm.new_cursor.y - 7, 15, 15);
    switch(global_wm.cursor_shape)
    {
        case CURSOR_SHAPE_X:
                /* 黑色 */
			engine_set_color(cursor_gd_handler, 0, 0, 0, 0);
            engine_draw_point(cursor_gd_handler, global_wm.new_cursor.x, global_wm.new_cursor.y);
            engine_draw_x(cursor_gd_handler, global_wm.new_cursor.x, global_wm.new_cursor.y, 3, 3);
              /* 白色 */
			engine_set_color(cursor_gd_handler, global_color_limit.r_limit, global_color_limit.g_limit, global_color_limit.b_limit, 0);
            engine_draw_x(cursor_gd_handler, global_wm.new_cursor.x, global_wm.new_cursor.y, 3, 4);
            
            break;

        case CURSOR_SHAPE_HAND:
            break;

        default:
            break;
    }

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
