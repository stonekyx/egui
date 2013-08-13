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

#include <color.h>

static si_t
get_actual_areas
(si_t dst_x,
 si_t dst_y,
 si_t src_x,
 si_t src_y,
 si_t width,
 si_t height,
 struct rectangle * const ans_src,
 struct rectangle * const ans_dst)
{
    static struct rectangle src_area, dst_area, temp_src_area, temp_dst_area, screen_area, temp_area;

    /* 屏幕区域 */
    screen_area.x = 0;
    screen_area.y = 0;
    screen_area.width = global_screen.width;
    screen_area.height = global_screen.height;
    /* 来源区域 */
    src_area.x = src_x;
    src_area.y = src_y;
    src_area.width = width;
    src_area.height = height;
    /* 目标区域 */
    dst_area.x = dst_x;
    dst_area.y = dst_y;
    dst_area.width = width;
    dst_area.height = height;

    /* 来源区域非法 */
    if(area_intersection(&screen_area, &src_area, &temp_src_area) == -1)
    {
        return -1;
    }
    /* 目标区域非法 */
    if(area_intersection(&screen_area, &dst_area, &temp_dst_area) == -1)
    {
        return -1;
    }

    /* 移动目标区域与屏幕区域的交集 */
    temp_area.x = temp_dst_area.x + src_x - dst_x;
    temp_area.y = temp_dst_area.y + src_y - dst_y;
    temp_area.width = temp_dst_area.width;
    temp_area.height = temp_dst_area.height;

    /* 获得实际的来源区域 */
    if(area_intersection(&temp_area, &temp_src_area, &src_area) == -1)
    {
        return -1;
    }

    /* 移动来源区域与屏幕区域的交集 */
    temp_area.x = temp_src_area.x - src_x + dst_x;
    temp_area.y = temp_src_area.y - src_y + dst_y;
    temp_area.width = temp_src_area.width;
    temp_area.height = temp_src_area.height;

    /* 获得实际的目标区域 */
    if(area_intersection(&temp_area, &temp_dst_area, &dst_area) == -1)
    {
        return -1;
    }

    *ans_src = src_area;
    *ans_dst = dst_area;

    return 0;
}

si_t
screen_cpy_area
(void * dst,
 void * src,
 si_t dst_x,
 si_t dst_y,
 si_t src_x,
 si_t src_y,
 si_t width,
 si_t height)
{
    ui_t bit, byte_quantity, size, line_byte;
    ui_t src_h_offset, src_h_bit;
    ui_t dst_h_offset, dst_h_bit;
    byte_t * src_addr;
    byte_t * dst_addr;
    struct rectangle src_area, dst_area;

    if(get_actual_areas(dst_x, dst_y, src_x, src_y, width, height,
                &src_area, &dst_area)<0) {
        return -1;
    }

    src_x = src_area.x;
    src_y = src_area.y;
    dst_x = dst_area.x;
    dst_y = dst_area.y;
    width = src_area.width;
    height = dst_area.height;

    /* 获得起始像素相对于显存的位偏移量 */
    src_h_bit = src_y * global_fix_screen_info.line_length * CHAR_BIT + src_x * global_screen.color_depth;
    dst_h_bit = dst_y * global_fix_screen_info.line_length * CHAR_BIT + dst_x * global_screen.color_depth;
    /* 水平线总共的位数 */
    bit = width * global_screen.color_depth;

    /* 每行所占的字节数 */
    line_byte = global_fix_screen_info.line_length;
    /* 竖直线的长度 */
    size = height;

    src_addr = src;
    dst_addr = dst;

    if(global_screen.color_depth < 8)
    {
        /* do nothing */
    }
    else
    {
        byte_quantity = bit >> 3;

        /* 目标区域在来源区域之后并且两个区域有重叠 */
        if(src_h_bit <= dst_h_bit && src_addr == dst_addr)
        {
            /* 获得最后一行起始像素相对于显存的位偏移量 */
            src_h_bit = (src_y + height - 1) * global_fix_screen_info.line_length * CHAR_BIT + src_x * global_screen.color_depth;
            dst_h_bit = (dst_y + height - 1) * global_fix_screen_info.line_length * CHAR_BIT + dst_x * global_screen.color_depth;

            /* 开始字节的偏移量 */
            src_h_offset = src_h_bit >> 3;
            dst_h_offset = dst_h_bit >> 3;

            src_addr += src_h_offset;
            dst_addr += dst_h_offset;

            while(size)
            {
                -- size;

                memmove(dst_addr, src_addr, byte_quantity);

                src_addr -= line_byte;
                dst_addr -= line_byte;
            }
        }
        /* 目标区域在来源区域之前 */
        else
        {
            /* 开始字节的偏移量 */
            src_h_offset = src_h_bit >> 3;
            dst_h_offset = dst_h_bit >> 3;

            src_addr += src_h_offset;
            dst_addr += dst_h_offset;

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

si_t
screen_cpy_area_alpha
(void * dst,
 void * src,
 si_t dst_x,
 si_t dst_y,
 si_t src_x,
 si_t src_y,
 si_t width,
 si_t height)
{
    struct rectangle src_area, dst_area;

    if(get_actual_areas(dst_x, dst_y, src_x, src_y, width, height,
                &src_area, &dst_area)<0) {
        return -1;
    }

    src_x = src_area.x;
    src_y = src_area.y;
    dst_x = dst_area.x;
    dst_y = dst_area.y;
    width = src_area.width;
    height = dst_area.height;

    {
        register int i;
        for(i=src_x; i<src_x+width; i++) {
            register int j;
            for(j=src_y; j<src_y+height; j++) {
                static struct color src_pix;
                if(screen_get_pixel_raw(src, global_screen.width,
                            global_screen.height,
                            global_screen.color_depth,
                            &src_area, &src_pix, i, j)<0) {
                    continue;
                }
                screen_set_pixel_raw(dst, global_screen.width,
                        global_screen.height,
                        global_screen.color_depth,
                        &dst_area, &src_pix,
                        i-src_x+dst_x, j-src_y+dst_y);
                /* automatic alpha blended */
            }
        }
    }

    return 0;
}
