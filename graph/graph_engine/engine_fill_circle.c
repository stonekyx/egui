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

static si_t
__fill_circle_with_mp__
(struct screen * s,
 struct rectangle * r,
 struct color * c,
 si_t x0,
 si_t y0,
 si_t radius)
{
    si_t d = 5 - (radius << 2), x = 0, y = radius;

    while(x <= y)
    {
        screen_set_h_line(s, r, c, x0 - x, y0 + y, x0 + x, y0 + y);
        screen_set_h_line(s, r, c, x0 - x, y0 - y, x0 + x, y0 - y);
        screen_set_h_line(s, r, c, x0 - y, y0 + x, x0 + y, y0 + x);
        screen_set_h_line(s, r, c, x0 - y, y0 - x, x0 + y, y0 - x);

        if(d < 0)
        {
            d += (x << 3) + 12;
            ++ x;
        }
        else
        {
            d += ((x - y) << 3) + 20;
            -- y;
            ++ x;
        }
    }

    return 0;
}

si_t
engine_fill_circle
(si_t graphics_device_handle,
 si_t x0,
 si_t y0,
 si_t radius)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    return __fill_circle_with_mp__(&(gd->screen), &(gd->rectangle), &(gd->color), x0, y0, radius);
}
