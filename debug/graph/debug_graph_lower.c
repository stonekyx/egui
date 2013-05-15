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
 * along with EDUGUI; if not, write to the Free Software * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * All rights reserved.
**/

# include <stdio.h>
# include <string.h>
# include <unistd.h>

# include <graph.h>

# include "debug_graph_lower.h"

si_t debug_graph_lower()
{
    struct color c;
    struct screen s;
    struct rectangle a;

    /* 设置区域结构 */
    a.x = 0;
    a.y = 0;
    a.width = 101;
    a.height = 101;

    /* 获得屏幕资源 */
    memcpy(&s, &global_screen, sizeof(struct screen));

    /* 获得颜色资源 */
    screen_color_limit();

    printf("width      %ld\n", s.width);
    printf("height      %ld\n", s.height);
    printf("video_space %ld\n", s.size);
    printf("color_depth %ld\n", s.color_depth);

    printf("R 0 - %lX\n", global_color_limit.r_limit);
    printf("G 0 - %lX\n", global_color_limit.g_limit);
    printf("B 0 - %lX\n", global_color_limit.b_limit);
    printf("A 0 - %lX\n", global_color_limit.a_limit);

    c.r = global_color_limit.r_limit;
    c.g = 0;
    c.b = 0;

    /* 将屏幕清空成红色 */
    screen_clear(&s, &c);
    c.r = 127;
    c.g = 127;
    c.b = 127;

    screen_set_pixel(&s, &a, &c, -100, -100);
    screen_cpy_area
    (s.memory_addr,
     s.buffer_addr,
     0,
     0,
     0,
     0,
     s.width,
     s.height);
    sleep(1);

    screen_set_h_line(&s, &a, &c, -100, 50, 200, 50);
    screen_cpy_area
    (s.memory_addr,
     s.buffer_addr,
     0,
     0,
     0,
     0,
     s.width,
     s.height);
    sleep(1);

    screen_set_v_line(&s, &a, &c, 50, -100, 50, 200);
    screen_cpy_area
    (s.memory_addr,
     s.buffer_addr,
     0,
     0,
     0,
     0,
     s.width,
     s.height);
    sleep(1);

    screen_set_x_line(&s, &a, &c, -100, -100, 200, 200);
    screen_cpy_area
    (s.memory_addr,
     s.buffer_addr,
     0,
     0,
     0,
     0,
     s.width,
     s.height);
    sleep(1);

    screen_set_x_line(&s, &a, &c, -100, 200, 200, -100);
    screen_cpy_area
    (s.memory_addr,
     s.buffer_addr,
     0,
     0,
     0,
     0,
     s.width,
     s.height);
    sleep(1);

    screen_set_area(&s, &a, &c, -100, -100, 300, 300);
    screen_cpy_area
    (s.memory_addr,
     s.buffer_addr,
     0,
     0,
     0,
     0,
     s.width,
     s.height);
    sleep(1);

    /* 将屏幕清空成黑色 */
    screen_clear_black(&s);
    screen_cpy_area
    (s.memory_addr,
     s.buffer_addr,
     16,
     16,
     16,
     16,
     s.width - 16,
     s.height - 16);
    sleep(1);

    a.x = 0;
    a.y = 0;
    a.width = s.width;
    a.height = s.height;

    c.r = global_color_limit.r_limit;
    c.g = global_color_limit.g_limit;
    c.b = global_color_limit.b_limit;

    screen_set_area(&s, &a, &c, 100, 668, 200, 200);
    c.b = 0;
    screen_set_area(&s, &a, &c, -100, 200, 200, 200);
    screen_cpy_area
    (s.buffer_addr,
     s.buffer_addr,
     -100,
     200,
     100,
     668,
     200,
     200);
    screen_flush(0, 0, s.width, s.height);
    sleep(1);

    /* 复制有重叠的区域 */
    c.r = 0;
    c.g = global_color_limit.g_limit;
    c.b = 0;

    screen_set_area(&s, &a, &c, 0, 0, 300, 300);

    c.r = global_color_limit.r_limit;
    c.g = global_color_limit.g_limit;
    c.b = 0;

    screen_set_area(&s, &a, &c, 300, 0, 300, 300);
    screen_flush(0, 0, s.width, s.height);
    sleep(1);

    screen_cpy_area
    (s.memory_addr,
     s.memory_addr,
     300,
     100,
     200,
     0,
     200,
     200);

    sleep(1);

    printf("debuged graph_lower\n");

    return 0;
}

