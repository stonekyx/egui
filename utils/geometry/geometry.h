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

# ifndef _GEOMETRY_H_
# define _GEOMETRY_H_ 1

# include <base_type.h>

/**
 * 点
**/
struct point
{
    /** 横坐标 **/
    si_t x;
    
	/** 纵坐标 **/
    si_t y;
};

/**
 * @brief 填充一个点
 * 每次单个赋值好麻烦，不如包装成一个setter
 *
 * @param p 待填充的点
 * @param x 横坐标
 * @param y 纵坐标
 **/
extern void point_set(struct point* p, si_t x, si_t y);

/**
 * 线
**/
struct line
{
    /** 第一个点的横坐标 **/
    si_t x_1;
    
	/** 第一个点的纵坐标 **/
    si_t y_1;
    
	/** 第二个点的横坐标 **/
    si_t x_2;
   
	/** 第二个点的纵坐标 **/
    si_t y_2;
};


/**
 * 矩形
**/
struct rectangle
{
    /** 左上角横坐标 **/
    si_t x;
    
	/** 左上角竖坐标 **/
    si_t y;
    
	/** 矩形的宽度 **/
    si_t width;
    
	/** 矩形的高度 **/
    si_t height;

};

/**
 * @brief 填充一个rectangle 即一个setter
 * rectangle是常用几何图形，每次一个个赋值太麻烦了，干脆封装成一个函数了
 *
 * @param r 待填充的矩形
 * @param x 横坐标
 * @param y 纵坐标
 * @param width 长
 * @param height 宽
 **/
extern void rectangle_set(struct rectangle* r, si_t x, si_t y, si_t width, si_t height);

/**
 * @brief 移动一个矩形 即坐标+=偏移量
 *
 * @param r 矩形
 * @param x_offset 横向偏移
 * @param y_offset 纵向偏移
 **/
extern void rectangle_move(struct rectangle* r, si_t x_offset, si_t y_offset);

/**
 * @brief 区域的并集 p ∪ q
 *
 * @param p 第一个区域指针
 * @param q 第二个区域指针
 * @param result 存放结果的指针
 *
 * @return 0
**/
extern si_t area_union (struct rectangle * p, struct rectangle * q, struct rectangle * result);

/**
 * @brief 区域的交集 p ∩ q
 *
 * @param p 第一个区域指针
 * @param q 第二个区域指针
 * @param result 存放结果的指针
 *
 * @return 有至少一个像素的交集返回 0，没有返回 -1。
**/
extern si_t area_intersection (struct rectangle * p, struct rectangle * q, struct rectangle * result);

/**
 * @brief 区域的差集 p — q
 *
 * @param p 第一个区域指针
 * @param q 第二个区域指针
 * @param result 存放结果的指针
 *
 * @return 成功返回 0，失败返回 -1。
**/
extern si_t area_difference (struct rectangle * p, struct rectangle * q, struct rectangle * result);

/**
 * @brief 判断点是否在区域中
 *
 * @param p 点
 * @param r 区域
 *
 * @return 点在区域中返回1 否则返回0
 **/
extern si_t is_point_in_area(struct point* p, struct rectangle* r);

/**
 * @brief 保证点不出区域 若点在区域外则拉回区域边缘
 *
 * @param p 点
 * @param r 区域
 **/
extern void ensure_point_in_area(struct point* p, struct rectangle* r);

/* ----------- Bresenham's line algorithm ----------- */

struct bresenham_iterator;

/**
 * @brief 初始化Bresenham算法，返回一个用于循环直线上点的枚举器。
 * Bresenham算法用于近似绘制任意两点之间的直线。
 *
 * @param x1, y1 起点
 * @param x2, y2 终点
 * @param cb 回调函数
 **/
extern struct bresenham_iterator *
bresenham_init(si_t x1, si_t y1, si_t x2, si_t y2);

/**
 * @brief 取Bresenham算法得出的下一个点。每次调用时先返回一个可用点，
 * 再进行下一步循环。
 *
 * @param it 枚举器
 * @return 非空的点指针，或者当循环已经结束时返回NULL
 **/
extern struct point *bresenham_next(struct bresenham_iterator *it);

/**
 * @brief 类似list的循环操作，可对点列上每个元素进行任意操作。
 *
 * @param pos 调用者提供，用于循环的点指针(struct point *)
 * @param it 调用者提供，指向枚举器的指针，调用者不需要初始化
 * @param x1, y1 起点
 * @param x2, y2 终点
 **/
#define bresenham_for_each(pos,it,x1,y1,x2,y2) \
    for(it = bresenham_init((x1),(y1),(x2),(y2)); \
            ((pos)=bresenham_next(it)); )

# endif
