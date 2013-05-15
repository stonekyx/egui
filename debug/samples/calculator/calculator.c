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

# include <stdio.h>
# include <string.h>
# include <stdlib.h>

# include <unistd.h>

# include "client_lib.h"
# include "application.h"

# include "calculator.h"

struct label * l;

struct button * b_0;
struct button * b_1;
struct button * b_2;
struct button * b_3;
struct button * b_4;
struct button * b_5;
struct button * b_6;
struct button * b_7;
struct button * b_8;
struct button * b_9;

/*
    memory recall
    读取存储器的内容
*/
struct button * b_mr;

/*
    memory clear
    清空存储器的内容
*/
struct button * b_mc;

/*
    加上存储器的内容
*/
struct button * b_madd;

/*
    减去存储器的内容
*/
struct button * b_msub;

/*
    clear
*/
struct button * b_c;

struct button * b_add;
struct button * b_sub;
struct button * b_mul;
struct button * b_div;

struct button * b_dot;

struct button * b_equal;

struct button * b_nagetive_positive;

/* 第一个操作数 */
char operand_1[11];
/* 第二个操作数*/
char operand_2[11];
/* 存储器 */
char memory[11];
/* 运算符 */
char operator_char;

/* 0~9 */
si_t button_operand_callback(void * btn, void * msg)
{
    struct button * b = btn;
    union message * m = msg;
    si_t length = strlen(operand_2);

    switch(m->base.type)
    {
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
            /*
            printf("%lX single click\n", (si_t)b);
            */

            if(operand_2[0] == '0' && operand_2[1] == 0)
            {
                if(b->text[0] == '.')
                {
                    operand_2[0] = '0';
                    operand_2[1] = '.';
                    operand_2[2] = 0;
                }
                else
                {
                    operand_2[0] = b->text[0];
                    operand_2[1] = 0;
                }
            }
            else
            {
                if(length < 10)
                {
                    operand_2[length] = b->text[0];
                    operand_2[length + 1] = 0;
                }
            }

            l->text = operand_2;
            label_repaint(l);
            label_show(l);

            break;

        default:
            button_default_callback(btn, msg);
            break;
    }


    return 0;
}

/* + - *、/  */
si_t button_operator_callback(void * btn, void * msg)
{
    struct button * b = btn;
    union message * m = msg;
	si_t error_flag ;
	/*    f_t f1, f2; */
    si_t i1, i2;
	error_flag = 0;
    switch(m->base.type)
    {
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
            /*
            printf("%lX single click\n", (si_t)b);
            */

            memcpy(operand_2, l->text, 11);

            /* 第一次运算 */
            if(operator_char == 0)
            {
                memcpy(operand_1, operand_2, 11);

                operand_2[0] = 0;
            }
            else
            {
                /*
                f1 = atof(operand_1);
                f2 = atof(operand_2);
                */
                i1 = atoi(operand_1);
                i2 = atoi(operand_2);

                /* 上一次的符号 */
                if(operator_char == '+')
                {
                    /*
                    f1 += f2;
                    */
                    i1 += i2;
                }
                else if(operator_char == '-')
                {
                    /*
                    f1 -= f2;
                    */
                    i1 -= i2;
                }
                else if(operator_char == '*')
                {
                    /*
                    f1 *= f2;
                    */
                    i1 *= i2;
                }
                else if(operator_char == '/')
                {
                    /*
                    f1 /= f2;
                    */
                    if(i2 != 0)
                    {
                        i1 /= i2;
                    }
					else
					{
						error_flag = 1;
					}
                }
                sprintf(operand_1, "%d", (int)i1);

                operand_2[0] = '0';
                operand_2[1] = 0;
                l->text =(error_flag == 0)? operand_1 : "Error" ;

                label_repaint(l);
                label_show(l);
            }

            if(b->text[0] == '+')
            {
                operator_char = '+';
            }
            else if(b->text[0] == '-')
            {
                operator_char = '-';
            }
            else if(b->text[0] == '*')
            {
                operator_char = '*';
            }
            else if(b->text[0] == '/')
            {
                operator_char = '/';
            }
            else if(b->text[0] == '=')
            {
                operator_char = 0;
            }

            break;

        default:
            button_default_callback(btn, msg);
            break;
    }

    return 0;
}

