# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <linux/input.h>

# include "event.h"
# include "log.h"

static si_t deal_with_mouse(struct input_device * self, struct list* msg_list);
static si_t mouse_exit(struct input_device * self);

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

static si_t basic_mouse(struct input_device * self, union message * m)
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
    m->mouse.time.tv_sec = ie.time.tv_sec;
    m->mouse.time.tv_usec = ie.time.tv_usec;

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
        /* 松开鼠标的键 */
        if(ie.value == 0)
        {
            /* 消息类型 */
            m->mouse.type = MESSAGE_TYPE_MOUSE_RELEASE;
            m->mouse.value = 0;
        }
        /* 按下鼠标的键 */
        else if(ie.value == 1)
        {
            /* 消息的类型 */
            m->mouse.type = MESSAGE_TYPE_MOUSE_PRESS;
            m->mouse.value = 1;
        }

        /* 哪个键 */
        switch(ie.code)
        {
            /* 左键 */
            case BTN_LEFT:
                m->mouse.code = INPUT_CODE_MOUSE_L_KEY;
                break;
            /* 右键 */
            case BTN_RIGHT:
                m->mouse.code = INPUT_CODE_MOUSE_R_KEY;
                break;
            /* 中键 */
            case BTN_MIDDLE:
                m->mouse.code = INPUT_CODE_MOUSE_M_KEY;
                break;
            default:
                break;
        }

        return 0;
    }
    else if(ie.type == EV_REL || ie.type == EV_ABS)
    {
        /* 消息类型 */
        if(ie.type == EV_REL) {
            m->mouse.type = MESSAGE_TYPE_MOUSE_MOVE;
        } else {
            m->mouse.type = MESSAGE_TYPE_MOUSE_MOVE_POINT;
        }
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
    } else if(ie.type == EV_MSC){
        if(ie.code == MSC_SCAN) { /* physical mapping of a key, ignoring */
            return 1;
        }
    }

    return -1;
}

static si_t deal_with_mouse(struct input_device * self, struct list* msg_list)
{
	union message msg;
    int get_msg_res;
	if(0 > (get_msg_res=basic_mouse(self, &msg)))
	{
		EGUI_PRINT_ERROR("failed to get mouse msg");
		return -1;
	}
    if(1==get_msg_res) return 0;

	list_push_back(msg_list, &msg, sizeof(union message));

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

si_t mouse_init(struct input_device * self, char * mouse_device_path, si_t double_click_delay)
{
    si_t fd;

    if((fd = open(mouse_device_path, O_RDONLY)) < 0)
    {
		EGUI_PRINT_SYS_ERROR("failed to open mouse device: open()");
        return -1;
    }

    uds_init_from_naked_socket(&self->uds, fd);
	self->deal_with_input = deal_with_mouse;
	self->input_exit = mouse_exit;

	mouse_double_click_delay = double_click_delay;

    return 0;
}


static si_t mouse_exit(struct input_device * self)
{
	return uds_exit(&self->uds);
}

