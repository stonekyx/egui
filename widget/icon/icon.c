/**
 * @file icon.c
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
# include <string.h>
# include <fcntl.h> /* open() */
# include <unistd.h> /* close() */

# include <base_type.h>
# include <config_parser.h>
# include <log.h>
# include <comm.h>
# include <client_lib.h>
# include <graph.h>

# include "icon.h"

void *icon_init(si_t id)
{
    eicon * addr;

    /* 分配存储空间 */
    addr = (eicon *)malloc(sizeof(eicon));

    if(addr == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to malloc");

        return NULL;
    }

    /* 申请图形设备 */
    addr->gd = graphics_device_init(0, 0, 0, 0, 0, 0 ,0 ,0 ,0);

    /* 申请失败 */
    if(addr->gd == 0)
    {
        /* 释放存储空间 */
        free(addr);

        return NULL;
    }

    /* struct icon 的成员 */
    addr->parent = NULL;
    addr->lchild = NULL;
    addr->rchild = NULL;
    addr->name = "struct icon";
    addr->id = id;

    /* 默认工作区域 */
    addr->area.x = 0;
    addr->area.y = 0;
    addr->area.width = 0;
    addr->area.height = 0;

    /* 默认边界宽度 */
    addr->border_size = 0;

    /* 默认最大宽度 */
    addr->maximum_width = 0; /* should use macro */
    /* 默认最大高度 */
    addr->maximum_height = 0; /* should use macro */
    /* 默认最小宽度 */
    addr->minimum_width = 0;
    /* 默认最小高度 */
    addr->minimum_height = 0;

    /* 默认鼠标形状 */
    addr->cursor = CURSOR_SHAPE_X;

    /* 默认是否能处理键盘输入消息 */
    addr->input_enable = 0;

    /* 默认是否可以刷新 */
    addr->update_enable = 1;

    /* 默认是否可见 */
    addr->visible = 1;

    /* 默认是否拥有键盘焦点*/
    addr->keybd_focus = 0;

    /* 默认是否是窗口 */
    addr->is_window = 0;

    /* FIXME: should not be hard code */
    addr->back_color.r = 255;
    addr->back_color.g = 255;
    addr->back_color.b = 255;
    addr->back_color.a = 0;

    addr->fore_color.r = 0;
    addr->fore_color.g = 0;
    addr->fore_color.b = 0;
    addr->fore_color.a = 0;

	addr->img_feild.x = 0;
	addr->img_feild.y = 0;
	addr->img_feild.width = 0;
	addr->img_feild.height = 0;

	addr->text_feild.x = 0;
	addr->text_feild.y = 0;
	addr->text_feild.width = 0;
	addr->text_feild.height = 0;
	
	addr->is_text_visiable = 0;
	memset(addr->img_path , 0 ,sizeof(char)*255);
	memset(addr->text , 0, sizeof(char)*255);
    /* 默认的回调函数 */
    addr->callback = icon_default_callback;

    return addr;
}

/*
 * @breif 本函数功能是通过判断
 *
 */
si_t   icon_update_all_areas ( eicon * ic )
{	 
  	struct rectangle area;		
	/* 总字数 ， 单字宽度，单字高度，每行字数 ,总行数,最后一行的字数 */
	si_t char_total_num ;
	si_t font_height;
	si_t font_width;
	si_t text_line_num;
	si_t maxlen;

    widget_absolute_area(WIDGET_POINTER(ic), &area);


	if(icon_get_is_text_visiable(ic) == 1)
	{
		/* 总字数 ， 单字宽度，单字高度 */
		char_total_num = strlen( ic->text );
		font_height = get_font_height( ic->gd ) ;
		font_width  = get_font_width( ic->gd ) ;
		maxlen = area.width * 2 / font_width;

		text_line_num = char_total_num / maxlen + 1; 
		if( font_height  > ic->area.height || font_width  >ic->area.width)
		{
            EGUI_PRINT_ERROR("font for desktop icon is too large");

        	return -1;
		}

		ic->text_feild.x = area.x ;
		ic->text_feild.y = area.y + area.height - ( text_line_num * font_height );
		ic->text_feild.width = area.width;
		ic->text_feild.height = text_line_num * font_height ;
	
		ic->img_feild.x = area.x + area.width/4 ; 
		ic->img_feild.y = area.y  ;
		ic->img_feild.width = area.width/2 ;
		ic->img_feild.height = area.height - ic->text_feild.height;
		
	}
	else
	{
		ic->img_feild.x = area.x ; 
		ic->img_feild.y = area.y ;
		ic->img_feild.width = area.width ;
		ic->img_feild.height = area.height ;
	}
    return 1;
}

