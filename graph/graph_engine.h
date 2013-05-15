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

# ifndef _GRAPH_ENGINE_H_
# define _GRAPH_ENGINE_H_ 1

# include "graph_lower.h"

# define __AMOUNT__(head, tail) ((tail) - (head) + 1)

/**
 * 整数取绝对值
 *
 * @param i 要取绝对值的数
 *
 * @return 返回绝对值
**/
# define ABSOLUTE_I(i) \
    (((i) < 0) ? (-(i)) : (i))

/**
 * 通过结构中成员的地址获得结构的地址
 *
 * @param member_addr 成员的地址
 * @param struct_name 结构的名字
 * @param member_name 成员的名字
 *
 * @return 结构的地址
**/
# define CONTAINER_OF(member_addr, struct_name, member_name) \
    ((struct_name *)((byte_t *)(member_addr) - (ui_t)(&(((struct_name *)0)->member_name))))

/**
 * 压缩类型
**/
enum __bitmap_compression__
{
    BI_RGB = 0,
    BI_RLE8,
    BI_RLE4,
    BI_BITFIELDS,
    BI_JPEG,
    BI_PNG
};

struct bitmap_head
{
    /**
     * 文件类型
     *
     * ui16_t
     *
     * 必须是 0X4D42 (BM)
    **/
    ui08_t bhType[__AMOUNT__(1, 2)];
    /**
     * 文件大小
     *
     * ui32_t
     *
     * 以字节为单位
    **/
    ui08_t bhSize[__AMOUNT__(3, 6)];
    /**
     * 保留
     *
     * ui16_t
     *
     * 必须是 0
    **/
    ui08_t bhReserved1[__AMOUNT__(7, 8)];
    /**
     * 保留
     *
     * ui16_t
     *
     * 必须是 0
    **/
    ui08_t bhReserved2[__AMOUNT__(9, 10)];
    /**
     * 位图数据的偏移量
     *
     * ui32_t
    **/
    ui08_t bhOffBits[__AMOUNT__(11, 14)];
};

struct bitmap_info
{
    /**
     * 结构的大小
     *
     * ui32_t
     *
     * 以字节为单位
    **/
    ui08_t biSize[__AMOUNT__(1, 4)];
    /**
     * 位图的宽度
     *
     * si32_t
     *
     * 以像素为单位
    **/
    ui08_t biWidth[__AMOUNT__(5, 8)];
    /**
     * 位图的高度
     *
     * si32_t
     *
     * 以像素为单位
     *
     * 若是正数，则位图原点在左下角。
     * 若是负数，则位图原点在左上角且 biCompression 必须是 BI_RGB 或者 BI_BITFIELDS。此时不能压缩位图。
    **/
    ui08_t biHeight[__AMOUNT__(9, 12)];
    /**
     * 目标屏幕的数目
     *
     * ui16_t
     *
     * 必须是 1
    **/
    ui08_t biPlanes[__AMOUNT__(13, 14)];
    /**
     * 颜色深度
     *
     * ui16_t
     *
     * 必须是 01, 04, 08, 16, 24, 32 中的一个
     *
     * == 01
     *  	颜色列表中有 pow(2, 01) 种颜色。
     *  	位图数据区域用 01 位对应颜色列表中的一种颜色。
     *
     * == 04
     *  	颜色列表中有 pow(2, 04) 种颜色。
     *  	位图数据区域用 01 位对应颜色列表中的一种颜色。
     *
     * == 08
     *  	颜色列表中有 pow(2, 08) 种颜色。
     *  	位图数据区域用 08 位对应颜色列表中的一种颜色。
     *
     * == 16
     *     颜色列表中有 pow(2, 16) 种颜色。
     *     位图数据区域用 16 位对应颜色列表中的一种颜色。
     *     若 biCompression == BI_RGB，则不使用颜色列表。
     * 从低到高 16 位的每 5 位分别对应蓝色绿色红色。最高的
     * 1 位不使用。
     *     若 biCompression == BI_BITFIELDS，则不使用颜色列表。
     * 存放颜色列表的位置存放 3 个 ui32_t，分别对应红色绿色蓝色
     * 的掩码。
     *
     * == 24
     *     不使用颜色列表，位图数据区域用 24 位对应颜色一种颜色。
     * 从低到高 24 位的每 8 位分别对应蓝色绿色红色。
     *
     * == 32
     *     颜色列表中有 pow(2, 32) 种颜色。
     *     位图数据区域用 32 位对应颜色列表中的一种颜色。
     *     若 biCompression == BI_RGB，则不使用颜色列表。
     * 从低到高 32 位的每 8 位分别对应蓝色绿色红色。最高的
     * 8 位不使用。
     *     若 biCompression == BI_BITFIELDS，则不使用颜色列表。
     * 存放颜色列表的位置存放 3 个 ui32_t，分别对应红色绿色蓝色
     * 的掩码。
    **/
    ui08_t biBitCount[__AMOUNT__(15, 16)];
    /**
     * 位图压缩种类
     *
     * ui32_t
     *
     * 必须是 BI_RGB, BI_BITFIELDS, BI_ALPHABITFIELDS
    **/
    ui08_t biCompression[__AMOUNT__(17, 20)];
    /**
     * 位图数据大小
     *
     * ui32_t
     *
     * 以字节为单位
     *
     * 若 biCompression == BI_RGB，biSizeImage 可能为 0。
     *
     * biWidth * biHeight * biBitCount / CHAR_BIT
    **/
    ui08_t biSizeImage[__AMOUNT__(21, 24)];
    /**
     * 位图的水平大小
     *
     * si32_t
     *
     * 以 pixel/meter 为单位
    **/
    ui08_t biXPelsPerMeter[__AMOUNT__(25, 28)];
    /**
     * 位图的竖直大小
     *
     * si32_t
     *
     * 以 pixel/meter 为单位
    **/
    ui08_t biYPelsPerMeter[__AMOUNT__(29, 32)];
    /**
     * 颜色列表中实际被使用颜色的数目
     *
     * ui32_t
    **/
    ui08_t biClrUsed[__AMOUNT__(33, 36)];
    /**
     * 显示位图需要颜色种类的数目
     *
     * ui32_t
    **/
    ui08_t biClrImportant[__AMOUNT__(37, 40)];
};

