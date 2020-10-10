#include"room_mgr.hpp"
#include"../game_common/common/tcp_server.hpp"
#include"../game_common/room_mgr_on_ack.hpp"


extern void room_mgr_cmd_entry(int fd, int cmd, char* data, int len, bool same_endian);

static void tcp_connect(int fd)
{
	printf("tcp %d connect\n", fd);
}
static void tcp_disconnect(int fd)
{
	printf("tcp %d disconnect\n", fd);
}



static void tcp_request(int fd, int cmd, char* data, int len, bool same_endian)
{
	//printf("in tcprequest\n");
	printf("fd is %d,cmd is %d, data_len is %d,same_endian is %d\n", fd, cmd, len, same_endian);
	for (int i = 0; i < len; i++)
	{
		printf("%c", data[i]);
	}
	printf("\n");
	//printf("\nout tcprequest\n");
	room_mgr_cmd_entry(fd, cmd, data, len, same_endian);
}

extern void handle_exception();

void test_print()
{
	printf("sizeof(on_get_all_domain_t)=%d\n", sizeof(on_get_all_domain_t));
	printf("sizeof(ack_get_all_domain_t)=%d\n", sizeof(ack_get_all_domain_t));
	printf("sizeof(on_get_all_channel_t)=%d\n", sizeof(on_get_all_channel_t));
	printf("sizeof(ack_get_all_channel_t)=%d\n", sizeof(ack_get_all_channel_t));
	printf("sizeof(on_get_all_room_t)=%d\n", sizeof(on_get_all_room_t));
	printf("sizeof(ack_get_all_room_t)=%d\n", sizeof(ack_get_all_room_t));
}

int main()
{
	printf("room_mgr server in 127.0.0.1:10001\n");
	test_print();
	handle_exception();
	//tcp 服务器初始化,最大允许20个客户端连接，包括
	TCP_SERVER* tcp = TCP_SERVER::get_instance(TCP_SERVER_TYPE_MUTI_THREAD, 20);
	room_mgr * roommgr = room_mgr::get_instance();

	tcp->reg_connected_callback(tcp_connect);
	tcp->reg_disconnected_callback(tcp_disconnect);
	tcp->reg_requests_callback(tcp_request);
	tcp->listen_and_serve(10001);
}