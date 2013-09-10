/**
 * @file scrool_bar.c
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

#include <stdio.h>
#include <stdlib.h>

#include "base_type.h"
#include "config_parser.h"
#include "log.h"
#include "client_lib.h"

#include "scroll_bar.h"

#define TMP_ARRAY_SIZE 256

/* 滚动条样式全局对象 */
static struct scroll_bar_style scroll_bar_default_style = 
{
	0,  /* .flag */

	/* 默认工作区域 */
	0,      /* .area_x */
	0,      /* .area_y */
	0,      /* .area_width */
	0,      /* .area_height */

	/* 默认边界宽度 */
	1,      /* .border_size */

	/* 默认宽度/高度 */
	0,      /* .maximum_width */
	0,      /* .minimum_width */
	0,      /* .maximum_height */
	0,      /* .minimum_height */

	/* 默认鼠标形状 */
	CURSOR_SHAPE_X, /* .cursor */

	/* 背景色 */
	/* FIXME:should not be hard code */
	0xcb,	/* .back_color_r */
	0xf3,	/* .back_color_g */
	0xfb,	/* .back_color_b */
	0,      /* .back_color_a */

	/* 前景色 */
	/* FIXME:should not be hard code */
	0x46,   /* .fore_color_r */
	0xa5,   /* .fore_color_g */
	0xe5,   /* .fore_color_b */
	0,      /* .fore_color_a */

	/* outterframe颜色 */
	/* FIXME:should not be hard code */
	120,    /* .outterframe_color_r */
	120,    /* .outterframe_color_g */
	120,    /* .outterframe_color_b */
	0,      /* .outterframe_color_a */

	/* innerframe颜色 */
	/* FIXME:should not be hard code */
	0x18,   /* .innerframe_color_r */
	0x29,   /* .innerframe_color_g */
	0x45,   /* .innerframe_color_b */
	0,      /* .innerframe_color_a */
};

/**
 * @brief 用scroll_bar全局样式对象初始化scroll_bar对象
 *
 * @param style scroll_bar样式对象指针
 * @param sb scroll_bar指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t scroll_bar_init_with_default_style(struct scroll_bar * sb)
{
	char tmp_str[TMP_ARRAY_SIZE] = {'\0'};
	si_t tmp_int = -1;
	/* scroll_bar全局样式对象指针 */
	struct scroll_bar_style * style = &scroll_bar_default_style;

	/* 如果scroll_bar全局样式对象尚未加载配置 */
	if(!style->flag)
	{
		struct config_parser parser;
        const char *SCROLL_BAR_STYLE_FILE = get_config_path("scroll_bar.cfg");

		/* 初始化配置处理器 */
		if(config_parser_init(SCROLL_BAR_STYLE_FILE, &parser) != 0)
		{
			EGUI_PRINT_ERROR("fail to init scroll_bar style from config file %s.", SCROLL_BAR_STYLE_FILE);

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

		config_parser_get_int(&parser, "outterframe_color_r", &(style->outterframe_color_r));
		config_parser_get_int(&parser, "outterframe_color_g", &(style->outterframe_color_g));
		config_parser_get_int(&parser, "outterframe_color_b", &(style->outterframe_color_b));
		config_parser_get_int(&parser, "outterframe_color_a", &(style->outterframe_color_a));

		config_parser_get_int(&parser, "innerframe_color_r", &(style->innerframe_color_r));
		config_parser_get_int(&parser, "innerframe_color_g", &(style->innerframe_color_g));
		config_parser_get_int(&parser, "innerframe_color_b", &(style->innerframe_color_b));
		config_parser_get_int(&parser, "innerframe_color_a", &(style->innerframe_color_a));

		/* 退出配置处理器 */
		config_parser_exit(&parser);
		style->flag = 1;
	}

	/* 用scroll_bar默认样式初始化scroll_bar各样式属性 */
	sb->area.x = style->area_x;
	sb->area.y = style->area_y;
	sb->area.width = style->area_width;
	sb->area.height = style->area_height;

	sb->border_size = style->border_size;

	sb->maximum_width = style->maximum_width;
	sb->minimum_width = style->minimum_width;
	sb->maximum_height = style->maximum_height;
	sb->minimum_height = style->minimum_height;

	sb->cursor = style->cursor;

	sb->back_color.r = style->back_color_r;
	sb->back_color.g = style->back_color_g;
	sb->back_color.b = style->back_color_b;
	sb->back_color.a = style->back_color_a;

	sb->fore_color.r = style->fore_color_r;
	sb->fore_color.g = style->fore_color_g;
	sb->fore_color.b = style->fore_color_b;
	sb->fore_color.a = style->fore_color_a;

	sb->outterframe_color.r = style->outterframe_color_r;
	sb->outterframe_color.g = style->outterframe_color_g;
	sb->outterframe_color.b = style->outterframe_color_b;
	sb->outterframe_color.a = style->outterframe_color_a;

	sb->innerframe_color.r = style->innerframe_color_r;
	sb->innerframe_color.g = style->innerframe_color_g;
	sb->innerframe_color.b = style->innerframe_color_b;
	sb->innerframe_color.a = style->innerframe_color_a;


	return 0;
}

