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

# include <stdlib.h>
# include <string.h>

# include "tree.h"

/**
 * 获得某个子树最左边的节点
 *
 * @param tn 树节点结构指针
 *
 * @return 子树最左边节点地址
**/
static void *
__l_most_node__
(struct tree_node * tn)
{
    while(tn->lchild != NULL)
    {
        tn = tn->lchild;
    }

    return tn;
}

/**
 * 获得某个子树最右边的节点
 *
 * @param tn 树节点结构指针
 *
 * @return 子树最右边节点地址
**/
static void *
__r_most_node__
(struct tree_node * tn)
{
    while(tn->rchild != NULL)
    {
        tn = tn->rchild;
    }

    return tn;
}

# if 0
/**
 * 前序遍历根节点为某个节点的子树
 *
 * @param tn 节点地址
 * @param function 操作函数
 *
 * @return 0
**/
static si_t
__n_l_r_order__
(struct tree_node * tn,
 si_t (* function)(void *))
{
    if(tn != NULL)
    {
        function(tn->data);
        __n_l_r_order__(tn->lchild, function);
        __n_l_r_order__(tn->rchild, function);
    }

    return 0;
}

/**
 * 中序遍历根节点为某个节点的子树
 *
 * @param tn 节点地址
 * @param function 操作函数
 *
 * @return 0
**/
static si_t
__l_n_r_order__
(struct tree_node * tn,
 si_t (* function)(void *))
{
    if(tn != NULL)
    {
        __l_n_r_order__(tn->lchild, function);
        function(tn->data);
        __l_n_r_order__(tn->rchild, function);
    }

    return 0;
}

/**
 * 后序遍历根节点为某个节点的子树
 *
 * @param tn 节点地址
 * @param function 操作函数
 *
 * @return 0
**/
static si_t
__l_r_n_order__
(struct tree_node * tn,
 si_t (* function)(void *))
{
    if(tn != NULL)
    {
        __l_r_n_order__(tn->lchild, function);
        __l_r_n_order__(tn->rchild, function);
        function(tn->data);
    }

    return 0;
}
# endif

/**
 * 找 node 的下一个节点
 *
 * @param t 树的指针
 * @param node
 *
 * @return 如果有，那么返回下一个节点的地址；否则返回 NULL。
**/
static void *
__l_n_r_increment__
(struct tree * t,
 struct tree_node * node)
{
    /* 找出父节点 */
    struct tree_node * temp = node->parent;

    /* 有右子节点 */
    if(node->rchild != NULL)
    {
        /* 就向右走 */
        node = node->rchild;

        /* 以右子节点为根节点的子树的最左节点 */
        /* 一直往左走 */
        while(node->lchild != NULL)
        {
            node = node->lchild;
        }
    }
    /* 没有右子节点 */
    else
    {
        /* 节点是右子节点 */
        while(node == temp->rchild)
        {
            /* 一直上溯，直到不为右子节点。 */
            node = temp;
            temp = temp->parent;
        }

        /* 找根节点的下一个节点恰巧根节点没有右子节点 */
        if(node->rchild != temp)
        {
            node = temp;
        }
    }

    /* 下一个节点是 struct tree::node */
    /* 找的是最右边节点的下一个节点 */
    if(&(t->node) == node)
    {
        return NULL;
    }
    else
    {
        return node;
    }
}

# if 0
/**
 * 找 node 的上一个节点
 *
 * @param t 树的指针
 * @param node
 *
 * @return 如果有，那么返回上一个节点的地址；否则返回 NULL。
**/
static void *
__l_n_r_decrement__
(struct tree * t,
 struct tree_node * node)
{
    /* 找出父节点 */
    struct tree_node * temp = node->parent;

    /* 有作子节点 */
    if(node->lchild != NULL)
    {
        /* 就向左走 */
        node = node->lchild;

        /* 以左子节点为根节点的子树的最右节点 */
        /* 一直往右走 */
        while(node->rchild != NULL)
        {
            node = node->rchild;
        }
    }
    /* 没有左子节点 */
    else
    {
        /* 节点是左子节点 */
        while(node == temp->lchild)
        {
            /* 一直上溯，直到不为左子节点。 */
            node = temp;
            temp = temp->parent;
        }

        /* 找根节点的上一个节点恰巧根节点没有左子节点 */
        if(node->lchild != temp)
        {
            node = temp;
        }
    }

