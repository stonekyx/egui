/**
 * @file window_info.h
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
# ifndef WINDOW_INFO_H_
# define WINDOW_INFO_H_ 1

# include "geometry.h"
# include "data_structures.h"

struct window_info
{
    /**
     * 从 object 继承
    **/
    OBJECT_DEFINITION

    /*
     * 该窗口所在的区域
    **/
    struct rectangle area;

    /**
     * 用户工作区域
    **/
    struct rectangle work_area;

    /**
     * 标题栏区域
    **/
    struct rectangle title_bar_area;

    /**
     * 菜单按钮区域
    **/
    struct rectangle menu_button_area;

    /**
     * 最小化按钮区域
    **/
    struct rectangle minimize_button_area;

    /**
     * 最大化按钮区域
    **/
    struct rectangle maximize_button_area;

    /**
     * 退出按钮区域
    **/
    struct rectangle close_button_area;

	/** 标题 **/
    char* title;

    /** 能否最大化，1 或者 0 **/
    si_t maximize_enable;

    /** 能否最小化，1 或者 0 **/
    si_t minimize_enable;

    /** 最大的宽度 **/
    si_t maximum_x_size;

    /** 最小的宽度 **/
    si_t minimum_x_size;

    /** 最大的高度 **/
    si_t maximum_y_size;

    /** 最小的高度 **/
    si_t minimum_y_size;

    /** 是否是还原 0 is no, 1 is yes because maximized, 2 is yes because minimized **/
    si_t need_restore;

    /** 模态 **/
    si_t modal;
};

/**
 * 添加窗口配置项
 *
 * @param f 窗口边框宽
 * @param t 窗口标题栏高度
 **/
extern void window_info_config(si_t f, si_t t);

/**
 * 初始化一个window_info
 *
 * @param title 窗口标题
 * @param maxable 是否可以最大化（偷懒 写一个有语法错误的名字）
 * @param minable 是否可以最小化（偷懒 写一个有语法错误的名字）
 * @param modal 模态
 *
 * @return 成功返回窗口指针,否则返回NULL
 **/
extern struct window_info* window_info_init(char* title, si_t maxable, si_t minable, si_t modal);
extern void window_info_exit(struct window_info* w);

/**
 * @brief 清理一个window_info以及所有子孙window_info
 *
 * @param win_info_ptr 窗口指针
 **/
extern void window_info_tree_exit(struct window_info* win_info_ptr);

/**
 * @brief 移动窗口
 *
 * @param win_info_ptr 窗口指针
 * @param x_offset 横向偏移
 * @param y_offset 纵向偏移
 **/
extern void window_info_move(struct window_info* win_info_ptr, si_t x_offset, si_t y_offset);

/**
 * @brief 设置窗口各部分大小
 *
 * @param win_info_ptr 窗口指针
 * @param x 窗口工作区域横坐标
 * @param y 窗口工作区域纵坐标
 * @param w 窗口工作区域宽
 * @param h 窗口工作区域高
 **/
extern void window_info_resize(struct window_info* win_info_ptr, si_t x, si_t y, si_t w, si_t h);

/**
 * @brief 窗口最大化
 *
 * @param win_info_ptr 窗口指针
 * @param max_area 最大化之后的窗口区域
 **/
extern void window_info_maximum(struct window_info* win_info_ptr, struct rectangle* max_area);

/**
 * @brief 判断点是否在窗口左边框内
 *
 * @param win_info_ptr 窗口指针
 * @param p 点坐标
 *
 * @return 是返回1，否返回0
 **/
extern si_t window_info_is_click_left_frame(struct window_info* win_info_ptr, struct point* p);

/**
 * @brief 判断点是否在窗口右边框内
 *
 * @param win_info_ptr 窗口指针
 * @param p 点坐标
 *
 * @return 是返回1，否返回0
 **/
extern si_t window_info_is_click_right_frame(struct window_info* win_info_ptr, struct point* p);

/**
 * @brief 判断点是否在窗口上边框内
 *
 * @param win_info_ptr 窗口指针
 * @param p 点坐标
 *
 * @return 是返回1，否返回0
 **/
extern si_t window_info_is_click_up_frame(struct window_info* win_info_ptr, struct point* p);

/**
 * @brief 判断点是否在窗口下边框内
 *
 * @param win_info_ptr 窗口指针
 * @param p 点坐标
 *
 * @return 是返回1，否返回0
 **/
extern si_t window_info_is_click_down_frame(struct window_info* win_info_ptr, struct point* p);

/**
 * @brief 返回窗口可以拥有的最小宽度
 *
 * @param win_info_ptr 窗口指针
 **/
extern si_t window_info_least_width(struct window_info* win_info_ptr);

#endif