static si_t scroll_bar_default_widget_show(struct scroll_bar* s, union message* msg)
{
	struct rectangle area;

	widget_absolute_area(WIDGET_POINTER(s), &area);

	/* 设置区域 */
	set_area(s->gd, area.x, area.y, area.width, area.height);

	/* 更新 */
	update(s->gd);

	return 0;
}

static si_t scroll_bar_default_widget_repaint(struct scroll_bar* s, union message* msg)
{
	struct rectangle area;
	struct rectangle elevator_area;
	si_t x, y;

	/* 获得左上角的绝对坐标 */
	widget_absolute_coordinate(WIDGET_POINTER(s), &x, &y);

	/* 获得绝对的工作区域 */
	/* 将会舍弃不在父控件内的部分*/
	widget_absolute_area(WIDGET_POINTER(s), &area);

	set_area(s->gd, area.x, area.y, area.width, area.height);

	/**
	 * backgroud
	 **/
	/**
	 * background
	 **/
	set_color(s->gd, s->back_color.r, s->back_color.g, s->back_color.b, s->back_color.a);
	fill_rectangle(s->gd, area.x, area.y, area.width, area.height);
	/**
	 * frame of the scroll_bar
	 **/
	set_color(s->gd, s->outterframe_color.r, s->outterframe_color.g, s->outterframe_color.b, s->outterframe_color.a);
	draw_rectangle(s->gd, area.x, area.y, area.width, area.height);

	/* 每次都先更新一下对应的值 */
	scroll_bar_set_view(s, s->total_len, s->cur_offset);
	if(s->elevator_height == 0)
		return 0;

	/* 画上下箭头所在的区域 */
	set_color(s->gd, s->outterframe_color.r, s->outterframe_color.g, s->outterframe_color.b, s->outterframe_color.a);
	/* 画边框 */
	if(s->is_vertical)
	{
		draw_rectangle(s->gd, area.x, area.y, area.width, area.width);
		draw_rectangle(s->gd, area.x, area.y + area.height - area.width, area.width, area.width);
	}
	else
	{
		draw_rectangle(s->gd, area.x, area.y, area.height, area.height);
		draw_rectangle(s->gd, area.x + area.width - area.height, area.y, area.height, area.height);
	}
	/* 画上下箭头 */
	set_color(s->gd, s->fore_color.r, s->fore_color.g, s->fore_color.b, s->fore_color.a);
	if(s->is_vertical)
	{
		fill_arrow(s->gd, area.x + (area.width >> 1), area.y + area.width - (area.width >> 2), area.width >> 2, 1);
		fill_arrow(s->gd, area.x + (area.width >> 1), area.y + area.height - (area.width - (area.width >> 2)), area.width >> 2, 2);
	}
	else
	{
		fill_arrow(s->gd, area.x + area.height - (area.height >> 2), area.y + (area.height >> 1), area.height >> 2, 3);
		fill_arrow(s->gd, area.x + area.width - (area.height - (area.height >> 2)), area.y + (area.height >> 1), area.height >> 2, 4);
	}

	/** elevator的绝对坐标 **/
	if(s->is_vertical)
	{
		rectangle_set(&elevator_area, area.x, area.y + s->area.width + s->elevator_offset, area.width, s->elevator_height);
	}
	else
	{
		rectangle_set(&elevator_area, area.x + s->area.height + s->elevator_offset, area.y, s->elevator_height, area.height);
	}

	/* 画elevator */
	/* 背景色 */
	set_color(s->gd, s->fore_color.r, s->fore_color.g, s->fore_color.b, s->fore_color.a);
	fill_rectangle(s->gd, elevator_area.x, elevator_area.y, elevator_area.width, elevator_area.height);
	/* 边框 */
	set_color(s->gd, s->innerframe_color.r, s->innerframe_color.g, s->innerframe_color.b, s->innerframe_color.a);
	draw_rectangle(s->gd, elevator_area.x, elevator_area.y, elevator_area.width, elevator_area.height);

	return 0;
}


/**
 * function that will be called in list_for_each for each subscribe_info of the scroll_bar
 **/
static si_t event_to_be_dispatched;
static si_t do_handle_event(void* subscribe_info)
{
	struct subscribe_info* si = (struct subscribe_info*)subscribe_info;
	if(si->event == SCROLL_BAR_EVENT_ALL || si->event == event_to_be_dispatched)
		si->handler(si->subscriber, si->publisher, event_to_be_dispatched);
	return 0;
}


