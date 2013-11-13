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

# include "list.h"

si_t
list_clear
(struct list * l)
{
    struct list_node * ptr = l->node.next, * tmp;

    /* 从头部到尾依次删除 */
    while(ptr != &(l->node))
    {
        tmp = ptr->next;
        free(ptr->data);
        free(ptr);
        ptr = tmp;
    }

    /* 更新根节点 */
    l->node.next = &(l->node);
    l->node.prev = &(l->node);
    l->node.data = NULL;

    return 0;
}

void *
list_begin
(struct list * l)
{
    /* 头部节点地址 */
    return l->node.next;
}

void *
list_end
(struct list * l)
{
    /* 尾部节点地址 */
    return l->node.prev;
}

void *
list_front
(struct list * l)
{
    /* 头部节点元素地址 */
    return l->node.next->data;
}

void *
list_back
(struct list * l)
{
    /* 尾部节点元素地址 */
    return l->node.prev->data;
}

ui_t
list_count
(struct list * l,
 si_t (* comp)(void *))
{
    struct list_node * ptr = l->node.next;
    ui_t n = 0;

    /* 从头部节点到尾部节点依次计数 */
    while(ptr != &(l->node))
    {
        /* 满足条件 */
        if(comp(ptr->data))
        {
            ++ n;
        }

        ptr = ptr->next;
    }

    return n;
}

struct list_node *
list_find
(struct list * l,
 si_t (* comp)(void *))
{
    struct list_node * addr = l->node.next;

    /* 从头部节点到尾部节点依次查找 */
    while(addr != &(l->node))
    {
        /* 第一个满足条件 */
        if(comp(addr->data))
        {
            /* 返回节点地址 */
            return addr;
        }

        addr = addr->next;
    }

    /* 没有找到发返回 NULL */
    return NULL;
}

void *
list_lower_bound
(struct list * l,
 void * data,
 si_t (* comp)(void *, void *))
{
    struct list_node * addr = l->node.next;

    while(addr != &(l->node))
    {
        /* 第一个大于或者等于 data 的元素所在节点的地址 */
        /* (addr->data >= data) <==> (!(addr->data < data)) */
        if(!comp(addr->data, data))
        {
            return addr;
        }

        addr = addr->next;
    }

    return NULL;
}

void *
list_upper_bound
(struct list * l,
 void * data,
 si_t (* comp)(void *, void *))
{
    struct list_node * addr = l->node.next;

    while(addr != &(l->node))
    {
        /* 第一个大于 data 的元素所在节点的地址 */
        /* (addr->data > data) <==> (data < addr->data) */
        if(comp(data, addr->data))
        {
            return addr;
        }

        addr = addr->next;
    }

    return NULL;
}

si_t
list_insert
(struct list * l,
 struct list_node * position,
 void * data,
 ui_t datasize)
{
    struct list_node * tmp;

    tmp = (struct list_node *)malloc(sizeof(struct list_node));

    tmp->prev = position;
    tmp->next = position->next;
    tmp->data = position->data;
    position->next->prev = tmp;
    position->next = tmp;

    /* 插入新元素 */
    position->data = (void *)malloc(datasize);
    memcpy(position->data, data, datasize);

    /* 更新链表元素个数 */
    l->node.data = (void *)((ui_t)(l->node.data) + 1);

    return 0;
}

si_t
list_erase
(struct list * l,
 struct list_node * position)
{
    position->prev->next = position->next;
    position->next->prev = position->prev;

    free(position->data);
    free(position);

    /* 更新链表元素个数 */
    l->node.data = (void *)((ui_t)(l->node.data) - 1);

    return 0;
}

si_t
list_pop_front
(struct list * l)
{
    /* 指向链表头部元素 */
    struct list_node * ptr= l->node.next;

    if(ptr != &(l->node))
    {
        ptr->next->prev = ptr->prev;
        ptr->prev->next = ptr->next;
        free(ptr->data);
        free(ptr);

        /* 更新链表元素个数 */
        l->node.data = (void *)((ui_t)(l->node.data) - 1);
    }

    return 0;
}

si_t
list_pop_back
(struct list * l)
{
    /* 指向链表尾部元素 */
    struct list_node * ptr = l->node.prev;

    if(ptr != &(l->node))
    {
        ptr->next->prev = ptr->prev;
        ptr->prev->next = ptr->next;
        free(ptr->data);
        free(ptr);

        /* 更新链表元素个数 */
        l->node.data = (void *)((ui_t)(l->node.data) - 1);
    }

    return 0;
}

si_t
list_push_front
(struct list * l,
 void * data,
 ui_t datasize)
{
    struct list_node * ptr;

    /* 新建链表节点 */
    ptr = (struct list_node *)malloc(sizeof(struct list_node));
    ptr->data = (void *)malloc(datasize);
    memcpy(ptr->data, data, datasize);

    /* 插入到链表头部 */
    ptr->next = l->node.next;
    ptr->prev = &(l->node);

    l->node.next->prev = ptr;
    l->node.next = ptr;

    /* 更新链表元素个数 */
    l->node.data = (void *)((ui_t)(l->node.data) + 1);

    return 0;
}

si_t
list_push_back
(struct list * l,
 void * data,
 ui_t datasize)
{
    struct list_node * ptr;

    /* 新建链表节点 */
    ptr = (struct list_node *)malloc(sizeof(struct list_node));
    ptr->data = (void *)malloc(datasize);
    memcpy(ptr->data, data, datasize);

    /* 插入到链表尾部 */
    ptr->next = &(l->node);
    ptr->prev = l->node.prev;

    l->node.prev->next = ptr;
    l->node.prev = ptr;

    /* 更新链表元素个数 */
    l->node.data = (void *)((ui_t)(l->node.data) + 1);

    return 0;
}

si_t
list_for_each
(struct list * l,
 si_t (* function)(void *))
{
    struct list_node * ptr;

    /* 链表头部元素 */
    ptr = l->node.next;

    while(ptr != &(l->node))
    {
        function(ptr->data);

        ptr = ptr->next;
    }

    return 0;
}

ui_t
list_size
(struct list * l)
{
    return (ui_t)(l->node.data);
}

si_t
list_empty
(struct list * l)
{
    if(l->node.next == &(l->node))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

si_t
list_init
(struct list * l)
{
    memset(l, 0, sizeof(struct list));

    l->node.prev = &(l->node);
    l->node.next = &(l->node);
    l->node.data = NULL;

    return 0;
}

si_t
list_exit
(struct list * l)
{
    return list_clear(l);
}

addr_t
list_element_at
(struct list *l, ui_t idx)
{
    struct list_node *pos;
    ui_t cur = 0;
    if(idx > list_size(l)) {
        return NULL;
    }
    list_for_each_macro(l, pos) {
        if(cur==idx) {
            return pos->data;
        }
        cur++;
    }
    return NULL;
}
