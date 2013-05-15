# include <string.h>
# include <unistd.h>
# include <stdio.h>

# include "application.h"
# include "widget.h"

int main()
{
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;
    struct window * w = NULL;
	struct image_view* i = NULL;

    /* 初始化用户应用程序 */
    application_init(video_access_mode, app_type, "test_image_view");

    /* 申请窗口 */
    w = window_init("window with image_view");
    /* 申请失败 */
    if(w == NULL)
    {
        application_exit();
        return -1;
    }
	window_set_bounds(w, 300, 100, 520, 520);
  
	i = image_view_init("/home/sisi/dst/debug/widget/file.bmp");
	if(NULL == i)
	{
		application_exit();
		return -1;
	}
	image_view_set_bounds(i, 10, 10, 500, 500);

	/* 添加子控件 */
    object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(i));

    /* 添加顶层窗口 */
    application_add_window(NULL, w);
    /* 设置主窗口 */
    application_set_main_window(w);

    /* 运行 */
    application_exec();

    return 0;
}
