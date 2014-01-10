/**
 * @file button.h
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

# ifndef _BUTTON_H_
# define _BUTTON_H_ 1

#ifdef __cplusplus
extern "C" {
#endif

# include <base_type.h>
# include <comm.h>

# include "../widget/widget.h"

struct ButtonImpl;
class Button:protected Widget {
  public:
    Button(si_t id=0);
    ~Button();
  private:
    ButtonImpl *_pimpl;
};

/**
 * @brief button的默认回调函数
 * 处理鼠标按下事件，鼠标抬起事件，重绘事件，显示事件
 *
 * @param self
 * @param msg
 *
 * @return 0
 **/
extern si_t button_default_callback(addr_t self, addr_t msg);

/**
 * @brief button重绘
 **/
extern void button_repaint(struct button* b);

/**
 * @brief button显示
 **/
extern void button_show(struct button* b);

/**
 * @brief 使用前设置窗口部件结构体
 *
 * @details
 *
 * @param text 显示在button上的内容
 *
 * @return 0
**/
extern struct button* button_init(const char* text);

/**
 * @brief 使用后清理窗口部件结构体
 *
 * @details
 *
 * @param b 窗口部件结构体指针
 *
 * @return 0
**/
extern si_t button_exit(struct button * b);

/**
 * @brief 设置按钮的位置
 * 需要保留原值的将对应参数设为负数
 *
 * @param b 按钮控件指针
 * @param x 相对父控件的横坐标
 * @param y 相对父控件的纵坐标
 * @param width 宽度
 * @param height 长度
 **/
extern void button_set_bounds(struct button * widget, si_t x, si_t y, si_t width , si_t height);

/**
 * @brief 根据字体和文字长度自动设置按钮的边界
 *
 * @param b 按钮控件的指针
 **/
extern void button_set_bounds_auto(struct button *b);

/**
 * @brief 设置按钮的颜色
 * 需要保留原值的将对应参数指针设置为空
 *
 * @param b 控件
 * @param fcolor 前景色
 * @param bcolor 背景色
 **/
extern void button_set_color(struct button* b, struct color* fcolor, struct color* bcolor);

/**
 * @brief 设置按钮的字体
 *
 * @param b 控件
 * @param font 字体
 **/
extern void button_set_font(struct button* b, si_t font);

# endif
