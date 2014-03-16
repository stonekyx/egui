/**
 * @file panel.c
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
 * it under the terms of the GNU General Publpnl Lpnlense as published by
 * the Free Software Foundation, either version 2 of the Lpnlense, or
 * (at your option) any later version.
 *
 * EDUGUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Publpnl Lpnlense for more details.
 *
 * You should have received a copy of the GNU General Publpnl Lpnlense
 * along with EDUGUI; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * All rights reserved.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <base_type.h>
#include <config_parser.h>
#include <log.h>
#include <comm.h>
#include <client_lib.h>
#include <graph.h>
#include "compiler.h"

# include "panel.h"

# define TMP_ARRAY_SIZE 256

/* panel样式全局对象 */
static struct panel_style panel_default_style =
{
    {
        /* 初始化，默认未访问 */
        0,  /* .flag */

        /* 默认工作区域 */
        0,  /* .area_x */
        0,  /* .area_y */
        400,  /* .area_width */
        400,  /* .area_height */

        /* 默认边界宽度 */
        0,  /* .border_size */

        /* 默认宽度&高度 */
        0,  /* .maximum_width */
        0,  /* .minimum_width */
        0,  /* .maximum_height */
        0,  /* .minimum_height */

        /* 默认鼠标形状 */
        CURSOR_SHAPE_X, /* .cursor */

        /* 默认背景色 */
        255,    /* .back_color_r */
        255,    /* .back_color_g */
        255,  /* .back_color_b */
        0,  /* .back_color_a */

        /* 默认前景色 */
        0,  /* .fore_color_r */
        0,  /* .fore_color_g */
        0,  /* .fore_color_b */
        0,  /* .fore_color_a */
    }
};

/**
 * @brief 用panel全局样式对象初始化panel对象
 *
 * @param style panel样式对象指针
 * @param p panel指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t panel_init_with_default_style(struct panel * p)
{
    char *config_path = get_config_path("panel.cfg");

    si_t res = widget_init_with_default_style(config_path,
            WIDGET_POINTER(p), &panel_default_style.common,
            NULL, 0);
    free(config_path);

    return res;
}
void * panel_init(si_t id)
{
    struct panel * addr;

    /* 分配存储空间 */
    addr = (struct panel *)malloc(sizeof(struct panel));

    if(addr == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to malloc");
        return NULL;
    }
    if(!(addr=panel_init_data(addr, id))) {
        return NULL;
    }

    return addr;
}

void * panel_init_data(struct panel *addr, si_t id)
{
    if(!(addr=widget_init_common(WIDGET_POINTER(addr), id))) {
        return NULL;
    }

    addr->name = "struct panel";

    /* 用全局样式对象初始化panel样式 */
    panel_init_with_default_style(addr);

	/* 默认的回调函数 */
    addr->callback = panel_default_callback ;

    return addr;
}

si_t panel_exit(struct panel * i)
{
	/*
	 vector_exit( &(i->widget_vector) );
	*/	

    return widget_exit(WIDGET_POINTER(i));
}

si_t point_in_area(struct point* point, struct rectangle* area)
{
    if(point->x >= area->x &&
        point->y >= area->y &&
        point->x <= area->x + area->width - 1 &&
        point->y <= area->y + area->height -1)
        return 1;
    else
        return 0;
}


/* 
void  panel_add_widget( struct panel * pnl, ewidget * w )
{
	vector_push_back( &(pnl->widget_vector), w, sizeof(ewidget) );
	return ;
}
*/

/*  
ewidget* panel_get_widget_by_index( struct panel * pnl, si_t index )
{
	ewidget * wgt ;
	wgt = (ewidget *)vector_get_item_by_index( &(pnl->widget_vector), index);
	
	return wgt;	

}
*/

	
si_t panel_default_callback(void* self , void* msg )
{
	
    struct panel* pnl = self;
    union message * m = (union message *)msg;

    widget_default_callback(pnl, m);

    switch(m->base.type)
    {
        case MESSAGE_TYPE_WIDGET_REPAINT:
            panel_default_widget_repaint(pnl, m);
            panel_default_widget_show(pnl, m);
            break;

        case MESSAGE_TYPE_WIDGET_SHOW:
            panel_default_widget_show(pnl, m);
            break;

        case MESSAGE_TYPE_MOUSE_PRESS:
            panel_default_mouse_press(pnl, m);
            panel_default_widget_show(pnl, m);
            break;

        case MESSAGE_TYPE_MOUSE_RELEASE:
            panel_default_mouse_release(pnl, m);
            panel_default_widget_show(pnl, m);
            break;

        default:
            break;
    }

    return 0;
}


/* 外部 API */
si_t panel_repaint(struct panel * pnl)
{
  	union message msg;

    msg.base.type = MESSAGE_TYPE_WIDGET_REPAINT;
    msg.widget_repaint.area.x = pnl->area.x;
    msg.widget_repaint.area.y = pnl->area.y;
    msg.widget_repaint.area.width = pnl->area.width;
    msg.widget_repaint.area.height = pnl->area.height;

    panel_default_widget_repaint(pnl, &msg);

    return 1;

}

