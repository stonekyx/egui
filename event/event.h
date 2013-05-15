# ifndef _EVENT_H_
# define _EVENT_H_ 1

# include "comm.h"

struct input_device
{
	struct egui_uds uds;
    si_t (*deal_with_input)(struct input_device * self, struct list* msg_list);
    si_t (*input_exit)(struct input_device * self);
};

/* 键盘设备 */
extern si_t keybd_init(struct input_device * self, char * keybd_device_path);

/* 鼠标设备 */
extern si_t mouse_init(struct input_device * self, char * mouse_device_path, si_t double_click_delay);

# endif

