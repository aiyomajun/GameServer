/*
server_mgr收到消息的应答

*/
#ifndef _SERVER_MGR_ON_ACK_HPP
#define _SERVER_MGR_ON_ACK_HPP
#include"player.hpp"
#include"room_mgr_common.hpp"

//通知server_mgr有玩家加入房间
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	player_t player_in;//加入房间的玩家
	
	//在其他服务器上的玩家
	player_t left_player_in_other_server[MAX_PLYAER_COUNT];
	int left_player_in_other_server_count;
}notify_svrmgr_player_enter_room_t;


//通知server_mgr有玩家离开房间
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	player_t player_in;//加入房间的玩家

	//在其他服务器上的玩家
	player_t left_player_in_other_server[MAX_PLYAER_COUNT];
	int left_player_in_other_server_count;

}notify_svrmgr_player_leave_room_t;

//通知server_mgr房间类型改变
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	int changed_type;
	//在其他服务器上的玩家
	player_t left_player_in_other_server[MAX_PLYAER_COUNT];
	int left_player_in_other_server_count;
}notify_svrmgr_room_type_changed_t;

//通知
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	int changed_type;
	//在其他服务器上的玩家
	player_t left_player_in_other_server[MAX_PLYAER_COUNT];
	int left_player_in_other_server_count;
}notify_svrmgr_room_level_changed_t;

#endif