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

# include "../graph_engine.h"

si_t
engine_update
(si_t graphics_device_handle)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    /* 没有缓冲区 */
    if(gd->screen.video_access_mode == VIDEO_ACCESS_MODE_DIRECT)
    {
        /* do nothing */
    }
    /* 有缓冲区 */
    else if(gd->screen.video_access_mode == VIDEO_ACCESS_MODE_BUFFER)
    {
        /* 将 buffer 复制到 memory */
        screen_cpy_area
        (gd->screen.memory_addr,
         gd->screen.buffer_addr,
         gd->rectangle.x,
         gd->rectangle.y,
         gd->rectangle.x,
         gd->rectangle.y,
         gd->rectangle.width,
         gd->rectangle.height);
    }

    return 0;
}
