
#ifndef _TCP_SERVER_HPP_
#define _TCP_SERVER_HPP_



#include "thread_pool.hpp"
#define TCP_MAX_DATA_LEN 4*1024*10
#define TCP_MAX_CONNECTION_COUNT 100000+6


#define REVERT_ENDIAN_INT32(cmd) ((cmd << 24) & 0xff000000) | ((cmd << 8) & 0xff0000) | \
				((cmd >> 24) & 0xff) | ((cmd << 8) & 0xff00)


enum TCP_SERVER_TYPE
{
	TCP_SERVER_TYPE_SINGLE_THREAD,
	TCP_SERVER_TYPE_MUTI_THREAD
};

typedef struct TAG_SOCKETMNG
{
	//int fd;
	bool used;//����socket������þ�̬����ʽ�������־��������ڵ��Ƿ���ʹ��
	pthread_mutex_t mutex;//ÿ�β������fdʱ��ֻ����һ���̲߳��������һ�߳�����
	char remote_ip[20];
	int remote_port;
	//int connected_time;//�ܹ���������ʱ��
	time_t last_chat_time;//���һ��ͨ�ŵ�ʱ���
	char buff[TCP_MAX_DATA_LEN + 1];//�ݴ�ͨ�Ź����е���ʱ���ݣ�
	int offset;//buff���������ݵ�ƫ��
}SOCKETMNG;

typedef void(*TCP_REQUEST_CALLBACK)(int fd,int cmd,char *addr,int len,bool same_endian);

typedef void(*TCP_DISCONNECTED_CALLBACK)(int);
typedef TCP_DISCONNECTED_CALLBACK TCP_CONNECT_CALLBACK;

void tcp_connect_common_entry(void * args);


class TCP_SERVER
{
public:
	static TCP_SERVER *get_instance();
	static TCP_SERVER *get_instance(TCP_SERVER_TYPE type, int max_conn);
	static void set_socket_nonblocking(int socket);
	static void send_data(int cmd,char *data, int len, int socket);
	void listen_and_serve(int port);
	void reg_requests_callback(TCP_REQUEST_CALLBACK callback);
	void reg_disconnected_callback(TCP_DISCONNECTED_CALLBACK callback);
	void reg_connected_callback(TCP_CONNECT_CALLBACK callback);
	SOCKETMNG *sockmng;//���10W��tcp����
	
	void empty_sockmng(int fd);
	TCP_REQUEST_CALLBACK tcp_request_callback;
	TCP_DISCONNECTED_CALLBACK tcp_disconnected_callback;
	TCP_CONNECT_CALLBACK tcp_connect_callback;
	int sockfd;
	int epfd;
	
private:
	TCP_SERVER();
	TCP_SERVER(TCP_SERVER_TYPE type, int max_conn);
	static TCP_SERVER * tcp_server;
	void epoll_del_and_close_fd(int fd);
	THREAD_POOL *thread_pool;
	bool tcp_server_use_mutithread;//�Ƿ�ʹ�ö��߳�
};

#endif // !_TCP_SERVER_HPP_