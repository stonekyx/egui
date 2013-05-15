/**
 * @file log.h
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

# ifndef __LOG_H__
# define __LOG_H__ 1

# include <stdio.h>
# include <errno.h>
# include <string.h>

#define EGUI_PRINT_SYS_ERROR(args...) \
	do{ \
		fprintf(stderr, "ERROR:[%s:%d]\t", __FILE__,__LINE__); \
		fprintf(stderr, ##args); \
		fprintf(stderr, ": %s\n", strerror(errno)); \
	}while(0)
#define EGUI_PRINT_ERROR(args...) \
	do{ \
		fprintf(stderr, "ERROR:[%s:%d]\t", __FILE__, __LINE__); \
		fprintf(stderr, ##args); \
		fprintf(stderr, "\n"); \
	} while(0)
#define EGUI_PRINT_INFO(args...) \
	do{ \
		fprintf(stderr, "INFO:[%s:%d]\t", __FILE__, __LINE__); \
		fprintf(stderr, ##args); \
		fprintf(stderr, "\n"); \
	} while(0)


# endif
