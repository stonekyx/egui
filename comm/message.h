/**
 * @file message.h
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

# ifndef _MESSAGE_H_
# define _MESSAGE_H_ 1

# include <sys/time.h>

# include <base_type.h>
# include <geometry.h>
# include <limits.h>

# define MESSAGE_BASE_DEFINITION \
    \
    /** 消息的种类，它唯一标识了一种消息类型 **/ \
    si_t type; \
    \
    /** 消息要发送到的那个窗口描述符 **/ \
    si_t window_descriptor; \
    \
    /** 消息放入消息队列时的鼠标坐标 **/ \
    struct point cursor_position; \
    \
    /** 消息的时间 **/ \
    struct timeval time;

/**
 * 消息种类
**/
enum __message_type__
{
    /** 获得键盘焦点 **/
    MESSAGE_TYPE_KEYBD_ENTER = 0,
    /** 释放键盘焦点 **/
    MESSAGE_TYPE_KEYBD_LEAVE,
    /** 键盘按下 **/
    MESSAGE_TYPE_KEYBD_PRESS,
    /** 键盘释放 **/
    MESSAGE_TYPE_KEYBD_RELEASE,
    /** 长时间按下键盘的键 **/
    MESSAGE_TYPE_KEYBD_CLICK,

    /** 鼠标移动 **/
    MESSAGE_TYPE_MOUSE_MOVE,
    /** 鼠标绝对坐标移动（与上面的区别类似 結標淡希 和 白井黒子） **/
    MESSAGE_TYPE_MOUSE_MOVE_POINT,
    /** 鼠标进入一个区域 **/
    MESSAGE_TYPE_MOUSE_ENTER,
    /** 鼠标离开一个区域 **/
    MESSAGE_TYPE_MOUSE_LEAVE,
    /** 鼠标的键按下 **/
    MESSAGE_TYPE_MOUSE_PRESS,
    /** 鼠标的键释放 **/
    MESSAGE_TYPE_MOUSE_RELEASE,
    /** 鼠标单击 **/
    MESSAGE_TYPE_MOUSE_SINGLE_CLICK,
    /** 鼠标双击 **/
    MESSAGE_TYPE_MOUSE_DOUBLE_CLICK,

    /** 窗口绘制消息 **/
    MESSAGE_TYPE_WIDGET_REPAINT,
    /** 窗口部件显示 **/
    MESSAGE_TYPE_WIDGET_SHOW,
    /** 窗口部件移动 **/
    MESSAGE_TYPE_WIDGET_MOVE,
    /** 窗口部件改变大小 **/
    MESSAGE_TYPE_WIDGET_RESIZE,

    /** 窗口激活 **/
    MESSAGE_TYPE_WINDOW_ACTIVATE,
    /** 窗口激死 **/
    MESSAGE_TYPE_WINDOW_DEACTIVATE,
    /** MINIMIZED, MAXIMIZED, FULLSIZED 改变 **/
    MESSAGE_TYPE_WINDOW_STATE_CHANGE,
    /** 关闭窗口 **/
    MESSAGE_TYPE_WINDOW_CLOSE,
    /** 最大化窗口 **/
    MESSAGE_TYPE_WINDOW_MAXIMIZE,
    /** 最小化窗口 **/
    MESSAGE_TYPE_WINDOW_MINIMIZE,
    /** 还原窗口 **/
    MESSAGE_TYPE_WINDOW_RESTORE,
	/* 注册窗口：仅适用于桌面 **/
	MESSAGE_TYPE_WINDOW_REGISTER,
	/* 取消窗口：仅适用于桌面 **/
	MESSAGE_TYPE_WINDOW_CANCEL,

	/* 程序推出 */
	MESSAGE_TYPE_APPLICATION_QUIT,

    /** 自定义 **/
    MESSAGE_TYPE_CUSTOM,

    /** 消息种类的数目 **/
    MESSAGE_TYPE_QUANTITY,

    /** 消息种类的最大数目 **/
    /** ISO C restricts enumerator values to range of 'int' **/
    MESSAGE_TYPE_MAX = INT_MAX
};

/**
 * 把message_type转换成字符串便于打印日志信息
 **/
extern char* message_type_to_str(si_t message_type);

/**
 * 前置声明。。。
 **/
union message;

struct message_base
{
    MESSAGE_BASE_DEFINITION
};
extern void message_set_window_descriptor(union message* m, si_t w);
extern si_t message_get_window_descriptor(const union message* m);
extern void message_set_type(union message* m, si_t t);
extern si_t message_get_type(const union message* m);
extern void message_set_cursor_position(union message* m, struct point* c);
extern const struct point* message_get_cursor_position(const union message* m);
extern void message_set_time(union message* m, struct timeval* t);
extern const struct timeval* message_get_time(const union message* m);

