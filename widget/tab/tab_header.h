#ifndef TAB_HEADER_H

#define TAB_HEADER_H

#include "../widget/widget.h"
#include "tab_page.h"

struct tab_header_unit {
    struct tab_page *page;
    ui_t width; /* Need to be updated when font in @sample changes */
    ui_t left_border; /* e.g. /^^^x^^x^^^^\ <- start of this unit,
                        then left_border=sum(width) + intv*(n+1)*/
};

struct tab_header {
    WIDGET_DEFINITION
    struct list units; /* list of (struct tab_header_unit) */
    struct label *sample; /* All header units use the same font, and
                            it changes with label widget. */
    struct tab_header_unit *focus;
    struct tab *parent_tab;
};

struct tab_header *tab_header_init(void);

si_t tab_header_exit(struct tab_header *t);

void tab_header_add_page(
        struct tab_header *self,
        struct tab_page *page_ptr);

void tab_header_repaint(struct tab_header *self);

void tab_header_show(struct tab_header *self);

struct tab_header_unit *tab_header_find_unit(
        const struct tab_header *self,
        const struct point *cursor_pos);

void tab_header_set_focus(
        struct tab_header *self,
        struct tab_header_unit *new_focus);

void tab_header_set_font(
        struct tab_header *self,
        si_t font);

#endif
