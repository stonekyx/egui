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

# include "vector.h"

# define _VECTOR_DEFAULT_CAPACITY_ 8

ui_t
vector_size
(struct vector * v)
{
    return v->curr - v->head;
}

si_t
vector_empty
(struct vector * v)
{
    return v->curr == v->head;
}

void *
vector_begin
(struct vector * v)
{
    return v->head;
}

void *
vector_end
(struct vector * v)
{
    return v->curr - 1;
}

void *
vector_front
(struct vector * v)
{
    return v->head->data;
}

void *
vector_back
(struct vector * v)
{
    return (v->curr - 1)->data;
}

void *
vector_at
(struct vector * v, si_t i)
{
    return (v->head + i)->data;
}

si_t
vector_move_front
(struct vector * v,
 si_t i)
{
    struct vector_node temp;

    /* 保存第 i + 1 个节点 */
    memcpy
    (&temp,
     v->head + i,
     sizeof(struct vector_node));

    /* 将前面的向后移动一个位置 */
    memmove
    (v->head + 1,
     v->head,
     i * sizeof(struct vector_node));

    /* 设置最前面的 */
    memcpy
    (v->head,
     &temp,
     sizeof(struct vector_node));

    return 0;
}

si_t
vector_move_back
(struct vector * v,
 si_t i)
{
    struct vector_node temp;

    /* 保存第 i + 1 个节点 */
    memcpy
    (&temp,
     v->head + i,
     sizeof(struct vector_node));

    /* 将后面的向前移动一个位置 */
    memcpy
    (v->head + i,
     v->head + i + 1,
     (v->curr - v->head - i - 1) * sizeof(struct vector_node));

    /* 设置最后面的 */
    memcpy
    (v->curr - 1,
     &temp,
     sizeof(struct vector_node));

    return 0;
}

ui_t
vector_count
(struct vector * v,
 si_t (* comp)(void *))
{
    struct vector_node * addr = v->head;
    ui_t n = 0;

    while(addr < v->curr)
    {
        if(comp(addr->data))
        {
            ++ n;
        }

        ++ addr;
    }

    return n;
}

si_t
vector_find
(struct vector * v,
 void * data,
 si_t (* comp)(void *, void *))
{
    struct vector_node * addr = v->head;

    while(addr < v->curr)
    {
        if(comp(addr->data, data))
        {
            return addr - v->head;
        }

        ++ addr;
    }

    return -1;
}

si_t
vector_lower_bound
(struct vector * v,
 void * data,
 si_t (* comp)(void *, void *))
{
    struct vector_node * addr = v->head;

    while(addr < v->curr)
    {
        /* (addr->data >= data) <==> (!(addr->data < data)) */
        if(!comp(addr->data, data))
        {
            return addr - v->head;
        }

        ++ addr;
    }

    return -1;
}

si_t
vector_upper_bound
(struct vector * v,
 void * data,
 si_t (* comp)(void *, void *))
{
    struct vector_node * addr = v->head;

    while(addr < v->curr)
    {
        /* (addr->data > data) <==> (data < addr->data) */
        if(comp(data, addr->data))
        {
            return addr - v->head;
        }

        ++ addr;
    }

    return -1;
}

si_t
vector_push_back
(struct vector * v,
 void * data,
 ui_t datasize)
{
    struct vector_node * addr = NULL;
    ui_t temp = 0;

    /* 节点个数 */
    temp = v->curr - v->head;

    /* 需要更多内存 */
    if(v->curr == v->tail)
    {
        addr = (struct vector_node *)calloc(sizeof(struct vector_node), (temp + 1) << 1);
        memcpy(addr, v->head, sizeof(struct vector_node) * temp);
        free(v->head);

        v->head = addr;
        v->curr = addr + temp;
        v->tail = addr + (temp << 1) + 1;
    }

    /* 插入元素 */
    v->curr->data = malloc(datasize);
    memcpy(v->curr->data, data, datasize);

    /* 更新指针 */
    ++ v->curr;

    return 0;
}

si_t
vector_pop_back
(struct vector * v)
{
    -- v->curr;

    free(v->curr->data);

    return 0;
}

si_t
vector_insert
(struct vector * v,
 ui_t i,
 void * data,
 ui_t datasize)
{
    struct vector_node * addr = NULL;
    ui_t temp = 0;

    /* 节点个数 */
    temp = v->curr - v->head;

    /* 需要更多内存 */
    if(v->curr == v->tail)
    {
        addr = (struct vector_node *)calloc(sizeof(struct vector_node), (temp + 1) << 1);
        memcpy(addr, v->head, sizeof(struct vector_node) * temp);
        free(v->head);

        v->head = addr;
        v->curr = addr + temp;
        v->tail = addr + (temp << 1) + 1;
    }

    memmove
    (v->head + i + 1,
     v->head + i,
     sizeof(struct vector_node) * (v->curr - v->head - i));

    /* 插入元素 */
    (v->head + i)->data = (void *)malloc(datasize);
    memcpy((v->head + i)->data, data, datasize);

    /* 更新指针 */
    ++ v->curr;

    return 0;
}

si_t
vector_erase
(struct vector * v,
 ui_t i)
{
    free((v->head + i)->data);

    memcpy
    (v->head + i,
     v->head + i + 1,
     sizeof(struct vector_node) * (v->curr - v->head - i - 1));

    -- v->curr;

    return 0;
}

si_t
vector_for_each
(struct vector * v,
 void * data,
 si_t (* function)(void *, void *))
{
    struct vector_node * ptr = v->head;

    while(ptr != v->curr)
    {
        function(ptr->data, data);
        ++ ptr;
    }

    return 0;
}

# if 0
si_t
vector_sort
(struct vector * v,
 si_t (* comp)(void *, void *))
{
    si_t i;
    ui_t ** tmp;
    struct vector_node * ptr;

    if(v->curr != v->head)
    {
        tmp = (ui_t **)calloc(sizeof(void *), v->curr - v->head);

        for(i = 0, ptr = v->head; ptr != v->curr; ++ i)
        {
            tmp[i] = (ui_t *)ptr->data;
            ++ ptr;
        }

        qsort(tmp, v->curr - v->head, sizeof(void *), comp);

        for(i = 0, ptr = v->head; ptr != v->curr; ++ i)
        {
            ptr->data = tmp[i];
            ++ ptr;
        }

        free(tmp);
    }

    return 0;
}
# endif

si_t
vector_clear
(struct vector * v)
{
    if(v->curr != v->head)
    {
        while(v->curr != v->head)
        {
            -- v->curr;

            free(v->curr->data);
        }
    }

    return 0;
}

si_t
vector_init
(struct vector * v)
{
    v->head = (struct vector_node *)calloc(sizeof(struct vector_node), _VECTOR_DEFAULT_CAPACITY_);
    v->tail = v->head + _VECTOR_DEFAULT_CAPACITY_ - 1;
    v->curr = v->head;

    return 0;
}

si_t
vector_exit
(struct vector * v)
{
    while(v->curr > v->head)
    {
        -- v->curr;
        free(v->curr->data);
    }

    free(v->head);

    v->head = NULL;
    v->curr = NULL;
    v->tail = NULL;

    return 0;
}

