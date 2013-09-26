/**
 * @file window_manager_input_handler.c
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

# include <string.h>

# include "base_type.h"
# include "server_lib.h"
# include "cursor.h"
# include "event.h"
# include "window_manager.h"
# include "flush_in_z_order.h"
# include "application_info.h"
# include "window_info.h"
# include "window_info_iterator.h"
# include "log.h"

/* 记录鼠标的左键是否按下 */
static si_t title_bar_press = 0;
/* 记录最小化按钮是否被按下 */
static si_t minimize_button_press = 0;
/* 记录最大化按钮是否被按下 */
static si_t maximize_button_press = 0;
/* 记录退出按钮是否被按下 */
static si_t close_button_press = 0;
/* 记录左边框是否被按下 */
static si_t l_frame_press = 0;
/* 记录上边框是否被按下 */
static si_t t_frame_press = 0;
/* 记录右边框是否被按下 */
static si_t r_frame_press = 0;
/* 记录下边框是否被按下 */
static si_t b_frame_press = 0;

/* 发送窗口部件移动消息时的偏移量 */
static si_t widget_move_new_x_offset = 0;
static si_t widget_move_new_y_offset = 0;
static si_t widget_move_old_x_offset = 0;
static si_t widget_move_old_y_offset = 0;

/* 发送窗口部件改变大小消息时的偏移量 */
static si_t widget_resize_new_x_offset = 0;
static si_t widget_resize_new_y_offset = 0;
static si_t widget_resize_old_x_offset = 0;
static si_t widget_resize_old_y_offset = 0;

/*****************************************************************************************************/

/* traversal_app {{{ */
si_t traversal_app(struct application_info * app_info_ptr)
{
	si_t i, n;
	struct object * tree, * node;

	n = vector_size(&(app_info_ptr->window_info_vector));

	for(i = 0; i < n; ++ i)
	{
		tree = vector_at(&(app_info_ptr->window_info_vector), i);

		node = tree->lchild;
		while(node != NULL)
		{
			struct window_info * win_info_ptr = (struct window_info *)node;
			EGUI_PRINT_INFO("traversal: app %s, window %s", app_info_ptr->name, win_info_ptr->title);

			node = object_tree_iterator_increment(tree, node);
		}
	}

	return 0;
}
/* }}}*/

/**
 * 处理桌面图标被按下
 *
 * @param msg 必须是鼠标按下消息
 *
 * @return 0 
 **/
static si_t handle_desktop_press(union message * msg)
{
	/* 在桌面上按下了鼠标的某个键 */
	if(global_wm.active_win_info_ptr == NULL && global_wm.active_app_info_ptr == NULL && NULL != global_wm.desktop_app_ptr)
	{
		send_input_message(&global_wm.desktop_app_ptr->uds, msg, 0);
	}
	return 0;
}

/**
 * 处理桌面图标被松开
 *
 * @param msg 必须是鼠标松开消息
 *
 * @return 0
 **/
static si_t handle_desktop_release(union message * msg)
{
	if(global_wm.active_win_info_ptr == NULL && global_wm.active_app_info_ptr == NULL && NULL != global_wm.desktop_app_ptr)
	{
		send_input_message(&global_wm.desktop_app_ptr->uds, msg, 0);
	}

	return 0;
}

static si_t _do_find_clicked_window(struct window_info_iterator* iter, addr_t arg)
{
	struct point* pos = (struct point*)arg;
	if(is_point_in_area(pos, &iter->win_info_ptr->area))
	{
		return 1;
	}
	return 0;
}

/**
 * 通过按下鼠标消息设置那个是活动的
 * 设置活动用户应用程序和活动窗口
 *
 * @param msg 必须是鼠标按下消息
 *
 * @return 0
 **/
