#include <stdlib.h>

#include <log.h>

#include "tab_page.h"

static si_t tab_page_head_button_callback(addr_t self, addr_t msg)
{
    struct button *b = self;
    union message *m = msg;
    struct tab_page *belong = b->custom_data;
    switch(m->base.type) {
        case MESSAGE_TYPE_MOUSE_PRESS:
            button_default_callback(self, msg);
            belong->head_pressed = 1;
            if(belong->belong) {
                tab_set_focus(belong->belong, belong);
            }
            break;
        case MESSAGE_TYPE_MOUSE_RELEASE:
            if(!belong->head_pressed) {
                button_default_callback(self, msg);
            }
            break;
        default:
            return button_default_callback(self, msg);
    }
    return 0;
}

struct tab_page *tab_page_init(const char *title)
{
    struct tab_page *addr = calloc(1, sizeof(struct tab_page));
    if(!addr) {
        EGUI_PRINT_SYS_ERROR("malloc failed");
        return NULL;
    }
    addr->page_head = button_init(title);
    addr->page_head->custom_data = addr;
    addr->page_head->callback = tab_page_head_button_callback;
    button_set_bounds_auto(addr->page_head);
    addr->head_pressed = 0;
    return addr;
}

si_t tab_page_exit(struct tab_page *b)
{
    button_exit(b->page_head);
    free(b);
    return 0;
}

void tab_page_set_color(struct tab_page *p, struct color *fore, struct color *back)
{
    button_set_color(p->page_head, fore, back);
}

void tab_page_set_font(struct tab_page *p, si_t font)
{
    button_set_font(p->page_head, font);
    button_set_bounds_auto(p->page_head);
}

si_t tab_page_head_release(struct tab_page *tp)
{
    union message msg;
    msg.base.type = MESSAGE_TYPE_MOUSE_RELEASE;
    return tab_page_head_button_callback(tp->page_head, &msg);
}
