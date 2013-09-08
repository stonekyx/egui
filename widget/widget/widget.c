/**
 * @file  widget.c
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
# include <stdlib.h>

# include "config_parser.h"
# include "log.h"
# include "client_lib.h"

# include "widget.h"

void *widget_init_common(struct widget *addr, si_t id)
{
    /* 申请图形设备 */
    addr->gd = graphics_device_init(0, 0, 0, 0, 0, 0 ,0 ,0 ,0);

    /* 申请失败 */
    if(addr->gd == 0)

    {
        /* 释放存储空间 */
        free(addr);

        return NULL;
    }

    /* struct object 的成员 */
    addr->parent = NULL;
    addr->lchild = NULL;
    addr->rchild = NULL;
    addr->name = "struct widget";
    addr->id = id;

    /* 默认工作区域 */
    addr->area.x = 0;
    addr->area.y = 0;
    addr->area.width = 0;
    addr->area.height = 0;

    /* 默认边界宽度 */
    addr->border_size = 0;

    /* 默认最大宽度 */
    addr->maximum_width = 0; /* should use macro */
    /* 默认最大高度 */
    addr->maximum_height = 0; /* should use macro */
    /* 默认最小宽度 */
    addr->minimum_width = 0;
    /* 默认最小高度 */
    addr->minimum_height = 0;

    /* 默认鼠标形状 */
    addr->cursor = CURSOR_SHAPE_X;

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

    addr->back_color.r = 255;
    addr->back_color.g = 255;
    addr->back_color.b = 255;
    addr->back_color.a = 0;

    addr->fore_color.r = 0;
    addr->fore_color.g = 0;
    addr->fore_color.b = 0;
    addr->fore_color.a = 0;
	
    /* 默认的回调函数 */
    addr->callback = NULL;

    list_init(&addr->children);

    return addr;
}

void *widget_init(si_t id)
{
    struct widget * addr;

    /* 分配存储空间 */
    addr = (struct widget *)malloc(sizeof(struct widget));

    if(addr == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to malloc");
        return NULL;
    }
    return widget_init_common(addr, id);
}

si_t widget_exit(void * w)
{
    struct widget *sw = w;
    list_for_each(&sw->children, (si_t (*)(void*))widget_exit);

    graphics_device_exit(sw->gd);

    free(sw);

    return 0;
}

si_t widget_absolute_coordinate(struct widget * w, si_t * x, si_t * y)
{
    si_t x_offset, y_offset;
    struct object * self, * parent;

    x_offset = 0;
    y_offset = 0;

    self = OBJECT_POINTER(w);
    while((parent = self->parent) != NULL)
    {
        x_offset += WIDGET_POINTER(self)->area.x;
        y_offset += WIDGET_POINTER(self)->area.y;

        self = parent;
    }

    * x = WIDGET_POINTER(self)->area.x + x_offset;
    * y = WIDGET_POINTER(self)->area.y + y_offset;

    return 0;
}
si_t widget_absolute_area_old (struct widget * w, struct rectangle * result)
{
    struct object * self, * parent;
    struct rectangle  off_set_area;

    si_t flag = 0;

	off_set_area.x = 0;
	off_set_area.y = 0;
	off_set_area.width = 0;
	off_set_area.height = 0;


    self = OBJECT_POINTER(w);
    while((parent = self->parent) != NULL)
    {
	    /*if(area_intersection(&(WIDGET_POINTER(parent)->area),&(WIDGET_POINTER(self)->area), result) == -1)
        {
            flag = 1;
            break;
        }*/

		result->x = WIDGET_POINTER(self)->area.x;	
		result->y = WIDGET_POINTER(self)->area.y;


		off_set_area.x += result->x ;
		off_set_area.y += result->y ;
        self = parent;
    }

    if(flag == 1)
    {
        return -1;
    }
    else
    {
		
        result->x = WIDGET_POINTER(self)->area.x + off_set_area.x;
        result->y = WIDGET_POINTER(self)->area.y + off_set_area.y;
		result->width = w->area.width;
		result->height = w->area.height;
		
        return 0;
    }
}


si_t widget_absolute_area (struct widget * w, struct rectangle * result)
{
    struct object * self, * parent;
    struct rectangle temp, parent_area;
    si_t flag = 0;

    *result = w->area;

    parent_area.x = 0;
    parent_area.y = 0;

    temp.x = w->area.x;
    temp.y = w->area.y;
    temp.width = w->area.width;
    temp.height = w->area.height;

    self = OBJECT_POINTER(w);
    while((parent = self->parent) != NULL)
    {
        parent_area.width = WIDGET_POINTER(parent)->area.width;
        parent_area.height = WIDGET_POINTER(parent)->area.height;

        if(area_intersection(&parent_area, &temp, result) == -1)
        {
            flag = 1;
            break;
        }

        temp.x = (result->x += WIDGET_POINTER(parent)->area.x);
        temp.y = (result->y += WIDGET_POINTER(parent)->area.y);
        temp.width = result->width;
        temp.height = result->height;

        self = parent;
    }

    if(flag == 1) {
        return -1;
    } else {
        return 0;
    }
}

