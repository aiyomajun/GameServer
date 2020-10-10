#include"room_mgr.hpp"
#include"room_mgr_cmd.hpp"
#include"../game_common/cmd.hpp"
#include"../game_common/room_mgr_on_ack.hpp"
#include"../game_common/common/tcp_server.hpp"

static cmd_handle_t room_mgr_cmd[]=
{
	{ROOM_MGR_CMD_ON_GET_ALL_DOMAIN,room_mgr_get_all_domain},
    {ROOM_MGR_CMD_ON_GET_ALL_CHANNEL,room_mgr_get_all_channel},
	{ROOM_MGR_CMD_ON_GET_ALL_ROOM, room_mgr_get_all_room},
    {ROOM_MGR_CMD_ON_SEARCH_ROOM, room_mgr_search_room},
    {ROOM_MGR_CMD_ON_ENTER_ROOM, room_mgr_enter_room},
    {ROOM_MGR_CMD_ON_LEAVE_ROOM, room_mgr_leave_room},
    {ROOM_MGR_CMD_ON_CREATE_ROOM_AND_ENTER, room_mgr_create_room_and_enter},
	{ROOM_MGR_CMD_ON_CREATE_ROOM, room_mgr_create_room},
	{ROOM_MGR_CMD_ON_DESTROY_ROOM, room_mgr_destroy_room},
	{ROOM_MGR_CMD_ON_START_GAME, room_mgr_start_game},
	{ROOM_MGR_CMD_ON_GET_ROOM_OF_PLAYER, room_mgr_get_room_of_player},
	{ROOM_MGR_CMD_ON_CHANGE_ROOM_TYPE, room_mgr_change_room_type},
	{ROOM_MGR_CMD_ON_CAHNGE_ROOM_OWNER, room_mgr_change_room_owner},
	{ROOM_MGR_CMD_ON_KICK_PLAYER_OFF, room_mgr_kick_off_player}

};


void room_mgr_cmd_entry(int fd, int cmd, char* data, int len, bool same_endian)
{
	int i;
	for (i = 0; i < sizeof(room_mgr_cmd) / sizeof(cmd_handle_t); i++)
	{
		if (cmd == room_mgr_cmd[i].cmd)
		{
			room_mgr_cmd[i].cmd_callback(fd, len, data, same_endian);
			return;
		}
	}
	//没找到这个cmd
	printf("no such cmd%d\n", cmd);
	return;
}

void room_mgr_get_all_domain(int fd, int len, char*data, bool same_endian)
{
	
	room_mgr *mgr = room_mgr::get_instance();
	TCP_SERVER* tcp = TCP_SERVER::get_instance();
	on_get_all_domain_t *ogad;
	ack_get_all_domain_t agad;
	if (len != sizeof(on_get_all_domain_t))
	{
		//发来的数据不合法
	}
	ogad = (on_get_all_domain_t *)data;
	if (!same_endian)
	{
		//大小端不对，改变大小端
	}
	int out_len;
	int ret;
	ret = mgr->get_all_domain(agad.domain_info, MAX_DOMAIN_COUNT, agad.total_domain);
	agad.code = ret;
	printf("get all domain %d\n", agad.total_domain);
	tcp->send_data(ROOM_MGR_CMD_ACK_GET_ALL_DOMAIN, (char*)&agad, \
		sizeof(ack_get_all_domain_t), fd);

}
void room_mgr_get_all_channel(int fd, int len, char*data, bool same_endian)
{

}
void room_mgr_get_all_room(int fd, int len, char*data, bool same_endian)
{

}

void room_mgr_search_room(int fd, int len, char * data, bool same_endian)
{
}

void room_mgr_enter_room(int fd, int len, char * data, bool same_endian)
{
}

void room_mgr_leave_room(int fd, int len, char * data, bool same_endian)
{
}

void room_mgr_create_room_and_enter(int fd, int len, char * data, bool same_endian)
{
}

void room_mgr_create_room(int fd, int len, char * data, bool same_endian)
{
}

void room_mgr_destroy_room(int fd, int len, char * data, bool same_endian)
{
}

void room_mgr_start_game(int fd, int len, char * data, bool same_endian)
{
}

void room_mgr_get_room_of_player(int fd, int len, char * data, bool same_endian)
{
}

void room_mgr_change_room_type(int fd, int len, char * data, bool same_endian)
{
}

void room_mgr_change_room_level(int fd, int len, char * data, bool same_endian)
{
}

void room_mgr_change_room_owner(int fd, int len, char * data, bool same_endian)
{
}

void room_mgr_kick_off_player(int fd, int len, char * data, bool same_endian)
{
}
