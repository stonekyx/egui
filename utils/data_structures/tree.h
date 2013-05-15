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

# ifndef _TREE_H_
# define _TREE_H_ 1

# include <base_type.h>

/**
 * 树的节点
**/
struct tree_node
{
    /**
     * 父亲节点指针
    **/
    struct tree_node * parent;
    /**
     * 左子节点指针
    **/
    struct tree_node * lchild;
    /**
     * 右子节点指针
    **/
    struct tree_node * rchild;
    /**
     * 数据
    **/
    void * data;
};

/**
 * 树
**/
struct tree
{
    /**
     * 根节点的父亲节点
     *
     * node 的 lchild 最左边的节点
     *
     * node 的 rchild 最右边的节点
     *
     * node 的 parent 树的根节点
    **/
    struct tree_node node;
};

/**
 * 申请树
 *
 * @param t 树结构指针
 *
 * @return 0
**/
extern
si_t
tree_init
(struct tree * t);

/**
 * 释放树
 *
 * @param t 树结构指针
 *
 * @return 0
**/
extern
si_t
tree_exit
(struct tree * t);

/**
 * 获得树元素个数
 *
 * @param t 树指针
 *
 * @return 树元素个数
**/
extern
ui_t
tree_size
(struct tree * t);

/**
 * 判断树是否空
 *
 * @param t 树指针
 *
 * @return 空返回 1，否则返回 0。
**/
extern
si_t
tree_empty
(struct tree * t);

/**
 * 获得树的头部节点
 *
 * 获得最左边的节点
 *
 * @param t 树结构指针
 *
 * @return 最头部节点的地址
**/
extern
void *
tree_begin
(struct tree * t);

/**
 * 获得树的尾部节点
 *
 * 获得最右边的节点
 *
 * @param t 树结构指针
 *
 * @return 最尾部节点的地址
**/
extern
void *
tree_end
(struct tree * t);

/**
 * 对树中满足 comp 的元素进行计数
 *
 * @param t 树指针
 * @param data 元素地址
 * @param comp 比较函数
 *
 * @return 满足 comp 的元素个数
**/
extern
ui_t
tree_count
(struct tree * t,
 void * data,
 si_t (* comp)(void *, void *));

/**
 * 在树中查找元素
 *
 * 在 t 所指树中查找第一个满足 comp 的元素
 *
 * @param t 树指针
 * @param data 元素指针
 * @param comp 元素比较函数指针
 *
 * @return 返回树中第一个满足 comp 的元素所在节点的指针，如果没有返回 NULL。
**/
extern
void *
tree_find
(struct tree * t,
 void * data,
 si_t (* comp)(void *, void *));

/**
 * 在树中查找可以插入某个元素的第一个位置
 *
 * 查找可以插入 data 的第一个位置
 * 也就是第一个大于或者等于 data 的元素所在位置
 *
 * @param t 树指针
 * @param data 元素指针
 * @param comp 元素比较函数指针
 *
 * @return 返回树中可以插入 data 的第一个位置
**/
extern
void *
tree_lower_bound
(struct tree * t,
 void * data,
 si_t (* comp)(void *, void *));

/**
 * 在树中查找可以插入某个元素的最后一个位置
 *
 * 查找可以插入 data 的最后一个位置
 * 也就是第一个大于 data 的元素所在位置
 *
 * @param t 树指针
 * @param data 元素指针
 * @param comp 元素比较函数指针
 *
 * @return 返回树中可以插入 data 的最后一个位置
**/
extern
void *
tree_upper_bound
(struct tree * t,
 void * data,
 si_t (* comp)(void *, void *));

/**
 * 插入节点
 *
 * 将
 *
 * @param t 树结构指针
 * @param comp 元素比较函数
 * @param data 元素地址
 * @param datasize 元素大小
 *
 * @return 0
**/
extern
si_t
tree_insert
(struct tree * t,
 si_t (* comp)(void *, void *),
 void * data,
 ui_t datasize);

/**
 * 删除节点
 *
 * @param t 树结构指针
 * @param position 节点位置
 *
 * @return 0
**/
extern
si_t
tree_erase
(struct tree * t,
 struct tree_node * position);

/**
 * 删除某个节点的左子树
 *
 * @param t 树结构指针
 * @param position 节点位置
 *
 * @return 0
**/
extern
si_t
tree_erase_l_subtree
(struct tree * t,
 struct tree_node * position);

/**
 * 删除某个节点的右子树
 *
 * @param t 树结构指针
 * @param position 节点位置
 *
 * @return 0
**/
extern
si_t
tree_erase_r_subtree
(struct tree * t,
 struct tree_node * position);

/**
 * 清空树
 *
 * @param t 树指针
 *
 * @return 0
**/
extern
si_t
tree_clear
(struct tree * t);

/**
 * 对树中的每个元素执行操作
 *
 * @param t 树指针
 * @param function 操作元素的函数指针
 *
 * @return 0
**/
extern
si_t
tree_for_each
(struct tree * t,
 si_t (* function)(void *));

# endif
