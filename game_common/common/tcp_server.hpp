
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
	bool used;//由于socket管理采用静态管理方式，这个标志标明这个节点是否在使用
	pthread_mutex_t mutex;//每次操作这个fd时，只允许一个线程操作，另个一线程阻塞
	char remote_ip[20];
	int remote_port;
	//int connected_time;//总共的连接总时长
	time_t last_chat_time;//最近一次通信的时间点
	char buff[TCP_MAX_DATA_LEN + 1];//暂存通信过程中的临时数据，
	int offset;//buff中有用数据的偏移
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
	SOCKETMNG *sockmng;//最大10W个tcp连接
	
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
	bool tcp_server_use_mutithread;//是否使用多线程
};

#endif // !_TCP_SERVER_HPP_