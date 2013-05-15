# include "../graph_engine.h"

struct color* engine_get_color(si_t graphics_device_handle)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    return &gd->color;
}

