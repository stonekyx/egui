#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

#include "comm.h"
#include "client_lib.h"
#include "widget.h"
#include "application.h"
#include "log.h"

#define LABEL_MAX 128
#define FILE_MAX  4096
static struct color barely_blue = {0xcb, 0xf3, 0xfb, 0};
static struct color dark_blue = {0x18, 0x29, 0x45, 0};
static struct color light_blue = {0x46, 0xa5, 0xe5, 0};
static struct color light_green = {0xcc, 0xff, 0x99, 0};
#if 0 /** commented by padme 2012-05-20 {{{ **/
static struct color light_yellow = {0xff, 0xff, 0xcc, 0};
#endif /** commented by padme 2012-05-20 }}} **/

/**
 * 存储文件内容和文件名的buffer
 **/
static char* file_buf = NULL;		/* 指向存储文件内容的main窗口的text_line的buffer */
static char* file_name_buf = NULL;	/* 指向存储文件名的main窗口的label的buffer */
static char* save_file_name_buf = NULL;	 /* 指向存储文件名的save窗口的textline的buffer */
static char* open_file_name_buf = NULL;	 /* 指向存储文件名的open窗口的textline的buffer */

void text_line_subscribe_scrollbar(struct widget* subscriber, struct widget* pulisher, int event)
{
	struct text_line* t = TEXT_LINE_POINTER(subscriber);
	struct scroll_bar* s = SCROLL_BAR_POINTER(pulisher);
	int lineno_cur = text_line_get_line_cur(t);
	int lines_per_page = text_line_get_max_line_shown(t);

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

void scrollbar_subscribe_text_line(struct widget* subscriber, struct widget* pulisher, int event)
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

si_t save_file(char* file, char* buf, char* warning_sign)
{
	si_t fd = -1;
	if((fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1)
	{
		EGUI_PRINT_SYS_ERROR("failed to open file %s. open()", file);
		sprintf(warning_sign, "error when saved: failed to open file");
		return -1;
	}
	if(write(fd, buf, strlen(buf)) == -1)
	{
		EGUI_PRINT_SYS_ERROR("failed to write buf to file %s. write()", file);
		sprintf(warning_sign, "error when saved: failed to write file");
		return -1;
	}
	if(close(fd) == -1)
	{
		EGUI_PRINT_SYS_ERROR("failed to close file %s. close()", file);
		sprintf(warning_sign, "error when saved: failed to close file");
		return -1;
	}
	sprintf(warning_sign, "save file successfully!");
	return 0;
}

si_t save_window_button_callback(void* btn, void* msg)
{
	union message* m = (union message*)msg;
	char* bufa;
	char* bufb;
	char str[] = "save successfully";
	switch(m->base.type)
	{
	case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
		save_file(text_line_get_buf(ta), text_line_get_buf(tb), l->text);

		break;
	default:
		button_default_callback(btn, msg);
		return 0;
		break;
	}
	label_repaint(l);
	label_show(l);
	return 0;
}

si_t load_file(char* file_path, char* buf,  char* warning_sign)
{
	int fd;
	if((fd = open(file_path, O_RDONLY))==-1)
	{
		EGUI_PRINT_SYS_ERROR("failed to open %s. open()", file_path);
		sprintf(warning_sign, "error when load file: failed to open");
		return -1;
	}
	if(read(fd, buf, FILE_MAX) == -1)
	{	
		EGUI_PRINT_SYS_ERROR("failed to read %s. read()", file_path);
		sprintf(warning_sign, "error when load file: failed to read");
		return -1;
	}
	if(close(fd) == -1)
	{
		EGUI_PRINT_SYS_ERROR("failed to close%s. close()", file_path);
		sprintf(warning_sign, "error when load file: failed to close");
		return -1;
	} 
	sprintf(warning_sign, "load file successfully!");
	return 0;
}

void open_file_pop_window( char* file_path )
{
	char buffer_stream[FILE_MAX];
	struct window * win;
	struct text_line *ta;
	win = window_init(66);
	win->title = "Open Files";
	win->maximize_enable = 1;
	win->minimize_enable = 1;
	window_set_bounds(win, 100, 100, 300, 100);
	win->callback = window_default_callback;
	application_add_window(NULL, win);

	ta = text_line_init(67);
	ta->fore_color = dark_blue;
	text_line_set_bufsize(ta, FILE_MAX);
	text_line_set_multilines(ta, 0);
	text_line_set_bounds(ta, 0, 0, 300, 100);	      
	object_attach_child(OBJECT_POINTER(win), OBJECT_POINTER(ta));

	/* load files */
	memset(buffer_stream, 0, FILE_MAX); 
	load_file(file_path, buffer_stream, buffer_stream);
	text_line_set_placeholder(ta, buffer_stream);
}

si_t open_window_button_callback(void * btn, void *msg )
{
	union message * m = (union message*)msg;
	switch(	m->base.type)
	{
	case MESSAGE_TYPE_MOUSE_SINGLE_CLICK:
		{
			char * file_path = text_line_get_buf(ta);
			open_file_pop_window(file_path);
			break;
		}
	default:
		button_default_callback(btn, msg);

	}

	return 0;
}

int main(int argc, char* argv[])
{
	struct window* w = NULL;
	struct label* file_name_label = NULL;
	struct label* log_label = NULL;
	struct button* save_window_button = NULL;
	struct button* open_window_button = NULL;
	struct text_line* file_context_text_line = NULL;
	struct scroll_bar * s = NULL; 
	char msg[LABEL_MAX] = {0};
	char name[LABEL_MAX] = {0};
	si_t video_access_mode = VIDEO_ACCESS_MODE_BUFFER;
	si_t app_type = APPLICATION_TYPE_NORMAL;

	/* 初始化用户应用程序 */
	application_init(video_access_mode, app_type, "editerbasic");

	/**
	 * window
	 **/
	w = window_init(0);
	if(w == NULL)
	{
		application_exit();
		return -1;
	}
	window_set_bounds(w, 300, 100, 550, 300);
	w->title = "editer_basic";
	w->back_color = light_green;

	/**
	 * label that show process
	 **/
	log_label = label_init(1);
	if(log_label == NULL)
	{
		application_exit();
		return -1;
	}
	log_label->text = msg;
	label_set_bounds(log_label, 5, 275, 450, 20);
	log_label->back_color = w->back_color;

	/**
	 * label that shows filename
	 **/
	file_name_label = label_init(2);
	if(file_name_label == NULL)
	{
		application_exit();
		return -1;
	}
	file_name_buf = file_name_label->text;
	label_set_bounds(ta, 5, 5, 410, 30);
	ta->fore_color = dark_blue;

	/**
	 * 按下会打开保存窗口的button
	 **/
	save_window_button = button_init(3);
	if(save_window_button == NULL)
	{
		application_exit();
		return -1;
	}
	button_set_bounds(save_window_button, 415, 5, 60, 30);
	save_window_button->text = "save";
	save_window_button->font = FONT_MATRIX_12;
	save_window_button->back_color = barely_blue;
	save_window_button->fore_color = dark_blue;
	save_window_button->callback = save_window_button_callback;

	/**
	 * 按下会打开打开文件窗口的button
	 **/
	open_window_button = button_init(4);
	if(open_window_button == NULL){
		application_exit();
		return -1;
	}
	button_set_bounds(open_window_button, 480, 5, 60, 30);
	open_window_button->text = "open";
	open_window_button->font = FONT_MATRIX_12;
	open_window_button->back_color = barely_blue;
	open_window_button->fore_color = dark_blue;
	open_window_button->callback = open_window_button_callback;

	/**
	 * text_line that shows file context
	 **/
	file_context_text_line = text_line_init(5);
	if(tb == NULL)
	{
		application_exit();
		return -1;
	}
	text_line_set_bounds(file_context_text_line, 5, 40, 520, 230);
	file_context_text_line->fore_color = dark_blue;

	/**
	 * scroll_bar for text_line b
	 **/
	s = scroll_bar_init(6);
	if(s == NULL)
	{
		application_exit();
		return -1;
	}
	scroll_bar_set_bounds(s, 525, 40, 20, 230);

	/**
	 * empty editer
	 **/
	if(argc == 1)
	{
		int fonty = get_font_height(file_context_text_line->gd);
		int maxshown;
		strncpy(l->text, str, sizeof(str));

		/**
		 * filename null
		 **/
		file_name_label->text = "new file";

		/**
		 * content empty
		 **/
		text_line_set_bufsize(file_name_text, FILE_MAX);
		text_line_set_placeholder(tb, "place holder");
		text_line_set_multilines(tb, 0);
		text_line_register_move_handler(tb, WIDGET_POINTER(s), TEXT_LINE_EVENT_ALL, scrollbar_subscribe_text_line);

		/**
		 * set for scroll_bar
		 **/    
#if 0 /** commented by padme 2012-05-28 {{{ **/
		maxshown = fonty * text_line_get_max_line_shown(tb);
		s->area.y += (s->area.height - maxshown)/ 2;
		s->area.height = maxshown;
#endif /** commented by padme 2012-05-28 }}} **/
		scroll_bar_set_vertical(s);
		scroll_bar_set_line_height(s, fonty);
		scroll_bar_set_view(s, maxshown, 0);
		scroll_bar_register_move_handler(s, WIDGET_POINTER(tb), SCROLL_BAR_EVENT_ALL, text_line_subscribe_scrollbar);
	}
	/**
	 * open with file
	 **/
	else if(argc == 2)
	{
		struct stat st;
		if(stat(argv[1], &st) == -1)
		{
			application_exit();
			return -1;
		}

		if(S_ISREG(st.st_mode) && access(argv[1], R_OK | W_OK) == 0)
		{
			/**
			 * file too large
			 **/
			if(st.st_size > FILE_MAX)
			{
				char str[] = "file too large";
				strncpy(l->text, str, sizeof(str));
			}
			else
			{
				char str[] = "open successfully";
				char* buf;
				int fd;
				int fonty = get_font_height(tb->gd);
				int total, maxshown;
				fflush(stdout);

				/**
				 * label show success
				 **/
				strncpy(l->text, str, sizeof(str));

				/**
				 * copy saved filename to ta
				 **/
				text_line_set_bufsize(ta, LABEL_MAX);
				text_line_set_placeholder(ta, "enter filename");
				buf = text_line_get_buf(ta);
				strncpy(buf, argv[1], LABEL_MAX);

				/**
				 * content area are multilines
				 **/
				text_line_set_bufsize(tb, FILE_MAX);
				text_line_set_placeholder(tb, "place holder");
				text_line_set_multilines(tb, 0);
				text_line_register_move_handler(tb, WIDGET_POINTER(s), TEXT_LINE_EVENT_ALL, scrollbar_subscribe_text_line);

				/**
				 * copy file content to buf
				 **/
				load_file(argv[1], text_line_get_buf(tb), l->text);

				/**
				 * set for scroll_bar
				 **/    
				maxshown = fonty * text_line_get_max_line_shown(tb);
				s->area.y += (s->area.height - maxshown)/ 2;
				s->area.height = maxshown;
				total = text_line_get_line_total(tb);
				scroll_bar_set_vertical(s);
				scroll_bar_set_line_height(s, fonty);
				scroll_bar_set_view(s, fonty * total, 0);
				scroll_bar_register_move_handler(s, WIDGET_POINTER(tb), SCROLL_BAR_EVENT_ALL, text_line_subscribe_scrollbar);
			}
		}
		else
		{
			char str[] = "permission denied";
			strncpy(l->text, str, sizeof(str));
		}
	}
	/**
	 * so far, so wrong
	 **/
	else
	{
		char str[] = "wrong parameter!";
		strncpy(l->text, str, sizeof(str));
	}

	object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(l));
	object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(tb));
	object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(ta));
	object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(b));
	object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(s));
	object_attach_child(OBJECT_POINTER(w), OBJECT_POINTER(bt_open));

	/* 添加顶层窗口 */
	application_add_window(NULL, w);
	/* 设置主窗口 */
	application_set_main_window(w);

	/* 运行 */
	application_exec();

	return 0;
}
