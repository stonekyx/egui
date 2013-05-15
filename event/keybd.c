# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <linux/input.h>

# include "event.h"
# include "log.h"

static si_t keybd_exit(struct input_device * self);
static si_t deal_with_keybd(struct input_device * self, struct list* msg_list);

si_t keybd_init(struct input_device * self, char * keybd_device_path)
{
    si_t fd;

    fd = open(keybd_device_path, O_RDONLY);

    if(fd < 0)
    {
		EGUI_PRINT_SYS_ERROR("failed to open keybd device: open()");
        return -1;
    }

    /* TODO:是否检查指针合法性？ */
    uds_init_from_naked_socket(&self->uds, fd);
	self->deal_with_input = deal_with_keybd;
	self->input_exit = keybd_exit;

    return 0;
}

static si_t deal_with_keybd(struct input_device * self, struct list* msg_list)
{
	struct input_event ie;
	union message msg;

	/* 读取数据 */
	if(uds_read(&self->uds, &ie, sizeof(struct input_event)) < 0)
	{
		EGUI_PRINT_ERROR("failed to read from keybd input device.");
		return -1;
	}

	/* 时间 */
	msg.mouse.time.tv_sec = ie.time.tv_sec;
	msg.mouse.time.tv_usec = ie.time.tv_usec;

	/**
	 * 只处理按键事件
	 **/
	if(ie.type != EV_KEY)
	{
		return 0;
	}

	/* 松开键盘的键 */
	if(ie.value == 0)
	{
		msg.keybd.type = MESSAGE_TYPE_KEYBD_RELEASE;
		msg.keybd.value = 0;
	}
	/* 按下键盘的键 */
	else if(ie.value == 1)
	{
		msg.keybd.type = MESSAGE_TYPE_KEYBD_PRESS;
		msg.keybd.value = 1;
	}
	/* 长时间按下键盘的键 */
	else if(ie.value == 2)
	{
		msg.keybd.type = MESSAGE_TYPE_KEYBD_CLICK;
		msg.keybd.value = 2;
	}

	/* 哪个键 */
	switch(ie.code)
	{
	case KEY_ESC:
		msg.keybd.code = INPUT_CODE_KEYBD_ESC;
		break;
	case KEY_1:
		msg.keybd.code = INPUT_CODE_KEYBD_1;
		break;
	case KEY_2:
		msg.keybd.code = INPUT_CODE_KEYBD_2;
		break;
	case KEY_3:
		msg.keybd.code = INPUT_CODE_KEYBD_3;
		break;
	case KEY_4:
		msg.keybd.code = INPUT_CODE_KEYBD_4;
		break;
	case KEY_5:
		msg.keybd.code = INPUT_CODE_KEYBD_5;
		break;
	case KEY_6:
		msg.keybd.code = INPUT_CODE_KEYBD_6;
		break;
	case KEY_7:
		msg.keybd.code = INPUT_CODE_KEYBD_7;
		break;
	case KEY_8:
		msg.keybd.code = INPUT_CODE_KEYBD_8;
		break;
	case KEY_9:
		msg.keybd.code = INPUT_CODE_KEYBD_9;
		break;
	case KEY_0:
		msg.keybd.code = INPUT_CODE_KEYBD_0;
		break;
	case KEY_MINUS:
		msg.keybd.code = INPUT_CODE_KEYBD_SUB;
		break;
	case KEY_EQUAL:
		msg.keybd.code = INPUT_CODE_KEYBD_EQUAL;
		break;
	case KEY_BACKSPACE:
		msg.keybd.code = INPUT_CODE_KEYBD_BACKSPACE;
		break;
	case KEY_TAB:
		msg.keybd.code = INPUT_CODE_KEYBD_TAB;
		break;
	case KEY_Q:
		msg.keybd.code = INPUT_CODE_KEYBD_Q;
		break;
	case KEY_W:
		msg.keybd.code = INPUT_CODE_KEYBD_W;
		break;
	case KEY_E:
		msg.keybd.code = INPUT_CODE_KEYBD_E;
		break;
	case KEY_R:
		msg.keybd.code = INPUT_CODE_KEYBD_R;
		break;
	case KEY_T:
		msg.keybd.code = INPUT_CODE_KEYBD_T;
		break;
	case KEY_Y:
		msg.keybd.code = INPUT_CODE_KEYBD_Y;
		break;
	case KEY_U:
		msg.keybd.code = INPUT_CODE_KEYBD_U;
		break;
	case KEY_I:
		msg.keybd.code = INPUT_CODE_KEYBD_I;
		break;
	case KEY_O:
		msg.keybd.code = INPUT_CODE_KEYBD_O;
		break;
	case KEY_P:
		msg.keybd.code = INPUT_CODE_KEYBD_P;
		break;
	case KEY_LEFTBRACE:
		msg.keybd.code = INPUT_CODE_KEYBD_L_BRACE;
		break;
	case KEY_RIGHTBRACE:
		msg.keybd.code = INPUT_CODE_KEYBD_R_BRACE;
		break;
	case KEY_ENTER:
		msg.keybd.code = INPUT_CODE_KEYBD_ENTER;
		break;
	case KEY_LEFTCTRL:
		msg.keybd.code = INPUT_CODE_KEYBD_L_CTRL;
		break;
	case KEY_A:
		msg.keybd.code = INPUT_CODE_KEYBD_A;
		break;
	case KEY_S:
		msg.keybd.code = INPUT_CODE_KEYBD_S;
		break;
	case KEY_D:
		msg.keybd.code = INPUT_CODE_KEYBD_D;
		break;
	case KEY_F:
		msg.keybd.code = INPUT_CODE_KEYBD_F;
		break;
	case KEY_G:
		msg.keybd.code = INPUT_CODE_KEYBD_G;
		break;
	case KEY_H:
		msg.keybd.code = INPUT_CODE_KEYBD_H;
		break;
	case KEY_J:
		msg.keybd.code = INPUT_CODE_KEYBD_J;
		break;
	case KEY_K:
		msg.keybd.code = INPUT_CODE_KEYBD_K;
		break;
	case KEY_L:
		msg.keybd.code = INPUT_CODE_KEYBD_L;
		break;
	case KEY_SEMICOLON:
		msg.keybd.code = INPUT_CODE_KEYBD_SEMICOLON;
		break;
	case KEY_APOSTROPHE:
		msg.keybd.code = INPUT_CODE_KEYBD_APOSTROPHE;
		break;
	case KEY_GRAVE:
		msg.keybd.code = INPUT_CODE_KEYBD_GRAVE;
		break;
	case KEY_LEFTSHIFT:
		msg.keybd.code = INPUT_CODE_KEYBD_L_SHIFT;
		break;
	case KEY_BACKSLASH:
		msg.keybd.code = INPUT_CODE_KEYBD_BACKSLASH;
		break;
	case KEY_Z:
		msg.keybd.code = INPUT_CODE_KEYBD_Z;
		break;
	case KEY_X:
		msg.keybd.code = INPUT_CODE_KEYBD_X;
		break;
	case KEY_C:
		msg.keybd.code = INPUT_CODE_KEYBD_C;
		break;
	case KEY_V:
		msg.keybd.code = INPUT_CODE_KEYBD_V;
		break;
	case KEY_B:
		msg.keybd.code = INPUT_CODE_KEYBD_B;
		break;
	case KEY_N:
		msg.keybd.code = INPUT_CODE_KEYBD_N;
		break;
	case KEY_M:
		msg.keybd.code = INPUT_CODE_KEYBD_M;
		break;
	case KEY_COMMA:
		msg.keybd.code = INPUT_CODE_KEYBD_COMMA;
		break;
	case KEY_DOT:
		msg.keybd.code = INPUT_CODE_KEYBD_DOT;
		break;
	case KEY_SLASH:
		msg.keybd.code = INPUT_CODE_KEYBD_SLASH;
		break;
	case KEY_RIGHTSHIFT:
		msg.keybd.code = INPUT_CODE_KEYBD_R_SHIFT;
		break;
	case KEY_KPASTERISK:
		msg.keybd.code = INPUT_CODE_KEYBD_KPASTERISK;
		break;
	case KEY_LEFTALT:
		msg.keybd.code = INPUT_CODE_KEYBD_L_ALT;
		break;
	case KEY_SPACE:
		msg.keybd.code = INPUT_CODE_KEYBD_SPACE;
		break;
	case KEY_CAPSLOCK:
		msg.keybd.code = INPUT_CODE_KEYBD_CAPSLOCK;
		break;
	case KEY_F1:
		msg.keybd.code = INPUT_CODE_KEYBD_F1;
		break;
	case KEY_F2:
		msg.keybd.code = INPUT_CODE_KEYBD_F2;
		break;
	case KEY_F3:
		msg.keybd.code = INPUT_CODE_KEYBD_F3;
		break;
	case KEY_F4:
		msg.keybd.code = INPUT_CODE_KEYBD_F4;
		break;
	case KEY_F5:
		msg.keybd.code = INPUT_CODE_KEYBD_F5;
		break;
	case KEY_F6:
		msg.keybd.code = INPUT_CODE_KEYBD_F6;
		break;
	case KEY_F7:
		msg.keybd.code = INPUT_CODE_KEYBD_F7;
		break;
	case KEY_F8:
		msg.keybd.code = INPUT_CODE_KEYBD_F8;
		break;
	case KEY_F9:
		msg.keybd.code = INPUT_CODE_KEYBD_F9;
		break;
	case KEY_F10:
		msg.keybd.code = INPUT_CODE_KEYBD_F10;
		break;
	case KEY_NUMLOCK:
		msg.keybd.code = INPUT_CODE_KEYBD_NUMLOCK;
		break;
	case KEY_SCROLLLOCK:
		msg.keybd.code = INPUT_CODE_KEYBD_SCROLLLOCK;
		break;
	case KEY_KP7:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_7;
		break;
	case KEY_KP8:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_8;
		break;
	case KEY_KP9:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_9;
		break;
	case KEY_KPMINUS:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_SUB;
		break;
	case KEY_KP4:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_4;
		break;
	case KEY_KP5:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_5;
		break;
	case KEY_KP6:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_6;
		break;
	case KEY_KPPLUS:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_ADD;
		break;
	case KEY_KP1:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_1;
		break;
	case KEY_KP2:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_2;
		break;
	case KEY_KP3:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_3;
		break;
	case KEY_KP0:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_0;
		break;
	case KEY_KPDOT:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_DOT;
		break;
	case KEY_F11:
		msg.keybd.code = INPUT_CODE_KEYBD_F11;
		break;
	case KEY_F12:
		msg.keybd.code = INPUT_CODE_KEYBD_F12;
		break;
	case KEY_KPENTER:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_ENTER;
		break;
	case KEY_RIGHTCTRL:
		msg.keybd.code = INPUT_CODE_KEYBD_R_CTRL;
		break;
	case KEY_KPSLASH:
		msg.keybd.code = INPUT_CODE_KEYBD_KP_SLASH;
		break;
	case KEY_SYSRQ:
		msg.keybd.code = INPUT_CODE_KEYBD_SYSRQ;
		break;
	case KEY_RIGHTALT:
		msg.keybd.code = INPUT_CODE_KEYBD_R_ALT;
		break;
	case KEY_HOME:
		msg.keybd.code = INPUT_CODE_KEYBD_HOME;
		break;
	case KEY_UP:
		msg.keybd.code = INPUT_CODE_KEYBD_UP;
		break;
	case KEY_PAGEUP:
		msg.keybd.code = INPUT_CODE_KEYBD_PAGEUP;
		break;
	case KEY_LEFT:
		msg.keybd.code = INPUT_CODE_KEYBD_LEFT;
		break;
	case KEY_RIGHT:
		msg.keybd.code = INPUT_CODE_KEYBD_RIGHT;
		break;
	case KEY_END:
		msg.keybd.code = INPUT_CODE_KEYBD_END;
		break;
	case KEY_DOWN:
		msg.keybd.code = INPUT_CODE_KEYBD_DOWN;
		break;
	case KEY_PAGEDOWN:
		msg.keybd.code = INPUT_CODE_KEYBD_PAGEDOWN;
		break;
	case KEY_INSERT:
		msg.keybd.code = INPUT_CODE_KEYBD_INSERT;
		break;
	case KEY_DELETE:
		msg.keybd.code = INPUT_CODE_KEYBD_DELETE;
		break;
	case KEY_PAUSE:
		msg.keybd.code = INPUT_CODE_KEYBD_PAUSE;
		break;
	case KEY_LEFTMETA:
		msg.keybd.code = INPUT_CODE_KEYBD_L_META;
		break;
	case KEY_RIGHTMETA:
		msg.keybd.code = INPUT_CODE_KEYBD_R_META;
		break;
	case KEY_COMPOSE:
		msg.keybd.code = INPUT_CODE_KEYBD_COMPOSE;
		break;
	default:
		break;
	}

	list_push_back(msg_list, &msg, sizeof(union message));
	return 0;

}

static si_t keybd_exit(struct input_device * self)
{
	return uds_exit(&self->uds);
}

