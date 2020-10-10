#define REVERT_ENDIAN_INT32(cmd) ((cmd << 24) & 0xff000000) | ((cmd << 8) & 0xff0000) | \
				((cmd >> 24) & 0xff) | ((cmd << 8) & 0xff00)


class TCP_CLIENT
{
public:
	TCP_CLIENT();
	~TCP_CLIENT();
	int tryconnect(const char* ip, int port);
	bool send_data(int cmd, char *data, int len, int socket);
	/*
	    接收一个命令返回的结果
		成功返回0,失败返回-1
		*cmd，用来填充接收成功的cmd，
		*data,接收成功的数据缓存
		max_len，data区的最大长度
		*len,接收成功的实际数据长度
		*same_endian 大小端是否相同
	*/
	int recv_data(int *cmd, char *data, int max_len, int *len, bool *same_endian);
	void set_recv_timeout(int second,int ms);//ms
	void disconnect();
	bool is_connected();
	int get_sockfd();
private:
	int sockfd;
	bool connected;
	char ip[20];
	int port;
};