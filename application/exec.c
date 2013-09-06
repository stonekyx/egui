/**
 * @file exec.c
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

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>

# include "geometry.h"
# include "comm.h"
# include "widget.h"
# include "log.h"

# include "application.h"
# include "manipulate_window.h"

/**
 * traverse the whole window vector and widget tree decreamentally
 *
 * @param do_for_each_widget: function pointer which uses the each widget and the msg as parameter
 * @param msg: pointer to message
 * @return: NULL if traversal all, otherwise returns the specific widget that stopped the traversal
 **/
static struct widget* application_widgets_for_each_decreament(struct widget*(*do_for_each_widget)(struct widget*, union message*), union message* msg)
{
    int n, i;
    struct object* tree, *node;
    n = vector_size(&(global_application.window_vector));

    for(i = n - 1; i >= 0; -- i)
    {
        tree = vector_at(&(global_application.window_vector), i);
        node = tree->rchild;
        while(node != NULL)
        {
            if(do_for_each_widget(WIDGET_POINTER(node), msg))
                return WIDGET_POINTER(node);
            node = object_tree_iterator_decrement(tree, node);
        }
    }
    return NULL;
}

/**
 * traverse the whole window vector and widget tree increamentally
 *
 * @param do_for_each_widget: function pointer which uses the each widget as parameter
 * @param msg: pointer to message
 * @return: NULL if traversal all, otherwise returns the specific widget that stopped the traversal
 **/
static struct widget* application_widgets_for_each_increament(struct widget*(*do_for_each_widget)(struct widget*, union message*), union message* msg)
{
    int n, i;
    struct object* tree, *node;
    n = vector_size(&(global_application.window_vector));

    for(i = 0; i != n; ++ i)
    {
        tree = vector_at(&(global_application.window_vector), i);
        node = tree->lchild;
        while(node != NULL)
        {
            if(do_for_each_widget(WIDGET_POINTER(node), msg))
                return WIDGET_POINTER(node);
            node = object_tree_iterator_increment(tree, node);
        }
    }
    return NULL;
}

/**
 * dispatch simple message for all widget
 * 
 * @param w: widget
 * @param msg: pointer to message
 **/
static struct widget* do_dispatch_simple_message_to_all(struct widget* w, union message* msg)
{
    if(w->callback != NULL)
    {
        w->callback(w, msg);
    }
    return NULL;
}

/**
 * dispatch repaint event, which requires some extra operations
 *
 * @param w: pointer to the widget
 * @param msg: no use
 * @return: NULL
 **/
static si_t application_update_keybd_focus(struct widget* w);
static struct widget* do_dispatch_repaint_event_to_all(struct widget* w, union message* msg)
{
    if(w->callback != NULL)
    {
        union message msg;
        msg.widget_repaint.type = MESSAGE_TYPE_WIDGET_REPAINT;
        msg.widget_repaint.area = w->area;

        w->callback(w, &msg);
    }
    return NULL;
}


/**
 * dispatch mouse event, which requires some extra operations
 * 
 * @param w: pointer to the widget
 * @param msg: pointer to message
 * @return: 1 if found and dispatched, 0 if not found
 **/
static struct widget* do_dispatch_mouse_event_and_returns(struct widget* w, union message* msg)
{
    struct rectangle area;

    /**
     * calculate absolute area of the widget
     **/
    if(w->is_window == 1)
    {
        area = w->area;
    }
    else
    {
        widget_absolute_area(w, &area);
    }

    /**
     * if cursor position in widget area
     **/
    if(msg->mouse.cursor_position.x >= area.x 
        && msg->mouse.cursor_position.x <= area.x + area.width - 1 
        && msg->mouse.cursor_position.y >= area.y 
        && msg->mouse.cursor_position.y <= area.y + area.height - 1)
    {
        if(w->callback != NULL)
        {
            w->callback(w, msg);
        }
        /**
         * update widget keybd focus
         **/
        application_update_keybd_focus(w);
        return w; 
    }

    return NULL;
}

/**
 * dispatch keybd event, which requires some extra operations
 *
 * @param w: widget
 * @param msg: pointer to the message
 **/
