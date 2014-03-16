/**
 * @file dialog.c
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

# include <base_type.h>
# include <config_parser.h>
# include <log.h>
# include <comm.h>
# include <client_lib.h>
# include "compiler.h"

# include "dialog.h"
# include "../window/paint_window_decoration.h"

# define TMP_ARRAY_SIZE 256

/* dialog样式全局对象 */
static struct dialog_style dialog_default_style = 
{
    {
        /* 初始化，默认未访问 */
        0,  /* .flag */

        /* 默认工作区域 */
        0,  /* .area_x */
        0,  /* .area_y */
        0,  /* .area_width */
        0,  /* .area_height */

        /* 默认边界宽度 */
        /* FIXME:不应hard code */
        3,  /* .border_size */

        /* 默认宽度&高度 */
        0,  /* .maximum_width */
        0,  /* .minimum_width */
        0,  /* .maximum_height */
        0,  /* .minimum_height */

        /* 默认鼠标形状 */
        CURSOR_SHAPE_X, /* .cursor */

        /* 默认背景色 */
        /* FIXME:不应该hard code */
        0,    /* .back_color_r */
        255,    /* .back_color_g */
        0,  /* .back_color_b */
        0,  /* .back_color_a */

        /* 默认前景色 */
        0,  /* .fore_color_r */
        0,  /* .fore_color_g */
        0,  /* .fore_color_b */
        0,  /* .fore_color_a */
    },

    /* 默认是否可以最大化/最小化/全屏 */
    0,  /* .maximize_enable */
    0,  /* .minimize_enable */
    0,  /* .is_full_screen */

    /* 边框/标题栏尺寸 */
    3,  /* .frame_size */
    30, /* .title_bar_size */
};

/**
 * @brief 用dialog全局样式对象初始化dialog对象
 *
 * @param style dialog样式对象指针
 * @param d dialog指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t dialog_init_with_default_style(struct dialog * d)
{
    static struct widget_style_entry extra[] = {
        {.key="maximize_enable",    .type=WIDGET_STYLE_TYPE_INT,
            .val=&(dialog_default_style.maximize_enable)},
        {.key="minimize_enable",    .type=WIDGET_STYLE_TYPE_INT,
            .val=&(dialog_default_style.minimize_enable)},
        {.key="is_full_screen",     .type=WIDGET_STYLE_TYPE_INT,
            .val=&(dialog_default_style.is_full_screen)},

        {.key="frame_size",         .type=WIDGET_STYLE_TYPE_INT,
            .val=&(dialog_default_style.frame_size)},
        {.key="title_bar_size",     .type=WIDGET_STYLE_TYPE_INT,
            .val=&(dialog_default_style.title_bar_size)},
    };
    char *config_path = get_config_path("dialog.cfg");

    si_t res = widget_init_with_default_style(config_path,
            WIDGET_POINTER(d), &dialog_default_style.common,
            extra, sizeof(extra)/sizeof(extra[0]));
    free(config_path);

    d->maximize_enable = dialog_default_style.maximize_enable;
    d->minimize_enable = dialog_default_style.minimize_enable;
    d->is_full_screen = dialog_default_style.is_full_screen;
    /*TODO: check the following code.
     * d->frame_size = dialog_default_style.frame_size;
     * d->title_bar_size = dialog_default_style.title_bar_size;
     */

    return res;
}

si_t
dialog_default_widget_show
(struct dialog * d,
 union message * msg)
{
    NOT_USED(msg);
    /* 设置区域 */
    set_area
    (d->gd,
     d->area.x - dialog_default_style.frame_size,
     d->area.y - dialog_default_style.frame_size - dialog_default_style.title_bar_size,
     d->area.width + dialog_default_style.frame_size + dialog_default_style.frame_size,
     d->area.height + dialog_default_style.frame_size + dialog_default_style.frame_size + dialog_default_style.title_bar_size);

    update(d->gd);

    return 0;
}

si_t
dialog_default_widget_repaint
(struct dialog * d,
 union message * msg)
{
    struct rectangle real_area;

    area_intersection(&(d->area), &(msg->widget_repaint.area), &real_area);

    /* 设置图形设备的工作区域 */
    set_area
    (d->gd,
     real_area.x,
     real_area.y,
     real_area.width,
     real_area.height);

    /* 使用背景色 */
    set_color
    (d->gd,
     d->back_color.r,
     d->back_color.g,
     d->back_color.b,
     d->back_color.a);

    fill_rectangle
    (d->gd,
     real_area.x,
     real_area.y,
     real_area.width,
     real_area.height);

    /* 绘制装饰性的部分 */
    paint_window_decoration(WINDOW_POINTER(d));