    /* 上一个节点是 struct tree::node */
    /* 找的是最左边节点的上一个节点 */
    if(&(t->node) == node)
    {
        return NULL;
    }
    else
    {
        return node;
    }
}
# endif

/**
 * 删除节点以及节点的左子树和节点的右子树
 *
 * @param t 树指针
 * @param tn 节点指针
 *
 * @return 0
**/
static si_t
__erase_subtree__
(struct tree * t,
 struct tree_node * tn)
{
    if(tn != NULL)
    {
        __erase_subtree__(t, tn->lchild);
        __erase_subtree__(t, tn->rchild);

        free(tn->data);
        free(tn);

        t->node.data = (void *)((ui_t)(t->node.data) - 1);
    }

    return 0;
}

/**
 * 删除没有子节点的节点
 *
 * @param t 树指针
 * @param tn 要删除的节点
 *
 * @return 0
**/
static si_t
__erase_node_with_0_child__
(struct tree * t,
 struct tree_node * tn)
{
    /* tn 是根节点 */
    if(tn == t->node.parent)
    {
        t->node.parent = NULL;

        /* 被删除节点是树的最左边节点 */
        /* 有必要更新 root->parent 的 lchild 成员 */
        t->node.lchild = NULL;

        /* 被删除节点是树的最右边节点 */
        /* 有必要更新 root->parent 的 rchild 成员 */
        t->node.rchild = NULL;
    }
    /* tn 不是根节点 */
    else
    {
        /* tn 是左子节点 */
        if(tn->parent->lchild == tn)
        {
            /* 设置父节点的 lchild */
            tn->parent->lchild = NULL;

            /* 被删除节点是树的最左边节点 */
            /* 有必要更新 root->parent 的 lchild 成员 */
            if(t->node.lchild == tn)
            {
                t->node.lchild = __l_most_node__(t->node.parent);
            }

            /* 被删除节点不可能是树的最右边节点 */
            /* 没有必要更新 root->parent 的 rchild 成员 */
        }
        /* tn 是右子节点 */
        else
        {
            /* 设置父节点的 rchild */
            tn->parent->rchild = NULL;

            /* 被删除节点不可能是树的最左边节点 */
            /* 没有必要更新 root->parent 的 lchild 成员 */

            /* 被删除节点可能是树的最右边节点 */
            /* 有必要更新 root->parent 的 rchild 成员 */
            if(t->node.rchild == tn)
            {
                t->node.rchild = __r_most_node__(t->node.parent);
            }
        }
    }

    /* 删除元素 */
    free(tn->data);
    /* 删除节点 */
    free(tn);

    return 0;
}

/**
 * 删除只有左子节点的节点
 *
 * @param t 树指针
 * @param tn 要删除的节点
 *
 * @return 0
**/
static si_t
__erase_node_with_l_child__
(struct tree * t,
 struct tree_node * tn)
{
    /* tn 是根节点 */
    if(tn == t->node.parent)
    {
        tn->lchild->parent = &(t->node);

        t->node.parent = tn->lchild;

        /* 被删除节点不可能是树的最左边节点 */
        /* 没有必要更新 root->parent 的 lchild 成员 */

        /* 被删除节点是树的最右边节点 */
        /* 有必要更新 root->parent 的 rchild 成员 */
        t->node.rchild = __r_most_node__(t->node.parent);
    }
    /* tn 不是根节点 */
    else
    {
        /* 设置左子节点 */
        tn->lchild->parent = tn->parent;

        /* tn 是左子节点 */
        if(tn->parent->lchild == tn)
        {
            /* 设置父节点的 lchild */
            tn->parent->lchild = tn->lchild;

            /* 被删除节点不可能是树的最左边节点 */
            /* 没有必要更新 root->parent 的 lchild 成员 */

            /* 被删除节点不可能是树的最右边节点 */
            /* 没有必要更新 root->parent 的 rchild 成员 */
        }
        /* tn 是右子节点 */
        else
        {
            /* 设置父节点的 rchild */
            tn->parent->rchild = tn->lchild;

            /* 被删除节点不可能是树的最左边节点 */
            /* 没有必要更新 root->parent 的 lchild 成员 */

            /* 被删除节点是树的最右边节点 */
            /* 有必要更新 root->parent 的 rchild 成员 */
            if(t->node.rchild == tn)
            {
                t->node.rchild = __r_most_node__(t->node.parent);
            }
        }
    }