static struct widget* do_dispatch_keybd_event_and_returns(struct widget* w, union message* msg)
{
    struct rectangle area;

    if(w->is_window == 1)
    {
        area = w->area;
    }
    else
    {
        widget_absolute_area(w, &area);
    }

    /**
     * only if this widget have keyboard focus
     **/
    if(w->input_enable && w->keybd_focus)
    {
        if(w->callback != NULL)
        {
            w->callback(w, msg);
        }
        return w;
    }

    return NULL;
}

/**
 * parse each widget
 *
 * @param w: pointer to widget
 * @param m: no use
 * @return: NULL
 **/
static struct widget* do_parse_widget(struct widget* w, union message* m)
{
    /**
     * record focusable widget
     **/
    if(w->input_enable == 1)
    {
        list_push_back(&global_application.focus_list, &w, sizeof(w));
    }

    /**
     * register window and update active window
     **/
    if(w->is_window)
    {
        struct window* parent = (struct window*)object_parent(OBJECT_POINTER(w));
		struct window* window_ptr = WINDOW_POINTER(w);
		si_t parent_descriptor = (parent == NULL ? 0 : parent->descriptor);
		si_t window_descriptor = register_window(parent_descriptor, window_ptr->title, 
				window_ptr->area.x, window_ptr->area.y, window_ptr->area.width, window_ptr->area.height, 
				window_ptr->minimize_enable, window_ptr->maximize_enable, window_ptr->modal);
		if(0 == window_descriptor)
		{
			EGUI_PRINT_ERROR("failed to register window %s", window_ptr->title);
			return NULL;
		}
		window_ptr->descriptor = window_descriptor;
        global_application.focus = window_ptr;

        /**
         * find icon for the application
         **/
        if((window_ptr->icon_path = (char*)malloc(256)) == NULL)
        {
			EGUI_PRINT_ERROR("failed to register window");
			return NULL;
        }
        else
        {
            /**
             * in C89 standard, snprintf() is NOT included in <stdio.h>
             * so you have to use sprintf, which may be dangerous. be careful
             **/
            sprintf(window_ptr->icon_path, "%s/.icons/%s.bmp", global_application.icon_root_path, global_application.name);
            if(access(window_ptr->icon_path, R_OK) == -1)
            {
                sprintf(window_ptr->icon_path, "%s/.icons/default.bmp", global_application.icon_root_path);
            }
        }
    }

    return NULL;
}

/**
 * find the specific window and returns
 * 
 * @param msg: pointer to msg
 * @return: 1 if found
 **/
static struct widget* do_find_window(struct widget* w, union message* msg)
{
    if(w->is_window && msg->base.window_descriptor == WINDOW_POINTER(w)->descriptor)
        return w;
    return NULL;
}

/**
 * dispatch msg to all subchild of a widget
 * 
 * @param node: the widget
 * @param type: msg type
 * @return: 0
 **/
static si_t dispatch_msg_to_subchilds(struct object* node, int type)
{
    struct object* n, *t, *parent;

    /**
     * change the structure of the tree just for this
     **/
    n = node;
    t = n->parent;
    parent = t->parent;
    t->parent = n;
    n = object_tree_l_most_node(t);
    while(n != NULL)
    {
        union message msg;

        /**
         * pack the message
         **/
        if(type == MESSAGE_TYPE_WINDOW_DEACTIVATE)
        {
            msg.base.type = MESSAGE_TYPE_WINDOW_DEACTIVATE;
        }
        else if(type == MESSAGE_TYPE_WIDGET_REPAINT)
        {
            struct widget* w = WIDGET_POINTER(n);
            msg.widget_repaint.type = MESSAGE_TYPE_WIDGET_REPAINT;
            msg.widget_repaint.area = w->area;
        }
        else if(type == MESSAGE_TYPE_WINDOW_MINIMIZE 
            || type == MESSAGE_TYPE_WINDOW_RESTORE)
        {
            msg.base.window_descriptor = WINDOW_POINTER(n)->descriptor;
        }

        if(WIDGET_POINTER(n)->callback != NULL)
            WIDGET_POINTER(n)->callback(n, &msg);
        n = object_tree_iterator_increment(t, n);
    }
    t->parent = parent;
    return 0;
}

