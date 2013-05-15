# include <data_structures.h>

# include "../graph_engine.h"

# if 0

/*
    digital differential analyzer
*/
static si_t
__draw_line_with_dda__
(struct screen * s,
 struct rectangle * r,
 struct color * c,
 si_t x1,
 si_t y1,
 si_t x2,
 si_t y2)
{
    si_t i, step;
    f_t dx, dy, x, y;

    /* 竖直线 */
    if(x1 == x2)
    {
        return screen_set_v_line(s, r, c, x1, y1, x2, y2);
    }

    /* 水平线 */
    if(y1 == y2)
    {
        return screen_set_h_line(s, r, c, x1, y1, x2, y2);
    }

    /* 特殊的斜线 */
    if(ABSOLUTE_I(x1 - x2) == ABSOLUTE_I(y1 -y2))
    {
        return screen_set_x_line(s, r, c, x1, y1, x2, y2);
    }

    /* -1 < 斜率 < +1 */
    if(ABSOLUTE_I(x1 - x2) > ABSOLUTE_I(y1 -y2))
    {
        step = ABSOLUTE_I(x1 - x2);
    }
    /* 斜率 <= -1 或者 +1 <= 斜率 */
    else
    {
        step = ABSOLUTE_I(y1 - y2);
    }

    dx = (f_t)(x2 - x1) / (f_t)step;
    dy = (f_t)(y2 - y1) / (f_t)step;

    x = (f_t)x1;
    y = (f_t)y1;

    for(i = 0; i <= step; ++ i)
    {
        screen_set_pixel(s, r, c, ROUND(x), ROUND(y));
        x += dx;
        y += dy;
    }

    return 0;
}

/*
    middle points
*/
static si_t
__draw_line_with_mp__
(struct screen * s,
 struct rectangle * r,
 struct color * c,
 si_t x1,
 si_t y1,
 si_t x2,
 si_t y2)
{
    si_t upper_dd, lower_dd, A, B, delta, x, y;

    /* 竖直线 */
    if(x1 == x2)
    {
        return screen_set_v_line(s, r, c, x1, y1, x2, y2);
    }

    /* 水平线 */
    if(y1 == y2)
    {
        return screen_set_h_line(s, r, c, x1, y1, x2, y2);
    }

    /* 特殊的斜线 */
    if(ABSOLUTE_I(x1 - x2) == ABSOLUTE_I(y1 -y2))
    {
        return screen_set_x_line(s, r, c, x1, y1, x2, y2);
    }

    /*
    1. k > +1
        delta = f(x + 0.5, y + 1) - f(x, y) -> 2B + A
    2. 0 < k < +1
        delta = f(x + 1, y + 0.5) - f(x, y) -> 2A + B
    3. 0 > k > -1
        delta = f(x + 1, y - 0.5) - f(x, y) -> 2A - B
    4. k < -1
        delta = f(x - 0.5, y + 1) - f(x, y) -> 2B - A
    */

    /* -1 < 斜率 < +1 */
    if(ABSOLUTE_I(x2 - x1) > ABSOLUTE_I(y2 - y1))
    {
        /* x1 总是小于 x2 */
        if(x1 > x2)
        {
            x1 = x1 ^ x2;
            x2 = x2 ^ x1;
            x1 = x1 ^ x2;

            y1 = y1 ^ y2;
            y2 = y2 ^ y1;
            y1 = y1 ^ y2;
        }

        A = y1 - y2;
        B = x2 - x1;
        x = x1;
        y = y1;

        /* 0 < k < +1 */
        if((x2 - x1 > 0 && y2 - y1 > 0)||(x2 - x1 < 0 && y2 - y1 < 0))
        {
            delta = (A << 1) + B;
            upper_dd = A << 1;
            lower_dd = (A + B) << 1;

            while(x <= x2)
            {
                screen_set_pixel(s, r, c, x, y);

                ++ x;
                if(delta >= 0)
                {
                    delta += upper_dd;
                }
                else
                {
                    ++ y;
                    delta += lower_dd;
                }
            }
        }
        /* 0 > k > -1 */
        else
        {
            delta = (A << 1) - B;
            upper_dd = (A - B) << 1;
            lower_dd = A << 1;

            while(x <= x2)
            {
                screen_set_pixel(s, r, c, x, y);

                ++ x;
                if(delta >= 0)
                {
                    -- y;
                    delta += upper_dd;
                }
                else
                {
                    delta += lower_dd;
                }
            }
        }
    }
    /* 斜率 <= -1 或者 +1 <= 斜率 */
    else
    {
        /* y1 总是小于 y2 */
        if(y1 > y2)
        {
            x1 = x1 ^ x2;
            x2 = x2 ^ x1;
            x1 = x1 ^ x2;

            y1 = y1 ^ y2;
            y2 = y2 ^ y1;
            y1 = y1 ^ y2;
        }

        A = y1 - y2;
        B = x2 - x1;
        x = x1;
        y = y1;

        /* k > +1 */
        if((x2 - x1 > 0 && y2 - y1 > 0)||(x2 - x1 < 0 && y2 - y1 < 0))
        {
            delta = (B << 1) + A;
            upper_dd = (A + B) << 1;
            lower_dd = B << 1;

            while(y <= y2)
            {
                screen_set_pixel(s, r, c, x, y);

                ++ y;
                if(delta >= 0)
                {
                    ++ x;
                    delta += upper_dd;
                }
                else
                {
                    delta += lower_dd;
                }
            }
        }
        /* k < -1 */
        else
        {
            delta = (B << 1) - A;
            upper_dd = (B - A) << 1;
            lower_dd = B << 1;

            while(y <= y2)
            {
                screen_set_pixel(s, r, c, x, y);

                ++ y;
                if(delta <= 0)
                {
                    -- x;
                    delta += upper_dd;
                }
                else
                {
                    delta += lower_dd;
                }
            }
        }
    }

    return 0;
}

