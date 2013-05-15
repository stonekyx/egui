#include <stdio.h>
#include <fcntl.h>

#include "base_type.h"
#include "comm.h"
#include "event.h"
#include "log.h"

#define KEYBD_DEVICE_PATH "/dev/input/event2"
#define MOUSE_DEVICE_PATH "/dev/input/event4"

static void input_device_destructor(struct egui_uds* uds, addr_t arg)
{
	struct input_device* id_ptr = arg;
	if(0 != id_ptr->input_exit(id_ptr))
	{
		EGUI_PRINT_ERROR("failed to exit input device");
	}
}

static const char* get_event_str(int type)
{
	switch(type)
	{
	case MESSAGE_TYPE_KEYBD_RELEASE:
		return "MESSAGE_TYPE_KEYBD_RELEASE";
		break;
	case MESSAGE_TYPE_KEYBD_PRESS:
		return "MESSAGE_TYPE_KEYBD_PRESS";
		break;
	case MESSAGE_TYPE_KEYBD_CLICK:
		return "MESSAGE_TYPE_KEYBD_CLICK";
		break;
	case MESSAGE_TYPE_MOUSE_RELEASE:
		return "MESSAGE_TYPE_MOUSE_RELEASE";
		break;
	case MESSAGE_TYPE_MOUSE_PRESS:
		return "MESSAGE_TYPE_MOUSE_PRESS";
		break;
	case MESSAGE_TYPE_MOUSE_MOVE:
		return "MESSAGE_TYPE_MOUSE_MOVE";
		break;
	case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
		return "MESSAGE_TYPE_MOUSE_SINGLE_CLICK";
		break;
	case MESSAGE_TYPE_MOUSE_DOUBLE_CLICK:
		return "MESSAGE_TYPE_MOUSE_DOUBLE_CLICK";
		break;
	default:
		break;
	}
	return "";
}

static si_t input_event_handler(struct egui_uds* uds_ptr, addr_t arg)
{
	struct input_device* id_ptr = arg;
	struct list msg_list;

	list_init(&msg_list);

	id_ptr->deal_with_input(id_ptr, &msg_list);

	while(!list_empty(&msg_list))
	{
		union message* m = list_front(&msg_list);
		EGUI_PRINT_INFO("eMsg(type:%s code:%d value:%d)", get_event_str(m->mouse.type), (int)m->mouse.code, (int)m->mouse.value);
		list_pop_front(&msg_list);
	}

	list_exit(&msg_list);

	return SELECTER_RETURN_TYPE_CONTINUE;
}

int main()
{
    struct input_device keybd_device;
    struct input_device mouse_device;
	struct event_listener server_listener;

	event_listener_init(&server_listener);

    if(keybd_init(&keybd_device, KEYBD_DEVICE_PATH) < 0)
    {
		EGUI_PRINT_ERROR("failed to init keybd device");
        return -1;
    }
	event_listener_add_read_event(&server_listener, &keybd_device.uds, &keybd_device, input_event_handler, input_device_destructor);

    if (mouse_init(&mouse_device, MOUSE_DEVICE_PATH, 10000) < 0)
    {
		EGUI_PRINT_ERROR("failed to init mouse device");
        return -1;
    }
	event_listener_add_read_event(&server_listener, &mouse_device.uds, &mouse_device, input_event_handler, input_device_destructor);

	if(0 != event_listener_exec(&server_listener))
	{
		EGUI_PRINT_ERROR("failed to execute event_listener");
		return -1;
	}

	event_listener_exit(&server_listener);

    keybd_device.input_exit(&keybd_device);
    mouse_device.input_exit(&mouse_device);

    return 0;
}