static si_t
handle_keybd_press
(union message * msg)
{
    application_widgets_for_each_decreament(do_dispatch_keybd_event_and_returns, msg);
    return 0;
}

static si_t
handle_keybd_release
(union message * msg)
{
    application_widgets_for_each_decreament(do_dispatch_keybd_event_and_returns, msg);
    return 0;
}

static si_t
handle_keybd_click
(union message * msg)
{
    application_widgets_for_each_decreament(do_dispatch_keybd_event_and_returns, msg);
    return 0;
}

/**
 * param is a pointer to a pointer
 * because focus_list store the descriptor of widget
 * so the data pointer in focus_list node points to the descriptor of widget
 * which is also pointer of widget, so...
 * got it?
 **/
static si_t application_keybd_lose_focus(void* wptr)
{
    struct widget* w = WIDGET_POINTER(*(struct widget**)wptr);
    if(w->keybd_focus)
    {
        w->keybd_focus = 0;
        if(w->callback != NULL)
        {
            union message msg;
            msg.base.type = MESSAGE_TYPE_KEYBD_LEAVE;
            w->callback(w, &msg);
        }
    }
    return 0;
}

static si_t application_update_keybd_focus(struct widget* w)
{
    /**
     * set w lose focus first so that it will not be paint twice 
     **/
    if(w != NULL && w->input_enable)
    {
        w->keybd_focus = 0;
    }

    /**
     * set all other input_enable applications lose focus
     **/
    list_for_each(&global_application.focus_list, application_keybd_lose_focus);

    /**
     * gain focus
     **/
    if(w != NULL && w->input_enable)
    {
        w->keybd_focus = 1;
    }
    
    return 0;
}

static si_t
handle_mouse_move
(union message * msg)
{
    return 0;
}

static si_t
handle_mouse_press
(union message * msg)
{
    application_widgets_for_each_decreament(do_dispatch_mouse_event_and_returns, msg);

    return 0;
}

static si_t
handle_mouse_release
(union message * msg)
{
    application_widgets_for_each_decreament(do_dispatch_mouse_event_and_returns, msg);

    return 0;
}

static si_t
handle_mouse_single_click
(union message * msg)
{
    application_widgets_for_each_decreament(do_dispatch_mouse_event_and_returns, msg);

    return 0;
}

static si_t
handle_mouse_double_click
(union message * msg)
{
    application_widgets_for_each_decreament(do_dispatch_mouse_event_and_returns, msg);

    return 0;
}

static si_t
handle_widget_repaint
(union message * msg)
{
    application_widgets_for_each_decreament(do_dispatch_simple_message_to_all, msg);

    return 0;
}

static si_t
handle_widget_move
(union message * msg)
{
    struct widget* w;
    w = application_widgets_for_each_increament(do_find_window, msg);
    if(w != NULL && w->callback != NULL)
	{
        w->callback(w, msg);
	}

    application_widgets_for_each_increament(do_dispatch_repaint_event_to_all, NULL);

    return 0;
}

static si_t
handle_widget_resize
(union message * msg)
{
    struct widget* w;
    w = application_widgets_for_each_increament(do_find_window, msg);
    if(w->callback != NULL)
        w->callback(w, msg);

    application_widgets_for_each_increament(do_dispatch_repaint_event_to_all, NULL);

    return 0;
}

