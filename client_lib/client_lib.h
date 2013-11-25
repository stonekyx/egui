/*
 * @file   client_lib.h
 *
 * @author Dalian University of Technology
 *
 * @version 2.0
 *
 * @data 2012.02.15
 *
 * @section LICENSE
 *
 * Copyright (C) 2010 Dalian University of Technology
 *
 * This file is part of Egui.
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

# ifndef _CLIENT_LIB_H_
# define _CLIENT_LIB_H_ 1

# include "base_type.h"
# include "geometry.h"
# include "color.h"
# include "comm.h"
# include "data_structures.h"


/**
 * 使用client_lib库之前必须先init操作
 *
 * @param uds_ptr 通信句柄
 * @param queue_ptr 消息队列句柄
 **/
extern void client_lib_init(struct egui_uds* uds_ptr, struct queue* queue_ptr);

/**
 * @brief 注册应用程序
 *
 * @param video_access_mode 视频访问模式：直接写framebuffer还是先写内存缓冲区
 * @param application_type 应用程序类型
 * @param name 应用程序名字
 **/
extern si_t register_application(si_t video_access_mode, si_t application_type, const char* name);

/**
 * @brief 取消应用程序
 **/
extern si_t cancel_application();

/*
 * @brief 申请图形设备结构体
 *
 * 图形设备这个结构体可抽象为一支画笔，这支画笔拥有颜色和字体，画笔的矩形区域可抽象为一块画布。利用这支画笔，可以在显存上绘制图形。
 *
 * @param x 区域顶点的横坐标
 * @param y 区域顶点的竖坐标
 * @param width 区域的宽度
 * @param height 区域的高度
 * @param r 红色
 * @param g 绿色
 * @param b 蓝色
 * @param a 透明程度（透明程度随着 a 的增大而增大）
 * @param font 文字字体
 *
 * @return 图形设备描述符fd
 */
extern si_t graphics_device_init(si_t x, si_t y, si_t width, si_t height, si_t r, si_t g, si_t b, si_t a, si_t font);

/*
 * @brief 释放图形设备结构体
 *
 * @param gd 图形设备描述符,类似fd，可理解为画笔
 *
 * @return 0
*/
extern si_t graphics_device_exit(si_t gd);

/*
 * @brief 请求关闭窗口管理程序
 *
 * @return 0
*/
extern si_t window_manager_quit();

/*
 * @brief 刷新
 *
 * @param gd 图形设备描述符
 *
 * @return 0
*/
extern si_t update(si_t gd);

/**
 * @brief 重绘桌面
 *
 * @return 0
 **/
extern si_t desktop_dirty();

/*
 * @brief 注册窗口
 *
 *
 * @param parent_window_descriptor 父窗口描述符
 * @param title 本窗口标题
 * @param x 本窗口横坐标
 * @param y 本窗口纵坐标
 * @param width 本窗口宽度
 * @param height 本窗口长度
 * @param minimize_enable 本窗口是否可以最小化
 * @param maximize_enable 本窗口是否可以最大化
 * @param modal 本窗口模态
 *
 * @return 窗口的描述符
*/
extern si_t register_window(si_t parent_window_descriptor, const char* title, si_t x, si_t y, si_t width, si_t height, si_t minimize_enable, si_t maximize_enable, si_t modal);

/*
 * @brief 注销窗口
 *
 * @param window_descriptor 窗口描述符
 *
 * @return 0
*/
extern si_t cancel_window(si_t window_descriptor);

/*
 * @brief 移动窗口
 *
 * @param window_descriptor 窗口描述符
 * @param x_axis 目标位置的横坐标
 * @param y_axis 目标位置的纵坐标
 *
 * @return 0
*/
extern si_t move_window(si_t window_descriptor, si_t x_axis, si_t y_axis);

/*
 * @brief 改变窗口大小
 *
 * @param window_descriptor 窗口描述符
 * @param x_axis 窗口横坐标
 * @param y_axis 窗口纵坐标
 * @param x_size 窗口宽度
 * @param y_size 窗口高度
 *
 * @return 0
*/
extern si_t resize_window(si_t window_descriptor, si_t x_axis, si_t y_axis, si_t x_size, si_t y_size);

