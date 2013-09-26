#include <log.h>

#include "tab_page.h"

struct tab_page *tab_page_init(const char *title)
{
    struct tab_page *addr = calloc(1, sizeof(struct tab_page));
    if(!addr) {
        EGUI_PRINT_SYS_ERROR("malloc failed");
        return NULL;
    }
    addr->page_head = button_init(title);
    list_init(&addr->widgets);
    return addr;
}

si_t tab_page_exit(struct tab_page *b)
{
    list_exit(&b->widgets);
    button_exit(b->page_head);
    free(b);
}

void tab_page_set_color(struct tab_page *p, struct color *fore, struct color *back)
{
    button_set_color(p->page_head, fore, back);
}

void tab_page_set_font(struct tab_page *p, si_t font)
{
    button_set_font(p->page_head, font);
}
