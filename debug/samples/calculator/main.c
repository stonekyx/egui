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

# include "application.h"

# include "calculator.h"

/*
    测试 button
*/
int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;
    struct window * w;

    /* 初始化用户应用程序 */
    application_init(video_access_mode, app_type, "calculator");

    /* 申请窗口 */
    w = window_init("calc");
    /* 申请失败 */
    if(w == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w, 300, 100, 163, 244);
    w->back_color.r = 255;
    w->back_color.g = 0;
    w->back_color.b = 255;
    w->back_color.a = 0;

    my_init();

    /* 将按钮添加到窗口 */
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(l));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_mc));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_madd));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_msub));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_mr));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_add));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_sub));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_mul));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_div));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_7));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_8));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_9));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_c));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_4));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_5));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_6));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_nagetive_positive));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_1));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_2));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_3));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_equal));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_0));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b_dot));

    /* 添加顶层窗口 */
    application_add_window(NULL, w);
    /* 设置主窗口 */
    application_set_main_window(w);

    /* 运行 */
    application_exec();

    return 0;
}
