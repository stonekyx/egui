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
    make screen white
*/
si_t
screen_clear_white
(struct screen * s)
{
    /* 0 = White 1 = Black */
    if(global_fix_screen_info.visual == FB_VISUAL_MONO01)
    {
        if(s->video_access_mode == VIDEO_ACCESS_MODE_BUFFER)
        {
            memset(s->buffer_addr, 0X00, s->size);
        }
        else if(s->video_access_mode == VIDEO_ACCESS_MODE_DIRECT)
        {
            memset(s->memory_addr, 0X00, s->size);
        }
        else
        {
            return -1;
        }
    }
    /* 1 = White 0 = Black */
    else if(global_fix_screen_info.visual == FB_VISUAL_MONO10)
    {
        if(s->video_access_mode == VIDEO_ACCESS_MODE_BUFFER)
        {
            memset(s->buffer_addr, 0XFF, s->size);
        }
        else if(s->video_access_mode == VIDEO_ACCESS_MODE_DIRECT)
        {
            memset(s->memory_addr, 0XFF, s->size);
        }
        else
        {
            return -1;
        }
    }
    else if(global_fix_screen_info.visual == FB_VISUAL_PSEUDOCOLOR)
    {
        if(s->video_access_mode == VIDEO_ACCESS_MODE_BUFFER)
        {
            memset(s->buffer_addr, 0XFF, s->size);
        }
        else if(s->video_access_mode == VIDEO_ACCESS_MODE_DIRECT)
        {
            memset(s->memory_addr, 0XFF, s->size);
        }
        else
        {
            return -1;
        }
    }
    else if(global_fix_screen_info.visual == FB_VISUAL_STATIC_PSEUDOCOLOR)
    {
        if(s->video_access_mode == VIDEO_ACCESS_MODE_BUFFER)
        {
            memset(s->buffer_addr, 0XFF, s->size);
        }
        else if(s->video_access_mode == VIDEO_ACCESS_MODE_DIRECT)
        {
            memset(s->memory_addr, 0XFF, s->size);
        }
        else
        {
            return -1;
        }
    }
    else if(global_fix_screen_info.visual == FB_VISUAL_DIRECTCOLOR)
    {
        if(s->video_access_mode == VIDEO_ACCESS_MODE_BUFFER)
        {
            memset(s->buffer_addr, 0XFF, s->size);
        }
        else if(s->video_access_mode == VIDEO_ACCESS_MODE_DIRECT)
        {
            memset(s->memory_addr, 0XFF, s->size);
        }
        else
        {
            return -1;
        }
    }
    else if(global_fix_screen_info.visual == FB_VISUAL_TRUECOLOR)
    {
        if(s->video_access_mode == VIDEO_ACCESS_MODE_BUFFER)
        {
            memset(s->buffer_addr, 0XFF, s->size);
        }
        else if(s->video_access_mode == VIDEO_ACCESS_MODE_DIRECT)
        {
            memset(s->memory_addr, 0XFF, s->size);
        }
        else
        {
            return -1;
        }
    }

    return 0;
}