static si_t
handle_window_activate
(union message * msg)
{
	struct object* node;
	si_t n = 0, i = 0;
	node = OBJECT_POINTER(application_widgets_for_each_increament(do_find_window, msg));

	if(node == NULL)
	{
		return 0;
	}

	object_move_first(node);
	if(global_application.focus != NULL)
	{
		dispatch_msg_to_subchilds(node, MESSAGE_TYPE_WINDOW_DEACTIVATE);
	}

	global_application.focus = WINDOW_POINTER(node);
	if(WIDGET_POINTER(node)->callback != NULL)
	{
		WIDGET_POINTER(node)->callback(node, msg);
	}

	dispatch_msg_to_subchilds(node, MESSAGE_TYPE_WIDGET_REPAINT);

	/**
	 * 次序提前
	 **/
	node = object_get_root(node)->parent;
	n = vector_size(&global_application.window_vector);
	for(i = 0; i < n; ++ i)
	{
		if(vector_at(&global_application.window_vector, i) == node)
		{
			vector_move_back(&global_application.window_vector, i);
			break;
		}
	}

	return 0;
}


static si_t
handle_window_deactivate
(union message * msg)
{
    struct widget* w;

    if(global_application.focus->descriptor == msg->base.window_descriptor)
    {
        global_application.focus = NULL;
    }

    w = application_widgets_for_each_increament(do_find_window, msg);
    if(w != NULL)
    {
        if(w->callback != NULL)
            w->callback(w, msg);

        dispatch_msg_to_subchilds(OBJECT_POINTER(w), MESSAGE_TYPE_WIDGET_REPAINT);

        /**
         * set every focus widget lose focus
         **/
        application_update_keybd_focus(NULL);
    }

    return 0;
}

static si_t
handle_window_minimize
(union message * msg)
{
    struct widget* w;
    w = application_widgets_for_each_increament(do_find_window, msg);
    if(w->callback != NULL)
        w->callback(w, msg);
    dispatch_msg_to_subchilds(OBJECT_POINTER(w), MESSAGE_TYPE_WINDOW_MINIMIZE);

    application_widgets_for_each_increament(do_dispatch_repaint_event_to_all, NULL);

    return 0;
}

static si_t
handle_window_maximize
(union message * msg)
{
    struct widget* w;
    w = application_widgets_for_each_increament(do_find_window, msg);
    if(w->callback != NULL)
        w->callback(w, msg);

    application_widgets_for_each_increament(do_dispatch_repaint_event_to_all, NULL);

    return 0;
}

static si_t
handle_window_restore
(union message * msg)
{
    struct widget* w;
    w = application_widgets_for_each_increament(do_find_window, msg);
    if(w->callback != NULL)
        w->callback(w, msg);
    dispatch_msg_to_subchilds(OBJECT_POINTER(w), MESSAGE_TYPE_WINDOW_RESTORE);

    application_widgets_for_each_increament(do_dispatch_repaint_event_to_all, NULL);

    return 0;
}

static si_t
handle_window_cancel
(union message * msg)
{
    struct widget* w;
    w = application_widgets_for_each_increament(do_find_window, msg);
    if(w->callback != NULL)
        w->callback(w, msg);

    /* 如果要关闭主窗口 */
    if(global_application.main_window == WINDOW_POINTER(w))
    {
        application_del_window(WINDOW_POINTER(w));
    }
    else
    {
        cancel_window(WINDOW_POINTER(w)->descriptor);
        application_del_window(WINDOW_POINTER(w));

        /* 没有顶层窗口了 */
        /* 退出程序 */
        /* 这是为了处理没有设置主窗口的情况 */
        if(vector_size(&(global_application.window_vector)) == 0)
        {
            application_exit();
        }
        /* 还有顶层窗口 */
        else
        {
            application_widgets_for_each_increament(do_dispatch_repaint_event_to_all, NULL);
        }
    }

    return 0;
}

