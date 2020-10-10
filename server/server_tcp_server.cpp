#include<stdio.h>
void tcp_connect_callback(int fd)
{
	printf("tcp %d connect\n", fd);
}
void tcp_disconnect_callback(int fd)
{
	printf("tcp %d disconnect\n", fd);
}

void tcp_request_callback(int fd, int cmd, char *data, int len, bool same_endian)
{
	//printf("in tcprequest\n");
	printf("fd is %d,cmd is %d, data_len is %d,same_endian is %d\n", fd, cmd, len, same_endian);
	for (int i = 0; i < len; i++)
	{
		printf("%c", data[i]);
	}
	printf("\n");
	//printf("\nout tcprequest\n");

}