/*
websocket��ʹ����Ҫ��װһ����
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
	char *data;//��������ݵ��׵�ַ����malloc����
	int len;//��������ݵĳ���
	struct lws*wsi;
	bool need_to_free_data; //�Ƿ���Ҫfree��data,ֻҪ���Ȳ���0,һ�㶼��Ҫ
	bool need_to_free_myself;//�Ƿ���Ҫfree����ṹ�壬һ�㶼��Ҫ

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

//�����յ�������ص�ֻ����һ��
void reg_websocket_requests_callback(WEBSOCKET_REQUEST_CALLBACK callback);

//websocket �Ͽ����ӵĻص�,Ŀǰ���Ϊһ��,�������ģ�鶼��Ҫ֪���Ļ�����ʱ��д��һ����������
void reg_websocket_disconnect_callback(WEBSOCKET_DISCONNECT_CALLBACK callback);

//websocket�Ŀͻ������ӻص�
void reg_websocket_connect_callback(WEBSOCKET_CONNECT_CALLBAK callback);


void websocket_clientmng_init();


#endif // !_WEBSOCKET_SERVER_HPP_



	




