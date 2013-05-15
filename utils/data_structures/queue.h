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

# ifndef _QUEUE_H_
# define _QUEUE_H_ 1

# include "list.h"

/**
 * 队列
**/
struct queue
{
    /**
     * 容器
    **/
    struct list container;
};

/**
 * 申请队列
 *
 * @param q 队列指针
 *
 * @return 0
**/
extern
si_t
queue_init
(struct queue * q);

/**
 * 释放队列
 *
 * @param q 队列指针
 *
 * @return 0
**/
extern
si_t
queue_exit
(struct queue * q);

/**
 * 获得队列中元素的个数
 *
 * @param q 队列指针
 *
 * @return 队列中元素的个数
**/
extern
ui_t
queue_size
(struct queue * q);

/**
 * 判断队列是否空
 *
 * @param q 队列指针
 *
 * @return 空返回 1，否则返回 0。
**/
extern
si_t
queue_empty
(struct queue * q);

/**
 * 获得队列头部元素
 *
 * @param q 队列指针
 *
 * @return 队列头部元素的地址，如果没有返回 NULL。
**/
extern
void *
queue_front
(struct queue * q);

/**
 * 获得队列尾部元素
 *
 * @param q 队列指针
 *
 * @return 队列尾部元素的地址，如果没有返回 NULL。
**/
extern
void *
queue_back
(struct queue * q);

/**
 * 向队列尾部添加元素
 *
 * @param q 队列指针
 * @param data 元素指针
 * @param data_size 元素大小
 *
 * @return 0
**/
extern
si_t
queue_push
(struct queue * q,
 void * data,
 ui_t data_size);

/**
 * 从队列头部删除元素
 *
 * @param q 队列指针
 *
 * @return 0
**/
extern
si_t
queue_pop
(struct queue * q);

# endif
