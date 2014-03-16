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
# include <string.h>
# include "config_parser.h"
# include "log.h"
# include "client_lib.h"
# include "application.h"
# include "widget.h"

# define TMP_ARRAY_SIZE 256

si_t widget_default_callback(void* self, void* msg)
{
    struct widget * w = self;
    union message * m = msg;
    int i,flag=0;
    for(i=0;i<w->msg_num;i++)
    {
        if(w->msg_fct[i].msg_type->base.type == m->base.type)
        {
            flag=1;
            w->msg_fct[i].function(w, m);
        }
    }
    if(flag==1)
        return 0;
    else
        return -1;
}

struct widget_style widget_default_style = 
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

static si_t widget_init_extra_entry(struct config_parser *parser, struct widget_style_entry *entry)
{
    si_t res = 0;
    switch(entry->type) {
        case WIDGET_STYLE_TYPE_INT:
            if(-1 == config_parser_get_int(parser,
                        entry->key, (si_t*)entry->val)) {
                res = -1;
            }
            break;
        case WIDGET_STYLE_TYPE_STR:
            if(-1 == config_parser_get_str(parser,
                        entry->key, (char*)entry->val)) {
                res = -1;
            }
            break;
        case WIDGET_STYLE_TYPE_FLOAT:
            if(-1 == config_parser_get_float(parser,
                        entry->key, (float*)entry->val)) {
                res = -1;
            }
            break;
        default:
            break;
    }
    return res;
}

static si_t widget_init_extra_with_default_style(struct config_parser *parser, struct widget_style_entry extra[], ui_t extralen)
{
    struct widget_style_entry *entry;
    si_t res = 0;
    for(entry = extra; entry<extra+extralen; entry++) {
        if(-1 == widget_init_extra_entry(parser, entry)) {
            res = -1;
        }
    }
    return res;
}

/**
 * @brief 用widget全局样式对象初始化widget对象
 *
 * @param path 配置文件路径
 * @param w widget指针
 * @param style widget样式对象指针
 * @param extra 其他需要初始化的属性列表，可以为NULL
 * @param extralen extra列表的长度
 *
 * @return 成功返回0，否则返回-1
 **/
si_t widget_init_with_default_style(const char *path, struct widget * w, struct widget_style *style, struct widget_style_entry extra[], ui_t extralen)
{
    char tmp_str[TMP_ARRAY_SIZE] = {'\0'};
    si_t tmp_int = -1;
    si_t res = 0;

    /* 如果widget全局样式对象尚未加载配置 */
    if(!style->flag)
    {
        struct config_parser parser;

        /* 初始化配置处理器 */
        if(!path || config_parser_init(path, &parser) != 0)
        {
            EGUI_PRINT_ERROR("fail to init widget style from config file %s.", path);
            res = -1;
        } else {

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

            if(-1 == widget_init_extra_with_default_style(&parser, extra, extralen)) {
                res = -1;
            }

            /* 退出配置处理器 */
            config_parser_exit(&parser);
        }
        style->flag = 1;
    }

    /* 用widget默认样式初始化widget各样式属性 */
    w->area.x = style->area_x;
    w->area.y = style->area_y;
    w->area.width = style->area_width;
    w->area.height = style->area_height;

    w->border_size = style->border_size;

    w->maximum_width = style->maximum_width;
    w->minimum_width = style->minimum_width;
    w->maximum_height = style->maximum_height;
    w->minimum_height = style->minimum_height;

    w->cursor = style->cursor;

    w->back_color.r = style->back_color_r;
    w->back_color.g = style->back_color_g;
    w->back_color.b = style->back_color_b;
    w->back_color.a = style->back_color_a;

    w->fore_color.r = style->fore_color_r;
    w->fore_color.g = style->fore_color_g;
    w->fore_color.b = style->fore_color_b;
    w->fore_color.a = style->fore_color_a;

    return res;
}

void *widget_init(si_t id)
{
    struct widget * addr;
    char *config_path;

    /* 分配存储空间 */
    addr = (struct widget *)malloc(sizeof(struct widget));
    if(addr == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to malloc");

        return NULL;
    }

    widget_init_with_default_style(
            config_path = get_config_path("widget.cfg"),
            addr, &widget_default_style, NULL, 0);
    free(config_path);

    return widget_init_common(addr, id);
}

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

    /* 默认的回调函数 */
    addr->callback = NULL;

    addr->msg_num=0;
    
    //addr->msg_num2=0;

    return addr;
}

si_t widget_exit(struct widget * w)
{
    graphics_device_exit(w->gd);

    free(w);

    return 0;
}

si_t widget_absolute_coordinate(struct widget * w, si_t * x, si_t * y)
{
    struct object * self, * parent;

    self = OBJECT_POINTER(w);
    *x = w->area.x;
    *y = w->area.y;
    if(w->is_window) {
        return 0;
    }
    while((parent = object_parent(self)) != NULL)
    {
        *x += WIDGET_POINTER(parent)->area.x;
        *y += WIDGET_POINTER(parent)->area.y;

        self = parent;
        if(WIDGET_POINTER(parent)->is_window) {
            break;
        }
    }
    return 0;
}

si_t widget_absolute_area (struct widget * w, struct rectangle * result)
{
    struct object * self, * parent;
    struct rectangle temp, parent_area;
    si_t flag = 0;

    *result = w->area;

    /* windows always use absolute coordinates. */
    if(w->is_window) {
        return 0;
    }

    parent_area.x = 0;
    parent_area.y = 0;

    temp.x = w->area.x;
    temp.y = w->area.y;
    temp.width = w->area.width;
    temp.height = w->area.height;

    self = OBJECT_POINTER(w);
    while((parent = object_parent(self)) != NULL)
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
        if(WIDGET_POINTER(parent)->is_window) {
            break;
        }
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

si_t widget_listen(void* w,void* m, si_t (* response)(void *,void *))
{
    if(w == NULL)
        return -1;
    struct widget * self = w;
   // union message * msg = m;
    /*sit_t num;
    num = widget_default_callback(self, msg);
    w->map[num] = response;*/
    self->msg_fct[self->msg_num].msg_type = m;
    self->msg_fct[self->msg_num].function = response;
    self->msg_num=self->msg_num+1;
    return 0;
}

/*extern si_t str_to_msg(char* str)
{
    si_t msg,i;
    msg=str[0];
    for(i=1; i<=strlen(str), i++)
    {
        msg = msg*128 + str[i];
    }
    return msg;
}*/

extern si_t widget_listen_custom(void* w, char* str, si_t (* response)(void *,void *))
{
    if(w == NULL)
        return -1;
    struct widget * self = w;
    union message * msg = str;
//    msg = str_to_msg(m);
    self->msg_fct[self->msg_num].msg_type = msg;
    self->msg_fct[self->msg_num].function = response;
    self->msg_num=self->msg_num+1;
    return 0;
}

struct widget* widget_trigger_single(struct widget* w, union message* m)
{
    int i;
    for(i=0;i<w->msg_num;i++)
    {
        if(w->msg_fct[i].msg_type->base.type == m->base.type)
        {
            w->msg_fct[i].function(w, m);
        }
    }
    return NULL;
}

si_t widget_trigger(char *str)
{
    union message *m = str;
    application_widgets_for_each_decreament_public(widget_trigger_single, m);
    return 0;
}

