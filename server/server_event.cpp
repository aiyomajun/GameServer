/*
server_event.cpp主要用来接收客户端的命令
客户端的命令全全是用json格式的
*/
#include"server.hpp"
#include"server_event.hpp"
#include"../game_common/cmd.hpp"
#include"../game_common/room_mgr_on_ack.hpp"
#include"../game_common/server_common.hpp"

extern void websocket_send_data(const char* addr, int len, struct lws*wsi);
server_event_t all_event[]=
{
	{"SetPlayerName",UE4_on_set_player_name},
    {"GetAllDomain",UE4_on_get_all_domain},
    {"GetAllChannel",UE4_on_get_all_channel},
    {"GetAllRoom",UE4_on_get_all_room},
    {"SearchRoom",UE4_on_search_room},
	{"CreateRoom",UE4_on_create_room},
    {"CreateRoomAndEnter",UE4_on_create_room_and_enter},
    {"DestroyRoom",UE4_on_destroy_room},
    {"EnterRoom",UE4_on_enter_room},
    {"LeaveRoom",UE4_on_leave_room},
    {"UnreadyGame",UE4_on_unready_game},
    {"ReadyGame",UE4_on_ready_game},
    {"StartGame",UE4_on_start_game},
    {"ChangeRoomOwner",UE4_on_change_room_owner},
    {"ChangeRoomType",UE4_on_change_room_type},
    {"SelectHero",UE4_on_select_hero},
    {"KichOffPlayer",UE4_on_kick_off_player}
};

void event_parsing_engining(std::string &Event, CJsonObject &Data, struct lws *wsi)
{
	int i;
	for (i = 0; i < sizeof(all_event) / sizeof(server_event_t); i++)
	{
		if (all_event[i].Event == Event)
		{
			all_event[i].ue4_request_callback(Data, wsi);
			return;
		}
	}
	//没有找到，暂时什么也不做,
	//后面给ue4发一个通知
	printf("no suce event:%s\n", Event.c_str());
}

void UE4_on_set_player_name(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_get_all_domain(CJsonObject &param, struct lws *wsi)
{
	CJsonObject response;
	CJsonObject *d;
	std::string ret;
	server *s = server::get_instance();
	on_get_all_domain_t on;
	ack_get_all_domain_t ack;
	int code;
	int i;
	code = s->get_all_domain(&on, &ack);
	
	if (code == SERVER_CODE_OK)
	{
		printf("total_domain is %d\n", ack.total_domain);
		for (i = 0; i < ack.total_domain; i++)
		{
			d = new CJsonObject();
			d->Add("DomainName", ack.domain_info[i].name);
			d->Add("DomainId", ack.domain_info[i].id);
			response.Add(d);
			delete d;
		}
		ret = response.ToString();
		websocket_send_data(ret.c_str(), ret.length(), wsi);
	}
	else
	{
		printf("get all domain from room_mgr failed\n");
	}

}
void UE4_on_get_all_channel(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_get_all_room(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_search_room(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_create_room(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_create_room_and_enter(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_destroy_room(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_enter_room(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_leave_room(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_unready_game(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_ready_game(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_start_game(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_change_room_owner(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_change_room_type(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_select_hero(CJsonObject &param, struct lws *wsi)
{

}
void UE4_on_kick_off_player(CJsonObject &param, struct lws *wsi)
{

}