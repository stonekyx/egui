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

# include "manipulate_window.h"

# include <stddef.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

# include "application.h"
# include "log.h"

si_t application_set_main_window(struct window * window)
{
	/* 指向对象树根节点 */
	global_application.main_window = window;

	return 0;
}

/*
   总是将孩子节点加在子节点链的最右边

   如果 parent 在对象树中
   那么完成添加工作后一定要更新 root->parent 的 lchild 成员和 rchild 成员
   这是为了保证遍历的正确
   */
si_t application_add_window(struct window * parent, struct window * window)
{
	struct object node, * tree;

	/* 添加顶层窗口 */
	if(parent == NULL)
	{
		/* node 的父节点是 window */
		node.parent = OBJECT_POINTER(window);
		node.lchild = object_tree_l_most_node(OBJECT_POINTER(window));
		node.rchild = object_tree_r_most_node(OBJECT_POINTER(window));
		node.name = NULL;
		node.id = 0;

		/* 将窗口添加到向量 */
		vector_push_back(&(global_application.window_vector), &node, sizeof(struct object));

		tree = vector_back(&(global_application.window_vector));

		/* window 的父节点是 node */
		window->parent = tree;
	}
	/* 添加非顶层窗口 */
	else
	{
		object_insert_child(OBJECT_POINTER(parent), OBJECT_POINTER(window));
	}
	/**
	 * if already running in app_exec
	 **/
	if(global_application.exec_flag)
	{
		union message msg;
		msg.base.type = MESSAGE_TYPE_WIDGET_REPAINT;
		msg.widget_repaint.area = window->area;

		if(global_application.focus != NULL)
		{
			struct window* w = WINDOW_POINTER(global_application.focus);
			struct object* tree, *node, *tree_real_parent;
			w->is_activated = 0;
			msg.widget_repaint.area = w->area;
			if(w->callback != NULL)
			{
				w->callback(w, &msg);
			}

			node = OBJECT_POINTER(w);
			tree = node->parent;
			tree_real_parent = tree->parent;
			tree->parent = node;
			node = object_tree_l_most_node(tree);
			while(node != NULL)
			{
				union message msg;
				msg.base.type = MESSAGE_TYPE_WIDGET_REPAINT;
				msg.widget_repaint.area = WIDGET_POINTER(node)->area;
				if(WIDGET_POINTER(node)->callback != NULL)
					WIDGET_POINTER(node)->callback(node, &msg);

				node = object_tree_iterator_increment(tree, node);
			}
			tree->parent = tree_real_parent;
		}

		window->descriptor = register_window((parent == NULL ? 0 : parent->descriptor), window->title, 
			window->area.x, window->area.y, window->area.width, window->area.height, 
			window->minimize_enable, window->maximize_enable, window->modal);
		if(window->descriptor == 0)
		{
			EGUI_PRINT_ERROR("failed to register window");
			application_exit();
			return -1;
		}

		/**
		 * find icon for the application
		 **/
		if((window->icon_path = (char*)malloc(256)) == NULL)
		{
			EGUI_PRINT_SYS_ERROR("failed to malloc for icon path. malloc");
			application_exit();
			return -1;
		}
		else
		{
			/**
			 * in C89 standard, snprintf() is NOT included in <stdio.h>
			 * so you have to use sprintf, which may be dangerous. be careful
			 **/
			sprintf(window->icon_path, "%s/icons/%s.bmp", global_application.icon_root_path, global_application.name);
			if(access(window->icon_path, R_OK) == -1)
			{
				sprintf(window->icon_path, "%s/icons/default.bmp", global_application.icon_root_path);
			}
		}

		msg.widget_repaint.area = window->area;
		global_application.focus = window;
		window->is_activated = 1;
		if(window->callback != NULL)
		{
			window->callback(window, &msg);
		}
	}

	return 0;
}

void __widget_destructor(void* arg)
{
	struct widget* w = WIDGET_POINTER(arg);
	graphics_device_exit(w->gd);
	free(arg);
}

/*
   如果 window 有左子节点
   那么这个左子节点并不是 window 子对象
   所以在析构以 window 为根的树之前一定要要将 window->lchild 成员清空

   如果 window 在对象树中
   那么完成删除工作后一定要更新 root->parent 的 lchild 成员和 rchild 成员
   这是为了保证遍历的正确
   */
si_t application_del_window(struct window * window)
{
	si_t is_top_window = 0;

	/* 删除主窗口 */
	/* 退出程序 */
	if(window == global_application.main_window)
	{
		application_exit();
		return 0;
	}

	if(window->parent->parent == OBJECT_POINTER(window))
	{
		is_top_window = 1;
	}

	cancel_window(window->descriptor);

	object_delete(OBJECT_POINTER(window), __widget_destructor);
	if(is_top_window)
	{
		si_t i = 0, n = 0;
		/* 有多少个窗口 */
		n = vector_size(&(global_application.window_vector));

		/* 找到这个窗口在向量中的索引 */
		for(i = 0; i < n; ++ i)
		{
			struct object* tree = vector_at(&(global_application.window_vector), i);
			if(tree->parent == OBJECT_POINTER(window))
			{
				/* 从窗口向量中删除这个节点 */
				vector_erase(&(global_application.window_vector), i);
				break;
			}
		}
	}

	return 0;
}
