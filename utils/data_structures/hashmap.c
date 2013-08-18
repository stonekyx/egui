/**
 * @file hashmap.c
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

# include "hashmap.h"

# define HASHMAP_DEFAULT_DATA_SIZE 8
# define HASHMAP_DEFAULT_PRIME 313l

static
ui_t
hashmap_default_hash_func
(addr_t key, ui_t key_size, ui_t prime)
{
    ui_t res = 0;
    unsigned char *src = key;
    for(; key_size; key_size--) {
        res = res*prime + *src++;
    }
    return res;
}

si_t
hashmap_init
(struct hashmap *q)
{
    if(NULL == q) {
        return -1;
    }
    q->data_size = HASHMAP_DEFAULT_DATA_SIZE;
    q->data = calloc(q->data_size, sizeof(struct hashmap_pair));
    q->hash_func = hashmap_default_hash_func;
    q->prime = HASHMAP_DEFAULT_PRIME;
}

si_t
hashmap_exit
(struct hashmap *q)
{
    free(q->data);
    q->data = NULL;
    q->data_size = 0;
    q->hash_func = NULL;
    q->prime = 0;
}
