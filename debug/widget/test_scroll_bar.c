# include <string.h>
# include <unistd.h>
# include <stdio.h>

# include "geometry.h"
# include "comm.h"
# include "application.h"
# include "widget.h"
# include "client_lib.h"


static void label_handler(struct widget* subscriber, struct widget* publisher, si_t event)
{
    struct scroll_bar* s = (struct scroll_bar*)publisher;
    struct label* l = (struct label*)subscriber;
    sprintf(l->text, "e:%3ld, %3ld, s:%3ld, %3ld",  s->elevator_offset, s->elevator_height, s->total_len, s->cur_offset);
    label_repaint(l);
    label_show(l);
}

int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;
    struct window * w;
    struct scroll_bar* s;
    struct label* l;
    char show[25];

    /* 初始化用户应用程序 */
    application_init(video_access_mode, app_type, "scroll_bar");

    /* 申请窗口 */
    w = window_init("window with scroll_bar");
    /* 申请失败 */
    if(w == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w, 300, 100, 448, 200);

    s = scroll_bar_init(1, 400, 20);
    /* 申请失败 */
    if(s == NULL)
    {
        application_exit();
        return -1;
    }
	scroll_bar_set_bounds(s, 428, 0, 20, 200);
 
    l = label_init(show);
    /* 申请失败 */
    if(l == NULL)
    {
        application_exit();
        return -1;
    }
	label_set_bounds(l, 10, 40, 400, 20);
    sprintf(show, "not started");

    scroll_bar_register_move_handler(s, WIDGET_POINTER(l), SCROLL_BAR_EVENT_ALL, label_handler);
   
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(s));
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(l));

    /* 添加顶层窗口 */
    application_add_window(NULL, w);
    /* 设置主窗口 */
    application_set_main_window(w);

    /* 运行 */
    application_exec();

    return 0;
}
