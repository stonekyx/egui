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
奇数
***       ***
*  *     *  *
*   *   *   *
 *   * *   *
  *   *   *
   * * * *
    *   *
   * * * *
  *   *   *
 *   * *   *
*   *   *   *
*  *     *  *
***       ***

3:3:1:3:3
b:c:a:c:b

if(size % 13 == 0)
{
	a = 1;
	b = size / 13 * 3;
	c = b;
}

if(size % 13 != 0)
{
	a = 1;
	b = (size - a) / 4;
	c = (size - a) / 2 - b;
}

++++++++++++++++++++++++++++++++++++++++++++++++++++++++

偶数
***        ***
*  *      *  *
*   *    *   *
 *   *  *   *
  *   **   *
   *  **  *
    **  **
    **  **
   *  **  *
  *   **   *
 *   *  *   *
*   *    *   *
*  *      *  *
***        ***

3:3:2:3:3
b:c:a:c:b

if(size % 14 == 0)
{
	a = 2;
	b = size / 14 * 3;
	c = b;
}

if(size % 14 != 0)
{
	a = 2;
	b = (size - a) / 4;
	c = (size - a) / 2 - b;
}

*/
# if 0
si_t
engine_draw_x
(si_t graphics_device_handle,
 si_t x,
 si_t y,
 si_t size)
{
    si_t b;
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;


    if(size == 0)
    {
        return 0;
    }

    b = (size + 1) >> 1;

    /*
       +
        +
         +
    */
    screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size + b - 1, y - size, x, y - b + 1);
    screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size - b + 1, y + size, x, y + b - 1);
    screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y - size + b - 1, x - b + 1, y);
    screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size, y + size - b + 1, x + b - 1, y);

    /*
         +
        +
       +
    */
    screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size, y - size + b - 1, x + b - 1, y);
    screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y + size - b + 1, x - b + 1, y);
    screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size - b + 1, y - size, x, y - b + 1);
    screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size + b - 1, y + size, x, y + b - 1);

    /*
       h-line
    */
    screen_set_h_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y - size, x - size + b - 1, y - size);
    screen_set_h_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y + size, x - size + b - 1, y + size);
    screen_set_h_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size - b + 1, y - size, x + size, y - size);
    screen_set_h_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size - b + 1, y + size, x + size, y + size);

    /*
       v-line
    */
    screen_set_v_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y - size, x - size, y - size + b - 1);
    screen_set_v_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size, y - size, x + size, y - size + b - 1);
    screen_set_v_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y + size - b + 1, x - size, y + size);
    screen_set_v_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size, y + size - b + 1, x + size, y + size);

    return 0;
}
# endif


si_t
engine_draw_x
(si_t graphics_device_handle,
 si_t x,
 si_t y,
 si_t a,
 si_t b)
{
    si_t size = a + b;
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;


    if(a != 0 && b != 0)
    {
        /*
           +
            +
             +
        */
        screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - a, y - size, x, y - b);
        screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + a, y + size, x, y + b);
        screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y - a, x - b, y);
        screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size, y + a, x + b, y);

        /*
             +
            +
           +
        */
        screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size, y - a, x + b, y);
        screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y + a, x - b, y);
        screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + a, y - size, x, y - b);
        screen_set_x_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - a, y + size, x, y + b);

        /*
           h-line
        */
        screen_set_h_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y - size, x - a, y - size);
        screen_set_h_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y + size, x - a, y + size);
        screen_set_h_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + a, y - size, x + size, y - size);
        screen_set_h_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + a, y + size, x + size, y + size);

        /*
           v-line
        */
        screen_set_v_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y - size, x - size, y - a);
        screen_set_v_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size, y - size, x + size, y - a);
        screen_set_v_line(&(gd->screen), &(gd->rectangle), &(gd->color), x - size, y + a, x - size, y + size);
        screen_set_v_line(&(gd->screen), &(gd->rectangle), &(gd->color), x + size, y + a, x + size, y + size);
    }

    return 0;
}