static si_t mask_active_by_mouse_down(union message * msg)
{
	struct window_info_iterator iter;
	window_info_iterator_clear(&iter);

	if(!all_app_traversal_decrement(&iter, _do_find_clicked_window, &msg->mouse.cursor_position)) {
        /* Clear useless pointers in tainted iter */
        window_info_iterator_clear(&iter);
    }

	/* 在桌面上按下了鼠标的某个键 */
	if(iter.app_info_ptr == NULL || iter.win_info_ptr == NULL)
	{
		/* 原来有活动窗口 */
		if(global_wm.active_win_info_ptr != NULL)
		{
			/* 发送激死消息给原来的活动窗口 */
			send_window_deactivate_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)global_wm.active_win_info_ptr);
			/** 通知桌面 **/
			if(NULL != global_wm.desktop_app_ptr)
			{
				send_window_deactivate_message(&global_wm.desktop_app_ptr->uds, NULL,
					(si_t)object_get_root(OBJECT_POINTER(global_wm.active_win_info_ptr)));
			}

			/* 改变活动窗口 */
			global_wm.active_win_info_ptr = NULL;
			global_wm.active_app_info_ptr = NULL;
		}
	}
	/* 在窗口上按下了鼠标的某个键 */
	else if(global_wm.active_win_info_ptr != iter.win_info_ptr)
	{
		/* 原来有活动窗口 */
		if(global_wm.active_win_info_ptr != NULL)
		{
			/* 发送激死消息给原来的活动窗口 */
			send_window_deactivate_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)global_wm.active_win_info_ptr);
			/** 通知桌面 **/
			if(NULL != global_wm.desktop_app_ptr)
			{
				send_window_deactivate_message(&global_wm.desktop_app_ptr->uds, NULL,
					(si_t)object_get_root(OBJECT_POINTER(global_wm.active_win_info_ptr)));
			}
		}

		/* 将活动窗口移动到该用户应用程序的窗口向量的尾部 */
		vector_move_back(&(iter.app_info_ptr->window_info_vector), iter.win_index);

		/* 将活动程序移动到用户应用程序向量的尾部 */
		vector_move_back(&(global_wm.application_info_vector), iter.app_index);

		/* 将活动窗口作为父控件的长子控件 */
		object_move_first(OBJECT_POINTER(iter.win_info_ptr));

		/* 改变活动窗口 */
		global_wm.active_win_info_ptr = iter.win_info_ptr;
		global_wm.active_app_info_ptr = iter.app_info_ptr;

		send_window_activate_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)iter.win_info_ptr);
		if(NULL != global_wm.desktop_app_ptr)
		{
			send_window_activate_message(&global_wm.desktop_app_ptr->uds, NULL, (si_t)iter.top_win_info_ptr);
		}
	}

	return 0;
}


/**
 * 处理按钮被按下
 *
 * @param msg 必须是鼠标按下消息
 *
 * @return 0
 **/
si_t handle_button_press(union message * msg)
{
	/* 有包含窗口的用户应用程序 */
	if(global_wm.active_app_info_ptr != NULL && global_wm.active_win_info_ptr != NULL)
	{
		if(msg->mouse.code == INPUT_CODE_MOUSE_L_KEY)
		{
			/* 在活动窗口的最小化按钮内按键 */
			if(global_wm.active_win_info_ptr->minimize_enable == 1 
				&& is_point_in_area(&(msg->mouse.cursor_position), &(global_wm.active_win_info_ptr->minimize_button_area)))
			{
				minimize_button_press = 1;
			}
			/* 在活动窗口的最大化按钮内按键 */
			if(global_wm.active_win_info_ptr->maximize_enable == 1 
				&& is_point_in_area(&(msg->mouse.cursor_position), &(global_wm.active_win_info_ptr->maximize_button_area)))
			{
				maximize_button_press = 1;
			}
			/* 在活动窗口的退出按钮内按键 */
			if(is_point_in_area(&(msg->mouse.cursor_position), &(global_wm.active_win_info_ptr->close_button_area)))
			{
				close_button_press = 1;
			}
		}
	}

	return 0;
}

static si_t _do_find_next_active_window(struct window_info_iterator* iter, addr_t arg)
{
	struct window_info* this_top_win_info_ptr = (struct window_info*)arg;
	/* 不能与当前活动窗口同为一个顶层窗口 不能是最小化的窗口 */
	if(iter->top_win_info_ptr != this_top_win_info_ptr
		&& iter->win_info_ptr->area.x >= 0)
	{
		return 1;
	}
	return 0;
}

