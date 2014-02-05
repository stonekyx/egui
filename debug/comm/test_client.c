/**
 * 测试客户端
 **/
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "comm.h"
#include "log.h"
#include "compiler.h"

#define SERVER_PATH "/tmp/test_server_sock"
const char* help_str = "usage:\n1. draw_line x1 y1 x2 y2\n2. register_window parent_descripter window_title\n3. window_manager_exit";

/**
 * 发送draw line请求
 **/
int fake_draw_line(struct egui_uds* uds_ptr, si_t x1, si_t y1, si_t x2, si_t y2)
{
	struct packet_body_draw_line body;
	si_t ret = 0;
	request_set_draw_line(&body, 0, x1, y1, x2, y2);
	if(0 != comm_send_request(uds_ptr, REQUEST_TYPE_DRAW_LINE, &body, sizeof(body)))
	{
		EGUI_PRINT_ERROR("failed to send draw line request");
		return -1;
	}
	if(0 != comm_recv_respond(uds_ptr, RESPOND_TYPE_DRAW_LINE, &ret, NULL))
	{
		EGUI_PRINT_ERROR("failed to recv draw line respond");
		return -1;
	}

	EGUI_PRINT_INFO("send draw line request, recv ret %d", (int)ret);
	return ret;
}

/**
 * 发送window_manager_exit请求
 **/
int fake_window_manager_exit(struct egui_uds* uds_ptr)
{
	si_t ret = 0;
	if(0 != comm_send_request(uds_ptr, REQUEST_TYPE_WINDOW_MANAGER_QUIT, NULL, 0))
	{
		EGUI_PRINT_ERROR("failed to send window manager exit request");
		return -1;
	}
	if(0 != comm_recv_respond(uds_ptr, RESPOND_TYPE_WINDOW_MANAGER_QUIT, &ret, NULL))
	{
		EGUI_PRINT_ERROR("failed to recv window manager exit respond");
		return -1;
	}

	EGUI_PRINT_INFO("send window manager exit request, recv ret %d", (int)ret);
	return ret;
}

/**
 * 发送注册窗口请求
 **/
int fake_register_window(struct egui_uds* uds_ptr, si_t parent_fd, char* title)
{
	struct packet_body_register_window body;
	si_t ret = 0;

	request_set_register_window(&body, parent_fd, title, strlen(title) + 1, 0, 0, 0, 0, 0, 0, 0);
	if(0 != comm_send_request_with_extra(uds_ptr, REQUEST_TYPE_REGISTER_WINDOW, 
			&body, sizeof(body), title, strlen(title) + 1))
	{
		EGUI_PRINT_ERROR("failed to send register window request");
		return -1;
	}
	if(0 != comm_recv_respond(uds_ptr, RESPOND_TYPE_REGISTER_WINDOW, &ret, NULL))
	{
		EGUI_PRINT_ERROR("failed to recv register window respond");
		return -1;
	}

	EGUI_PRINT_INFO("send register window request, recv ret %d", (int)ret);

	return 0;
}

/**
 * 对标准输入的写操作将触发的回调函数
 * 即event_listener_add_write_handler()中标准输入的通信句柄所对应的处理函数
 *
 * 读取命令，调用对应函数发送请求
 * 1. $0 draw_line x1 y1 x2 y2
 * 2. $0 register_window fd title
 * 3. $0 window_manager_exit
 **/
si_t command_handler(struct egui_uds* uds_ptr, addr_t arg)
{
	char command_buf[1024];
	struct egui_uds* server_uds_ptr = (struct egui_uds*)arg;
	const char draw_line_command[] = "draw_line";
	const char register_window_command[] = "register_window";
	const char window_manager_exit_command[] = "window_manager_exit";
	NOT_USED(uds_ptr);

	EGUI_PRINT_INFO("callback command_handler() is called");

	scanf("%s", command_buf);

	if(0 == strncmp(command_buf, draw_line_command, sizeof(draw_line_command)))
	{
		si_t x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		EGUI_PRINT_INFO("get draw_line_command");
		scanf("%d%d%d%d", (int*)&x1, (int*)&y1, (int*)&x2, (int*)&y2);
		if(0 != fake_draw_line(server_uds_ptr, x1, y1, x2, y2))
		{
			EGUI_PRINT_ERROR("failed to execute draw line command");
		}
	}
	else if(0 == strncmp(command_buf, register_window_command, sizeof(register_window_command)))
	{
		si_t fd = 0;
		char title_buf[1024] = {0};
		EGUI_PRINT_INFO("get register_window_command");
		scanf("%d%s", (int*)&fd, title_buf);
		if(0 != fake_register_window(server_uds_ptr, fd, title_buf))
		{
			EGUI_PRINT_ERROR("failed to execute register window command");
		}
	}
	else if(0 == strncmp(command_buf, window_manager_exit_command, sizeof(window_manager_exit_command)))
	{
		EGUI_PRINT_INFO("get window_manager_exit_command");
		if(0 != fake_window_manager_exit(server_uds_ptr))
		{
			EGUI_PRINT_ERROR("failed to execute window manager exit command");
		}
		/** 如果用户输入退出窗口管理器的命令，那么客户端的程序也应当退出 **/
		return SELECTER_RETURN_TYPE_END;
	}
	else
	{
		EGUI_PRINT_ERROR("unknow command %s", command_buf);
		EGUI_PRINT_INFO(help_str);
	}
	return SELECTER_RETURN_TYPE_CONTINUE;
}

/**
 * 对来自服务器的消息的处理函数
 * 即event_listener_add_write_handler()中客户端通信句柄对应的处理函数
 * 
 * 只处理窗口激活消息
 **/
si_t server_message_handler(struct egui_uds* uds_ptr, addr_t arg)
{
	union message msg;
	NOT_USED(arg);

	EGUI_PRINT_INFO("callback server_message_handler() is called");

	if(0 != comm_recv_msg(uds_ptr, &msg))
	{
		EGUI_PRINT_ERROR("failed to recv message from server");
		return SELECTER_RETURN_TYPE_CONTINUE;
	}

	switch(message_get_type(&msg))
	{
	case MESSAGE_TYPE_WINDOW_ACTIVATE:
		EGUI_PRINT_INFO("recieved window activate message!");
		break;
	default:
		EGUI_PRINT_INFO("recieved unknown message, type = %d", (int)message_get_type(&msg));
		break;
	}
	return SELECTER_RETURN_TYPE_CONTINUE;
}

int main(void)
{
	struct event_listener client_listener;
	struct egui_uds client_uds;
	struct egui_uds stdin_uds;

	event_listener_init(&client_listener);

	/**
	 * 初始化客户端通信句柄并将其对应的回调函数设置为server_message_handler
	 **/
	if(0 != uds_init(&client_uds, SOCK_STREAM, SERVER_PATH, PEER_ROLE_CLIENT))
	{
		EGUI_PRINT_ERROR("Failed to init server uds");
		return -1;
	}
	event_listener_add_read_event(&client_listener, &client_uds, NULL, server_message_handler, NULL);

	/**
	 * 初始化标准输入通信句柄并将对应的回调函数设置为command_handler
	 **/
	uds_init_from_naked_socket(&stdin_uds, STDIN_FILENO);
	event_listener_add_read_event(&client_listener, &stdin_uds, &client_uds, command_handler, NULL);

	/** 打印帮助信息 **/
	EGUI_PRINT_INFO(help_str);

	if(0 != event_listener_exec(&client_listener))
	{
		EGUI_PRINT_ERROR("failed to execute event_listener");
		return -1;
	}

	event_listener_exit(&client_listener);

	return 0;
}
