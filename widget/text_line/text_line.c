/**
 * @file text_line.h
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "text_line.h"
#include "base_type.h"
#include "config_parser.h"
#include "log.h"
#include "comm.h"
#include "client_lib.h"

#define TMP_ARRAY_SIZE 256
#define MAXBUF 80

/* 文本行样式全局对象 */
static struct text_line_style text_line_default_style =
{
    {
        /* 初始化，默认未访问 */
        0,  /* .flag */

        /* 默认工作区域 */
        0,  /* .area_x */
        0,  /* .area_y */
        0,  /* .area_width */
        0,  /* .area_height */

        /* 默认边界宽度 */
        1,  /* .border_size */

        /* 默认宽度&高度 */
        0,  /* .maximum_width */
        0,  /* .minimum_width */
        0,  /* .maximum_height */
        0,  /* .minimum_height */

        /* 默认鼠标形状 */
        CURSOR_SHAPE_X, /* .cursor */

        /* 默认背景色 */
        /* FIXME:不应该hard code */
        255,    /* .back_color_r */
        255,    /* .back_color_g */
        255,  /* .back_color_b */
        0,  /* .back_color_a */

        /* 默认前景色 */
        0,  /* .fore_color_r */
        0,  /* .fore_color_g */
        0,  /* .fore_color_b */
        0,  /* .fore_color_a */
    },

    /* 默认边框颜色 */
    0xcb,  /* .frame_color_r */
    0xf3,  /* .frame_color_g */
    0xfb,  /* .frame_color_b */
    0,  /* .frame_color_a */

    /* 默认字体 */
    FONT_MATRIX_08, /* .font */
};

/**
 * @brief 用text_line全局样式对象初始化text_line对象
 *
 * @param tl text_line指针
 *
 * @return 成功返回0，否则返回-1
 **/
static si_t text_line_init_with_default_style(struct text_line * tl)
{
    char tmp_str[TMP_ARRAY_SIZE] = {'\0'};
    si_t tmp_int = -1;
    char *config_path = get_config_path("text_line.cfg");
    struct widget_style_entry extra[] = {
        {.key="frame_color_r", .type=WIDGET_STYLE_TYPE_INT,
            .val=&(text_line_default_style.frame_color_r)},
        {.key="frame_color_g", .type=WIDGET_STYLE_TYPE_INT,
            .val=&(text_line_default_style.frame_color_g)},
        {.key="frame_color_b", .type=WIDGET_STYLE_TYPE_INT,
            .val=&(text_line_default_style.frame_color_b)},
        {.key="frame_color_a", .type=WIDGET_STYLE_TYPE_INT,
            .val=&(text_line_default_style.frame_color_a)},
        {.key="font", .type=WIDGET_STYLE_TYPE_STR,
            .val=&(text_line_default_style.font)},
    };

    si_t res = widget_init_with_default_style(config_path,
            WIDGET_POINTER(tl), &text_line_default_style.common,
            extra, sizeof(extra)/sizeof(extra[0]));
    free(config_path);

    if((tmp_int = get_font_enum_from_str(tmp_str)) >= 0) {
        text_line_default_style.font = tmp_int;
    }

    tl->frame_color.r = text_line_default_style.frame_color_r;
    tl->frame_color.g = text_line_default_style.frame_color_g;
    tl->frame_color.b = text_line_default_style.frame_color_b;
    tl->frame_color.a = text_line_default_style.frame_color_a;

    tl->font = text_line_default_style.font;

    return res;
}

/* declaration of static functions {{{ */
static char keybd_code_to_char_number(si_t code);
static char keybd_code_to_char_letter(si_t code);
static si_t text_line_handle_special_key(struct text_line* t, si_t code);
static char* get_next_line_start(char* this_line_start_index, si_t maxline, si_t is_enter_enable, si_t is_linewrap);
static char* get_prev_line_start(char* this_line_start_index, si_t maxline, si_t is_enter_enable, si_t is_linewrap);
static si_t lines_between(char* p, char* q, si_t maxline, si_t is_enter_enable, si_t is_linewrap);
static si_t text_line_default_show_text(struct text_line* t, si_t x, si_t y);
static si_t text_line_draw_frame(si_t gd, si_t x, si_t y, si_t width, si_t height, si_t margin);
/* }}} */
extern si_t text_line_default_widget_show(struct text_line* t, union message* msg)
{
    struct rectangle area;

    widget_absolute_area(WIDGET_POINTER(t), &area);

    /* 设置区域 */
    set_area
    (t->gd,
     area.x,
     area.y,
     area.width,
     area.height);

    /* 更新 */
    update(t->gd);

    return 0;
}

