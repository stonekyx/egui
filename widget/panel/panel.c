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

# include "panel.h"

# define PANEL_STYLE_FILE "/home/work/.egui_style/panel.cfg"
# define TMP_ARRAY_SIZE 256

/* panel样式全局对象 */
static struct panel_style panel_default_style =
{
    /* 初始化，默认未访问 */
    0,  /* .flag */

    /* 默认工作区域 */
    0,  /* .area_x */
    0,  /* .area_y */
    0,  /* .area_width */
    0,  /* .area_height */

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
    /* FIXME:不应该hard code */
    255,    /* .back_color_r */
    255,    /* .back_color_g */
    255,  /* .back_color_b */
    0,  /* .back_color_a */

    /* 默认前景色 */
    0,  /* .fore_color_r */
    0,  /* .fore_color_g */
    0,  /* .fore_color_b */
    0,  /* .fore_color_a */
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
    char tmp_str[TMP_ARRAY_SIZE] = {'\0'};
    si_t tmp_int = -1;
    /* panel全局样式对象指针 */
    struct panel_style * style = &panel_default_style;

    /* 如果panel全局样式对象尚未加载配置 */
    if(!style->flag)
    {
        struct config_parser parser;

        /* 初始化配置处理器 */
        if(config_parser_init(PANEL_STYLE_FILE, &parser) != 0)
        {
            EGUI_PRINT_ERROR("fail to init panel style from config file %s.", PANEL_STYLE_FILE);

            return -1;
        }

        /* 从配置读取各项配置,赋予style指针 */
        config_parser_get_int(&parser, "area_x", &(style->area_x));
        config_parser_get_int(&parser, "area_y", &(style->area_y));
        config_parser_get_int(&parser, "area_width", &(style->area_width));
        config_parser_get_int(&parser, "area_height", &(style->area_height));

        config_parser_get_int(&parser, "border_size", &(style->border_size));

        config_parser_get_int(&parser, "maximum_width", &(style->maximum_width));
        config_parser_get_int(&parser, "minimum_width", &(style->minimum_width));
        config_parser_get_int(&parser, "maximum_height", &(style->maximum_height));
        config_parser_get_int(&parser, "minimum_height", &(style->minimum_height));

        config_parser_get_str(&parser, "cursor", tmp_str);
        if((tmp_int = get_cursor_enum_from_str(tmp_str)) >= 0)
        {
            style->cursor = tmp_int;
        }

        config_parser_get_int(&parser, "back_color_r", &(style->back_color_r));
        config_parser_get_int(&parser, "back_color_g", &(style->back_color_g));
        config_parser_get_int(&parser, "back_color_b", &(style->back_color_b));
        config_parser_get_int(&parser, "back_color_a", &(style->back_color_a));

        config_parser_get_int(&parser, "fore_color_r", &(style->fore_color_r));
        config_parser_get_int(&parser, "fore_color_g", &(style->fore_color_g));
        config_parser_get_int(&parser, "fore_color_b", &(style->fore_color_b));
        config_parser_get_int(&parser, "fore_color_a", &(style->fore_color_a));

        /* 退出配置处理器 */
        config_parser_exit(&parser);
        style->flag = 1;
    }

    /* 用panel默认样式初始化panel各样式属性 */
    p->area.x = style->area_x;
    p->area.y = style->area_y;
    p->area.width = style->area_width;
    p->area.height = style->area_height;

    p->border_size = style->border_size;

    p->maximum_width = style->maximum_width;
    p->minimum_width = style->minimum_width;
    p->maximum_height = style->maximum_height;
    p->minimum_height = style->minimum_height;

    p->cursor = style->cursor;

    p->back_color.r = style->back_color_r;
    p->back_color.g = style->back_color_g;
    p->back_color.b = style->back_color_b;
    p->back_color.a = style->back_color_a;

    p->fore_color.r = style->fore_color_r;
    p->fore_color.g = style->fore_color_g;
    p->fore_color.b = style->fore_color_b;
    p->fore_color.a = style->fore_color_a;

    return 0;
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

    /* 申请图形设备 */
    addr->gd = graphics_device_init(0, 0, 0, 0, 0, 0 ,0 ,0 ,0);


    /* 申请失败 */
    if(addr->gd == 0)
    {
        /* 释放存储空间 */
        free(addr);

        return NULL;
    }

    /* struct panel 的成员 */
    addr->parent = NULL;
    addr->lchild = NULL;
    addr->rchild = NULL;
    addr->name = "struct panel";
    addr->id = id;

    /* 默认是否能处理键盘输入消息 */
    addr->input_enable = 0;

    /* 默认是否可以刷新 */
    addr->update_enable = 1;

    /* 默认是否可见 */
    addr->visible = 1;

    /* 默认是否拥有键盘焦点*/
    addr->keybd_focus = 0;

    /* 默认是否是窗口 */
    addr->is_window = 0;

    /* 用全局样式对象初始化panel样式 */
    panel_init_with_default_style(addr);

	/* 默认的回调函数 */
    addr->callback = panel_default_callback ;

    return addr;
}

si_t panel_exit(struct panel * i)
{
     graphics_device_exit(i->gd);
	/*
	 vector_exit( &(i->widget_vector) );
	*/	

    free(i);

    return 1;
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

    msg.widget_repaint.area.x = pnl->area.x;
    msg.widget_repaint.area.y = pnl->area.y;
    msg.widget_repaint.area.width = pnl->area.width;
    msg.widget_repaint.area.height = pnl->area.height;

    panel_default_widget_repaint(pnl, &msg);

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

	/*  绘制panel本身的区域 */
	widget_absolute_area( WIDGET_POINTER( pnl ), &area );

    set_area(pnl->gd,area.x,area.y,area.width,area.height);

	set_color(pnl->gd, pnl->back_color.r, pnl->back_color.g, pnl->back_color.b, pnl->back_color.a);

	fill_rectangle(pnl->gd, area.x , area.y, area.width, area.height);

	/*  绘制panel子控件的区域 */
/*	tree = OBJECT_POINTER(pnl);
	node = tree->rchild;
	while(node != NULL)
	{   printf("%d ",i++);
		wgt = (struct widget*)node;
		widget_absolute_area(wgt,&area);
		printf("child_area(%d,%d,%d,%d)  ",area.x, area.y,area.width,area.height);

		wgt->callback(wgt, msg);
		node = object_tree_iterator_decrement(tree,node);
	}
*/
	return 1;
}

/* 外部API  */
si_t panel_show(struct panel * pnl)
{
  	union message msg;

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
	panel->area.x = x;
	panel->area.y = y;
	panel->area.width = width;
	panel->area.height = height;
	return;
}


