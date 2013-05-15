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

# include "queue.h"

si_t
queue_init
(struct queue * q)
{
    list_init(&(q->container));

    return 0;
}

si_t
queue_exit
(struct queue * q)
{
    list_exit(&(q->container));

    return 0;
}

ui_t
queue_size
(struct queue * q)
{
    return list_size(&(q->container));
}

si_t
queue_empty
(struct queue * q)
{
    return list_empty(&(q->container));
}

void *
queue_front
(struct queue * q)
{
    return list_front(&(q->container));
}

void *
queue_back
(struct queue * q)
{
    return list_back(&(q->container));
}

si_t
queue_push
(struct queue * q,
 void * data,
 ui_t datasize)
{
    list_push_back(&(q->container), data, datasize);

    return 0;
}

si_t
queue_pop(struct queue * q)
{
    list_pop_front(&(q->container));

    return 0;
}
