# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <linux/input.h>

# include "event.h"
# include "log.h"
# include "config.h"

static si_t deal_with_event(struct input_device * self, struct list* msg_list);
static si_t evdev_exit(struct input_device * self);

static si_t mouse_double_click_delay = 1;

struct mouse_click_status
{
    /** 左键的状态.0 松开，1 按下  **/
    si_t l_key;

    /** 中键的状态.0 松开，1 按下  **/
    si_t m_key;

    /** 右键的状态.0 松开，1 按下  **/
    si_t r_key;

    /** 以前有过  **/
    si_t has_previous_click;

    /** 以前有过哪个键  **/
    si_t previous_click_key;
};

static struct timeval mouse_previous_click_time;

/**
 * 点击的状态
 **/
static struct mouse_click_status mouse_click = {0, 0, 0, 0, 0};

/**
 * @brief 获得两个 timeval 的差
 *
 * t1 - t2
 *
 * @param t1 结束时间
 * @param t2 起始时间
 * @param result 保存结果
 *
 * @return t1 >= t2 返回 0，t1 < t2 返回 -1。
 **/
static si_t diff_timeval(struct timeval * t1, struct timeval * t2, struct timeval * result)
{
    if(t1->tv_usec < t2->tv_usec)
    {
        result->tv_usec = t1->tv_usec + 1000000 - t2->tv_usec;
        result->tv_sec = t1->tv_sec - 1 - t2->tv_sec;
    }
    else
    {
        result->tv_usec = t1->tv_usec - t2->tv_usec;
        result->tv_sec = t1->tv_sec - t2->tv_sec;
    }

    return 0;
}

