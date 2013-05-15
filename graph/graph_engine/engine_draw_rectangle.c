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
engine_draw_rectangle
(si_t graphics_device_handle,
 si_t xaxis,
 si_t yaxis,
 si_t xsize,
 si_t ysize)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    /* 绘制矩形的四条边 */
    screen_set_h_line(&(gd->screen), &(gd->rectangle), &(gd->color), xaxis, yaxis, xaxis + xsize - 1, yaxis);
    screen_set_h_line(&(gd->screen), &(gd->rectangle), &(gd->color), xaxis, yaxis + ysize - 1, xaxis + xsize - 1, yaxis + ysize - 1);
    screen_set_v_line(&(gd->screen), &(gd->rectangle), &(gd->color), xaxis, yaxis, xaxis, yaxis + ysize - 1);
    screen_set_v_line(&(gd->screen), &(gd->rectangle), &(gd->color), xaxis + xsize - 1, yaxis, xaxis + xsize - 1, yaxis + ysize - 1);

    return 0;
}