/**
 * 键盘输入事件编码
**/
enum __keybd_input_code__
{
    /** Esc **/
    INPUT_CODE_KEYBD_ESC = 0,
    /** 1! **/
    INPUT_CODE_KEYBD_1,
    /** 2@ **/
    INPUT_CODE_KEYBD_2,
    /** 3# **/
    INPUT_CODE_KEYBD_3,
    /** 4$ **/
    INPUT_CODE_KEYBD_4,
    /** 5% **/
    INPUT_CODE_KEYBD_5,
    /** 6^ **/
    INPUT_CODE_KEYBD_6,
    /** 7& **/
    INPUT_CODE_KEYBD_7,
    /** 8* **/
    INPUT_CODE_KEYBD_8,
    /** 9( **/
    INPUT_CODE_KEYBD_9,
    /** 0) **/
    INPUT_CODE_KEYBD_0,
    /** -_ **/
    INPUT_CODE_KEYBD_SUB,
    /** =+ **/
    INPUT_CODE_KEYBD_EQUAL,
    /** BackSpace **/
    INPUT_CODE_KEYBD_BACKSPACE,
    /** Tab **/
    INPUT_CODE_KEYBD_TAB,
    /** qQ **/
    INPUT_CODE_KEYBD_Q,
    /** wW **/
    INPUT_CODE_KEYBD_W,
    /** eE **/
    INPUT_CODE_KEYBD_E,
    /** rR **/
    INPUT_CODE_KEYBD_R,
    /** tT **/
    INPUT_CODE_KEYBD_T,
    /** yY **/
    INPUT_CODE_KEYBD_Y,
    /** uU **/
    INPUT_CODE_KEYBD_U,
    /** iI **/
    INPUT_CODE_KEYBD_I,
    /** oO **/
    INPUT_CODE_KEYBD_O,
    /** pP **/
    INPUT_CODE_KEYBD_P,
    /** [{ **/
    INPUT_CODE_KEYBD_L_BRACE,
    /** ]} **/
    INPUT_CODE_KEYBD_R_BRACE,
    /** Enter **/
    INPUT_CODE_KEYBD_ENTER,
    /** L Ctrl **/
    INPUT_CODE_KEYBD_L_CTRL,
    /** aA **/
    INPUT_CODE_KEYBD_A,
    /** sS **/
    INPUT_CODE_KEYBD_S,
    /** dD **/
    INPUT_CODE_KEYBD_D,
    /** fF **/
    INPUT_CODE_KEYBD_F,
    /** gG **/
    INPUT_CODE_KEYBD_G,
    /** hH **/
    INPUT_CODE_KEYBD_H,
    /** jJ **/
    INPUT_CODE_KEYBD_J,
    /** hH **/
    INPUT_CODE_KEYBD_K,
    /** lL **/
    INPUT_CODE_KEYBD_L,
    /** ;: **/
    INPUT_CODE_KEYBD_SEMICOLON,
    /**  **/
    INPUT_CODE_KEYBD_APOSTROPHE,
    /**  **/
    INPUT_CODE_KEYBD_GRAVE,
    /** L Shift **/
    INPUT_CODE_KEYBD_L_SHIFT,
    /** / **/
    INPUT_CODE_KEYBD_BACKSLASH,
    /** zZ **/
    INPUT_CODE_KEYBD_Z,
    /** xX **/
    INPUT_CODE_KEYBD_X,
    /** cC **/
    INPUT_CODE_KEYBD_C,
    /** vV **/
    INPUT_CODE_KEYBD_V,
    /** bB **/
    INPUT_CODE_KEYBD_B,
    /** nN **/
    INPUT_CODE_KEYBD_N,
    /** mM **/
    INPUT_CODE_KEYBD_M,
    /** ,< **/
    INPUT_CODE_KEYBD_COMMA,
    /** .> **/
    INPUT_CODE_KEYBD_DOT,
    /**  **/
    INPUT_CODE_KEYBD_SLASH,
    /** R Shift **/
    INPUT_CODE_KEYBD_R_SHIFT,
    /**  **/
    INPUT_CODE_KEYBD_KPASTERISK,
    /** L Alt **/
    INPUT_CODE_KEYBD_L_ALT,
    /**   **/
    INPUT_CODE_KEYBD_SPACE,
    /** Caps Lock **/
    INPUT_CODE_KEYBD_CAPSLOCK,
    /** F1 **/
    INPUT_CODE_KEYBD_F1,
    /** F2 **/
    INPUT_CODE_KEYBD_F2,
    /** F3 **/
    INPUT_CODE_KEYBD_F3,
    /** F4 **/
    INPUT_CODE_KEYBD_F4,
    /** F5 **/
    INPUT_CODE_KEYBD_F5,
    /** F6 **/
    INPUT_CODE_KEYBD_F6,
    /** F7 **/
    INPUT_CODE_KEYBD_F7,
    /** F8 **/
    INPUT_CODE_KEYBD_F8,
    /** F9 **/
    INPUT_CODE_KEYBD_F9,
    /** F10 **/
    INPUT_CODE_KEYBD_F10,
    /** F11 **/
    INPUT_CODE_KEYBD_F11,
    /** F12 **/
    INPUT_CODE_KEYBD_F12,
    /** Num Lock **/
    INPUT_CODE_KEYBD_NUMLOCK,
    /** Scroll Lock **/
    INPUT_CODE_KEYBD_SCROLLLOCK,
    /** 7Home **/
    INPUT_CODE_KEYBD_KP_7,
    /** 8↑ **/
    INPUT_CODE_KEYBD_KP_8,
    /** 9Pg Up **/
    INPUT_CODE_KEYBD_KP_9,
    /** - **/
    INPUT_CODE_KEYBD_KP_SUB,
    /** 4← **/
    INPUT_CODE_KEYBD_KP_4,
    /** 5 **/
    INPUT_CODE_KEYBD_KP_5,
    /** 6→ **/
    INPUT_CODE_KEYBD_KP_6,
    /** + **/
    INPUT_CODE_KEYBD_KP_ADD,
    /** 1 **/
    INPUT_CODE_KEYBD_KP_1,
    /** 2↓ **/
    INPUT_CODE_KEYBD_KP_2,
    /** 3 **/
    INPUT_CODE_KEYBD_KP_3,
    /** 0Ins **/
    INPUT_CODE_KEYBD_KP_0,
    /** .Del **/
    INPUT_CODE_KEYBD_KP_DOT,
    /** Enter **/
    INPUT_CODE_KEYBD_KP_ENTER,
    /** R Ctrl **/
    INPUT_CODE_KEYBD_R_CTRL,
    /** / **/
    INPUT_CODE_KEYBD_KP_SLASH,
    /** Print Screen SysRq **/
    INPUT_CODE_KEYBD_SYSRQ,
    /** R Alt **/
    INPUT_CODE_KEYBD_R_ALT,
    /** Home **/
    INPUT_CODE_KEYBD_HOME,
    /** ↑ **/
    INPUT_CODE_KEYBD_UP,
    /** Page Up **/
    INPUT_CODE_KEYBD_PAGEUP,
    /** ← **/
    INPUT_CODE_KEYBD_LEFT,
    /** → **/
    INPUT_CODE_KEYBD_RIGHT,
    /** End **/
    INPUT_CODE_KEYBD_END,
    /** ↓ **/
    INPUT_CODE_KEYBD_DOWN,
    /** Page Down **/
    INPUT_CODE_KEYBD_PAGEDOWN,
    /** Insert **/
    INPUT_CODE_KEYBD_INSERT,
    /** Delete **/
    INPUT_CODE_KEYBD_DELETE,
    /** Pause Break **/
    INPUT_CODE_KEYBD_PAUSE,
    /** L Meta **/
    INPUT_CODE_KEYBD_L_META,
    /** R Meta **/
    INPUT_CODE_KEYBD_R_META,

