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

/* 填充一个任意多边形。使用了自行设计的拙劣算法，虽然效率应该比较高，
 * 但有些细节可能会出错。更加规范的实现可以参考SVG的even-odd rule。
 *
 * 这里用到的算法基本思路如下：
 * 整个过程类似于在纸上画图，先画出线稿，然后在中间涂色。
 * 关于凹多边形的考虑是，利用计算几何中矢量叉积计算多边形面积的原理，
 * 将多边形按边的顺序分成若干三角形，并重复地在01矩阵上绘制三角形，
 * 每次绘制时，如果某个点之前已经是1，那么就将其反转。最终的01矩阵中
 * 值为1的点就是应该绘制的点，然后只需要调用screen_set_pixel即可。
 *
 * 直接实现上面这个想法的话，效率还是比较低，因为很多三角形会被重复
 * 覆盖。有个办法可以将这个过程简化为对三角形三条边的绘制。
 *
 * 这里又用到前面提到的even-odd rule里一个基本原理：一个点向无穷远处
 * 作射线，中途跨过的线的条数如果是奇数，说明这个点在多边形内部，
 * 反之在外部。不过我们不是在整个多边形上应用这一原则，而是只在绘制
 * 三角形这个子过程中用一下：与线相交的条数，在离散的矩阵上就可以
 * 简化为周围的点数（特别是，我们只在一行上考虑，也就是射线只能向
 * 左右两边发射）；而既然这个点数就能代表一个点是否在内部这一信息，
 * 我们就没有必要再特地去绘制它了。只要标记出三角形周长上的点即可。
 * 同样，由于有很多三角形要画，如果一个点已经是1，那么将它反转为0。
 * 仔细思考后，发现这样好像确实可以正确表示每一行的状态。
 *
 * 用这种方式标记完所有三角形的边以后，可以一次性将所有三角形重叠后
 * 的状态描绘出来，这就是第二次涂色的过程。涂色完成以后，算法就
 * 基本上结束了。最后再枚举一下01矩阵，在屏幕上真实绘制。
 */

/* 获得屏幕上有效的绘制区域，超出屏幕外的部分将被后面的代码忽略 */
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
    /* check_area是将所有顶点包围起来的最小矩形 */
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
        if(check_area.width < p[i].x-check_area.x+1) {
            check_area.width = p[i].x-check_area.x+1;
        }
        if(check_area.height < p[i].y-check_area.y+1) {
            check_area.height = p[i].y-check_area.y+1;
        }
    }

    /* ------- check_area is ready ------- */

    /* 跟gd中屏幕的区域和工作区域分别相交，求出公共部分 */
    draw_area.width = gd->screen.width;
    draw_area.height = gd->screen.height;
    area_intersection(&draw_area, &check_area, &draw_area);
    area_intersection(&draw_area, &gd->rectangle, &draw_area);

    return &draw_area;
}

/* 分配map数组的空间。当重复调用fill_polygon时，不会分配新空间，而是
 * 将原来的空间进行适当扩充，并清空后重复使用。 */
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

/* 辅助函数，将map中代表坐标(x,y)的位反转 */
static void map_toggle_bit(addr_t map, si_t width, si_t x, si_t y)
{
    ui_t flat_pixel = x + y*width;
    ((byte_t*)map)[flat_pixel/CHAR_BIT] ^= (1u<<(flat_pixel%CHAR_BIT));
}

/* 辅助函数，获取map中代表(x,y)的位 */
static byte_t map_get_bit(addr_t map, si_t width, si_t x, si_t y)
{
    ui_t flat_pixel = x + y*width;
    return ((byte_t*)map)[flat_pixel/CHAR_BIT] & (1u<<(flat_pixel%CHAR_BIT));
}

/* 将map中代表(x,y)的位置为1 */
static void map_set_bit(addr_t map, si_t width, si_t x, si_t y)
{
    ui_t flat_pixel = x + y*width;
    ((byte_t*)map)[flat_pixel/CHAR_BIT] |= (1u<<(flat_pixel%CHAR_BIT));
}