static si_t handle_minimize_button_release(union message* msg)
{
	struct object* tree = NULL;
	struct object* node = NULL;
	int i = 0, j = 0;
	struct window_info_iterator iter;
	/* 不能最小化或不在活动窗口的最小化按钮内释放 */
	if(global_wm.active_win_info_ptr->minimize_enable != 1
		|| !is_point_in_area(&(msg->mouse.cursor_position), &(global_wm.active_win_info_ptr->minimize_button_area)))
	{
		return 0;
	}

	/**
	 * send minmize msg to every client window(including subwindows)
	 **/
	tree = object_get_root(OBJECT_POINTER(global_wm.active_win_info_ptr))->parent;
	node = tree->lchild;
	while(node != NULL)
	{
		EGUI_PRINT_INFO("send minimized msg to %s", ((struct window_info*)node)->title);
		send_window_minimize_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)node);
		node = object_tree_iterator_increment(tree, node);
	}

	/**
	 * send deactive msg to client
	 **/
	send_window_deactivate_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)global_wm.active_win_info_ptr);
	/** 通知桌面 **/
	if(NULL != global_wm.desktop_app_ptr)
	{
		send_window_deactivate_message(&global_wm.desktop_app_ptr->uds, NULL, (si_t)tree->parent);
	}

	window_info_iterator_clear(&iter);
	/* get next active window and application */
	if(all_app_traversal_decrement(&iter, _do_find_next_active_window, tree->parent))
	{
		global_wm.active_win_info_ptr = iter.win_info_ptr;
		global_wm.active_app_info_ptr = iter.app_info_ptr;
	}
	else
	{
		global_wm.active_win_info_ptr = NULL;
		global_wm.active_app_info_ptr = NULL;
		return 0;
	}

	/**
	 * 获得顶层窗口
	 **/
	node = object_get_root(OBJECT_POINTER(global_wm.active_win_info_ptr));
	/**
	 * move next active window back
	 **/
	i = application_info_get_win_index(global_wm.active_app_info_ptr, (struct window_info*)node);
	j = window_manager_get_app_index(global_wm.active_app_info_ptr);
	vector_move_back(&(global_wm.active_app_info_ptr->window_info_vector), i);
	vector_move_back(&(global_wm.application_info_vector), j);

	/**
	 * send active msg
	 **/
	send_window_activate_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)global_wm.active_win_info_ptr);
	if(NULL != global_wm.desktop_app_ptr)
	{
		send_window_activate_message(&global_wm.desktop_app_ptr->uds, NULL, (si_t)node);
	}

	return 0;
}

/**
 * 处理按钮被松开
 *
 * @param msg 必须是鼠标松开消息
 *
 * @return 0
 **/
static si_t handle_button_release(union message * msg)
{
	if(global_wm.active_app_info_ptr == NULL && global_wm.active_win_info_ptr == NULL)
	{
		return 0;
	}
	/**
	 * 只处理鼠标左键
	 **/
	if(msg->mouse.code != INPUT_CODE_MOUSE_L_KEY)
	{
		return 0;
	}
	/* 活动窗口的最小化按钮被按下 */
	if(minimize_button_press == 1)
	{
		minimize_button_press = 0;
		handle_minimize_button_release(msg);
	}
	/* 活动窗口的最大化按钮被按下 */
	else if(maximize_button_press == 1)
	{
		maximize_button_press = 0;

		if(global_wm.active_win_info_ptr->maximize_enable == 1 && is_point_in_area(&(msg->mouse.cursor_position), &(global_wm.active_win_info_ptr->maximize_button_area)))
		{
			/**
			 * the user who press this button meaned to maximize it
			 **/
			if(global_wm.active_win_info_ptr->need_restore == 0)
			{
				send_window_maximize_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)global_wm.active_win_info_ptr);
				global_wm.active_win_info_ptr->need_restore = 1;
			}
			/**
			 * the user who press this button meaned to restore it from maximization
			 **/
			else if(global_wm.active_win_info_ptr->need_restore == 1)
			{
				send_window_restore_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)global_wm.active_win_info_ptr);
				global_wm.active_win_info_ptr->need_restore = 0;
			}
		}
	}
	/* 活动窗口的退出按钮被按下 */
	else if(close_button_press == 1)
	{
		close_button_press = 0;

		if(is_point_in_area(&(msg->mouse.cursor_position), &(global_wm.active_win_info_ptr->close_button_area)))
		{
			send_window_cancel_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)global_wm.active_win_info_ptr);
		}
	}

	return 0;
}