struct rgb
{
    /**
     * 蓝色的值
     *
     * ui08_t
    **/
    ui08_t rgbBlue[__AMOUNT__(1, 1)];
    /**
     * 绿色的值
     *
     * ui08_t
    **/
    ui08_t rgbGreen[__AMOUNT__(2, 2)];
    /**
     * 红色的值
     *
     * ui08_t
    **/
    ui08_t rgbRed[__AMOUNT__(3, 3)];
    /**
     * 保留
     *
     * ui08_t
     *
     * 必须是 0
    **/
    ui08_t rgbReserved[__AMOUNT__(4, 4)];
};

struct color_field
{
    ui08_t rField[__AMOUNT__(1, 4)];
    ui08_t gField[__AMOUNT__(5, 8)];
    ui08_t bField[__AMOUNT__(9, 12)];
};

/**
 * 位图
**/
struct bitmap
{
    /**
     * 位图文件路径
    **/
    char * path;
    /**
     * 位图文件起始地址
    **/
    ui08_t * addr;
    /**
     * 位图文件大小
    **/
    ui_t size;
    /**
     * 位图文件头部区域地址
    **/
    struct bitmap_head * head;
    /**
     * 位图文件信息区域地址
    **/
    struct bitmap_info * info;
    /**
     * 颜色列表区域地址
    **/
    struct rgb * color_table;
    /**
     * 颜色掩码区域地址
    **/
    struct color_field * color_field;
    /**
     * 颜色深度
    **/
    ui_t color_depth;
    /**
     * 位图文件数据区域地址
    **/
    ui08_t * data;
};


/**
 * 图形设备
**/
struct graphics_device
{
    /**
     * 屏幕结构
    **/
    struct screen screen;

    /**
     * 用户工作区域
    **/
    struct rectangle rectangle;

    /**
     * 颜色
    **/
    struct color color;

    /**
     * 字体类型
    **/
    si_t font;
};

/**
 * 获得系统中各个颜色分量的最大值
 *
 * 获得颜色分两的最大值，结果存放到 result
 *
 * @param result 存放颜色限制的地址
 *
 * @return 成功返回 0，失败返回 -1。
**/
extern si_t engine_get_color_limit(struct color_limit * result);


extern si_t engine_graphics_device_init(si_t x_axis, si_t y_axis, si_t x_size, si_t y_size, si_t r, si_t g, si_t b, si_t a, si_t font);

/**
 * 释放图形设备
 *
 * @param graphics_device_handler 图形设备描述符
 *
 * @return 0
**/
extern si_t engine_graphics_device_exit(si_t graphics_device_handle);

/**
 * 清空屏幕
 *
 * @param graphics_device_handler 图形设备描述符
 *
 * @return 0
**/
extern si_t engine_clear(si_t graphics_device_handle);

/**
 * 将屏幕清空成黑色
 *
 * @param graphics_device_handler 图形设备描述符
 *
 * @return 0
**/
extern si_t engine_clear_black(si_t graphics_device_handle);

/**
 * 将屏幕清空成白色
 *
 * @param graphics_device_handler 图形设备描述符
 *
 * @return 0
**/
extern si_t engine_clear_white(si_t graphics_device_handle);

