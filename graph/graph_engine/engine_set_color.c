# include "../graph_engine.h"

si_t engine_set_color(si_t graphics_device_handle, si_t r, si_t g, si_t b, si_t a)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    gd->color.r = r;
    gd->color.g = g;
    gd->color.b = b;
    gd->color.a = a;

    return 0;
}

