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

# include "application.h"
# include "client_lib.h"

/*
    测试客户机关闭服务器

    1、客户机发送关闭服务器的请求
    2、服务器接受关闭服务器的请求
    3、服务器发送客户机退出的消息
    4、客户机接受客户机退出的消息
    5、服务器进行清理工作
    6、客户机进行清理工作
*/
int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_WINDOW_MANAGER_EXIT;

    /* 初始化用户应用程序 */
    application_init(video_access_mode, app_type, "shutdown");

    window_manager_quit();

    /* 运行 */
    application_exec();

    return 0;
}

