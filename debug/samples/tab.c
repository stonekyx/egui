#include <stdio.h>

#include "application.h"
#include "widget.h"

int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
    si_t app_type = APPLICATION_TYPE_NORMAL;

    struct window *w1, *w2;
    struct tab *t;

    application_init(video_access_mode, app_type, "tab sample");

    if((w1 = window_init("main window"))==NULL) {
        application_exit();
        return -1;
    }
    window_set_bounds(w1, 300, 100, 448, 200);

    if((w2 = window_init("vice window"))==NULL) {
        application_exit();
        return -1;
    }
    window_set_bounds(w2, 400, 200, 448, 200);
    {
        struct color win_back = {.r=0xD8, .g=0xA8, .b=0xC0, .a=0};
        struct color win_fore = {.r=0x30, .g=0x30, .b=0x48, .a=0};
        window_set_color(w2, &win_fore, &win_back);
    }

    t = tab_init();
    tab_set_bounds(t, 5, 5, 440, 190);
    object_attach_child(OBJECT_POINTER(w2), OBJECT_POINTER(t));
    {
        struct tab_page *tp1 = tab_page_init("test page 1");
        struct tab_page *tp2 = tab_page_init("page 2");
        struct tab_page *tp3 = tab_page_init("p3");
        struct tab_page *tp4 = tab_page_init("by_stone");
        struct button *b1 = button_init("test button");
        struct checkbox *cb = checkbox_init(1);
        struct spinbox *spb = spinbox_init(INT_MIN, INT_MAX, 10);
        button_set_bounds(b1, 10, 24, -1, -1);
        button_set_bounds_auto(b1);
        checkbox_set_bounds(cb, 0, 0, -1, -1);
        spinbox_set_bounds(spb, 0, 1, 100, 50);
        object_attach_child(OBJECT_POINTER(tp1), OBJECT_POINTER(b1));
        object_attach_child(OBJECT_POINTER(tp2), OBJECT_POINTER(cb));
        object_attach_child(OBJECT_POINTER(tp3), OBJECT_POINTER(spb));
        tab_add_page(t, tp1);
        tab_add_page(t, tp2);
        tab_add_page(t, tp3);
        tab_add_page(t, tp4);
        {
            struct color tc1 = {.r=0xc0, .g=0x60, .b=0x90, .a=0};
            struct color tc2 = {.r=0xa8, .g=0xa8, .b=0xc0, .a=0};
            struct color tc3 = {.r=0x78, .g=0x78, .b=0xa8, .a=0};
            struct color tc4 = {.r=0x48, .g=0x48, .b=0x78, .a=0};
            struct color bbc = {.r=0xd8, .g=0x90, .b=0xa8, .a=0};
            struct color bfc = {.r=0xff, .g=0xff, .b=0xff, .a=0};
            tab_page_set_color(tp1, NULL, &tc1);
            tab_page_set_color(tp2, NULL, &tc2);
            tab_page_set_color(tp3, NULL, &tc3);
            tab_page_set_color(tp4, NULL, &tc4);
            button_set_color(b1, &bfc, &bbc);
        }
    }

    application_add_window(NULL, w1);
    application_add_window(NULL, w2);
    application_set_main_window(w1);

    application_exec();
    return 0;
}

