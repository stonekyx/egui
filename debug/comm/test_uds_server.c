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
	struct egui_uds client_uds;
	char buf[1024] = {0};

	if(0 != uds_init(&server_uds, SOCK_STREAM, SERVER_PATH, PEER_ROLE_SERVER))
	{
		EGUI_PRINT_ERROR("failed to init server socket");
		return -1;
	}

	if(0 != accept_as_uds(&server_uds, &client_uds))
	{
		EGUI_PRINT_ERROR("failed to accept client uds");
		return -1;
	}
	EGUI_PRINT_INFO("accept client socket successfully!");

	if(uds_write(&client_uds, SERVER_MSG, sizeof(SERVER_MSG)) < 0)
	{
		EGUI_PRINT_ERROR("failed to write server msg");
		return -1;
	}
	EGUI_PRINT_INFO("write %s successfully", SERVER_MSG);

	if(uds_read(&client_uds, buf, sizeof(buf)) < 0)
	{
		EGUI_PRINT_ERROR("failed to read client msg");
		return -1;
	}

	if(0 != strncmp(buf, CLIENT_MSG, sizeof(CLIENT_MSG)))
	{
		EGUI_PRINT_ERROR("unknow client msg %s", buf);
	}
	EGUI_PRINT_INFO("read %s successfully", buf);

	if(0 != uds_exit(&client_uds))
	{
		EGUI_PRINT_ERROR("failed to clean client socket");
		return -1;
	}

	if(0 != uds_exit(&server_uds))
	{
		EGUI_PRINT_ERROR("failed to clean server socket");
		return -1;
	}
	return 0;
}
