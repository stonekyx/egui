#include <stdio.h>

#include "application.h"
#include "widget.h"

int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;
    struct window * w;
    struct tab_header *t;
    application_init(video_access_mode, app_type, "multi_windows");
    w = window_init("main window");
    if(w == NULL) {
        application_exit();
        return -1;
    }
	window_set_bounds(w, 300, 100, 448, 200);
    application_add_window(NULL, w);
    application_set_main_window(w);

    t = tab_header_init();
    {
        struct tab_page *p1 = tab_page_init("test1"),
                        *p2 = tab_page_init("test page 2"),
                        *p3 = tab_page_init("page_3");
        struct color blue = {.r=0x1A, .g=0x7B, .b=0xBD, .a=0};
        tab_header_add_page(t, p1);
        tab_header_add_page(t, p2);
        tab_header_add_page(t, p3);
        tab_page_set_color(p1, NULL, &blue);
    }
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(t));

    application_exec();

    return 0;
}

