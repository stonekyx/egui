/**
 * @file server_lib.h
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

# ifndef _SERVER_LIB_H_
# define _SERVER_LIB_H_ 1

#include "comm.h"


/**
 * 服务器的初始化函数
 * 服务器端的库对客户端请求的处理将综合成一个handler，作为上层窗口管理器的event_listener中所有客户端句柄的读事件处理函数
 * 即使用event_listener_add_read_event()将这个handler作为每个客户端的通信句柄的读事件处理函数
 *
 * @param upper_event_listener 上层应用的event_listener
 * @param handler 析构函数
 **/
extern void server_lib_init(struct event_listener* upper_event_listener, void(*handler)(addr_t));

/**
 * 添加新客户端 当上层需要添加一个客户端的时候
 * 也会同时使用event_listener_add_read_event()将默认的handler作为这个客户端的通信句柄的读事件处理函数
 *
 * @param uds_ptr 通信句柄
 * @param app_adr 应用程序的指针
 **/
extern void server_lib_add_new_client(struct egui_uds* uds_ptr, addr_t app_addr);

/***********************************************************************************
 * 对请求的处理
 * 请求分为两类：一类是客户端使用图形设备，在server_lib中自动调用对应graph的函数来执行，无须调用server_lib的窗口管理器处理
 * 一类是管理窗口/应用程序的命令，这些需要接受请求后传递给上层窗口管理器处理
 * 
 * server_lib将第二类请求以回调函数的形式将逻辑处理部分交给上层实现，通信部分由下层comm模块处理
 ***********************************************************************************/
/**
 * 添加对注册应用程序的处理函数
 * 
 * @param handler 待添加的处理函数，参数分别为：应用程序指针，视频访问模式，应用程序属性，应用程序名称
 **/
extern void add_register_application_handler(si_t(*handler)(addr_t, si_t, si_t, char*));

/**
 * 添加对取消应用程序的处理函数
 *
 * @param handler 待添加的处理函数，参数分别为：应用程序指针
 **/
extern void add_cancel_application_handler(si_t(*handler)(addr_t));

/**
 * 添加对初始化图形设备的处理函数
 *
 * 注意：graphics_device_init原本是graph模块已有的接口，
 * 但是需要窗口管理器设置图形设备的视频访问模式相关信息，
 * 而这些信息是和图形设备所属的应用程序的设置相关的
 * 因此在调用graphics_device_init()之后仍需要上层窗口管理器处理设置应用程序相关参数
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针，图形设备描述符
 **/
extern void add_graphics_device_init_handler(si_t(*handler)(addr_t, si_t));

/**
 * 添加对清理图形设备的处理函数
 *
 * 注意：graphics_device_exit原本是graph模块已有的接口，
 * 但是需要窗口管理器在所述的应用程序的队列中删除这个图形设备
 * 因此在调用graphics_device_exit()之后仍需要上层窗口管理器处理
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针，图形设备描述符
 **/
extern void add_graphics_device_exit_handler(si_t(*handler)(addr_t, si_t));

/**
 * 添加对退出窗口管理器命令的处理函数(需要关闭窗口管理器的权限)
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针
 **/
extern void add_window_manager_exit_handler(si_t(*handler)(addr_t));

/**
 * 添加对重写桌面的处理函数(需要桌面权限)
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针
 **/
extern void add_desktop_dirty_handler(si_t(*handler)(addr_t));

/**
 * 添加刷新图形设备的处理函数
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针, 图形设备描述符
 **/
extern void add_update_handler(si_t(*handler)(addr_t, si_t));

/**
 * 添加对激活窗口的处理函数
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针, 窗口描述符
 **/
extern void add_activate_window_handler(si_t(*handler)(addr_t, si_t));

/**
 * 添加对注册窗口的处理函数
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针, 父窗口描述符，窗口标题，初始化窗口的左上角横坐标和纵坐标，初始化窗口的长和宽，是否可以最大化，是否可以最小化，模泰
 **/
extern void add_register_window_handler(si_t(*handler)(addr_t, si_t, char*, si_t, si_t, si_t, si_t, si_t, si_t, si_t));

/**
 * 添加对取消窗口请求的处理函数
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针, 窗口描述符
 **/
extern void add_cancel_window_handler(si_t(*handler)(addr_t, si_t));

/**
 * 添加对移动窗口的处理函数
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针, 窗口描述符，新的窗口的左上角横坐标和纵坐标
 **/
extern void add_move_window_handler(si_t(*handler)(addr_t, si_t, si_t, si_t));

/**
 * 添加对重置窗口大小的请求的处理函数
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针, 窗口描述符，新的窗口的左上角横坐标和纵坐标，新的窗口的长和宽
 **/