static char keybd_code_to_char_number(si_t code)
{
    switch(code)
    {
    case INPUT_CODE_KEYBD_1:
        return '1';
    case INPUT_CODE_KEYBD_2:
        return '2';
    case INPUT_CODE_KEYBD_3:
        return '3';
    case INPUT_CODE_KEYBD_4:
        return '4';
    case INPUT_CODE_KEYBD_5:
        return '5';
    case INPUT_CODE_KEYBD_6:
        return '6';
    case INPUT_CODE_KEYBD_7:
        return '7';
    case INPUT_CODE_KEYBD_8:
        return '8';
    case INPUT_CODE_KEYBD_9:
        return '9';
    case INPUT_CODE_KEYBD_0:
        return '0';
    case INPUT_CODE_KEYBD_KP_7:
        return '7';
    case INPUT_CODE_KEYBD_KP_8:
        return '8';
    case INPUT_CODE_KEYBD_KP_9:
        return '9';
    case INPUT_CODE_KEYBD_KP_SUB:
    case INPUT_CODE_KEYBD_SUB:
        return '-';
    case INPUT_CODE_KEYBD_KP_4:
        return '4';
    case INPUT_CODE_KEYBD_KP_5:
        return '5';
    case INPUT_CODE_KEYBD_KP_6:
        return '6';
    case INPUT_CODE_KEYBD_KP_ADD:
        return '+';
    case INPUT_CODE_KEYBD_KP_1:
        return '1';
    case INPUT_CODE_KEYBD_KP_2:
        return '2';
    case INPUT_CODE_KEYBD_KP_3:
        return '3';
    case INPUT_CODE_KEYBD_KP_0:
        return '0';
    case INPUT_CODE_KEYBD_KP_DOT:
        return '.';
    case INPUT_CODE_KEYBD_KP_SLASH:
        return '/';
    case INPUT_CODE_KEYBD_SPACE:
        return ' ';
    default:
        break;
    }
    return 0;
}


static char keybd_code_to_char_letter(si_t code)
{
    switch(code)
    {
    case INPUT_CODE_KEYBD_Q:
        return 'q';
    case INPUT_CODE_KEYBD_W:
        return 'w';
    case INPUT_CODE_KEYBD_E:
        return 'e';
    case INPUT_CODE_KEYBD_R:
        return 'r';
    case INPUT_CODE_KEYBD_T:
        return 't';
    case INPUT_CODE_KEYBD_Y:
        return 'y';
    case INPUT_CODE_KEYBD_U:
        return 'u';
    case INPUT_CODE_KEYBD_I:
        return 'i';
    case INPUT_CODE_KEYBD_O:
        return 'o';
    case INPUT_CODE_KEYBD_P:
        return 'p';
    case INPUT_CODE_KEYBD_A:
        return 'a';
    case INPUT_CODE_KEYBD_S:
        return 's';
    case INPUT_CODE_KEYBD_D:
        return 'd';
    case INPUT_CODE_KEYBD_F:
        return 'f';
    case INPUT_CODE_KEYBD_G:
        return 'g';
    case INPUT_CODE_KEYBD_H:
        return 'h';
    case INPUT_CODE_KEYBD_J:
        return 'j';
    case INPUT_CODE_KEYBD_K:
        return 'k';
    case INPUT_CODE_KEYBD_L:
        return 'l';
    case INPUT_CODE_KEYBD_Z:
        return 'z';
    case INPUT_CODE_KEYBD_X:
        return 'x';
    case INPUT_CODE_KEYBD_C:
        return 'c';
    case INPUT_CODE_KEYBD_V:
        return 'v';
    case INPUT_CODE_KEYBD_B:
        return 'b';
    case INPUT_CODE_KEYBD_N:
        return 'n';
    case INPUT_CODE_KEYBD_M:
        return 'm';
    case INPUT_CODE_KEYBD_SPACE:
        return ' ';
    default:
        break;
    }
    return 0;
}

/**
 * function that will be called in list_for_each for each subscribe_info of the scroll_bar
 **/
static si_t event_to_be_dispatched;
static si_t do_handle_event(void* subscribe_info)
{
    struct widget_subscribe_info* si = (struct widget_subscribe_info*)subscribe_info;
    if(si->event == TEXT_LINE_EVENT_ALL || si->event == event_to_be_dispatched)
        si->handler(si->subscriber, si->publisher, event_to_be_dispatched);
    return 0;
}

/**
 * only change ruler_cur
 **/
