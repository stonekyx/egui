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
# include "client_lib.h"
# include "application.h"
# include "widget.h"

/*
    测试 label
*/
int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;
    struct window * w = NULL;
    struct label * l = NULL;

    /* 初始化用户应用程序 */
    application_init(video_access_mode, app_type, "label");

    /* 申请窗口 */
    w = window_init("window with label");
    /* 申请失败 */
    if(w == NULL)
    {
        application_exit();

        return -1;
    }
	window_set_bounds(w, 300, 100, 448, 200);

    /* 申请按钮 */
    l = label_init("queue is dead");
    /* 申请失败 */
    if(l == NULL)
    {
        application_exit();

        return -1;
    }
	label_set_bounds(l, 50, 50, 100, 50);

    /* 将按钮添加到窗口 */
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(l));

    /* 添加顶层窗口 */
    application_add_window(NULL, w);
    /* 设置主窗口 */
    application_set_main_window(w);

    /* 运行 */
    application_exec();

    return 0;
}
