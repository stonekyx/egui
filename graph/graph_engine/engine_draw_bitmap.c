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

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

# include "../graph_engine.h"
# include "log.h"

/* static declarition {{{ */
static ui_t __getdot__(void * addr, ui_t color_depth, ui_t position);
static si_t __is_color_table_used__(struct bitmap_info * info);
static si_t __is_color_field_used__(struct bitmap_info * info);
static si_t __show_with_color_table__(struct bitmap * bm, struct graphics_device* gd, struct rectangle* location);
static si_t __show_with_color_field__(struct bitmap * bm, struct graphics_device* gd, struct rectangle* location);
static si_t __show_only_data__(struct bitmap * bm, struct graphics_device* gd, struct rectangle* location);
static si_t bitmap_init(struct bitmap * bm);
static si_t bitmap_exit(struct bitmap * bm);
static si_t show_bitmap(struct bitmap * bm, struct graphics_device* gd, struct rectangle* location);
static si_t bitmap_align(struct bitmap* bm, struct rectangle* location, int align);
/* }}} */

si_t engine_draw_bitmap(si_t graphics_device_handle, char* path, si_t align)
{
	struct bitmap bm;
    struct rectangle area;
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;
    area = gd->rectangle;

	bm.path = path;

	if(bitmap_init(&bm) == -1)
        return -1;

    bitmap_align(&bm, &area, align);

	show_bitmap(&bm, gd, &area);

	bitmap_exit(&bm);

    return 0;
}

