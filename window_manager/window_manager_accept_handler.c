/**
 * @file window_manager_accept_handler
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

# include "comm.h"
# include "data_structures.h"
# include "base_type.h"

# include "window_manager.h"
# include "application_info.h"
# include "server_lib.h"
# include "log.h"
# include "compiler.h"

si_t window_manager_accept_handler(struct egui_uds* uds_ptr, addr_t arg)
{
    struct application_info app_info;
    struct application_info* app_info_ptr = NULL;
    static int flag = 0;
    NOT_USED(arg);

	application_info_init(&app_info);

	if(0 != accept_as_uds(uds_ptr, &app_info.uds))
	{
		EGUI_PRINT_ERROR("failed to accept client uds");
		return SELECTER_RETURN_TYPE_END;
	}
	if(0 == flag)
    {
        screen_clear_black(&global_screen);
        screen_flush(0,0,global_screen.width,global_screen.height);
        flag = 1;
    }
	/* 将用户应用程序加入到向量 */
	vector_push_back(&global_wm.application_info_vector, &app_info, sizeof(struct application_info));
	app_info_ptr = vector_back(&global_wm.application_info_vector);

	/* 添加新的客户端 */
	server_lib_add_new_client(&app_info_ptr->uds, app_info_ptr);

    return 0;
}
