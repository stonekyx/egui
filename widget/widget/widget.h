/**
 * @file wbl.widget.h
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

#ifndef _WBL_WIDGET_H_
#define _WBL_WIDGET_H_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include "base_type.h"
#include "comm.h"
#include "graph.h"
#include "geometry.h"

/**
 * 窗口部件结构体
**/
struct widget {
    /* 从 object 继承 */
    OBJECT_DEFINITION

    /* 图形设备描述符 */
    ui_t gd;

    /* 边框宽度 */
    si_t border_size;

    /* 最大宽度 */
    si_t maximum_width;

    /* 最小宽度 */
    si_t minimum_width;

    /* 最大高度 */
    si_t maximum_height;

    /* 最小高度 */
    si_t minimum_height;

    /* 光标的形状
       如果没有设置，将使使用父窗口部件的光标形状
       默认的是箭头光标 */
    si_t cursor;

    /* 如果真，可以处理输入消息；
       如果假，不可以处理输入消息。 */
    si_t input_enable;

    /* 如果真，可以处理绘制消息；
       如果假，不可以处理绘制消息。 */
    si_t update_enable;

    /* 是否可见 */
    si_t visible;

    /* 是否拥有键盘焦点 */
    si_t keybd_focus;

    /* 是否是窗口
       当部件是窗口类对象或者它的派生类对象是值为 1 */
    si_t is_window;

    /* 部件的工作区域 */
    struct rectangle area;

    /* 背景色 */
    struct color back_color;

    /* 前景色 */
    struct color fore_color;

    /* 处理消息的回调函数 */
      si_t(*callback) (void *, void *);
};

/* 将地址强制转换成 struct widget */
#define WIDGET_POINTER(addr) ((struct widget *)(addr))

/****************************************************
 * @brief 用widget全局样式对象初始化widget对象
 * @param path 配置文件路径
 * @param w widget指针
 * @param style widget样式对象指针
 * @param extra 其他需要初始化的属性列表，可以为NULL
 * @param extralen extra列表的长度
 * @return 成功返回0，否则返回-1
 ****************************************************/
extern si_t widget_init_with_default_style(const char *path,
                                           struct widget *w,
                                           struct widget_style *style,
                                           struct widget_style_entry
                                           extra[], ui_t extralen);

/**************************************************
 * @brief 从字符串得到字体枚举值
 * @param str 要匹配的字体类型字符串
 * @return 成功返回str对应字体的枚举值,否则返回-1
 **************************************************/
extern si_t get_font_enum_from_str(const char *str);

/******************************************************
 * @brief 从字符串得到鼠标形状枚举值
 * @param str 要匹配的鼠标形状类型字符串
 * @return 成功返回str对应鼠标形状的枚举值,否则返回-1
 ******************************************************/
extern si_t get_cursor_enum_from_str(const char *str);

#ifdef __cplusplus
}   /* end extern "C" */
#endif

#ifdef __cplusplus

struct WidgetImpl;
class Widget:public widget {
  protected:
    Widget(si_t id=0);
  public:
    ~Widget();
    void init_style();
    struct point get_absolute_coordinate();
    struct rectangle get_absolute_area();
    void set_bounds(struct rectangle *);
    void set_color(struct color *f, struct color *b);
    virtual void repaint()=0;
    virtual void show()=0;
  private:
    WidgetImpl *_pimpl;
};

typedef void (*widget_event_handler) (struct widget * subscriber,
                                      struct widget * publisher, si_t event);

/* subscriber is the widget that observe specific event occurs on specific
   widget(publisher) */
class widget_subscribe_info {
  public:
    /* the widget that subscribe events */
    struct widget *subscriber;
    /* the widget that publish event */
    struct widget *publisher;
    /* the event to be observed */
    si_t event;
    /* the function that would be called when event happen the three
       parameters are: the subscriber, the publisher, the event */
    widget_event_handler handler;
};

#endif

#endif
