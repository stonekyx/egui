# include <data_structures.h>

# include "../graph_engine.h"

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

    {
        struct point *pos;
        struct bresenham_iterator *it;
        bresenham_for_each(pos, it, x1, y1, x2, y2) {
            screen_set_pixel(s, r, c, pos->x, pos->y);
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
