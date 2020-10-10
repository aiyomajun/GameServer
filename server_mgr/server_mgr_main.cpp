

/*

这里是server_mgr的主程序。

*/

#include<iostream>
#include"../game_common/common/tcp_server.hpp"
#include"../game_common/common/common_api.hpp"
#include "server_mgr.hpp"

void tcp_connect(int fd)
{
	printf("tcp %d connect\n", fd);
}
void tcp_disconnect(int fd)
{
	printf("tcp %d disconnect\n", fd);
}



void tcp_request(int fd, int cmd, char* data, int len, bool same_endian)
{
	//printf("in tcprequest\n");
	printf("fd is %d,cmd is %d, data_len is %d,same_endian is %d\n", fd, cmd, len, same_endian);
	for (int i = 0; i < len; i++)
	{
		printf("%c", data[i]);
	}
	printf("\n");
	//printf("\nout tcprequest\n");

}


int main()
{
	handle_exception();

	//tcp 服务器初始化
	TCP_SERVER* tcp = TCP_SERVER::get_instance(TCP_SERVER_TYPE_MUTI_THREAD, 20);

	//server_mgr初始化
	server_mgr* ser_mgr = server_mgr::get_instance();

	tcp->reg_connected_callback(tcp_connect);
	tcp->reg_disconnected_callback(tcp_disconnect);
	tcp->reg_requests_callback(tcp_request);
	tcp->listen_and_serve(10000);
}