static si_t text_line_handle_special_key(struct text_line* t, si_t code)
{
    si_t buflen = strlen(t->buf);
    switch(code)
    {
        /**
         * handle backspace
         **/
    case INPUT_CODE_KEYBD_BACKSPACE:
        /**
         * in case out of range
         **/
        if(t->ruler_cur == t->buf)
        {
            return -1;
        }
        else
        {
            char* cur = -- t->ruler_cur - 1;

            /**
             * move back
             **/
            while(++ cur != t->buf + buflen)
            {
                *cur = *(cur + 1);
            }
            *cur = '\0';

            /**
             * dispatch cursor_move event to subscriber
             **/
            event_to_be_dispatched = TEXT_LINE_EVENT_CURRENT_CHANGE;
            list_for_each(&t->subscribe_info_list, do_handle_event);
        }
        break;

        /**
         * handle enter
         **/
    case INPUT_CODE_KEYBD_ENTER:
        /**
         * make sure it's handleable
         **/
        if(!t->is_enter_enable)
            return -1;
        else
        {
            char* cur = t->buf + buflen;
            /**
             * in case of overflow
             **/
            if(buflen + 1 > t->buf_size - 1)
                return -1;

            if(t->buf + buflen > t->ruler_cur)
            {
                /**
                 * move back
                 **/
                while(-- cur != t->ruler_cur)
                {
                    *(cur + 1) = *cur;
                }
                *(t->ruler_cur) = '\n';
                ++ t->ruler_cur;
            }
            else
            {
                /**
                 * update buf
                 **/
                *(t->ruler_cur) = '\n';
                ++ t->ruler_cur;
                *(t->ruler_cur) = '\0';
            }

            /**
             * dispatch cursor_move event to subscriber
             **/
            event_to_be_dispatched = TEXT_LINE_EVENT_CURRENT_CHANGE;
            list_for_each(&t->subscribe_info_list, do_handle_event);
        }
        break;

        /**
         * handle left arrow
         **/
    case INPUT_CODE_KEYBD_LEFT:
        /**
         * in case out of range
         **/
        if(t->ruler_cur == t->buf)
        {
            return -1;
        }
        -- t->ruler_cur;

        /**
         * dispatch cursor_move event to subscriber
         **/
        event_to_be_dispatched = TEXT_LINE_EVENT_CURRENT_CHANGE;
        list_for_each(&t->subscribe_info_list, do_handle_event);

        break;

        /**
         * handle right arrow
         **/
    case INPUT_CODE_KEYBD_RIGHT:
        /**
         * in case out of range
         **/
        if(t->ruler_cur == t->buf + buflen)
        {
            return -1;
        }
        ++ t->ruler_cur;

        /**
         * dispatch cursor_move event to subscriber
         **/
        event_to_be_dispatched = TEXT_LINE_EVENT_CURRENT_CHANGE;
        list_for_each(&t->subscribe_info_list, do_handle_event);

        break;

        /**
         * handle up arrow
         **/
    case INPUT_CODE_KEYBD_UP:
        /**
         * only handle when is_wrap was set
         * or is_enter_enable was set
         **/
        if(!t->is_linewrap && !t->is_enter_enable)
        {
            return -1;
        }
        else
        {
            char* prev_line_start = get_prev_line_start(t->ruler_line_start, t->chars_per_line, t->is_enter_enable, t->is_linewrap);
            fflush(stdout);
            if(prev_line_start == NULL)
                break;
            /**
             * this line is longer than previous line, like this:
             * before:
             * x x x
             * x x x x|x
             * after:
             * x x x|
             * x x x x x
             **/
            /**
             * minus 1: keep the '\n' in mind
             **/
            if(prev_line_start + (t->ruler_cur - t->ruler_line_start) > t->ruler_line_start - 1)
            {
                t->ruler_cur = t->ruler_line_start - 1;
            }
            else
            {
                t->ruler_cur = prev_line_start + (t->ruler_cur - t->ruler_line_start);
            }

            /**
             * dispatch cursor_move event to subscriber
             **/
            event_to_be_dispatched = TEXT_LINE_EVENT_CURRENT_CHANGE;
            list_for_each(&t->subscribe_info_list, do_handle_event);
        }

        break;

        /**
         * handle down arrow
         **/
    case INPUT_CODE_KEYBD_DOWN:
        /**
         * only handle when is_wrap was set
         * or is_enter_enable was set
         **/
        if(!t->is_linewrap && !t->is_enter_enable)
        {
            return -1;
        }
        else
        {
            char* next_line_start = get_next_line_start(t->ruler_line_start, t->chars_per_line, t->is_enter_enable, t->is_linewrap);
            char* next_next_line_start;
            if(next_line_start == NULL)
            {
                break;
            }
            next_next_line_start = get_next_line_start(next_line_start, t->chars_per_line, t->is_enter_enable, t->is_linewrap);
            /**
             * like this:
             * before:
             * x x x|x
             * x x
             * after:
             * x x x x
             * x x|
             **/
            if(next_next_line_start == NULL && next_line_start + (t->ruler_cur - t->ruler_line_start) > t->buf + buflen - 1)
                t->ruler_cur = t->buf + buflen;
            /**
             * like this:
             * before:
             * x x x|x
             * x x
             * x x x
             * after:
             * x x x x
             * x x|
             * x x x
             **/
            else if(next_next_line_start != NULL && next_line_start + (t->ruler_cur - t->ruler_line_start) > next_next_line_start)
                t->ruler_cur = next_next_line_start - 1;
            else
                t->ruler_cur = next_line_start + (t->ruler_cur - t->ruler_line_start);

            /**
             * dispatch cursor_move event to subscriber
             **/
            event_to_be_dispatched = TEXT_LINE_EVENT_CURRENT_CHANGE;
            list_for_each(&t->subscribe_info_list, do_handle_event);
        }
        break;

    default:
        return 1;
    }
    return 0;
}

