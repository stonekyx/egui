# include <string.h>
# include "server_lib.h"
# include "graph.h"
# include "log.h"

#define SERVER_PATH "/tmp/test_server_sock"
#define FB_DEV_FILE_PATH "/dev/fb0"

si_t fake_graphics_device_init_handler(addr_t arg, si_t gd)
{
	struct graphics_device* gd_ptr = (struct graphics_device*)gd;

    memcpy(&(gd_ptr->screen), &global_screen, sizeof(struct screen));
	gd_ptr->screen.video_access_mode = VIDEO_ACCESS_MODE_DIRECT;
    gd_ptr->screen.buffer_addr = NULL;

	EGUI_PRINT_INFO("fake_graphics_device_init_handler() is called! gd = %d", (int)gd);

	return 0;
}

si_t fake_graphics_device_exit_handler(addr_t arg, si_t gd)
{
	EGUI_PRINT_INFO("fake_graphics_device_exit_handler() is called! gd = %d", (int)gd);
	return 0;
}

si_t fake_maximize_window_handler(addr_t arg, si_t fd, struct rectangle* area)
{
	EGUI_PRINT_INFO("maximize_window_handler() is called! fd = %d!", (int)fd);
	area->x = fd + 1;
	area->y = fd + 2;
	area->width = fd + 3;
	area->height = fd + 4;
	return 0;
}

si_t fake_window_manager_exit_handler(addr_t arg)
{
	EGUI_PRINT_INFO("fake_window_manager_exit_handler() is called!");
	return 0;
}

si_t fake_update_handler(addr_t arg, si_t gd)
{
	engine_update(gd);
	EGUI_PRINT_INFO("fake_update_handler() is called!");
	return 0;
}

si_t server_accept_handler(struct egui_uds* uds_ptr, addr_t arg)
{
	struct egui_uds client_uds = {0};
	if(0 != accept_as_uds(uds_ptr, &client_uds))
	{
		EGUI_PRINT_ERROR("failed to accept client uds");
		return SELECTER_RETURN_TYPE_END;
	}
	server_lib_add_new_client(&client_uds, NULL);

	EGUI_PRINT_INFO("add a new client!");

	return SELECTER_RETURN_TYPE_CONTINUE;
}

int main()
{
	struct egui_uds server_uds;
	struct event_listener server_listener;

	/**
	 * 初始化屏幕
	 **/
	if(0 != screen_init(FB_DEV_FILE_PATH))
	{
		EGUI_PRINT_ERROR("failed to init screen");
		return -1;
	}

	/**
	 * 初始化通信句柄
	 **/
	if(0 != uds_init(&server_uds, SOCK_STREAM, SERVER_PATH, PEER_ROLE_SERVER))
	{
		EGUI_PRINT_ERROR("Failed to init server uds");
		return -1;
	}

	/**
	 * 初始化event_listener
	 **/
	event_listener_init(&server_listener);
	event_listener_add_read_event(&server_listener, &server_uds, NULL, server_accept_handler, NULL);

	/**
	 * 客户端库初始化
	 **/
	server_lib_init(&server_listener, NULL);

	/**
	 * 添加各项handler
	 **/
	add_graphics_device_init_handler(fake_graphics_device_init_handler);
	add_graphics_device_exit_handler(fake_graphics_device_exit_handler);
	add_maximize_window_handler(fake_maximize_window_handler);
	add_window_manager_exit_handler(fake_window_manager_exit_handler);
	add_update_handler(fake_update_handler);
	
	/**
	 * 运行event_listener
	 **/
	if(0 != event_listener_exec(&server_listener))
	{
		EGUI_PRINT_ERROR("failed to execute event_listener");
		return -1;
	}

	/**
	 * 清理event_listener
	 **/
	event_listener_exit(&server_listener);

	/**
	 * 清理屏幕
	 **/
	screen_exit();
	return 0;
}
