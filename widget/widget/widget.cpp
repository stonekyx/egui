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

#include <stdio.h>
#include <stdlib.h>

extern "C" {

#include "config_parser.h"
#include "log.h"
#include "client_lib.h"

}

#include "widget.h"
#include "widget_impl.h"

#define TMP_ARRAY_SIZE 256

const char *const WidgetImpl::config_name = "widget.cfg";
const char *WidgetImpl::config_path =::get_config_path(WidgetImpl::
                                                       config_name);

WidgetImpl::WidgetStyle WidgetImpl::default_style = {
    /* 初始化，默认未访问 */
    0,                      /* .flag */

    /* 默认工作区域 */
    0,                      /* .area_x */
    0,                      /* .area_y */
    0,                      /* .area_width */
    0,                      /* .area_height */

    /* 默认边界宽度 */
    0,                      /* .border_size */

    /* 默认宽度&高度 */
    0,                      /* .maximum_width */
    0,                      /* .minimum_width */
    0,                      /* .maximum_height */
    0,                      /* .minimum_height */

    /* 默认鼠标形状 */
    CURSOR_SHAPE_X,         /* .cursor */

    /* 默认背景色 */
    255,                    /* .back_color_r */
    255,                    /* .back_color_g */
    255,                    /* .back_color_b */
    0,                      /* .back_color_a */

    /* 默认前景色 */
    0,                      /* .fore_color_r */
    0,                      /* .fore_color_g */
    0,                      /* .fore_color_b */
    0,                      /* .fore_color_a */
};

WidgetImpl::WidgetImpl(Widget *p):owner(p)
{
}

WidgetImpl::WidgetStyle *WidgetImpl::get_default_style()
{
    return &default_style;
}

const char *WidgetImpl::get_config_path()
{
    return config_path;
}

struct config_parser *WidgetImpl::read_style_config()
{
    WidgetStyle *style = get_default_style();
    struct config_parser *parser = new config_parser();

    /* 初始化配置处理器 */
    if (config_parser_init(get_config_path(), parser) != 0) {
        EGUI_PRINT_ERROR
            ("fail to init widget style from config file %s.",
             get_config_path());
        return NULL;
    }
    /* 从配置读取各项配置,赋予style指针 */
    config_parser_get_int(parser, "area_x", &(style->area_x));
    config_parser_get_int(parser, "area_y", &(style->area_y));
    config_parser_get_int(parser, "area_width", &(style->area_width));
    config_parser_get_int(parser, "area_height", &(style->area_height));

    config_parser_get_int(parser, "border_size", &(style->border_size));

    config_parser_get_int(parser, "maximum_width", &(style->maximum_width));
    config_parser_get_int(parser, "minimum_width", &(style->minimum_width));
    config_parser_get_int(parser, "maximum_height", &(style->maximum_height));
    config_parser_get_int(parser, "minimum_height", &(style->minimum_height));

    static char tmp_str[TMP_ARRAY_SIZE];
    int tmp_int;
    config_parser_get_str(parser, "cursor", tmp_str);
    if ((tmp_int = get_cursor_enum_from_str(tmp_str)) >= 0) {
        style->cursor = tmp_int;
    }

    config_parser_get_int(parser, "back_color_r", &(style->back_color_r));
    config_parser_get_int(parser, "back_color_g", &(style->back_color_g));
    config_parser_get_int(parser, "back_color_b", &(style->back_color_b));
    config_parser_get_int(parser, "back_color_a", &(style->back_color_a));

    config_parser_get_int(parser, "fore_color_r", &(style->fore_color_r));
    config_parser_get_int(parser, "fore_color_g", &(style->fore_color_g));
    config_parser_get_int(parser, "fore_color_b", &(style->fore_color_b));
    config_parser_get_int(parser, "fore_color_a", &(style->fore_color_a));

    style->flag = 1;
    return parser;
}

/*******************************************************
 * @brief 用widget全局样式对象初始化widget对象
 *******************************************************/
void WidgetImpl::init_with_default_style()
{
    WidgetStyle *style = get_default_style();

    /* 用widget默认样式初始化widget各样式属性 */
    owner->area.x = style->area_x;
    owner->area.y = style->area_y;
    owner->area.width = style->area_width;
    owner->area.height = style->area_height;

    owner->border_size = style->border_size;

    owner->maximum_width = style->maximum_width;
    owner->minimum_width = style->minimum_width;
    owner->maximum_height = style->maximum_height;
    owner->minimum_height = style->minimum_height;

    owner->cursor = style->cursor;

    owner->back_color.r = style->back_color_r;
    owner->back_color.g = style->back_color_g;
    owner->back_color.b = style->back_color_b;
    owner->back_color.a = style->back_color_a;

    owner->fore_color.r = style->fore_color_r;
    owner->fore_color.g = style->fore_color_g;
    owner->fore_color.b = style->fore_color_b;
    owner->fore_color.a = style->fore_color_a;
}

Widget::Widget(si_t id):_pimpl(new WidgetImpl(this))
{
    /* 申请图形设备 */
    gd = graphics_device_init(0, 0, 0, 0, 0, 0, 0, 0, 0);

    /* 申请失败 */
    if (gd == 0) {
        EGUI_PRINT_ERROR("Failed to init graphics device.");
        exit(1);
    }

    /* struct object 的成员 */
    parent = NULL;
    lchild = NULL;
    rchild = NULL;
    name = NULL;
    this->id = id;

    /* 默认是否能处理键盘输入消息 */
    input_enable = 0;

    /* 默认是否可以刷新 */
    update_enable = 1;

    /* 默认是否可见 */
    visible = 1;

    /* 默认是否拥有键盘焦点 */
    keybd_focus = 0;

    /* 默认是否是窗口 */
    is_window = 0;

    /* 默认的回调函数 */
    callback = NULL;
}

