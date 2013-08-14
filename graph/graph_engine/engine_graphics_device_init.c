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

# include <stdlib.h>

# include "../graph_engine.h"
# include "log.h"

si_t
engine_graphics_device_init
(si_t x,
 si_t y,
 si_t width,
 si_t height,
 si_t r,
 si_t g,
 si_t b,
 si_t a,
 si_t font)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)malloc(sizeof(struct graphics_device));

    if(gd == NULL)
    {
		EGUI_PRINT_SYS_ERROR("failed to malloc for graphics_device: malloc()");
        return 0;
    }

    gd->rectangle.x = x;
    gd->rectangle.y = y;
    gd->rectangle.width = width;
    gd->rectangle.height = height;
    gd->color.r = r;
    gd->color.g = g;
    gd->color.b = b;
    gd->color.a = a;
    gd->font = font;

    /* 设置屏幕结构 */
    gd->screen.width = global_screen.width;
    gd->screen.height = global_screen.height;
    gd->screen.color_depth = global_screen.color_depth;
    /* XXX: Default to VIDEO_ACCESS_MODE_DIRECT? */
    /* XXX: Access mode is not set. */

    /* 指向缓冲区 */
    gd->screen.memory_addr = global_screen.buffer_addr;
    /* XXX: gd->screen.buffer_addr is not set, not even used. */
    /* 显存的大小 */
    gd->screen.size = global_screen.size;

    return (si_t)gd;
}