extern void add_resize_window_handler(si_t(*handler)(addr_t, si_t, si_t, si_t, si_t, si_t));

/**
 * 添加对最小化窗口请求的处理函数
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针, 窗口描述符
 **/
extern void add_minimize_window_handler(si_t(*handler)(addr_t, si_t));

/**
 * 添加对最大化窗口请求的处理函数
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针, 窗口描述符，最大化之后的窗口区域
 **/
extern void add_maximize_window_handler(si_t(*handler)(addr_t, si_t, struct rectangle*));

/**
 * 添加对还原窗口请求的处理函数
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针, 窗口描述符, 还原区域
 **/
extern void add_restore_window_handler(si_t(*handler)(addr_t, si_t, struct rectangle*));

/**
 * 添加对获取窗口标题请求的处理函数
 *
 * @param handler 待添加的处理函数，参数分别为: 应用程序指针, 窗口描述符, 标题字符串的指针
 **/
extern void add_get_window_title_handler(si_t(*handler)(addr_t, si_t, char**));
/***********************************************************************************
 * 对消息的处理
 ***********************************************************************************/
/**
 * 发送输入消息(keybd or mouse)
 * 输入消息在event模块处获得，填充对应的key, value, code，接着期望在server_lib的调用方获取发送的窗口描述符并发送
 *
 * @param uds_ptr 客户端通信句柄 
 * @param msg 已被读取的输入事件 
 * @param window_descripter 活动窗口的描述符 
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t send_input_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter);

/**
 * 发送控件移动消息
 *
 * @param uds_ptr 客户端通信句柄
 * @param msg 已被读取的输入事件(可为NULL)
 * @param window_descripter 活动窗口的描述符
 * @param x_offset 移动的横向偏移
 * @param y_offset 移动的纵向偏移
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t send_widget_move_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter, si_t x_offset, si_t y_offset);

/**
 * 发送控件调整大小消息
 *
 * @param uds_ptr 客户端通信句柄
 * @param msg 已被读取的输入事件(可为NULL)
 * @param window_descripter 活动窗口的描述符
 * @param x_axis 新的位置的左上角横坐标
 * @param y_axis 新的位置的左上角纵坐标
 * @param x_size 新的位置的宽
 * @param y_size 新的位置的长
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t send_widget_resize_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size);

/**
 * 发送窗口激活消息
 *
 * @param uds_ptr 客户端通信句柄
 * @param msg 已被读取的输入事件(可为NULL)
 * @param window_descripter 窗口的描述符
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t send_window_activate_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter);

/**
 * 发送窗口激死消息
 *
 * @param uds_ptr 客户端通信句柄
 * @param msg 已被读取的输入事件(可为NULL)
 * @param window_descripter 窗口的描述符
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t send_window_deactivate_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter);

/**
 * 发送窗口最小化消息
 *
 * @param uds_ptr 客户端通信句柄
 * @param msg 已被读取的输入事件(可为NULL)
 * @param window_descripter 窗口的描述符
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t send_window_minimize_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter);

/**
 * 发送窗口最大化消息
 *
 * @param uds_ptr 客户端通信句柄
 * @param msg 已被读取的输入事件(可为NULL)
 * @param window_descripter 窗口的描述符
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t send_window_maximize_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter);

/**
 * 发送窗口还原消息
 *
 * @param uds_ptr 客户端通信句柄
 * @param msg 已被读取的输入事件(可为NULL)
 * @param window_descripter 窗口的描述符
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t send_window_restore_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter);

/**
 * 发送窗口关闭消息
 *
 * @param uds_ptr 客户端通信句柄
 * @param msg 已被读取的输入事件(可为NULL)
 * @param window_descripter 窗口的描述符
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t send_window_close_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter);

/**
 * 发送窗口注册消息
 *
 * @param uds_ptr 客户端通信句柄
 * @param msg 已被读取的输入事件(可为NULL)
 * @param window_descripter 窗口的描述符
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t send_window_register_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter);

/**
 * 发送窗口取消消息
 *
 * @param uds_ptr 客户端通信句柄
 * @param msg 已被读取的输入事件(可为NULL)
 * @param window_descripter 窗口的描述符
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t send_window_cancel_message(struct egui_uds* uds_ptr, union message* msg, si_t window_descripter);

/**
 * 发送应用程序退出消息
 *
 * @param uds_ptr 客户端通信句柄
 *
 * @return 成功返回0，否则返回-1
 **/
extern si_t send_application_quit_message(struct egui_uds* uds_ptr);

# endif
