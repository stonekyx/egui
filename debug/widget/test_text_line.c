
# include <stdio.h>
# include <string.h>
# include <unistd.h>

# include "widget.h"
# include "application.h"

int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;
    struct window * w;
    struct text_line * ta;
    char lyrics[]= "The boy with the thorn in his side\n"
		"Behind the hatred there lies\n"
		"A murderous desire for love\n"
		"\n"
		"How can they look into my eyes\n"
		"And still they don't believe me\n"
		"How can they hear me say those words\n"
		"And still they don't believe me\n"
		"And if they don't believe me now\n"
		"Will they ever believe me?\n"
		"And if they don't believe me now\n"
		"Will they ever believe me?\n"
		"\n"
		"The boy with the thorn in his side\n"
		"Behind the hatred there lies\n"
		"A plundering desire for love\n";

    char* buf;

    /* 初始化用户应用程序 */
    application_init(video_access_mode, app_type, "text_line");

    /* 申请窗口 */
    w = window_init("window with text_line");
    /* 申请失败 */
    if(w == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w, 300, 100, 548, 300);

    ta = text_line_init(2 * sizeof(lyrics), 0);
    /* 申请失败 */
    if(ta == NULL)
    {
        application_exit();

        return -1;
    }
	text_line_set_bounds(ta, 5, 5, 538, 290);
    text_line_set_multilines(ta);
    buf = text_line_get_buf(ta);
    strncpy(buf, lyrics, sizeof(lyrics));

    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(ta));

    /* 添加顶层窗口 */
    application_add_window(NULL, w);
    /* 设置主窗口 */
    application_set_main_window(w);

    /* 运行 */
    application_exec();

    return 0;
}