/**
 * returns pointer to character that's the start index of next line
 * return NULL if "next line"do not exists
 **/
static char* get_next_line_start(char* this_line_start, si_t maxline, si_t is_enter_enable, si_t is_linewrap)
{
    if(!is_enter_enable)
    {
        if(strlen(this_line_start) < maxline)
            return NULL;
        return this_line_start + maxline;
    }
    if(is_linewrap)
    {
        char* next = this_line_start;
        si_t i = 0;
        if(*next == '\n')
            return next + 1;
        while(*(++ next) != '\n')
        {
            if(next - this_line_start == maxline - 1)
                break;
            if(*next == '\0')
                return NULL;
            ++ i;
        }
        return next + 1;
    }
    else
    {
        char* this_sentence_start = this_line_start;
        char* next_sentence_start = this_line_start;
        /**
         * find the offset to start index of this sentence
         **/
        while(*(--this_sentence_start) != '\n')
            if(*this_sentence_start == '\0')
                return NULL;
        ++ this_sentence_start;

        /**
         * find the start index of next sentence
         **/
        while(*(++ next_sentence_start) != '\n')
            if(*next_sentence_start == '\0')
                return NULL;
        -- next_sentence_start;

        return next_sentence_start + (this_sentence_start - this_line_start);
    }
}

/**
 * returns pointer to character that's the start index of previous line
 * return NULL if "previous line" do not exists
 **/
static char* get_prev_line_start(char* this_line_start, si_t maxline, si_t is_enter_enable, si_t is_linewrap)
{
    if(!is_enter_enable)
    {
        char* cur = this_line_start;
        while(*(-- cur) != '\0')
            if(this_line_start - cur == maxline)
                return cur;
        return NULL;
    }
    if(is_linewrap)
    {
        char* prev = this_line_start;
        char* last;

        if(*(prev - 1) == '\0')
            return NULL;

        if(*(prev - 1) == '\n')
            -- prev;
        last = prev;
        while(*(-- prev) != '\n')
        {
            if(*prev == '\0')
                return prev + 1;
            if(last - prev == maxline)
                return prev;
        }
        return prev + 1;
    }
    else
    {
        char* this_sentence_start = this_line_start;
        char* prev_sentence_start;
        /**
         * find the offset to start index of this sentence
         **/
        while(*(--this_sentence_start) != '\n')
            if(*this_sentence_start == '\0')
                break;
        ++ this_sentence_start;

        /**
         * find the start index of previous sentence
         **/
        prev_sentence_start = this_sentence_start - 1;
        while(*(-- prev_sentence_start) != '\n')
            if(*prev_sentence_start == '\0')
                return NULL;
        ++ prev_sentence_start;

        return prev_sentence_start + (this_sentence_start - this_line_start);
    }
    return NULL;
}

/**
 * returns lines between p and q
 * p is the start of certain line
 **/
static si_t lines_between(char* p, char* q, si_t maxline, si_t is_enter_enable, si_t is_linewrap)
{
    if(q - p < 0)
        return -1;
    
    /**
     * then it's a long, long sentence: only calculate distance
     **/
    if(!is_enter_enable)
        return (q - p) / maxline;

    /**
     * some long, long senteces: only count enter between p and q
     **/
    if(!is_linewrap)
    {
        si_t line = 0;
        char* next;
        while((next = strstr(next, "\n")) != NULL)
        {
            if(next > q)
                return line;
            ++ line;
        }
        return line;
    }
    else
    {
        char* cur = p;
        char* start = cur;
        si_t line = 0;
        if(cur == q)
            return 0;
        while(++ cur != q)
        {
            if(*cur == '\n' || cur - start == maxline)
            {
                line ++;
                start = cur;
            }
        }
        return line;
    }
    return -1;
}

