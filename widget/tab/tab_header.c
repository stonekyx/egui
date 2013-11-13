#include "comm.h"
#include "base_type.h"

#include "tab_header.h"

static si_t tab_header_default_callback(addr_t s, addr_t m)
{
    struct tab_header *self = s;
    union message *msg = m;
    switch(m->base.type) {
        case MESSAGE_TYPE_MOUSE_PRESS:
            tab_header_set_focus(
                    self,
                    tab_header_find_unit(
                        self,
                        msg->base.cursor_position));
            /* Fall through */
        case MESSAGE_TYPE_WIDGET_REPAINT:
            tab_header_repaint(self);
            break;
        case MESSAGE_TYPE_WIDGET_SHOW:
            tab_header_show(self);
            break;
        default:
            break;
    }
    return 0;
}

struct tab_header *tab_header_init(void)
{
    struct tab_header *addr;
    addr = calloc(1, sizeof(struct tab_header));
    if(NULL == addr) {
        EGUI_PRINT_SYS_ERROR("failed to malloc");
        return NULL;
    }
    if(!(addr=widget_init_common(WIDGET_POINTER(addr), 0))) {
        return NULL;
    }

    addr->name = "struct tab_header";
    addr->callback = tab_header_default_callback;
    list_init(&addr->pages);

    return addr;
}

void tab_header_set_callback(
        struct tab_header *self,
        tab_header_callback_t callback)
{
    self->callback_to_tab = callback;
}

void tab_header_add_page(
        struct tab_header *self,
        struct tab_page *page_ptr)
{
    struct tab_header_unit tmp = {
        .page = page_ptr,
        .width = 0,
        .height = 0
    };
    list_push_back(&self->pages, &tmp, sizeof(struct tab_header_unit));
}

void tab_header_repaint(struct tab_header *self)
{
    struct list_node *pos;
    list_for_each_r_macro(&self->pages, pos) {
        tab_header_repaint_unit(self, *(struct tab_page **)pos->data);
    }
}

void tab_header_show(struct tab_header *self)
{
}

struct tab_header_unit *tab_header_find_unit(
        struct tab_header *self,
        struct point *cursor_pos)
{
}

void tab_header_set_focus(
        struct tab_header *self,
        struct tab_header_unit *new_focus)
{
}