static si_t basic_event(struct input_device * self, union message * m)
{
    struct input_event ie;
    static int syn_ok;

    /* 读取数据 */
    if(uds_read(&self->uds, &ie, sizeof(struct input_event)) < 0)
    {
        EGUI_PRINT_ERROR("failed to read from keybd input device.");
        return -1;
    }

    if(ie.type != EV_SYN && !syn_ok) {
        return 1;
    }

    /* 时间 */
    m->base.time.tv_sec = ie.time.tv_sec;
    m->base.time.tv_usec = ie.time.tv_usec;

    if(ie.type == EV_SYN) {
        switch(ie.code) {
            case SYN_REPORT:
                syn_ok=1;
                return 1;
                break;
            case SYN_CONFIG:
                return 1;
                break;
            case SYN_MT_REPORT: /* FIXME: for touch-pad events, not implemented. */
                return 1;
                break;
            case SYN_DROPPED:
                syn_ok=0;
                return 1;
                break;
            default:
                EGUI_PRINT_ERROR("Unknown input_event code %d for type EV_SYN.", ie.code);
                return -1;
        }
    } else if(ie.type == EV_KEY)
    {
        int is_mouse_event = 0;

        /* 哪个键 */
        switch(ie.code)
        {
            case BTN_LEFT: /* 左键 */
                m->mouse.code = INPUT_CODE_MOUSE_L_KEY;
                is_mouse_event = 1;
                break;
            case BTN_RIGHT: /* 右键 */
                m->mouse.code = INPUT_CODE_MOUSE_R_KEY;
                is_mouse_event = 1;
                break;
            case BTN_MIDDLE: /* 中键 */
                m->mouse.code = INPUT_CODE_MOUSE_M_KEY;
                is_mouse_event = 1;
                break;
            case KEY_ESC:
                m->keybd.code = INPUT_CODE_KEYBD_ESC;
                break;
            case KEY_1:
                m->keybd.code = INPUT_CODE_KEYBD_1;
                break;
            case KEY_2:
                m->keybd.code = INPUT_CODE_KEYBD_2;
                break;
            case KEY_3:
                m->keybd.code = INPUT_CODE_KEYBD_3;
                break;
            case KEY_4:
                m->keybd.code = INPUT_CODE_KEYBD_4;
                break;
            case KEY_5:
                m->keybd.code = INPUT_CODE_KEYBD_5;
                break;
            case KEY_6:
                m->keybd.code = INPUT_CODE_KEYBD_6;
                break;
            case KEY_7:
                m->keybd.code = INPUT_CODE_KEYBD_7;
                break;
            case KEY_8:
                m->keybd.code = INPUT_CODE_KEYBD_8;
                break;
            case KEY_9:
                m->keybd.code = INPUT_CODE_KEYBD_9;
                break;
            case KEY_0:
                m->keybd.code = INPUT_CODE_KEYBD_0;
                break;
            case KEY_MINUS:
                m->keybd.code = INPUT_CODE_KEYBD_SUB;
                break;
            case KEY_EQUAL:
                m->keybd.code = INPUT_CODE_KEYBD_EQUAL;
                break;
            case KEY_BACKSPACE:
                m->keybd.code = INPUT_CODE_KEYBD_BACKSPACE;
                break;
            case KEY_TAB:
                m->keybd.code = INPUT_CODE_KEYBD_TAB;
                break;
            case KEY_Q:
                m->keybd.code = INPUT_CODE_KEYBD_Q;
                break;
            case KEY_W:
                m->keybd.code = INPUT_CODE_KEYBD_W;
                break;
            case KEY_E:
                m->keybd.code = INPUT_CODE_KEYBD_E;
                break;
            case KEY_R:
                m->keybd.code = INPUT_CODE_KEYBD_R;
                break;
            case KEY_T:
                m->keybd.code = INPUT_CODE_KEYBD_T;
                break;
            case KEY_Y:
                m->keybd.code = INPUT_CODE_KEYBD_Y;
                break;
            case KEY_U:
                m->keybd.code = INPUT_CODE_KEYBD_U;
                break;
            case KEY_I:
                m->keybd.code = INPUT_CODE_KEYBD_I;
                break;
            case KEY_O:
                m->keybd.code = INPUT_CODE_KEYBD_O;
                break;
            case KEY_P:
                m->keybd.code = INPUT_CODE_KEYBD_P;
                break;
            case KEY_LEFTBRACE:
                m->keybd.code = INPUT_CODE_KEYBD_L_BRACE;
                break;
            case KEY_RIGHTBRACE:
                m->keybd.code = INPUT_CODE_KEYBD_R_BRACE;
                break;
            case KEY_ENTER:
                m->keybd.code = INPUT_CODE_KEYBD_ENTER;
                break;
            case KEY_LEFTCTRL:
                m->keybd.code = INPUT_CODE_KEYBD_L_CTRL;
                break;
            case KEY_A:
                m->keybd.code = INPUT_CODE_KEYBD_A;
                break;
            case KEY_S:
                m->keybd.code = INPUT_CODE_KEYBD_S;
                break;
            case KEY_D:
                m->keybd.code = INPUT_CODE_KEYBD_D;
                break;
            case KEY_F:
                m->keybd.code = INPUT_CODE_KEYBD_F;
                break;
            case KEY_G:
                m->keybd.code = INPUT_CODE_KEYBD_G;
                break;
            case KEY_H:
                m->keybd.code = INPUT_CODE_KEYBD_H;
                break;
            case KEY_J:
                m->keybd.code = INPUT_CODE_KEYBD_J;
                break;
            case KEY_K:
                m->keybd.code = INPUT_CODE_KEYBD_K;
                break;
            case KEY_L:
                m->keybd.code = INPUT_CODE_KEYBD_L;
                break;
            case KEY_SEMICOLON:
                m->keybd.code = INPUT_CODE_KEYBD_SEMICOLON;
                break;
            case KEY_APOSTROPHE:
                m->keybd.code = INPUT_CODE_KEYBD_APOSTROPHE;
                break;
            case KEY_GRAVE:
                m->keybd.code = INPUT_CODE_KEYBD_GRAVE;
                break;
            case KEY_LEFTSHIFT:
                m->keybd.code = INPUT_CODE_KEYBD_L_SHIFT;
                break;
            case KEY_BACKSLASH:
                m->keybd.code = INPUT_CODE_KEYBD_BACKSLASH;
                break;
            case KEY_Z:
                m->keybd.code = INPUT_CODE_KEYBD_Z;
                break;
            case KEY_X:
                m->keybd.code = INPUT_CODE_KEYBD_X;
                break;
            case KEY_C:
                m->keybd.code = INPUT_CODE_KEYBD_C;
                break;
            case KEY_V:
                m->keybd.code = INPUT_CODE_KEYBD_V;
                break;
            case KEY_B:
                m->keybd.code = INPUT_CODE_KEYBD_B;
                break;
            case KEY_N:
                m->keybd.code = INPUT_CODE_KEYBD_N;
                break;
            case KEY_M:
                m->keybd.code = INPUT_CODE_KEYBD_M;
                break;
            case KEY_COMMA:
                m->keybd.code = INPUT_CODE_KEYBD_COMMA;
                break;
            case KEY_DOT:
                m->keybd.code = INPUT_CODE_KEYBD_DOT;
                break;
            case KEY_SLASH:
                m->keybd.code = INPUT_CODE_KEYBD_SLASH;
                break;
            case KEY_RIGHTSHIFT:
                m->keybd.code = INPUT_CODE_KEYBD_R_SHIFT;
                break;
            case KEY_KPASTERISK:
                m->keybd.code = INPUT_CODE_KEYBD_KPASTERISK;
                break;
            case KEY_LEFTALT:
                m->keybd.code = INPUT_CODE_KEYBD_L_ALT;
                break;
            case KEY_SPACE:
                m->keybd.code = INPUT_CODE_KEYBD_SPACE;
                break;
            case KEY_CAPSLOCK:
                m->keybd.code = INPUT_CODE_KEYBD_CAPSLOCK;
                break;
            case KEY_F1:
                m->keybd.code = INPUT_CODE_KEYBD_F1;
                break;
            case KEY_F2:
                m->keybd.code = INPUT_CODE_KEYBD_F2;
                break;
            case KEY_F3:
                m->keybd.code = INPUT_CODE_KEYBD_F3;
                break;
            case KEY_F4:
                m->keybd.code = INPUT_CODE_KEYBD_F4;
                break;
            case KEY_F5:
                m->keybd.code = INPUT_CODE_KEYBD_F5;
                break;
            case KEY_F6:
                m->keybd.code = INPUT_CODE_KEYBD_F6;
                break;
            case KEY_F7:
                m->keybd.code = INPUT_CODE_KEYBD_F7;
                break;
            case KEY_F8:
                m->keybd.code = INPUT_CODE_KEYBD_F8;
                break;
            case KEY_F9:
                m->keybd.code = INPUT_CODE_KEYBD_F9;
                break;
            case KEY_F10:
                m->keybd.code = INPUT_CODE_KEYBD_F10;
                break;
            case KEY_NUMLOCK:
                m->keybd.code = INPUT_CODE_KEYBD_NUMLOCK;
                break;
            case KEY_SCROLLLOCK:
                m->keybd.code = INPUT_CODE_KEYBD_SCROLLLOCK;
                break;
            case KEY_KP7:
                m->keybd.code = INPUT_CODE_KEYBD_KP_7;
                break;
            case KEY_KP8:
                m->keybd.code = INPUT_CODE_KEYBD_KP_8;
                break;
            case KEY_KP9:
                m->keybd.code = INPUT_CODE_KEYBD_KP_9;
                break;
            case KEY_KPMINUS:
                m->keybd.code = INPUT_CODE_KEYBD_KP_SUB;
                break;
            case KEY_KP4:
                m->keybd.code = INPUT_CODE_KEYBD_KP_4;
                break;
            case KEY_KP5:
                m->keybd.code = INPUT_CODE_KEYBD_KP_5;
                break;
            case KEY_KP6:
                m->keybd.code = INPUT_CODE_KEYBD_KP_6;
                break;
            case KEY_KPPLUS:
                m->keybd.code = INPUT_CODE_KEYBD_KP_ADD;
                break;
            case KEY_KP1:
                m->keybd.code = INPUT_CODE_KEYBD_KP_1;
                break;
            case KEY_KP2:
                m->keybd.code = INPUT_CODE_KEYBD_KP_2;
                break;
            case KEY_KP3:
                m->keybd.code = INPUT_CODE_KEYBD_KP_3;
                break;
            case KEY_KP0:
                m->keybd.code = INPUT_CODE_KEYBD_KP_0;
                break;
            case KEY_KPDOT:
                m->keybd.code = INPUT_CODE_KEYBD_KP_DOT;
                break;
            case KEY_F11:
                m->keybd.code = INPUT_CODE_KEYBD_F11;
                break;
            case KEY_F12:
                m->keybd.code = INPUT_CODE_KEYBD_F12;
                break;
            case KEY_KPENTER:
                m->keybd.code = INPUT_CODE_KEYBD_KP_ENTER;
                break;
            case KEY_RIGHTCTRL:
                m->keybd.code = INPUT_CODE_KEYBD_R_CTRL;
                break;
            case KEY_KPSLASH:
                m->keybd.code = INPUT_CODE_KEYBD_KP_SLASH;
                break;
            case KEY_SYSRQ:
                m->keybd.code = INPUT_CODE_KEYBD_SYSRQ;
                break;
            case KEY_RIGHTALT:
                m->keybd.code = INPUT_CODE_KEYBD_R_ALT;
                break;
            case KEY_HOME:
                m->keybd.code = INPUT_CODE_KEYBD_HOME;
                break;
            case KEY_UP:
                m->keybd.code = INPUT_CODE_KEYBD_UP;
                break;
            case KEY_PAGEUP:
                m->keybd.code = INPUT_CODE_KEYBD_PAGEUP;
                break;
            case KEY_LEFT:
                m->keybd.code = INPUT_CODE_KEYBD_LEFT;
                break;
            case KEY_RIGHT:
                m->keybd.code = INPUT_CODE_KEYBD_RIGHT;
                break;
            case KEY_END:
                m->keybd.code = INPUT_CODE_KEYBD_END;
                break;
            case KEY_DOWN:
                m->keybd.code = INPUT_CODE_KEYBD_DOWN;
                break;
            case KEY_PAGEDOWN:
                m->keybd.code = INPUT_CODE_KEYBD_PAGEDOWN;
                break;
            case KEY_INSERT:
                m->keybd.code = INPUT_CODE_KEYBD_INSERT;
                break;
            case KEY_DELETE:
                m->keybd.code = INPUT_CODE_KEYBD_DELETE;
                break;
            case KEY_PAUSE:
                m->keybd.code = INPUT_CODE_KEYBD_PAUSE;
                break;
            case KEY_LEFTMETA:
                m->keybd.code = INPUT_CODE_KEYBD_L_META;
                break;
            case KEY_RIGHTMETA:
                m->keybd.code = INPUT_CODE_KEYBD_R_META;
                break;
            case KEY_COMPOSE:
                m->keybd.code = INPUT_CODE_KEYBD_COMPOSE;
                break;
            default:
                break;
        }

        if(is_mouse_event) {
            m->mouse.value = ie.value;
            switch(ie.value) {
                case 0:
                    m->mouse.type = MESSAGE_TYPE_MOUSE_RELEASE;
                    break;
                case 1:
                    m->mouse.type = MESSAGE_TYPE_MOUSE_PRESS;
                    break;
                default:
                    break;
            }
        } else {
            m->keybd.value = ie.value;
            switch(ie.value) {
                case 0:
                    m->keybd.type = MESSAGE_TYPE_KEYBD_RELEASE;
                    break;
                case 1:
                    m->keybd.type = MESSAGE_TYPE_KEYBD_PRESS;
                    break;
                case 2: /* 长时间按下键盘的键 */
                    m->keybd.type = MESSAGE_TYPE_KEYBD_CLICK;
                    break;
                default:
                    break;
            }
        }

        return 0;
    }
    else if(ie.type == EV_REL)
    {
        /* 消息类型 */
        m->mouse.type = MESSAGE_TYPE_MOUSE_MOVE;
        /* 偏移多少 */
        m->mouse.value = ie.value;

        /* 哪个方向的偏移 */
        switch(ie.code)
        {
            case REL_X:
                m->mouse.code = INPUT_CODE_MOUSE_X_OFFSET;
                break;
            case REL_Y:
                m->mouse.code = INPUT_CODE_MOUSE_Y_OFFSET;
                break;
            case REL_WHEEL:
                m->mouse.code = INPUT_CODE_MOUSE_Z_OFFSET;
                break;
            default:
                break;
        }

        return 0;
    } else if(ie.type == EV_ABS) {
        static struct input_absinfo absinfo_x, absinfo_y, absinfo_wheel;
        static int init_flag = 0;
        const struct input_absinfo *absinfo_use;
        if(!init_flag) {
            ioctl(self->uds.sock_fd, EVIOCGABS(ABS_X), &absinfo_x);
            ioctl(self->uds.sock_fd, EVIOCGABS(ABS_Y), &absinfo_y);
            ioctl(self->uds.sock_fd, EVIOCGABS(ABS_WHEEL), &absinfo_wheel);
            init_flag = 1;
        }
        m->mouse.type = MESSAGE_TYPE_MOUSE_MOVE_POINT;
        switch(ie.code) {
            case ABS_X:
                m->mouse.code = INPUT_CODE_MOUSE_X_OFFSET;
                absinfo_use = &absinfo_x;
                break;
            case ABS_Y:
                m->mouse.code = INPUT_CODE_MOUSE_Y_OFFSET;
                absinfo_use = &absinfo_y;
                break;
            case ABS_WHEEL:
                m->mouse.code = INPUT_CODE_MOUSE_Z_OFFSET;
                absinfo_use = &absinfo_wheel;
                break;
            default:
                /* There are other kinds of signals like ABS_PRESSURE,
                 * which is related to touchpads or touch screens.
                 *
                 * Besides, Linux 3.10.10 and later versions give ABS signals
                 * even for touchpad on laptops, making the control harder
                 * in our project. We should consider separating this part
                 * from the server, making it into a plugin, so we can write
                 * other kinds of handlers. */
                return 1;
                break;
        }
        m->mouse.value =
            (ie.value-absinfo_use->minimum)/
            (double)(absinfo_use->maximum-absinfo_use->minimum)*MOUSE_RESOLUTION;
        return 0;
    } else if(ie.type == EV_MSC){
        if(ie.code == MSC_SCAN) { /* physical mapping of a key, ignoring */
            return 1;
        }
    }

    return -1;
}

