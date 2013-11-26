/**
 * @file geometry.c
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

# include "geometry.h"
# include <stdlib.h>

void point_set(struct point* p, si_t x, si_t y)
{
	p->x = x;
	p->y = y;
}

void rectangle_set(struct rectangle* r, si_t x, si_t y, si_t width, si_t height)
{
	r->x = x;
	r->y = y;
	r->width = width;
	r->height = height;
}

void rectangle_move(struct rectangle* r, si_t x_offset, si_t y_offset)
{
	r->x += x_offset;
	r->y += y_offset;
}

si_t
area_union
(struct rectangle * p,
 struct rectangle * q,
 struct rectangle * result)
{
    si_t pb, pr, qb, qr;

    if(p->width == 0 || p->height == 0)
    {
        result->x = q->x;
        result->y = q->y;
        result->width = q->width;
        result->height = q->height;

        return -1;
    }

    if(q->width == 0 || q->height == 0)
    {
        result->x = p->x;
        result->y = p->y;
        result->width = p->width;
        result->height = p->height;

        return -1;

    }

    /* 获得下边届 */
    pb = p->y + p->height - 1;
    /* 获得右边界 */
    pr = p->x + p->width - 1;
    /* 获得下边界 */
    qb = q->y + q->height - 1;
    /* 获得右边界 */
    qr = q->x + q->width - 1;

    result->x = p->x < q->x ? p->x : q->x;
    result->y = p->y < q->y ? p->y : q->y;
    result->width = pr < qr ? qr - result->x + 1 : pr - result->x + 1;
    result->height = pb < qb ? qb - result->y + 1 : pb - result->y + 1;

    return 0;
}

si_t
area_intersection_old
(struct rectangle * parent,
 struct rectangle * child,
 struct rectangle * result)
{
    si_t cx, cy, cw, ch, pw, ph;

    cx = child->x;
	cy = child->y;
	cw = child->width;
	ch = child->height;

	pw = parent->width;
	ph = parent->height;
       
	/* 没有交集 */
    if(cx > pw || cy > ph)
    {
        result->x = 0;
        result->y = 0;
        result->width = 0;
        result->height = 0;

        return -1;
    }
    /* 有交集 */
    else
    {
        result->x = cx;
		result->y = cy; 
        result->width = cw; 
        result->height = cy;

		if(cw + cx > pw)
		{
			result->width = pw - cx;
		}
		if(ch + cy > ph)
		{
			result->height = ph - cy;
		}

        return 0;
    }
}

si_t
area_intersection
(struct rectangle * p,
 struct rectangle * q,
 struct rectangle * result)
{
    si_t pb, pr, qb, qr;

    /* 获得下边届 */
    pb = p->y + p->height - 1;
    /* 获得右边界 */
    pr = p->x + p->width - 1;
    /* 获得下边界 */
    qb = q->y + q->height - 1;
    /* 获得右边界 */
    qr = q->x + q->width - 1;

    /* 没有交集 */
    if
    ((p->x > qr) ||
     (p->y > qb) ||
     (pb < q->y) ||
     (pr < q->x))
    {
        result->x = 0;
        result->y = 0;
        result->width = 0;
        result->height = 0;

        return -1;
    }
    /* 有交集 */
    else
    {
        result->x = p->x < q->x ? q->x : p->x;
        result->y = p->y < q->y ? q->y : p->y;
        result->width = pr < qr ? pr - result->x + 1 : qr - result->x + 1;
        result->height = pb < qb ? pb - result->y + 1 : qb - result->y + 1;

        return 0;
    }
}

si_t
area_difference
(struct rectangle * p,
 struct rectangle * q,
 struct rectangle * result)
{
    struct rectangle temp;

