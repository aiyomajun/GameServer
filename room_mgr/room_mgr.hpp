/*
class room_mgr
function: to manage all the rooms of the server
mode:singleton
author:majun
*/
#ifndef _ROOM_MGR_HPP
#define _ROOM_MGR_HPP
#include"../game_common/room_mgr_common.hpp"





class room_mgr
{
private:
    /*
    private member
    */
    static room_mgr* room_manager;//pointer pointed to self

    /*
    all_domain初始化的时候会创建一些空房间
    */
    domain_t all_domain[MAX_DOMAIN_COUNT];

private:
    /*
    private function
    */
    room_mgr();
    void room_init();
	inline void copy_player_t(player_t *dst, player_t *ori);
	inline void copy_position_t(position_t *dst, position_t*ori);
	inline int cmp_player_t(player_t &p1, player_t &p2);
	inline void position_t_init(position_t &po);//玩家进入房间后初始化

public:
    /*
    public function
    */
    static room_mgr* get_instance();
	//获取所有的domain的简要信息
    int get_all_domain(domain_info_t* domain_info, int max_len,int &out_len);

	//获取某个区下的所有channel简要信息
    int get_all_channel(int domain_id, channel_info_t* channel_info, int max_len, int &out_len);

	//获取某个channel下的所有房间信息
    int get_all_room(int domain_id, int channel_id, room_info_t* room_info, int max_len, int &out_len);

	//搜索由comment匹配的房间
    //int search_room(int domain_id, int channel_id, room_info_t* room_info, int max_len);

	//某位玩家申请加入某个房间
    int enter_room(int domain_id, int channel_id, int room_id, player_t& player,enter_room_info_t & enter_info);

	//某位玩家离开某个房间
	int leave_room(int domain_id, int channel_id, int room_id, player_t& player, leave_room_info_t &leaver_info);

	//创建房间并且加入这个房间
	int create_room_and_enter(int domain_id, int channel_id, room_create_info_t& info,int &room_id,int&room_pos);

	//创建房间但是不加入这个房间
	//int create_room(int domain_id, int channel_id, room_create_info_t& info, int& room_id);

	//销毁某个房间,如果是房主可以销毁，否则销毁失败
	int destroy_room(int domain_id, int channel_id, int room_id,player_t& player);

    //由player发来的开始战斗命令
	//int start_game(int domain_id, int channel_id, int room_id, player_t& player);

    //找到这个玩家所在的domain channel room
	//int get_room_of_player(player_t& player, player_state_t& ps);

    //改变房间的种类(玩法)
	//int change_room_type(int domain_id, int channel_id, int room_id, int type, player_t& player);

	//改变游戏难度
	//int change_room_level(int domain_id, int channel_id, int room_id, int level, player_t& player);

    //改变房主,只有房主才能操作
	//int change_room_owner(int domain_id, int channel_id, int room_id,player_t& player_from, player_t& player_to);

    //player_request请求剔除will_be_kicked_off玩家
	//int kick_off_player(int domain_id, int channel_id, int room_id, player_t& player_request, player_t& will_be_kicked_off);

};
#endif // !_ROOM_MGR_HPP

//待解决，怎么快速记录每个玩家在哪个房间