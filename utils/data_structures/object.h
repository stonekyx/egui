/**
 * @file object.h
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

# ifndef _OBJECT_H_
# define _OBJECT_H_ 1

# include "base_type.h"

/*
    类似于
    struct object
    {
        // 指向父窗口部件
        struct object * parent;
        // 指向兄弟窗口部件
        struct object * sibling;
        // 指向孩子窗口部件
        struct object * children;
        // 名字
        char * name;
        // ID
        si_t id;
    };
*/
/**
 * 对象结构体
**/
# define OBJECT_DEFINITION \
    /**
     * 父节点
    **/ \
    struct object * parent; \
    \
    /**
     * 左子节点
    **/ \
    struct object * lchild; \
    \
    /**
     * 右子节点
    **/ \
    struct object * rchild; \
    \
    /**
     * 类的名称
    **/ \
    char * name; \
    \
    /**
     * 类的 ID
    **/ \
    si_t id;

struct object
{
    OBJECT_DEFINITION
};

/**
 * 将地址强制转换成 struct object *
**/
# define OBJECT_POINTER(addr) ((struct object *)(addr))

/**
 * @brief 申请对象
 *
 * @details
 *
 * @return 申请成功返回对象的指针，否则返回 NULL。
**/
extern
void *
object_init
(si_t id);

/**
 * @brief 使用后清理对象结构体
 *
 * @details
 *
 * @param obj 对象结构体地址
 *
 * @return 0
**/
extern
si_t
object_exit
(struct object * obj);

/**
 * @brief 释放对象树
 *
 * @details 释放以 root 为根节点的对象树，root 节点也会被释放
 *
 * @param root 对象树的根节点
 *
 * @return 0
**/
extern
si_t
object_tree_free
(struct object * root);

/**
 * 获得某个子树最左边的节点
 *
 * @param node 树节点结构指针
 *
 * @return 子树最左边节点地址
**/
extern
void *
object_tree_l_most_node
(struct object * node);

/**
 * 获得某个子树最右边的节点
 *
 * @param node 树节点结构指针
 *
 * @return 子树最右边节点地址
**/
extern
void *
object_tree_r_most_node
(struct object * node);

/**
 * 以 l n r 的顺序找 node 的下一个节点
 *
 * @param tree 树的指针
 * @param node
 *
 * @return 如果有，那么返回下一个节点的地址；否则返回 NULL。
**/
extern
void *
object_tree_iterator_increment
(struct object * tree,
 struct object * node);

/**
 * 以 l n r 的顺序找 node 的上一个节点
 *
 * @param tree 树的指针
 * @param node
 *
 * @return 如果有，那么返回上一个节点的地址；否则返回 NULL。
**/
extern
void *
object_tree_iterator_decrement
(struct object * tree,
 struct object * node);

/**
 * @brief 为 obj 对象添加新的子对象 child，添加后为最小的孩子
 *
 * @details 子控件只能在父窗口区域内显示。
 *
 * 如果要添加窗口类对象以及它的派生类对象
 * 请使用 application_add_window
 *
 * @param obj 父对象结构体地址，必须不为空
 * @param child 子对象结构体地址
 *
 * @return 0
**/
extern si_t object_attach_child (struct object * obj, struct object * child);

/**
 * @brief 为 obj 对象添加长子对象 child
 *
 * @details 子控件只能在父窗口区域内显示。
 *
 * 如果要添加窗口类对象以及它的派生类对象
 * 请使用 application_add_window
 *
 * @param obj 父对象结构体地址，必须不为空
 * @param child 子对象结构体地址
 *
 * @return 0
**/
extern si_t object_insert_child(struct object* parent, struct object* child);

/**
 * @brief 删除某个子对象以及它下面的对象
 *
 * @details
 * 如果要删除窗口类对象以及它的派生类对象
 * 请使用 application_del_window
 * 另外 edugui 中只有窗口类对象以及它的派生类对象可以作为顶层对象
 * 也就是说该函数不能用来删除顶层对象
 *
 * @param obj 对象结构体地址，obj 不能是顶层对象
 * @param destructor 清理的结构体指针
 *
 * @return 0
**/
extern si_t object_delete(struct object* object, void(*destructor)(void*));

/**
 * @brief 获得父对象
 *
 * @details
 *
 * @param obj 子对象结构体地址
 *
 * @return 如果有则返回父对象地址；如果没有则返回 NULL。
**/
extern void * object_parent(struct object * obj);

/**
 * @brief 变量对象，左-右-根的顺序
 * @param root 二叉树根节点 
 * @param action 遍历执行的函数
 **/
extern si_t object_tree_for_each(struct object * root, void(* action)(void *));

/**
 * @brief 将一个节点移到父节点的长子节点
 *
 * @param obj 待移动的节点
 **/
extern void object_move_first(struct object* obj);

/**
 * @brief 获得一个节点所在树的根节点
 *
 * @detail 即获得顶层窗口指针
 *
 * @return 跟节点指针
 **/
extern struct object* object_get_root(struct object* obj);

# endif
