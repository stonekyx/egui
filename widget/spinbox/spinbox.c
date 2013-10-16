/**
 * @file spinbox.c
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

# include "spinbox.h"
# include "../button/button.h"
# include "../text_line/text_line.h"

# define TMP_ARRAY_SIZE 256

static si_t do_handle_event(void *subscribe_info);

/* spinbox样式全局对象 */
static struct spinbox_style spinbox_default_style =
{
    {
        /* 初始化，默认未访问 */
        0,  /* .flag */

        /* 默认工作区域 */
        0,  /* .area_x */
        0,  /* .area_y */
        100,  /* .area_width */
        30,  /* .area_height */

        /* 默认边界宽度 */
        1,  /* .border_size */

        /* 默认宽度&高度 */
        0,  /* .maximum_width */
        0,  /* .minimum_width */
        0,  /* .maximum_height */
        0,  /* .minimum_height */

        /* 默认鼠标形状 */
        CURSOR_SHAPE_X, /* .cursor */

        /* 默认背景色 */
        0,    /* .back_color_r */
        255,    /* .back_color_g */
        0,  /* .back_color_b */
        0,  /* .back_color_a */

        /* 默认前景色 */
        255,  /* .fore_color_r */
        0,  /* .fore_color_g */
        0,  /* .fore_color_b */
        0,  /* .fore_color_a */
    }
};

/**
 * @brief 用spinbox全局样式对象初始化spinbox对象
 *
 * @param style spinbox样式对象指针
 * @param b spinbox指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t spinbox_init_with_default_style(struct spinbox * b)
{
    char *config_path = get_config_path("spinbox.cfg");

    si_t res = widget_init_with_default_style(config_path,
            WIDGET_POINTER(b), &spinbox_default_style.common,
            NULL, 0);
    free(config_path);

    return res;
}

static si_t spinbox_default_widget_show(struct spinbox * b, union message * msg) 
{
    text_line_show(b->text_number);
    button_show(b->button_up);
    button_show(b->button_down);
    return 0;
}

static si_t spinbox_default_widget_repaint(struct spinbox * b, union message * msg)
{
    text_line_repaint(b->text_number);
    button_repaint(b->button_up);
    button_repaint(b->button_down);
    return 0;
}

static si_t spinbox_value_change(struct spinbox *s, si_t delta)
{
    if(!s) {
        return 0;
    }
    if(s->minval<=s->maxval &&
            (s->value+delta < s->minval || s->value+delta > s->maxval)) {
        return 0;
    }
    s->value += delta;
    if(delta) {
        list_for_each(&s->subscribe_info_list, do_handle_event);
    }
    if(atoi(text_line_get_buf(s->text_number)) != s->value) {
        sprintf(text_line_get_buf(s->text_number), "%ld", s->value);
        if(s->text_number->ruler_cur >
                s->text_number->buf + strlen(s->text_number->buf)) {
            s->text_number->ruler_cur =
                s->text_number->buf + strlen(s->text_number->buf);
        }
        text_line_repaint(s->text_number);
        text_line_show(s->text_number);
    }
    return 1;
}

static void spinbox_text_event_handler(struct widget *subscriber, struct widget *publisher, si_t event)
{
    struct spinbox *self = SPINBOX_POINTER(subscriber);
    struct text_line *text = TEXT_LINE_POINTER(publisher);
    switch(event) {
        si_t new_value;
        case TEXT_LINE_EVENT_CURRENT_CHANGE:
            new_value = atoi(text_line_get_buf(text));
            if(!spinbox_value_change(self, new_value-self->value)) {
                /* sync with text failed */
                if(self->minval <= self->maxval) {
                    if(new_value < self->minval) {
                        spinbox_value_change(self, self->minval-self->value);
                    } else {
                        spinbox_value_change(self, self->maxval-self->value);
                    }
                }
            }
            break;
        default:
            break;
    }
}

static si_t spinbox_button_callback(addr_t self, addr_t msg)
{
    struct button *b = self;
    union message *m = msg;
    si_t delta = 0;
    if(!strcmp(b->text, "v")) {
        delta = -1;
    } else if(!strcmp(b->text, "^")) {
        delta = 1;
    }
    switch(m->base.type) {
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
            spinbox_value_change(SPINBOX_POINTER(b->custom_data), delta);
            break;
        default:
            return button_default_callback(self, msg);
    }
    return 0;
}

si_t spinbox_default_callback(addr_t self, addr_t msg)
{
    struct spinbox * b = self;
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_WIDGET_REPAINT:
            spinbox_default_widget_repaint(b, m);
            spinbox_default_widget_show(b, m);
            break;

        case MESSAGE_TYPE_WIDGET_SHOW:
            spinbox_default_widget_show(b, m);
            break;

        default:
            break;
    }

    return 0;
}

