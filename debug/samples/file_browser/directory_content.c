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

/**
 * IMPORTANT! if this macro was not defined, then error will be shown "implicit declaration of lstat"
 * more see http://blog.csdn.net/stevenliyong/article/details/4160181
 **/
# define _GNU_SOURCE

# include <stdio.h>
# include <string.h>
# include <stdlib.h>

# include <dirent.h>
# include <unistd.h>
# include <sys/stat.h>

# include "directory_item.h"
# include "directory_content.h"
# include "log.h"


/**
 * qsort 的比较函数
 *
 * int (* compare)(const void *, const void *);
**/
static si_t
comp
(void * a,
 void * z)
{
    return
    strcmp
    (((struct directory_item *)(((struct vector_node *)a)->data))->name + 2,
     ((struct directory_item *)(((struct vector_node *)z)->data))->name + 2);
}

/**
 * 对向量进行快速排序
**/
static si_t
quick_sort
(struct vector * v)
{
    qsort(vector_begin(v), vector_size(v), sizeof(struct vector_node), (__compar_fn_t)comp);

    return 0;
}

si_t
directory_content
(char * path,
 struct vector * v)
{
    struct directory_item di;
    struct stat file_info;
    struct dirent * dirent_ptr;
    DIR * dir_ptr;

    /* 打开目录 */
    dir_ptr = opendir(path);

    if(dir_ptr == NULL)
    {
		EGUI_PRINT_SYS_ERROR("failed to open dir %s: opendir", path);
        return -1;
    }

    /* 一次读取目录项 */
    while((dirent_ptr = readdir(dir_ptr)) != 0)
    {
        /* 保存文件名 */
        strncpy(di.name + 2, dirent_ptr->d_name, strlen(dirent_ptr->d_name) + 1);

        /* 获得文件信息 */
        if(lstat(dirent_ptr->d_name, &file_info) < 0)
        {
			EGUI_PRINT_SYS_ERROR("failed to gain file %s stat: lstat", dirent_ptr->d_name);
            return -1;
        }

        /* 是目录 */
        if(S_ISDIR(file_info.st_mode))
        {
            di.name[0] = 'D';
            di.name[1] = ' ';
            if(access(dirent_ptr->d_name, X_OK) == 0)
                di.is_directory = 1;
            else
                di.is_directory = 0;
        }
        else if(S_ISREG(file_info.st_mode))
        {
            di.name[0] = 'F';
            di.name[1] = ' ';
            if(access(dirent_ptr->d_name, X_OK) == 0)
                di.is_directory = 2;
            else if(access(dirent_ptr->d_name, R_OK) == 0)
                di.is_directory = 3;
            else
                di.is_directory = 0;
        }
        else
        {
            di.name[0] = 'F';
            di.name[1] = ' ';
            di.is_directory = 0;
        }

        vector_push_back(v, &di, sizeof(struct directory_item));
    }

    /* 对目录项进行排序 */
    quick_sort(v);

    closedir(dir_ptr);

    return 0;
}