/*
 * @breaf private,
 *
 * */

si_t file_exist(char * file_path)
{
	si_t fd;
	
	if( ( file_path != NULL ) && ( fd = open(file_path , O_RDONLY) )== -1 )
	{
		return 0;
	}
	else
	{
		close( fd );
		return 1;
	}
}



si_t icon_draw_img(eicon *ic)
{
	set_area(ic->gd, ic->img_feild.x , ic->img_feild.y, ic->img_feild.width  , ic->img_feild.height );

	fill_rectangle(ic->gd, ic->img_feild.x , ic->img_feild.y, ic->img_feild.width  , ic->img_feild.height );
	
	set_color(ic->gd, ic->back_color.r, ic->back_color.g, ic->back_color.b, ic->back_color.a);

	if(file_exist(ic->img_path))
	{	
		draw_img(ic->gd, ic->img_path, ALIGN_HORIZONTAL_TYPE_CENTER | ALIGN_VERTICAL_TYPE_CENTER );
	}

	return 1;
}

si_t icon_show_text(eicon * ic)
{
  	 
	/* 总字数 ， 单字宽度，单字高度，每行字数 ,总行数,最后一行的字数 */
	si_t char_total_num ;
	si_t font_width;      
	si_t font_height; 	  	
	si_t maxlen;
	si_t text_line_num;
	si_t last_line_text_num;

	si_t i = 0;
	

	char_total_num = strlen( ic->text );

	font_width  = get_font_width( ic->gd ) ;

	font_height = get_font_height( ic->gd ) ;

	maxlen = (ic->area.width * 2 )/ font_width;

	last_line_text_num = char_total_num % maxlen ;

	text_line_num = char_total_num / maxlen + 1;
  
	set_color(ic->gd, ic->fore_color.r, ic->fore_color.g, ic->fore_color.b, ic->fore_color.a);

	/* 设置区域 , 文字区域*/
 	set_area(ic->gd,  ic->text_feild.x , ic->text_feild.y , ic->text_feild.width , ic->text_feild.height);

/*	 填充矩形 
	fill_rectangle(ic->gd,  ic->text_feild.x, ic->text_feild.y, ic->text_feild.width , ic->text_feild.height);
*  */
	/* 显示文字 */
	for( i = 0; i < text_line_num -1; i++ )
	{
		show_text(ic->gd, ic->text_feild.x, ic->text_feild.y + i * font_height  , ic->text + i*maxlen ,maxlen);
	}

	show_text(  ic->gd, 
			    ic->text_feild.x + ( ic->text_feild.width  - last_line_text_num * font_width ) / 2,
			    ic->text_feild.y + i * font_height  , 
			    ic->text + i * maxlen ,
			    last_line_text_num
			 );
	return 0;
	
}


si_t
icon_exit
(struct icon * i)
{
    graphics_device_exit(i->gd);

    free(i);

    return 0;
}
void  icon_set_img_path(eicon *ic, char * img_path)
{
	strcpy(ic->img_path,img_path);
	return ;
}
char* icon_get_img_path(eicon *ic)
{
	return ic->img_path;
}
void  icon_set_text(eicon *ic, char * text)
{
	strcpy(ic->text, text);
	return;
}
char* icon_get_text(eicon *ic)
{
	return ic->text;
}

void  icon_set_type(eicon *ic, si_t type)
{
	ic->type = type;
	return ;
}

si_t icon_get_type(eicon *ic)
{
	return ic->type;
}
void  icon_set_is_text_visiable(eicon *ic, si_t visiable_model)
{
	ic->is_text_visiable = visiable_model;
	return ;
}
si_t icon_get_is_text_visiable(eicon *ic)
{
	return ic->is_text_visiable;
}


