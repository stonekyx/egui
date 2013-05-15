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

# include "geometry.h"
# include "comm.h"
# include "application.h"
# include "widget.h"

si_t
button_callback
(void * btn,
 void * msg)
{
    struct button * b = btn;
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
            printf("%lX single click\n", (si_t)b);

            break;

        default:
            button_default_callback(btn, msg);
            break;
    }


    return 0;
}

/*
    测试 button
*/
int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;
    struct window * w = NULL;
    struct button * b = NULL;

    /* 初始化用户应用程序 */
    application_init(video_access_mode, app_type, "button");

    /* 申请窗口 */
    w = window_init("window with button");
    /* 申请失败 */
    if(w == NULL)
    {
        application_exit();

        return -1;
    }
	window_set_bounds(w,300,100,448,200);

    /* 申请按钮 */
    b = button_init("hello");
    /* 申请失败 */
    if(b == NULL)
    {
        application_exit();

        return -1;
    }
	button_set_bounds(b,50,50,50,50);
    b->callback = button_callback;

    /* 将按钮添加到窗口 */
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b));

  
    /* 添加顶层窗口 */
    application_add_window(NULL, w);
    /* 设置主窗口 */
    application_set_main_window(w);

    /* 运行 */
    application_exec();

    return 0;
}
