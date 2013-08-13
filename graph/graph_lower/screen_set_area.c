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

ui08_t
color_byte
(ui_t color_depth,
 ui_t color)
{
    ui08_t temp = 0;

    if(color_depth == 1)
    {
        if(color == 0)
        {
            temp = 0X00;
        }
        else if(color == 1)
        {
            temp = 0XFF;
        }
    }
    else if(color_depth == 2)
    {
        if(color == 0)
        {
            temp = 0X00;
        }
        else if(color == 1)
        {
            temp = 0X55;
        }
        else if(color == 2)
        {
            temp = 0XAA;
        }
        else if(color == 3)
        {
            temp = 0XFF;
        }
    }
    else if(color_depth == 4)
    {
        if(color == 0)
        {
            temp = 0X00;
        }
        else if(color == 1)
        {
            temp = 0X11;
        }
        else if(color == 2)
        {
            temp = 0X22;
        }
        else if(color == 3)
        {
            temp = 0X33;
        }
        else if(color == 4)
        {
            temp = 0X44;
        }
        else if(color == 5)
        {
            temp = 0X55;
        }
        else if(color == 6)
        {
            temp = 0X66;
        }
        else if(color == 7)
        {
            temp = 0X77;
        }
        else if(color == 8)
        {
            temp = 0X88;
        }
        else if(color == 9)
        {
            temp = 0X99;
        }
        else if(color == 10)
        {
            temp = 0XAA;
        }
        else if(color == 11)
        {
            temp = 0XBB;
        }
        else if(color == 12)
        {
            temp = 0XCC;
        }
        else if(color == 13)
        {
            temp = 0XDD;
        }
        else if(color == 14)
        {
            temp = 0XEE;
        }
        else if(color == 15)
        {
            temp = 0XFF;
        }
    }

    return temp;
}

si_t
screen_set_area
(struct screen * s,
 struct rectangle * a,
 struct color * c,
 si_t x,
 si_t y,
 si_t width,
 si_t height)
{
    ui_t offset, color, pixel, n, h_offset, t_offset, h_bit, bit, t_bit, byte_quantity, line_bit, size;
    byte_t * addr, * video;
    byte_t temp, mask;
    si_t i, j, t_border, b_border, l_border, r_border;
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

    /* 矩形不在工作区域与屏幕区域的交集内 */
    if
    ((x > result_area.x + result_area.width - 1) ||
     (x + width - 1 < result_area.x) ||
     (y > result_area.y + result_area.height - 1) ||
     (y + height - 1 < result_area.y))
    {
            return -1;
    }
    /* 矩形在工作区域与屏幕区域的交集内 */
    else
    {
        l_border = x < result_area.x ? result_area.x : x;
        t_border = y < result_area.y ? result_area.y : y;
        r_border =
        x + width - 1 < result_area.x + result_area.width - 1 ? x + width - 1 : result_area.x + result_area.width - 1;
        b_border =
        y + height - 1 < result_area.y + result_area.height - 1 ? y + height - 1 : result_area.y + result_area.height - 1;
        x = l_border;
        y = t_border;
        width = r_border - l_border + 1;
        height = b_border - t_border + 1;
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
            color = (1 << 8) - 1;
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
            color = (1 << 8) - 1;
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

    /* 获得起始像素相对于显存的位偏移量 */
    h_bit = (y * s->width + x) * s->color_depth;
    /* 水平线总共的位数 */
    bit = width * s->color_depth;
    /* 获得结束像素之后的像素相对于显存的位偏移量 */
    t_bit = h_bit + bit;
    /* 每行所占的位数 */
    line_bit = s->width * s->color_depth;
    /* 竖直线的长度 */
    size = height;

    if(s->color_depth < 8)
    {
        while(size)
        {
            /* 开始位在字节内的偏移量 */
            h_offset = h_bit & 7;
            /* 结束位之后的位在字节内的偏移量 */
            t_offset = t_bit & 7;
            /* 完整字节的数目 */
            byte_quantity = bit >> 3;
            /* 字节地址 */
            addr = video;

            if(h_offset + bit < 8)
            {
                /* 开始位所在的字节 */
                addr += (h_bit >> 3);
                /* 将所有相关的位设为 0 */
                /* 101 */
                mask = ~((((ui08_t)-1) << h_offset) & (((ui08_t)-1) >> (8 - t_offset)));
                * addr &= mask;
                /* 获得颜色 */
                temp = color_byte(s->color_depth, color);
                /* 调整颜色 */
                temp <<= h_offset;
                temp &= ((ui08_t)-1) >> (8 - t_offset);
                /* 最后或上颜色 */
                * addr |= temp;
            }
            else
            {
                /* 设置开始位置 */
                if(h_offset != 0)
                {
                    /* 开始位所在的字节 */
                    addr += (h_bit >> 3);
                    /* 首先将开始位以及之后的位设为 0 */
                    * addr &= ((ui08_t)-1) >> (8 - h_offset);
                    /* 获得颜色 */
                    temp = color_byte(s->color_depth, color);
                    /* 调整颜色 */
                    temp <<=  h_offset;
                    /* 最后或上颜色 */
                    * addr |= temp;

                    /* 第一个完整字节是下一个 */
                    addr += 1;
                    /* 重新计算完整字节的数目 */
                    byte_quantity = (bit - 8 + h_offset) >> 3;
                }

                /* 获得颜色 */
                temp = color_byte(s->color_depth, color);
                /* 设置完整的字节 */
                memset(addr, temp, byte_quantity);

                /* 设置结束位置 */
                if(t_offset != 0)
                {
                    /* 结束位所在的字节 */
                    addr += byte_quantity;
                    /* 首先将结束位以及之前的位设为 0 */
                    * addr &= ((ui08_t)-1) << t_offset;
                    /* 获得颜色 */
                    temp = color_byte(s->color_depth, color);
                    /* 调整颜色 */
                    temp >>= (8 - t_offset);
                    /* 然后或上颜色 */
                    * addr |= temp;
                }
            }

            h_bit += line_bit;
            t_bit += line_bit;

            -- size;
        }
    }
    else
    {
        /* 像素对应的字节数 */
        pixel = s->color_depth >> 3;
        /* 获得起始像素相对于显存的字节偏移量 */
        offset = h_bit >> 3;
        /* 屏幕每行对应的字节数 */
        n = line_bit >> 3;

        /* 将各个像素依次写入 */
        for(i = 0; i < height; ++ i)
        {
            size = offset;

            /* 写一行 */
            for(j = 0; j < width; ++ j)
            {
                /* 将颜色写入视频缓冲区 */
                memcpy(video + size, &color, pixel);

                size += pixel;
            }

            offset += n;
        }
    }

    return 0;
}