/**
 * 申请位图资源
 *
 * @param bm 位图结构指针
 *
 * @return 0 if success, -1 if any error occurs
**/
static si_t bitmap_init(struct bitmap * bm)
{
    si_t fd;
    struct stat file_stat;

    /* 打开位图文件 */
    if((fd = open(bm->path, O_RDONLY)) < 0)
    {
		EGUI_PRINT_SYS_ERROR("failed to open bmp file %s: open()", bm->path);
        return -1;
    }

    /* 获得位图文件大小 */
    if(stat(bm->path, &file_stat) != 0)
    {
		EGUI_PRINT_SYS_ERROR("failed to get status of bmp file: stat()");
        return -1;
    }

    /* 将文件大小赋给位图结构体成员 */
    bm->size = file_stat.st_size;

    /* 将位图映射到内存 */
    if((bm->addr = mmap(NULL, bm->size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
		EGUI_PRINT_SYS_ERROR("failed to map bitmap file to memory: mmap()");
        return -1;
    }

    close(fd);

    /* 位图文件头部区域地址 */
    bm->head = (struct bitmap_head *)(bm->addr);
    /* 位图文件信息区域地址 */
    bm->info = (struct bitmap_info *)(bm->addr + sizeof(struct bitmap_head));

    /* 使用了颜色列表 */
    if(__is_color_table_used__(bm->info) == 1)
    {
        /* 初始化颜色列表地址 */
        bm->color_table = (struct rgb *)(bm->addr + sizeof(struct bitmap_head) + sizeof(struct bitmap_info));
    }
    /* 没有使用颜色列表 */
    else
    {
        bm->color_table = NULL;
    }

    /* 使用了颜色掩码 */
    if(__is_color_field_used__(bm->info) == 1)
    {
        bm->color_table = NULL;
        /* 初始化颜色掩码地址 */
        bm->color_field = (struct color_field *)(bm->addr + sizeof(struct bitmap_head) + sizeof(struct bitmap_info));
    }
    /* 没有使用颜色掩码 */
    else
    {
        bm->color_field = NULL;
    }

    /* 初始化位图数据区域地址 */
    bm->data = bm->addr + bm->head->bhOffBits;
    /* 初始化位图颜色深度 */
    bm->color_depth = bm->info->biBitCount;

    return 0;
}

/**
 * 释放位图资源
 *
 * @param bm 位图结构指针
 *
 * @return 0
**/
static si_t bitmap_exit(struct bitmap * bm)
{
    /* 解除文件到内存的映射 */
    munmap(bm->addr, bm->size);

    /* 所有资源清空 */
    memset(bm, 0, sizeof(struct bitmap));

    return 0;
}

/**
 * 位图显示函数
 *
 * @param bm 位图结构指针
 *
 * @return 0
**/
static si_t show_bitmap(struct bitmap * bm, struct graphics_device* gd, struct rectangle* location)
{
    /* 使用了颜色列表 */
    if(bm->color_table != NULL)
    {
        __show_with_color_table__(bm, gd, location);
    }
    /* 使用了颜色掩码 */
    else if(bm->color_table != NULL)
    {
        __show_with_color_field__(bm, gd, location);
    }
    /* 既没有使用颜色列表也没有使用颜色掩码 */
    else
    {
        __show_only_data__(bm, gd, location);
    }

    return 0;
}

/**
 * adjust the bitmap to the area according to the alignment parameter
 *
 * @param bm: pointer to bitmap
 * @param location: location of the whole area as input, location of the area that fit the picture as output
 * @param align: see ALIGN_TYPE
 *
 * @return 0
 **/
static si_t bitmap_align(struct bitmap* bm, struct rectangle* location, int align)
{
    int x = bm->info->biWidth;
    int origin = bm->info->biHeight;
    int y = origin > 0 ? origin : (- origin);

    /**
     * horizontally
     **/
    if(location->width > x)
    {
        switch(align & ALIGN_HORIZONTAL_TYPE_MASK)
        {
        case ALIGN_HORIZONTAL_TYPE_LEFT:
            location->width = x;
            break;

        case ALIGN_HORIZONTAL_TYPE_CENTER:
            location->x += (location->width - x) / 2;
            location->width = x;
            break;

        case ALIGN_HORIZONTAL_TYPE_RIGHT:
            location->x += location->width - x;
            location->width = x;
            break;

        default:
            break;
        }
    }
    
    /**
     * vertically
     **/
    if(location->height > y)
    {
        switch(align & ALIGN_VERTICAL_TYPE_MASK)
        {
        case ALIGN_VERTICAL_TYPE_UP:
            location->height = y;
            break;

        case ALIGN_VERTICAL_TYPE_CENTER:
            location->y += (location->height - y) / 2;
            location->height = y;
            break;

        case ALIGN_VERTICAL_TYPE_DOWN:
            location->y += location->height - y;
            location->height = y;
            break;

        default:
            break;
        }
    }

    return 0;
}

/**
 * 获得位图数据区域中某个像素的值
 *
 * @param addr 位图数据区域的起始地址
 * @param color_depth 每个像素所占的位数
 * @param position 像素的位置
 *
 * @return 像素的值
**/
static ui_t __getdot__(void * addr, ui_t color_depth, ui_t position)
{
    ui08_t * temp = addr;
    ui_t value;

    switch(color_depth)
    {
        /* 01 位颜色 */
        case 1:
            value = temp[position >> 3];
            value >>= position & 7U;
            return value & 0X1U;
        /* 04 位颜色 */
        case 4:
            value = temp[position >> 1];
            value >>= (position & 1U) << 2;
            return value & 0XFU;
        /* 08 位颜色 */
        case 8:
            return temp[position];
        /* 16 位颜色 */
        case 16:
            temp += position + position;
            return * ((ui16_t *)temp);
        /* 24 位颜色 */
        case 24:
            temp += position + position + position;
            value = * ((ui32_t *)temp);
            return value & 0XFFFFFFU;
        /* 32 位颜色 */
        case 32:
            temp += position << 2;
            return * ((ui32_t *)temp);

        default:
            break;
    }

    return 0;
}

/**
 * 判断位图是否使用了颜色列表
 *
 * @param info 位图信息区域的起始地址
 *
 * @return 使用了颜色列表，返回 1；没有使用颜色列表返回 0。
**/
static si_t __is_color_table_used__(struct bitmap_info * info)
{
    switch(info->biBitCount)
    {
        case 1:
            return 1;
        case 4:
            return 1;
        case 8:
            return 1;

        /* 若 biCompression == BI_RGB，则不使用颜色列表。 */
        /* 若 biCompression == BI_BITFIELDS，则不使用颜色列表。 */
        case 16:
            if
            ((info->biCompression == BI_RGB) ||
             (info->biCompression == BI_BITFIELDS))
            {
                return 0;
            }
            else
            {
                return 1;
            }

        case 24:
            return 0;

        /* 若 biCompression == BI_RGB，则不使用颜色列表。 */
        /* 若 biCompression == BI_BITFIELDS，则不使用颜色列表。 */
        case 32:
            if
            ((info->biCompression == BI_RGB) ||
             (info->biCompression == BI_BITFIELDS))
            {
                return 0;
            }
            else
            {
                return 1;
            }

        default:
            break;
    }

    return -1;
}

/**
 * 判断位图是否使用了颜色掩码
 *
 * @param info 位图信息区域的起始地址
 *
 * @return 使用了颜色颜色掩码，返回 1；没有使用颜色掩码返回 0。
**/
static si_t __is_color_field_used__(struct bitmap_info * info)
{
    switch(info->biBitCount)
    {
        case 1:
            return 0;
        case 4:
            return 0;
        case 8:
            return 0;

        /* 若 biCompression == BI_BITFIELDS，则不使用颜色掩码。 */
        case 16:
            if(info->biCompression == BI_BITFIELDS)
            {
                return 1;
            }
            else
            {
                return 0;
            }

        case 24:
            return 0;

        /* 若 biCompression == BI_BITFIELDS，则不使用颜色掩码。 */
        case 32:
            if(info->biCompression == BI_BITFIELDS)
            {
                return 1;
            }
            else
            {
                return 0;
            }

        default:
            break;
    }

    return -1;
}

/**
 * 有颜色列表时的显示位图
 *
 * @param bm 位图结构指针
 *
 * @return 0
**/
static si_t __show_with_color_table__(struct bitmap * bm, struct graphics_device* gd, struct rectangle* location)
{
    ui_t offset;
    struct rgb * rgb;
    struct color c={0};
    si_t x, y, i, j, origin;

    /* 获得位图的宽度 */
    x = bm->info->biWidth;
    /* 获得位图的高度 */
    origin = bm->info->biHeight;
    y = origin > 0 ? origin : (- origin);

    /* 原点在左下角 */
    if(origin > 0)
    {
        for(j = 0; j < location->height; ++ j)
        {
            /* 当前行起始像素的偏移量 */
            offset = (y - j - 1) * x;

            for(i = 0; i < location->width; ++ i)
            {
                /* 获得像素颜色在颜色列表中的地址 */
                rgb = bm->color_table + __getdot__(bm->data, bm->color_depth, offset + i);

                /* 获取颜色分量 */
                c.r = (rgb->rgbRed) * global_color_limit.r_limit / (ui08_t)-1;
                c.g = (rgb->rgbGreen) * global_color_limit.g_limit / (ui08_t)-1;
                c.b = (rgb->rgbBlue) * global_color_limit.b_limit / (ui08_t)-1;

                /* 设置图形设备的颜色 */
                screen_set_pixel(&(gd->screen), location, &c, location->x + i, location->y + j);
            }
        }
    }
    /* 原点在左上角 */
    else if(origin < 0)
    {
        for(j = 0; j < location->height; ++ j)
        {
            /* 当前行起始像素的偏移量 */
            offset = j * x;

            for(i = 0; i < location->width; ++ i)
            {
                /* 获得像素颜色在颜色列表中的地址 */
                rgb = bm->color_table + __getdot__(bm->data, bm->color_depth, offset + i);

                /* 获取颜色分量 */
                c.r = (rgb->rgbRed) * global_color_limit.r_limit / (ui08_t)-1;
                c.g = (rgb->rgbGreen) * global_color_limit.g_limit / (ui08_t)-1;
                c.b = (rgb->rgbBlue) * global_color_limit.b_limit / (ui08_t)-1;

                /* 设置图形设备的颜色 */
                screen_set_pixel(&(gd->screen), location, &c, location->x + i, location->y + j);
            }
        }
    }

    return 0;
}

/**
 * 有颜色掩码时的显示位图函数
 *
 * @param bm 位图结构指针
 *
 * @return 0
**/
static si_t __show_with_color_field__(struct bitmap * bm, struct graphics_device* gd, struct rectangle* location)
{
    ui_t r_field, g_field, b_field, offset, rgb;
    struct color c={0};
    si_t x, y, i, j, origin;

    /* 获得颜色掩码 */
    r_field = bm->color_field->rField;
    g_field = bm->color_field->gField;
    b_field = bm->color_field->bField;

    /* 获得位图的宽度 */
    x = bm->info->biWidth;
    /* 获得位图的高度 */
    origin = bm->info->biHeight;
    y = origin > 0 ? origin : (- origin);

    /* 原点在左下角 */
    if(origin > 0)
    {
        for(j = 0; j < location->height; ++ j)
        {
            /* 当前行起始像素的偏移量 */
            offset = (y - j - 1) * x;

            for(i = 0; i < location->width; ++ i)
            {
                /* 获得像素的值 */
                rgb = __getdot__(bm->data, bm->color_depth, offset + i);

                /* 获取颜色分量 */
                c.r = (rgb & r_field) * global_color_limit.r_limit / (ui08_t)-1;
                c.g = (rgb & g_field) * global_color_limit.g_limit / (ui08_t)-1;
                c.b = (rgb & b_field) * global_color_limit.b_limit / (ui08_t)-1;

                screen_set_pixel(&(gd->screen), location, &c, location->x + i, location->y + j);
            }
        }
    }
    /* 原点在左上角 */
    else if(origin < 0)
    {
        for(j = 0; j < location->height; ++ j)
        {
            /* 当前行起始像素的偏移量 */
            offset = j * x;

            for(i = 0; i < location->width; ++ i)
            {
                /* 获得像素的值 */
                rgb = __getdot__(bm->data, bm->color_depth, offset + i);

                /* 获取颜色分量 */
                c.r = (rgb & r_field) * global_color_limit.r_limit / (ui08_t)-1;
                c.g = (rgb & g_field) * global_color_limit.g_limit / (ui08_t)-1;
                c.b = (rgb & b_field) * global_color_limit.b_limit / (ui08_t)-1;

                screen_set_pixel(&(gd->screen), location, &c, location->x + i, location->y + j);
            }
        }
    }

    return 0;
}

/**
 * 没有颜色列表也没有颜色掩码时的显示位图函数
 *
 * @param bm 位图结构指针
 *
 * @return 0
**/
static si_t __show_only_data__(struct bitmap * bm, struct graphics_device* gd, struct rectangle* location)
{
    ui_t offset, rgb, r_mask, g_mask, b_mask, r_shift, g_shift, b_shift;
    struct color c={0};
    si_t x, y, i, j, origin;

    /* 获得位图的宽度 */
    x = bm->info->biWidth;
    /* 获得位图的高度 */
    origin = bm->info->biHeight;
    y = origin > 0 ? origin : (- origin);

    switch(bm->info->biBitCount)
    {
        case 16:
            b_mask = 0X1F;
            g_mask = 0X3E0;
            r_mask = 0X7C00;
            b_shift = 0X0;
            g_shift = 0X5;
            r_shift = 0XA;

            break;
        case 24:
            b_mask = 0XFF;
            g_mask = 0XFF00;
            r_mask = 0XFF0000;
            b_shift = 0X0;
            g_shift = 0X8;
            r_shift = 0X10;

            break;
        case 32:
            b_mask = 0XFF;
            g_mask = 0XFF00;
            r_mask = 0XFF0000;
            b_shift = 0X0;
            g_shift = 0X8;
            r_shift = 0X10;

            break;
        default:
            return 0;
    }

    /* 原点在左下角 */
    if(origin > 0)
    {
        for(j = 0; j < location->height; ++ j)
        {
            /* 当前行起始像素的偏移量 */
            offset = (y - j - 1) * x;

            for(i = 0; i < location->width; ++ i)
            {
                /* 获得像素的值 */
                rgb = __getdot__(bm->data, bm->color_depth, offset + i);

                /* 设置颜色 */
                c.b = ((rgb & b_mask) >> b_shift) * global_color_limit.b_limit / (ui08_t)-1;
                c.g = ((rgb & g_mask) >> g_shift) * global_color_limit.g_limit / (ui08_t)-1;
                c.r = ((rgb & r_mask) >> r_shift) * global_color_limit.r_limit / (ui08_t)-1;

                screen_set_pixel(&(gd->screen), location, &c, location->x + i, location->y + j);
            }
        }
    }
    /* 原点在左上角 */
    else if(origin < 0)
    {
        for(j = 0; j < location->height; ++ j)
        {
            /* 当前行起始像素的偏移量 */
            offset = j * x;

            for(i = 0; i < location->width; ++ i)
            {
                /* 获得像素的值 */
                rgb = __getdot__(bm->data, bm->color_depth, offset + i);

                /* 设置颜色 */
                c.b = ((rgb & b_mask) >> b_shift) * global_color_limit.b_limit / (ui08_t)-1;
                c.g = ((rgb & g_mask) >> g_shift) * global_color_limit.g_limit / (ui08_t)-1;
                c.r = ((rgb & r_mask) >> r_shift) * global_color_limit.r_limit / (ui08_t)-1;

                screen_set_pixel(&(gd->screen), location, &c, location->x + i, location->y + j);
            }
        }
    }

    return 0;
}

