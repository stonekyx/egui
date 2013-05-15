/**
 * @file
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

# include "../graph_engine.h"

si_t
engine_get_font_height
(si_t graphics_device_handle)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

    switch(gd->font)
    {
        case FONT_MATRIX_08:
            return 16;

        case FONT_MATRIX_10:
            return 20;

        case FONT_MATRIX_12:
            return 24;

        case FONT_MATRIX_14:
            return 28;

        case FONT_MATRIX_16:
            return 32;

        case FONT_MATRIX_20:
            return 40;

        case FONT_MATRIX_24:
            return 48;

        case FONT_MATRIX_28:
            return 56;

        case FONT_MATRIX_32:
            return 64;

        case FONT_MATRIX_40:
            return 80;

        case FONT_MATRIX_48:
            return 96;

        case FONT_MATRIX_56:
            return 112;

        case FONT_MATRIX_64:
            return 128;

        default:
            break;
    }

    return -1;
}
