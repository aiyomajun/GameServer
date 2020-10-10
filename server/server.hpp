/*
server.hpp run in a spesific server machine
*/

#ifndef _SERVER_HPP_
#define _SERVER_HPP_
#include<iostream>
#include"../game_common/common/tcp_client.hpp"
#include"../game_common/common/tcp_server.hpp"
#include"../game_common/room_mgr_on_ack.hpp"
#include"../game_common/room_mgr_common.hpp"


/*
class server
server 接受客户端和server_mgr的命令
*/
class server
{
private:
	static server* svr;
	server();
public:
	static server* get_instance();
	TCP_CLIENT *room_mgr_conn;//连接到room_mgr的tcp连接
	TCP_CLIENT *server_mgr_conn;//连接到server_mgr的tcp连接
	/*
	说明下，server和server_mgr会建立两个tcp连接
	其中一个server_mgr作为服务端，sever作为客户端，
	这面的命令都是server主动发出，server_mgr被动回答
	另一个是server作为服务端，server_mgr作为客户端
	这里的命令都是server_mgr主动发出，server被动回答
	*/
	TCP_SERVER *server_server; //供server_mgr连接的tcp服务端

public:
	////////////////////////////////////////////////////////////
	int get_all_domain(on_get_all_domain_t *on, ack_get_all_domain_t * ack);

	//获取某个区下的所有channel简要信息
	int get_all_channel(on_get_all_channel_t *on,ack_get_all_channel_t *ack);

	//获取某个channel下的所有房间信息
	int get_all_room(on_get_all_room_t *on,ack_get_all_room_t *ack);

	//搜索由comment匹配的房间
	//int search_room(on_search_room_t * on,ack_search_room_t *ack);

	//某位玩家申请加入某个房间
	int enter_room(on_enter_room_t*on,ack_enter_room_t *ack);

	//某位玩家离开某个房间
	int leave_room(on_leave_room_t*on,ack_leave_room_t *ack);

	//创建房间并且加入这个房间
	int create_room_and_enter(on_create_room_and_endter_t *on,ack_create_room_and_enter_t *ack);

	//创建房间但是不加入这个房间
	//int create_room(on_create_room_t *on,on_create_room_t *ack);

	//销毁某个房间,如果是房主可以销毁，否则销毁失败
	int destroy_room(on_destroy_room_t *on,ack_destroy_room_t *ack);

	//由player发来的开始战斗命令
	int start_game(on_start_game_t *on,ack_start_game_t *ack);

	//找到这个玩家所在的domain channel room
	int get_room_of_player(on_get_room_of_player_t *on,ack_get_room_of_player_t *ack);

	//改变房间的种类(玩法)
	int change_room_type(on_change_room_type_t *on,ack_change_room_type_t *ack);

	//改变游戏难度
	int change_room_level(on_change_room_level_t *on,ack_change_room_level_t *ack);

	//改变房主,只有房主才能操作
	int change_room_owner(on_change_room_owner_t *on,ack_change_room_owner_t *ack);

	//踢掉玩家
	int kick_off_player(on_kick_player_off_t *on,ack_kick_player_off_t *ack);


public:
	//通知server_mgr 玩家加入房间
	//int notify_server_mgr_player_enter_room();
};

#endif // !_SERVER_HPP_
