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

static byte_t *
screen_get_raw_video(struct screen *s)
{
    if(s->video_access_mode == VIDEO_ACCESS_MODE_DIRECT)
    {
        return s->memory_addr;
    }
    else if(s->video_access_mode == VIDEO_ACCESS_MODE_BUFFER)
    {
        return s->buffer_addr;
    }
    else
    {
        return NULL;
    }
}

void
screen_get_pixel_raw_nocheck
(void * video,
 si_t depth,
 struct color *c,
 si_t x,
 si_t y)
{
    ui_t offset, color;
    /* 获得像素关于显存的偏移量 */
    offset = y * global_fix_screen_info.line_length + x * (depth >> 3);

    /* 将像素的值赋给 color */
    memcpy(&color, (byte_t*)video + offset, depth >> 3);

    /* 将 color 转换成 struct color */
    screen_value_to_color(c, &color);
}

/*
 * 从裸数组中获取像素。
 */
si_t
screen_get_pixel_raw
(void * video,
 si_t width,
 si_t height,
 si_t depth,
 struct rectangle * a,
 struct color * c,
 si_t x,
 si_t y)
{
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

    screen_get_pixel_raw_nocheck(video, depth, c, x, y);

    return 0;
}

si_t
screen_get_pixel_nocheck
(struct screen *s,
 struct color *c,
 si_t x,
 si_t y)
{
    byte_t * video = screen_get_raw_video(s);
    if(!video) {
        return -1;
    }
    screen_get_pixel_raw_nocheck(video,
            s->color_depth, c, x, y);
    return 0;
}

/*
  get picture element

  获得像素的值
*/
si_t
screen_get_pixel
(struct screen * s,
 struct rectangle * a,
 struct color * c,
 si_t x,
 si_t y)
{
    byte_t * video = screen_get_raw_video(s);
    if(!video) {
        return -1;
    }
    return screen_get_pixel_raw(video, s->width, s->height, s->color_depth,
            a, c, x, y);
}