si_t button_special_callback(void *btn , void *msg)
{
	struct button * b = (struct button*)btn;
	union message * m = (union message*)msg;
	si_t temp_memory , curr_operand;
	
	temp_memory = atoi(memory);
	curr_operand = atoi(l->text);

	switch(m->base.type)
	{
		case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
			if(b->text[1] == 'S')
			{
				memcpy(memory,l->text,11);
				printf("MS Done --> now memory=%d\n",atoi(memory));
			}
			else if(b->text[1] == 'R') 
			{
				sprintf(operand_2,"%d", atoi(memory));
				l->text = operand_2;
				operator_char = 0;
				operand_1[0] = '0';
				operand_1[1] = 0;

				label_repaint(l);
				label_show(l);
				printf("MR Done --> now memory= %d\n",atoi(memory));
			}
			else if(b->text[1] == '+')
			{
				temp_memory += curr_operand;
				sprintf(memory, "%d", (int)temp_memory);
				printf("M+ Done --> now memory=%d\n",atoi(memory));
	
			}
			else if(b->text[1] == '-')
			{
				temp_memory -= curr_operand;
				sprintf(memory, "%d", (int)temp_memory);
				printf("M- Done --> now memory=%d\n",atoi(memory));
			}
			else if(b->text[1] == 'C')
			{
				memory[0] = '0';
				memory[1] = 0; 	
				printf("MC Done --> now memory=%d\n",atoi(memory));
			}
			break;
		default:
			button_default_callback(btn,msg);
			break;

	}
	return 0;
}
si_t button_clear_callback(void * btn, void * msg)
{
    union message * m = msg;

    switch(m->base.type)
    {
        case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
            /*
            printf("%lX single click\n", (si_t)b);
            */
            l->text = "0";
            operand_1[0] = '0';
            operand_2[0] = '0';
            memory[0] = '0';
            operand_1[1] = 0;
            operand_2[1] = 0;
            memory[1] = 0;
            operator_char = 0;

            label_repaint(l);
            label_show(l);

        default:
            button_default_callback(btn, msg);
            break;
    }

    return 0;
}

