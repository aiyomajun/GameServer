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
	    ����һ������صĽ��
		�ɹ�����0,ʧ�ܷ���-1
		*cmd�����������ճɹ���cmd��
		*data,���ճɹ������ݻ���
		max_len��data������󳤶�
		*len,���ճɹ���ʵ�����ݳ���
		*same_endian ��С���Ƿ���ͬ
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