static si_t text_line_default_show_text(struct text_line* t, si_t x, si_t y)
{
    si_t fontx=0, fonty=0, startx=0, starty=0, rulerx=0, rulery1=0, rulery2=0, max_line=0;

    /**
     * start axis;
     * the magic number 6 is:
     * 2 for outter frame
     * + 3 for inner frame
     * + 1 for margin between text and the inner frame
     **/
    max_line = text_line_get_max_line_shown(t);
    fonty = get_font_height(t->gd);
    fontx = get_font_width(t->gd);
    startx = x + 6;
    starty = y + 6 + (t->area.height - 12 - fonty * max_line) / 2;

    /**
     * if there are text in buffer
     **/
    if(strlen(t->buf) != 0)
    {
        char* strshow;
        char* start;
        si_t i = 0;
        si_t lines;

        /**
         * adjust start index
         * also, lines refers to the line that current ruler is in
         **/
        lines = lines_between(t->show_start, t->ruler_cur, t->chars_per_line, t->is_enter_enable, t->is_linewrap);
        if(lines == -1)
        {
            start = get_prev_line_start(t->show_start, t->chars_per_line, t->is_enter_enable, t->is_linewrap);
            if(!start)
                return -1;
            t->show_start = start;
            lines = 0;

            /**
             * dispatch page up event to subscriber
             **/
            event_to_be_dispatched = TEXT_LINE_EVENT_PAGE_UP;
            list_for_each(&t->subscribe_info_list, do_handle_event);
        }
        else if(lines == max_line)
        {
            start = get_next_line_start(t->show_start, t->chars_per_line, t->is_enter_enable, t->is_linewrap);
            if(!start)
                return -1;
            t->show_start = start;
            lines = max_line - 1;

            /**
             * dispatch page down event to subscriber
             **/
            event_to_be_dispatched = TEXT_LINE_EVENT_PAGE_DOWN;
            list_for_each(&t->subscribe_info_list, do_handle_event);
        }

        /**
         * show line by line
         **/
        strshow = t->show_start;
        while(i != max_line && strshow != NULL)
        {
            char* next = get_next_line_start(strshow, t->chars_per_line, t->is_enter_enable, t->is_linewrap);
            si_t showlen;
            if(next == NULL)
            {
                showlen = strlen(strshow);
            }
            else
            {
                showlen = next - strshow;
            }

            /**
             * locate ruler
             **/
            if(i == lines)
            {
                t->ruler_line_start = strshow;
                rulerx = startx + (t->ruler_cur - strshow) * fontx; 
                rulery1 = starty;
                rulery2 = starty + fonty;
            }

            /**
             * show text
             **/
            set_color(t->gd,
                t->fore_color.r,
                t->fore_color.g,
                t->fore_color.b,
                t->fore_color.a);
            if(text_line_is_keybd_type_password(t))
            {
                char tmpstr[MAXBUF];
                si_t left = showlen;

                while(left > MAXBUF)
                {
                    memset(tmpstr, '*', MAXBUF);
                    show_text(t->gd, startx, starty, tmpstr, MAXBUF);

                    left -= MAXBUF;
                    startx += MAXBUF * fontx;
                }

                memset(tmpstr, '*', left - 1);
                if(t->keybd_focus)
                    tmpstr[left - 1] = strshow[showlen - 1];
                else
                    tmpstr[left - 1] = '*';
                show_text(t->gd, startx, starty, tmpstr, left);
            }
            else
            {
                /**
                 * enter cannot be shown!
                 **/
                if(showlen > 0 && strshow[showlen - 1] == '\n')
                    show_text(t->gd, startx, starty, strshow, showlen - 1);
                else
                    show_text(t->gd, startx, starty, strshow, showlen);

            }
            ++ i;
            strshow = next;
            starty += fonty;
        }
    }
    /**
     * else if placeholder need to be displayed
     **/
    else if(t->placeholder != NULL && !t->keybd_focus)
    {
	/* FIXME */
        set_color(t->gd, t->frame_color.r, t->frame_color.g, t->frame_color.b, t->frame_color.a);
        show_text(t->gd, startx, starty, t->placeholder, strlen(t->placeholder) + 1);
        return 0;
    }
    /**
     * no input; just a ruler
     **/
    else
    {
        rulerx = startx;
        rulery1 = starty;
        rulery2 = starty + fonty;
    }

    /**
     * show ruler
     **/
    if(t->keybd_focus)
    {
        set_color(t->gd, t->fore_color.r, t->fore_color.g, t->fore_color.b, t->fore_color.a);
        draw_line(t->gd, rulerx, rulery1, rulerx, rulery2);
    }

    return 0;
}

static si_t text_line_draw_frame(si_t gd, si_t x, si_t y, si_t width, si_t height, si_t margin)
{
    si_t n, m;
    /**
     * up line
     **/
    draw_line(gd, 
        x + margin,
        y,
        x + width - margin - 1,
        y);
    /**
     * down line
     **/
    draw_line(gd, 
        x + margin,
        y + height - 1,
        x + width - margin - 1,
        y + height - 1);
    /**
     * left line
     **/
    draw_line(gd, 
        x,
        y + margin,
        x,
        y + height - margin - 1);
    /**
     * right line
     **/
    draw_line(gd, 
        x + width - 1,
        y + margin,
        x + width - 1,
        y + height - margin - 1);

    n = margin;
    while(-- n)
    {
        m = margin - n;
        draw_point(gd, x + n, y + m); 
        draw_point(gd, x + width - n - 1, y + m); 
        draw_point(gd, x + n, y + height - 1 - m); 
        draw_point(gd, x + width - 1 - n, y + height - 1 - m); 
    }

    return 0;
}

