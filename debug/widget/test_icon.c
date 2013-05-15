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

# include <style.h>
# include <enumerator.h>
# include <geometry.h>
# include <message.h>
# include <comm.h>
# include <application.h>
# include <wclib.h>
# include <wblib.h>

/*
    测试 eicon
*/
int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
    ewindow * w;
    eicon * ic;

    /* 初始化用户应用程序 */
    application_init(video_access_mode, "eicon");

    /* 申请窗口 */
    w = window_init(1);
    /* 申请失败 */
    if(w == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w,300,100,500,200);
    w->title = "window with eicon";
    w->minimize_enable = 1;
    w->maximize_enable = 1;
    w->callback = window_default_callback;

    /* 申请按钮 */
    ic = icon_init(2);
    /* 申请失败 */
    if(ic == NULL)
    {
        application_exit();
        return -1;
    }
    icon_set_bounds(ic ,50,50,100,120);
	icon_set_text(ic,"ehello");
	icon_set_img_path(ic,"/home/orange/Egui2.0/img/2.bmp");
	icon_set_is_text_visiable(ic ,1);
    ic->callback = icon_default_callback;

    /* 将按钮添加到窗口 */
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(ic));

	/* 添加顶层窗口 */
    application_add_window(NULL, w);
    /* 设置主窗口 */
    application_set_main_window(w);

    /* 运行 */
    application_exec();

    return 0;
}