void spinbox_repaint(struct spinbox* b)
{
	widget_repaint(WIDGET_POINTER(b));
}

void spinbox_show(struct spinbox* b)
{
	widget_show(WIDGET_POINTER(b));
}

struct spinbox* spinbox_init(si_t minval, si_t maxval, si_t initval)
{
    struct spinbox * addr;

    /* 分配存储空间 */
    addr = (struct spinbox *)malloc(sizeof(struct spinbox));

    if(addr == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to malloc");

        return NULL;
    }

    if(NULL == (addr = widget_init_common(WIDGET_POINTER(addr), 0))) {
        return NULL;
    }
    /* struct spinbox 的成员 */
    addr->name = "struct spinbox";

    /* 用全局样式对象初始化spinbox样式 */
    spinbox_init_with_default_style(addr);
	
    /* 默认的回调函数 */
    addr->callback = spinbox_default_callback;

    addr->maxval = maxval;
    addr->minval = minval;
    addr->value = initval;

    /* 25 should be enough for 64-bit integers. */
    addr->text_number = text_line_init(25, 0);
    text_line_set_bounds(addr->text_number,
            0, 0,
            addr->area.width-addr->area.height, addr->area.height);
    addr->button_up = button_init("^");
    addr->button_up->custom_data = addr;
    addr->button_up->callback = spinbox_button_callback;
    button_set_bounds(addr->button_up,
            addr->area.width-addr->area.height+1, 0,
            addr->area.height, addr->area.height/2);
    addr->button_down = button_init("v");
    addr->button_down->custom_data = addr;
    addr->button_down->callback = spinbox_button_callback;
    button_set_bounds(addr->button_down,
            addr->area.width-addr->area.height+1, addr->area.height/2+1,
            addr->area.height, addr->area.height/2);

    text_line_clear_keybd_type(addr->text_number);
    text_line_set_keybd_type(addr->text_number, TEXT_LINE_KEYBOARD_TYPE_NUMBER);
    sprintf(text_line_get_buf(addr->text_number),
            "%ld", addr->value);
    text_line_register_move_handler(addr->text_number, WIDGET_POINTER(addr),
            TEXT_LINE_EVENT_CURRENT_CHANGE, spinbox_text_event_handler);

    object_attach_child(OBJECT_POINTER(addr), OBJECT_POINTER(addr->text_number));
    object_attach_child(OBJECT_POINTER(addr), OBJECT_POINTER(addr->button_up));
    object_attach_child(OBJECT_POINTER(addr), OBJECT_POINTER(addr->button_down));

    list_init(&addr->subscribe_info_list);

    return addr;
}

/*
    一般用户并不需要调用这个
    用户应用程序退出之后，application 会释放这个控件所占的空间。
    而窗口管理程序会释放所有资源，包括申请的每个图形设备。
*/
si_t spinbox_exit(struct spinbox * b)
{
    list_exit(&b->subscribe_info_list);

    text_line_exit(b->text_number);
    button_exit(b->button_up);
    button_exit(b->button_down);

    return widget_exit(WIDGET_POINTER(b));
}

void spinbox_set_bounds(struct spinbox * b, si_t x, si_t y, si_t width , si_t height)
{
	widget_set_bounds(WIDGET_POINTER(b), x, y, width, height);
    text_line_set_bounds(b->text_number,
            0, 0,
            b->area.width-b->area.height, b->area.height);
    button_set_bounds(b->button_up,
            b->area.width-b->area.height+1, 0,
            b->area.height, b->area.height/2);
    button_set_bounds(b->button_down,
            b->area.width-b->area.height+1, b->area.height/2+1,
            b->area.height, b->area.height/2);
}

void spinbox_set_color(struct spinbox* b, struct color* fcolor, struct color* bcolor)
{
	widget_set_color(WIDGET_POINTER(b), fcolor, bcolor);
}

void spinbox_set_font(struct spinbox* b, si_t font)
{
    text_line_set_font(b->text_number, font);
}

/* ---------------------------------------------------------- */
/*                         events                             */
/* ---------------------------------------------------------- */
static si_t event_to_be_dispatched;
static si_t do_handle_event(void *subscribe_info)
{
    struct widget_subscribe_info *si = subscribe_info;
    if(si->event == SPINBOX_EVENT_ALL ||
            si->event == event_to_be_dispatched) {
        si->handler(si->subscriber, si->publisher, event_to_be_dispatched);
    }
    return 0;
}

void spinbox_register_move_handler(struct spinbox *pub, struct widget *sub, si_t event, widget_event_handler handler)
{
    struct widget_subscribe_info si;
    si.subscriber = sub;
    si.publisher = WIDGET_POINTER(pub);
    si.event = event;
    si.handler = handler;
    list_push_back(&pub->subscribe_info_list, &si, sizeof(si));
}
