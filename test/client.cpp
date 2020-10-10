/*client_tcp.c*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<errno.h> 
#include<signal.h>
#include<execinfo.h>
#include"../game_common/common/tcp_client.hpp"
#include"../game_common/cmd.hpp"
#include"../game_common/room_mgr_on_ack.hpp"
void exception_entry(int signum)
{
	int n;
	int i;
	char **s;
	void* buff[10];
	printf("here comes a bug,see the log file\n");
	n = backtrace((void **)&buff, sizeof(buff) / sizeof(void*));
	s = backtrace_symbols(buff, n);
	if (s == NULL)
	{
		return;
	}
	for (i = 0; i < n; i++)
	{
		printf("%s\n", s[i]);
	}
	free(s);
	_exit(1);
}
void handle_exception()
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGSEGV, exception_entry);
	signal(SIGABRT, exception_entry);
	signal(SIGILL, exception_entry);
	signal(SIGFPE, exception_entry);
}


void * recv_entry(void * args)
{
	int fd = (int)args;

}

TCP_CLIENT tcp;
void handle_command(int cmd);
int main(int argc ,char *argv[]){
	//创建套接字

	if (argc != 3)
	{
		printf("arg error run as ./client 127.0.0.01 10001\n");
		exit(2);
	}
	tcp.tryconnect(argv[1], atoi(argv[2]));
	printf("connect resul is %d\n", tcp.is_connected());
	while (1)
	{
		int cmd;
		printf("请输入命令\n");
		scanf("%d", &cmd);
		handle_command(cmd);
	}
}
void handle_command(int cmd)
{

	tcp.send_data(cmd,)
}