void widget_set_bounds(struct widget* w, si_t x, si_t y, si_t width , si_t height)
{
	/* 检验参数 参数若小于0则不改变对应的值 */
	w->area.x = (x >= 0 ? x : w->area.x);
	w->area.y = (y >= 0 ? y : w->area.y);
	w->area.width = (width >= 0 ? width : w->area.width);
	w->area.height = (height >= 0 ? height : w->area.height);
}

void widget_set_color(struct widget* w, struct color* fcolor, struct color* bcolor)
{
	/* 检验参数 指针若为空则不改变对应的值 */
	w->fore_color = (NULL == fcolor ? w->fore_color : *fcolor);
	w->back_color = (NULL == bcolor ? w->back_color : *bcolor);
}

void widget_repaint(struct widget* w)
{
	union message msg;
	msg.widget_repaint.type = MESSAGE_TYPE_WIDGET_REPAINT;
	msg.widget_repaint.area = w->area;
	w->callback(w, &msg);
}

void widget_show(struct widget* w)
{
	union message msg;
	msg.widget_show.type = MESSAGE_TYPE_WIDGET_SHOW;
	w->callback(w, &msg);
}

si_t get_font_enum_from_str(const char * str)
{
    if(NULL == str)
    {
        return -1;
    }

    if(0 == strcmp(str, "FONT_MATRIX_08"))
    {
        return FONT_MATRIX_08;
    }
    if(0 == strcmp(str, "FONT_MATRIX_10"))
    {
        return FONT_MATRIX_10;
    }
    if(0 == strcmp(str, "FONT_MATRIX_12"))
    {
        return FONT_MATRIX_12;
    }
    if(0 == strcmp(str, "FONT_MATRIX_14"))
    {
        return FONT_MATRIX_14;
    }
    if(0 == strcmp(str, "FONT_MATRIX_16"))
    {
        return FONT_MATRIX_16;
    }
    if(0 == strcmp(str, "FONT_MATRIX_20"))
    {
        return FONT_MATRIX_20;
    }
    if(0 == strcmp(str, "FONT_MATRIX_24"))
    {
        return FONT_MATRIX_24;
    }
    if(0 == strcmp(str, "FONT_MATRIX_28"))
    {
        return FONT_MATRIX_28;
    }
    if(0 == strcmp(str, "FONT_MATRIX_32"))
    {
        return FONT_MATRIX_32;
    }
    if(0 == strcmp(str, "FONT_MATRIX_40"))
    {
        return FONT_MATRIX_40;
    }
    if(0 == strcmp(str, "FONT_MATRIX_48"))
    {
        return FONT_MATRIX_48;
    }
    if(0 == strcmp(str, "FONT_MATRIX_56"))
    {
        return FONT_MATRIX_56;
    }
    if(0 == strcmp(str, "FONT_MATRIX_64"))
    {
        return FONT_MATRIX_64;
    }
    if(0 == strcmp(str, "FONT_VECTOR"))
    {
        return FONT_VECTOR;
    }

    return -1;
}

si_t get_cursor_enum_from_str(const char * str)
{
    if(NULL == str)
    {
        return -1;
    }

    if(0 == strcmp(str, "CURSOR_SHAPE_X"))
    {
        return CURSOR_SHAPE_X;
    }
    if(0 == strcmp(str, "CURSOR_SHAPE_ARROW"))
    {
        return CURSOR_SHAPE_ARROW;
    }
    if(0 == strcmp(str, "CURSOR_SHAPE_HAND"))
    {
        return CURSOR_SHAPE_HAND;
    }
    if(0 == strcmp(str, "CURSOR_SHAPE_TEXT_INPUT"))
    {
        return CURSOR_SHAPE_TEXT_INPUT;
    }
    if(0 == strcmp(str, "CURSOR_SHAPE_TEXT_SELECT"))
    {
        return CURSOR_SHAPE_TEXT_SELECT;
    }
    if(0 == strcmp(str, "CURSOR_SHAPE_H_RESIZE"))
    {
        return CURSOR_SHAPE_H_RESIZE;
    }
    if(0 == strcmp(str, "CURSOR_SHAPE_V_RESIZE"))
    {
        return CURSOR_SHAPE_V_RESIZE;
    }

    return -1;
}
