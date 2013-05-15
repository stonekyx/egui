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

# ifndef _BIT_OPS_H_
# define _BIT_OPS_H_ 1

# include <base_type.h>

/**
 * 设置数据的某一位为 1
 *
 * 从 0 开始计数
 *
 * @param addr 数据的地址
 * @param n 要设置的位
 *
 * @return 0
**/
# define SET_BIT(addr, n) \
    (((ui08_t *)(addr))[(n) >> 3] |= (0X1 << ((n) & 0X7)))

/**
 * 设置数据的某一位为 0
 *
 * 从 0 开始计数
 *
 * @param addr 数据的地址
 * @param n 要设置的位
 *
 * @return 0
**/
# define CLR_BIT(addr, n) \
    (((ui08_t *)(addr))[(n) >> 3] &= ~(0X1 << ((n) & 0X7)))

/**
 * 获得数据的某一位
 *
 * 从 0 开始计数
 *
 * @param addr 数据的地址
 * @param n 要获得的位
 *
 * @return 第 n 位的值
**/
# define GET_BIT(addr, n) \
    ((((ui08_t *)(addr))[(n) >> 3] >> ((n) & 0X7)) & 0X1)

/**
 * 数据算术右移
 *
 * 标准并没有规定有符号右移采用哪种方式（算术右移、逻辑右移）。
 * 但是实际上，几乎所有的编译器都假设有符号数使用算术右移。
 * 许多程序员也都假设使用这种右移。
 *
 * @param addr 数据的地址
 * @param n 数据的大小
 * @param shift 向右移动的位数
 *
 * @return 0
**/
extern
si_t
arithmetic_r_shift
(void * addr,
 ui_t n,
 ui_t shift);

/**
 * 数据算术左移
 *
 * @param addr 数据的地址
 * @param n 数据的大小
 * @param shift 向左移动的位数
 *
 * @return 0
**/
extern
si_t
arithmetic_l_shift
(void * addr,
 ui_t n,
 ui_t shift);

/**
 * 数据逻辑右移
 *
 * @param addr 数据的地址
 * @param n 数据的大小
 * @param shift 向右移动的位数
 *
 * @return 0
**/
extern
si_t
logical_r_shift
(void * addr,
 ui_t n,
 ui_t shift);

/**
 * 数据逻辑左移
 *
 * @param addr 数据的地址
 * @param n 数据的大小
 * @param shift 向左移动的位数
 *
 * @return 0
**/
extern
si_t
logical_l_shift
(void * addr,
 ui_t n,
 ui_t shift);

/**
 * 数据循环右移
 *
 * @param addr 数据的地址
 * @param n 数据的大小
 * @param rotate 向右循环移动的位数
 *
 * @return 0
**/
extern
si_t
r_rotate
(void * addr,
 ui_t n,
 ui_t rotate);

/**
 * 数据循环左移
 *
 * @param addr 数据的地址
 * @param n 数据的大小
 * @param rotate 向左循环移动的位数
 *
 * @return 0
**/
extern
si_t
l_rotate
(void * addr,
 ui_t n,
 ui_t rotate);

# endif
