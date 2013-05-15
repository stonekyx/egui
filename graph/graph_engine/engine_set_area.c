# include "../graph_engine.h"

si_t engine_set_area(si_t graphics_device_handle, si_t x, si_t y, si_t width, si_t height)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    gd->rectangle.x = x;
    gd->rectangle.y = y;
    gd->rectangle.width = width;
    gd->rectangle.height = height;

    return 0;
}