Widget::~Widget()
{
    graphics_device_exit(gd);
}

void Widget::init_style()
{
    if(_pimpl->default_style.flag == 0) {
        struct config_parser *t = _pimpl->read_style_config();
        config_parser_exit(t);
        delete t;
    }
    _pimpl->init_with_default_style();
}

struct point Widget::get_absolute_coordinate()
{
    struct object *self, *parent;
    struct point res;

    self = OBJECT_POINTER(dynamic_cast<widget*>(this));
    res.x = area.x;
    res.y = area.y;
    if (is_window) {
        return res;
    }
    while ((parent = (struct object *)object_parent(self)) != NULL) {
        Widget *w_parent = static_cast<Widget*>(WIDGET_POINTER(parent));
        res.x += w_parent->area.x;
        res.y += w_parent->area.y;

        self = parent;
        if (w_parent->is_window) {
            break;
        }
    }
    return res;
}

struct rectangle Widget::get_absolute_area()
{
    struct object *self, *parent;
    struct rectangle parent_area;
    struct rectangle res;

    /* windows always use absolute coordinates. */
    if (is_window) {
        return area;
    }

    res = area;

    parent_area.x = 0;
    parent_area.y = 0;

    self = OBJECT_POINTER(dynamic_cast<widget*>(this));
    while ((parent = (struct object *)object_parent(self)) != NULL) {
        Widget *w_parent = static_cast<Widget*>(WIDGET_POINTER(parent));
        parent_area.width = w_parent->area.width;
        parent_area.height = w_parent->area.height;

        if (area_intersection(&parent_area, &res, &res) == -1) {
            break;
        }

        res.x += w_parent->area.x;
        res.y += w_parent->area.y;

        self = parent;
        if (w_parent->is_window) {
            break;
        }
    }
    return res;
}

void Widget::set_bounds(struct rectangle *b)
{
    /* 检验参数 参数若小于0则不改变对应的值 */
    area.x = (b->x >= 0 ? b->x : area.x);
    area.y = (b->y >= 0 ? b->y : area.y);
    area.width = (b->width >= 0 ? b->width : area.width);
    area.height = (b->height >= 0 ? b->height : area.height);
}

void Widget::set_color(struct color *fcolor, struct color *bcolor)
{
    /* 检验参数 指针若为空则不改变对应的值 */
    fore_color = (NULL == fcolor ? fore_color : *fcolor);
    back_color = (NULL == bcolor ? back_color : *bcolor);
}

si_t get_font_enum_from_str(const char *str)
{
    if (NULL == str) {
        return -1;
    }

    if (0 == strcmp(str, "FONT_MATRIX_08")) {
        return FONT_MATRIX_08;
    }
    if (0 == strcmp(str, "FONT_MATRIX_10")) {
        return FONT_MATRIX_10;
    }
    if (0 == strcmp(str, "FONT_MATRIX_12")) {
        return FONT_MATRIX_12;
    }
    if (0 == strcmp(str, "FONT_MATRIX_14")) {
        return FONT_MATRIX_14;
    }
    if (0 == strcmp(str, "FONT_MATRIX_16")) {
        return FONT_MATRIX_16;
    }
    if (0 == strcmp(str, "FONT_MATRIX_20")) {
        return FONT_MATRIX_20;
    }
    if (0 == strcmp(str, "FONT_MATRIX_24")) {
        return FONT_MATRIX_24;
    }
    if (0 == strcmp(str, "FONT_MATRIX_28")) {
        return FONT_MATRIX_28;
    }
    if (0 == strcmp(str, "FONT_MATRIX_32")) {
        return FONT_MATRIX_32;
    }
    if (0 == strcmp(str, "FONT_MATRIX_40")) {
        return FONT_MATRIX_40;
    }
    if (0 == strcmp(str, "FONT_MATRIX_48")) {
        return FONT_MATRIX_48;
    }
    if (0 == strcmp(str, "FONT_MATRIX_56")) {
        return FONT_MATRIX_56;
    }
    if (0 == strcmp(str, "FONT_MATRIX_64")) {
        return FONT_MATRIX_64;
    }
    if (0 == strcmp(str, "FONT_VECTOR")) {
        return FONT_VECTOR;
    }

    return -1;
}

si_t get_cursor_enum_from_str(const char *str)
{
    if (NULL == str) {
        return -1;
    }

    if (0 == strcmp(str, "CURSOR_SHAPE_X")) {
        return CURSOR_SHAPE_X;
    }
    if (0 == strcmp(str, "CURSOR_SHAPE_ARROW")) {
        return CURSOR_SHAPE_ARROW;
    }
    if (0 == strcmp(str, "CURSOR_SHAPE_HAND")) {
        return CURSOR_SHAPE_HAND;
    }
    if (0 == strcmp(str, "CURSOR_SHAPE_TEXT_INPUT")) {
        return CURSOR_SHAPE_TEXT_INPUT;
    }
    if (0 == strcmp(str, "CURSOR_SHAPE_TEXT_SELECT")) {
        return CURSOR_SHAPE_TEXT_SELECT;
    }
    if (0 == strcmp(str, "CURSOR_SHAPE_H_RESIZE")) {
        return CURSOR_SHAPE_H_RESIZE;
    }
    if (0 == strcmp(str, "CURSOR_SHAPE_V_RESIZE")) {
        return CURSOR_SHAPE_V_RESIZE;
    }

    return -1;
}
