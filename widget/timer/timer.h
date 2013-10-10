/**
 * @file timer.h
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

# ifndef _TIMER_H_
# define _TIMER_H_ 1

/* For the use of struct itimerspec */
# define _POSIX_C_SOURCE 199309L

# include <time.h>

# include "../widget/widget.h"

struct timer {
    struct widget widget;
    struct list subscribe_info_list;
    timer_t timer;
    struct itimerspec its;
};

enum timer_event_type {
    TIMER_EVENT_ALL
};

struct timer* timer_init(si_t id);

si_t timer_exit(struct timer * b);

void timer_register_event_handler(struct timer *pub, struct widget *sub, si_t event, widget_event_handler handler);

void timer_set_time(struct timer *timer, const struct itimerspec *value);

void timer_run(struct timer *timer);

# endif