static si_t scroll_bar_default_mouse_press(struct scroll_bar* s, union message* msg)
{
	struct rectangle area;
	si_t scroll_bar_length = s->is_vertical ? s->area.height : s->area.width;
	si_t arrow_length = s->is_vertical ? s->area.width : s->area.height;
	si_t new_offset;

	widget_absolute_area(WIDGET_POINTER(s), &area);
	if(s->is_vertical)
		s->click_pos = msg->mouse.cursor_position.y - area.y;
	else
		s->click_pos = msg->mouse.cursor_position.x - area.x;

	/**
	 * five area: 
	 * pressed on up arrow area: up a line, directly;
	 **/
	if(s->click_pos < arrow_length)
	{
		new_offset = s->cur_offset - s->line_height;
		if(new_offset < 0)
			event_to_be_dispatched = SCROLL_BAR_EVENT_CHANGE;
		else
			event_to_be_dispatched = SCROLL_BAR_EVENT_LINE_UP;
	}
	/**
	 * pressed above elevator: up a screen, directly;
	 **/
	else if(s->click_pos < s->elevator_offset + arrow_length)
	{
		new_offset = s->cur_offset - scroll_bar_length;
		if(new_offset < 0)
			event_to_be_dispatched = SCROLL_BAR_EVENT_CHANGE;
		else
			event_to_be_dispatched = SCROLL_BAR_EVENT_PAGE_UP;
	}
	/**
	 * pressed on elevator: record and wait
	 **/
	else if(s->click_pos < s->elevator_offset + arrow_length + s->elevator_height)
	{
		return 0;
	}
	/**
	 * press below elevator: down a screen, directly;
	 **/
	else if(s->click_pos < scroll_bar_length - arrow_length)
	{
		new_offset = s->cur_offset + scroll_bar_length;
		if(new_offset > s->total_len - scroll_bar_length)
			event_to_be_dispatched = SCROLL_BAR_EVENT_CHANGE;
		else
			event_to_be_dispatched = SCROLL_BAR_EVENT_PAGE_DOWN;
	}
	/**
	 * press on down arrow area: down a line, directly;
	 **/
	else
	{
		new_offset = s->cur_offset + s->line_height;
		if(new_offset > s->total_len - scroll_bar_length)
			event_to_be_dispatched = SCROLL_BAR_EVENT_CHANGE;
		else
			event_to_be_dispatched = SCROLL_BAR_EVENT_LINE_DOWN;
	}
	s->click_pos = -1;
	scroll_bar_update_offset(s, new_offset);

	/**
	 * dispatch event to every widget that asked for notified when event occured
	 **/
	list_for_each(&s->subscribe_info_list, do_handle_event);

	return 0;
}

static si_t scroll_bar_default_mouse_release(struct scroll_bar* s, union message* msg)
{
	struct rectangle area;
	si_t distance;
	si_t real_distance;

	widget_absolute_area(WIDGET_POINTER(s), &area);

	/**
	 * only dispatch one event
	 * pressed on elevator: record and wait
	 **/
	if(s->click_pos == -1)
	{
		return 0;
	}

	if(s->is_vertical)
		distance = msg->mouse.cursor_position.y - area.y - s->click_pos ;
	else
		distance = msg->mouse.cursor_position.x - area.x - s->click_pos ;
	s->click_pos = -1;

	event_to_be_dispatched = SCROLL_BAR_EVENT_CHANGE;
	/**
	 * distance in real : distance = total_len : scroll_bar_len
	 **/
	if(s->is_vertical)
		real_distance = distance * s->total_len / area.height;
	else
		real_distance = distance * s->total_len / area.width;
	scroll_bar_update_offset(s, real_distance + s->cur_offset);

	/**
	 * dispatch event to every widget that asked for notified when event occured
	 **/
	list_for_each(&s->subscribe_info_list, do_handle_event);

	return 0;
}

si_t scroll_bar_default_callback(void* s, void* m)
{
	struct scroll_bar* self = s;
	union message* msg = m;

	switch(msg->base.type)
	{
	case MESSAGE_TYPE_WIDGET_SHOW:
		scroll_bar_default_widget_show(self, msg);
		break;

	case MESSAGE_TYPE_WIDGET_REPAINT:
		scroll_bar_default_widget_repaint(self, msg);
		scroll_bar_default_widget_show(self, msg);
		break;

	case MESSAGE_TYPE_MOUSE_PRESS:
		scroll_bar_default_mouse_press(self, msg);
		scroll_bar_default_widget_repaint(self, msg);
		scroll_bar_default_widget_show(self, msg);
		break;

	case MESSAGE_TYPE_MOUSE_RELEASE:
		scroll_bar_default_mouse_release(self, msg);
		scroll_bar_default_widget_repaint(self, msg);
		scroll_bar_default_widget_show(self, msg);
		break;

	default:
		break;

	}
	return 0;
}

