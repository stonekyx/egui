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

/*
    middle points for ellispe
*/
static si_t
__draw_ellipse_with_mp__
(struct screen * s,
 struct rectangle * r,
 struct color * c,
 si_t x0,
 si_t y0,
 si_t a,
 si_t b)
{
    f_t delta, aa, bb;
    si_t x, y;

    aa = a * a;
    bb = b * b;
    x = 0;
    y = b;

    delta = bb - aa * b + (0.25 * aa);

    while(aa * (y - 0.5) > bb * (x + 1))
    {
        screen_set_pixel(s, r, c, x0 + x, y0 + y);
        screen_set_pixel(s, r, c, x0 - x, y0 + y);
        screen_set_pixel(s, r, c, x0 + x, y0 - y);
        screen_set_pixel(s, r, c, x0 - x, y0 - y);

        if(delta <= 0)
        {
            delta += bb * ((x << 1) + 3);
        }
        else
        {
            delta += bb * ((x << 1) + 3) + aa * (2 - (y << 1));
            -- y;
        }
        ++ x;
    }

    delta = bb * (x + 0.5) * (x + 0.5) + aa * (y - 1) * (y - 1) - aa * bb;

    while(y > 0)
    {
        screen_set_pixel(s, r, c, x0 + x, y0 + y);
        screen_set_pixel(s, r, c, x0 - x, y0 + y);
        screen_set_pixel(s, r, c, x0 + x, y0 - y);
        screen_set_pixel(s, r, c, x0 - x, y0 - y);

        if(delta <= 0)
        {
            delta += bb * ((x << 1) + 2) + aa * (3 - (y << 1));
            ++ x;
        }
        else
        {
            delta += aa * (3 - (y << 1));
        }
        -- y;
    }

    screen_set_pixel(s, r, c, x0 + a, y0);
    screen_set_pixel(s, r, c, x0, y0 + b);
    screen_set_pixel(s, r, c, x0 - a, y0);
    screen_set_pixel(s, r, c, x0, y0 - b);

    return 0;
}

si_t
engine_draw_ellipse
(si_t graphics_device_handle,
 si_t x,
 si_t y,
 si_t a,
 si_t b)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    return __draw_ellipse_with_mp__(&(gd->screen), &(gd->rectangle), &(gd->color), x, y, a, b);
}
