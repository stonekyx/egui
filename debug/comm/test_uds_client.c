/**
 * 测试uds模块的服务器端
 * 协议：
 * server listen
 * client connect server
 * server accept
 * server send client: hello, client
 * client send server: hello, server
 * both exit
 **/

#include "comm.h"
#include "log.h"

const char SERVER_MSG[] = "hello, client";
const char CLIENT_MSG[] = "hello, server";

#define SERVER_PATH "/tmp/test_uds_server_socket"

int main()
{
	struct egui_uds server_uds;
	char buf[1024] = {0};

	if(0 != uds_init(&server_uds, SOCK_STREAM, SERVER_PATH, PEER_ROLE_CLIENT))
	{
		EGUI_PRINT_ERROR("failed to init server socket");
		return -1;
	}
	EGUI_PRINT_INFO("connect to server successfully!");

	if(uds_read(&server_uds, buf, sizeof(SERVER_MSG)) < 0)
	{
		EGUI_PRINT_ERROR("failed to read server msg");
		return -1;
	}

	if(0 != strncmp(buf, SERVER_MSG, sizeof(SERVER_MSG)))
	{
		EGUI_PRINT_ERROR("unknow client msg %s", buf);
	}
	EGUI_PRINT_INFO("read %s successfully", buf);

	if(uds_write(&server_uds, CLIENT_MSG, sizeof(CLIENT_MSG)) < 0)
	{
		EGUI_PRINT_ERROR("failed to write client msg");
		return -1;
	}
	EGUI_PRINT_INFO("write %s successfully", CLIENT_MSG);

	if(0 != uds_exit(&server_uds))
	{
		EGUI_PRINT_ERROR("failed to clean server socket");
		return -1;
	}

	return 0;
}