/**
 * 标记标题栏是否被按下
 *
 * @param msg 必须是鼠标按下消息
 *
 * @return 0
 **/
static si_t handle_title_bar_press(union message * msg)
{
	/* 有包含窗口的用户应用程序 */
	if(global_wm.active_app_info_ptr != NULL && global_wm.active_win_info_ptr != NULL)
	{
		if(msg->mouse.code == INPUT_CODE_MOUSE_L_KEY)
		{
			/* 在活动窗口的标题栏内按键 */
			if(is_point_in_area(&(msg->mouse.cursor_position), &(global_wm.active_win_info_ptr->title_bar_area)))
			{
				title_bar_press = 1;/*
									   printf("title_bar_press = 1\n");*/
			}
		}
	}

	return 0;
}

/**
 * 处理控件的移动
 *
 * 标记标题栏是否被按下
 *
 * @param msg 必须是鼠标移动消息
 *
 * @return 0
 **/
static si_t accumulate_widget_move(union message * msg)
{
	struct point p;

	/* 无包含窗口的用户应用程序 */
	if(global_wm.active_app_info_ptr == NULL && global_wm.active_win_info_ptr == NULL)
	{
		return 0;
	}
	if(title_bar_press != 1)
	{
		return 0;
	}

	screen_flush
		(global_wm.active_win_info_ptr->area.x + widget_move_old_x_offset,
		 global_wm.active_win_info_ptr->area.y + widget_move_old_y_offset,
		 global_wm.active_win_info_ptr->area.width,
		 global_wm.active_win_info_ptr->area.height);

	/* keep the cursor in desktop workarea */
	ensure_point_in_area(&global_wm.new_cursor, &global_wm.work_area);

	point_set(&p, 
		global_wm.active_win_info_ptr->area.x + widget_move_new_x_offset, global_wm.active_win_info_ptr->area.y + widget_move_new_y_offset);
	/* make sure window did not run away */
	ensure_point_in_area(&p, &global_wm.work_area);
	widget_move_old_x_offset = p.x - global_wm.active_win_info_ptr->area.x;
	widget_move_old_y_offset = p.y - global_wm.active_win_info_ptr->area.y;

	/* 绘制活动窗口的边框 */
	engine_draw_rectangle
		(global_wm.gd_handler,
		 global_wm.active_win_info_ptr->area.x + widget_move_old_x_offset,
		 global_wm.active_win_info_ptr->area.y + widget_move_old_y_offset,
		 global_wm.active_win_info_ptr->area.width,
		 global_wm.active_win_info_ptr->area.height);

	if(msg->mouse.code == INPUT_CODE_MOUSE_X_OFFSET)
	{
		widget_move_new_x_offset += global_wm.new_cursor.x - global_wm.old_cursor.x;
	}
	else if(msg->mouse.code == INPUT_CODE_MOUSE_Y_OFFSET)
	{
		widget_move_new_y_offset += global_wm.new_cursor.y - global_wm.old_cursor.y;
	}

	return 0;
}

/**
 * 发送控件移动消息
 *
 * @param msg 必须是鼠标松开消息
 *
 * @return 0
 **/
static si_t handle_widget_move(union message * msg)
{
	/* 有包含窗口的用户应用程序 */
	if(global_wm.active_app_info_ptr != NULL && global_wm.active_win_info_ptr != NULL)
	{
		/* 松开消息 */
		if(msg->mouse.code == INPUT_CODE_MOUSE_L_KEY && title_bar_press == 1)
		{
			screen_flush
				(global_wm.active_win_info_ptr->area.x + widget_move_old_x_offset,
				 global_wm.active_win_info_ptr->area.y + widget_move_old_y_offset,
				 global_wm.active_win_info_ptr->area.width,
				 global_wm.active_win_info_ptr->area.height);

			cursor_paint();

			/* 发生了偏移 */
			if(widget_move_old_x_offset != 0 || widget_move_old_y_offset != 0)
			{
				send_widget_move_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)global_wm.active_win_info_ptr, 
					global_wm.active_win_info_ptr->work_area.x + widget_move_old_x_offset, 
					global_wm.active_win_info_ptr->work_area.y + widget_move_old_y_offset);
			}

			widget_move_old_x_offset = 0;
			widget_move_old_y_offset = 0;
			widget_move_new_x_offset = 0;
			widget_move_new_y_offset = 0;

			title_bar_press = 0;
		}
	}

	return 0;
}

