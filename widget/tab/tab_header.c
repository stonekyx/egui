#include "comm.h"
#include "base_type.h"

#include "tab_header.h"

#define TAB_HEADER_UNIT_INTERVAL 13

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

static void tab_header_paint_unit(
        struct tab_header *self,
        const struct tab_header_unit *unit,
        const struct rectangle *abs_area)
{
    set_color(self->gd, 0, 0, 0, 0);
    draw_line(self->gd,
            unit->left_border + abs_area->x
                -TAB_HEADER_LEFT_PADDING-TAB_HEADER_UNIT_INTERVAL,
            abs_area->y + abs_area->height - 1,
            unit->left_border + abs_area->x
                -TAB_HEADER_LEFT_PADDING,
            abs_area->y);
    draw_line(self->gd,
            unit->left_border + abs_area->x
                -TAB_HEADER_LEFT_PADDING,
            abs_area->y,
            unit->left_border + abs_area->x + unit->width
                +TAB_HEADER_LEFT_PADDING,
            abs_area->y);
    draw_line(self->gd,
            unit->left_border + abs_area->x + unit->width
                +TAB_HEADER_LEFT_PADDING,
            abs_area->y,
            unit->left_border + abs_area->x + unit->width
                +TAB_HEADER_LEFT_PADDING+TAB_HEADER_UNIT_INTERVAL,
            abs_area->y + abs_area->height - 1);
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
    addr->width = TAB_HEADER_UNIT_INTERVAL + TAB_HEADER_LEFT_PADDING;

    addr->name = "struct tab_header";
    addr->callback = tab_header_default_callback;
    list_init(&addr->pages);
    addr->sample = label_init("sample_for_tab_header");

    addr->height = get_font_height(addr->sample->gd) +
        2*TAB_HEADER_TOP_PADDING + 1;

    return addr;
}

si_t tab_header_exit(struct tab_header *t)
{
    label_exit(t->sample);
    list_exit(t->pages);
    return widget_exit(WIDGET_POINTER(t));
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
        .width = get_font_width(self->sample->gd)*
            strlen(page_ptr->page_head),
        .height = self->height - 2*TAB_HEADER_TOP_PADDING - 1,
        .left_border = self->width
    };
    list_push_back(&self->pages, &tmp, sizeof(struct tab_header_unit));
    self->width += tmp.width +
        TAB_HEADER_UNIT_INTERVAL + TAB_HEADER_LEFT_PADDING;
}

void tab_header_repaint(struct tab_header *self)
{
    struct list_node *pos;
    struct rectangle abs_area;
    widget_absolute_area(WIDGET_POINTER(self), &abs_area);
    list_for_each_r_macro(&self->pages, pos) {
        if(pos == self->focus) {
            continue;
        }
        tab_header_repaint_unit(self,
                (struct tab_header_unit *)pos->data, &abs_area);
    }
    tab_header_repaint_unit(self, self->focus, &abs_area);
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
