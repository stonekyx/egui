/**
 * @file hashmap.h
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

# ifndef _HASHMAP_H_
# define _HASHMAP_H_ 1

# include <base_type.h>

struct hashmap_key;
struct hashmap_pair;
struct hashmap;

typedef ui_t (*hash_func_t)(const struct hashmap_key *key, ui_t prime);
typedef ui_t (*probe_func_t)(const struct hashmap *hashmap, ui_t src, const struct hashmap_key *key);

struct hashmap *hashmap_init(void);

si_t hashmap_exit(struct hashmap *);

si_t hashmap_insert(struct hashmap *q, const struct hashmap_pair *data);

struct hashmap_key *
hashmap_key_generate(const_addr_t key, ui_t key_size);

struct hashmap_pair *
hashmap_pair_generate(const_addr_t key, ui_t key_size, addr_t value);

addr_t hashmap_find(const struct hashmap *q, struct hashmap_key *key, si_t free_key);

si_t hashmap_erase(struct hashmap *q, struct hashmap_key *key, si_t free_key);

# endif
