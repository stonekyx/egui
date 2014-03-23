#include "comm.h"
#include "base_type.h"
#include "client_lib.h"
#include "log.h"

#include "tab.h"
#include "tab_header.h"
#include "../label/label.h"

#define TAB_HEADER_UNIT_INTERVAL 13
#define TAB_HEADER_LEFT_PADDING 2
#define TAB_HEADER_TOP_PADDING 2

static si_t tab_header_default_callback(addr_t s, addr_t m)
{
    struct tab_header *self = s;
    union message *msg = m;
    switch(msg->base.type) {
        case MESSAGE_TYPE_MOUSE_PRESS:
            tab_header_set_focus(
                    self,
                    tab_header_find_unit(
                        self,
                        &msg->base.cursor_position));
            /* Fall through */
        case MESSAGE_TYPE_WIDGET_REPAINT:
            tab_header_repaint(self);
            /* Fall through */
        case MESSAGE_TYPE_WIDGET_SHOW:
            tab_header_show(self);
            break;
        default:
            break;
    }
    return 0;
}

static void tab_header_repaint_unit(
        struct tab_header *self,
        const struct tab_header_unit *unit,
        const struct point *p,
        const struct color *c)
{
    struct point pnts[]={
        {
            .x= unit->left_border + p->x
                -TAB_HEADER_LEFT_PADDING-TAB_HEADER_UNIT_INTERVAL,
            .y= p->y + self->area.height - 1
        },
        {
            .x= unit->left_border + p->x
                -TAB_HEADER_LEFT_PADDING,
            .y= p->y
        },
        {
            .x= unit->left_border + p->x + unit->width-1
                +TAB_HEADER_LEFT_PADDING,
            .y= p->y
        },
        {
            .x= unit->left_border + p->x + unit->width-1
                +TAB_HEADER_LEFT_PADDING+TAB_HEADER_UNIT_INTERVAL,
            .y= p->y + self->area.height - 1
        },
    };
    int i;
    if(!c) {
        c = &WIDGET_POINTER(unit->page)->back_color;
    }
    set_color(self->gd, c->r, c->g, c->b, c->a);
    fill_polygon(self->gd, pnts, 4);
    set_color(self->gd, 0, 0, 0, 0);
    for(i=0; i<3; i++) {
        draw_line(self->gd, pnts[i].x, pnts[i].y, pnts[i+1].x, pnts[i+1].y);
    }
    show_text(self->gd,
            p->x + unit->left_border,
            p->y + TAB_HEADER_TOP_PADDING,
            unit->page->page_head,
            strlen(unit->page->page_head));
}

static int tab_header_point_in_unit(const struct tab_header *self,
        const struct point *p,
        const struct tab_header_unit *u)
{
    if(p->y<0 || p->y>=self->area.height ||
            p->x<(si_t)u->left_border
            -TAB_HEADER_LEFT_PADDING-TAB_HEADER_UNIT_INTERVAL ||
            p->x>=(si_t)u->left_border+(si_t)u->width-1
            +TAB_HEADER_LEFT_PADDING+TAB_HEADER_UNIT_INTERVAL) {
        return 0;
    }
    if(p->x>=(si_t)u->left_border-TAB_HEADER_LEFT_PADDING &&
            p->x<(si_t)u->left_border+(si_t)u->width
            +TAB_HEADER_LEFT_PADDING) {
        return 1;
    }
    if(p->x<(si_t)u->left_border-TAB_HEADER_LEFT_PADDING) { /* left triangle */
        si_t tx = p->x - (u->left_border
            -TAB_HEADER_LEFT_PADDING-TAB_HEADER_UNIT_INTERVAL);
        /* y+1    x_t - x
         * --- >= ------- , triangle similarity ratio
         * y_t       x_t
         */
        return TAB_HEADER_UNIT_INTERVAL*(p->y+1) >=
            self->area.height*(TAB_HEADER_UNIT_INTERVAL-tx);
    } else { /* right triangle */
        si_t tx = p->x -
            (u->left_border+u->width-1+TAB_HEADER_LEFT_PADDING);
        /* y+1    x+1
         * --- >= ---
         * y_t    x_t
         */
        return TAB_HEADER_UNIT_INTERVAL*(p->y+1) >=
            self->area.height*(tx+1);
    }
}

