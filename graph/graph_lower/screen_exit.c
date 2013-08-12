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

# include <stdlib.h>
# include <string.h>
# include <sys/mman.h>

# include <data_structures.h>
# include <log.h>

# include "../graph_lower.h"

si_t
screen_exit()
{
    /* make screen black */
    /*
    memset(global_screen.memory_addr, 0, global_screen.size);
    */

    /* 释放颜色列表 */
    free(global_cmap.red);
    free(global_cmap.green);
    free(global_cmap.blue);
    free(global_cmap.transp);

    /* 释放视频缓冲区 */
    free(global_screen.buffer_addr);

    /* 释放视频存储区 */
    if(-1 == munmap(global_screen.memory_addr, global_screen.size)) {
        EGUI_PRINT_SYS_ERROR("failed to unmap framebuffer: munmap()");
        return -1;
    }

    return 0;
}