static si_t text_line_default_widget_repaint(struct text_line* t, union message* msg)
{
    struct rectangle area;
    si_t x, y;

    /* 获得左上角的绝对坐标 */
    widget_absolute_coordinate(WIDGET_POINTER(t), &x, &y);

    /* 获得绝对的工作区域 */
    /* 将会舍弃不在父控件内的部分*/
    widget_absolute_area(WIDGET_POINTER(t), &area);

    set_area
        (t->gd,
         area.x,
         area.y,
         area.width,
         area.height);

    /**
     * backgroud
     **/
    set_color(t->gd,
        t->back_color.r,
        t->back_color.g,
        t->back_color.b,
        t->back_color.a);
    fill_rectangle(t->gd,
        area.x + 4,
        area.y + 4,
        area.width - 8,
        area.height - 8);

    /**
     * outter frame: black if keyboard focus; grey if not
     **/
    if(t->keybd_focus)
    {
	/* FIXME */
        set_color
            (t->gd,
             t->frame_color.r,
             t->frame_color.g,
             t->frame_color.b,
             t->frame_color.a);
        text_line_draw_frame
            (t->gd,
             area.x,
             area.y,
             area.width,
             area.height,
             3);
        text_line_draw_frame
            (t->gd,
             area.x + 1,
             area.y + 1,
             area.width - 2,
             area.height - 2,
             2);
    }
    else
    {
	/* FIXME */
        set_color
            (t->gd,
             t->frame_color.r,
             t->frame_color.g,
             t->frame_color.b,
             t->frame_color.a);
        text_line_draw_frame
            (t->gd,
             area.x,
             area.y,
             area.width,
             area.height,
             3);
        text_line_draw_frame
            (t->gd,
             area.x + 1,
             area.y + 1,
             area.width - 2,
             area.height - 2,
             2);
    }

    /**
     * inner frame
     **/
    set_color
        (t->gd,
         t->frame_color.r,
         t->frame_color.g,
         t->frame_color.b,
         t->frame_color.a);
    text_line_draw_frame
        (t->gd,
         area.x + 2,
         area.y + 2,
         area.width - 4,
         area.height - 4,
         1);
    text_line_draw_frame
        (t->gd,
         area.x + 3,
         area.y + 3,
         area.width - 6,
         area.height - 6,
         1);
    text_line_draw_frame
        (t->gd,
         area.x + 4,
         area.y + 4,
         area.width - 8,
         area.height - 8,
         1);

    set_area
        (t->gd,
         area.x + 6,
         area.y + 6,
         area.width - 12,
         area.height - 12);


    /**
     * show text
     **/
    text_line_default_show_text(t, area.x, area.y);

    return 0;
}


/**
 * return -1 if out of range or do not have keyboard focus
 **/
static si_t text_line_default_keybd_press(struct text_line* t, union message* msg)
{
    si_t buflen;
    char input_char=0;
    si_t retval;

    /**
     * do not have keyboard focus: return -1
     **/
    if(!t->keybd_focus)
    {
        return -1;
    }

    buflen = strlen(t->buf);

    if((retval = text_line_handle_special_key(t, msg->keybd.code)) != 1)
        return retval;

    /**
     * decode input
     **/
    if(!input_char && text_line_is_keybd_type_number(t))
    {
        input_char = keybd_code_to_char_number(msg->keybd.code);
    }
    if(!input_char && text_line_is_keybd_type_letter(t))
    {
        input_char = keybd_code_to_char_letter(msg->keybd.code);
    }
    if(input_char == 0)
    {
        return 0;
    }


    /**
     * in case buffer overflow
     **/
    if(buflen + 1 > t->buf_size - 1)
    {
        return -1;
    }

    /**
     * insert input
     **/
    if(t->buf + buflen > t->ruler_cur)
    {
        char* cur = t->buf + buflen;
        /**
         * move back
         **/
        while(-- cur != t->ruler_cur - 1)
        {
            *(cur + 1) = *cur;
        }
        *(t->ruler_cur) = input_char;
        ++ t->ruler_cur;
    }
    else
    {
        /**
         * update buf
         **/
        *(t->ruler_cur) = input_char;
        ++ t->ruler_cur;
        *t->ruler_cur = '\0';
    }

    /**
     * dispatch cursor_move event to subscriber
     **/
    event_to_be_dispatched = TEXT_LINE_EVENT_CURRENT_CHANGE;
    list_for_each(&t->subscribe_info_list, do_handle_event);

    return 0;
}


