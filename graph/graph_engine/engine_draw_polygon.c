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
engine_draw_polygon
(si_t graphics_device_handle,
 struct point * p,
 ui_t point_count)
{
    ui_t i = 0;

    if(point_count <= 0) {
        return 0;
    }

    -- point_count;

    while(i < point_count)
    {
        engine_draw_line(graphics_device_handle, p[i].x, p[i].y, p[i + 1].x, p[i + 1].y);
        ++ i;
    }

    engine_draw_line(graphics_device_handle, p[point_count].x, p[point_count].y, p[0].x, p[0].y);

    return 0;
}
