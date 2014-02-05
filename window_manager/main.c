/**
 * @file main.c
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

#include "log.h"
#include <signal.h>
#include <color.h>
#include "compiler.h"
#include "window_manager.h"

int main(int argc, char* argv[])
{
	/**
	 * TODO LOG
	 **/
	/**
	 * TODO 命令行参数
	 **/
	NOT_USED(argc);
	NOT_USED(argv);
#if 0 /** commented by padme 2012-05-12 {{{ **/
	if(SIG_ERR == signal(SIGCHLD,SIG_IGN))
	{
		EGUI_PRINT_ERROR("failed to set singal handler for SIGCHLD");
		return -1;
	}

	if(SIG_ERR == signal(SIGPIPE,SIG_IGN))
	{
		EGUI_PRINT_ERROR("failed to set singal handler for SIGPIPE");
		return -1;
	}
#endif /** commented by padme 2012-05-12 }}} **/


	if(0 != window_manager_init())
	{
		EGUI_PRINT_ERROR("failed to init window_manager");
		return -1;
	}

	if(0 != window_manager_run())
	{
		EGUI_PRINT_ERROR("failed to run window_manager");
	}

	window_manager_exit();

	return 0;
}

