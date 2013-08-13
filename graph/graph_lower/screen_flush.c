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
# include <limits.h>

# include <data_structures.h>

# include "../graph_lower.h"

si_t
screen_flush
(si_t x,
 si_t y,
 si_t width,
 si_t height)
{
    ui_t bit, byte_quantity, size, line_byte;
    ui_t h_offset, h_bit;
    byte_t * src_addr, * dst_addr;
    struct rectangle screen_area, temp_area, real_area;

    if(global_screen.video_access_mode == VIDEO_ACCESS_MODE_DIRECT)
    {
        return -1;
    }
    else if(global_screen.video_access_mode == VIDEO_ACCESS_MODE_BUFFER)
    {
        /* 屏幕区域 */
        screen_area.x = 0;
        screen_area.y = 0;
        screen_area.width = global_screen.width;
        screen_area.height = global_screen.height;
        /* 来源区域 */
        temp_area.x = x;
        temp_area.y = y;
        temp_area.width = width;
        temp_area.height = height;

        /* 区域非法 */
        if(area_intersection(&screen_area, &temp_area, &real_area) == -1)
        {
            return -1;
        }

        x = real_area.x;
        y = real_area.y;
        width = real_area.width;
        height = real_area.height;

        /* 获得起始像素相对于显存的位偏移量 */
        h_bit = y * global_fix_screen_info.line_length * CHAR_BIT + x * global_screen.color_depth;
        /* 水平线总共的位数 */
        bit = width * global_screen.color_depth;

        /* 每行所占的字节数 */
        line_byte = global_fix_screen_info.line_length;

        /* 竖直线的长度 */
        size = height;

        src_addr = global_screen.buffer_addr;
        dst_addr = global_screen.memory_addr;

        if(global_screen.color_depth < 8)
        {
            ;
        }
        else
        {
            /* 开始字节的偏移量 */
            h_offset = h_bit >> 3;

            src_addr += h_offset;
            dst_addr += h_offset;

            byte_quantity = bit >> 3;

            while(size)
            {
                -- size;

                memcpy(dst_addr, src_addr, byte_quantity);

                src_addr += line_byte;
                dst_addr += line_byte;
            }
        }
    }

    return 0;
}
