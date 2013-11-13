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

# ifndef _LIST_H_
# define _LIST_H_ 1

# include <base_type.h>

/**
 * 链表节点结构
**/
struct list_node
{
    /**
     * 前一个节点指针
    **/
    struct list_node * prev;

    /**
     * 后一个节点指针
    **/
    struct list_node * next;

    /**
     * 节点数据存储地址
    **/
    void * data;
};

/**
 * 链表结构
**/
struct list
{
    /**
     * 根节点
     *
     * node 的 next 指向链表的头部节点
     *
     * node 的 prev 指向链表的尾部节点
     *
     * node 的 data 保存链表节点的个数
    **/
    struct list_node node;
};

/**
 * 申请链表
 *
 * @param l 链表指针
 *
 * @return 0
**/
extern 
si_t 
list_init(struct list * l);

/**
 * 释放链表
 *
 * @param l 链表指针
 *
 * @return 0
**/
extern
si_t
list_exit
(struct list * l);

/**
 * 获得链表元素个数
 *
 * @param l 链表指针
 *
 * @return 链表元素个数
**/
extern
ui_t
list_size
(struct list * l);

/**
 * 判断链表是否空
 *
 * @param l 链表指针
 *
 * @return 空返回 1，否则返回 0。
**/
extern
si_t
list_empty
(struct list * l);

/**
 * 获得链表头部节点地址
 *
 * @param l 链表指针
 *
 * @return 链表头部节点地址
**/
extern
void *
list_begin
(struct list * l);

/**
 * 获得链表尾部节点地址
 *
 * @param l 链表指针
 *
 * @return 链表尾部节点地址
**/
extern
void *
list_end
(struct list * l);

/**
 * 获得链表头部元素
 *
 * @param l 链表指针
 *
 * @return 链表头部元素的地址
**/
extern
void *
list_front
(struct list * l);

/**
 * 获得链表尾部元素
 *
 * @param l 链表指针
 *
 * @return 链表尾部元素的地址
**/
extern
void *
list_back
(struct list * l);

/**
 * 对链表中满足 func 的元素进行计数
 *
 * @param l 链表指针
 * @param func 条件函数
 *
 * @return 满足 func 的元素个数
**/
extern
ui_t
list_count
(struct list * l,
 si_t (* func)(void *));

/**
 * 在链表中查找元素
 *
 * 在 l 所指链表中查找第一个满足 func 的元素
 * 链表无须有序
 *
 * @param l 链表指针
 * @param func 元素比较函数指针
 *
 * @return 返回链表中第一个满足 func 的元素所在节点的指针，如果没有返回 NULL。
**/
extern
struct list_node *
list_find
(struct list * l,
 si_t (* func)(void *));

/**
 * 在链表中查找可以插入某个元素的第一个位置
 * XXX: Operation doesn't match function name
 *
 * 查找可以插入 data 的第一个位置
 * 也就是第一个大于或者等于 data 的元素所在位置
 *
 * @param l 链表指针
 * @param data 元素指针
 * @param comp 元素比较函数指针
 *
 * @return 返回链表中可以插入 data 的第一个位置
**/
extern
void *
list_lower_bound
(struct list * l,
 void * data,
 si_t (* comp)(void *, void *));

/**
 * 在链表中查找可以插入某个元素的最后一个位置
 * XXX: Operation doesn't match function name
 *
 * 查找可以插入 data 的最后一个位置
 * 也就是第一个大于 data 的元素所在位置
 *
 * @param l 链表指针
 * @param data 元素指针
 * @param comp 元素比较函数指针
 *
 * @return 返回链表中可以插入 data 的最后一个位置
**/
extern
void *
list_upper_bound
(struct list * l,
 void * data,
 si_t (* comp)(void *, void *));

/**
 * 向链表头部添加元素
 *
 * @param l 链表指针
 * @param data 元素指针
 * @param datasize 元素大小
 *
 * @return 0
**/
extern
si_t
list_push_front
(struct list * l,
 void * data,
 ui_t datasize);

/**
 * 向链表尾部添加元素
 *
 * @param l 链表指针
 * @param data 元素指针
 * @param datasize 元素大小
 *
 * @return 0
**/
extern
si_t
list_push_back
(struct list * l,
 void * data,
 ui_t datasize);

/**
 * 从链表头部删除元素
 *
 * @param l 链表指针
 *
 * @return 0
**/
extern
si_t
list_pop_front
(struct list * l);

/**
 * 从链表尾部删除元素
 *
 * @param l 链表指针
 *
 * @return 0
**/
extern
si_t
list_pop_back
(struct list * l);

/**
 * 向链表插入元素
 *
 * 在位于 l 所指链表的 position 处插入元素 data
 *
 * @param l 链表指针
 * @param position 链表节点指针
 * @param data 元素指针
 * @param datasize 元素大小
 *
 * @return 0
**/
extern
si_t
list_insert
(struct list * l,
 struct list_node * position,
 void * data,
 ui_t datasize);

/**
 * 从链表删除元素
 *
 * 将位于 l 所指链表的 position 处的元素删除
 *
 * @param l 链表指针
 * @param position 链表节点指针
 *
 * @return 0
**/
extern
si_t
list_erase
(struct list * l,
 struct list_node * position);

/**
 * 清空链表
 *
 * @param l 链表指针
 *
 * @return 0
**/
extern
si_t
list_clear
(struct list * l);

/**
 * 对链表中的每个元素执行操作
 *
 * @param l 链表指针
 * @param function 操作元素的函数指针
 *
 * @return 0
**/
extern
si_t
list_for_each
(struct list * l,
 si_t (* function)(void *));

#define list_for_each_macro(pos, list) \
    for((pos)=(list)->node.next; (pos)!=&(list)->node; (pos)=(pos)->next)

#define list_for_each_r_macro(pos, list) \
    for((pos)=(list)->node.prev; (pos)!=&(list)->node; (pos)=(pos)->prev)

/**
 * 找到链表的第idx个元素的数据
 *
 * @param l 链表指针
 * @param idx 要找的元素下标（从0开始）
 *
 * @return 找到的元素包含的数据指针
**/
extern
addr_t
list_element_at
(struct list *l, ui_t idx);

# endif