    INPUT_CODE_KEYBD_COMPOSE,

    INPUT_CODE_KEYBD_QUANTITY,

    /** ISO C restricts enumerator values to range of 'int' **/
    INPUT_CODE_KEYBD_MAX = INT_MAX
};

/**
 * 当键盘的键被按下时，产生该事件。
 * 当键盘的键被松开时，产生该事件。
**/
struct message_keybd
{
    MESSAGE_BASE_DEFINITION

    /** 哪个键 **/
    si_t code;

    /** 按下还是松开 **/
    si_t value;

    /**
     * key and button mask
    **/
    ui_t mask;
};

extern void message_set_keybd(union message* msg, si_t t, si_t c, si_t v, si_t m);
extern si_t message_get_keybd_code(const union message* m);
extern si_t message_get_keybd_value(const union message* m);
extern si_t message_get_keybd_mask(const union message* m);

/**
 * keyboard lose focus
 **/
struct message_keyboard_leave
{
    MESSAGE_BASE_DEFINITION
};

extern void message_set_keyboard_leave(union message* m);

/**
 * 鼠标输入事件编码
**/
enum __mouse_input_code__
{
    /** L KEY **/
    INPUT_CODE_MOUSE_L_KEY = 0,
    /** R KEY **/
    INPUT_CODE_MOUSE_R_KEY,
    /** M KEY **/
    INPUT_CODE_MOUSE_M_KEY,
    /** X OOFSET **/
    INPUT_CODE_MOUSE_X_OFFSET,
    /** Y OFFSET **/
    INPUT_CODE_MOUSE_Y_OFFSET,
    /** Z OFFSET **/
    INPUT_CODE_MOUSE_Z_OFFSET,

