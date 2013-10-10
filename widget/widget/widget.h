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

# ifndef _WBL_WIDGET_H_
# define _WBL_WIDGET_H_ 1

# include "base_type.h"
# include "comm.h"
# include "graph.h"

/**
 * 窗口部件结构体
**/
# define WIDGET_DEFINITION \
    \
	/**
     * 从 object 继承
    **/ \
    OBJECT_DEFINITION \
    \
    /**
     * 图形设备描述符
    **/ \
    ui_t gd; \
    \
    /**
     * 边框宽度
    **/ \
    si_t border_size; \
    \
    /**
     * 最大宽度
    **/ \
    si_t maximum_width; \
    \
    /**
     * 最小宽度
    **/ \
    si_t minimum_width; \
    \
    /**
     * 最大高度
    **/ \
    si_t maximum_height; \
    \
    /**
     * 最小高度
    **/ \
    si_t minimum_height; \
    \
    /**
     * 光标的形状
     * 如果没有设置，将使使用父窗口部件的光标形状
     * 默认的是箭头光标
    **/ \
    si_t cursor; \
    \
    /**
     * 如果真，可以处理输入消息；
     * 如果假，不可以处理输入消息。
    **/ \
    si_t input_enable; \
    \
    /**
     * 如果真，可以处理绘制消息；
     * 如果假，不可以处理绘制消息。
    **/ \
    si_t update_enable; \
    \
    /**
     * 是否可见
    **/ \
    si_t visible; \
    \
    /**
     * 是否拥有键盘焦点
    **/ \
    si_t keybd_focus; \
    \
    /**
     * 是否是窗口
     *
     * 当部件是窗口类对象或者它的派生类对象是值为 1
    **/ \
    si_t is_window; \
    \
    /**
     * 部件的工作区域
    **/ \
    struct rectangle area; \
    \
    /**
     * 背景色
    **/ \
    struct color back_color; \
    \
    /**
     * 前景色
    **/ \
    struct color fore_color; \
    \
    /**
     * 处理消息的回调函数
    **/ \
    si_t (* callback)(void *, void *);

struct widget
{
    WIDGET_DEFINITION
};

struct widget_style
{
    /* 一个标志 */
    si_t flag;

    /* 工作区域 */
    si_t area_x;
    si_t area_y;
    si_t area_width;
    si_t area_height;

    /* 边界宽度 */
    si_t border_size;

    /* 高度&宽度 */
    si_t maximum_width;
    si_t minimum_width;
    si_t maximum_height;
    si_t minimum_height;

    /* 鼠标形状 */
    si_t cursor;

    /* 背景色 */
    si_t back_color_r;
    si_t back_color_g;
    si_t back_color_b;
    si_t back_color_a;

    /* 前景色 */
    si_t fore_color_r;
    si_t fore_color_g;
    si_t fore_color_b;
    si_t fore_color_a;
};

enum widget_style_type
{
    WIDGET_STYLE_TYPE_INT,
    WIDGET_STYLE_TYPE_STR,
    WIDGET_STYLE_TYPE_FLOAT,
};

struct widget_style_entry
{
    const char *key;
    enum widget_style_type type;
    addr_t val;
};

typedef void (*widget_event_handler)(struct widget *subscriber, struct widget *publisher, si_t event);

/**
 * subscriber is the widget that observe specific event occurs on specific widget(publisher)
 **/
struct widget_subscribe_info
{
    /**
     * the widget that subscribe events
     **/
    struct widget* subscriber;
    /**
     * the widget that publish event
     **/
    struct widget* publisher;
    /**
     * the event to be observed
     **/
    si_t event;
    /**
     * the function that would be called when event happen
     * the three parameters are: the subscriber, the publisher, the event
     **/
    widget_event_handler handler;
};

/**
 * 将地址强制转换成 struct widget *
**/
# define WIDGET_POINTER(addr) ((struct widget *)(addr))

/**
 * @brief 用widget全局样式对象初始化widget对象
 *
 * @param path 配置文件路径
 * @param w widget指针
 * @param style widget样式对象指针
 * @param extra 其他需要初始化的属性列表，可以为NULL
 * @param extralen extra列表的长度
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t widget_init_with_default_style(const char *path, struct widget * w, struct widget_style *style, struct widget_style_entry extra[], ui_t extralen);

/**
 * @brief 使用前设置窗口部件结构体
 *
 * @details
 *
 * @return 0
**/
extern void *widget_init(si_t id);

/**
 * @brief 初始化widget结构体的数据
 *
 * @details 调用前需要保证已经分配足够的空间
 *
 * @return 成功返回widget指针，失败返回NULL
**/
extern void *widget_init_common(struct widget *w, si_t id);

/**
 * @brief 使用后清理窗口部件结构体
 *
 * @details
 *
 * @param w 窗口部件结构体指针
 *
 * @return 0
**/
extern si_t widget_exit(struct widget * w);

/**
 * @brief 获得窗口部件的绝对坐标
 *
 * @param w 窗口部件结构体指针
 * @param x 存放绝对横坐标
 * @param y 存放绝对纵坐标
 *
 * @return 成功返回 0，失败返回 -1。
**/
extern si_t widget_absolute_coordinate(struct widget * w, si_t * x, si_t * y);

/**
 * @brief 获得窗口部件的绝对坐标区域
 *
 * @param w 窗口部件结构体指针
 * @param result 存放绝对区域的地址
 *
 * @return 成功返回 0，失败返回 -1。
**/
extern si_t widget_absolute_area(struct widget * w, struct rectangle * result);

/**
 * @brief 设置控件区域
 * 参数若小于0则对应的位置不发生改变
 *
 * @param widget 组件
 * @param x  组件左上角 x 
 * @param y  组件左上角 y
 * @param width  组件宽度
 * @param height 组件高度
 *
 **/ 
extern void widget_set_bounds(struct widget * widget, si_t x, si_t y, si_t width , si_t height);

/**
 * @brief 设置基本颜色
 * 参数若为空则对应的位置不发生改变
 *
 * @param w 控件
 * @param fcolor 前景色
 * @param bcolor 背景色
 **/
extern void widget_set_color(struct widget* w, struct color* fcolor, struct color* bcolor);

/**
 * @brief 重绘控件
 **/
extern void widget_repaint(struct widget* w);

/**
 * @brief 显示控件
 **/
extern void widget_show(struct widget* w);

/**
 * @brief 从字符串得到字体枚举值
 *
 * @param str 要匹配的字体类型字符串
 *
 * @return 成功返回str对应字体的枚举值,否则返回-1
 **/
extern si_t get_font_enum_from_str(const char * str);

/**
 * @brief 从字符串得到鼠标形状枚举值
 *
 * @param str 要匹配的鼠标形状类型字符串
 *
 * @return 成功返回str对应鼠标形状的枚举值,否则返回-1
 **/
extern si_t get_cursor_enum_from_str(const char * str);

# endif
