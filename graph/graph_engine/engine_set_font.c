# include "../graph_engine.h"

si_t engine_set_font(si_t graphics_device_handle, si_t font)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    gd->font = font;

    return 0;
}