    INPUT_CODE_MOUSE_QUANTITY,

    /** ISO C restricts enumerator values to range of 'int' **/
    INPUT_CODE_MOUSE_MAX = INT_MAX
};

/**
 * 修饰符编码
**/
enum __input_modifier__
{
    /** 按下 Num Lock **/
    INPUT_MODIFIER_KEYBD_NUMLOCK,
    /** 按下 Caps Lock **/
    INPUT_MODIFIER_KEYBD_CAPSLOCK,

    /** 按下 L Ctrl **/
    INPUT_MODIFIER_KEYBD_L_CTRL,

    /** 按下 R Ctl **/
    INPUT_MODIFIER_KEYBD_R_CTRL,

    /** 按下 L Shift **/
    INPUT_MODIFIER_KEYBD_L_SHIFT,

    /** 按下 R Shift **/
    INPUT_MODIFIER_KEYBD_R_SHIFT,

    /** 按下 L Alt **/
    INPUT_MODIFIER_KEYBD_L_ALT,

    /** 按下 R Alt **/
    INPUT_MODIFIER_KEYBD_R_ALT,

    /** 按下 L Meta **/
    INPUT_MODIFIER_KEYBD_L_META,

    /** 按下 R Meta **/
    INPUT_MODIFIER_KEYBD_R_META,

    /** 按下 L Key **/
    INPUT_MODIFIER_MOUSE_L_KEY,

    /** 按下 M Key **/
    INPUT_MODIFIER_MOUSE_M_KEY,

    /** 按下 R Key **/
    INPUT_MODIFIER_MOUSE_R_KEY,

    INPUT_MODIFIER_QUANTITY,

    /** ISO C restricts enumerator values to range of 'int' **/
    INPUT_MODIFIER_MAX = INT_MAX
};

enum __input_modifier_mask__
{
    INPUT_MODIFIER_MASK_KEYBD_NUMLOCK  = ((ui_t)1) << 0,

    INPUT_MODIFIER_MASK_KEYBD_CAPSLOCK = ((ui_t)1) << 1,

    INPUT_MODIFIER_MASK_KEYBD_L_CTRL   = ((ui_t)1) << 2,

    INPUT_MODIFIER_MASK_KEYBD_R_CTRL   = ((ui_t)1) << 3,

    INPUT_MODIFIER_MASK_KEYBD_L_SHIFT  = ((ui_t)1) << 4,

    INPUT_MODIFIER_MASK_KEYBD_R_SHIFT  = ((ui_t)1) << 5,

    INPUT_MODIFIER_MASK_KEYBD_L_ALT    = ((ui_t)1) << 6,

    INPUT_MODIFIER_MASK_KEYBD_R_ALT    = ((ui_t)1) << 7,

    INPUT_MODIFIER_MASK_KEYBD_L_META   = ((ui_t)1) << 8,

    INPUT_MODIFIER_MASK_KEYBD_R_META   = ((ui_t)1) << 9,

    INPUT_MODIFIER_MASK_MOUSE_L_KEY    = ((ui_t)1) << 10,

    INPUT_MODIFIER_MASK_MOUSE_M_KEY    = ((ui_t)1) << 11,

    INPUT_MODIFIER_MASK_MOUSE_R_KEY    = ((ui_t)1) << 12
};

/**
 * 设置某一位
**/
# define INPUT_MODIFIER_BIT_SET(bit, bitset) ((* (bitset)) = (bit) | (* (bitset)))

/**
 * 清理某一位
**/
# define INPUT_MODIFIER_BIT_CLR(bit, bitset) ((* (bitset)) = (~(bit)) & (* (bitset)))

/**
 * 设置所有位
**/
# define INPUT_MODIFIER_BIT_SET_ALL(bitset) ((* (bitset)) = ((ui_t)0) & (* (bitset)))

/**
 * 清理所有位
**/
# define INPUT_MODIFIER_BIT_CLR_ALL(bitset) ((* (bitset)) = ((ui_t)-1) | (* (bitset)))

