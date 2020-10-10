
/*
server_mgr.hpp
api to manage all the server
author:majun
*/

#ifndef _SERVER_MGR_HPP_
#define _SERVER_MGR_HPP_
#include"server_mgr_common.hpp"
#include"../game_common/player.hpp"

//客户端的状态存储用一级哈希
#define CLIENT_STATE_HASH 1001

/*
class server_mgr:to manage all the server maching
*/
class server_mgr
{
private:
	room_mgr_connect_t room_mgr_conn;//connection handle that connect to room_mgr

	server_t* all_server;//all the server available to serve client
	int server_count;// the server machine num 
	
	pthread_mutex_t client_state_lock[CLIENT_STATE_HASH];
	client_state_t* client_state[CLIENT_STATE_HASH];//record which server client is connected to

	static server_mgr* server_manager;//server_mgr handler

	server_mgr();
	void server_init();
	void client_state_init();

public:

	int hash_by_player(player_t& player);//每个player 映射的哈希值
	int fd_to_server_index(int fd);//每个socket到all_server的index映射
	static server_mgr* get_instance();


	////////////////由login上发来的命令begin
	//login 请求一个最小的负载均衡的服务器，返回填充st
	bool on_request_minimal_load_balance_server(server_t &st);
	////////////////由login上发来的命令end



	////////////////由server上发来的房间状态改变命令begin
	//某个玩家加入某个房间后由sever 送命令给server_mgr，然后server_mgr通知这个房间内的所有其他玩家
	void on_player_enter_room(room_identity_t &ri,player_t& player);

	//单个server上的某个玩家离开房间后，这个server发消息给server_mgr,然后server_mgr通知其他的server服务器告知玩家退 出房间
	void on_player_leaver_room(room_identity_t &ri, player_t& player);

	//当某个玩家被踢出房间时通知房间里面的其他玩家
	void on_player_kicked_off(room_identity_t &ri, player_t& player);

	//当房间类型改变时通知其他的玩家
	void on_room_type_changed(room_identity_t &ri, int changed_type);

	//当房间被房主销毁时，通知其他在房间的玩家
	void on_room_destroyed(room_identity_t &ri);

	//当房间的房主改变时，通知其他的玩家
	void on_room_owner_changed(room_identity_t &ri, player_t& new_owner);

	//当房主点击开始战斗的时候通知其他的房间里面的其他玩家开始战斗
	void on_start_game(room_identity_t &ri);

	//请求添加好友
	void on_request_add_friend(player_t& request_player, player_t &wanted_player);

	//请求添加好友应答
	void on_reply_add_friend(player_t& request_player, player_t &wanted_player);
	////////////////由server上发来的房间状态改变命令end


	
	////////////////由server上发来的每个client上线离线信息begin
	//玩家连上了server服务器后发送给server_mgr
	void on_client_connect_to_server(player_t& player,int server_index);

	//玩家从server上断开连接后发送给server_mgr
	void on_client_disconnect_from_sever(player_t& player,int server_index);
	////////////////由server上发来的每个client上线离线信息end
	
};




#endif // !_SERVER_MGR_HPP_

