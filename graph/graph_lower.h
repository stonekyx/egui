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

# ifndef _GRAPH_LOWER_H_
# define _GRAPH_LOWER_H_ 1

# include <linux/fb.h>

# include <base_type.h>
# include <geometry.h>
# include <color.h>

# include "bitops.h"
# include "graph_enumerator.h"

/**
 * 屏幕结构体
**/
struct screen
{
    /**
     * 水平尺寸
    **/
    si_t width;

    /**
     * 竖直尺寸
    **/
    si_t height;

    /**
     * 视频访问模式
    **/
    si_t video_access_mode;

    /**
     * 颜色深度
    **/
    si_t color_depth;

    /**
     * 视频存储区大小或者视频缓冲区大小
    **/
    si_t size;

    /**
     * 视频存储区起始地址
    **/
    byte_t * memory_addr;

    /**
     * 视频缓冲区起始地址
    **/
    byte_t * buffer_addr;
};

/**
 * 全局变量
 *
 * 屏幕的可变信息
**/
extern struct fb_var_screeninfo global_var_screen_info;

/**
 * 全局变量
 *
 * 屏幕的固定信息
**/
extern struct fb_fix_screeninfo global_fix_screen_info;

/**
 * 全局变量
 *
 * 颜色表
**/
extern struct fb_cmap global_cmap;

/**
 * 全局变量
 *
 * 屏幕结构体
**/
extern struct screen global_screen;

/**
 * 全局变量
 *
 * 颜色的限制
**/
extern struct color_limit global_color_limit;

/**
 * 申请屏幕资源
 *
 * @param path framebuffer设备路径
 *
 * @return 成功返回0，失败返回-1。
*/
extern si_t screen_init(const char* path);

/**
 * 释放屏幕资源
 *
 * @return 成功返回0，失败返回-1。
**/
extern si_t screen_exit();

/**
 * 获取屏幕的宽度
 **/
extern si_t screen_get_width();

/**
 * 获取屏幕的长度
 **/
extern si_t screen_get_height();

/**
 * 将屏幕绘制成白色
 *
 * 将 s 所指的屏幕上的每一个像素绘制成白色
 *
 * @param s 屏幕结构体指针
 *
 * @return 成功返回0，失败返回-1。
**/
extern si_t screen_clear_white(struct screen * s);

/**
 * 将屏幕绘制成黑色
 *
 * 将 s 所指的屏幕上的每一个像素绘制成黑色
 *
 * @param s 屏幕结构体指针
 *
 * @return 成功返回0，失败返回-1。
**/
extern si_t screen_clear_black(struct screen * s);

/**
 * 刷新屏幕的某一区域
 *
 * 将 s 所指的屏幕上 reactangle 所指的区域刷新为视频缓冲区的值
 *
 * @param s 屏幕结构体指针
 *
 * @param r 区域指针
 *
 * @return 成功返回0，失败返回-1。
**/
extern si_t screen_update(struct screen * s, struct rectangle * r);

/*
///////////////////////////////////////////////////////////////
    color.c
///////////////////////////////////////////////////////////////
*/

extern si_t screen_color_limit();

/**
 * 通过颜色的结构体表示获得颜色的数值表示
 *
 * get field of color
 *
 * @param value 颜色数值地址
 * @param c 颜色结构体指针
 *
 * @return 0
**/
extern si_t screen_color_to_value(ui_t * value, struct color * c);

/**
 * 通过颜色的数值表示获得颜色的结构体表示
 *
 * set field of color
 *
 * @param c 颜色结构体指针
 * @param value 颜色数值地址
 *
 * @return 0
**/
extern si_t screen_value_to_color(struct color * c, ui_t * value);


/**
 * 通过颜色的结构体表示获得颜色的索引表示
 *
 * get field of color
 *
 * @param index 颜色索引地址
 * @param c 颜色结构体指针
 *
 * @return 0
**/
extern si_t screen_color_to_index(ui_t * index, struct color * c);

/**
 * 通过颜色的索引表示获得颜色的结构体表示
 *
 * set field of color
 *
 * @param c 颜色结构体指针
 * @param index 颜色索引地址
 *
 * @return 0
**/
extern si_t screen_index_to_color(struct color * c, ui_t * index);

/*
///////////////////////////////////////////////////////////////
    pixel.c
///////////////////////////////////////////////////////////////
*/

/**
 * 绘制屏幕上的某一点
 *
 * 将 s 所指的屏幕上坐标为（x，y）的像素绘制成 c 所指的颜色
 *
 * @param s 屏幕结构体指针
 * @param c 颜色结构体指针
 * @param x 像素的横坐标
 * @param y 像素的纵坐标
 * @param a 像素所在的区域
 *
 * @return 成功返回0，失败返回-1。
**/
extern si_t screen_set_pixel(struct screen * s, struct rectangle * a, struct color * c, si_t x, si_t y);

