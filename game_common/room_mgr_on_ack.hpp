#ifndef _ROOM_MGR_ON_ACK_HPP_
#define _ROOM_MGR_ON_ACK_HPP_
#include"cmd.hpp"
#include"player.hpp"
#include"room_mgr_common.hpp"

/*
房间相关的请求应答放在此文件下
*/
//////////////////////////////////////***************************/////////////////////////////////////////////
//获取所有的domani
typedef struct
{
    int no_use;//此命令不需要参数，但是为了避免发一个空消息，这里用no_use填充
}on_get_all_domain_t;

typedef struct
{
	int code;
    int total_domain;
    domain_info_t domain_info[MAX_DOMAIN_COUNT];
}ack_get_all_domain_t;//获取所有domain的回应

//////////////////////////////////////***************************/////////////////////////////////////////////
//获取所有的channel
typedef struct
{
    int domain_id;

}on_get_all_channel_t;

typedef struct
{
	int code;
    int total_channel;
    channel_info_t channel_info[MAX_CHANNEL_COUNT];
}ack_get_all_channel_t;

//////////////////////////////////////***************************/////////////////////////////////////////////
//获取所有的房间
typedef struct
{
    int domain_id;
    int channel_id;
}on_get_all_room_t;

typedef struct
{
	int code;
    int total_room;
    room_info_t room_info[MAX_ROOM_COUNT];
}ack_get_all_room_t;


//////////////////////////////////////***************************/////////////////////////////////////////////
//查询房间
typedef struct
{
	int domain_id;
	int channel_id;
	char comment[MAX_ROOM_COMMENT_LEN];
}on_search_room_t;

typedef struct
{
	int code;
	int domain_id;
	int channel_id;
	int total_room;
	room_info_t room_info[MAX_ROOM_COUNT];
}ack_search_room_t;

//////////////////////////////////////***************************/////////////////////////////////////////////
//加入房间
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	player_t player;
}on_enter_room_t;

typedef struct
{
	int code;//返回码
	int pos;//进入房间的位置

}ack_enter_room_t;
//////////////////////////////////////***************************/////////////////////////////////////////////
//离开房间
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	player_t player;
}on_leave_room_t;

typedef struct
{
	int code;//返回码
}ack_leave_room_t;
//////////////////////////////////////***************************/////////////////////////////////////////////
//创建房间并且加入
typedef struct
{
	int domain_id;
	int channel_id;
	player_t player;
}on_create_room_and_endter_t;

typedef struct
{
	int code;
	int pos;
}ack_create_room_and_enter_t;
//////////////////////////////////////***************************/////////////////////////////////////////////
//创建房间但是不加入房间,这个接口暂时不适用
typedef struct
{
	int domain_id;
	int channel_id;
	player_t player;
}on_create_room_t;

typedef struct
{
	int code;
}ack_create_room_t;
//////////////////////////////////////***************************/////////////////////////////////////////////
//销毁房间
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	player_t player;
}on_destroy_room_t;

typedef struct
{
	int code;
	
}ack_destroy_room_t;
//////////////////////////////////////***************************/////////////////////////////////////////////
//开始游戏
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	player_t player;
}on_start_game_t;
typedef struct
{
	int code;
	//
}ack_start_game_t;
//////////////////////////////////////***************************/////////////////////////////////////////////
//查询某个玩家在哪个房间里面
typedef struct
{
	player_t player;
}on_get_room_of_player_t;

typedef struct
{
	int code;
	//

}ack_get_room_of_player_t;
//////////////////////////////////////***************************/////////////////////////////////////////////
//改变房间类型
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	player_t player;
	int change_type;
}on_change_room_type_t;

typedef struct
{
	int code;

}ack_change_room_type_t;

//////////////////////////////////////***************************/////////////////////////////////////////////
//改变房间等级
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	player_t player;
	int level;
}on_change_room_level_t;
typedef struct
{
	int code;
}ack_change_room_level_t;
//////////////////////////////////////***************************/////////////////////////////////////////////
//改变房主
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	player_t old_owner;
	player_t new_player;
}on_change_room_owner_t;

typedef struct
{
	int code;

}ack_change_room_owner_t;

//////////////////////////////////////***************************/////////////////////////////////////////////
//踢走玩家
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	player_t kicker;
	player_t be_kicked;
}on_kick_player_off_t;

typedef struct
{
	int code;
}ack_kick_player_off_t;

#endif // !_ROOM_MGR_ON_ACK_HPP_
