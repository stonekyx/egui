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

# ifndef _STACK_H_
# define _STACK_H_ 1

# include "vector.h"

/**
 * 栈
**/
struct stack
{
    /**
     * 容器
    **/
    struct vector container;
};

/**
 * 申请栈
 *
 * @param s 栈指针
 *
 * @return 0
**/
extern
si_t
stack_init
(struct stack * s);

/**
 * 释放栈
 *
 * @param s 栈指针
 *
 * @return 0
**/
extern
si_t
stack_exit
(struct stack * s);

/**
 * 获得栈中元素的个数
 *
 * @param s 栈指针
 *
 * @return 栈中元素的个数
**/
extern
ui_t
stack_size
(struct stack * s);

/**
 * 判断栈是否空
 *
 * @param s 栈指针
 *
 * @return 空返回 1，否则返回 0。
**/
extern
si_t
stack_empty
(struct stack * s);

/**
 * 获得栈顶元素
 *
 * @param s 栈指针
 *
 * @return 栈顶部元素的地址，如果没有返回 NULL。
**/
extern
void *
stack_top
(struct stack * s);

/**
 * 向栈顶添加元素
 *
 * @param s 栈指针
 * @param data 元素指针
 * @param datasize 元素大小
 *
 * @return 0
**/
extern
si_t
stack_push
(struct stack * s,
 void * data,
 ui_t datasize);

/**
 * 从栈顶删除元素
 *
 * @param s 栈指针
 *
 * @return 0
**/
extern
si_t
stack_pop
(struct stack * s);

# endif
