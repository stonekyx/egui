/**
 * @file
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

# include <assert.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>

#include "application.h"

# include "my_widget.h"
# include "directory_item.h"
# include "directory_content.h"


/* 工作目录 */
char working_directory[PATH_MAX];
/* 目录项列表 */
struct vector file_list;

static struct color barely_blue = {0xcb, 0xf3, 0xfb, 0};
static struct color light_blue = {0x46, 0xa5, 0xe5, 0};
static struct color dark_blue = {0x18, 0x29, 0x45, 0};
static struct color light_green = {0xcc, 0xff, 0x99, 0};

struct window * w;
/* b1 表示返回上级目录，b2 表示向上，b3 表示向下 */
struct button * b1;
/* 显示路径 */
struct label * l;
/* 显示目录项的控件 */
struct my_widget * mw;
/* scroll bar for my widget */
struct scroll_bar* s;

static void mywidget_subscribe_scrollbar(struct widget* subscriber, struct widget* pulisher, int event)
{
    struct scroll_bar* s = SCROLL_BAR_POINTER(pulisher);
    int lines_per_page = s->area.height / s->line_height;

    switch(event)
    {
        /**
         * press up arrow
         **/
    case SCROLL_BAR_EVENT_LINE_UP:
        if(mw->start > 0)
        {
            -- (mw->start);

            my_widget_repaint(mw);
            my_widget_show(mw);
        }
        break;

        /**
         * press down arow
         **/
    case SCROLL_BAR_EVENT_LINE_DOWN:
        if(mw->start + mw->count < vector_size(&file_list))
        {
            ++ (mw->start);

            my_widget_repaint(mw);
            my_widget_show(mw);
        }
        break;

        /**
         * press spaces above elevator
         **/
    case SCROLL_BAR_EVENT_PAGE_UP:
        if((mw->start -= lines_per_page) < 0)
            mw->start = 0;
        my_widget_repaint(mw);
        my_widget_show(mw);

        break;

        /**
         * press spaces below elevator
         **/
    case SCROLL_BAR_EVENT_PAGE_DOWN:
        if((mw->start += lines_per_page) > vector_size(&file_list) - mw->count)
            mw->start = vector_size(&file_list) - mw->count;
        my_widget_repaint(mw);
        my_widget_show(mw);
        break;

        /**
         * move elevator
         **/
    case SCROLL_BAR_EVENT_CHANGE:
        mw->start = s->cur_offset / s->line_height;
        my_widget_repaint(mw);
        my_widget_show(mw);
        break;

    default:
        return;
        break;
    }
}

si_t
button1_callback
(void * btn,
 void * msg)
{
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:

                chdir("..");

                getcwd(working_directory, PATH_MAX);

                vector_clear(&file_list);

                directory_content(working_directory, &file_list);

                mw->start = 0;
                scroll_bar_set_view(s, get_font_height(mw->gd) * vector_size(&file_list), 0);
                scroll_bar_update_offset(s, 0);
                scroll_bar_repaint(s);
                scroll_bar_show(s);

                my_widget_repaint(mw);
                my_widget_show(mw);

                label_repaint(l);
                label_show(l);

            break;

        default:
            button_default_callback(btn, msg);
            break;
    }


    return 0;
}

#if 0 /* {{{ */
si_t
button2_callback
(void * btn,
 void * msg)
{
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
            if(mw->start > 0)
            {
                -- (mw->start);

                my_widget_repaint(mw);
                my_widget_show(mw);
            }

            break;

        default:
            button_default_callback(btn, msg);
            break;
    }


    return 0;
}

si_t
button3_callback
(void * btn,
 void * msg)
{
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
            if(mw->start + mw->count < vector_size(&file_list))
            {
                ++ (mw->start);

                my_widget_repaint(mw);
                my_widget_show(mw);
            }

            break;

        default:
            button_default_callback(btn, msg);
            break;
    }


    return 0;
}
#endif /* }}} */

