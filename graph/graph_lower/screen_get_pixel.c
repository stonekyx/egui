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
    ui_t offset, color;
    byte_t * video;
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

    /* 点不在工作区域与屏幕区域的交集内 */
    if
    ((x < result_area.x) ||
     (x > result_area.x + result_area.width - 1) ||
     (y < result_area.y) ||
     (y > result_area.y + result_area.height - 1))
    {
        return -1;
    }

    /* 获得像素关于显存的偏移量 */
    offset = (y * s->width + x) * (s->color_depth >> 3);

    /* 将像素的值赋给 color */
    memcpy(&color, video + offset, s->color_depth >> 3);

    /* 将 color 转换成 struct color */
    screen_value_to_color(c, &color);

    return 0;
}
