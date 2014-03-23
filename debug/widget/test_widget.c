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
# include <stdlib.h>

# include "geometry.h"
# include "data_structures.h"
# include "widget.h"

/*
    测试 widget 类

    获得 widget 类的绝对工作区域和左上角的绝对坐标
*/
int main()
{
    struct object node, * temp;
    struct widget obj[4];
    struct rectangle area;
    si_t i;

    for(i = 0; i < 4; ++ i)
    {
        obj[i].parent = NULL;
        obj[i].lchild = NULL;
        obj[i].rchild = NULL;
        obj[i].id = i;
        obj[i].area.x = 1;
        obj[i].area.y = 1;
        obj[i].area.width = 5;
        obj[i].area.height = 5;

        printf("%ld - %lX\n", i, (ui_t)(obj + i));
    }

    object_attach_child(OBJECT_POINTER(obj + 0), OBJECT_POINTER(obj + 1));
    object_attach_child(OBJECT_POINTER(obj + 1), OBJECT_POINTER(obj + 2));
    object_attach_child(OBJECT_POINTER(obj + 2), OBJECT_POINTER(obj + 3));

    obj[0].parent = &node;
    node.parent = OBJECT_POINTER(obj + 0);
    node.lchild = object_tree_l_most_node(OBJECT_POINTER(obj + 0));
    node.rchild = object_tree_r_most_node(OBJECT_POINTER(obj + 0));

    printf("show tree in l-n-r order\n");
    temp = node.lchild;
    while(temp != NULL)
    {
        printf("%ld - %lX\n", temp->id, (ui_t)temp);

        temp = object_tree_iterator_increment(&node, temp);
    }

    printf("show tree in r-n-l order\n");
    temp = node.rchild;
    while(temp != NULL)
    {
        printf("%ld - %lX\n", temp->id, (ui_t)temp);

        temp = object_tree_iterator_decrement(&node, temp);
    }

    application_widget_absolute_area(obj + 3, &area);
    printf("application_widget_absolute_area\n");
    printf("%ld %ld %ld %ld\n", area.x, area.y, area.width, area.height);

    widget_absolute_coordinate(obj + 3, &(area.x), &(area.y));
    printf("widget_absolute_coordinate\n");
    printf("%ld %ld\n", area.x, area.y);

    return 0;
}