si_t icon_default_callback(void* self , void* msg )
{
	
    eicon* ic = self;
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_WIDGET_REPAINT:
            icon_default_widget_repaint(ic, m);
            icon_default_widget_show(ic, m);
            break;

        case MESSAGE_TYPE_WIDGET_SHOW:
            icon_default_widget_show(ic, m);
            break;

        case MESSAGE_TYPE_MOUSE_PRESS:
            icon_default_mouse_press(ic, m);
            icon_default_widget_show(ic, m);
            break;

        case MESSAGE_TYPE_MOUSE_RELEASE:
            icon_default_mouse_release(ic, m);
            icon_default_widget_show(ic, m);
            break;

        default:
            break;
    }

    return 0;
}


/* 外部 API */
si_t icon_repaint(eicon * ic)
{
  	union message msg;

    msg.widget_repaint.area.x = ic->area.x;
    msg.widget_repaint.area.y = ic->area.y;
    msg.widget_repaint.area.width = ic->area.width;
    msg.widget_repaint.area.height = ic->area.height;

    icon_default_widget_repaint(ic, &msg);

    return 0;

}


si_t icon_default_widget_repaint(eicon* ic , union message * msg)
{  
	icon_update_all_areas( ic );

	icon_draw_img( ic );	

	if(icon_get_is_text_visiable(ic) == 1)
	{
		icon_show_text(ic);
	}

    return 0;

}

/* 外部API  */
si_t icon_show(eicon * ic)
{
  	union message msg;

    msg.widget_repaint.area.x = ic->area.x;
    msg.widget_repaint.area.y = ic->area.y;
    msg.widget_repaint.area.width = ic->area.width;
    msg.widget_repaint.area.height = ic->area.height;

    icon_default_widget_show(ic, &msg);

    return 0;

}
/*  */
si_t icon_default_widget_show(eicon* ic , union message * msg)
{
	struct rectangle area;
    
    widget_absolute_area(WIDGET_POINTER(ic), &area);
	set_area( ic->gd, area.x, area.y, area.width, area.height );
	update( ic->gd );

	return 0;
}

/* 
 * 图标被按下后，应该能够显示四周边框,Color:black，其他的特效，待完成。
 * 因此，需要将画笔的颜色设置为黑色
 *
 **/
si_t icon_default_mouse_press(eicon* ic , union message * msg)
{
	struct rectangle area;
     
    widget_absolute_area(WIDGET_POINTER(ic), &area);

    /* 设置区域 */
    set_area(ic->gd, area.x, area.y, area.width, area.height);
	  
	/* 上边框 */
    set_color(ic->gd, 0, 0, 0, 0);

    draw_line( ic->gd, area.x, area.y, area.x + area.width - 1, area.y );

	/* 左边框 */
    draw_line( ic->gd, area.x, area.y, area.x, area.y + area.height - 1);

     /* 右边框 */
    draw_line(ic->gd, area.x + area.width - 1, area.y, area.x + area.width - 1, area.y + area.height - 1);

    /* 下边框 */
    draw_line(ic->gd, area.x, area.y + area.height - 1, area.x + area.width - 1, area.y + area.height - 1);

    return 0;
	
}

 
/* 图标被按下后，应该能够显示四周边框,Color:black，其他的特效，待完成。
 * 因此，需要将画笔的颜色设置为黑色
 *
 **/
si_t icon_default_mouse_release(eicon* ic , union message * msg)
{
 	struct rectangle area;   

	widget_absolute_area(WIDGET_POINTER(ic), &area);

    /* 设置区域 */
    set_area(ic->gd, area.x, area.y, area.width, area.height);
	  
	/* 上边框 */
    /* FIXME: should not be hard code */
    set_color(ic->gd, 255, 255, 255, 0);
  	
	draw_line( ic->gd, area.x, area.y, area.x + area.width - 1, area.y );

	/* 左边框 */
    draw_line( ic->gd, area.x, area.y, area.x, area.y + area.height - 1);

     /* 右边框 */
    draw_line(ic->gd, area.x + area.width - 1,  area.y,  area.x + area.width - 1, area.y + area.height - 1);

    /* 下边框 */
    draw_line(ic->gd, area.x,  area.y + area.height - 1, area.x + area.width - 1, area.y + area.height - 1);

    return 0;

}

void icon_set_bounds(eicon* icon, si_t x, si_t y, si_t width , si_t height)
{
	icon->area.x = x;
	icon->area.y = y;
	icon->area.width = width;
	icon->area.height = height;
	return ;
}



