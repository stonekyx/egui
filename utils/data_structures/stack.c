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

# include "stack.h"

si_t
stack_init
(struct stack * s)
{
    vector_init(&(s->container));

    return 0;
}

si_t
stack_exit
(struct stack * s)
{
    vector_exit(&(s->container));

    return 0;
}

ui_t
stack_size
(struct stack * s)
{
    return vector_size(&(s->container));
}

si_t
stack_empty
(struct stack * s)
{
    return vector_empty(&(s->container));
}

void *
stack_top
(struct stack * s)
{
    return vector_back(&(s->container));
}

si_t
stack_push
(struct stack * s,
 void * data,
 ui_t datasize)
{
    vector_push_back(&(s->container), data, datasize);

    return 0;
}

si_t
stack_pop
(struct stack * s)
{
    vector_pop_back(&(s->container));

    return 0;
}