/*
 * @brief 最小化窗口
 *
 * @param window_descriptor 窗口描述符
 *
 * @return 0
*/
extern si_t minimize_window(si_t window_descriptor);

/*
 * @brief 最大化窗口
 *
 * @param window_descriptor 窗口描述符
 * @param result 最大化后的窗口大小
 *
 * @return 0
*/
extern si_t maximize_window(si_t window_descriptor, struct rectangle* result);

/*
 * @brief 还原窗口
 *
 * @param window_descriptor 窗口描述符
 * @param area pointer to restore area
 *
 * @return 0
*/
extern si_t restore_window(si_t window_descriptor, struct rectangle* area);

/**
 * @brief 激活窗口
 * 这个函数只有desktop可以使用
 *
 * @param window_descriptor 待激活的窗口
 *
 * @return 执行情况
 *
 **/
extern si_t activate_window(si_t window_descriptor);

/*
 * @brief 将屏幕设置成某种颜色
 *
 * @param gd 图形设备描述符
 *
 * @return 0
*/
extern si_t clear(si_t gd);

/*
 * @brief 将屏幕设置成黑色
 *
 * @param gd 图形设备描述符
 *
 * @return 0
*/
extern si_t clear_black(si_t gd);

/*
 * @brief 将屏幕设置成白色
 *
 * @param gd 图形设备描述符
 *
 * @return 0
*/
extern si_t clear_white (si_t gd);

/*
 * @brief 绘制一个点
 *
 * 在屏幕上绘制点（x，y）
 *
 * @param gd 图形设备描述符
 * @param x 点的横坐标
 * @param y 点的纵坐标
 *
 * @return 0
*/
extern si_t draw_point(si_t gd, si_t x, si_t y);

/*
 * @brief 绘制一条直线
 *
 * 在屏幕上绘制通过点（x1，y1）和点（x2，y2）的直线
 *
 * @param gd 图形设备描述符
 * @param x1 第一个点的横坐标
 * @param y1 第一个点的纵坐标
 * @param x2 第二个点的横坐标
 * @param y2 第二个点的纵坐标
 *
 * @return 0
*/
extern si_t draw_line(si_t gd, si_t x1, si_t y1, si_t x2, si_t y2);

/*
 * @brief 绘制一个矩形
 *
 * 在屏幕上绘制顶点坐标为（x，y）宽度为 xsize 高度为 ysize 的矩形
 *
 * @param gd 图形设备描述符
 * @param x 矩形顶点的横坐标
 * @param y 矩形顶点的竖坐标
 * @param width 矩形的宽度
 * @param width 矩形的高度
 *
 * @return 0
*/
extern si_t draw_rectangle(si_t gd, si_t x, si_t y, si_t width, si_t height);

/*
 * @brief 绘制一个多边形
 *
 * 在屏幕上绘制通过多个点的多边形
 *
 * @param gd 图形设备描述符
 * @param p 点的结构体数组
 * @param n 点的个数
 *
 * @return 0
*/
extern si_t draw_polygon(si_t gd, const struct point * p, ui_t n);

/*
 * @brief 绘制一个圆
 *
 * 在屏幕上绘制圆心为(x，y)半径为 radius 的圆
 *
 * @param gd 图形设备描述符
 * @param x  圆心的横坐标
 * @param y  圆心的纵坐标
 * @param radius 圆的半径
 * @return 0
*/
extern si_t draw_circle (si_t gd, si_t x, si_t y, si_t radius);

/*
 * @brief 绘制一个椭圆
 *
 * @details 在屏幕上绘制中心为（x，y）轴为 a、b 的椭圆
 *
 * @param gd 图形设备描述符
 * @param x 椭圆中心的横坐标
 * @param y 椭圆中心的纵坐标
 * @param a 椭圆的第一根轴
 * @param b 椭圆的第二根轴
 *
 * @return 0
*/
extern si_t draw_ellipse(si_t gd, si_t x, si_t y, si_t a, si_t b);

