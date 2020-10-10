#include"../game_common/common/websocket_server.hpp"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include"../game_common/common/websocket_server.hpp"
#include"../game_common/common/CJsonObject.hpp"
#include"../game_common/common/thread_pool.hpp"
THREAD_POOL	*thread_pool_for_websocket = NULL;
bool websocket_use_muti_threads = true;




extern void event_parsing_engining(std::string &Event, CJsonObject &Data, struct lws *wsi);

void websocket_server_init()
{
	websocket_use_muti_threads = true;
	thread_pool_for_websocket = new THREAD_POOL(8);
}



void ue4_requests_comes(char *data, int len, struct lws* wsi)
{
	/*
	char buff[40960];
	char *tt;
	CJsonObject *co;
	if (len < 40960-1)
	{
		memcpy(buff, data, len);
		buff[len] = '\0';
		co = new CJsonObject(buff);
	}
	else
	{
		tt =(char*) malloc(len + 1);
		memcpy(tt, data, len);
		tt[len] = '\0';
		co = new CJsonObject(tt);
		free(tt);
	}
	*/
	data[len] = '\0';
	CJsonObject co(data);
	CJsonObject Data;
	std::string Event;
	//这里是具体的event解析
	if(!co.Get("Event", Event))
	{
		printf("parse Event failed\n");
		return;//不用回复消息
	}
	if(!co.Get("Data", Data))
	{
		printf("parse Data failed\n");
		return;//不用回复消息
	}
	event_parsing_engining(Event, Data, wsi);
}

void websocket_thread_requests_fun(void *args)
{
	ARGS_FOR_WEBSOCKET* a = (ARGS_FOR_WEBSOCKET*)args;
	ue4_requests_comes(a->data, a->len, a->wsi);
	if (a->need_to_free_data)
	{
		free(a->data);
	}
	if (a->need_to_free_myself)
	{
		free(a);
	}
}

void websocket_request_callback_fun(char * addr, int len, struct lws*wsi)
{
	/*
	printf("in websocket_fun\n");
	for (int i = 0; i < len; i++)
	{
		printf("%c", addr[i]);
	}
	printf("\n");
	*/

	if (!websocket_use_muti_threads)//单线程的websocket 走这
	{
		char * tm = (char *)malloc(len + 1);
		memcpy(tm, addr, len);
		ue4_requests_comes(tm, len, wsi);
		free(tm);
	}
	else //多线程的websocket走这
	{
		//assert(thread_pool_for_websocket);
		ARGS_FOR_WEBSOCKET *args = (ARGS_FOR_WEBSOCKET*)malloc(sizeof(ARGS_FOR_WEBSOCKET));
		args->need_to_free_myself = true;

		if (len <= 0)
		{
			args->data = NULL;
			args->need_to_free_data = false;
			args->len = len;
			args->wsi = wsi;

		}
		else
		{
			//这里申请len+1长度的字节，因为websocket和ue4的通信是采用的json格式
			args->data = (char*)malloc(len+1);
			memcpy(args->data, addr, len);
			args->need_to_free_data = true;
			args->len = len;
			args->wsi = wsi;
		}

		thread_pool_for_websocket->throw_task_to_me(websocket_thread_requests_fun, (void*)args);

	}

}

void websocket_disconnect_callback_fun(struct lws*wsi)
{
	printf("websocket client %ld disconnected\n", (long int)wsi);
}

void websocket_connect_callback_fun(struct lws*wsi)
{
	printf("here comes a websocket connection %ld\n", (long int)wsi);
}



void* start_websocket_server(void *args)
{
	int port = reinterpret_cast<long>(args);
	websocket_server_init();
	
	reg_websocket_requests_callback(websocket_request_callback_fun);


	reg_websocket_disconnect_callback(websocket_disconnect_callback_fun);

	
	reg_websocket_connect_callback(websocket_connect_callback_fun);

	websocket_server_entry(port);
}
