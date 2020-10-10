#include "tcp_server.hpp"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <iostream>
#include <fcntl.h>

TCP_SERVER* TCP_SERVER::tcp_server = NULL;

void TCP_SERVER::set_socket_nonblocking(int socket)
{
	int flags = fcntl(socket, F_GETFL, 0);
	fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

//默认是一个使用多线程的
TCP_SERVER::TCP_SERVER()
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		exit(1);
	}
	tcp_request_callback = NULL;
	tcp_disconnected_callback = NULL;
	tcp_connect_callback = NULL;
	sockmng = (SOCKETMNG*)malloc(sizeof(SOCKETMNG)*TCP_MAX_CONNECTION_COUNT);
	for (int i = 0; i < TCP_MAX_CONNECTION_COUNT; i++)
	{
		sockmng[i].used = false;
		sockmng[i].offset = 0;
		pthread_mutex_init(&sockmng[i].mutex, NULL);
	}
	tcp_server_use_mutithread = true;
	thread_pool = new THREAD_POOL(8);
}

TCP_SERVER::TCP_SERVER(TCP_SERVER_TYPE type, int max_conn)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		exit(1);
	}
	tcp_request_callback = NULL;
	tcp_disconnected_callback = NULL;
	tcp_connect_callback = NULL;
	sockmng = (SOCKETMNG*)malloc(sizeof(SOCKETMNG)*max_conn);
	for (int i = 0; i < max_conn; i++)
	{
		sockmng[i].used = false;
		sockmng[i].offset = 0;
		pthread_mutex_init(&sockmng[i].mutex, NULL);
	}
	if (type == TCP_SERVER_TYPE_MUTI_THREAD)
	{
		tcp_server_use_mutithread = true;
		thread_pool = new THREAD_POOL(8);
	}
	else
	{
		tcp_server_use_mutithread = false;
		thread_pool = NULL;
	}
}

void TCP_SERVER::epoll_del_and_close_fd(int fd)
{
	
	epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);
}


void TCP_SERVER::reg_requests_callback(TCP_REQUEST_CALLBACK callback)
{
	tcp_request_callback = callback;
}


void TCP_SERVER::reg_disconnected_callback(TCP_DISCONNECTED_CALLBACK callback)
{
	tcp_disconnected_callback = callback;
}

void TCP_SERVER::reg_connected_callback(TCP_CONNECT_CALLBACK callback)
{
	tcp_connect_callback = callback;
}

TCP_SERVER* TCP_SERVER::get_instance()
{
	if (tcp_server)
	{
		return tcp_server;
	}
	tcp_server = new TCP_SERVER();
	return tcp_server;
}

TCP_SERVER * TCP_SERVER::get_instance(TCP_SERVER_TYPE type, int max_conn)
{
	if (tcp_server)
	{
		return tcp_server;
	}
	tcp_server = new TCP_SERVER(type, max_conn);
	return tcp_server;
	
}

void TCP_SERVER::empty_sockmng(int fd)
{
	sockmng[fd].used = false;
	sockmng[fd].offset = 0;
}


void TCP_SERVER::send_data(int cmd, char *data, int len, int socket)
{
	/*
	{
		int endian;
		int cmd;
		int len;
		int no_use;
		char data[0];
	}
	*/
	char buff[40960];
	int *ptr = (int*)buff;
	ptr[0] = 0x12345678;
	ptr[1] = cmd;
	ptr[2] = len;
	//ptr[3] = 0;
	char *d = buff + 4 * sizeof(int);
	if(len>0)
	{ 
		memcpy(d, data, len);
	}
	send(socket, buff, len + sizeof(int) * 4, 0);
}


