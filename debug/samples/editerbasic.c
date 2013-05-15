#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "widget.h"
#include "application.h"
#include "client_lib.h"
#include "log.h"

#define LABEL_MAX 128
#define FILE_MAX  4096

static struct color barely_blue = {0xcb, 0xf3, 0xfb, 0};
static struct color dark_blue = {0x18, 0x29, 0x45, 0};
static struct color light_green = {0xcc, 0xff, 0x99, 0};
static struct color light_blue = {0x46, 0xa5, 0xe5, 0};
#if 0
static struct color light_yellow = {0xff, 0xff, 0xcc, 0};
#endif

struct text_line* file_context_text_line = NULL;
struct label* log_label = NULL;
struct label* file_label = NULL;
struct window* main_window = NULL;
struct window* save_window = NULL;
struct text_line* save_text_line = NULL;

void text_line_subscribe_scrollbar(struct widget* subscriber, struct widget* pulisher, si_t event)
{
    struct text_line* t = TEXT_LINE_POINTER(subscriber);
    struct scroll_bar* s = SCROLL_BAR_POINTER(pulisher);
    si_t lineno_cur = text_line_get_line_cur(t);
    si_t lines_per_page = text_line_get_max_line_shown(t);

    switch(event)
    {
    /**
     * press up arrow
     **/
    case SCROLL_BAR_EVENT_LINE_UP:
        text_line_update_ruler_line(t, lineno_cur - 1);
        break;

    /**
     * press down arow
     **/
    case SCROLL_BAR_EVENT_LINE_DOWN:
        text_line_update_ruler_line(t, lineno_cur + 1);
        break;

    /**
     * press spaces above elevator
     **/
    case SCROLL_BAR_EVENT_PAGE_UP:
        text_line_update_ruler_line(t, lineno_cur - lines_per_page);
        break;

    /**
     * press spaces below elevator
     **/
    case SCROLL_BAR_EVENT_PAGE_DOWN:
        text_line_update_ruler_line(t, lineno_cur + lines_per_page);
        break;

    /**
     * move elevator
     **/
    case SCROLL_BAR_EVENT_CHANGE:
        text_line_update_ruler_line(t, s->cur_offset / s->line_height);
        break;

    default:
        return;
        break;
    }
    text_line_repaint(t);
    text_line_show(t);
}


void scrollbar_subscribe_text_line(struct widget* subscriber, struct widget* pulisher, si_t event)
{
    struct scroll_bar* s = SCROLL_BAR_POINTER(subscriber);

    switch(event)
    {
    /**
     * page start offset move up
     **/
    case TEXT_LINE_EVENT_PAGE_UP:
        scroll_bar_update_offset(s, s->cur_offset - s->line_height);
        break;

    /**
     * page start offset move down
     **/
    case TEXT_LINE_EVENT_PAGE_DOWN:
        scroll_bar_update_offset(s, s->cur_offset + s->line_height);
        break;

    /**
     * page start offset changed
     **/
    case TEXT_LINE_EVENT_PAGE_CHANGE:
        return;
        break;

    /**
     * current ruler changed
     **/
    case TEXT_LINE_EVENT_CURRENT_CHANGE:
        return;
        break;

    default:
        return;
        break;
    }
    scroll_bar_repaint(s);
    scroll_bar_show(s);
}

