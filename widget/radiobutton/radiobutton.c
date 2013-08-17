/**
 * @file radiobutton.c
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
# include <graph.h>

# include "radiobutton.h"

# define TMP_ARRAY_SIZE 256

static si_t min(si_t x, si_t y)
{
    return x<y?x:y;
}

/* 按钮样式全局对象 */
static struct radiobutton_style radiobutton_default_style =
{
    /* 初始化，默认未访问 */
    0,  /* .flag */

    /* 默认工作区域 */
    0,  /* .area_x */
    0,  /* .area_y */
    13,  /* .area_width */
    13,  /* .area_height */

    /* 默认边界宽度 */
    1,  /* .border_size */

    /* 默认宽度&高度 */
    13,  /* .maximum_width */
    13,  /* .minimum_width */
    13,  /* .maximum_height */
    13,  /* .minimum_height */

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

/**
 * @brief 用radiobutton全局样式对象初始化radiobutton对象
 *
 * @param style radiobutton样式对象指针
 * @param b radiobutton指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t radiobutton_init_with_default_style(struct radiobutton * b)
{
    char tmp_str[TMP_ARRAY_SIZE] = {'\0'};
    si_t tmp_int = -1;
    /* radiobutton全局样式对象指针 */
    struct radiobutton_style * style = &radiobutton_default_style;

    /* 如果radiobutton全局样式对象尚未加载配置 */
    if(!style->flag)
    {
        struct config_parser parser;
        const char *RADIOBUTTON_STYLE_FILE = get_config_path("radiobutton.cfg");

        /* 初始化配置处理器 */
        if(config_parser_init(RADIOBUTTON_STYLE_FILE, &parser) != 0) {
            EGUI_PRINT_ERROR("fail to init radiobutton style from config file %s.", RADIOBUTTON_STYLE_FILE);
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
            if((tmp_int = get_cursor_enum_from_str(tmp_str)) >= 0) {
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
    }

    /* 用radiobutton默认样式初始化radiobutton各样式属性 */
    b->area.x = style->area_x;
    b->area.y = style->area_y;
    b->area.width = style->area_width;
    b->area.height = style->area_height;

    b->border_size = style->border_size;

    b->maximum_width = style->maximum_width;
    b->minimum_width = style->minimum_width;
    b->maximum_height = style->maximum_height;
    b->minimum_height = style->minimum_height;

    b->cursor = style->cursor;

    b->back_color.r = style->back_color_r;
    b->back_color.g = style->back_color_g;
    b->back_color.b = style->back_color_b;
    b->back_color.a = style->back_color_a;

    b->fore_color.r = style->fore_color_r;
    b->fore_color.g = style->fore_color_g;
    b->fore_color.b = style->fore_color_b;
    b->fore_color.a = style->fore_color_a;

    return 0;
}

static si_t radiobutton_default_widget_show(struct radiobutton * b, union message * msg) 
{
    struct rectangle area;

    widget_absolute_area(WIDGET_POINTER(b), &area);

    /* 设置区域 */
    set_area(b->gd, area.x, area.y, area.width, area.height);

    /* 更新 */
    update(b->gd);

    return 0;
}

static void _paint_radiobutton(struct radiobutton* c, si_t is_selected, struct rectangle* area, int x, int y)
{
    struct widget *p =
        WIDGET_POINTER(object_parent(OBJECT_POINTER(c)));
    /* 外围背景色 */
    set_color(p->gd, p->back_color.r, p->back_color.g, p->back_color.b, p->back_color.a);
    fill_rectangle(c->gd, area->x, area->y, area->width, area->height);
    /* 背景色 */
    set_color(c->gd, c->back_color.r, c->back_color.g, c->back_color.b, c->back_color.a);
    fill_circle(c->gd,
            x + c->area.width/2,
            y + c->area.height/2,
            min(c->area.width, c->area.height)/2);

    /* 前景色 */
    set_color(c->gd, c->fore_color.r, c->fore_color.g, c->fore_color.b, c->fore_color.a);
    draw_circle(c->gd,
            x + c->area.width/2,
            y + c->area.height/2,
            min(c->area.width, c->area.height)/2);

    if(is_selected) {
        fill_circle(c->gd,
                    x + c->area.width/2,
                    y + c->area.height/2,
                    min(c->area.width, c->area.height)/4);
    }
}

static si_t radiobutton_default_widget_repaint(struct radiobutton *c, union message *msg)
{
    struct rectangle area;
    si_t x, y;

    /* 获得左上角的绝对坐标 */
    widget_absolute_coordinate(WIDGET_POINTER(c), &x, &y);
    /* 获得绝对的工作区域 */
    /* 将会舍弃不在父控件内的部分*/
    widget_absolute_area(WIDGET_POINTER(c), &area);
    /* 设置区域 */
    set_area(c->gd, area.x, area.y, area.width, area.height);

    _paint_radiobutton(c, c->selected, &area, x, y);
    return 0;
}

si_t radiobutton_default_callback(addr_t self, addr_t msg)
{
    struct radiobutton * b = self;
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_WIDGET_REPAINT:
            radiobutton_default_widget_repaint(b, m);
            radiobutton_default_widget_show(b, m);
            break;
        case MESSAGE_TYPE_WIDGET_SHOW:
            radiobutton_default_widget_show(b, m);
            break;
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
            radiobutton_toggle(b);
            radiobutton_default_widget_repaint(b, m);
            radiobutton_default_widget_show(b, m);
            b->click_callback(b->user_data);
            break;
        default:
            break;
    }
    return 0;
}

void radiobutton_repaint(struct radiobutton* c)
{
    widget_repaint(WIDGET_POINTER(c));
}

void radiobutton_show(struct radiobutton* c)
{
    widget_show(WIDGET_POINTER(c));
}

struct radiobutton* radiobutton_init(int selected)
{
    struct radiobutton * addr;

    /* 分配存储空间 */
    addr = (struct radiobutton *)malloc(sizeof(struct radiobutton));
    if(addr == NULL) {
        EGUI_PRINT_SYS_ERROR("fail to malloc");
        return NULL;
    }

    /* 申请图形设备 */
    addr->gd = graphics_device_init(0, 0, 0, 0, 0, 0 ,0 ,0 ,0);
    if(addr->gd == 0) {
        free(addr);
        return NULL;
    }

    /* object 父类的成员 */
    addr->parent = NULL;
    addr->lchild = NULL;
    addr->rchild = NULL;
    addr->name = "struct radiobutton";
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

    /* 用全局样式对象初始化radiobutton样式 */
    radiobutton_init_with_default_style(addr);

    /* 默认的回调函数 */
    addr->callback = radiobutton_default_callback;

    addr->selected = selected;

    addr->click_callback = NULL;

    addr->user_data = NULL;

    return addr;
}

/*
   一般用户并不需要调用这个
   用户应用程序退出之后，application 会释放这个控件所占的空间。
   而窗口管理程序会释放所有资源，包括申请的每个图形设备。
   */
si_t radiobutton_exit(struct radiobutton * c)
{
    graphics_device_exit(c->gd);
    free(c);
    return 0;
}

void radiobutton_set_bounds(struct radiobutton *c, si_t x, si_t y, si_t width , si_t height)
{
    widget_set_bounds(WIDGET_POINTER(c), x, y, width, height);
}

void radiobutton_set_color(struct radiobutton *c, struct color *fcolor, struct color *bcolor)
{
    widget_set_color(WIDGET_POINTER(c), fcolor, bcolor);
}

void radiobutton_toggle(struct radiobutton* c)
{
    if(c->selected)
        c->selected = 0;
    else
        c->selected = 1;
}