# endif

/*
    bresenham
*/
static si_t
__draw_line_with_bresenham__
(struct screen * s,
 struct rectangle * r,
 struct color * c,
 si_t x1,
 si_t y1,
 si_t x2,
 si_t y2)
{
    si_t a, b, delta, x, y, dx, dy, stepx, stepy, i;

    /* 竖直线 */
    if(x1 == x2)
    {
        return screen_set_v_line(s, r, c, x1, y1, x2, y2);
    }

    /* 水平线 */
    if(y1 == y2)
    {
        return screen_set_h_line(s, r, c, x1, y1, x2, y2);
    }

    /* 特殊的斜线 */
    if(ABSOLUTE_I(x1 - x2) == ABSOLUTE_I(y1 -y2))
    {
        return screen_set_x_line(s, r, c, x1, y1, x2, y2);
    }

    dx = ABSOLUTE_I(x1 - x2);
    dy = ABSOLUTE_I(y1 - y2);

    if(x2 > x1)
    {
        stepx = 1;
    }
    else
    {
        stepx = -1;
    }

    if(y2 > y1)
    {
        stepy = 1;
    }
    else
    {
        stepy = -1;
    }

    x = x1;
    y = y1;

    /* -1 < 斜率 < +1 */
    if(dx > dy)
    {
        a = dy << 1;
        b = (dy - dx) << 1;
        delta = (dy << 1) - dx;

        for(i = 0; i <= dx; ++ i)
        {
            screen_set_pixel(s, r, c, x, y);
            x += stepx;
            if(delta < 0)
            {
                delta += a;
            }
            else
            {
                delta += b;
                y += stepy;
            }
        }
    }
    /* 斜率 <= -1 或者 +1 <= 斜率 */
    else
    {
        a = dx << 1;
        b = (dx - dy) << 1;
        delta = (dx << 1) - dy;

        for(i = 0; i <= dy; ++ i)
        {
            screen_set_pixel(s, r, c, x, y);
            y += stepy;
            if(delta < 0)
            {
                delta += a;
            }
            else
            {
                delta += b;
                x += stepx;
            }
        }
    }

    return 0;
}

si_t
engine_draw_line
(si_t graphics_device_handle,
 si_t x1,
 si_t y1,
 si_t x2,
 si_t y2)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    /*
    return __draw_line_with_dda__(&(gd->screen), &(gd->rectangle), &(gd->color), x1, y1, x2, y2);
    */

    /*
    return __draw_line_with_mp__(&(gd->screen), &(gd->rectangle), &(gd->color), x1, y1, x2, y2);
    */

    return __draw_line_with_bresenham__(&(gd->screen), &(gd->rectangle), &(gd->color), x1, y1, x2, y2);
}