static si_t save_file(char* file_name, char* buf)
{
	si_t fd = -1;
	if((fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1)
	{
		EGUI_PRINT_SYS_ERROR("failed to open %s. open:", file_name);
		return -1;
	}
	if(write(fd, buf, strlen(buf)) == -1)
	{
		EGUI_PRINT_SYS_ERROR("failed to write %s. write:", file_name);
		return -1;
	}
	if(close(fd) == -1)
	{
		EGUI_PRINT_SYS_ERROR("failed to close %s. close:", file_name);
		return -1;
	}
	return 0;
}

si_t save_button_callback(void* btn, void* msg)
{
    union message* m = (union message*)msg;
    switch(m->base.type)
    {
    case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
		if(0 == save_file(file_label->text, text_line_get_buf(file_context_text_line)))
		{
    		sprintf(log_label->text, "save successfully!");
		}
		else
		{
			sprintf(log_label->text, "failed to save file!");
		}

        break;
    default:
        button_default_callback(btn, msg);
        return 0;
        break;
    }
    label_repaint(log_label);
    label_show(log_label);
    return 0;
}

si_t save_window_ok_button_callback(void* btn, void* msg)
{
    union message* m = (union message*)msg;
	switch(m->base.type)
	{
    case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
		if(0 == save_file(text_line_get_buf(save_text_line), text_line_get_buf(file_context_text_line)))
		{
			sprintf(log_label->text, "save successfully!");
		}
		else
		{
			sprintf(log_label->text, "failed to save file!");
		}
		application_del_window(save_window);
		sprintf(file_label->text, "%s", text_line_get_buf(save_text_line));
		save_window = NULL;
		save_text_line = NULL;
		break;
    default:
        button_default_callback(btn, msg);
        return 0;
        break;
	}
    label_repaint(log_label);
    label_show(log_label);
    return 0;
}

si_t save_as_button_callback(void* btn, void* msg)
{
    union message* m = (union message*)msg;
	struct button* b = NULL;
	switch(m->base.type)
	{
	case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
		if(NULL == save_window)
		{
			save_window = window_init("save window");
            /* 申请失败 */
            if(NULL == main_window)
            {
				EGUI_PRINT_ERROR("failed to init save window");
				application_exit();
                return -1;
            }
			window_set_bounds(save_window, main_window->area.x + 30, main_window->area.y + 30, 300, 60);
			window_set_color(save_window, NULL, &barely_blue);

			save_text_line = text_line_init(FILE_MAX, 0);
			if(NULL == save_text_line)
			{
				EGUI_PRINT_ERROR("failed to init save_text_line on save window");
				application_exit();
				return -1;
			}
			text_line_set_bounds(save_text_line, 5, 5, 230, 50);
			text_line_set_placeholder(save_text_line, "input file name");
			text_line_set_color(save_text_line, &dark_blue, NULL, &light_green);

			b = button_init("ok");
			if(NULL == b)
			{
				EGUI_PRINT_ERROR("failed to init ok button on save window");
				application_exit();
				return -1;
			}
			button_set_bounds(b, 240, 10, 50, 40);
			button_set_color(b, &dark_blue, &light_green);
			button_set_font(b, FONT_MATRIX_12);
			b->callback = save_window_ok_button_callback;

			object_attach_child(OBJECT_POINTER(save_window), OBJECT_POINTER(b));
			object_attach_child(OBJECT_POINTER(save_window), OBJECT_POINTER(save_text_line));
			application_add_window(NULL, save_window);
			break;
		}
		else
		{
			EGUI_PRINT_ERROR("save window already open!");
		}
	default:
		button_default_callback(btn, msg);
	}
	return 0;
}

static si_t load_file(char* file_name, char* buf)
{
	si_t fd = -1;
	if((fd = open(file_name, O_RDONLY)) == -1)
	{
		EGUI_PRINT_SYS_ERROR("failed to open %s. open:", file_name);
		return -1;
	}
	if(read(fd, buf, FILE_MAX) == -1)
	{
		EGUI_PRINT_SYS_ERROR("failed to read %s. read: ", file_name);
		return -1;
	}
	if(close(fd) == -1)
	{
		EGUI_PRINT_SYS_ERROR("failed to close %s. close: ", file_name);
		return -1;
	} 
	return 0;
}

int main(int argc, char* argv[])
{
    struct scroll_bar* s;
    struct button* b1 = NULL, *b2 = NULL;
    char msg[LABEL_MAX];
	char file_msg[LABEL_MAX];
    si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;
	char* file_name = NULL;
	si_t fonty = 0;

    /**
     * open with file
     **/
    if(argc == 2)
	{
		struct stat st;
		if(stat(argv[1], &st) == -1)
		{
			EGUI_PRINT_ERROR("file %s does not exists", argv[1]);
			return -1;
		}

		if(!S_ISREG(st.st_mode) || access(argv[1], R_OK | W_OK) != 0)
		{
			EGUI_PRINT_ERROR("file %s cannot access!", argv[1]);
			return -1;
		}

		/**
		 * file too large
		 **/
		if(st.st_size > FILE_MAX)
		{
			EGUI_PRINT_ERROR("file %s does not exists\n", argv[1]);
			return -1;
		}
		file_name = argv[1];
    }
    else if(argc != 1)
    {
		EGUI_PRINT_ERROR("wrong parameter!\nusage: \nediterbasic\nor:\nediterbasic [filename]");
		return -1;
    }


    /* 初始化用户应用程序 */
    application_init(video_access_mode, app_type, "editerbasic");

    /**
     * window
     **/
    main_window = window_init("editer_basic");
    /* 申请失败 */
    if(main_window == NULL)
    {
        application_exit();
        return -1;
    }
    window_set_bounds(main_window, 300, 100, 550, 300);
	window_set_color(main_window, NULL, &light_green);

    /**
     * label that show process
     **/
	if(file_name)
	{
		sprintf(file_msg, "%s", file_name);
	}
	else
	{
		sprintf(file_msg, "new file");
	}
	file_label = label_init(file_msg);
    if(file_label == NULL)
    {
        application_exit();
        return -1;
    }
	label_set_bounds(file_label, 10, 10, 365, 20);
	label_set_color(file_label, &barely_blue, &light_blue);
	label_set_font(file_label, FONT_MATRIX_12);
 
    /**
     * save button
     **/
    b1 = button_init("save");
    if(b1 == NULL)
    {
        application_exit();
        return -1;
    }
	button_set_bounds(b1, 380, 5, 60, 30);
	button_set_color(b1, &dark_blue, &barely_blue);
	button_set_font(b1, FONT_MATRIX_12);
	b1->callback = save_button_callback;

	b2 = button_init("save as");
	if(b2 == NULL)
	{
		application_exit();
		return -1;
	}
	button_set_bounds(b2, 445, 5, 100, 30);
	button_set_color(b2, &dark_blue, &barely_blue);
	button_set_font(b2, FONT_MATRIX_12);
	b2->callback = save_as_button_callback;

    /**
     * text_line that shows file context
     **/
	file_context_text_line = text_line_init(FILE_MAX, 0);
    if(file_context_text_line == NULL)
    {
        application_exit();
        return -1;
    }
	text_line_set_bounds(file_context_text_line, 5, 40, 520, 230);
	text_line_set_color(file_context_text_line, &dark_blue, NULL, NULL);
	text_line_set_multilines(file_context_text_line);
	fonty = get_font_height(file_context_text_line->gd);

	if(file_name)
	{
		if(load_file(file_name, text_line_get_buf(file_context_text_line)) < 0)
		{
			EGUI_PRINT_ERROR("failed to load file %s", file_name);
			application_exit();
			return -1;
		}
		s = scroll_bar_init(1, fonty * text_line_get_max_line_shown(file_context_text_line), fonty);
	}
	else
	{
		s = scroll_bar_init(1, file_context_text_line->area.height, fonty);
	}
    if(s == NULL)
    {
        application_exit();
        return -1;
    }
	scroll_bar_set_bounds(s, 525, 40, 20, 230);

	text_line_register_move_handler(file_context_text_line, WIDGET_POINTER(s), TEXT_LINE_EVENT_ALL, scrollbar_subscribe_text_line);
	scroll_bar_register_move_handler(s, WIDGET_POINTER(file_context_text_line), SCROLL_BAR_EVENT_ALL, text_line_subscribe_scrollbar);

	log_label = label_init(msg);
    if(log_label == NULL)
    {
        application_exit();
        return -1;
    }
	label_set_bounds(log_label, 5, 275, 450, 20);
	label_set_color(log_label, NULL, &light_green);
	sprintf(msg, "open file successfully!");
 
    object_attach_child(OBJECT_POINTER(main_window), OBJECT_POINTER(file_label));
    object_attach_child(OBJECT_POINTER(main_window), OBJECT_POINTER(file_context_text_line));
    object_attach_child(OBJECT_POINTER(main_window), OBJECT_POINTER(log_label));
    object_attach_child(OBJECT_POINTER(main_window), OBJECT_POINTER(b1));
    object_attach_child(OBJECT_POINTER(main_window), OBJECT_POINTER(b2));
    object_attach_child(OBJECT_POINTER(main_window), OBJECT_POINTER(s));

    /* 添加顶层窗口 */
    application_add_window(NULL, main_window);
    /* 设置主窗口 */
    application_set_main_window(main_window);

    /* 运行 */
    application_exec();

    return 0;
}
