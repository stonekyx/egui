/**
 * @file window_info_iterator.c
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

#include "window_info_iterator.h"
#include "window_manager.h"

void window_info_iterator_clear(struct window_info_iterator* iter)
{
	iter->app_info_ptr = NULL;
	iter->top_win_info_ptr = NULL;
	iter->win_info_ptr = NULL;
	iter->app_index = iter->win_index = -1;
}

si_t win_tree_traversal_increment_from(struct window_info_iterator* iter, struct window_info* from, si_t(*action)(struct window_info_iterator*, void*), void* arg)
{
	struct object* tree = iter->top_win_info_ptr->parent;
	struct object* node = (struct object*)from;
	while(node != NULL)
	{
		si_t ret = 0;
		iter->win_info_ptr = (struct window_info*)node;
		ret = action(iter, arg);
		if(ret)
			return ret;
		node = object_tree_iterator_increment(tree, node);
	}
	return 0;
}

si_t win_tree_traversal_increment(struct window_info_iterator* iter, si_t(*action)(struct window_info_iterator*, void*), void* arg)
{
	return win_tree_traversal_increment_from(iter, (struct window_info*)iter->top_win_info_ptr->parent->lchild, action, arg);
}

si_t app_traversal_increment_from(struct window_info_iterator* iter, si_t from_index, si_t(*action)(struct window_info_iterator*, void*), void* arg)
{
	int n = vector_size(&iter->app_info_ptr->window_info_vector);
	for(iter->win_index = from_index; iter->win_index < n; ++ iter->win_index)
	{
		struct object* tree = vector_at(&iter->app_info_ptr->window_info_vector, iter->win_index);
		si_t ret = 0;
		iter->top_win_info_ptr = (struct window_info*)tree->parent;
		ret = win_tree_traversal_increment(iter, action, arg);
		if(ret)
			return ret;
	}
	return 0;
}

si_t app_traversal_increment(struct window_info_iterator* iter, si_t(*action)(struct window_info_iterator*, void*), void* arg)
{
	return app_traversal_increment_from(iter, 0, action, arg);
}

si_t all_app_traversal_increment_from(struct window_info_iterator* iter, si_t from, si_t(*action)(struct window_info_iterator*, void*), void* arg)
{
	int n = vector_size(&global_wm.application_info_vector);
	for(iter->app_index = from; iter->app_index < n; ++ iter->app_index)
	{
		si_t ret = 0;
		iter->app_info_ptr = vector_at(&global_wm.application_info_vector, iter->app_index);
		ret = app_traversal_increment(iter, action, arg);
		if(ret)
			return ret;
	}
	return 0;
}

si_t all_app_traversal_increment(struct window_info_iterator* iter, si_t(*action)(struct window_info_iterator*, void*), void* arg)
{
	return all_app_traversal_increment_from(iter, 0, action, arg);
}

si_t all_app_traversal_decrement(struct window_info_iterator* iter, si_t(*action)(struct window_info_iterator*, void*), void* arg)
{
	int n = vector_size(&global_wm.application_info_vector);
	for(iter->app_index = n - 1; iter->app_index >= 0; -- iter->app_index)
	{
		si_t m = 0;
		iter->app_info_ptr = vector_at(&global_wm.application_info_vector, iter->app_index);
		m = vector_size(&iter->app_info_ptr->window_info_vector);
		for(iter->win_index = m - 1; iter->win_index >= 0; -- iter->win_index)
		{
			struct object* tree = (struct object*)vector_at(&iter->app_info_ptr->window_info_vector, iter->win_index);
			struct object* node = tree->rchild;
			iter->top_win_info_ptr = (struct window_info*)tree->parent;
			while(node != NULL)
			{
				si_t ret = 0;
				iter->win_info_ptr = (struct window_info*)node;
				ret = action(iter, arg);
				if(ret)
					return ret;
				node = object_tree_iterator_decrement(tree, node);
			}		
		}
	}
	return 0;
}

si_t all_app_traversal_top_window_decrement(struct window_info_iterator* iter, si_t(*action)(struct window_info_iterator*, void*), void* arg)
{
	int n = vector_size(&global_wm.application_info_vector);
	for(iter->app_index = n - 1; iter->app_index >= 0; -- iter->app_index)
	{
		si_t m = 0;
		iter->app_info_ptr = vector_at(&global_wm.application_info_vector, iter->app_index);
		m = vector_size(&iter->app_info_ptr->window_info_vector);
		for(iter->win_index = m - 1; iter->win_index >= 0; -- iter->win_index)
		{
			si_t ret = 0;
			struct object* tree = (struct object*)vector_at(&iter->app_info_ptr->window_info_vector, iter->win_index);
			iter->top_win_info_ptr = (struct window_info*)tree->parent;
			ret = action(iter, arg);
			if(ret)
				return ret;
		}
	}
	return 0;
}