si_t panel_repaint_with_children(struct panel *pnl)
{
    union message msg;

    msg.base.type = MESSAGE_TYPE_WIDGET_REPAINT;
    msg.widget_repaint.area.x = pnl->area.x;
    msg.widget_repaint.area.y = pnl->area.y;
    msg.widget_repaint.area.width = pnl->area.width;
    msg.widget_repaint.area.height = pnl->area.height;

    panel_default_widget_repaint(pnl, &msg);
    panel_default_widget_repaint_children(pnl, &msg);

    return 1;
}

/* 
 * panel 中，是否需要绘制panel本身这个控件呢？zhu认为需要绘制。
 * 首先绘制panel本身的区域，可用背景色。
 * 然后再从列表中 依次取出每个元素，判断每个控件的具体类型，调用控件本身对应的callback 函数
 *
 * */

si_t panel_default_widget_repaint(struct panel* pnl , union message * msg)
{
	struct rectangle area;
	NOT_USED(msg);

	/*  绘制panel本身的区域 */
	widget_absolute_area( WIDGET_POINTER( pnl ), &area );

    set_area(pnl->gd,area.x,area.y,area.width,area.height);

	set_color(pnl->gd, pnl->back_color.r, pnl->back_color.g, pnl->back_color.b, pnl->back_color.a);

	fill_rectangle(pnl->gd, area.x , area.y, area.width, area.height);

	return 1;
}

si_t panel_default_widget_repaint_children(struct panel *pnl, union message *msg)
{
	struct object *tree, *node;
	/*  绘制panel子控件的区域 */
	tree = OBJECT_POINTER(pnl);
	node = tree->rchild;
	while(node != NULL)
	{
		WIDGET_POINTER(node)->callback(node, msg);
		node = object_tree_iterator_decrement(tree,node);
	}
	return 1;
}

/* 外部API  */
si_t panel_show(struct panel * pnl)
{
  	union message msg;

    msg.base.type = MESSAGE_TYPE_WIDGET_SHOW;
    msg.widget_repaint.area.x = pnl->area.x;
    msg.widget_repaint.area.y = pnl->area.y;
    msg.widget_repaint.area.width = pnl->area.width;
    msg.widget_repaint.area.height = pnl->area.height;

    panel_default_widget_show(pnl, &msg);

    return 1;

}
/*  */
si_t panel_default_widget_show(struct panel* pnl , union message * msg)
{
	struct rectangle area;
	struct object *tree, *node;
	struct widget * wgt;

	/* 先显示panel本身的，即更新绘图设备 */
    widget_absolute_area(WIDGET_POINTER(pnl), &area);
	set_area( pnl->gd, area.x, area.y, area.width, area.height );
	update( pnl->gd );

	/* When called from default_callback with another type of message,
	 * passing it to sub-widgets may intrigue other actions. */
	msg->base.type = MESSAGE_TYPE_WIDGET_SHOW;

	/*  绘制panel子控件的区域 */
	tree = OBJECT_POINTER(pnl);
	node = tree->rchild;
	while(node != NULL)
	{
		
		wgt = (struct widget*)node;
		wgt->callback(wgt, msg);
		node = object_tree_iterator_decrement(tree,node);
	}

	return 1;
}

si_t panel_default_mouse_press(struct panel* pnl , union message * msg)
{  
	si_t flag = 0;
	struct widget * wgt;

	struct rectangle area;

	struct object * tree, *node;

	tree = OBJECT_POINTER(pnl);
	node = tree->rchild;
	while(node != NULL)
	{
		widget_absolute_area(WIDGET_POINTER(node), &area);
		if(point_in_area(&(msg->mouse.cursor_position), &area))
		{
			wgt = (struct widget*)node;
			wgt->callback(wgt, msg);
			flag = 1;
			break;
		}
		node = object_tree_iterator_decrement(tree,node);

	}
	if(flag == 0)
	{
		/* 
		 * panel doesn't have children.
		 * so ,do nothing
		 * */
	}
	return 1;
}


/* 
 * 当鼠标点击在panel上时，要具体判断鼠标地具体地位置。
 * 在application模块中，只能判断到鼠标是否位于panel整个区域内。
 * 因此，这里需要具体判断鼠标是否在某个具体地控件内。如果在，那就要调用那个控件的默认回调函数。
 * 如果确实是只点在了panel本身上面而不在其他控件上面，那么，对于panel而言，它不需要做任何操作。
 *
 **/
 
si_t panel_default_mouse_release(struct panel* pnl , union message * msg)
{
/*	panel_event_in_which_widget(pnl, msg);*/
	si_t flag = 0;
	struct widget * wgt;

	struct rectangle area;

	struct object * tree, *node ;

	tree = OBJECT_POINTER(pnl);
	node = tree->rchild;
	while(node != NULL)
	{
		widget_absolute_area(WIDGET_POINTER(node), &area);
		if(point_in_area(&(msg->mouse.cursor_position), &area))
		{
			wgt = (struct widget*)node;
			wgt->callback(wgt, msg);
			flag = 1;
			break;
		}
		node = object_tree_iterator_decrement(tree,node);

	}
	if(flag == 0)
	{
		/* 
		 * panel doesn't have children.
		 * so ,do nothing
		 * */
	}
	return 1;
}


void panel_set_bounds(struct panel* panel, si_t x, si_t y, si_t width , si_t height)
{
	widget_set_bounds(WIDGET_POINTER(panel), x, y, width, height);
}

void panel_set_color(struct panel *p, struct color *fore, struct color *back)
{
    widget_set_color(WIDGET_POINTER(p), fore, back);
}