/*
    测试 button
*/
int main(int argc, char* argv[])
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;
    int font = 0;

    /* 初始化用户应用程序 */
    application_init(video_access_mode, app_type, "file_browser");

    /* 初始化向量 */
    /* 释放向量在窗口关闭时执行 */
    vector_init(&file_list);

    /**
     * use the second parameter
     **/
    if(argc == 2)
    {
        strncpy(working_directory, argv[1], sizeof(working_directory));
    }
    /**
     * else use default working path /
     **/
    else
    {
        working_directory[0] = '/';
        working_directory[1] = 0;
    }

    /* 切换到根目录 */
    chdir(working_directory);

    /* 获得工作目录的目录项 */
    directory_content(working_directory, &file_list);

    /* 申请窗口 */
    w = window_init("file_browser");
    /* 申请失败 */
    if(w == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w, 300, 100, 500, 440);
	window_set_color(w, NULL, &light_green);

    /* 申请按钮 */
    b1 = button_init("parent");
    /* 申请失败 */
    if(b1 == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b1, 5, 6, 90, 26);
	button_set_font(b1, FONT_MATRIX_12);
	button_set_color(b1, &light_blue, &barely_blue);
    b1->callback = button1_callback;

    /* 申请标签 */
    l = label_init(working_directory);
    /* 申请失败 */
    if(l == NULL)
    {
        application_exit();
        return -1;
    }
	label_set_bounds(l, 100, 6, 400, 26);
	label_set_color(l, NULL, &light_green);
	label_set_font(l, FONT_MATRIX_12);
    l->callback = label_default_callback;

    /* 申请标签 */
    mw = my_widget_init(5);
    /* 申请失败 */
    if(mw == NULL)
    {
        application_exit();
        return -1;
    }
    mw->area.x = 5;
    mw->area.y = 38;
    mw->area.width = 465;
    mw->area.height = 394;
    mw->font = FONT_MATRIX_12;
    mw->start = 0;
    mw->count = 16;
    set_font(mw->gd, mw->font);
    font = get_font_height(mw->gd);

    /**
     * scroll_bar
     **/
    s = scroll_bar_init(1, font * vector_size(&file_list), font);
    if(s == NULL)
    {
        application_exit();
        return -1;
    }
	scroll_bar_set_bounds(s, 475, 43, 20, 384);

#if 0 /** commented by padme 2012-05-31 {{{ **/
    set_font(mw->gd, mw->font);
    font = get_font_height(mw->gd);
    scroll_bar_set_vertical(s);
    scroll_bar_set_line_height(s, font);
    scroll_bar_set_view(s, font * vector_size(&file_list), 0);
    scroll_bar_update_offset(s, 0);
#endif /** commented by padme 2012-05-31 }}} **/
    scroll_bar_register_move_handler(s, WIDGET_POINTER(mw), SCROLL_BAR_EVENT_ALL, mywidget_subscribe_scrollbar);

    /* 将两个按钮添加到窗口 */
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b1));
#if 0 /* {{{ */
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b2));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b3));
#endif /* }}} */
    /* 将标签添加到按钮 */
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(l));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(mw));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(s));

#if 0
    /* 注册窗口 */
    register_window(NULL, w);
    w->is_activated = 1;

    /* 注册失败 */
    if(w->descriptor == 0)
    {
        application_exit();

        return -1;
    }
#endif

    /* 添加顶层窗口 */
    application_add_window(NULL, w);
    /* 设置主窗口 */
    application_set_main_window(w);

#if 0
    /* 绘制窗口、两个按钮、标签 */
    window_repaint(w);
    window_show(w);
    button_repaint(b1);
    button_show(b1);
    button_repaint(b2);
    button_show(b2);
    button_repaint(b3);
    button_show(b3);
    label_repaint(l);
    label_show(l);
    my_widget_repaint(mw);
    my_widget_show(mw);
#endif
    
    /* 运行 */
    application_exec();

    return 0;
}
