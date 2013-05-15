# include <string.h>
# include "client_lib.h"
# include "log.h"

#define SERVER_PATH "/tmp/test_server_sock"
const char* help_str = "usage:\ndraw_line x1 y1 x2 y2\n2. register_window parent_descripter window_title\n3. window_manager_exit";

int main()
{
	struct egui_uds client_uds = {0};
	si_t s_width = 0, s_height = 0, gd = 0;
	const char test_str[] = "hello, egui!";
	struct rectangle area = {0};
	/**
	 * 初始化客户端通信句柄
	 **/
	if(0 != uds_init(&client_uds, SOCK_STREAM, SERVER_PATH, PEER_ROLE_CLIENT))
	{
		EGUI_PRINT_ERROR("Failed to init server uds");
		return -1;
	}

	/**
	 * 初始化客户端
	 **/
	client_lib_init(&client_uds, NULL);
	EGUI_PRINT_INFO("client init sucessfully!");

	/**
	 * 获取屏幕的高和宽，输出
	 **/
	s_width = get_screen_width();
	s_height = get_screen_height();
	if(s_width < 0 || s_height < 0)
	{
		EGUI_PRINT_ERROR("failed to get screen width or height");
		return -1;
	}
	EGUI_PRINT_INFO("screen width = %d, height = %d", (int)s_width, (int)s_height);

	/**
	 * 初始化图形设备
	 **/
	gd = graphics_device_init(300, 300, 1000, 600, 255, 0, 0, 0, FONT_MATRIX_12);
	if(gd < 0)
	{
		EGUI_PRINT_ERROR("failed to init gd");
		return -1;
	}

	/**
	 * 显示字符串：hello egui
	 **/
	if(0 != show_text(gd, 300, 300, test_str, strlen(test_str)) 
		|| 0 != update(gd))
	{
		EGUI_PRINT_ERROR("failed to show test \"%s\"", test_str);
		return -1;
	}

	/**
	 * 图形设备退出
	 **/
	if(0 != graphics_device_exit(gd))
	{
		EGUI_PRINT_ERROR("failed to exit graph device");
		return -1;
	}

	/**
	 * 最大化窗口，输出返回的窗口大小
	 **/
	if(0 != maximize_window(10, &area))
	{
		EGUI_PRINT_ERROR("failed to max window");
		return -1;
	}
	EGUI_PRINT_INFO("maximize window area = (%d, %d), %d, %d", (int)area.x, (int)area.y, (int)area.width, (int)area.height);

	if(0 != window_manager_quit())
	{
		EGUI_PRINT_ERROR("failed to let window manager quit");
		return -1;
	}

	/**
	 * 清理通信句柄
	 **/
	if(0 != uds_exit(&client_uds))
	{
		EGUI_PRINT_ERROR("failed to clean uds");
		return -1;
	}
	return 0;
}
