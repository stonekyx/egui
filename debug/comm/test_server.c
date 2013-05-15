/**
 * 测试的server:
 * 监听是否有客户端的连接请求和来自已建立连接的客户端的请求
 * 如果有新的客户端连接，则添加到客户端队列，并将客户端的通信句柄加入监听集合
 * 如果已建立连接的客户端发来请求，则使用回调函数处理
 *
 * 此测试函数对各个类型的request, respond, message测试覆盖不会很全面。
 **/
#include "comm.h"
#include "log.h"

#define SERVER_PATH "/tmp/test_server_sock"

/**
 * 退出server程序的返回值
 **/
static si_t EXIT_NUMBER = 255;

/**
 * event_listener里面析构函数 仅仅清理通信句柄
 **/
void uds_destructor(struct egui_uds* uds, addr_t arg)
{
	uds_exit(uds);
}

/**
 * 假的engine_draw_line，即客户端发来draw_line请求后服务器所调用的实际操作的函数
 **/
si_t fake_engine_draw_line(si_t gd, si_t x1, si_t y1, si_t x2, si_t y2)
{
	EGUI_PRINT_INFO("fake_engine_draw_line() is called! graphics_device %d draw line from(%d, %d) to (%d, %d)",
		(int)gd, (int)x1, (int)y1, (int)x2, (int)y2);
	return SELECTER_RETURN_TYPE_CONTINUE;
}

/**
 * 请求处理函数；这个函数是comm_recv_request_and_send_respond()里面的回调函数
 **/
si_t request_handler(addr_t body, si_t detail_type, union respond* respond_ptr, addr_t arg)
{
	char* str = NULL;
	si_t ret = 0;
	si_t fd = 0;
	union message msg;
	struct queue* message_queue_ptr = (struct queue*)arg;
	switch(detail_type)
	{
		/**
		 * 测试普通情况：发送的请求实质是客户端需要远程调用一个函数
		 **/
	case REQUEST_TYPE_DRAW_LINE:
		ret = request_call_by_draw_line(body, fake_engine_draw_line);
		respond_set_normal(respond_ptr, RESPOND_TYPE_DRAW_LINE, ret);
		EGUI_PRINT_INFO("recieved draw line request!");
		break;

		/**
		 * 测试特殊情况：发送请求后产生消息，消息先放到队列中，稍后回处理
		 **/
	case REQUEST_TYPE_REGISTER_WINDOW:
		/** 读取请求包中的窗口标题和父窗口的句柄 **/
		request_get_register_window(body, &fd, &str, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

		/** 返回的包是新窗口句柄，此处假设为父窗口句柄+1 **/
		ret = fd + 1;
		respond_set_normal(respond_ptr, RESPOND_TYPE_REGISTER_WINDOW, ret);

		/** 此外，还会发送给客户端一个激活新窗口的消息 **/
		message_set_window_activate(&msg, ret);
		queue_push(message_queue_ptr, &msg, sizeof(msg));

		EGUI_PRINT_INFO("recieved register window request! title = %s", str);
		break;

		/**
		 * 测试特殊情况：发送请求要求服务器退出循环
		 **/
	case REQUEST_TYPE_WINDOW_MANAGER_QUIT:
		ret = EXIT_NUMBER;
		respond_set_normal(respond_ptr, RESPOND_TYPE_WINDOW_MANAGER_QUIT, 0);
		EGUI_PRINT_INFO("recived exit request! SERVER EXIT!");
		break;

	default:
		break;
	}

	return ret;
}

/**
 * 客户端请求处理函数：这个函数是selector里面的客户端的通信句柄对应的回调函数
 **/
si_t client_request_handler(struct egui_uds* uds_ptr, addr_t arg)
{
	struct queue message_queue;
	si_t ret = 0;

	EGUI_PRINT_INFO("callback client_request_handler() is called");

	queue_init(&message_queue);

	/** 接受request回应respond **/
	ret = comm_recv_request_and_send_respond(uds_ptr, &message_queue, request_handler);
	if(EXIT_NUMBER == ret)
	{
		return SELECTER_RETURN_TYPE_END;
	}

	/** 发送相关消息 **/
	while(!queue_empty(&message_queue))
	{
		union message* message_ptr = (union message*)queue_front(&message_queue);
		if(0 != comm_send_message(uds_ptr, message_ptr))
		{
			EGUI_PRINT_ERROR("failed to send message!");
		}
		queue_pop(&message_queue);
	}

	queue_exit(&message_queue);
	return SELECTER_RETURN_TYPE_CONTINUE;
}

/**
 * 服务器连接处理函数：这个函数是event_listener里面服务器端通信句柄对应的回调函数
 * 接受客户端的通信句柄，将其添加到event_listener中
 **/
si_t server_accept_handler(struct egui_uds* uds_ptr, addr_t arg)
{
	struct event_listener* server_listener = (struct event_listener*)arg;
	struct egui_uds new_uds;

	EGUI_PRINT_INFO("callback server_accept_handler() is called");

	if(0 != accept_as_uds(uds_ptr, &new_uds))
	{
		EGUI_PRINT_ERROR("Failed to accept client connection");
		return SELECTER_RETURN_TYPE_CONTINUE;
	}

	event_listener_add_read_event(server_listener, &new_uds, NULL, client_request_handler, uds_destructor);

	return SELECTER_RETURN_TYPE_CONTINUE;
}

int main(void)
{
	struct event_listener server_listener;
	struct egui_uds server_uds;

	if(0 != uds_init(&server_uds, SOCK_STREAM, SERVER_PATH, PEER_ROLE_SERVER))
	{
		EGUI_PRINT_ERROR("Failed to init server uds");
		return -1;
	}

	event_listener_init(&server_listener);
	event_listener_add_read_event(&server_listener, &server_uds, &server_listener, server_accept_handler, NULL);

	if(0 != event_listener_exec(&server_listener))
	{
		EGUI_PRINT_ERROR("failed to execute event_listener");
		return -1;
	}

	event_listener_exit(&server_listener);

	if(0 != uds_exit(&server_uds))
	{
		EGUI_PRINT_ERROR("failed to exit server uds");
		return -1;
	}

	return 0;
}
