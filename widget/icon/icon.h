/**
 * @file icon.h
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

# ifndef _ICON_H_
# define _ICON_H_ 1

# include <base_type.h>
# include <comm.h>

# include "../widget/widget.h"

/**
 * 窗口部件结构体
 **/
# define ICON_DEFINITION \
    /**
     * 从 widget 继承
    **/ \
    WIDGET_DEFINITION


/*
 *@brief  icon 控件
 *@param img_path 图标中图片的存放路径
 *@param text     图标的名称，即文本
 *@param type     图标类型
 *
 * 包括普通文件图标、目录文件图标等等，详细请参看 /src/data/enumerator/icon_type. 
 * 这里 / 代表Egui工程根目录。
 *
 *@param is_text_visiable 图标是否显示文本内容。
 *@param text_feild 图标中的文本区域，即图标中 文本的画布区域。
 *@param img_feild  图标中的图片区域，即图标中 图片的画布区域。
 *
*/
struct icon
{
    ICON_DEFINITION
	char  img_path[255];
	char  text[255];
	/* see the enum icon_type */
	si_t type;
	si_t is_text_visiable;
	
	struct rectangle text_feild;
	struct rectangle img_feild;

};
typedef struct icon eicon;



/**
 * 将地址强制转换成 struct icon *
**/
# define ICON_POINTER(addr) ((struct icon *)(addr))


extern void  icon_set_img_path(eicon *ic, char * img_path);
extern char* icon_get_img_path(eicon *ic);

extern void  icon_set_text(eicon *ic, char * text);
extern char* icon_get_text(eicon *ic);



extern void  icon_set_type(eicon *ic, si_t type);
extern si_t  icon_get_type(eicon *ic);

extern void  icon_set_is_text_visiable(eicon *ic, si_t visiable_model);
extern si_t  icon_get_is_text_visiable(eicon *ic);



extern si_t icon_default_callback(void* self , void* msg );

/**
 * @brief 使用前,设置窗口部件结构体
 *
 * @details
 *
 * @param id 窗口部件的标识符
 *
 * @return 0
**/
extern void * icon_init(si_t id);


/**
 * @brief 使用后清理窗口部件结构体
 *
 * @details
 *
 * @param i 窗口部件结构体指针
 *
 * @return 0
**/
extern si_t icon_exit (eicon * ic);

/**
 * 绘制窗口部件
 *
 * @param i 窗口部件的地址
 *
 * @return 0
**/
extern void icon_set_bounds(eicon *ic, si_t x, si_t y, si_t width , si_t height);

extern si_t icon_repaint(struct icon * i);
extern si_t icon_default_widget_repaint(eicon* ic , union message * msg);


extern si_t icon_show_text(struct icon * i);

extern si_t icon_show(struct icon * i);
extern si_t icon_default_widget_show(eicon* ic , union message * msg);



extern si_t icon_default_mouse_press(eicon* ic , union message * msg);



extern si_t icon_default_mouse_release(eicon* ic , union message * msg);

# endif
