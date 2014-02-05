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
# include <string.h>

# include "../graph_lower.h"

si_t
screen_clear
(struct screen * s,
 struct color * c)
{
    ui_t pixel, color;
    byte_t * addr;

    if(s->video_access_mode == VIDEO_ACCESS_MODE_BUFFER)
    {
        addr = s->buffer_addr;
    }
    else if(s->video_access_mode == VIDEO_ACCESS_MODE_DIRECT)
    {
        addr = s->memory_addr;
    }
    else
    {
        return -1;
    }

    /* 像素对应的字节数 */
    pixel = s->color_depth >> 3;

    /* 获得颜色的值 */
    if(global_fix_screen_info.visual == FB_VISUAL_DIRECTCOLOR)
    {
        /* 获得颜色索引 */
        screen_color_to_index(&color, c);
    }
    else if(global_fix_screen_info.visual == FB_VISUAL_PSEUDOCOLOR)
    {
        /* 获得颜色索引 */
        screen_color_to_index(&color, c);
    }
    else if(global_fix_screen_info.visual == FB_VISUAL_TRUECOLOR)
    {
        /* 获得颜色的值 */
        screen_color_to_value(&color, c);
    }

    {
        register ui_t i;
        const ui_t width_byte = s->width * (s->color_depth<<3);
        for(i=0; i<s->height; i++) {
            register byte_t *taddr = addr;
            while(taddr < addr+width_byte) {
                memcpy(taddr, &color, pixel);
                taddr += pixel;
            }
            addr += global_fix_screen_info.line_length;
        }
    }

    return 0;
}
