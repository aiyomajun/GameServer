#ifndef _SERVER_COMMON_HPP_
#define _SERVER_COMMON_HPP_
#include<iostream>
#include"../game_common/player.hpp"
typedef  int room_mgr_connect_t;
/*
server_t:describle a server machine
*/
typedef struct
{
	char ip[20];//每个服务器的ip 255.255.255.255 最多15个字节
	int port;//每个服务器的端口
	int max_allowed_client_count;//allow how many client to connect to it
	int current_connected_client_count;//current served for client counts
}server_t;


/*
client_t:to describe a client machine
*/

/*
每个客户端所在的server服务器的记录，为了防止频繁的申请释放内存，
在服务端起来的时候预先申请一些节点，然后将used字段置为false,
当客户端下线了，只是简单的将used字段置为false即可
当客户端上线了，将used字段置为false,并且重新赋值player和server_index字段即可
*/
typedef struct tag_client_state_t
{
	bool used;
	player_t player;//标记哪一个玩家上线，
	int server_index;//玩家在哪一个服务器上
	struct tag_client_state_t* next;
	//int player_state;//玩家的当前状态
}client_state_t;


#endif // !_SERVER_COMMON_HPP_