static si_t text_line_default_mouse_press(struct text_line* t, union message* msg)
{
    /**
     * TODO
     **/
    return 0;
}

static si_t text_line_default_mouse_release(struct text_line* t, union message* msg)
{
    /**
     * TODO
     **/
    return 0;
}

static si_t text_line_default_keybd_leave(struct text_line* t, union message* msg)
{
    /**
     * TODO
     **/
    return 0;
}

extern si_t text_line_default_callback(void* t, void* m)
{
    union message* msg = (union message*)m;
    struct text_line* tl = (struct text_line*)t;

    switch(msg->base.type)
    {
    case MESSAGE_TYPE_WIDGET_REPAINT:
        text_line_default_widget_repaint(tl, msg);
        text_line_default_widget_show(tl, msg);
        break;

    case MESSAGE_TYPE_WIDGET_SHOW:
        text_line_default_widget_show(tl, msg);
        break;

    case MESSAGE_TYPE_MOUSE_PRESS:
        text_line_default_mouse_press(tl, msg);
        text_line_default_widget_repaint(tl, msg);
        text_line_default_widget_show(tl, msg);
        break;

    case MESSAGE_TYPE_MOUSE_RELEASE:
        text_line_default_mouse_release(tl, msg);
        text_line_default_widget_repaint(tl, msg);
        text_line_default_widget_show(tl, msg);
        break;

    case MESSAGE_TYPE_KEYBD_PRESS:
        text_line_default_keybd_press(tl, msg);
        text_line_default_widget_repaint(tl, msg);
        text_line_default_widget_show(tl, msg);
        break;

    case MESSAGE_TYPE_KEYBD_LEAVE:
        text_line_default_keybd_leave(tl, msg);
        text_line_default_widget_repaint(tl, msg);
        text_line_default_widget_show(tl, msg);

    default:
        break;
    }
    return 0;
}

void text_line_repaint(struct text_line* t)
{
	widget_repaint(WIDGET_POINTER(t));
}

void text_line_show(struct text_line* t)
{
	widget_show(WIDGET_POINTER(t));
}

struct text_line* text_line_init(si_t bufsize, si_t cur_line)
{
    struct text_line* addr;

    /* 分配存储空间 */
    if((addr = (struct text_line*)malloc(sizeof(struct text_line))) == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to malloc");
        return NULL;
    }

    if(!(addr=widget_init_common(WIDGET_POINTER(addr), 0))) {
        return NULL;
    }

    addr->name = "struct text_line";

    /* 默认是否能处理键盘输入消息 */
    addr->input_enable = 1;

    /* 用全局样式对象初始化text_line样式 */
    text_line_init_with_default_style(addr);

    /* 默认的回调函数 */
    addr->callback = text_line_default_callback;

    /**
     * for text_line
     **/
    /**
     * default set buf to null
     **/
    addr->buf = NULL;
    addr->buf_size = 0;

    /**
     * default keyboad type
     **/
    addr->keybd_type = TEXT_LINE_KEYBOARD_TYPE_NUMBER | TEXT_LINE_KEYBOARD_TYPE_LETTER;

    /**
     * default pointer to NULL
     **/
    addr->ruler_cur = NULL;
    addr->ruler_line_start = NULL;
    addr->show_start = NULL;

    /**
     * default single line
     **/
    addr->is_linewrap = 0;
    addr->is_enter_enable = 0;

    addr->placeholder = NULL;

    /**
     * default handler
     **/
    list_init(&addr->subscribe_info_list);

	text_line_set_bufsize(addr, bufsize);

	text_line_update_ruler_line(addr, cur_line);

    return addr;
}

extern si_t text_line_exit(struct text_line* t)
{
    if(t->buf != NULL)
        free(t->buf);

    if(t->placeholder != NULL)
        free(t->placeholder);

    return widget_exit(WIDGET_POINTER(t));
}

extern si_t text_line_set_bufsize(struct text_line* t, si_t size)
{
    if(t->buf != NULL)
        free(t->buf);

    /* 分配存储空间 */
    /**
     * plus one: use as tag
     **/
    if((t->buf = (char*)malloc(size + 1)) == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to malloc");
        return -1;
    }

    * (t->buf) = '\0';
    t->buf_size = size;

    t->ruler_cur = t->ruler_line_start = t->show_start = t->buf;

    return 0;
}

extern void text_line_set_wrap(struct text_line* t)
{
    t->is_linewrap = 1;
}

extern void text_line_set_nowrap(struct text_line* t)
{
    t->is_linewrap = 0;
}

extern void text_line_set_enter_enable(struct text_line* t)
{
    t->is_enter_enable = 1;
}

extern void text_line_set_enter_disable(struct text_line* t)
{
    t->is_enter_enable = 0;
}

extern void text_line_set_multilines(struct text_line* t)
{
    t->is_linewrap = 1;
    t->is_enter_enable = 1;
}

