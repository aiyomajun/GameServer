/*
server_mgr�յ���Ϣ��Ӧ��

*/
#ifndef _SERVER_MGR_ON_ACK_HPP
#define _SERVER_MGR_ON_ACK_HPP
#include"player.hpp"
#include"room_mgr_common.hpp"

//֪ͨserver_mgr����Ҽ��뷿��
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	player_t player_in;//���뷿������
	
	//�������������ϵ����
	player_t left_player_in_other_server[MAX_PLYAER_COUNT];
	int left_player_in_other_server_count;
}notify_svrmgr_player_enter_room_t;


//֪ͨserver_mgr������뿪����
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	player_t player_in;//���뷿������

	//�������������ϵ����
	player_t left_player_in_other_server[MAX_PLYAER_COUNT];
	int left_player_in_other_server_count;

}notify_svrmgr_player_leave_room_t;

//֪ͨserver_mgr�������͸ı�
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	int changed_type;
	//�������������ϵ����
	player_t left_player_in_other_server[MAX_PLYAER_COUNT];
	int left_player_in_other_server_count;
}notify_svrmgr_room_type_changed_t;

//֪ͨ
typedef struct
{
	int domain_id;
	int channel_id;
	int room_id;
	int changed_type;
	//�������������ϵ����
	player_t left_player_in_other_server[MAX_PLYAER_COUNT];
	int left_player_in_other_server_count;
}notify_svrmgr_room_level_changed_t;

#endif