static si_t deal_with_event(struct input_device * self, struct list* msg_list)
{
    union message msg;
    int get_msg_res;
    memset(&msg, 0, sizeof(msg));
    if(0 > (get_msg_res=basic_event(self, &msg)))
    {
        EGUI_PRINT_ERROR("failed to get event");
        return -1;
    }
    if(1==get_msg_res) return 0;

    list_push_back(msg_list, &msg, sizeof(union message));

    /************************************
     * 以下处理鼠标的点击事件和双击事件 *
     ************************************/

    if(msg.mouse.type == MESSAGE_TYPE_MOUSE_PRESS)
    {
        /* 更改鼠标各个键的状态 */
        if(msg.mouse.code == INPUT_CODE_MOUSE_L_KEY)
        {
            mouse_click.l_key = 1;
        }
        else if(msg.mouse.code == INPUT_CODE_MOUSE_M_KEY)
        {
            mouse_click.m_key = 1;
        }
        else if(msg.mouse.code == INPUT_CODE_MOUSE_R_KEY)
        {
            mouse_click.r_key = 1;
        }
    }
    else if(msg.mouse.type == MESSAGE_TYPE_MOUSE_RELEASE)
    {
        union message click_message;
        /* 更改鼠标各个键的状态 */
        if(msg.mouse.code == INPUT_CODE_MOUSE_L_KEY)
        {
            mouse_click.l_key = 0;
        }
        else if(msg.mouse.code == INPUT_CODE_MOUSE_M_KEY)
        {
            mouse_click.m_key = 0;
        }
        else if(msg.mouse.code == INPUT_CODE_MOUSE_R_KEY)
        {
            mouse_click.r_key = 0;
        }

        /* 以前没有 */
        if(mouse_click.has_previous_click == 0)
        {
            mouse_click.has_previous_click = 1;
            mouse_click.previous_click_key = msg.mouse.code;
            mouse_previous_click_time = msg.mouse.time;

            memcpy(&click_message, &msg, sizeof(union message));

            click_message.mouse.type = MESSAGE_TYPE_MOUSE_SINGLE_CLICK;

            list_push_back(msg_list, &click_message, sizeof(union message));
        }
        /* 以前有过 */
        else if(mouse_click.has_previous_click == 1)
        {
            /* 两次的键一致 */
            if(mouse_click.previous_click_key == msg.mouse.code)
            {
                struct timeval difference = {0};
                /* difference = basic->mouse.time - mouse_previous_click_time */
                diff_timeval(&(msg.mouse.time),&mouse_previous_click_time, &difference);

                memcpy(&click_message, &msg, sizeof(union message));

                /* 在规定的时间内 */
                if(difference.tv_sec == 0 && difference.tv_usec < mouse_double_click_delay)
                {
                    click_message.mouse.type = MESSAGE_TYPE_MOUSE_DOUBLE_CLICK;

                    mouse_click.has_previous_click = 0;
                }
                else
                {
                    click_message.mouse.type = MESSAGE_TYPE_MOUSE_SINGLE_CLICK;
                    mouse_previous_click_time = msg.mouse.time;
                }

                list_push_back(msg_list, &click_message, sizeof(union message));
            }
            /* 两次的键不一致 */
            else
            {
                mouse_click.previous_click_key = msg.mouse.code;
                mouse_previous_click_time = msg.mouse.time;

                memcpy(&click_message, &msg, sizeof(union message));

                click_message.mouse.type = MESSAGE_TYPE_MOUSE_SINGLE_CLICK;

                list_push_back(msg_list, &click_message, sizeof(union message));
            }
        }
    }
    else if(msg.mouse.type == MESSAGE_TYPE_MOUSE_MOVE)
    {
        union message* msg_ptr = list_back(msg_list);

        if(mouse_click.l_key == 1)
        {
            /*
               INPUT_MODIFIER_BIT_SET(INPUT_MODIFIER_MASK_MOUSE_L_KEY, &(msg_ptr->mouse.mask));
               */
            msg_ptr->mouse.mask = INPUT_MODIFIER_MASK_MOUSE_L_KEY | msg_ptr->mouse.mask;
        }

        if(mouse_click.m_key == 1)
        {
            /*
               INPUT_MODIFIER_BIT_SET(INPUT_MODIFIER_MASK_MOUSE_M_KEY, &(msg_ptr->mouse.mask));
               */
            msg_ptr->mouse.mask = INPUT_MODIFIER_MASK_MOUSE_M_KEY | msg_ptr->mouse.mask;
        }

        if(mouse_click.r_key == 1)
        {
            /*
               INPUT_MODIFIER_BIT_SET(INPUT_MODIFIER_MASK_MOUSE_R_KEY, &(msg_ptr->mouse.mask));
               */
            msg_ptr->mouse.mask = INPUT_MODIFIER_MASK_MOUSE_R_KEY | msg_ptr->mouse.mask;
        }
    }

    return 0;
}

si_t evdev_init(struct input_device * self, char * device_path)
{
    si_t fd;

    if((fd = open(device_path, O_RDONLY)) < 0)
    {
        EGUI_PRINT_SYS_ERROR("failed to open device: open()");
        return -1;
    }

    uds_init_from_naked_socket(&self->uds, fd);
    self->deal_with_input = deal_with_event;
    self->input_exit = evdev_exit;

    return 0;
}

void set_double_click_delay(si_t double_click_delay)
{
    mouse_double_click_delay = double_click_delay;
}


static si_t evdev_exit(struct input_device * self)
{
    return uds_exit(&self->uds);
}

