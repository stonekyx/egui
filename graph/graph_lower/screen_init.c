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

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/mman.h>
# include <sys/ioctl.h>

# include <data_structures.h>
# include "../graph_lower.h"
# include "log.h"


struct fb_var_screeninfo global_var_screen_info;
struct fb_fix_screeninfo global_fix_screen_info;
struct fb_cmap global_cmap;
struct screen global_screen;

si_t screen_init(const char* path)
{
    si_t fb_fd, i, retval;

    fb_fd = open(path, O_RDWR);

    if(fb_fd < 0)
    {
		EGUI_PRINT_SYS_ERROR("failed to open framebuffer: open()");
        return -1;
    }

    /* 获得可变的屏幕信息 */
    retval = ioctl(fb_fd, FBIOGET_VSCREENINFO, &global_var_screen_info);

    if(retval == -1)
    {
		EGUI_PRINT_SYS_ERROR("failed to obtain variable screen info: ioctl()");
        return -1;
    }

    /* 获得恒定的屏幕信息 */
    retval = ioctl(fb_fd, FBIOGET_FSCREENINFO, &global_fix_screen_info);

    if(retval == -1)
    {
		EGUI_PRINT_SYS_ERROR("failed to obtain fixed screen info: ioctl()");
        return -1;
    }

    /* 获得屏幕的宽度 */
    global_screen.width = global_var_screen_info.xres;

    /* 获得屏幕的高度 */
    global_screen.height = global_var_screen_info.yres;

    /* 获得屏幕的颜色深度 */
    global_screen.color_depth = global_var_screen_info.bits_per_pixel;

    /* 获得视频存储器的大小或者视频缓冲区的大小 */
    global_screen.size = global_fix_screen_info.smem_len;

    /* XXX: Why always VIDEO_ACCESS_MODE_BUFFER? */
    global_screen.video_access_mode = VIDEO_ACCESS_MODE_BUFFER;

    /* 映射视频地址 */
    global_screen.memory_addr =
    mmap(NULL, global_screen.size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);

    if(global_screen.memory_addr == MAP_FAILED)
    {
		EGUI_PRINT_SYS_ERROR("failed to map screen to memory: mmap()");
        return -1;
    }

    global_screen.buffer_addr = (byte_t *)malloc(global_screen.size);

    if(global_screen.buffer_addr == NULL)
    {
		EGUI_PRINT_SYS_ERROR("failed to malloc for screen buffer: malloc()");
        return -1;
    }

    /* 0 = White 1 = Black */
    if(global_fix_screen_info.visual == FB_VISUAL_MONO01)
    {
        /* make screen black */
        memset(global_screen.memory_addr, 0XFF, global_screen.size);
        memset(global_screen.buffer_addr, 0XFF, global_screen.size);
    }
    /* 1 = White 0 = Black */
    else if(global_fix_screen_info.visual == FB_VISUAL_MONO10)
    {
        /* make screen black */
        memset(global_screen.memory_addr, 0X00, global_screen.size);
        memset(global_screen.buffer_addr, 0X00, global_screen.size);
    }
    /* 真彩 */
    else if(global_fix_screen_info.visual == FB_VISUAL_TRUECOLOR)
    {
        /* make screen black */
        memset(global_screen.memory_addr, 0X00, global_screen.size);
        memset(global_screen.buffer_addr, 0X00, global_screen.size);
    }
    /* 使用颜色列表 */
    else if
    (global_fix_screen_info.visual == FB_VISUAL_PSEUDOCOLOR ||
     global_fix_screen_info.visual == FB_VISUAL_DIRECTCOLOR)
    {
        global_cmap.start = 0;
        global_cmap.len = 1 << global_var_screen_info.bits_per_pixel;

        /* 分配颜色表 */
        global_cmap.red = (ui16_t *)malloc(global_cmap.len << 1);

        if(global_cmap.red == NULL)
        {
			EGUI_PRINT_SYS_ERROR("failed to malloc red color map: malloc()");
            return -1;
        }

        global_cmap.green = (ui16_t *)malloc(global_cmap.len << 1);

        if(global_cmap.green == NULL)
        {
			EGUI_PRINT_SYS_ERROR("failed to malloc green color map: malloc()");
            return -1;
        }

        global_cmap.blue = (ui16_t *)malloc(global_cmap.len << 1);

        if(global_cmap.blue == NULL)
        {
			EGUI_PRINT_SYS_ERROR("failed to malloc blue color map: malloc()");
            return -1;
        }

        global_cmap.transp = (ui16_t *)malloc(global_cmap.len << 1);

        if(global_cmap.transp == NULL)
        {
			EGUI_PRINT_SYS_ERROR("failed to malloc transparent color map: malloc()");
            return -1;
        }

        /* 设置颜色表 */
        for(i = 0; i < global_cmap.len; ++ i)
        {
            global_cmap.red[i] = ((i << 1) + 1) * ((ui16_t)-1) / (global_cmap.len << 1);
        }
        memcpy(global_cmap.green, global_cmap.red, global_cmap.len << 1);
        memcpy(global_cmap.blue, global_cmap.red, global_cmap.len << 1);
        memcpy(global_cmap.transp, global_cmap.red, global_cmap.len << 1);

        if(ioctl(fb_fd, FBIOPUTCMAP, &global_cmap) == -1)
        {
			EGUI_PRINT_SYS_ERROR("failed to set color map: ioctl()");
            return -1;
        }

        /* make screen black */
        memset(global_screen.memory_addr, 0X00, global_screen.size);
        memset(global_screen.buffer_addr, 0X00, global_screen.size);
    }
    /* 使用只读颜色列表 */
    else if(global_fix_screen_info.visual == FB_VISUAL_STATIC_PSEUDOCOLOR)
    {
        /* unspecified */
    }

    /* 关闭 fb 设备文件 */
    close(fb_fd);

    return 0;
}
