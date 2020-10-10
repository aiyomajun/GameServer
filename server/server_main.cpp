/*
server_main��������,
server ��Ҫ��¼ÿ���ͻ��˵�״̬
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
����������Ҫͬʱ�����˺Ϳͻ��˵Ĳ��֣���������񲿷֣�
Ȼ��sleep5�뱣֤���еķ������ˣ�����ͻ��˲��֡�
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
	/*������server����server_mgr����tcp_server��������
	����Ҫsleep 5�룬��ֹ��ʼ����ʱ��tcp������
	*/
	sleep(5);
	// ÿ��server������room_mgr��server_mgr
	if (s->room_mgr_conn->tryconnect("127.0.0.1", 10001) < 0)
	{
		printf("server connect to room_mgr failed\n");
	}
	if (s->server_mgr_conn->tryconnect("127.0.0.1", 10000) < 0)
	{
		printf("server connect to server_mgr failed\n");
	}
	//websocket�Ĵ����̣߳�
	pthread_create(&pt, NULL, start_websocket_server, reinterpret_cast<void*>(20000));
	printf("server init done\n");
	while (1)
	{
		sleep(5);
		//�ȴ���������
	}
}