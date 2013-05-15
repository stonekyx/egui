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

# include <stdio.h>
# include <string.h>
# include <unistd.h>

# include <graph.h>

# include "debug_graph_engine.h"

static struct graphics_device gd;
static si_t gd_handler = (si_t)&gd;
static struct color_limit cl;
static struct point p[5];

static si_t debug_draw()
{
    si_t i;

    /* 设置颜色 黄色 */
    gd.color.r = cl.r_limit;
    gd.color.g = cl.g_limit;
    gd.color.b = 0;

    /* 设置点 */
    p[0].x = 0;
    p[0].y = 0;
    p[1].x = 500;
    p[1].y = 100;
    p[2].x = 200;
    p[2].y = 300;
    p[3].x = 0;
    p[3].y = 250;

    /* 绘制 s.width 条直线 */
    i = 0;
    while(i < gd.screen.width)
    {
        engine_draw_line(gd_handler, i, 0, gd.screen.width - 1 - i, gd.screen.height - 1);
        engine_update(gd_handler);
        ++ i;
    }

    /* 绘制 s.height 条直线 */
    i = 0;
    while(i < gd.screen.height)
    {
        engine_draw_line(gd_handler, 0, i, gd.screen.width - 1, gd.screen.height - 1 - i);
        engine_update(gd_handler);
        ++ i;
    }

    /* 设置颜色 蓝色 */
    gd.color.r = 0;
    gd.color.g = 0;
    gd.color.b = cl.b_limit;

    /* 将屏幕清空成黑色 */
    engine_clear_black(gd_handler);
    /* 绘制多变形 */
    engine_draw_polygon(gd_handler, p, 3);
    engine_update(gd_handler);
    sleep(1);

    /* 将屏幕清空成黑色 */
    engine_clear_black(gd_handler);
    /* 绘制多变形 */
    engine_draw_polygon(gd_handler, p, 4);
    engine_update(gd_handler);
    sleep(1);

    /* 将屏幕清空成黑色 */
    engine_clear_black(gd_handler);

    /* 绘制圆 */
    i = 0;
    while(i ++ < gd.screen.height)
    {
        engine_draw_circle(gd_handler, gd.screen.width >> 1, gd.screen.height >> 1, i >> 1);
        engine_update(gd_handler);
        engine_clear_black(gd_handler);
    }

    /* 绘制椭圆 */
    /* a > b */

    i = 0;
    while(i ++ < (gd.screen.height - 1) >> 1)
    {
        engine_draw_ellipse(gd_handler, (gd.screen.width - 1) >> 1, (gd.screen.height - 1) >> 1, (gd.screen.width - 1) >> 1, i);
        engine_update(gd_handler);
        engine_clear_black(gd_handler);
    }

    /* 绘制椭圆 */
    /* a < b */
    i = 0;
    while(i ++ < (gd.screen.width - 1) >> 1)
    {
        engine_draw_ellipse(gd_handler, (gd.screen.width - 1) >> 1, (gd.screen.height - 1) >> 1, i, (gd.screen.height - 1) >> 1);
        engine_update(gd_handler);
        engine_clear_black(gd_handler);
    }

    /* 绘制 x */
    engine_draw_x(gd_handler, 640, 383, 191, 191);
    engine_update(gd_handler);
    sleep(1);

    /* 填充 x */
    engine_fill_x(gd_handler, 640, 383, 191, 191);
    engine_update(gd_handler);
    sleep(1);
    engine_clear_black(gd_handler);

    /* 绘制箭头 */
    engine_draw_arrow(gd_handler, 300, 200, 100, 1);
    engine_draw_arrow(gd_handler, 300, 400, 100, 2);
    engine_draw_arrow(gd_handler, 200, 300, 100, 3);
    engine_draw_arrow(gd_handler, 400, 300, 100, 4);
    engine_update(gd_handler);
    sleep(1);

    /* 填充箭头 */
    engine_fill_arrow(gd_handler, 300, 200, 100, 1);
    engine_fill_arrow(gd_handler, 300, 400, 100, 2);
    engine_fill_arrow(gd_handler, 200, 300, 100, 3);
    engine_fill_arrow(gd_handler, 400, 300, 100, 4);
    engine_update(gd_handler);
    sleep(1);

    return 0;
}

static si_t debug_fill()
{
    gd.color.r = 0;
    gd.color.g = cl.g_limit;
    gd.color.b = 0;

    /* 绘制矩形 */
    engine_fill_rectangle(gd_handler, 100, 100, 401, 201);
    engine_update(gd_handler);
    sleep(1);
    engine_clear_black(gd_handler);

    p[0].x = 651;
    p[0].y = 200;
    p[1].x = 601;
    p[1].y = 250;
    p[2].x = 701;
    p[2].y = 250;

    gd.color.r = 0;
    gd.color.g = cl.g_limit;
    gd.color.b = 0;
    /* 填充圆 */
    engine_fill_circle(gd_handler, gd.screen.width >> 1, gd.screen.height >> 1, gd.screen.height >> 2);
    engine_update(gd_handler);
    sleep(1);
    engine_clear_black(gd_handler);

    gd.color.a = cl.a_limit;
    gd.color.r = cl.r_limit;

    /* 填充圆 */
    engine_fill_circle(gd_handler, gd.screen.width >> 1, gd.screen.height >> 1, gd.screen.height >> 1);
    engine_update(gd_handler);
    sleep(1);
    engine_clear_black(gd_handler);

    /* 填充椭圆 */
    engine_fill_ellipse(gd_handler, gd.screen.width >> 1, gd.screen.height >> 1, gd.screen.width >> 2, gd.screen.height >> 2);
    engine_update(gd_handler);
    sleep(1);
    engine_clear_black(gd_handler);

    /* 填充椭圆 */
    engine_fill_ellipse(gd_handler, gd.screen.width >> 1, gd.screen.height >> 1, gd.screen.height >> 2, gd.screen.width >> 2);
    engine_update(gd_handler);
    sleep(1);

    return 0;
}

static si_t debug_text()
{
    si_t i, temp;

    gd.color.r = cl.r_limit;
    gd.color.g = 0;
    gd.color.b = 0;
    gd.color.a = 0;

    temp = 0;
    for(i = FONT_MATRIX_08; i < FONT_QUANTITY - 1; ++ i)
    {
        gd.font = i;
        engine_show_text(gd_handler, 0, temp, "FONT_MATRIX", 11);
        temp = temp + engine_get_font_height(gd_handler);
    }

    gd.font = FONT_MATRIX_08;
    engine_show_text(gd_handler, 0, gd.screen.height - engine_get_font_height(gd_handler), "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", 96);
    engine_update(gd_handler);
    sleep(1);

    return 0;
}

si_t debug_graph_engine()
{
    /* 获得屏幕资源 */
    memcpy(&(gd.screen), &global_screen, sizeof(struct screen));
    engine_get_color_limit(&cl);

    gd.rectangle.x = 0;
    gd.rectangle.y = 0;
    gd.rectangle.width = global_screen.width;
    gd.rectangle.height = global_screen.height;

    debug_draw();
    engine_clear_black(gd_handler);
    debug_fill();
    engine_clear_black(gd_handler);
    debug_text();

    printf("debuged graph_engine\n");

    return 0;
}

