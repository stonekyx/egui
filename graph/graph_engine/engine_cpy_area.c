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
engine_cpy_area
(si_t graphics_device_handle,
 si_t mode,
 si_t dst_x,
 si_t dst_y,
 si_t src_x,
 si_t src_y,
 si_t x_size,
 si_t y_size)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    /* 没有缓冲区 */
    if(gd->screen.video_access_mode == VIDEO_ACCESS_MODE_DIRECT)
    {
        if(mode == CPY_AREA_MODE_BUFFER_TO_MEMORY)
        {
            /* do nothing */
        }
        else if(mode == CPY_AREA_MODE_MEMORY_TO_BUFFER)
        {
            /* do nothing */
        }
        else if(mode == CPY_AREA_MODE_BUFFER_TO_BUFFER)
        {
            /* do nothing */
        }
        else if(mode == CPY_AREA_MODE_MEMORY_TO_MEMORY)
        {
            screen_cpy_area
            (gd->screen.memory_addr,
             gd->screen.memory_addr,
             dst_x,
             dst_y,
             src_x,
             src_y,
             x_size,
             y_size);
        }
    }
    /* 有缓冲区 */
    else if(gd->screen.video_access_mode == VIDEO_ACCESS_MODE_BUFFER)
    {
        if(mode == CPY_AREA_MODE_BUFFER_TO_MEMORY)
        {
            screen_cpy_area
            (gd->screen.memory_addr,
             gd->screen.buffer_addr,
             dst_x,
             dst_y,
             src_x,
             src_y,
             x_size,
             y_size);
        }
        else if(mode == CPY_AREA_MODE_MEMORY_TO_BUFFER)
        {
            screen_cpy_area
            (gd->screen.buffer_addr,
             gd->screen.memory_addr,
             dst_x,
             dst_y,
             src_x,
             src_y,
             x_size,
             y_size);
        }
        else if(mode == CPY_AREA_MODE_BUFFER_TO_BUFFER)
        {
            screen_cpy_area
            (gd->screen.buffer_addr,
             gd->screen.buffer_addr,
             dst_x,
             dst_y,
             src_x,
             src_y,
             x_size,
             y_size);
        }
        else if(mode == CPY_AREA_MODE_MEMORY_TO_MEMORY)
        {
            screen_cpy_area
            (gd->screen.memory_addr,
             gd->screen.memory_addr,
             dst_x,
             dst_y,
             src_x,
             src_y,
             x_size,
             y_size);
        }
    }

    return 0;
}
