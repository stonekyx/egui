# ifndef _EVENT_H_
# define _EVENT_H_ 1

# include "comm.h"

struct input_device
{
	struct egui_uds uds;
    si_t (*deal_with_input)(struct input_device * self, struct list* msg_list);
    si_t (*input_exit)(struct input_device * self);
};

extern si_t evdev_init(struct input_device * self, char * device_path);

extern void set_double_click_delay(si_t double_click_delay);

# endif