/*
 * @brief 绘制 X
 *
 * @param gd 图形设备描述符
 * @param x 中心横坐标
 * @param y 中心纵坐标
 * @param a 大小
 * @param b
 *
 * @return 0
**/
extern si_t draw_x(si_t gd, si_t x, si_t y, si_t a, si_t b);

/**
 * @brief 绘制一个箭头
 *
 * 箭头的画布为正方形，故参数中只需要给定 size 即可。
 *
 * @param gd 图形设备描述符
 * @param x  中心的横坐标
 * @param y  中心的纵坐标
 * @param size 大小
 * @param direction 方向 ，箭头的方向有 朝上、朝下、朝左、朝右4 个。
 *
 * @return 0
**/
extern si_t draw_arrow (si_t gd, si_t x, si_t y, si_t size, si_t direction); 

/*
 * @brief 绘制图片
 *
 * 本函数会解析其图片类型，如bmp、gif 等，然后绘制图片
 *
 * @param gd  图形设备描述符
 * @param name  图片文件名
 * @param align 图片绘制时的布局，默认是居中的。
 *
 * @return 0
 */
extern si_t draw_img(si_t gd, const char* img_path, si_t align);

/**
 * @brief 绘制bmp图
 *
 * 绘制指定路径下的bmp图像。
 *
 * @param gd 图形设备描述符
 * @param name bmp图路径
 * @param aliagn 图片的对齐方式
 *
 * @return 0
 **/
extern si_t draw_bitmap(si_t gd, const char* name, int align);

/*
 * @brief 填充一个矩形
 *
 * 在屏幕上填充顶点坐标为（x，y）宽度为 xsize 高度为 ysize 的矩形
 *
 * @param gd 图形设备描述符
 * @param x 矩形顶点的横坐标
 * @param y 矩形顶点的竖坐标
 * @param width 矩形的宽度
 * @param height 矩形的高度
 *
 * @return 0
*/
extern si_t fill_rectangle(si_t gd, si_t x, si_t y, si_t width, si_t height);

/*
 * @brief 填充一个圆
 *
 * 在屏幕上填充圆心为（x，y）半径为 radius 的圆
 *
 * @param gd 图形设备描述符
 * @param x 圆心的横坐标
 * @param y 圆心的纵坐标
 * @param radius 圆的半径
 *
 * @return 0
*/
extern si_t fill_circle(si_t gd, si_t x, si_t y, si_t radius);

/*
 * @brief 填充一个椭圆
 *
 * 在屏幕上填充中心为（x，y）轴为 a、b 的椭圆
 *
 * @param gd 图形设备描述符
 * @param x 椭圆中心的横坐标
 * @param y 椭圆中心的纵坐标
 * @param a 椭圆的第一根轴
 * @param b 椭圆的第二根轴
 *
 * @return 0
*/
extern si_t fill_ellipse(si_t gd, si_t x, si_t y, si_t a, si_t b);

/*
 * @brief 填充 X
 *
 * @param gd 图形设备描述符
 * @param x 中心横坐标
 * @param y 中心纵坐标
 * @param a 大小
 * @param b
 *
 * @return 0
*/
extern si_t fill_x(si_t gd, si_t x, si_t y, si_t a, si_t b);

/*
 * @brief 填充一个箭头
 *
 * @param gd 图形设备描述符
 * @param x 中心的横坐标
 * @param y 中心的纵坐标
 * @param size 大小
 * @param direction 方向
 *
 * @return 0
*/
extern si_t fill_arrow(si_t gd, si_t x, si_t y, si_t size, si_t direction); /* t b l r */

/*
 * @brief 填充多边形
 *
 * @param gd 图形设备描述符
 * @param p 组成多边形的点列表
 * @param point_count 多边形的点数
 *
 * @return ??? FIXME
*/
extern si_t fill_polygon(si_t gd, const struct point * p, ui_t point_count);