/**
 * 标记边框是否被按下
 *
 * @param msg 必须是鼠标按下消息
 *
 * @return 0
 **/
static si_t handle_frame_press(union message * msg)
{
	/* 有包含窗口的用户应用程序 */
	if(global_wm.active_app_info_ptr != NULL && global_wm.active_win_info_ptr != NULL && global_wm.active_win_info_ptr->need_restore == 0)
	{
		if(msg->mouse.code == INPUT_CODE_MOUSE_L_KEY)
		{
			/* 在活动窗口的左边界内按键 */
			if(window_info_is_click_left_frame(global_wm.active_win_info_ptr, &msg->mouse.cursor_position))
			{
				l_frame_press = 1;
			}
			/* 在活动窗口的上边界内按键 */
			else if(window_info_is_click_up_frame(global_wm.active_win_info_ptr, &msg->mouse.cursor_position))
			{
				t_frame_press = 1;
			}
			/* 在活动窗口的右边界内按键 */
			else if(window_info_is_click_right_frame(global_wm.active_win_info_ptr, &msg->mouse.cursor_position))
			{
				r_frame_press = 1;
			}
			/* 在活动窗口的下边界内按键 */
			else if(window_info_is_click_down_frame(global_wm.active_win_info_ptr, &msg->mouse.cursor_position))
			{
				b_frame_press = 1;
			}
		}
	}

	return 0;
}

/**
 * 处理控件的移动
 *
 * 标记标题栏是否被按下
 *
 * @param msg 必须是鼠标移动消息
 *
 * @return 0
 **/
static si_t accumulate_widget_resize(union message * msg)
{
	struct rectangle* win_work_area = &global_wm.active_win_info_ptr->work_area;
	struct rectangle* win_area = &global_wm.active_win_info_ptr->area;

	if(global_wm.active_app_info_ptr == NULL && global_wm.active_win_info_ptr == NULL)
	{
		return 0;
	}
	/* 左边框 */
	if(l_frame_press == 1)
	{
		screen_flush(win_area->x + widget_resize_old_x_offset, win_area->y, win_area->width - widget_resize_old_x_offset, win_area->height);

		widget_resize_new_x_offset += global_wm.new_cursor.x - global_wm.old_cursor.x;

		/* 向右拉左边框：检查窗口宽是否超出最小宽度 */
		if(widget_resize_new_x_offset > 0 && win_work_area->width - widget_resize_new_x_offset < window_info_least_width(global_wm.active_win_info_ptr))
		{
			widget_resize_old_x_offset = win_work_area->width - window_info_least_width(global_wm.active_win_info_ptr);
		}
		else
		{
			widget_resize_old_x_offset = widget_resize_new_x_offset;
		}

		/* 绘制活动窗口的边框 */
		engine_draw_rectangle(global_wm.gd_handler,
			win_area->x + widget_resize_old_x_offset, win_area->y, win_area->width - widget_resize_old_x_offset, win_area->height);
	}
	/* 上边框 */
	else if(t_frame_press == 1)
	{
		screen_flush(win_area->x, win_area->y + widget_resize_old_y_offset, win_area->width, win_area->height - widget_resize_old_y_offset);

		widget_resize_new_y_offset += global_wm.new_cursor.y - global_wm.old_cursor.y;

		/* 向下拉上边框：检查窗口高度是否超出最小高度 */
		if(widget_resize_new_y_offset > 0 && win_work_area->height - widget_resize_new_y_offset < 0)
		{
			widget_resize_old_y_offset = win_work_area->height;
		}
		else
		{
			widget_resize_old_y_offset = widget_resize_new_y_offset;
		}

		/* 绘制活动窗口的边框 */
		engine_draw_rectangle(global_wm.gd_handler,
			win_area->x, win_area->y + widget_resize_old_y_offset, win_area->width, win_area->height - widget_resize_old_y_offset);
	}
	/* 右边框 */
	else if(r_frame_press == 1)
	{
		screen_flush(win_area->x, win_area->y, win_area->width + widget_resize_old_x_offset, win_area->height);

		widget_resize_new_x_offset += global_wm.new_cursor.x - global_wm.old_cursor.x;

		/* 向左拉右边框：检查窗口宽是否超出最小宽度 */
		if(widget_resize_new_x_offset < 0 && win_work_area->width + widget_resize_new_x_offset < window_info_least_width(global_wm.active_win_info_ptr))
		{
			widget_resize_new_x_offset = window_info_least_width(global_wm.active_win_info_ptr) - win_work_area->width;
		}
		else
		{
			widget_resize_old_x_offset = widget_resize_new_x_offset;
		}

		/* 绘制活动窗口的边框 */
		engine_draw_rectangle(global_wm.gd_handler,
			win_area->x, win_area->y, win_area->width + widget_resize_old_x_offset, win_area->height);
	}
	/* 下边框 */
	else if(b_frame_press == 1)
	{
		screen_flush(win_area->x, win_area->y, win_area->width, win_area->height + widget_resize_old_y_offset);

		widget_resize_new_y_offset += global_wm.new_cursor.y - global_wm.old_cursor.y;

		/* 向上拉下边框：检查窗口高度是否超出最小高度 */
		if(widget_resize_new_y_offset < 0 && win_work_area->height + widget_resize_new_y_offset < 0)
		{
			widget_resize_old_y_offset = 0 - win_work_area->height;
		}
		else
		{
			widget_resize_old_y_offset = widget_resize_new_y_offset;
		}

		/* 绘制活动窗口的边框 */
		engine_draw_rectangle(global_wm.gd_handler,
			win_area->x, win_area->y, win_area->width, win_area->height + widget_resize_old_y_offset);
	}

	return 0;
}

