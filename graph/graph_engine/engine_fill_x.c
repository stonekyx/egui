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
engine_fill_x
(si_t graphics_device_handle,
 si_t x,
 si_t y,
 si_t a,
 si_t b)
{
    si_t size, i, x1, y1, x2, y2;
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    size = a + b;

    if(a != 0 && b != 0)
    {
        ++ b;

        /*
           +
            +
             +
        */
        for(x1 = x - size, y1 = y - size, x2 = x, y2 = y, i = 0; i < b; ++ i)
        {
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x1 + i, y1, x2, y2 - i);
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x1, y1 + i, x2 - i, y2);
        }

        for(x1 = x + size, y1 = y + size, x2 = x, y2 = y, i = 0; i < b; ++ i)
        {
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x1 - i, y1, x2, y2 + i);
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x1, y1 - i, x2 + i, y2);
        }

        /*
             +
            +
           +
        */
        for(x1 = x + size, y1 = y - size, x2 = x, y2 = y, i = 0; i < b; ++ i)
        {
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x1, y1 + i, x2 + i, y2);
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x1 - i, y1, x2, y2 - i);
        }

        for(x1 = x - size, y1 = y + size, x2 = x, y2 = y, i = 0; i < b; ++ i)
        {
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x1, y1 - i, x2 - i, y2);
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x1 + i, y1, x2, y2 + i);
        }
    }

    return 0;
}
