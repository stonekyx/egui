/**
 * @file dialog.h
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

# ifndef _DIALOG_H_
# define _DIALOG_H_ 1

# include <base_type.h>
# include <comm.h>

# include "../window/window.h"

/*
A dialog window is a top-level window mostly used for short-term tasks and brief communications with the user.
QDialogs may be modal or modeless. QDialogs can provide a return value, and they can have default buttons.
QDialogs can also have a QSizeGrip in their lower-right corner, using setSizeGripEnabled().

Note that QDialog (an any other widget that has type Qt::Dialog) uses the parent widget slightly differently from other classes in Qt.
A dialog is always a top-level widget, but if it has a parent,
its default location is centered on top of the parent's top-level widget (if it is not top-level itself).
It will also share the parent's taskbar entry.

Use the overload of the QWidget::setParent() function to change the ownership of a QDialog widget.
This function allows you to explicitly set the window flags of the reparented widget;
using the overloaded function will clear the window flags specifying
the window-system properties for the widget (in particular it will reset the Qt::Dialog flag).
*/
/**
 * 对话框结构体
**/
# define DIALOG_DEFINITION \
    /** 从 window 继承 **/ \
    WINDOW_DEFINITION \
    \
    /** **/ \
    si_t unspec;

struct dialog
{
    DIALOG_DEFINITION
};

/* dialog样式结构体 */
struct dialog_style
{
    struct widget_style common;

    /* 是否可以最大化/最小化/全屏 */
    si_t maximize_enable;
    si_t minimize_enable;
    si_t is_full_screen;

    /* 边框尺寸 */
    si_t frame_size;
    /* 标题栏尺寸 */
    si_t title_bar_size;
};


/**
 * 将地址强制转换成 struct dialog *
**/
# define DIALOG_POINTER(addr) ((struct dialog *)(addr))

extern
si_t
dialog_paint
(struct dialog * d);

extern
si_t
dialog_show
(struct dialog * d);

extern
si_t
dialog_default_widget_repaint_function
(void * self,
 void * msg);

extern
si_t
dialog_default_callback
(void * self,
 void * msg);

extern
void *
dialog_init
(si_t id);

extern
si_t
dialog_exit
(struct dialog * d);

# endif
