/**
 * @file label.c
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

# include <base_type.h>
# include <config_parser.h>
# include <log.h>
# include <comm.h>
# include <client_lib.h>

# include "label.h"

# define TMP_ARRAY_SIZE 256

/* label样式全局对象 */
static struct label_style label_default_style =
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

    /* 默认字体 */
    FONT_MATRIX_08, /* .font */
};

/**
 * @brief 用label全局样式对象初始化label对象
 *
 * @param style label样式对象指针
 * @param l label指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t label_init_with_default_style(struct label * l)
{
    char tmp_str[TMP_ARRAY_SIZE] = {'\0'};
    si_t tmp_int = -1;
    /* label全局样式对象指针 */
    struct label_style * style = &label_default_style;

    /* 如果label全局样式对象尚未加载配置 */
    if(!style->flag)
    {
        struct config_parser parser;
        const char *LABEL_STYLE_FILE = get_config_path("label.cfg");

        /* 初始化配置处理器 */
        if(config_parser_init(LABEL_STYLE_FILE, &parser) != 0)
        {
            EGUI_PRINT_ERROR("fail to init label style from config file %s.", LABEL_STYLE_FILE);

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

        tmp_int = -1;
        memset(tmp_str, 0, TMP_ARRAY_SIZE);
        config_parser_get_str(&parser, "font", tmp_str);
        if((tmp_int = get_font_enum_from_str(tmp_str)) >= 0)
        {
            style->font = tmp_int;
        }

        /* 退出配置处理器 */
        config_parser_exit(&parser);
        style->flag = 1;
    }

    /* 用label默认样式初始化label各样式属性 */
    l->area.x = style->area_x;
    l->area.y = style->area_y;
    l->area.width = style->area_width;
    l->area.height = style->area_height;

    l->border_size = style->border_size;

    l->maximum_width = style->maximum_width;
    l->minimum_width = style->minimum_width;
    l->maximum_height = style->maximum_height;
    l->minimum_height = style->minimum_height;

    l->cursor = style->cursor;

    l->back_color.r = style->back_color_r;
    l->back_color.g = style->back_color_g;
    l->back_color.b = style->back_color_b;
    l->back_color.a = style->back_color_a;

    l->fore_color.r = style->fore_color_r;
    l->fore_color.g = style->fore_color_g;
    l->fore_color.b = style->fore_color_b;
    l->fore_color.a = style->fore_color_a;

    l->font = style->font;

    return 0;
}

static si_t label_default_widget_show(struct label * l, union message * msg)
{
    struct rectangle label_absolute_area;

    widget_absolute_area(WIDGET_POINTER(l), &label_absolute_area);

    /* 设置区域 */
    set_area(l->gd, label_absolute_area.x, label_absolute_area.y, label_absolute_area.width, label_absolute_area.height);

    /* 更新 */
    update(l->gd);

    return 0;
}

static si_t label_default_widget_repaint(struct label * l, union message * msg)
{
    struct rectangle area;
    si_t x, y;

    /* 获得左上角的绝对坐标 */
    widget_absolute_coordinate(WIDGET_POINTER(l), &x, &y);
    /* 获得绝对的工作区域 */
    /* 将会舍弃不在父控件内的部分*/
    widget_absolute_area(WIDGET_POINTER(l), &area);
    /* 设置区域 */
    set_area(l->gd, area.x, area.y, area.width, area.height);

    set_color(l->gd, l->back_color.r, l->back_color.g, l->back_color.b, l->back_color.a);

    /* 填充矩形 */
    fill_rectangle(l->gd, area.x, area.y, area.width, area.height);

    set_color(l->gd, l->fore_color.r, l->fore_color.g, l->fore_color.b, l->fore_color.a);

    set_font(l->gd, l->font);

    show_text(l->gd, x + l->border_size, y + l->border_size, l->text, strlen(l->text));

    return 0;
}

si_t label_default_callback(addr_t self, addr_t msg)
{
    struct label * l = self;
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_WIDGET_REPAINT:
            label_default_widget_repaint(l, m);
            label_default_widget_show(l, m);
            break;

        case MESSAGE_TYPE_WIDGET_SHOW:
            label_default_widget_show(l, m);
            break;

        default:
            break;
    }

    return 0;
}

void label_repaint(struct label* l)
{
	widget_repaint(WIDGET_POINTER(l));
}

void label_show(struct label* l)
{
	widget_show(WIDGET_POINTER(l));
}

struct label* label_init(char* text)
{
    struct label * addr;

    /* 分配存储空间 */
    addr = (struct label *)malloc(sizeof(struct label));

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

    /* struct lable 的成员 */
    addr->parent = NULL;
    addr->lchild = NULL;
    addr->rchild = NULL;
    addr->name = "struct label";
    addr->id = 0;

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

    /* 用全局样式对象初始化label样式 */
    label_init_with_default_style(addr);

    /* 默认的回调函数 */
    addr->callback = label_default_callback;

    addr->text = text;

    return addr;
}

si_t label_exit(struct label * l)
{
    graphics_device_exit(l->gd);

    free(l);

    return 0;
}

void label_set_bounds(struct label * l, si_t x, si_t y, si_t width , si_t height)
{
	widget_set_bounds(WIDGET_POINTER(l), x, y, width, height);
}

void label_set_color(struct label* l, struct color* fcolor, struct color* bcolor)
{
	widget_set_color(WIDGET_POINTER(l), fcolor, bcolor);
}

void label_set_font(struct label* l, si_t font)
{
	l->font = font;
}