    /* 删除元素 */
    free(tn->data);
    /* 删除节点 */
    free(tn);

    return 0;
}

/**
 * 删除只有右子节点的节点
 *
 * @param t 树指针
 * @param tn 要删除的节点
 *
 * @return 0
**/
static si_t
__erase_node_with_r_child__
(struct tree * t,
 struct tree_node * tn)
{
    /* tn 是根节点 */
    if(tn == t->node.parent)
    {
        tn->rchild->parent = &(t->node);

        t->node.parent = tn->rchild;

        /* 被删除节点是树的最左边节点 */
        /* 有必要更新 root->parent 的 lchild 成员 */
        t->node.lchild = __l_most_node__(t->node.parent);

        /* 被删除节点不可能是树的最右边节点 */
        /* 没有必要更新 root->parent 的 rchild 成员 */
    }
    /* tn 不是根节点 */
    else
    {
        /* 设置 tn 的右子节点 */
        tn->rchild->parent = tn->parent;

        /* tn 是左子节点 */
        if(tn->parent->lchild == tn)
        {
            /* 设置父节点的 lchild */
            tn->parent->lchild = tn->rchild;

            /* 被删除节点是树的最左边节点 */
            /* 有必要更新 root->parent 的 lchild 成员 */
            if(t->node.lchild == tn)
            {
                t->node.lchild = __l_most_node__(t->node.parent);
            }

            /* 被删除节点不可能是树的最右边节点 */
            /* 没有必要更新 root->parent 的 rchild 成员 */
        }
        /* tn 是右子节点 */
        else
        {
            /* 设置父节点的 rchild */
            tn->parent->rchild = tn->rchild;

            /* 被删除节点不可能是树的最左边节点 */
            /* 没有必要更新 root->parent 的 lchild 成员 */

            /* 被删除节点不可能是树的最右边节点 */
            /* 没有必要更新 root->parent 的 rchild 成员 */
        }
    }

    /* 删除元素 */
    free(tn->data);
    /* 删除节点 */
    free(tn);

    return 0;
}

/**
 * 删除既有左子节点又有右子节点的节点
 *
 * @param t 树指针
 * @param tn 要删除的节点
 *
 * @return 0
**/
static si_t
__erase_node_with_2_child__
(struct tree * t,
 struct tree_node * tn)
{
    struct tree_node * rsubtree_l_most;

    /* tn 右子树最左边的节点 */
    rsubtree_l_most = __l_most_node__(tn->rchild);

    /* tn 是根节点 */
    if(tn == t->node.parent)
    {
        t->node.parent = rsubtree_l_most;
    }
    /* tn 不是根节点 */
    else
    {
        /* tn 是左子节点 */
        if(tn->parent->lchild == tn)
        {
            tn->parent->lchild = rsubtree_l_most;
        }
        /* tn 是右子节点 */
        else
        {
            tn->parent->rchild = rsubtree_l_most;
        }
    }

    rsubtree_l_most->lchild = tn->lchild;
    tn->lchild->parent = rsubtree_l_most;
    if(tn->rchild != rsubtree_l_most)
    {
        rsubtree_l_most->rchild = tn->rchild;
        tn->rchild->parent = rsubtree_l_most;
    }

    if(rsubtree_l_most->parent->lchild == rsubtree_l_most)
    {
        rsubtree_l_most->parent->lchild = NULL;
    }
    else
    {
        rsubtree_l_most->parent->rchild = NULL;
    }

    rsubtree_l_most->parent = tn->parent;

    /* 被删除节点不可能是树的最左边节点 */
    /* 没有必要更新 root->parent 的 lchild 成员 *//*
    if(t->node.lchild == tn)
    {
        t->node.lchild = __l_most_node__(t->node.parent);
    }*/

    /* 被删除节点不可能是树的最右边节点 */
    /* 没有必要更新 root->parent 的 rchild 成员 *//*
    if(t->node.rchild == tn)
    {
        t->node.rchild = __r_most_node__(t->node.parent);
    }*/

