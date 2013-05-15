/**
 * @file packet.h
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

# ifndef _PACKET_H_
# define _PACKET_H_ 1

# include <base_type.h>
# include <limits.h>

/**
 * 头部
**/
struct packet_head
{
    /**
     * enum __packet_type__
     *
     * 是请求、是回复、是消息
    **/
    si_t packet_type;

    /**
     * 哪种请求、哪种回复、哪种消息
    **/
    si_t detail_type;

    /** 负载的大小 **/
    ui_t size;
};

/**
 * 数据包类型
**/
enum __packet_type__
{
    /** 请求 **/
    PACKET_TYPE_REQUEST = 0,

    /** 响应 **/
    PACKET_TYPE_RESPOND,

    /** 消息 **/
    PACKET_TYPE_MESSAGE,

    /** 种类的树目 **/
    PACKET_TYPE_QUANTITY,

    /** 种类的最大数目 **/
    /** ISO C restricts enumerator values to range of 'int' **/
    PACKET_TYPE_MAX = INT_MAX
};

# endif