/**
 * 发送控件该变大小消息
 *
 * @param msg 必须是鼠标松开消息
 *
 * @return 0
 **/
si_t handle_widget_resize(union message * msg)
{
	struct rectangle* work_area = &global_wm.active_win_info_ptr->work_area;
	struct rectangle* area = &global_wm.active_win_info_ptr->area;
	/**
	 * 必须有活动窗口
	 **/
	if(global_wm.active_app_info_ptr == NULL && global_wm.active_win_info_ptr == NULL)
	{
		return 0;
	}
	/* 只处理松开消息 */
	if(msg->mouse.code != INPUT_CODE_MOUSE_L_KEY)
	{
		return 0;
	}

	if(l_frame_press == 1)
	{
		send_widget_resize_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)global_wm.active_win_info_ptr,
			work_area->x + widget_resize_old_x_offset, work_area->y, work_area->width - widget_resize_old_x_offset, work_area->height);

		screen_flush(area->x + widget_resize_old_x_offset, area->y, area->width - widget_resize_old_x_offset, area->height);
	}
	else if(t_frame_press == 1)
	{
		send_widget_resize_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)global_wm.active_win_info_ptr,
			work_area->x, work_area->y + widget_resize_old_y_offset, work_area->width, work_area->height - widget_resize_old_y_offset);

		screen_flush(area->x, area->y + widget_resize_old_y_offset, area->width, area->height - widget_resize_old_y_offset);
	}
	else if(r_frame_press == 1)
	{
		send_widget_resize_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)global_wm.active_win_info_ptr,
			work_area->x, work_area->y, work_area->width + widget_resize_old_x_offset, work_area->height);

		screen_flush(area->x, area->y, area->width + widget_resize_old_x_offset, area->height);
	}
	else if(b_frame_press == 1)
	{
		send_widget_resize_message(&global_wm.active_app_info_ptr->uds, msg, (si_t)global_wm.active_win_info_ptr,
			work_area->x, work_area->y, work_area->width, work_area->height + widget_resize_old_y_offset);

		screen_flush(area->x, area->y, area->width, area->height + widget_resize_old_y_offset);
	}

	cursor_paint();

	widget_resize_new_x_offset = 0;
	widget_resize_new_y_offset = 0;
	widget_resize_old_x_offset = 0;
	widget_resize_old_y_offset = 0;

	l_frame_press = 0;
	t_frame_press = 0;
	r_frame_press = 0;
	b_frame_press = 0;

	return 0;
}

