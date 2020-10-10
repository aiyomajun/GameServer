/*
server_main的主程序,
server 需要记录每个客户端的状态
*/
#include"server.hpp"
#include"../game_common/common/common_api.hpp"
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>



extern void* start_websocket_server(void *args);

extern void tcp_connect_callback(int fd);
extern void tcp_disconnect_callback(int fd);
extern void tcp_request_callback(int fd, int cmd, char *data, int len, bool same_endian);
static server *s = NULL;

void * tcp_server_entry(void *args)
{
	s->server_server->listen_and_serve(10002);
}


/*
对于所有需要同时起服务端和客户端的部分，都先起服务部分，
然后sleep5秒保证所有的服务都起了，再起客户端部分。
*/
int main()
{
	pthread_t pt;
	handle_exception();
	s = server::get_instance();
	s->server_server->reg_connected_callback(tcp_connect_callback);
	s->server_server->reg_disconnected_callback(tcp_disconnect_callback);
	s->server_server->reg_requests_callback(tcp_request_callback);
	pthread_create(&pt, NULL, tcp_server_entry, NULL);
	/*不管是server还是server_mgr，当tcp_server启动过后，
	都需要sleep 5秒，防止初始化的时候tcp连不上
	*/
	sleep(5);
	// 每个server先连上room_mgr和server_mgr
	if (s->room_mgr_conn->tryconnect("127.0.0.1", 10001) < 0)
	{
		printf("server connect to room_mgr failed\n");
	}
	if (s->server_mgr_conn->tryconnect("127.0.0.1", 10000) < 0)
	{
		printf("server connect to server_mgr failed\n");
	}
	//websocket的处理线程，
	pthread_create(&pt, NULL, start_websocket_server, reinterpret_cast<void*>(20000));
	printf("server init done\n");
	while (1)
	{
		sleep(5);
		//等待接收命令
	}
}