    return 0;
}

si_t
dialog_default_widget_move
(struct dialog * d,
 union message * msg)
{
    /* 更新位置 */
    d->area.x = msg->widget_move.position.x;
    d->area.y = msg->widget_move.position.y;

    move_window
    (d->descriptor,
     d->area.x,
     d->area.y);

    return 0;
}

si_t
dialog_default_widget_resize
(struct dialog * d,
 union message * msg)
{
    /* 更新位置 */
    d->area.x = msg->widget_resize.area.x;
    d->area.y = msg->widget_resize.area.y;
    d->area.width = msg->widget_resize.area.width;
    d->area.height = msg->widget_resize.area.height;

    resize_window
    (d->descriptor,
     d->area.x,
     d->area.y,
     d->area.width,
     d->area.height);

    return 0;
}
# if 0
si_t
dialog_default_window_close
(struct dialog * d,
 union message * msg)
{
    if(global_application.main_window == WINDOW_POINTER(d))
    {
        application_del_window(WINDOW_POINTER(d));
    }
    else
    {
        cancel_window(WINDOW_POINTER(d));
        application_del_window(WINDOW_POINTER(d));
    }

    return 0;
}
# endif
si_t
dialog_default_callback
(void * d,
 void * m)
{
    struct dialog * self = d;
    union message * msg = m;
    widget_default_callback(d,m);
    switch(msg->base.type)
    {
        case MESSAGE_TYPE_WIDGET_SHOW:
        /*
            printf("MESSAGE_TYPE_WIDGET_SHOW\n");
        */
            dialog_default_widget_show(self, msg);
            break;

        case MESSAGE_TYPE_WIDGET_REPAINT:
        /*
            printf("MESSAGE_TYPE_WIDGET_REPAINT\n");
        */
            dialog_default_widget_repaint(self, msg);
            dialog_default_widget_show(self, msg);
            break;

        case MESSAGE_TYPE_WIDGET_MOVE:
        /*
            printf("MESSAGE_TYPE_WIDGET_MOVE\n");
        */
            dialog_default_widget_move(self, msg);
            break;

        case MESSAGE_TYPE_WIDGET_RESIZE:
        /*
            printf("MESSAGE_TYPE_WIDGET_RESIZE\n");
        */
            dialog_default_widget_resize(self, msg);
            break;

        case MESSAGE_TYPE_WINDOW_ACTIVATE:
        /*
            printf("MESSAGE_TYPE_WINDOW_ACTIVATE\n");
        */
            break;

        case MESSAGE_TYPE_WINDOW_DEACTIVATE:
        /*
            printf("MESSAGE_TYPE_WINDOW_DEACTIVATE\n");
        */
            break;

        default:
            break;
    }

    return 0;
}

si_t
dialog_repaint
(struct dialog * w)
{
    union message msg;

    msg.widget_repaint.area.x = w->area.x;
    msg.widget_repaint.area.y = w->area.y;
    msg.widget_repaint.area.width = w->area.width;
    msg.widget_repaint.area.height = w->area.height;

    dialog_default_widget_repaint(w, &msg);

    return 0;
}

si_t
dialog_show
(struct dialog * w)
{
    union message msg;

    msg.widget_repaint.area.x = w->area.x - dialog_default_style.frame_size;
    msg.widget_repaint.area.y = w->area.y - dialog_default_style.frame_size - dialog_default_style.title_bar_size;
    msg.widget_repaint.area.width = w->area.width + dialog_default_style.frame_size + dialog_default_style.frame_size;
    msg.widget_repaint.area.height = w->area.height + dialog_default_style.frame_size + dialog_default_style.frame_size + dialog_default_style.title_bar_size;

    dialog_default_widget_show(w, &msg);

    return 0;
}

void * dialog_init(si_t id)
{
    struct dialog * addr;

    addr = (struct dialog *)malloc(sizeof(struct dialog));

    if(addr == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to malloc");
        return addr;
    }

    if(!(addr=widget_init_common(WIDGET_POINTER(addr), id))) {
        return NULL;
    }

    addr->name = "struct dialog";
    addr->is_window = 1;

    /* 默认是否是模态 */
    addr->modal = 0;
    /* 默认标题 */
    addr->title = NULL;

    /* 没注册前的窗口描述符 */
    addr->descriptor = 0;

    /* 用全局样式对象初始化dialog样式 */
    dialog_init_with_default_style(addr);

    /* 默认的回调函数 */
    addr->callback = dialog_default_callback;

    return addr;
}

si_t
dialog_exit
(struct dialog * d)
{
    return widget_exit(WIDGET_POINTER(d));
}
