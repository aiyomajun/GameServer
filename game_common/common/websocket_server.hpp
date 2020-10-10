/*
websocket的使用需要安装一个库
sudo apt-get install libwebsockets
*/
#ifndef _WEBSOCKET_SERVER_HPP_
#define _WEBSOCKET_SERVER_HPP_



#include <libwebsockets.h>
#include<iostream>

#ifndef WEBSOCKET_CLIENTMNG_ARRAY_HASH_LEN
#define WEBSOCKET_CLIENTMNG_ARRAY_HASH_LEN 109
#endif

#define WEBSOCKET_STATUS_CONNECTED 0
#define WEBSOCKET_STATUS_VERIFIED 1


typedef struct
{
	char *data;//请求的数据的首地址，由malloc开辟
	int len;//请求的数据的长度
	struct lws*wsi;
	bool need_to_free_data; //是否需要free掉data,只要长度不问0,一般都需要
	bool need_to_free_myself;//是否需要free这个结构体，一般都需要

}ARGS_FOR_WEBSOCKET;


typedef struct Tag_websocket_clientmng
{
	bool used;
	int status;
	struct lws *wsi;
	struct Tag_websocket_clientmng *next;
}WEBSOCKET_CLIENTMNG;


typedef void(*WEBSOCKET_REQUEST_CALLBACK)(char *, int, struct lws*);
typedef void(*WEBSOCKET_DISCONNECT_CALLBACK)(struct lws*);
typedef WEBSOCKET_DISCONNECT_CALLBACK WEBSOCKET_CONNECT_CALLBAK;

void websocket_server_entry(int port);
void websocket_send_data(const char* addr, int len, struct lws*wsi);

//数据收到的请求回调只能有一个
void reg_websocket_requests_callback(WEBSOCKET_REQUEST_CALLBACK callback);

//websocket 断开连接的回调,目前设计为一个,如果各个模块都需要知道的话，暂时先写在一个函数里面
void reg_websocket_disconnect_callback(WEBSOCKET_DISCONNECT_CALLBACK callback);

//websocket的客户端连接回调
void reg_websocket_connect_callback(WEBSOCKET_CONNECT_CALLBAK callback);


void websocket_clientmng_init();


#endif // !_WEBSOCKET_SERVER_HPP_



	




