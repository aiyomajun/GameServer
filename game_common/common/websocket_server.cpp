
#include <assert.h>
#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "websocket_server.hpp"
WEBSOCKET_REQUEST_CALLBACK websockt_request_callback = NULL;
WEBSOCKET_DISCONNECT_CALLBACK websocket_disconnect_callback = NULL;
WEBSOCKET_CONNECT_CALLBAK websocket_connect_callback = NULL;

WEBSOCKET_CLIENTMNG *websocket_client[WEBSOCKET_CLIENTMNG_ARRAY_HASH_LEN];

/* *
 * websocket_write_back: write the string data to the destination wsi.
 */
int websocket_write_back(struct lws *wsi_in, const char *str, int str_size_in)
{
	if (str == NULL || wsi_in == NULL)
		return -1;

	int n;
	int len;
	unsigned char *out = NULL;

	if (str_size_in < 1)
		len = strlen(str);
	else
		len = str_size_in;

	out = (unsigned char *)malloc(sizeof(unsigned char)*(LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING));
	//* setup the buffer*/
	memcpy(out + LWS_SEND_BUFFER_PRE_PADDING, str, len);
	//* write out*/
	n = lws_write(wsi_in, out + LWS_SEND_BUFFER_PRE_PADDING, len, LWS_WRITE_TEXT);

	//printf(KBLU"[websocket_write_back] %s\n"RESET, str);
	//* free the buffer*/
	free(out);

	return n;
}

void websocket_send_data(const char* addr, int len, struct lws*wsi)
{
	websocket_write_back(wsi, addr, len);
}

static int ws_service_callback(
	struct lws *wsi,
	enum lws_callback_reasons reason, void *user,
	void *in, size_t len)
{

	switch (reason) {

	case LWS_CALLBACK_ESTABLISHED:
		printf("[Main Service] Connection established\n");
		if (websocket_connect_callback)
		{
			websocket_connect_callback(wsi);
		}
		break;

		//* If receive a data from client*/
	case LWS_CALLBACK_RECEIVE:
		if (websockt_request_callback)
		{
			websockt_request_callback((char*)in, len, wsi);
		}
		break;
	case LWS_CALLBACK_CLOSED:
		printf("[Main Service] Client close.\n");
		if (websocket_disconnect_callback)
		{
			websocket_disconnect_callback(wsi);
		}
		break;

	default:
		break;
	}
	return 0;
}

struct per_session_data {
	int fd;
};

void reg_websocket_requests_callback(WEBSOCKET_REQUEST_CALLBACK callback)
{
	websockt_request_callback = callback;
}

void reg_websocket_disconnect_callback(WEBSOCKET_DISCONNECT_CALLBACK callback)
{
	websocket_disconnect_callback = callback;
}

void reg_websocket_connect_callback(WEBSOCKET_CONNECT_CALLBAK callback)
{
	websocket_connect_callback = callback;
}

void websocket_server_entry(int port)
{
	const char *interface = NULL;
	struct lws_context_creation_info info;
	struct lws_protocols protocol;
	struct lws_context *context;
	// Not using ssl
	const char *cert_path = NULL;
	const char *key_path = NULL;
	// no special options
	int opts = 0;


	//* register the signal SIGINT handler */
	/*
	struct sigaction act;
	act.sa_handler = INT_HANDLER;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction( SIGINT, &act, 0);
	*/

	//* setup websocket protocol */
	protocol.name = "yoke-protocol";
	protocol.callback = ws_service_callback;
	protocol.per_session_data_size = sizeof(struct per_session_data);
	protocol.rx_buffer_size = 0;

	//* setup websocket context info*/
	memset(&info, 0, sizeof info);
	info.port = port;
	info.iface = interface;
	info.protocols = &protocol;
	info.extensions = lws_get_internal_extensions();
	info.ssl_cert_filepath = cert_path;
	info.ssl_private_key_filepath = key_path;
	info.gid = -1;
	info.uid = -1;
	info.options = opts;

	//* create libwebsocket context. */
	context = lws_create_context(&info);
	if (context == NULL) {
		printf("[Main] Websocket context create error.\n");
		return ;
	}

	printf("[Main] Websocket context create success.\n");

	//* websocket service */
	while (1) {
		lws_service(context, 50);
	}
	usleep(10);
	//will never get here,just show how to destroy websock server
	lws_context_destroy(context);
	return;
}

void websocket_clientmng_init()
{
	for (int i = 0; i < WEBSOCKET_CLIENTMNG_ARRAY_HASH_LEN; i++)
	{

	}
}