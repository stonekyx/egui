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
# include <string.h>

# include "../graph_lower.h"

/*
    make screen black
*/
si_t
screen_clear_black
(struct screen * s)
{
    addr_t video;
    if(VIDEO_ACCESS_MODE_BUFFER == s->video_access_mode) {
        video = s->buffer_addr;
    } else if(VIDEO_ACCESS_MODE_DIRECT == s->video_access_mode){
        video = s->memory_addr;
    } else {
        return -1;
    }

    /* 0 = White 1 = Black */
    if(global_fix_screen_info.visual == FB_VISUAL_MONO01)
    {
        memset(video, 0XFF, s->size);
    }
    /* 1 = White 0 = Black */
    else if(global_fix_screen_info.visual == FB_VISUAL_MONO10)
    {
        memset(video, 0X00, s->size);
    }
    else if(global_fix_screen_info.visual == FB_VISUAL_PSEUDOCOLOR)
    {
        memset(video, 0X00, s->size);
    }
    else if(global_fix_screen_info.visual == FB_VISUAL_STATIC_PSEUDOCOLOR)
    {
        memset(video, 0X00, s->size);
    }
    else if(global_fix_screen_info.visual == FB_VISUAL_DIRECTCOLOR)
    {
        memset(video, 0X00, s->size);
    }
    else if(global_fix_screen_info.visual == FB_VISUAL_TRUECOLOR)
    {
        memset(video, 0X00, s->size);
    }

    return 0;
}
