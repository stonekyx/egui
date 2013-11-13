#ifndef TAB_HEADER_H

#define TAB_HEADER_H

typedef void (*tab_header_callback_t)(struct tab_page *new_focus);

struct tab_header_unit {
    struct tab_page *page;
    ui_t width;
    ui_t height;
};

struct tab_header {
    WIDGET_DEFINITION
    struct list units; /* list of (struct tab_header_unit *) */
    tab_header_callback_t callback_to_tab;
};

#endif