/**
 * 绘制一个点
 *
 * 在 s 所指的屏幕上绘制 c 所指颜色的点（x，y）
 *
 * @param graphics_device_handler 图形设备描述符
 * @param x 点的横坐标
 * @param y 点的纵坐标
 *
 * @return 0
**/
extern si_t engine_draw_point(si_t graphics_device_handle, si_t x, si_t y);

/**
 * 绘制一条直线
 *
 * 在 s 所指的屏幕上绘制 c 所指颜色的通过点（x1，y1）和点（x2，y2）的直线
 *
 * @param graphics_device_handler 图形设备描述符
 * @param x1 第一个点的横坐标
 * @param y1 第一个点的纵坐标
 * @param x2 第二个点的横坐标
 * @param y2 第二个点的纵坐标
 *
 * @return 0
**/
extern si_t engine_draw_line(si_t graphics_device_handle, si_t x1, si_t y1, si_t x2, si_t y2);

/**
 * 绘制一个矩形
 *
 * 在 s 所指的屏幕上绘制 c 所指颜色的顶点坐标为（x，y）宽度为 xsize 高度为 ysize 的矩形
 *
 * @param graphics_device_handler 图形设备描述符
 * @param x_axis 矩形顶点的横坐标
 * @param y_axis 矩形顶点的竖坐标
 * @param x_size 矩形的宽度
 * @param y_size 矩形的高度
 *
 * @return 0
**/
extern si_t engine_draw_rectangle(si_t graphics_device_handle, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size);

/**
 * 绘制一个多变形
 *
 * 在 s 所指的屏幕上绘制 c 所指颜色的通过多个点的多边形
 *
 * @param graphics_device_handler 屏幕结构体指针
 * @param p 点的结构体数组
 * @param point_count 点的个数
 *
 * @return 0
**/
extern si_t engine_draw_polygon(si_t graphics_device_handle, struct point * p, si_t point_count);

/**
 * 绘制一个圆
 *
 * 在 s 所指的屏幕上绘制 c 所指颜色的圆心为（x，y）半径为 radius 的圆
 *
 * @param graphics_device_handler 屏幕结构体指针
 * @param x 圆心的横坐标
 * @param y 圆心的纵坐标
 * @param radius 圆的半径
 *
 * @return 0
**/
extern si_t engine_draw_circle(si_t graphics_device_handle, si_t x, si_t y, si_t radius);

/**
 * 绘制一个椭圆
 *
 * 在 s 所指的屏幕上绘制 c 所指颜色的中心为（x，y）轴为 a、b 的椭圆
 *
 * @param graphics_device_handler 图形设备描述符
 * @param x 椭圆中心的横坐标
 * @param y 椭圆中心的纵坐标
 * @param a 椭圆的第一根轴
 * @param b 椭圆的第二根轴
 *
 * @return 0
**/
extern si_t engine_draw_ellipse(si_t graphics_device_handle, si_t x, si_t y, si_t a, si_t b);

/**
 * 绘制 X
 *
 * @param graphics_device_handler 图形设备描述符
 * @param x 中心横坐标
 * @param y 中心纵坐标
 * @param a 大小
 * @param b
 *
 * @return 0
**/
extern si_t engine_draw_x(si_t graphics_device_handle, si_t x, si_t y, si_t a, si_t b);

/**
 * 绘制箭头
 *
 * @param graphics_device_handler 图形设备描述符
 * @param x 中心横坐标
 * @param y 中心纵坐标
 * @param size 大小
 * @param direction 1 - 向上，2 - 向下，3 - 向左，4 - 向右。
 *
 * @return 0
**/
extern si_t engine_draw_arrow(si_t graphics_device_handle, si_t x, si_t y, si_t size, si_t direction);

/**
 * 绘制图片 暂时根据图片名称的后缀名来决定图形类型
 *
 * @param graphics_device_handler 图形设备描述符
 * @param path 图片路径
 * @param align 对齐方式
 *
 * @return 成功返回0 否则返回-1
 **/
extern si_t engine_draw_img(si_t graphics_device_handle, char* path, si_t align);

/**
 * draw bitmap
 *
 * @param graphics_device_handler: pointer to graph device
 * @param path: path of bitmap
 * @param align: how picture aligan, both horizontal and vertical, see also ALIGN_TYPE
 *
 * @return 0
 **/
extern si_t engine_draw_bitmap(si_t graphics_device_handle, char* path, si_t align);

/**
 * 填充一个矩形
 *
 * 在 s 所指的屏幕上填充 c 所指颜色的顶点坐标为（x，y）宽度为 xsize 高度为 ysize 的矩形
 *
 * @param graphics_device_handler 图形设备描述符
 * @param xaxis 矩形顶点的横坐标
 * @param yaxis 矩形顶点的竖坐标
 * @param xsize 矩形的宽度
 * @param ysize 矩形的高度
 *
 * @return 0
**/
extern si_t engine_fill_rectangle(si_t graphics_device_handle, si_t xaxis, si_t yaxis, si_t xsize, si_t ysize);

