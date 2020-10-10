#include "tcp_client.hpp"
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<errno.h> 
#include<sys/types.h> 
#include<sys/socket.h> 
#include<netinet/in.h> 
#include<arpa/inet.h>
#include<unistd.h>

TCP_CLIENT::TCP_CLIENT()
{
	memset(ip, 0, sizeof(ip));
	connected = false;
}

TCP_CLIENT::~TCP_CLIENT()
{
	close(sockfd);
}

int TCP_CLIENT::tryconnect(const char * ip, int port)
{
	strcat(this->ip, ip);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(port);

	//向服务器发送连接请求
	int ret=connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (ret < 0)
	{
		printf("connect to server failed\n");
		connected = false;
		close(sockfd);
	}
	else
	{
		printf("connect to server success\n");
		connected = true;
	}
	return ret;
	
}

bool TCP_CLIENT::send_data(int cmd, char * data, int len, int sockfd)
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

	if (!connected)//和server的连接处于断开状态
	{
		disconnect();
		if (tryconnect(ip, port) < 0)
		{
			return false;
		}

	}


	char buff[40960];//最大发送40960字节的数据
	int *ptr = (int*)buff;
	ptr[0] = 0x12345678;
	ptr[1] = cmd;
	ptr[2] = len;
	//ptr[3] = 0;
	char *d = buff + 4 * sizeof(int);
	if (len > 0)
	{
		memcpy(d, data, len);
	}
	send(sockfd, buff, len + sizeof(int) * 4, 0);
	return true;
}

//成功返回0，失败返回-1,并且断开此tcp连接
int TCP_CLIENT::recv_data(int *cmd, char *data, int max_len, int *len, bool *same_endian)
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
	int endian;
	int ret;
	int h[4];
	ret = recv(sockfd, (char*)h, sizeof(h), MSG_WAITALL);
	if (ret < 0)//出错了，由于是设置了recv的超时时间，有可能是没有任何数据到来
	{
		if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
		{
			//到这里没有错,有可能是超时了，但是还是当做错误处理
		}
		else//sockfd遇到了错误
		{

		}
		//虽然上面的if else没有用，但是留着，知道recv可能会遇到什么。
		disconnect();
		return -1;
	}
	else if (ret == 0)//对端关闭了连接
	{
		disconnect();
		return -1;
	}
	else//在这里才是在超时时间内收到了数据，但是不一定是收到了16个字节
	{
		if (ret < 16)
		{
			disconnect();
			return -1;
		}
		endian = h[0];
		if (endian != 0x12345678)
		{
			*cmd = REVERT_ENDIAN_INT32(h[1]);
			*len = REVERT_ENDIAN_INT32(h[2]);	
			*same_endian = false;
		}
		else
		{
			*cmd = h[1];
			*len = h[2];
			*same_endian = true;
		}
	}

	if (max_len < *len)//接收缓冲区长度不够
	{
		disconnect();
		return -1;
	}

	ret = recv(sockfd, data, *len, MSG_WAITALL);
	if (ret < 0)//出错了，由于是设置了recv的超时时间，有可能是没有任何数据到来
	{
		if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
		{
			//到这里没有错,有可能是超时了，但是还是当做错误处理
		}
		else//sockfd遇到了错误
		{

		}
		//虽然上面的if else没有用，但是留着，知道recv可能会遇到什么。
		disconnect();
		return -1;
	}
	else if (ret == 0)//对端关闭了连接
	{
		disconnect();
		return -1;
	}
	else//在这里才是在超时时间内收到了数据，但是不一定是收到了*len个字节
	{
		if (ret < *len)
		{
			disconnect();
			return -1;
		}
	}
	return 0;
}

void TCP_CLIENT::set_recv_timeout(int second, int ms)
{
	struct timeval timeout;
	timeout.tv_sec = second;
	timeout.tv_usec = ms;
	int ret;
	ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
}

void TCP_CLIENT::disconnect()
{
	close(sockfd);
	connected = false;
}

bool TCP_CLIENT::is_connected()
{
	return connected;
}

int TCP_CLIENT::get_sockfd()
{
	return sockfd;
}