void tcp_connect_common_entry(void * args)
{
	int fd;
	int ret;
	int *tmp;
	int endian;
	int cmd;
	int len;
	int no_use;
	bool same_endian = true;
	int left_len;
	char *ttmmpp;
	fd=reinterpret_cast<long>(args);
	TCP_SERVER * tcp = TCP_SERVER::get_instance();
	SOCKETMNG *mng = &tcp->sockmng[fd];
	//printf("aaaaaaaaaaaaaaaaa\n");
	if (pthread_mutex_trylock(&mng->mutex) == EBUSY)
	{
		return;
	}
	//printf("bbbbbbbbbbbbb\n");
	char *buff = mng->buff;
	ret = recv(fd, buff + mng->offset, TCP_MAX_DATA_LEN - mng->offset-1, 0);
	//printf("recv %d byte\n", ret);
	if (mng->offset >= TCP_MAX_DATA_LEN)
	{
		//»º³åÇøÂúÁË
	}
	if (ret < 0)
	{
		if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
		{

		}
		else
		{
			close(fd);
			epoll_ctl(tcp->epfd, EPOLL_CTL_DEL, fd, NULL);
			if (tcp->tcp_disconnected_callback)
			{
				tcp->tcp_disconnected_callback(fd);
			}
			tcp->empty_sockmng(fd);
	
		}	
	}
	else if (ret == 0)//¶Ô¶Ë¹Ø±ÕÁËÁ¬½Ó
	{
		//printf("client closed socket fd is%d\n",fd);
		close(fd);
		epoll_ctl(tcp->epfd, EPOLL_CTL_DEL, fd, NULL);
		if (tcp->tcp_disconnected_callback)
		{
			tcp->tcp_disconnected_callback(fd);
		}
		tcp->empty_sockmng(fd);

	}
	else//ÔÚÕâÀï²ÅÊÇsocketÕæÕýÊÕµ½ÁËÊý¾Ý
	{
		mng->offset += ret;
		/*
		ÅÐ¶ÏÊÇ·ñÊÕµ½ÁËÒ»Ìõ»òÕßÈô¸ÉÌõºÏ·¨µÄÏûÏ¢
		{
	        int endian;
	        int cmd;
	        int len;
	        int no_use;
	        char data[0];
		}
		*/
		left_len = mng->offset;
		ttmmpp = mng->buff;
		do
		{
			//printf("cccccccccccccccc\n");
			if (left_len < 4 * sizeof(int))//Í·»¹Ã»ÓÐÊÕÍê
			{
				if (left_len > 0)
				{
					char * m = (char*)malloc(left_len);
					memcpy(m, ttmmpp, left_len);
					memcpy(mng->buff, m, left_len);
					free(m);
				}
				mng->offset = left_len;
				break;
			}
			tmp =(int*) ttmmpp;
			endian = tmp[0];
			cmd = tmp[1];
			len = tmp[2];
			//no_use = tmp[3];
			same_endian = true;
			if (endian != 0x12345678)//´óÐ¡¶Ë²»¶Ô
			{
				same_endian = false;
				cmd = REVERT_ENDIAN_INT32(cmd);
				len = REVERT_ENDIAN_INT32(len);
			}
			if ((left_len - 4 * sizeof(int)) >= len)//´Õ¹»ÁËÒ»Ìõcmd
			{
				//printf("go to tcp request\n");
				if (tcp->tcp_request_callback)
				{
					tcp->tcp_request_callback(fd, cmd, ttmmpp+4*sizeof(int), len, same_endian);
				}
				ttmmpp = ttmmpp +4*sizeof(int)+ len;
				left_len = left_len - 4 * sizeof(int) - len;

			}
			else
			{
				if (left_len > 0)
				{
					char * m = (char*)malloc(left_len);
					memcpy(m, ttmmpp, left_len);
					memcpy(mng->buff, m, left_len);
					free(m);
				}
				mng->offset = left_len;
				break;
			}
		} while (1);
	}
	//printf("dddddddddddddddddd\n");
	pthread_mutex_unlock(&mng->mutex);
	//printf("eeeeeeeeeeeeeeeeeeee\n");

}

void TCP_SERVER::listen_and_serve(int port)
{
	int ret;
	int client_sockfd;
	struct sockaddr_in serv_addr;
	struct sockaddr_in client_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(port);
	//°ó¶¨ÎÄ¼þÃèÊö·ûºÍ·þÎñÆ÷µÄipºÍ¶Ë¿ÚºÅ
	bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	//½øÈë¼àÌý×´Ì¬£¬µÈ´ýÓÃ»§·¢ÆðÇëÇó
	listen(sockfd, TCP_MAX_CONNECTION_COUNT);
	struct epoll_event ev, events[100];
	ev.events = EPOLLIN;
	ev.data.fd = sockfd;
	epfd = epoll_create(TCP_MAX_CONNECTION_COUNT);
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

	int len = sizeof(struct sockaddr_in);
	int nfds;
	while (1)
	{

		nfds = epoll_wait(epfd, events, 100, 3000);
		//printf("nfds=%d\n", nfds);
		for (int i = 0; i < nfds; i++)
		{
			if (events[i].data.fd == sockfd)//Èç¹ûÊÇ¼àÌý¶Ë¿ÚÓÐÁ¬½Óµ½À´
			{
				client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, (socklen_t*)&len);
				//printf("here comes a connect fd is %d\n", client_sockfd);

				//利用client_addr把ip和port记录

				
				if (client_sockfd < 0)
				{
					printf("accept fail\n");
					continue;
				}
				if (tcp_connect_callback)
				{
					tcp_connect_callback(client_sockfd);
				}
				sockmng[client_sockfd].used = true;

				ev.events = EPOLLIN | EPOLLET;//ÉèÖÃÎªË®Æ½´¥·¢
				ev.data.fd = client_sockfd;
				set_socket_nonblocking(client_sockfd);//ÉèÖÃºóÐøµÄsocketÎª·Ç×èÈûÄ£Ê½
				epoll_ctl(epfd, EPOLL_CTL_ADD, client_sockfd, &ev);

			}
			else if (events[i].events & EPOLLIN)//¶ÔÓ¦µÄfd¶ËÓÐÊý¾Ýµ½À´
			{
				if (tcp_server_use_mutithread)//多线程
				{
					thread_pool->throw_task_to_me(tcp_connect_common_entry, reinterpret_cast<void*>(events[i].data.fd));
				}
				else
				{
					tcp_connect_common_entry(reinterpret_cast<void*>(events[i].data.fd));
				}

			}
		}
	}



}