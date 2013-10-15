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

# include <stdio.h>
# include <string.h>
# include <unistd.h>

# include "application.h"

void foo(addr_t data)
{
    puts((char*)data);
}

/*
    测试一个用户应用程序拥有两个顶层窗口
*/
int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;
    struct window * w1;
    struct window * w2;
    struct checkbox *c;

    /* 初始化用户应用程序 */
    application_init(video_access_mode, app_type, "multi_windows");

    w1 = window_init("main window");

    /* 申请失败 */
    if(w1 == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w1, 300, 100, 448, 200);

    w2 = window_init("vice window");
    /* 申请失败 */
    if(w2 == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w2, 400, 200, 448, 200);

    /* 添加顶层窗口 */
    application_add_window(NULL, w1);
    /* 添加顶层窗口 */
    application_add_window(NULL, w2);
    /* 设置主窗口 */
    application_set_main_window(w1);

    c = checkbox_init(0);

    c->click_callback = foo;
    c->user_data = "hello world!";

    object_attach_child(OBJECT_POINTER(w2), OBJECT_POINTER(c));

    /* add radiobuttons */
    {
        struct radiobutton *r, *s;
        r = radiobutton_init("test_group", 0);
        s = radiobutton_init("test_group", 1);
        radiobutton_set_bounds(r, 20, 20, 13, 13);
        radiobutton_set_bounds(s, 20, 40, 13, 13);
        object_attach_child(OBJECT_POINTER(w1), OBJECT_POINTER(r));
        object_attach_child(OBJECT_POINTER(w1), OBJECT_POINTER(s));
        r = radiobutton_init("test_group_1", 0);
        s = radiobutton_init("test_group_1", 0);
        radiobutton_set_bounds(r, 40, 20, 13, 13);
        radiobutton_set_bounds(s, 40, 40, -1, -1);
        object_attach_child(OBJECT_POINTER(w1), OBJECT_POINTER(r));
        object_attach_child(OBJECT_POINTER(w2), OBJECT_POINTER(s));
    }
    
    {
        struct spinbox *s;
        s = spinbox_init(0, 5, 0);
        spinbox_set_bounds(s, 60, 60, -1, -1);
        object_attach_child(OBJECT_POINTER(w2), OBJECT_POINTER(s));
    }

    {
        struct flowbox *f;
        struct button *b[10];
        int i;
        f = flowbox_init(0);
        flowbox_set_bounds(f, 5, 70, 405, 100);
        for(i=0; i<10; i++) {
            b[i] = button_init("test button");
            button_set_bounds(b[i], 0, 0, i==5?106:100, i==4?40:(i==6?45:30));
            object_attach_child(OBJECT_POINTER(f), OBJECT_POINTER(b[i]));
            flowbox_add_widget(f, WIDGET_POINTER(b[i]));
        }
        object_attach_child(OBJECT_POINTER(w1), OBJECT_POINTER(f));
    }

    /* 运行 */
    application_exec();

    return 0;
}
