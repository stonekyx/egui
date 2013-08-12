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
screen_set_v_line
(struct screen * s,
 struct rectangle * a,
 struct color * c,
 si_t x1,
 si_t y1,
 si_t x2,
 si_t y2)
{
    ui_t offset, color, pixel, n, size, bit, line_bit;
    byte_t * addr, * video;
    byte_t temp;
    struct rectangle result_area, screen_area;

    if(s->video_access_mode == VIDEO_ACCESS_MODE_DIRECT)
    {
        video = s->memory_addr;
    }
    else if(s->video_access_mode == VIDEO_ACCESS_MODE_BUFFER)
    {
        video = s->buffer_addr;
    }
    else
    {
        return -1;
    }

    screen_area.x = 0;
    screen_area.y = 0;
    screen_area.width = s->width;
    screen_area.height = s->height;

    /* 工作区域不在屏幕区域 */
    if(area_intersection(a, &screen_area, &result_area) == -1)
    {
        return -1;
    }

    /* 让 y1 总是小于 y2 */
    if(y2 < y1)
    {
        /* exchange y1 y2 */
        y1 = y1 ^ y2;
        y2 = y2 ^ y1;
        y1 = y1 ^ y2;
    }

    /* 竖直线不在工作区域与屏幕区域的交集内 */
    if
    ((x2 < result_area.x) ||
     (x1 > result_area.x + result_area.width - 1) ||
     (y2 < result_area.y) ||
     (y1 > result_area.y + result_area.height - 1))
    {
        return -1;
    }
    /* 竖直线在工作区域与屏幕区域的交集内 */
    else
    {
        y1 = y1 < result_area.y ? result_area.y : y1;
        y2 = y2 > result_area.y + result_area.height - 1 ? result_area.y + result_area.height - 1 : y2;
    }

    if(global_fix_screen_info.visual == FB_VISUAL_MONO01)
    {
        /* 白色 */
        if
        ((c->r == global_color_limit.r_limit) &&
         (c->g == global_color_limit.g_limit) &&
         (c->g == global_color_limit.g_limit))
        {
            color = 0;
        }
        /* 黑色 */
        else if
        ((c->r == 0) &&
         (c->g == 0) &&
         (c->g == 0))
        {
            color = 1;
        }
    }
    else if(global_fix_screen_info.visual == FB_VISUAL_MONO10)
    {
        /* 白色 */
        if
        ((c->r == global_color_limit.r_limit) &&
         (c->g == global_color_limit.g_limit) &&
         (c->g == global_color_limit.g_limit))
        {
            color = 1;
        }
        /* 黑色 */
        else if
        ((c->r == 0) &&
         (c->g == 0) &&
         (c->g == 0))
        {
            color = 0;
        }
    }

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

    /* 开始位的偏移量 */
    bit = y1 * global_fix_screen_info.line_length * CHAR_BIT + x1 * s->color_depth;
    /* 每行所占的位数 */
    line_bit = global_fix_screen_info.line_length * CHAR_BIT;
    /* 竖直线的长度 */
    size = y2 - y1 + 1;

    if(s->color_depth < 8)
    {
        if(s->color_depth == 1)
        {
            while(size)
            {
                offset = bit & 7;
                addr = video + (bit >> 3);
                /* 获得颜色 */
                temp = color;
                /* 调整颜色 */
                temp = (temp & 0X1) << offset;
                /* 将相关位设为 0 */
                * addr &= ~(0X1 << offset);
                /* 或上颜色 */
                * addr |= temp;
                /* 下一行 */
                bit = bit + line_bit;

                -- size;
            }
        }
        else if(s->color_depth == 2)
        {
            while(size)
            {
                offset = bit & 7;
                addr = video + (bit >> 3);
                /* 获得颜色 */
                temp = color;
                /* 调整颜色 */
                temp = (temp & 0X3) << offset;
                /* 将相关位设为 0 */
                * addr &= ~(0X3 << offset);
                /* 或上颜色 */
                * addr |= temp;
                /* 下一行 */
                bit = bit + line_bit;

                -- size;
            }
        }
        else if(s->color_depth == 4)
        {
            while(size)
            {
                offset = bit & 7;
                addr = video + (bit >> 3);
                /* 获得颜色 */
                temp = color;
                /* 调整颜色 */
                temp = (temp & 0XF) << offset;
                /* 将相关位设为 0 */
                * addr &= ~(0XF << offset);
                /* 或上颜色 */
                * addr |= temp;
                /* 下一行 */
                bit = bit + line_bit;

                -- size;
            }
        }
    }
    /* 颜色深度大于等于 8 */
    else
    {
        /* 像素对应的字节数 */
        pixel = s->color_depth >> 3;
        /* 获得起始像素相对于显存的偏移量 */
        offset = bit >> 3;
        /* 屏幕每行对应的字节数 */
        n = line_bit >> 3;

        /* 将各个像素依次写入 */
        while(size)
        {
            /* 将颜色写入视频缓冲区 */
            memcpy(video + offset, &color, pixel);
            /* 偏移量增加 */
            offset += n;
            -- size;
        }
    }

    return 0;
}