/**
 * 当鼠标的键被按下时，产生该事件。
 * 当鼠标的键被松开时，产生该事件。
 * 移动鼠标时，产生该事件。
 *
 * 不断的鼠标移动产生不断的 message_mouse_move 。
 * 难道鼠标每移动一点就产生一个 message_mouse_move ?
 * 当然不是。
 * 窗口管理程序并不包证多远的距离产生一个 message_mouse_move，
 * 但包证至少在移动的起始点和终止点各会产生一个 message_mouse_move。
**/
struct message_mouse
{
    MESSAGE_BASE_DEFINITION

    /** 哪个按钮或者哪个方向 **/
    si_t code;

    /** 偏移量 **/
    si_t value;

    /**
     * key and button mask
    **/
    ui_t mask;
};

extern void message_set_mouse(union message* msg, si_t t, si_t c, si_t v, si_t m);
extern si_t message_get_mouse_code(const union message* m);
extern si_t message_get_mouse_value(const union message* m);
extern si_t message_get_mouse_mask(const union message* m);

/**
 * 显示窗口
 **/
struct message_widget_show
{
    MESSAGE_BASE_DEFINITION
};

extern void message_set_widget_show(union message* m, si_t w);

/**
 * 重绘窗口
 **/
struct message_widget_repaint
{
    MESSAGE_BASE_DEFINITION
	/** 重绘区域 **/
    struct rectangle area;
};

extern void message_set_widget_repaint(union message* m, si_t w, struct rectangle* r);
extern const struct rectangle* message_get_widget_repaint_area(const union message* m);

/**
 * 移动窗口
 **/
struct message_widget_move
{
    MESSAGE_BASE_DEFINITION
	/** 移动新位置 **/
    struct point position;
};

extern void message_set_widget_move(union message* m, si_t w, struct point* p);
extern const struct point* message_get_widget_move_poisition(const union message* m);

/**
 * 调整窗口大小
 **/
struct message_widget_resize
{
    MESSAGE_BASE_DEFINITION

    struct rectangle area;
};

extern void message_set_widget_resize(union message* m, si_t w, struct rectangle* area);
extern const struct rectangle* message_get_widget_resize_area(const union message* m);

/**
 * 激活窗口
**/
struct message_window_activate
{
    MESSAGE_BASE_DEFINITION
};

extern void message_set_window_activate(union message* m, si_t w);

/**
 * 激死窗口
**/
struct message_window_deactivate
{
    MESSAGE_BASE_DEFINITION
};

extern void message_set_window_deactivate(union message* m, si_t w);

/**
 * 最小化窗口
 *
 * 按下最小化按钮发送这个消息
**/
struct message_window_minimize
{
    MESSAGE_BASE_DEFINITION
};

extern void message_set_window_minimize(union message* m, si_t w);

/**
 * 最大化窗口
 *
 * 按下最大化按钮发送这个消息
**/
struct message_window_maximize
{
    MESSAGE_BASE_DEFINITION
};

extern void message_set_window_maximize(union message* m, si_t w);

/**
 * 还原窗口
 *
 * 还原窗口发送这个消息
**/
struct message_window_restore
{
    MESSAGE_BASE_DEFINITION
};

extern void message_set_window_restore(union message* m, si_t w);

/**
 * 关闭窗口
 *
 * 按下关闭按钮发送这个消息
**/
struct message_window_close
{
    MESSAGE_BASE_DEFINITION
};

extern void message_set_window_close(union message* m, si_t w);

/**
 * 注册窗口
 *
 * 当一个窗口在服务器端被注册之后，服务器将这条消息发送给桌面程序
**/
struct message_window_register
{
    MESSAGE_BASE_DEFINITION
};

extern void message_set_window_register(union message* m, si_t w);

/**
 * 取消窗口
 *
 * 当一个窗口在服务器端被取消之后，服务器将这条消息发送给桌面程序
**/
struct message_window_cancel
{
    MESSAGE_BASE_DEFINITION
};

extern void message_set_window_cancel(union message* m, si_t w);

union message
{
    struct message_base              base;
    struct message_keybd             keybd;
    struct message_mouse             mouse;
    struct message_widget_show       widget_show;
    struct message_widget_repaint    widget_repaint;
    struct message_widget_move       widget_move;
    struct message_widget_resize     widget_resize;
    struct message_window_activate   window_activate;
    struct message_window_deactivate window_deactivate;
    struct message_window_minimize   window_minimize;
    struct message_window_maximize   window_maximize;
    struct message_window_restore    window_restore;
    struct message_window_close      window_close;
	struct message_window_register	 window_register;
	struct message_window_cancel	 window_cancel;
    struct message_keyboard_leave    keybd_leave;
};


# endif
