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

    *
   ***
  *****
 *******
*********

*********
 *******
  *****
   ***
    *

    *
   **
  ***
 ****
*****
 ****
  ***
   **
    *

*
**
***
****
*****
****
***
**
*

*/

si_t
engine_draw_arrow
(si_t graphics_device_handle,
 si_t x,
 si_t y,
 si_t size,
 si_t direction) /* t b l r */
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    switch(direction)
    {
        case 1: /* t */
            screen_set_h_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y, x + size, y);
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y, x, y - size);
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size, y, x, y - size);

            break;
        case 2: /* b */
            screen_set_h_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y, x + size, y);
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y, x, y + size);
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size, y, x, y + size);

            break;
        case 3: /* l */
            screen_set_v_line(&(gd->screen), &(gd->rectangle), &(gd->color), x, y - size, x, y + size);
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y, x, y - size);
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y, x, y + size);

            break;
        case 4: /* r */
            screen_set_v_line(&(gd->screen), &(gd->rectangle), &(gd->color), x, y - size, x, y + size);
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size, y, x, y - size);
            screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size, y, x, y + size);

            break;
        default:
            break;
    }

    return 0;
}
