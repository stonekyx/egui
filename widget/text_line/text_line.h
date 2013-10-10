/**
 * @file text_line.h
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

#ifndef _TEXT_LINE_H_
#define _TEXT_LINE_H_ 1

# include <base_type.h>
# include <comm.h>

# include "../widget/widget.h"

/**
 * widget structure
 **/
# define TEXT_LINE_DEFINITION \
    /**
     * inherited from widget
     **/ \
    WIDGET_DEFINITION \
    \
    /**
     * buffer for input text
     **/ \
    char* buf; \
    \
    /**
     * maximum length of imput text
     **/ \
    si_t buf_size; \
    \
    /**
     * keyboard type: number | letter | password
     **/ \
    si_t keybd_type; \
    \
    /**
     * pointer to current ruler
     **/ \
    char* ruler_cur; \
    \
    /**
     * pointer to the first character that show in text_area
     **/ \
    char* show_start; \
    \
    /**
     * pointer to start of ruler line
     **/ \
    char* ruler_line_start; \
    \
    /**
     * numbers of characters that could be hold in a line
     **/ \
    si_t chars_per_line; \
    \
    /**
     * inner frame color
     **/ \
    struct color frame_color; \
    \
    /**
     * font
     **/ \
    si_t font; \
    \
    /**
     * defalt msg in text_line
     **/ \
    char* placeholder; \
    \
    /**
     * wrap sentence if its too long
     **/ \
    si_t is_linewrap; \
    \
    /**
     * press enter to start a new line
     **/ \
    si_t is_enter_enable; \
    \
    /**
     * minimize number of line above or below
     **/ \
    si_t scroll_off; \
    /**
     * list of subscriber infomation
     **/ \
    struct list subscribe_info_list;


struct text_line
{
    TEXT_LINE_DEFINITION
};

/* text_line style struct */
struct text_line_style
{
    struct widget_style common;

    /* frame color */
    si_t frame_color_r;
    si_t frame_color_g;
    si_t frame_color_b;
    si_t frame_color_a;

    /* font */
    si_t font;
};

enum TEXT_LINE_KEYBOARD_TYPE
{
    TEXT_LINE_KEYBOARD_TYPE_NUMBER = 1,
    TEXT_LINE_KEYBOARD_TYPE_LETTER = 2,
    TEXT_LINE_KEYBOARD_TYPE_PASSWORD = 4
};

enum TEXT_LINE_EVENT
{
    /**
     * page start offset move up
     **/
    TEXT_LINE_EVENT_PAGE_UP,
    /**
     * page start offset move down
     **/
    TEXT_LINE_EVENT_PAGE_DOWN,
    /**
     * page start offset changed
     **/
    TEXT_LINE_EVENT_PAGE_CHANGE,
    /**
     * current ruler changed
     **/
    TEXT_LINE_EVENT_CURRENT_CHANGE,
    /**
     * all the events, only happen when register the handler
     **/
    TEXT_LINE_EVENT_ALL,

    TEXT_LINE_EVENT_MAX
};


/**
 * cast address to struct text_line pointer
 **/
# define TEXT_LINE_POINTER(addr) ((struct text_line*)(addr))

/**
 * @brief text_line的默认回调函数
 * 处理鼠标抬起事件，按下事件，键盘获得焦点和失去焦点事件，重绘事件，显示事件。
 *
 * @param self
 * @param msg
 *
 * @return 0
 **/
extern si_t text_line_default_callback(addr_t t, addr_t m);

/**
 * @brief 重绘text_line
 **/
extern void text_line_repaint(struct text_line* t);

/**
 * @brief 显示text_line
 **/
extern void text_line_show(struct text_line* t);

/**
 * @brief 使用前设置窗口部件结构体
 *
 * @return text_line指针
**/
extern struct text_line* text_line_init(si_t bufsize, si_t cur_line);

/**
 * @brief 使用后清理窗口部件结构体
 *
 * @param t 窗口部件结构体指针
 *
 * @return 0
**/
extern si_t text_line_exit(struct text_line* t);

/**
 * @brief 设置text_line的buffer的大小
 *
 * @param size buffer的大小
 *
 * @return 成功返回0 否则返回-1
 **/
extern si_t text_line_set_bufsize(struct text_line* t, si_t size);

/**
 * @brief 设置自动折行，即当一行的长度长过text_line的宽度时将多余的部分作为下一行
 **/
extern void text_line_set_wrap(struct text_line* t);
/**
 * @brief 设置不自动折行，即当一行的长度长过text_line的宽度时不显示多余的部分
 **/
extern void text_line_set_nowrap(struct text_line* t);

/**
 * @brief 设置接受输入
 **/
extern void text_line_set_enter_enable(struct text_line* t);
/**
 * @brief 设置不接受输入
 **/
extern void text_line_set_enter_disable(struct text_line* t);

/**
 * @brief 设置显示多行
 **/
extern void text_line_set_multilines(struct text_line* t);

/**
 * @brief 设置占位符
 **/
extern si_t text_line_set_placeholder(struct text_line* t, char* placeholder);

/**
 * @brief 获得text_line的buffer
 **/
extern char* text_line_get_buf(struct text_line* t);

/**
 * @brief 设置keyboard的类型
 *
 * @param type enum TEXT_LINE_KEYBOARD_TYPE
 **/
extern void text_line_set_keybd_type(struct text_line* t, si_t type);

/**
 * @brief 取消keyboard的类型
 *
 * @param type enum TEXT_LINE_KEYBOARD_TYPE
 **/
extern void text_line_cancel_keybd_type(struct text_line* t, si_t type);

/**
 * @brief 清空keyboard的类型
 **/
extern void text_line_clear_keybd_type(struct text_line* t);

/**
 * @brief text_line是否接受数字输入
 **/
extern si_t text_line_is_keybd_type_number(struct text_line* t);

/**
 * @brief text_line是否接受字母的输入
 **/
extern si_t text_line_is_keybd_type_letter(struct text_line* t);

/**
 * @brief text_line是否接受密码输入
 **/
extern si_t text_line_is_keybd_type_password(struct text_line* t);

/**
 * @brief 注册对
 **/
extern void text_line_register_move_handler(struct text_line* s, struct widget* w, si_t event, widget_event_handler handler);

/**
 * @brief 更新光标的位置
 **/
extern si_t text_line_update_ruler(struct text_line* t, char* new_ruler);

/**
 * @brief 更新光标的位置
 **/
extern si_t text_line_update_ruler_line(struct text_line* t, si_t line);

/**
 * @brief 更新当前所在行
 **/
extern si_t text_line_get_line_cur(struct text_line* t);

/**
 * @brief 获得总行数
 **/
extern si_t text_line_get_line_total(struct text_line* t);

/**
 * @brief 获得text_line显示最大的行数
 **/
extern si_t text_line_get_max_line_shown(struct text_line* t);

/**
 * @brief 设置text_line的边框大小
 * 需要保留原值的将对应参数设为负数
 *
 * @param t text_line控件指针
 * @param x 相对父控件的横坐标
 * @param y 相对父控件的纵坐标
 * @param width 宽度
 * @param height 长度
 **/
extern void text_line_set_bounds(struct text_line* text_line, si_t x, si_t y, si_t width , si_t height);

/**
 * @brief 设置text_line的颜色
 * 需要保留原值的将对应参数指针设置为空
 **/
extern void text_line_set_color(struct text_line* t, struct color* fore_color, struct color* back_color, struct color* frame_color);

/**
 * @brief 设置text_line字体
 **/
extern void text_line_set_font(struct text_line* t, si_t font);

#endif