/* 在row_start和row_end数组中标记点dp */
static void mark_point(si_t row_start[], si_t row_end[], struct rectangle *a,
        struct point *dp)
{
    if(dp->y<0 || dp->y>a->height) { /* not affective to the row. */
        return;
    }
    if(dp->x < 0) {
        dp->x = 0;
    }
    if(dp->x >= a->width) {
        /* 如果点在绘制区域右边的外面，需要在边界上标记终点，
         * 否则row_end可能保持-1，不能判断该行是否应该绘制 */
        dp->x = a->width-1;
    }
    /* 标记行起点和行终点 */
    if(row_start[dp->y] == -1 || row_start[dp->y] > dp->x) {
        row_start[dp->y] = dp->x;
    }
    if(row_end[dp->y] == -1 || row_end[dp->y] < dp->x) {
        row_end[dp->y] = dp->x;
    }
}

/* 在row_start和row_end中标记st和en之间的每个点 */
static void mark_line(si_t row_start[], si_t row_end[], struct rectangle *a,
        const struct point *st, const struct point *en)
{
    struct point *pos;
    struct point last;
    struct bresenham_iterator *it;
    if(st->x > en->x || (st->x == en->x && st->y > en->y)) {
        const struct point *tmp = st;
        st = en;
        en = tmp;
    }
    /* 用bresenham算法枚举线上每个点，并调用上面的mark_line进行标记。
     * 由于需要使用矩形a的内部相对坐标，所以用真实坐标减掉a的左上角。
     * last的使用可能看起来奇怪，应该是起初为了应对别的某种情况，
     * 而那种情况后来又消失了，代码没有改过来。 */
    bresenham_for_each(pos, it, st->x, st->y, en->x, en->y) {
        struct point dp;
        if((st->x == pos->x && st->y == pos->y) ||
                last.y == pos->y) {
            last = *pos;
            continue;
        }
        dp.x = last.x - a->x;
        dp.y = last.y - a->y;
        last = *pos;
        mark_point(row_start, row_end, a, &dp);
    }
    last.x -= a->x;
    last.y -= a->y;
    mark_point(row_start, row_end, a, &last);
}

/* 按照已完成的map线稿进行涂色。此后map中值为1的位表示应该绘制，
 * 反之不应该绘制。 */
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
                    /* 每行的右边界不绘制。
                     * TODO: 考虑右边界是屏幕边界的情况 */
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

/* 再遍历一次所有的边，并将它们在map里填充一次。
 * 因为之前的涂色过程忽略了一些边界。 */
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

/* 绘制map的主过程 */
static addr_t prepare_map(struct rectangle *area, struct point p[], ui_t pcnt)
{
    ui_t i;
    size_t map_size = (area->width * area->height + CHAR_BIT-1) / CHAR_BIT;
    size_t row_size = sizeof(si_t) * area->width;
    /* 分配空间 */
    addr_t map = get_map_space(map_size);
    si_t *row_start = malloc(row_size);
    si_t *row_end = malloc(row_size);
    for(i=1; i<pcnt-1; i++) {
        struct point *base = p+0,
                     *a = p+i,
                     *b = p+(i+1);
        si_t j;
        /* row_start和row_end每次只记录一个三角形的数据，使用前清空 */
        memset(row_start, -1, row_size);
        memset(row_end, -1, row_size);
        /* 三角形填充 */
        mark_line(row_start, row_end, area, base, a);
        mark_line(row_start, row_end, area, a, b);
        mark_line(row_start, row_end, area, b, base);
        /* 将row_start和row_end中的三角形填充数据写到map里。
         * 没错，应该是可以跳过row_start和row_end，直接往map里写。
         * 但没这么实现，应该是由于编码时考虑不完整，把步骤分得太清。*/
        for(j=0; j<area->height; j++) {
            if(row_start[j]>row_end[j]) {
                continue;
            }
            if(row_start[j]==-1 || row_end[j]==-1) {
                continue;
            }
            map_toggle_bit(map, area->width, row_start[j], j);
            if(row_end[j] < area->width) {
                map_toggle_bit(map, area->width, row_end[j], j);
            }
        }
    }
    free(row_start);
    free(row_end);
    /* 综合map里所有三角形填充的数据，进行第二次涂色 */
    fill_map(map, area);
    /* 补完边界 */
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