static void application_handle_message(union message* msg)
{
	/**
	 * 如果是桌面程序 那么首先将消息发给桌面的回调函数处理
	 **/
	if(global_application.application_type & APPLICATION_TYPE_DESKTOP)
	{
		if(global_application.desktop_msg_handler != NULL)
		{
			global_application.desktop_msg_handler(global_application.desktop_ptr, msg);
		}
	}

    switch(msg->base.type)
    {
    case MESSAGE_TYPE_KEYBD_PRESS:
        handle_keybd_press(msg);
        break;

    case MESSAGE_TYPE_KEYBD_RELEASE:
        handle_keybd_release(msg);
        break;

    case MESSAGE_TYPE_KEYBD_CLICK:
        handle_keybd_click(msg);
        break;

    case MESSAGE_TYPE_MOUSE_MOVE:
        handle_mouse_move(msg);
        break;

    case MESSAGE_TYPE_MOUSE_PRESS:
        handle_mouse_press(msg);
        break;

    case MESSAGE_TYPE_MOUSE_RELEASE:
        handle_mouse_release(msg);
        break;

    case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
        handle_mouse_single_click(msg);
        break;

    case MESSAGE_TYPE_MOUSE_DOUBLE_CLICK:
        handle_mouse_double_click(msg);
        break;

    case MESSAGE_TYPE_WIDGET_REPAINT:
        handle_widget_repaint(msg);
        break;

    case MESSAGE_TYPE_WIDGET_MOVE:
        handle_widget_move(msg);
        break;

    case MESSAGE_TYPE_WIDGET_RESIZE:
        handle_widget_resize(msg);
        break;

    case MESSAGE_TYPE_WINDOW_ACTIVATE:
        handle_window_activate(msg);
        break;

    case MESSAGE_TYPE_WINDOW_DEACTIVATE:
        handle_window_deactivate(msg);
        break;

    case MESSAGE_TYPE_WINDOW_MINIMIZE:
        handle_window_minimize(msg);
        break;

    case MESSAGE_TYPE_WINDOW_MAXIMIZE:
        handle_window_maximize(msg);
        break;

    case MESSAGE_TYPE_WINDOW_RESTORE:
        handle_window_restore(msg);
        break;

    case MESSAGE_TYPE_WINDOW_CANCEL:
        handle_window_cancel(msg);
        break;

    default:
        application_widgets_for_each_increament(do_find_window, msg);
        break;
    }
}

si_t application_event_handler(struct egui_uds* uds_ptr, addr_t arg)
{
	union message msg;

	if(0 != comm_recv_msg(uds_ptr, &msg))
	{
		EGUI_PRINT_ERROR("failed to recv msg");
		return SELECTER_RETURN_TYPE_CONTINUE;
	}

	application_handle_message(&msg);

	/**
	 * 处理器消息过程中，应用程序需要发送一些请求，等待回应的过程中，窗口管理器可能会发送消息。
	 * 因此当请求处理完毕之后，再逐一处理消息。
	 **/
	while(!queue_empty(&global_application.message_queue))
	{
		union message* msg = queue_front(&global_application.message_queue);
		application_handle_message(msg);
		queue_pop(&global_application.message_queue);
	}

	return SELECTER_RETURN_TYPE_CONTINUE;
}


/**
 * calculate the relationship between all the windows
 * extract all the widgets that ask for keyboard focus
 *
 **/
si_t application_parse()
{
    application_widgets_for_each_increament(do_parse_widget, NULL);

    /**
     * some application(for example, our shutdown program) may not even have a widget
     **/
    if(global_application.focus != NULL)
    {
        union message activate_msg;
        activate_msg.window_activate.type = MESSAGE_TYPE_WINDOW_ACTIVATE;
        /**
         * update active window
         **/
        global_application.focus->is_activated = 0;
        if(global_application.focus->callback != NULL)
        {
            global_application.focus->callback(global_application.focus, &activate_msg);
        }
    }

    return 0;
}

si_t application_exec()
{
    /**
     * parse the application before execute
     **/
    application_parse();

	event_listener_add_read_event(&global_application.app_event_listener, &global_application.uds, NULL, application_event_handler, NULL);

    printf("%lu\n", queue_size(&global_application.message_queue));
	while(!queue_empty(&global_application.message_queue))
	{
		union message* msg = queue_front(&global_application.message_queue);
        printf("%ld\n", msg->base.type);
		application_handle_message(msg);
		queue_pop(&global_application.message_queue);
	}

	global_application.exec_flag = 1;
	if(0 != event_listener_exec(&global_application.app_event_listener))
	{
		EGUI_PRINT_ERROR("failed to run event_listener.");
		return -1;
	}
	global_application.exec_flag = 0;

    return 0;
}