    /* 删除元素 */
    free(tn->data);
    /* 删除节点 */
    free(tn);

    return 0;
}

si_t
tree_init
(struct tree * t)
{
    /* 初始化根节点的父节点 */
    t->node.parent = NULL;
    t->node.lchild = NULL;
    t->node.rchild = NULL;
    t->node.data = (void *)((ui_t)0);

    return 0;
}

si_t
tree_exit
(struct tree * t)
{
    tree_clear(t);

    return 0;
}

ui_t
tree_size
(struct tree * t)
{
    return (ui_t)(t->node.data);
}

si_t
tree_empty
(struct tree * t)
{
    if(t->node.parent == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void *
tree_begin
(struct tree * t)
{
    /* 获得最左边的节点 */
    return t->node.lchild;
}

void *
tree_end
(struct tree * t)
{
    /* 获得最右边的节点 */
    return t->node.rchild;
}

void *
tree_find
(struct tree * t,
 void * data,
 si_t (* comp)(void *, void *))
{
    struct tree_node * parent = &(t->node);
    struct tree_node * temp = t->node.parent;

    /* 空树 */
    if(temp == NULL)
    {
        return NULL;
    }

    while(temp != NULL)
    {
        /* data <= temp->data */
        if(!comp(temp->data, data))
        {
            parent = temp;
            temp = temp->lchild;
        }
        else
        {
            temp = temp->rchild;
        }
    }

    return comp(data, parent->data) ? NULL : parent;
}

void *
tree_lower_bound
(struct tree * t,
 void * data,
 si_t (* comp)(void *, void *))
{
    struct tree_node * parent = &(t->node);
    struct tree_node * temp = t->node.parent;

    /* 空树 */
    if(temp == NULL)
    {
        return NULL;
    }

    while(temp != NULL)
    {
        /* data <= temp->data */
        if(!comp(temp->data, data))
        {
            parent = temp;
            temp = temp->lchild;
        }
        else
        {
            temp = temp->rchild;
        }
    }

    return parent;
}

void *
tree_upper_bound
(struct tree * t,
 void * data,
 si_t (* comp)(void *, void *))
{
    struct tree_node * parent = &(t->node);
    struct tree_node * temp = t->node.parent;

    /* 空树 */
    if(temp == NULL)
    {
        return NULL;
    }

    while(temp != NULL)
    {
        /* data < temp->data */
        if(comp(data, temp->data))
        {
            parent = temp;
            temp = temp->lchild;
        }
        else
        {
            temp = temp->rchild;
        }
    }

    return parent;
}

/*/
 * 唯一地插入
 *     若 temp 在 node 的左子树，则 temp < node 即 comp(temp, node)。
 *     若 temp 在 node 的右子树，则 temp > node 即 comp(node, temp)。
 * 雷同地插入
 *     若 temp 在 node 的左子树，则 temp <= node 即 !comp(node, temp)。
 *     若 temp 在 node 的右子树，则 temp >= node 即 !comp(temp, node)。
/*/
si_t
tree_insert
(struct tree * t,
 si_t (* comp)(void *, void *),
 void * data,
 ui_t datasize)
{
    struct tree_node * temp = t->node.parent;
    struct tree_node * parent = NULL;

    /* 更新节点数目 */
    t->node.data = (void *)((ui_t)(t->node.data) + 1);

    /* 空树 */
    if(temp == NULL)
    {
        /* 设置根节点的父节点 */
        t->node.parent = (struct tree_node *)malloc(sizeof(struct tree_node));
        t->node.lchild = t->node.parent;
        t->node.rchild = t->node.parent;

        /* 设置根节点 */
        t->node.parent->parent = &(t->node);
        t->node.parent->lchild = NULL;
        t->node.parent->rchild = NULL;

        /* 向根节点插入元素 */
        t->node.parent->data = (void *)malloc(datasize);
        memcpy(t->node.parent->data, data, datasize);

        return 0;
    }

    /* 找到插入的位置 */
    while(temp != NULL)
    {
        parent = temp;

        /* data < temp->data */
        if(comp(data, temp->data))
        {
            /* 插到左子树 */
            temp = temp->lchild;
        }
        /* data >= temp->data */
        /* 雷同地插入时，插入到右子树。*/
        else
        {
            /* 插到右子树 */
            temp = temp->rchild;
        }
    }

    /* 插入到左边 */
    if(comp(data, parent->data))
    {
        /* 设置父节点 */
        parent->lchild = (struct tree_node *)malloc(sizeof(struct tree_node));

        /* 设置新节点 */
        parent->lchild->rchild = NULL;
        parent->lchild->lchild = NULL;
        parent->lchild->parent = parent;

        /* 向新节点插入元素 */
        parent->lchild->data = (void *)malloc(datasize);
        memmove(parent->lchild->data, data, datasize);

        /* 新插入节点是树的最左边节点 */
        /* 有必要更新 root->parent 的 lchild 成员 */
        if( t->node.lchild == parent)
        {
            t->node.lchild = __l_most_node__(t->node.parent);
        }

        /* 新插入节点不可能是树的最右边节点 */
        /* 没有必要更新 root->parent 的 rchild 成员 */
    }
    /* 插入到右边 */
    else
    {
        /* 设置父节点 */
        parent->rchild = (struct tree_node *)malloc(sizeof(struct tree_node));

        /* 设置新节点 */
        parent->rchild->rchild = NULL;
        parent->rchild->lchild = NULL;
        parent->rchild->parent = parent;

        /* 向新节点插入元素 */
        parent->rchild->data = (void *)malloc(datasize);
        memmove(parent->rchild->data, data, datasize);

        /* 新插入节点不可能是树的最左边节点 */
        /* 没有必要更新 root->parent 的 lchild 成员 */

        /* 新插入节点是树的最右边节点 */
        /* 有必要更新 root->parent 的 rchild 成员 */
        if(t->node.rchild == parent)
        {
            t->node.rchild = __r_most_node__(t->node.parent);
        }
    }

    return 0;
}

si_t
tree_erase
(struct tree * t,
 struct tree_node * position)
{
    /* 更新节点数目 */
    t->node.data = (void *)((ui_t)(t->node.data) - 1);

    /* position 没有子节点 */
    if(position->lchild == NULL && position->rchild == NULL)
    {
        __erase_node_with_0_child__(t, position);
    }
    /* position 只有右子节点 */
    else if(position->lchild == NULL)
    {
        __erase_node_with_r_child__(t, position);
    }
    /* position 只有左子节点 */
    else if(position->rchild == NULL)
    {
        __erase_node_with_l_child__(t, position);
    }
    /* position 有左子节点和右子节点 */
    else
    {
        __erase_node_with_2_child__(t, position);
    }

    return 0;
}

si_t
tree_erase_l_subtree
(struct tree * t,
 struct tree_node * position)
{
    if(position != NULL)
    {
        __erase_subtree__(t, position->lchild);

        position->lchild = NULL;

        /* 可能删除了树的最左边节点 */
        /* 有必要更新 root->parent 的 lchild 成员 */
        t->node.lchild = __l_most_node__(t->node.parent);
    }

    return 0;
}

si_t
tree_erase_r_subtree
(struct tree * t,
 struct tree_node * position)
{
    if(position != NULL)
    {
        __erase_subtree__(t, position->rchild);

        position->rchild = NULL;

        /* 可能删除了树的最右边节点 */
        /* 有必要更新 root->parent 的 rchild 成员 */
        t->node.rchild = __r_most_node__(t->node.parent);
    }

    return 0;
}

si_t
tree_clear
(struct tree * t)
{
    /* 不是空树 */
    if(t->node.parent != NULL)
    {
        tree_erase_l_subtree(t, t->node.parent);
        tree_erase_r_subtree(t, t->node.parent);

        free(t->node.parent->data);
        free(t->node.parent);

        t->node.lchild = NULL;
        t->node.rchild = NULL;
        t->node.parent = NULL;

        t->node.data = (void *)((ui_t)(t->node.data) - 1);
    }

    return 0;
}

si_t
tree_for_each
(struct tree * t,
 si_t (* function)(void *))
{
    struct tree_node * temp;

    /* 顺序遍历 */
    /* 指向最左边节点 */
    temp = t->node.lchild;
    while(temp != NULL)
    {
        function(temp->data);
        temp = __l_n_r_increment__(t, temp);
    }

    return 0;
}