void scroll_bar_repaint(struct scroll_bar* s)
{
	widget_repaint(WIDGET_POINTER(s));
}

void scroll_bar_show(struct scroll_bar* s)
{
	widget_show(WIDGET_POINTER(s));
}

struct scroll_bar* scroll_bar_init(si_t is_vertical, si_t total_len, si_t line_height)
{
	struct scroll_bar* addr;

	/* 分配存储空间 */
	if((addr = (struct scroll_bar*)malloc(sizeof(struct scroll_bar))) == NULL)
	{
		EGUI_PRINT_SYS_ERROR("fail to malloc");
		return NULL;
	}

    if(!(addr=widget_init_common(WIDGET_POINTER(addr), 0))) {
        return NULL;
    }

	addr->name = "struct scroll_bar";

	/* 默认是否可见 */
	addr->visible = 0;

	/* 默认是否拥有键盘焦点*/
	addr->keybd_focus = 1;

	/* 用全局样式对象初始化scroll_bar样式 */
	scroll_bar_init_with_default_style(addr);

	/* 默认的回调函数 */
	addr->callback = scroll_bar_default_callback;

	/**
	 * for scroll_bar
	 **/
	addr->is_vertical = is_vertical;

	/**
	 * don't draw elevator
	 **/
	addr->elevator_offset = 0;
	addr->elevator_height = 0;

	addr->click_pos = -1;
	addr->cur_offset = 0;

	list_init(&addr->subscribe_info_list);

	addr->total_len = total_len;
	addr->line_height = line_height;

	return addr;
}

si_t scroll_bar_exit(struct scroll_bar* s)
{
	list_exit(&s->subscribe_info_list);
	return widget_exit(WIDGET_POINTER(s));
}

void scroll_bar_register_move_handler(struct scroll_bar* s, struct widget* w, si_t event, void(* handler)(struct widget*, struct widget*, si_t))
{
	struct subscribe_info si;
	si.subscriber = w;
	si.publisher = WIDGET_POINTER(s);
	si.event = event;
	si.handler = handler;
	list_push_back(&s->subscribe_info_list, &si, sizeof(struct subscribe_info));
}

void scroll_bar_set_horizonal(struct scroll_bar* s)
{
	s->is_vertical = 0;
}

void scroll_bar_set_vertical(struct scroll_bar* s)
{
	s->is_vertical = 1;
}

void scroll_bar_set_line_height(struct scroll_bar* s, si_t line_height)
{
	s->line_height = line_height;
}

void scroll_bar_set_view(struct scroll_bar* s, si_t total_len, si_t current_offset)
{
	s->total_len = total_len;
	if(s->is_vertical && s->total_len < s->area.height)
		s->total_len = s->area.height;
	else if(!s->is_vertical && s->total_len < s->area.width)
		s->total_len = s->area.width;
	scroll_bar_update_offset(s, current_offset);
}

void scroll_bar_update_offset(struct scroll_bar* s, si_t new_offset)
{
	si_t scroll_bar_length = s->is_vertical ? s->area.height : s->area.width;
	si_t arrow_length = s->is_vertical ? s->area.width : s->area.height;

	if(new_offset < 0)
	{
		s->cur_offset = 0;
	}
	else if(new_offset > s->total_len - scroll_bar_length)
	{
		s->cur_offset = s->total_len - scroll_bar_length;
	}
	else
	{
		s->cur_offset = new_offset;
	}

	/**
	 * calculate current bar
	 **/
	/**
	 * height of the elevator : total height of the building(scroll bar length - arrow length)
	 * = length of the scroll bar : total length of the scroll area
	 **/
	s->elevator_height = scroll_bar_length * (scroll_bar_length - 2 * arrow_length) / s->total_len;
	/**
	 * elevator offset : total height of the building
	 * = scroll area offset : total length of the scroll area
	 **/
	s->elevator_offset = (scroll_bar_length - 2 * arrow_length) * s->cur_offset / s->total_len;
}

void scroll_bar_set_bounds(struct scroll_bar* s, si_t x, si_t y, si_t width , si_t height)
{
	widget_set_bounds(WIDGET_POINTER(s), x, y, width, height);
}

void scroll_bar_set_colors(struct scroll_bar* s, struct color* fcolor, struct color* bcolor, struct color* ocolor, struct color* icolor)
{
	widget_set_color(WIDGET_POINTER(s), fcolor, bcolor);
	s->innerframe_color = (NULL == icolor ? s->innerframe_color : *icolor);
	s->outterframe_color = (NULL == ocolor ? s->outterframe_color : *ocolor);
}