/*
 * @brief 区域复制
 * 
 * 功能是将源区域的信息复制到目标区域，参数中给定了源区域和目标区域的矩形左上角坐标，以及矩形区域的长和宽。
 * 例如 窗口从A 移动到 B后，使用该函数可以将窗口在A处的信息完全复制到B。 
 * 如图：
 * －－－－－－－                   －－－－－－－
 * |            |                   |            |
 * |            |                   |            |
 * －－－－－－－  > > > > > > > > > －－－－－－
 *
 * @param gd      图形设备描述符
 * @param mode    复制时采用的模式(方法)，包括 CPY_AREA_MODE_BUFFER_TO_MEMORY、CPY_AREA_MODE_MEMORY_TO_BUFFER 等8 种方式。
 * @param dst_x   目标位置的横坐标
 * @param dst_y   目标位置的纵坐标
 * @param src_x   源位置的横坐标
 * @param src_y   源位置的纵坐标
 * @param width   矩形的宽度
 * @param height  矩形的高度
 *
 * @retur 0
*/
extern si_t cpy_area(si_t gd, si_t mode, si_t dst_x, si_t dst_y, si_t src_x, si_t src_y, si_t width, si_t height);

/*
 * @brief 显示字符串
 *
 * 从屏幕上显示左上角坐标为（x，y）的字符串 str
 *
 * @param gd 图形设备描述符
 * @param x 字符串左上角横坐标
 * @param y 字符串左上角纵坐标
 * @param str 要显示的字符串
 * @param size 字符串大小
 *
 * @return 0
*/
extern si_t show_text(si_t gd, si_t x, si_t y, const char * str, si_t size);

/*
 * @brief 获得区域宽度
 *
 * @param gd 图形设备描述符
 * @param result 存放区域的地址
 *
 * @return 区域高度
*/
extern si_t get_area(si_t gd, struct rectangle * result);

/*
 * 获得区域高度
 *
 * @param gd 图形设备描述符
 * @param result 存放颜色的地址
 *
 * @return 区域高度
*/
extern si_t get_color(si_t gd, struct color * result);

/*
 * @brief 获得系统中各个颜色分量的最大值
 *
 * 获得颜色分两的最大值，结果存放到 result
 *
 * @param result 存放颜色限制的地址
 *
 * @return 成功返回 0，失败返回 -1。
*/
extern si_t get_color_limit(struct color_limit * result);

/*
 * @brief 获得文字宽度
 *
 * @param gd 图形设备描述符
 *
 * @return 文字宽度
*/
extern  si_t get_font_width(si_t gd);

/*
 * @brief 获得文字高度
 *
 * @param gd 图形设备描述符
 *
 * @return 文字高度
*/
extern si_t get_font_height(si_t gd);

/*
 * @brief 获得屏幕宽度
 *
 * @return 屏幕宽度
*/
extern si_t get_screen_width();

/*
 * @brief 获得屏幕高度
 *
 * @return 屏幕高度
*/
extern si_t get_screen_height();

/*
 * @brief 设置画笔颜色
 *
 * @param gd 图形设备描述符
 * @param r 红色
 * @param g 绿色
 * @param b 蓝色
 * @param a 透明程度（透明程度随着 a 的增大而增大）
 *
 * @return 0
**/
extern si_t set_color(si_t gd, ui_t r, ui_t g, ui_t b, ui_t a);

/*
 * @brief 设置画布的矩形区域
 *
 * @param gd 图形设备描述符
 * @param x 区域顶点的横坐标
 * @param y 区域顶点的竖坐标
 * @param widget  区域的宽度
 * @param height  区域的高度
 *
 * @return 0
*/
extern si_t set_area(si_t gd, si_t x, si_t y, si_t width, si_t height );

/*
 * @brief 设置画笔的字体
 *
 * @param gd 图形设备描述符
 * @param font 字体编码 enum __font__
 *
 * @return 0
*/
extern si_t set_font(si_t gd, si_t font);

/**
 * @brief 获取窗口标题
 *
 * @param window_descripter 窗口描述符
 *
 * @return 若成功返回malloc的标题 否则返回NULL
 **/
extern char* get_window_title(si_t window_descripter);

# endif