extern si_t text_line_set_placeholder(struct text_line* t, char* str)
{
    if(t->placeholder != NULL)
        free(t->placeholder);

    /* 分配存储空间 */
    if((t->placeholder = (char*)malloc(strlen(str) + 1)) == NULL)
    {
        EGUI_PRINT_SYS_ERROR("fail to malloc");
        return -1;
    }
    memcpy(t->placeholder, str, strlen(str) + 1);

    return 0;
}

extern char* text_line_get_buf(struct text_line* t)
{
    return t->buf;
}

extern void text_line_set_keybd_type(struct text_line* t, si_t type)
{
    t->keybd_type |= type;
}

extern void text_line_cancel_keybd_type(struct text_line* t, si_t type)
{
    t->keybd_type &= ~type;
}

extern void text_line_clear_keybd_type(struct text_line* t)
{
    t->keybd_type = 0;
}

extern si_t text_line_is_keybd_type_number(struct text_line* t)
{
    return t->keybd_type & TEXT_LINE_KEYBOARD_TYPE_NUMBER;
}

extern si_t text_line_is_keybd_type_letter(struct text_line* t)
{
    return t->keybd_type & TEXT_LINE_KEYBOARD_TYPE_LETTER;
}

extern si_t text_line_is_keybd_type_password(struct text_line* t)
{
    return t->keybd_type & TEXT_LINE_KEYBOARD_TYPE_PASSWORD;
}

extern void text_line_register_move_handler(struct text_line* t, struct widget* w, si_t event, widget_event_handler handler)
{
    struct widget_subscribe_info si;
    si.subscriber = w;
    si.publisher = WIDGET_POINTER(t);
    si.event = event;
    si.handler = handler;
    list_push_back(&t->subscribe_info_list, &si, sizeof(struct widget_subscribe_info));
}

/**
 * jump to specific index
 **/
extern si_t text_line_update_ruler(struct text_line* t, char* new_ruler)
{
    char* start;
    char* next_start;
    if(t->buf + strlen(t->buf) <= new_ruler)
        return -1;

    start = t->buf;
    while((next_start = get_next_line_start(start, t->chars_per_line, t->is_enter_enable, t->is_linewrap)) != NULL)
    {
        if(next_start > new_ruler)
        {
            t->ruler_cur = next_start;
            t->show_start = start;
            return 0;
        }
        start = next_start;
    }

    return -1;
}

/**
 * jump to specific line
 **/
extern si_t text_line_update_ruler_line(struct text_line* t, si_t line)
{
    si_t i = 0;
    char* start;

    /**
     * find the start of the specific line
     **/
    start = t->buf;
    while(i != line)
    {
        if((start = get_next_line_start(start, t->chars_per_line, t->is_enter_enable, t->is_linewrap)) == NULL)
            return -1;
        ++ i;
    }

    /**
     * update
     **/
    t->show_start = start;
    t->ruler_cur = start;

    return 0;
}

extern si_t text_line_get_line_cur(struct text_line* t)
{
    si_t fontx = get_font_width(t->gd);
    t->chars_per_line = (t->area.width - 12) / fontx;
    return lines_between(t->buf, t->ruler_cur, t->chars_per_line, t->is_enter_enable, t->is_linewrap);
}

extern si_t text_line_get_line_total(struct text_line* t)
{
    si_t fontx = get_font_width(t->gd);
    t->chars_per_line = (t->area.width - 12) / fontx;
    return lines_between(t->buf, t->buf + strlen(t->buf), t->chars_per_line, t->is_enter_enable, t->is_linewrap);
}

extern si_t text_line_get_max_line_shown(struct text_line* t)
{
    si_t fontx, fonty, max_line;

    /**
     * make sure the font size fits the area
     * the magic number 12 is:
     * 4 for outter frame
     * + 6 for inner frame
     * + 2 for margin between text and the inner frame
     **/
    ++ t->font;
    do{
        -- t->font;
        set_font(t->gd, t->font);
        fontx = get_font_width(t->gd);
        fonty = get_font_height(t->gd);
    }while(fonty > (t->area.height - 12) || fontx > t->area.width - 12);
    max_line = (t->area.height - 12) / fonty;
    t->chars_per_line = (t->area.width - 12) / fontx;

    return max_line;
}

void text_line_set_bounds(struct text_line* t, si_t x, si_t y, si_t width , si_t height)
{
    widget_set_bounds(WIDGET_POINTER(t), x, y, width, height);
}

void text_line_set_color(struct text_line* t, struct color* fore_color, struct color* back_color, struct color* frame_color)
{
	widget_set_color(WIDGET_POINTER(t), fore_color, back_color);
	t->frame_color = (NULL == frame_color ? t->frame_color : *frame_color);
}

void text_line_set_font(struct text_line* t, si_t font)
{
	t->font = font;
}