si_t my_init()
{
    operand_1[0] = '0';
    operand_2[0] = '0';
    memory[0] = '0';
    operand_1[1] = 0;
    operand_2[1] = 0;
    memory[1] = 0;
    operator_char = 0;

    l = label_init("0");
    if(l == NULL)
    {
        application_exit();

        return -1;
    }
    l->border_size = 1;
	label_set_bounds(l, 0, 0, 163, 34);
    l->back_color.r = 0;
    l->back_color.g = 255;
    l->back_color.b = 0;
    l->back_color.a = 0;
	label_set_font(l, FONT_MATRIX_16);
    l->callback = label_default_callback;

    /* 申请按钮 */
    b_mc = button_init("MC");
    /* 申请失败 */
    if(b_mc == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_mc,0,35,40,34);
	button_set_font(b_mc, FONT_MATRIX_16);
    b_mc->callback = button_special_callback;

    /* 申请按钮 */
    b_madd = button_init("M+");
    /* 申请失败 */
    if(b_madd == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_madd, 41, 35, 40, 34);
	button_set_font(b_madd, FONT_MATRIX_16);
    b_madd->callback = button_special_callback;

    /* 申请按钮 */
    b_msub = button_init("M-");
    /* 申请失败 */
    if(b_msub == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_msub, 82, 35, 40, 34);
	button_set_font(b_msub, FONT_MATRIX_16);
    b_msub->callback = button_special_callback;

    /* 申请按钮 */
    b_mr = button_init("MR");
    /* 申请失败 */
    if(b_mr == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_mr, 123, 35, 40, 34);
	button_set_font(b_mr, FONT_MATRIX_16);
    b_mr->callback = button_special_callback;

    /* 申请按钮 */
    b_add = button_init("+");
    /* 申请失败 */
    if(b_add == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_add, 0, 70, 40, 34);
    button_set_font(b_add, FONT_MATRIX_16);
    b_add->callback = button_operator_callback;

    /* 申请按钮 */
    b_sub = button_init("-");
    /* 申请失败 */
    if(b_sub == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_sub, 41, 70, 40, 34);
    button_set_font(b_sub, FONT_MATRIX_16);
    b_sub->callback = button_operator_callback;

    /* 申请按钮 */
    b_mul = button_init("*");
    /* 申请失败 */
    if(b_mul == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_mul, 82, 70, 40, 34);
    button_set_font(b_mul, FONT_MATRIX_16);
    b_mul->callback = button_operator_callback;

    /* 申请按钮 */
    b_div = button_init("/");
    /* 申请失败 */
    if(b_div == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_div, 123, 70, 40, 34);
    button_set_font(b_div, FONT_MATRIX_16);
    b_div->callback = button_operator_callback;

    /* 申请按钮 */
    b_7 = button_init("7");
    /* 申请失败 */
    if(b_7 == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_7, 0, 105, 40, 34);
    button_set_font(b_7, FONT_MATRIX_16);
    b_7->callback = button_operand_callback;

    /* 申请按钮 */
    b_8 = button_init("8");
    /* 申请失败 */
    if(b_8 == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_8, 41, 105, 40, 34);
    button_set_font(b_8, FONT_MATRIX_16);
    b_8->callback = button_operand_callback;

    /* 申请按钮 */
    b_9 = button_init("9");
    /* 申请失败 */
    if(b_9 == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_9, 82, 105, 40, 34);
    button_set_font(b_9, FONT_MATRIX_16);
    b_9->callback = button_operand_callback;

    /* 申请按钮 */
    b_c = button_init("C");
    /* 申请失败 */
    if(b_c == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_c, 123, 105, 40, 34);
    button_set_font(b_c, FONT_MATRIX_16);
    b_c->callback = button_clear_callback;

    /* 申请按钮 */
    b_4 = button_init("4");
    /* 申请失败 */
    if(b_4 == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_4, 0, 140, 40, 34);
    button_set_font(b_4, FONT_MATRIX_16);
    b_4->callback = button_operand_callback;

    /* 申请按钮 */
    b_5 = button_init("5");
    /* 申请失败 */
    if(b_5 == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_5, 41, 140, 40, 34);
    button_set_font(b_5, FONT_MATRIX_16);
    b_5->callback = button_operand_callback;

    /* 申请按钮 */
    b_6 = button_init("6");
    /* 申请失败 */
    if(b_6 == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_6, 82, 140, 40, 34);
    button_set_font(b_6, FONT_MATRIX_16);
    b_6->callback = button_operand_callback;

    /* 申请按钮 */
    b_nagetive_positive = button_init("MS");
    /* 申请失败 */
    if(b_nagetive_positive == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_nagetive_positive, 123, 140, 40, 34);
    button_set_font(b_nagetive_positive, FONT_MATRIX_16);
    b_nagetive_positive->callback = button_special_callback;

    /* 申请按钮 */
    b_1 = button_init("1");
    /* 申请失败 */
    if(b_1 == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_1, 0, 175, 40, 34);
    button_set_font(b_1, FONT_MATRIX_16);
    b_1->callback = button_operand_callback;

    /* 申请按钮 */
    b_2 = button_init("2");
    /* 申请失败 */
    if(b_2 == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_2, 41, 175, 40, 34);
    button_set_font(b_2, FONT_MATRIX_16);
    b_2->callback = button_operand_callback;

    /* 申请按钮 */
    b_3 = button_init("3");
    /* 申请失败 */
    if(b_3 == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_3, 82, 175, 40, 34);
    button_set_font(b_3, FONT_MATRIX_16);
    b_3->callback = button_operand_callback;

    /* 申请按钮 */
    b_equal = button_init("=");
    /* 申请失败 */
    if(b_equal == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_equal, 123, 175, 40, 69);
    button_set_font(b_equal, FONT_MATRIX_16);
    b_equal->callback = button_operator_callback;

    /* 申请按钮 */
    b_0 = button_init("0");
    /* 申请失败 */
    if(b_0 == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_0, 0, 210, 81, 34);
    button_set_font(b_0, FONT_MATRIX_16);
    b_0->callback = button_operand_callback;

    /* 申请按钮 */
    b_dot = button_init(".");
    /* 申请失败 */
    if(b_dot == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b_dot, 82, 210, 40, 34);
    button_set_font(b_dot, FONT_MATRIX_16);
    b_dot->callback = button_operand_callback;

    return 0;
}
