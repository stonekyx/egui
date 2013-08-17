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

    /* 运行 */
    application_exec();

    return 0;
}