si_t window_manager_input_handler(struct egui_uds* uds_ptr, addr_t arg)
{
	struct input_device* input_device_ptr = (struct input_device*)arg;

	if(0 != input_device_ptr->deal_with_input(input_device_ptr, &global_wm.message_list))
	{
		EGUI_PRINT_ERROR("failed to deal with input");
	}

	/* 消息队列非空 */
	while(list_empty(&(global_wm.message_list)) == 0)
	{
		union message * message;
		/* 获得第一个消息 */
		message = list_front(&(global_wm.message_list));

		/* 消息中光标的位置 */
		message->base.cursor_position.x = global_wm.old_cursor.x;
		message->base.cursor_position.y = global_wm.old_cursor.y;

		/* 有包含窗口的用户应用程序 */
		if(global_wm.active_app_info_ptr != NULL && global_wm.active_win_info_ptr != NULL)
		{
			send_input_message(&global_wm.active_app_info_ptr->uds, message, (si_t)global_wm.active_win_info_ptr);
		}

		if(message->base.type == MESSAGE_TYPE_MOUSE_PRESS)
		{
			/**
			 * if not clicked in workspace area, send to desktop thread
			 **/
			if(!is_point_in_area(&message->base.cursor_position, &global_wm.work_area) && NULL != global_wm.desktop_app_ptr)
			{
				send_input_message(&global_wm.desktop_app_ptr->uds, message, 0);
			}
			else
			{
				/* 更新活动窗口 */
				mask_active_by_mouse_down(message);

				handle_desktop_press(message);

				handle_button_press(message);

				handle_title_bar_press(message);

				handle_frame_press(message);
			}
		}
		else if(message->base.type == MESSAGE_TYPE_MOUSE_RELEASE)
		{
			/**
			 * if not clicked in workspace area, send to desktop thread
			 **/
			if(!is_point_in_area(&message->base.cursor_position, &global_wm.work_area))
			{
				send_input_message(&global_wm.desktop_app_ptr->uds, message, 0);
			}
			else
			{
				handle_desktop_release(message);

				handle_button_release(message);

				handle_widget_move(message);

				handle_widget_resize(message);
			}
		}
		/* 绘制鼠标并且更新光标的坐标 */
		else if(message->base.type == MESSAGE_TYPE_MOUSE_MOVE ||
				message->base.type == MESSAGE_TYPE_MOUSE_MOVE_POINT)
		{
			if(message->mouse.code == INPUT_CODE_MOUSE_X_OFFSET)
			{
				if(message->base.type == MESSAGE_TYPE_MOUSE_MOVE) {
					global_wm.new_cursor.x = global_wm.new_cursor.x + message->mouse.value;
				} else {
					global_wm.new_cursor.x = message->mouse.value;
				}

				if(global_wm.new_cursor.x < 0)
				{
					global_wm.new_cursor.x = 0;
				}
				else if(global_wm.new_cursor.x >= global_screen.width)
				{
					global_wm.new_cursor.x = global_screen.width - 1;
				}
			}
			else if(message->mouse.code == INPUT_CODE_MOUSE_Y_OFFSET)
			{
				if(message->base.type == MESSAGE_TYPE_MOUSE_MOVE) {
					global_wm.new_cursor.y = global_wm.new_cursor.y + message->mouse.value;
				} else {
					global_wm.new_cursor.y = message->mouse.value;
				}

				if(global_wm.new_cursor.y < 0)
				{
					global_wm.new_cursor.y = 0;
				}
				else if(global_wm.new_cursor.y >= global_screen.height)
				{
					global_wm.new_cursor.y = global_screen.height - 1;
				}
			}

			accumulate_widget_move(message);
			accumulate_widget_resize(message);

			/* 更新鼠标下面的内容 */
			screen_flush(global_wm.old_cursor.x - 7, global_wm.old_cursor.y - 7, 15, 15);
			cursor_paint();

			global_wm.old_cursor = global_wm.new_cursor;
		}
		/* 删除已分发的消息 */
		list_pop_front(&(global_wm.message_list));
	}

	return 0;
}

void window_manager_input_destructor(struct egui_uds* uds, addr_t arg)
{
	struct input_device* id_ptr = arg;
	if(0 != id_ptr->input_exit(id_ptr))
	{
		EGUI_PRINT_ERROR("failed to exit input device");
	}
}
