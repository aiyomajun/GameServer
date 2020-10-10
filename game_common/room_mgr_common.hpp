#ifndef _ROOM_COMMON_HPP_
#define _ROOM_COMMON_HPP_
#include "../game_common/player.hpp"

//这里是每个房间的宏定义
#define ROOM_STATE_NOT_FIGHTING 0 //房间没有战斗，在选英雄状态
#define ROOM_STATE_BEING_FIGHTING 1//the room is fighting

//这里是每个房间里面的玩家的状态的宏定义
#define PLAYER_STATE_NOT_READY 0//当前位置玩家没有就绪
#define PLAYER_STATE_READY 1//当前玩家已经就绪
#define PLAYER_STATE_IN_FIGHTING 2//当前玩家正在战斗，只有房间状态正在战斗时，玩家状态才能是此值


#define MAX_DOMAIN_COUNT 2  //整个服务器下最大的区的个数
#define MAX_CHANNEL_COUNT 10//每个区下的最大的频道数
#define MAX_ROOM_COUNT 50 //每个频道下最大的房间个数
#define MAX_PLYAER_COUNT 16//每个房间最大允许的玩家个数

//每个domain最多允许的玩家个数
#define MAX_ALLOWED_PLAYER_COUNT MAX_DOMAIN_COUNT*MAX_CHANNEL_COUNT*MAX_ROOM_COUNT*MAX_PLYAER_COUNT


//一些字符串长度的宏
#define MAX_ROOM_COMMENT_LEN 60


#define MAX_DOMAIN_NAME_LEN 20
#define MAX_CHANNEL_NAME_LEN 20
#define MAX_ROOM_NAME_LEN 20



enum room_mgr_code
{
	ROOM_CODE_OK = 0,//房间相关的操作成功
	ROOM_CODE_SERVER_ERROR,//服务器错误
	ROOM_CODE_PERMISSION_DENIED,//权限不够
	ROOM_CODE_PARAM_ERROR,
    ROOM_CODE_INVALID_DOMAIN,//无效的区号
	ROOM_CODE_INVALID_CHANNEL,//无效的频道号
	ROOM_CODE_INVALID_ROOM,//无效的房间号
	ROOM_CODE_ENTER_ROOM_FAIL_FOR_ROOM_IS_FIGHTING, //加入房间失败,房间以及开始战斗了
	ROOM_CODE_ENTER_ROOM_FAIL_FOR_ROOM_IS_FULL,//加入房间失败，房间已满
	ROOM_CODE_CREATE_ROOM_FAIL_FOR_NOT_ALLOWED_TO_CREATE_MORE_ROOM_IN_THIS_CHANNEL,//创建房间失败,此频道不允许创建更多的房间
	ROOM_CODE_DESTROY_ROOM_FAIL_FOR_ROOM_IS_FIGHTING,//销毁房间失败,房间正在战斗
	ROOM_CODE_DESTROY_ROOM_FAIL_FOR_PERNISSION_DENIED,//销毁房间失败，不是房主
	ROOM_CODE_START_GAME_FAIL,//开始游戏失败
};







//房间里面每个位置的描述
typedef struct
{
	bool used;//这个位置是否有玩家
	int player_state;//这个位置的玩家状态
	player_t player;//玩家的具体信息
	int hero;//选择英雄时会用到哪一个英雄
}position_t;


/*
每个房间在内存中的创建和删除并不会真正的生成和销毁那段内存，
当创建一个房间时，找到used为false的那个结构体，置为True就行,
如果找不到证明这个区域下已经达到了最大能创建的房间数了
当销毁一个房间的时候，将used改为False即可
*/
typedef struct//room
{
	bool used;//标注这个房间是否在使用
    int room_id;
	char room_name[MAX_ROOM_NAME_LEN];
	
	int room_type;//房间类型，游戏模式
	int room_level;//游戏难度
	char room_comment[MAX_ROOM_COMMENT_LEN];//this is the key for searching
	//bool need_password;
	//std::string password;

	player_t room_owner;//房主
	int room_state; //房间状态

	int max_allowed_player_count;//该房间允许的最大的玩家数量

	position_t position[MAX_PLYAER_COUNT];//房间里面的所有位置
	int player_in_room_count;

	pthread_mutex_t lock;//房间锁

	std::string fight_server_ip;//战斗服务器的ip
	int fight_server_port;//战斗服务器的port

}room_t;


//channel 管理结构体
typedef struct//channel
{
	int channel_id;
	char channel_name[MAX_CHANNEL_NAME_LEN];
	int room_count;
	room_t room[MAX_ROOM_COUNT];
	pthread_mutex_t lock;//每个频道下的锁，所有房间操作的锁

}channel_t;


//domain管理结构体
typedef struct//domain
{
	int domain_id;
    char domain_name[MAX_DOMAIN_NAME_LEN];
	int channel_count;
	channel_t channel[MAX_CHANNEL_COUNT];

}domain_t;


//每个domain的描述信息
typedef struct
{
	int id;
	char name[MAX_DOMAIN_NAME_LEN];
}domain_info_t;


//每个channel的描述信息
typedef struct
{
	int id;
	char name[MAX_CHANNEL_NAME_LEN];
	//int total_player_count;//当前频道下的玩家数
}channel_info_t;



//每个房间的简述描述信息
typedef struct
{
	int id;
	char name[MAX_ROOM_NAME_LEN];
	char comment[MAX_ROOM_COMMENT_LEN];
	int player_count_in_it;
	int room_state;
}room_info_t;


//每个玩家的状态信息，在每个domain下 应该有一个player_state_t ps[MAX_DOMAIN_ALLOWED_PLAYER_COUNT]的数组
//房间快速找到每个王家在哪个房间里面 ,每个玩家掉线或者加入退出房间都应该快速更新这个数据结构
typedef struct
{
	player_t player;
	int domain_id;
	int in_channel_id;
	int in_room_id;
}player_state_t;


typedef struct
{
	char name[MAX_ROOM_NAME_LEN];
	char comment[MAX_ROOM_COMMENT_LEN];
	int max_allowed_player_count;//创建该房间时最大的允许玩家的个数
	player_t creator;//创建者信息
	//bool need_password;
	//char password[20]
	int room_type;//房间类型，比如cf中的刀战，枪战等
	int level;//游戏难度
}room_create_info_t;


typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
}room_identity_t;//标识唯一一个房间

typedef struct
{
	int pos;//加入成功后自己的位置
	position_t orin[MAX_PLYAER_COUNT];//原先在房间里面的玩家
}enter_room_info_t;

typedef struct
{
	bool is_room_master;//离开的是否是房主
	int left_player_count;//玩家离开后剩余的玩家个数
	player_t left_player[MAX_PLYAER_COUNT];//房间里面剩下的玩家
	player_t next_master;//当且仅当is_room_master为true。并且left_player_count>0时才有意义
}leave_room_info_t;

#endif

