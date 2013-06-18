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

# include <data_structures.h>

# include "../graph_lower.h"

si_t
screen_set_pixel_raw
(void * video,
 si_t width,
 si_t height,
 si_t depth,
 struct rectangle * a,
 struct color * c,
 si_t x,
 si_t y)
{
    ui_t offset, color, bit;
    byte_t * addr;
    struct rectangle result_area, screen_area;

    screen_area.x = 0;
    screen_area.y = 0;
    screen_area.width = width;
    screen_area.height = height;

    /* 工作区域不在屏幕区域 */
    if(area_intersection(a, &screen_area, &result_area) == -1)
    {
        return -1;
    }

    /* 点不在工作区域与屏幕区域的交集内 */
    if
    ((x < result_area.x) ||
     (x > result_area.x + result_area.width - 1) ||
     (y < result_area.y) ||
     (y > result_area.y + result_area.height - 1))
    {
        return -1;
    }

    /* 获得像素关于显存的位偏移量 */
    bit = (y * width + x) * depth;

    if(global_fix_screen_info.visual == FB_VISUAL_MONO01)
    {
        /* 白色 */
        if((c->r == global_color_limit.r_limit) && (c->g == global_color_limit.g_limit) && (c->b == global_color_limit.b_limit))
        {
            color = 0;
        }
        /* 黑色 */
        else if
        ((c->r == 0) &&
         (c->g == 0) &&
         (c->b == 0))
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
         (c->b == global_color_limit.b_limit))
        {
            color = 1;
        }
        /* 黑色 */
        else if
        ((c->r == 0) &&
         (c->g == 0) &&
         (c->b == 0))
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
        if(depth>=32) {
            /* if alpha channel is available, try alpha blending. */
            static struct color old;
            if(screen_get_pixel_raw(video, width, height, depth, a, &old, x, y)>=0) {
                old = *alpha_blend(c, &old);
                c = &old;
            }
        }
        /* 获得颜色的值 */
        screen_color_to_value(&color, c);
    }

    if(depth < 8)
    {
        /* 开始位在字节内的偏移量 */
        offset = bit & 7;
        /* 字节地址 */
        addr = video + (bit >> 3);

        if(depth == 1)
        {
            /* 调整颜色 */
            color = (color & 0X1) << offset;
            /* 将相关位设为 0 */
            * addr &= ~(0X1 << offset);
            /* 或上颜色 */
            * addr |= color;
        }
        else if(depth == 2)
        {
            /* 调整颜色 */
            color = (color & 0X3) << offset;
            /* 将相关位设为 0 */
            * addr &= ~(0X3 << offset);
            /* 或上颜色 */
            * addr |= color;
        }
        else if(depth == 4)
        {
            /* 调整颜色 */
            color = (color & 0XF) << offset;
            /* 将相关位设为 0 */
            * addr &= ~(0XF << offset);
            /* 或上颜色 */
            * addr |= color;
        }
    }
    else
    {
        /* 获得像素关于显存的字节偏移量 */
        offset = bit >> 3;

        /* 将颜色写入视频缓冲区 */
        memcpy((void *)(video + offset), (void *)(&color), depth >> 3);
    }

    return 0;
}

/*
    picture element
*/
si_t
screen_set_pixel
(struct screen * s,
 struct rectangle * a,
 struct color * c,
 si_t x,
 si_t y)
{
    byte_t * video;

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

    return screen_set_pixel_raw(video, s->width, s->height, s->color_depth,
            a, c, x, y);
}