/**
 * 获得屏幕上的某一点
 *
 * 将 s 所指的屏幕上坐标为（x，y）的像素保存到 c 所指的颜色结构体中
 *
 * @param s 屏幕结构体指针
 * @param c 颜色结构体指针
 * @param x 像素的横坐标
 * @param y 像素的纵坐标
 * @param a 像素所在的区域
 *
 * @return 成功返回0，失败返回-1。
**/
extern si_t screen_get_pixel(struct screen * s, struct rectangle * a, struct color * c, si_t x, si_t y);

/**
 * 在屏幕上绘制某一横线
 *
 * 在 s 所指的屏幕上 a 所指区域内绘制 c 所指颜色的起点坐标为（x1，y1）终点坐标为（x2，y2）的横线
 *
 * @param s 屏幕结构体指针
 * @param a 横线所在的区域
 * @param c 颜色结构体指针
 * @param x1 起点的横坐标
 * @param y1 起点的竖坐标
 * @param x2 终点的横坐标
 * @param y2 终点的竖坐标
 *
 * @return 成功返回0，失败返回-1。
**/
extern si_t screen_set_h_line(struct screen * s, struct rectangle * a, struct color * c, si_t x1, si_t y1, si_t x2, si_t y2);

/**
 * 在屏幕上绘制某一竖线
 *
 * 在 s 所指的屏幕上 a 所指区域内绘制 c 所指颜色的起点坐标为（x1，y1）终点坐标为（x2，y2）的竖线
 *
 * @param s 屏幕结构体指针
 * @param a 竖线所在的区域
 * @param c 颜色结构体指针
 * @param x1 起点的横坐标
 * @param y1 起点的竖坐标
 * @param x2 终点的横坐标
 * @param y2 终点的竖坐标
 *
 * @return 成功返回0，失败返回-1。
**/
extern si_t screen_set_v_line(struct screen * s, struct rectangle * a, struct color * c, si_t x1, si_t y1, si_t x2, si_t y2);

/**
 * 在屏幕上绘制某一斜线
 *
 * 在 s 所指的屏幕上 a 所指区域内绘制 c 所指颜色的起点坐标为（x1，y1）终点坐标为（x2，y2）的斜线
 *
 * @param s 屏幕结构体指针
 * @param a 斜线所在的区域
 * @param c 颜色结构体指针
 * @param x1 起点的横坐标
 * @param y1 起点的竖坐标
 * @param x2 终点的横坐标
 * @param y2 终点的竖坐标
 *
 * @return 成功返回0，失败返回-1。
**/
extern si_t screen_set_x_line(struct screen * s, struct rectangle * a, struct color * c, si_t x1, si_t y1, si_t x2, si_t y2);

/**
 * 在屏幕上绘制某一矩形
 *
 * 在 s 所指的屏幕上 a 所指区域内绘制 c 所指颜色的顶点坐标为（x，y）宽度为 xsize 高度为 ysize 的矩形
 *
 * @param s 屏幕结构体指针
 * @param a 斜线所在的区域
 * @param c 颜色结构体指针
 * @param x 矩形顶点的横坐标
 * @param y 矩形顶点的竖坐标
 * @param w 矩形的宽度
 * @param h 矩形的高度
 *
 * @return 成功返回0，失败返回-1。
**/
extern si_t screen_set_area(struct screen * s, struct rectangle * a, struct color * c, si_t x, si_t y, si_t w, si_t h);

/**
 * 将屏幕绘制成某种颜色
 *
 * 将 s 所指的屏幕上的每一个像素绘制成 c 所指的颜色
 *
 * @param s 屏幕结构体指针
 * @param c 颜色结构体指针
 *
 * @return 成功返回0，失败返回-1。
**/
extern si_t screen_clear(struct screen * s, struct color * c);

/**
 * @brief 把一个像素区域从 s 复制到 d
 *
 * @param dst 是目标存储区域的起始地址
 * @param src 是来源存储区域的起始地址
 * @param dst_x 是 dst 中像素区域左上角的横坐标
 * @param dst_y 是 dst 中像素区域左上角的纵坐标
 * @param src_x 是 src 中像素区域左上角的横坐标
 * @param src_y 是 src 中像素区域左上角的纵坐标
 * @param x_size 是像素区域的宽度
 * @param y_size 是像素区域的高度
 *
 * @return 0
**/
extern si_t screen_cpy_area(void * dst, void * src, si_t dst_x, si_t dst_y, si_t src_x, si_t src_y, si_t x_size, si_t y_size);

/**
 * 将视频缓冲区刷新到视频存储区
 *
 * @param x_axis 区域左上角的横坐标
 * @param y_axis 区域左上角的纵坐标
 * @param x_size 区域的宽度
 * @param y_size 区域的高度
 *
 * @return 0
**/
extern si_t screen_flush(si_t x_axis, si_t y_axis, si_t x_size, si_t y_size);

# endif