/**
 * 填充一个圆
 *
 * 在 s 所指的屏幕上填充 c 所指颜色的圆心为（x，y）半径为 radius 的圆
 *
 * @param graphics_device_handler 图形设备描述符
 * @param x 圆心的横坐标
 * @param y 圆心的纵坐标
 * @param radius 圆的半径
 *
 * @return 0
**/
extern si_t engine_fill_circle(si_t graphics_device_handle, si_t x, si_t y, si_t radius);

/**
 * 填充一个椭圆
 *
 * 在 s 所指的屏幕上填充 c 所指颜色的中心为（x，y）轴为 a、b 的椭圆
 *
 * @param graphics_device_handler 图形设备描述符
 * @param x 椭圆中心的横坐标
 * @param y 椭圆中心的纵坐标
 * @param a 椭圆的第一根轴
 * @param b 椭圆的第二根轴
 *
 * @return 0
**/
extern si_t engine_fill_ellipse(si_t graphics_device_handle, si_t x, si_t y, si_t a, si_t b);

/**
 * 填充 X
 *
 * @param graphics_device_handler 图形设备描述符
 * @param x 中心横坐标
 * @param y 中心纵坐标
 * @param a 大小
 * @param b
 *
 * @return 0
**/
extern si_t engine_fill_x(si_t graphics_device_handle, si_t x, si_t y, si_t a, si_t b);

/**
 * 填充箭头
 *
 * @param graphics_device_handler 图形设备描述符
 * @param x 中心横坐标
 * @param y 中心纵坐标
 * @param size 大小
 * @param direction 1 - 向上，2 - 向下，3 - 向左，4 - 向右。
 *
 * @return 0
**/
extern si_t engine_fill_arrow(si_t graphics_device_handle, si_t x, si_t y, si_t size, si_t direction);

/**
 * 区域拷贝
**/
extern si_t engine_cpy_area(si_t graphics_device_handle, si_t mode, si_t dst_x, si_t dst_y, si_t src_x, si_t src_y, si_t x_size, si_t y_size);

/**
 * 显示字符串
 *
 * 在 s 所指的屏幕上显示 c 所指颜色的字体为 font 左上角坐标为（x，y）的字符串 str
 *
 * @param graphics_device_handler 图形设备描述符
 * @param x 字符串左上角横坐标
 * @param y 字符串左上角纵坐标
 * @param str 要显示的字符串
 * @param length 字符串的长度
 *
 * @return 0
**/
extern si_t engine_show_text(si_t graphics_device_handle, si_t x, si_t y, char * str, si_t length);

/**
 * 获得字体的宽度
 *
 * @param graphics_device_handler 图形设备描述符
 *
 * @return 字体宽度
**/
extern si_t engine_get_font_width(si_t graphics_device_handle);

/**
 * 获得字体的高度
 *
 * @param graphics_device_handler 图形设备描述符
 *
 * @return 字体高度
**/
extern si_t engine_get_font_height(si_t graphics_device_handle);

/**
 * 刷新屏幕
 *
 * @param graphics_device_handler 图形设备描述符
 *
 * @return 0
**/
extern si_t engine_update(si_t graphics_device_handle);

/**
 * 设置图形设备颜色
 *
 * @param graphics_device_handler 图形设备描述符
 * @param r 红色分量
 * @param g 绿色分量
 * @param b 蓝色分量
 * @param a 透明分量
 *
 * @return 0
 **/
extern si_t engine_set_color(si_t graphics_device_handle, si_t r, si_t g, si_t b, si_t a);

/**
 * 设置图形设备区域
 *
 * @param graphics_device_handler 图形设备描述符
 * @param x 矩形顶点的横坐标
 * @param y 矩形顶点的竖坐标
 * @param width 矩形的宽度
 * @param width 矩形的高度
 *
 * @return 0
 **/
extern si_t engine_set_area(si_t graphics_device_handle, si_t x, si_t y, si_t width, si_t height);

/**
 * 设置图形设备字体
 *
 * @param graphics_device_handler 图形设备描述符
 * @param font 字体
 *
 * @return 0
 **/
extern si_t engine_set_font(si_t graphics_device_handle, si_t font);

/**
 * 获取图形设备颜色
 *
 * @param graphics_device_handler 图形设备描述符
 * @param c 颜色
 *
 * @return 0
 **/
extern struct color* engine_get_color(si_t graphics_device_handle);

/**
 * 获取图形设备区域
 *
 * @param graphics_device_handler 图形设备描述符
 * @param rec 区域
 *
 * @return 0
 **/
extern struct rectangle* engine_get_area(si_t graphics_device_handle);

# endif
