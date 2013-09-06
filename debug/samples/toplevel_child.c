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

/*
    测试顶层窗口和子窗口

    程序一共申请了四个窗口 w1 w2 w3 w4
    其中 w1 和 w2 是顶层窗口
    w3 和 w4 是 w2 的同一层次子窗口

    w2 总在 w3 和 w4 的后面
    w3 和 w4 的前后关系则可以改变
*/
int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;
    struct window * w1;
    struct window * w2;
    struct window * w3;
    struct window * w4;
    struct window * w5;
    struct radiobutton * r, *s;

    /* 初始化用户应用程序 */
    application_init(video_access_mode, app_type, "toplevel_child");

    w1 = window_init("uncle-1");
    /* 申请失败 */
    if(w1 == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w1, 300, 100, 448, 200);

    w2 = window_init("parent");
    /* 申请失败 */
    if(w2 == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w2, 350, 150, 448, 200);

    w3 = window_init("child-1");
    /* 申请失败 */
    if(w3 == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w3, 400, 200, 448, 200);

    w4 = window_init("child-2");
    /* 申请失败 */
    if(w4 == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w4, 450, 250, 448, 200);

    w5 = window_init("uncle-2");
    /* 申请失败 */
    if(w5 == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w5, 500, 300, 448, 200);

    /* 添加顶层窗口 */
    application_add_window(NULL, w1);
    /* 添加顶层窗口 */
    application_add_window(NULL, w2);
    /* 添加非顶层窗口 */
    application_add_window(w2, w3);
    /* 添加非顶层窗口 */
    application_add_window(w2, w4);
    /* 添加顶层窗口 */
    application_add_window(NULL, w5);
    /* 设置主窗口 */
    application_set_main_window(w1);

    r = radiobutton_init("test_group", 0);
    s = radiobutton_init("test_group", 1);
    radiobutton_set_bounds(r, 20, 20, 13, 13);
    radiobutton_set_bounds(s, 20, 40, 13, 13);
    object_attach_child(OBJECT_POINTER(w4), OBJECT_POINTER(r));
    object_attach_child(OBJECT_POINTER(w4), OBJECT_POINTER(s));

    /* 运行 */
    application_exec();

    return 0;
}
