#include "server.hpp"
#include"../game_common/room_mgr_on_ack.hpp"
#include"../game_common/cmd.hpp"
#include"../game_common/server_common.hpp"


server* server::svr = NULL;

server::server()
{
	room_mgr_conn = new TCP_CLIENT();
	server_mgr_conn = new TCP_CLIENT(); 

	//server_server是供server_mgr连接的。故使用单线程模型，最大连接数为20-4
	server_server = TCP_SERVER::get_instance(TCP_SERVER_TYPE_SINGLE_THREAD, 20);
}

server * server::get_instance()
{
	if (svr != NULL)
	{
		return svr;
	}
	svr = new server();
	return svr;
}

int server::get_all_domain(on_get_all_domain_t *on, ack_get_all_domain_t * ack)
{
	//首先先检测room_mgr_的连接状态
	TCP_CLIENT * client = room_mgr_conn;
	int cmd;
	int len;
	bool same_endian;
	bool ok;
	client->send_data(ROOM_MGR_CMD_ON_GET_ALL_DOMAIN, (char*)on,
		sizeof(on_get_all_domain_t), client->get_sockfd());

	ok=client->recv_data(&cmd, (char*)ack, sizeof(ack_get_all_domain_t), 
		&len, &same_endian);

	if (ok < 0)//接收失败
	{
		printf("server try to get all domain from room_mgr failed\n");
		//先空着
	}
	if (!same_endian)//大小端不同，先转一下
	{

	}
	if (ack->code != ROOM_CODE_OK)
	{
		//先空着
	}
	return SERVER_CODE_OK;

}

int server::get_all_channel(on_get_all_channel_t * on, ack_get_all_channel_t * ack)
{
	return 0;
}

int server::get_all_room(on_get_all_room_t * on, ack_get_all_room_t * ack)
{
	return 0;
}

int server::enter_room(on_enter_room_t * on, ack_enter_room_t * ack)
{
	return 0;
}

int server::leave_room(on_leave_room_t * on, ack_leave_room_t * ack)
{
	return 0;
}

int server::create_room_and_enter(on_create_room_and_endter_t * on, ack_create_room_and_enter_t * ack)
{
	return 0;
}

int server::destroy_room(on_destroy_room_t * on, ack_destroy_room_t * ack)
{
	return 0;
}

int server::start_game(on_start_game_t * on, ack_start_game_t * ack)
{
	return 0;
}

int server::get_room_of_player(on_get_room_of_player_t * on, ack_get_room_of_player_t * ack)
{
	return 0;
}

int server::change_room_type(on_change_room_type_t * on, ack_change_room_type_t * ack)
{
	return 0;
}

int server::change_room_level(on_change_room_level_t * on, ack_change_room_level_t * ack)
{
	return 0;
}

int server::change_room_owner(on_change_room_owner_t * on, ack_change_room_owner_t * ack)
{
	return 0;
}

int server::kick_off_player(on_kick_player_off_t * on, ack_kick_player_off_t * ack)
{
	return 0;
}
