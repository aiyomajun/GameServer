#ifndef _SERVER_EVENT_HPP_
#define _SERVER_EVENT_HPP_
#include<iostream>
#include"../game_common/common/CJsonObject.hpp"
struct lws;
typedef void(*UE4_REQUEST_CALLBACK)(CJsonObject &, struct lws *);

typedef struct
{
	std::string Event;//
	//std::string description;
	UE4_REQUEST_CALLBACK ue4_request_callback;
}server_event_t;

//所有Event的主入口
void event_parsing_engining(const std::string &Event, CJsonObject &Data,struct lws *wsi);

void UE4_on_set_player_name(CJsonObject &param, struct lws *wsi);
void UE4_on_get_all_domain(CJsonObject &param, struct lws *wsi);
void UE4_on_get_all_channel(CJsonObject &param, struct lws *wsi);
void UE4_on_get_all_room(CJsonObject &param, struct lws *wsi);
void UE4_on_search_room(CJsonObject &param, struct lws *wsi);
void UE4_on_create_room(CJsonObject &param, struct lws *wsi);
void UE4_on_create_room_and_enter(CJsonObject &param, struct lws *wsi);
void UE4_on_destroy_room(CJsonObject &param, struct lws *wsi);
void UE4_on_enter_room(CJsonObject &param, struct lws *wsi);
void UE4_on_leave_room(CJsonObject &param, struct lws *wsi);
void UE4_on_unready_game(CJsonObject &param, struct lws *wsi);
void UE4_on_ready_game(CJsonObject &param, struct lws *wsi);
void UE4_on_start_game(CJsonObject &param, struct lws *wsi);
void UE4_on_change_room_owner(CJsonObject &param, struct lws *wsi);
void UE4_on_change_room_type(CJsonObject &param, struct lws *wsi);
void UE4_on_select_hero(CJsonObject &param, struct lws *wsi);
void UE4_on_kick_off_player(CJsonObject &param, struct lws *wsi);


void UE4_on_send_to_room(CJsonObject &, struct lws *);
void UE4_on_send_to_world(CJsonObject &, struct lws *);

#endif