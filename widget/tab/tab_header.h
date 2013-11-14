#ifndef TAB_HEADER_H

#define TAB_HEADER_H

typedef void (*tab_header_callback_t)(struct tab_page *new_focus);

struct tab_header_unit {
    struct tab_page *page;
    ui_t width; /* Need to be updated when font in @sample changes */
    ui_t height;
    ui_t left_border; /* e.g. /^^^x^^x^^^^\ <- start of this unit,
                        then left_border=sum(width) + intv*(n+1)*/
};

struct tab_header {
    WIDGET_DEFINITION
    struct list units; /* list of (struct tab_header_unit *) */
    tab_header_callback_t callback_to_tab;
    struct label *sample; /* All header units use the same font, and
                            it changes with label widget. */
    struct tab_header_unit *focus;
};

#endif
