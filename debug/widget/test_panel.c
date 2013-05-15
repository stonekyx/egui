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
 * it under the terms of the GNU General Publpnl Lpnlense as published by
 * the Free Software Foundation, either version 2 of the Lpnlense, or
 * (at your option) any later version.
 *
 * EDUGUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Publpnl Lpnlense for more details.
 *
 * You should have received a copy of the GNU General Publpnl Lpnlense
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

# include "geometry.h"
# include "comm.h"
# include "application.h"
# include "client_lib.h"
# include "widget.h"

/*
    测试 epanel
*/
int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;
    ewindow * fream;
    epanel  * navigation;
	epanel  * work_space;
	elabel  * person_info;
	elabel  * trade_history;
	elabel  * my_cart;
	ebutton  * feedback;
    /* 初始化用户应用程序 */
    application_init(video_access_mode, app_type, "epanel");

    /* 申请窗口 */
    fream = window_init(1);
    /* 申请失败 */
    if(fream == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(fream,300,100,500,350);
    fream->title = "window with epanel";
    fream->minimize_enable = 1;
    fream->maximize_enable = 1;
    

	navigation = panel_init(2);
    /* 申请失败 */
    if( navigation == NULL)
    {
        application_exit();
        return -1;
    }
	navigation->back_color.r = 155;
	navigation->back_color.g = 97;
	navigation->back_color.b = 0;
	navigation->back_color.a= 0;
    panel_set_bounds(navigation,50,50,300,210);
    object_attach_child(OBJECT_POINTER(fream), OBJECT_POINTER(navigation));
 
 
	
	
	person_info = label_init(3);
	if(person_info == NULL)
    {
        application_exit();

        return -1;
    }
    person_info->text = "Person Info";
	label_set_bounds(person_info,0,0,200,50);
	object_attach_child(OBJECT_POINTER(navigation), OBJECT_POINTER(person_info));


	
 	
	trade_history = label_init(4);
	if(trade_history == NULL)
    {
        application_exit();

        return -1;
    }
    trade_history->text = "Trade History";
	label_set_bounds(trade_history,0,52,200,50);
	object_attach_child(OBJECT_POINTER(navigation), OBJECT_POINTER(trade_history));


 
	 my_cart= label_init(5);
	if(my_cart== NULL)
    {
        application_exit();

        return -1;
    }
    my_cart->text = "My Cart";
	label_set_bounds(my_cart,0,104,200,50);
	object_attach_child(OBJECT_POINTER(navigation), OBJECT_POINTER(my_cart));




	feedback= button_init(6);
	if(feedback== NULL)
    {
        application_exit();

        return -1;
    }
    feedback->text = "Feedback";
	button_set_bounds(feedback,0,156,200,50);
	object_attach_child(OBJECT_POINTER(navigation), OBJECT_POINTER(feedback));


	/* 添加顶层窗口 */
    application_add_window(NULL, fream);
    /* 设置主窗口 */
    application_set_main_window(fream);

    /* 运行 */
    application_exec();

    return 0;
}