static void object_disconnect(struct object *o)
{
    if(NULL==o || NULL==o->parent) {
        return;
    }
    if(o->parent->lchild == o) {
        o->parent->lchild = NULL;
    } else if(o->parent->rchild == o) {
        o->parent->rchild = NULL;
    }
    o->parent = NULL;
}

/* ---------------------static above---------------------- */

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
    addr->area.width = TAB_HEADER_UNIT_INTERVAL + TAB_HEADER_LEFT_PADDING;

    addr->name = "struct tab_header";
    addr->callback = tab_header_default_callback;
    list_init(&addr->units);
    addr->sample = label_init("sample_for_tab_header");

    addr->area.height = get_font_height(addr->sample->gd) +
        2*TAB_HEADER_TOP_PADDING + 1;

    return addr;
}

si_t tab_header_exit(struct tab_header *t)
{
    label_exit(t->sample);
    list_exit(&t->units);
    return widget_exit(WIDGET_POINTER(t));
}

void tab_header_add_page(
        struct tab_header *self,
        struct tab_page *page_ptr)
{
    struct tab_header_unit tmp = {
        .page = page_ptr,
        .width = get_font_width(self->sample->gd)*
            strlen(page_ptr->page_head),
        .left_border = self->area.width
    };
    list_push_back(&self->units, &tmp, sizeof(struct tab_header_unit));
    self->area.width += tmp.width +
        TAB_HEADER_UNIT_INTERVAL + TAB_HEADER_LEFT_PADDING;
    if(list_size(&self->units)==1) {
        tab_header_set_focus(self, list_back(&self->units));
    }
}

void tab_header_repaint(struct tab_header *self)
{
    struct list_node *pos;
    struct rectangle abs_area;
    struct point abs_coor;

    widget_absolute_coordinate(WIDGET_POINTER(self),
            &abs_coor.x, &abs_coor.y);
    application_widget_absolute_area(WIDGET_POINTER(self), &abs_area);
    set_area(self->gd, abs_area.x, abs_area.y,
            abs_area.width, abs_area.height);

    list_for_each_r_macro(&self->units, pos) {
        struct color c;
        struct tab_header_unit *i = pos->data;
        if(i == self->focus) {
            continue;
        }
        c = WIDGET_POINTER(i->page)->back_color;
        c.r *= 0.5;
        c.g *= 0.5;
        c.b *= 0.5;
        tab_header_repaint_unit(self, i, &abs_coor, &c);
    }
    if(self->focus) {
        tab_header_repaint_unit(self, self->focus, &abs_coor, NULL);
    }
}

void tab_header_show(struct tab_header *self)
{
    struct rectangle area;
    application_widget_absolute_area(WIDGET_POINTER(self), &area);
    set_area(self->gd, area.x, area.y, area.width, area.height);
    update(self->gd);
}

struct tab_header_unit *tab_header_find_unit(
        const struct tab_header *self,
        const struct point *cp)
{
    struct list_node *pos;
    struct point cursor_pos = *cp;
    struct point abs_coor;
    widget_absolute_coordinate(WIDGET_POINTER(self),
            &abs_coor.x, &abs_coor.y);
    cursor_pos.x -= abs_coor.x;
    cursor_pos.y -= abs_coor.y;
    if(self->focus &&
            tab_header_point_in_unit(self, &cursor_pos, self->focus)) {
        return self->focus;
    }
    list_for_each_macro(&self->units, pos) {
        if(pos->data == self->focus) {
            continue;
        }
        if(tab_header_point_in_unit(self, &cursor_pos, pos->data)) {
            return pos->data;
        }
    }
    return NULL;
}

void tab_header_set_focus(
        struct tab_header *self,
        struct tab_header_unit *new_focus)
{
    if(!new_focus) {
        return;
    }
    self->focus = new_focus;
    if(self->parent_tab) {
        object_disconnect(OBJECT_POINTER(self->parent_tab->focus));
        self->parent_tab->focus = new_focus->page;
        panel_set_bounds(PANEL_POINTER(new_focus->page),
                0, self->area.height, -1, -1);
        object_attach_child(OBJECT_POINTER(self->parent_tab),
                OBJECT_POINTER(new_focus->page));
        panel_repaint_with_children(PANEL_POINTER(new_focus->page));
        panel_show(PANEL_POINTER(new_focus->page));
    }
}

void tab_header_set_font(
        struct tab_header *self,
        si_t font)
{
    label_set_font(self->sample, font);
}
