# include "../graph_engine.h"

struct rectangle* engine_get_area(si_t graphics_device_handle)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    return &gd->rectangle;
}

