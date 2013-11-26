/**
 * @file engine_fill_polygon.c
 *
 * @author Dalian University of Technology
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 Dalian University of Technology
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

#include "../graph_engine.h"

static struct rectangle *
get_draw_area
(si_t graphics_device_handle,
 struct point *p,
 ui_t pcnt)
{
    struct rectangle check_area = { 0 };
    static struct rectangle draw_area;
    struct graphics_device *gd = (addr_t)graphics_device_handle;
    ui_t i;

    memset(&draw_area, 0, sizeof(draw_area));
    if(pcnt<=0) {
        return &draw_area;
    }
    check_area.x = p[0].x;
    check_area.y = p[0].y;
    for(i=1; i<pcnt; i++) {
        if(check_area.x > p[i].x) {
            check_area.x = p[i].x;
        }
        if(check_area.y > p[i].y) {
            check_area.y = p[i].y;
        }
    }
    for(i=0; i<pcnt; i++) {
        if(check_area.width < p[i].x-check_area.x) {
            check_area.width = p[i].x-check_area.x;
        }
        if(check_area.height < p[i].y-check_area.y) {
            check_area.height = p[i].y-check_area.y;
        }
    }

    /* ------- check_area is ready ------- */

    draw_area.width = gd->screen.width;
    draw_area.height = gd->screen.height;
    area_intersection(&draw_area, &check_area, &draw_area);
    area_intersection(&draw_area, &gd->rectangle, &draw_area);

    return &draw_area;
}

static addr_t get_map_space(size_t bytes)
{
    static size_t allocated_bytes = 0;
    static addr_t buffer = NULL;
    if(allocated_bytes<bytes) {
        buffer = realloc(buffer, bytes);
        allocated_bytes = bytes;
    }
    if(buffer) {
        memset(buffer, 0, bytes);
    }
    return buffer;
}

static void map_toggle_bit(addr_t map, si_t width, si_t x, si_t y)
{
    ui_t flat_pixel = x + y*width;
    ((byte_t*)map)[flat_pixel/CHAR_BIT] ^= (1u<<(flat_pixel%CHAR_BIT));
}

static int map_get_bit(addr_t map, si_t width, si_t x, si_t y)
{
    ui_t flat_pixel = x + y*width;
    return ((byte_t*)map)[flat_pixel/CHAR_BIT] & (1u<<(flat_pixel%CHAR_BIT));
}

static void map_set_bit(addr_t map, si_t width, si_t x, si_t y)
{
    ui_t flat_pixel = x + y*width;
    ((byte_t*)map)[flat_pixel/CHAR_BIT] |= (1u<<(flat_pixel%CHAR_BIT));
}

static void mark_line(si_t row_start[], si_t row_end[], struct rectangle *a,
        const struct point *st, const struct point *en)
{
    struct point *pos;
    struct bresenham_iterator *it;
    bresenham_for_each(pos, it, st->x, st->y, en->x, en->y) {
        si_t dx = pos->x - a->x;
        si_t dy = pos->y - a->y;
        if(dy<0 || dy>a->height) { /* not affective to the row. */
            continue;
        }
        if(dx < 0) {
            dx = 0;
        }
        if(dx >= a->width) {
            dx = a->width-1;
        }
        if(row_start[dy] == -1 || row_start[dy] > dx) {
            row_start[dy] = dx;
        }
        if(row_end[dy] == -1 || row_end[dy] < dx) {
            row_end[dy] = dx;
        }
    }
}

static void fill_map(addr_t map, struct rectangle *area)
{
    si_t j;
    for(j=0; j<area->height; j++) {
        si_t i;
        int state = 0;
        for(i=0; i<area->width; i++) {
            if(map_get_bit(map, area->width, i, j)) {
                state = ~state;
                if(!state) {
                    map_toggle_bit(map, area->width, i, j);
                }
            } else {
                if(state) {
                    map_toggle_bit(map, area->width, i, j);
                }
            }
        }
    }
}

static void set_border(addr_t map, struct rectangle *area, struct point p[], ui_t pcnt)
{
    ui_t i;
    for(i=0; i<pcnt; i++) {
        ui_t next = i==pcnt-1 ? 0 : (i+1);
        struct point *pos;
        struct bresenham_iterator *it;
        bresenham_for_each(pos, it, p[i].x, p[i].y, p[next].x, p[next].y) {
            si_t x = pos->x - area->x;
            si_t y = pos->y - area->y;
            if(x<0 || y<0 || x>=area->width || y>=area->height) {
                continue;
            }
            map_set_bit(map, area->width, x, y);
        }
    }
}

static addr_t prepare_map(struct rectangle *area, struct point p[], ui_t pcnt)
{
    ui_t i;
    size_t map_size = (area->width * area->height + CHAR_BIT-1) / CHAR_BIT;
    size_t row_size = sizeof(si_t) * area->width;
    addr_t map = get_map_space(map_size);
    si_t *row_start = malloc(row_size);
    si_t *row_end = malloc(row_size);
    for(i=1; i<pcnt-1; i++) {
        struct point *base = p+0,
                     *a = p+i,
                     *b = p+(i+1);
        si_t j;
        memset(row_start, -1, row_size);
        memset(row_end, -1, row_size);
        mark_line(row_start, row_end, area, base, a);
        mark_line(row_start, row_end, area, a, b);
        mark_line(row_start, row_end, area, b, base);
        for(j=0; j<area->height; j++) {
            map_toggle_bit(map, area->width, row_start[j], j);
            if(row_end[j]+1 < area->width) {
                map_toggle_bit(map, area->width, row_end[j]+1, j);
            }
        }
    }
    free(row_start);
    free(row_end);
    fill_map(map, area);
    set_border(map, area, p, pcnt);
    return map;
}

si_t
engine_fill_polygon
(si_t graphics_device_handle,
 struct point *p,
 ui_t pcnt)
{
    struct rectangle *area;
    struct graphics_device *gd = (addr_t)graphics_device_handle;
    addr_t map;
    si_t i;

    if(pcnt < 3) {
        return 1;
    }
    area = get_draw_area(graphics_device_handle, p, pcnt);
    map = prepare_map(area, p, pcnt);
    for(i=area->x; i < area->x+area->width; i++) {
        si_t j;
        for(j=area->y; j < area->y+area->height; j++) {
            if(map_get_bit(map, area->width, i-area->x, j-area->y)) {
                screen_set_pixel_nocheck(&gd->screen, &gd->color, i, j);
            }
        }
    }
    return 0;
}
