/**
 * @file
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
# include "widget.h"


extern struct label * l;

extern struct button * b_0;
extern struct button * b_1;
extern struct button * b_2;
extern struct button * b_3;
extern struct button * b_4;
extern struct button * b_5;
extern struct button * b_6;
extern struct button * b_7;
extern struct button * b_8;
extern struct button * b_9;

/*
    memory recall
    读取存储器的内容
*/
extern struct button * b_mr;

/*
    memory clear
    清空存储器的内容
*/
extern struct button * b_mc;

/*
    加上存储器的内容
*/
extern struct button * b_madd;

/*
    减去存储器的内容
*/
extern struct button * b_msub;

/*
    clear
*/
extern struct button * b_c;

extern struct button * b_add;
extern struct button * b_sub;
extern struct button * b_mul;
extern struct button * b_div;

extern struct button * b_dot;

extern struct button * b_equal;

extern struct button * b_nagetive_positive;

/* 第一个操作数 */
extern char operand_1[11];
/* 第二个操作数*/
extern char operand_2[11];
/* 存储器 */
extern char memory[11];
/* 运算符 */
extern char operator_char;

extern si_t my_init();
