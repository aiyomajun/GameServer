#include "server_mgr.hpp"



server_mgr* server_mgr::server_manager = NULL;
server_mgr::server_mgr()
{
	server_init();
	client_state_init();
}
void server_mgr::server_init()
{
	//读取配置文件,然后填充all_server
	//这里先一次性弄过求,假定只有一个server服务器
	all_server = (server_t*)malloc(sizeof(server_t) * 1);
	server_count = 1;
	all_server[0].ip = "127.0.0.1";
	all_server[0].port = 10000;
}
void server_mgr::client_state_init()
{
	register int i;
	register int j;
	client_state_t* tmp;
	client_state_t* tail;
    //先每个哈希头节点预先申请100个节点，用来存放后面上线的客户端信息
	//如果节点不够后面再动态开辟
	for ( i = 0; i < CLIENT_STATE_HASH; i++)
	{
		for (j = 0; j < 100; j++)
		{
			tmp = (client_state_t*)malloc(sizeof(client_state_t));
			tmp->used = false;
			tmp->next = NULL;
			
			if (j == 0)
			{
				client_state[i] = tmp;
				tail = tmp;
			}
			else
			{
				tail->next = tmp;
				tail = tmp;
			}
		}
	}
	//每个哈希链表的锁的初始化
	for (i = 0; i < CLIENT_STATE_HASH, i++)
	{
		pthread_mutex_init(&client_state_lock[i], NULL);
	}

}
int server_mgr::hash_by_player(player_t& player)
{
	unsigned int *t = (unsigned int*)player.player_name;
	index = *t % CLIENT_STATE_HASH;
	return index;
}
server_mgr* server_mgr::get_instance()
{
	if (server_manager != NULL)
	{
		return server_manager;
	}
	server_manager = new server_mgr();
	return server_manager;
}

//返回一个负载均衡最低的服务器
bool server_mgr::on_request_minimal_load_balance_server(server_t& st)
{
	memcpy(st.ip, all_server[0].ip);
	st.port = all_server[0].port;
	return true;
}

//当每个客户端连接到服务器后就通知服务器，记录该客户端的信息
void server_mgr::on_client_connect_to_server(player_t& player, int server_index)
{
	index = hash_by_player(player);
	client_state_t* head = client_state[index];
	client_state_t* tmp = head;
	client_state_t* mm;
	pthread_mutex_lock(&client_state_lock[index]);
	while (tmp != NULL)
	{
		if (tmp->used)
		{
			tmp = tmp->next;
			continue;
		}
		tmp->used = true;
		tmp->player = player;
		tmp->server_index = server_index;
		pthread_mutex_unlock(&client_state_lock[index]);
		return;

	}
	//预先开辟的空间不够了，整个列表全部占满了，新开辟一个空间放到链表尾部
	mm = malloc(sizeof(client_state_t));
	mm->next = NULL;
	mm->player = player;
	mm->used = true;
	mm->server_index = server_index;
	tmp->next = mm;
	pthread_mutex_unlock(&client_state_lock[index]);

}

void server_mgr::on_client_disconnect_from_sever(player_t& player, int server_index)
{
	index = hash_by_player(player);
	client_state_t* head = client_state[index];
	pthread_mutex_lock(&client_state_lock[index]);
	while (head != NULL)
	{
		if (strcmp(head->player.player_name, player.player_name) == 0)
		{
			head->used = false;
			pthread_mutex_unlock(&client_state_lock[index]);
			return;
		}
		head = head->next;
	}
	pthread_mutex_unlock(&client_state_lock[index]);
}
