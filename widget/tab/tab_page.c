#include <stdlib.h>

#include <log.h>

#include "tab_page.h"

struct tab_page *tab_page_init(const char *title)
{
    struct tab_page *addr = calloc(1, sizeof(struct tab_page));
    if(!addr) {
        EGUI_PRINT_SYS_ERROR("malloc failed");
        return NULL;
    }
    if(!(addr=tab_page_init_data(addr, title))) {
        return NULL;
    }
    return addr;
}

struct tab_page *tab_page_init_data(
        struct tab_page *addr,
        const char *title)
{
    if(panel_init_data(&addr->base, 0) != &addr->base) {
        return NULL;
    }
    addr->page_head = strdup(title);
    return addr;
}

si_t tab_page_exit(struct tab_page *b)
{
    free(b->page_head);
    panel_exit(&b->base);
    free(b);
    return 0;
}

void tab_page_set_color(struct tab_page *p, struct color *fore, struct color *back)
{
    panel_set_color(&p->base, fore, back);
}