    /* 没有交集 */
    if(area_intersection(p, q, &temp) == -1)
    {
        result[0].x = 0;
        result[0].y = 0;
        result[0].width = 0;
        result[0].height = 0;

        result[1].x = 0;
        result[1].y = 0;
        result[1].width = 0;
        result[1].height = 0;

        result[2].x = 0;
        result[2].y = 0;
        result[2].width = 0;
        result[2].height = 0;

        result[3].x = 0;
        result[3].y = 0;
        result[3].width = 0;
        result[3].height = 0;

        return -1;
    }
    /* 有交集 */
    else
    {
        result[0].x = p->x;
        result[0].y = p->y;
        result[0].width = p->width;
        result[0].height = p->height - temp.height;

        result[1].x = p->x;
        result[1].y = temp.y;
        result[1].width = p->width - temp.width;
        result[1].height = temp.height;

        result[2].x = temp.x + temp.width;
        result[2].y = temp.y;
        result[2].width = p->width - temp.x - temp.width + p->x;
        result[2].height = temp.height;

        result[3].x = p->x;
        result[3].y = temp.y + temp.height;
        result[3].width = p->width;
        result[3].height = p->height - temp.y - temp.height + p->y;

        return 0;
    }
}

extern si_t is_point_in_area(struct point* p, struct rectangle* r)
{
	if(p->x >= r->x && p->y >= r->y && p->x <= r->x + r->width - 1 && p->y <= r->y + r->height - 1)
		return 1;
	else
		return 0;
}

extern void ensure_point_in_area(struct point* p, struct rectangle* r)
{
    if(p->x <= r->x) 
        p->x = r->x;
    else if(p->x >= r->x + r->width - 1)
        p->x = r->x + r->width - 1;
    if(p->y <= r->y) 
        p->y = r->y;
    else if(p->y >= r->y + r->height -1)
        p->y = r->y + r->height -1;
}

/* ----------- Bresenham's line algorithm ----------- */

struct bresenham_iterator {
    si_t a, b, delta, x, y, dx, dy, stepx, stepy, i;
    struct point ret;
};

extern struct bresenham_iterator *
bresenham_init(si_t x1, si_t y1, si_t x2, si_t y2)
{
    struct bresenham_iterator *it =
        calloc(1, sizeof(struct bresenham_iterator));

    it->dx = labs(x1 - x2);
    it->dy = labs(y1 - y2);

    if(x2 > x1) {
        it->stepx = 1;
    } else {
        it->stepx = -1;
    }

    if(y2 > y1) {
        it->stepy = 1;
    } else {
        it->stepy = -1;
    }

    it->x = x1;
    it->y = y1;

    /* -1 < 斜率 < +1 */
    if(it->dx > it->dy) {
        it->a = it->dy << 1;
        it->b = (it->dy - it->dx) << 1;
        it->delta = (it->dy << 1) - it->dx;
        it->i = 0;
    } else {/* 斜率 <= -1 或者 +1 <= 斜率 */
        it->a = it->dx << 1;
        it->b = (it->dx - it->dy) << 1;
        it->delta = (it->dx << 1) - it->dy;
        it->i = 0;
    }
    return it;
}

extern struct point *bresenham_next(struct bresenham_iterator *it)
{
    /* -1 < 斜率 < +1 */
    if(it->dx > it->dy) {
        if(it->i <= it->dx) {
            it->ret.x = it->x;
            it->ret.y = it->y;

            it->x += it->stepx;
            if(it->delta < 0) {
                it->delta += it->a;
            } else {
                it->delta += it->b;
                it->y += it->stepy;
            }

            it->i ++; /* simulate a one-time for loop. */
            return &it->ret;
        }
    } else {
        if(it->i <= it->dy) {
            it->ret.x = it->x;
            it->ret.y = it->y;

            it->y += it->stepy;
            if(it->delta < 0) {
                it->delta += it->a;
            } else {
                it->delta += it->b;
                it->x += it->stepx;
            }

            it->i ++;
            return &it->ret;
        }
    }
    free(it);
    return NULL;
}
