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

# ifndef _DIRECTORY_ITEM_H_
# define _DIRECTORY_ITEM_H_ 1

# include <linux/limits.h>

# include "base_type.h"

struct directory_item
{
    /* 名字 */
    /* NAME_MAX 在 limits.h 中定义 */
    /* 前两个字节用于显示是目录还是其他文件 */
    /* D . 表示 . 是目录 */
    /* F file 表示 file 不是目录 */
    char name[NAME_MAX + 1 + 2];

    /* 是否是目录 */
    si_t is_directory;
};

# endif
