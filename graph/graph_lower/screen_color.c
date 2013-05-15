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

# include "../graph_lower.h"

/*
    struct fb_bitfield
    {
        // 最低位的偏移
        ui08_t offset;
        // 位序列的长度
        ui32_t length;
        // != 0 最重要的一位
        ui32_t msb_right;
    };
*/

struct color_limit global_color_limit;

/*
  将 struct color 转换成 color value

  颜色深度大于等于 8
*/
si_t
screen_color_to_value
(ui_t * value,
 struct color * c)
{
    * value = 0;
    * value = ((~((ui_t)-1 << global_var_screen_info.red.length) & c->r) << global_var_screen_info.red.offset);
    * value = ((~((ui_t)-1 << global_var_screen_info.green.length) & c->g) << global_var_screen_info.green.offset) | (* value);
    * value = ((~((ui_t)-1 << global_var_screen_info.blue.length) & c->b) << global_var_screen_info.blue.offset) | (* value);
    * value = ((~((ui_t)-1 << global_var_screen_info.transp.length) & c->a) << global_var_screen_info.transp.offset) | (* value);

    return 0;
}

/*
  将 color value 转换成 struct color

  颜色深度大于等于 8
*/
si_t
screen_value_to_color
(struct color * c,
 ui_t * value)
{
    c->r = ((~((ui_t)-1 << global_var_screen_info.red.length)) << global_var_screen_info.red.offset) & (* value);
    c->g = ((~((ui_t)-1 << global_var_screen_info.green.length)) << global_var_screen_info.green.offset) & (* value);
    c->b = ((~((ui_t)-1 << global_var_screen_info.blue.length)) << global_var_screen_info.blue.offset) & (* value);
    c->a = ((~((ui_t)-1 << global_var_screen_info.transp.length)) << global_var_screen_info.transp.offset) & (* value);

    return 0;
}

/*
  将 struct color 转换成 color index
*/
si_t
screen_color_to_index
(ui_t * index,
 struct color * c)
{
    if(global_fix_screen_info.visual == FB_VISUAL_PSEUDOCOLOR)
    {
        /* index = (c->r / c->r_limit) * cmap.len; */
        if(c->r == c->g && c->g == c->b)
        {
            * index = (c->r << global_var_screen_info.bits_per_pixel) >> global_var_screen_info.red.length;
        }
        else
        {
            return -1;
        }
    }

    return 0;
}

/*
  将 color index 转换成 struct color
*/
si_t
screen_index_to_color
(struct color * c,
 ui_t * index)
{
    if(global_fix_screen_info.visual == FB_VISUAL_DIRECTCOLOR)
    {
    }
    else if(global_fix_screen_info.visual == FB_VISUAL_PSEUDOCOLOR)
    {
        c->r = global_cmap.red[* index];
        c->g = global_cmap.green[* index];
        c->b = global_cmap.blue[* index];
        c->a = global_cmap.transp[* index];
    }

    return 0;
}

si_t
screen_color_limit()
{
    if(global_fix_screen_info.visual == FB_VISUAL_PSEUDOCOLOR ||
       global_fix_screen_info.visual == FB_VISUAL_DIRECTCOLOR)
    {
        global_color_limit.r_limit = global_cmap.red[(1 << global_var_screen_info.bits_per_pixel) - 1];
        global_color_limit.g_limit = global_cmap.green[(1 << global_var_screen_info.bits_per_pixel) - 1];
        global_color_limit.b_limit = global_cmap.blue[(1 << global_var_screen_info.bits_per_pixel) - 1];
        global_color_limit.a_limit = global_cmap.transp[(1 << global_var_screen_info.bits_per_pixel) - 1];
    }
    else if(global_fix_screen_info.visual == FB_VISUAL_TRUECOLOR)
    {
        /* limit = max */
        global_color_limit.r_limit = (1 << global_var_screen_info.red.length) - 1;
        global_color_limit.g_limit = (1 << global_var_screen_info.green.length) - 1;
        global_color_limit.b_limit = (1 << global_var_screen_info.blue.length) - 1;
        global_color_limit.a_limit = (1 << global_var_screen_info.transp.length) - 1;
    }
    else
    {
        /*
        globe_fix_screen_info.visual == FB_VISUAL_MONO01
        globe_fix_screen_info.visual == FB_VISUAL_MONO10
        globe_fix_screen_info.visual == FB_VISUAL_STATIC_PSEUDOCOLOR

        unspecified
        */

        return -1;
    }

    return 0;
}
