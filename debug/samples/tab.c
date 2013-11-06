#include <stdio.h>

#include "application.h"

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
        struct color blue = {.r=100, .g=100, .b=255, .a=0};
        struct color black = {.r=0, .g=0, .b=0, .a=0};
        button_set_bounds(b1, 10, 24, -1, -1);
        button_set_bounds_auto(b1);
        checkbox_set_bounds(cb, 0, 0, -1, -1);
        spinbox_set_bounds(spb, 0, 1, 100, 200);
        tab_page_set_color(tp2, &black, &blue);
        object_attach_child(OBJECT_POINTER(tp1), OBJECT_POINTER(b1));
        object_attach_child(OBJECT_POINTER(tp2), OBJECT_POINTER(cb));
        object_attach_child(OBJECT_POINTER(tp3), OBJECT_POINTER(spb));
        tab_add_page(t, tp1);
        tab_add_page(t, tp2);
        tab_add_page(t, tp3);
        tab_add_page(t, tp4);
    }

    application_add_window(NULL, w1);
    application_add_window(NULL, w2);
    application_set_main_window(w1);

    application_exec();
    return 0;
}

