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

# include <log.h>

# include "hashmap.h"

# define HASHMAP_DEFAULT_DATA_SIZE 4
# define HASHMAP_DEFAULT_PRIME 313l
# define HASHMAP_DEFAULT_GROW_THRESHOLD 0.8
# define HASHMAP_DEFAULT_SHRINK_THRESHOLD 0.2

si_t
hashmap_key_cmp
(const struct hashmap_key *key1,
 const struct hashmap_key *key2)
{
    if(!key1 || !key2) {
        return (key1?1:0) - (key2?1:0);
    }
    if(key1->size != key2->size)
        return key1->size - key2->size;
    return memcmp(key1->key, key2->key, key1->size);
}

/**
 * Notice: This probe function can guarantee the reachability from any
 * position to all other positions in the hashmap ONLY WHEN the data_size
 * is power of 2.
 **/
static
ui_t
hashmap_default_probe_func
(const struct hashmap *hashmap,
 ui_t src,
 const struct hashmap_key *key)
{
    int incri = 1;
    if(NULL == hashmap) {
        EGUI_PRINT_ERROR("Internal error in hashmap_default_probe_func: "
                "hashmap is NULL.");
        return 0;
    }
    src %= hashmap->data_size;
    while(hashmap->data[src].value != NULL &&
            hashmap_key_cmp(&hashmap->data[src].key, key)) {
        src = (src+incri) % hashmap->data_size;
        incri++;
    }
    return src;
}

static
ui_t
hashmap_default_hash_func
(const struct hashmap_key *key,
 ui_t prime)
{
    ui_t res;
    unsigned char *src;
    ui_t key_size;
    if(NULL == key) {
        return 0;
    }
    src = key->key;
    key_size = key->size;
    for(res=0; key_size; key_size--) {
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
    q->data_cnt = 0;
    q->data = calloc(q->data_size, sizeof(struct hashmap_pair));
    q->hash_func = hashmap_default_hash_func;
    q->probe_func = hashmap_default_probe_func;
    q->prime = HASHMAP_DEFAULT_PRIME;
    q->grow_threshold = HASHMAP_DEFAULT_GROW_THRESHOLD;
    q->shrink_threshold = HASHMAP_DEFAULT_SHRINK_THRESHOLD;
}

si_t
hashmap_exit
(struct hashmap *q)
{
    free(q->data);
    q->data = NULL;
    q->data_size = 0;
    q->data_cnt = 0;
    q->hash_func = NULL;
    q->probe_func = NULL;
    q->prime = 0;
    q->grow_threshold = 0;
    q->shrink_threshold = 0;
}

static
si_t
hashmap_rehash
(struct hashmap *q,
 ui_t new_size,
 struct hashmap_pair *old,
 ui_t old_size)
{
    int i;
    q->data = calloc(new_size, sizeof(struct hashmap_pair));
    if(NULL == q->data) {
        q->data = old;
        return -1;
    }
    q->data_size = new_size;
    q->data_cnt = 0;
    for(i=0; i<old_size; i++) {
        if(old[i].value)
            hashmap_insert(q, old+i);
    }
    free(old);
    return 0;
}

static
si_t
hashmap_grow
(struct hashmap *q)
{
    return hashmap_rehash(q, q->data_size*2, q->data, q->data_size);
}

static
si_t
hashmap_shrink
(struct hashmap *q)
{
    if(q->data_size<=4) {
        return 1;
    }
    return hashmap_rehash(q, q->data_size/2, q->data, q->data_size);
}

si_t
hashmap_insert
(struct hashmap *q,
 const struct hashmap_pair *data)
{
    ui_t hashval;
    ui_t judge_pos;
    if(NULL == q || NULL == data) {
        return -1;
    }
    if(q->data_cnt >= (int)(q->data_size*q->grow_threshold)) {
        hashmap_grow(q);
    }
    hashval = (q->hash_func)(&data->key, q->prime);
    judge_pos = (q->probe_func)(q, hashval, &data->key);
    if(q->data[judge_pos]) {
        return 1;
    } else {
        q->data[judge_pos] = *data;
        q->data_cnt++;
        return 0;
    }
}

struct hashmap_key
hashmap_key_generate
(const_addr_t key,
 ui_t key_size)
{
    struct hashmap_key res = {
        .key = key,
        .size = key_size
    };
    return res;
}

struct hashmap_pair
hashmap_pair_generate
(const_addr_t key,
 ui_t key_size,
 addr_t value)
{
    struct hashmap_pair res = {
        .key = hashmap_key_generate(key, size),
        .value = value
    };
    return res;
}

addr_t
hashmap_find
(const struct hashmap *q,
 const struct hashmap_key *key)
{
    ui_t hashval;
    ui_t judge_pos;
    if(NULL == q || NULL == key) {
        return NULL;
    }
    hashval = (q->hash_func)(key, q->prime);
    judge_pos = (q->probe_func)(q, hashval, key);
    if(q->data[judge_pos]) {
        return q->data[judge_pos].value;
    } else {
        return NULL;
    }
}

si_t
hashmap_erase
(struct hashmap *q,
 const struct hashmap_key *key)
{
    ui_t hashval;
    ui_t judge_pos;
    if(NULL == q || NULL == key) {
        return -1;
    }
    hashval = (q->hash_func)(key, q->prime);
    judge_pos = (q->probe_func)(q, hashval, key);
    if(q->data[judge_pos]) {
        q->data_cnt--;
        q->data[judge_pos] = NULL;
        if(q->data_cnt < q->data_size*q->shrink_threshold) {
            hashmap_shrink(q);
        }
        return 0;
    } else {
        return 1;
    }
}
