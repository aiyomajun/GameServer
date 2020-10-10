#include"room_mgr.hpp"
#include<string.h>
#include<string.h>
#include"../game_common/hero.hpp"

/*
room_manager的设计思想是只做一个房间的服务器，不主动向其他模块发送数据，
即使由于房主离开房间了，需要重新设置房主，也由server发送给server_mgr,然后server_mgr发送给其他的server，
最后server通知客户端
*/


room_mgr* room_mgr::room_manager = NULL;

room_mgr::room_mgr()
{
	room_init();
}

void room_mgr::room_init()
{
	//初始化所有的domain
	for (int i = 0; i < MAX_DOMAIN_COUNT; i++)
	{
		//初始化每个domain信息
		all_domain[i].channel_count = MAX_CHANNEL_COUNT;
		sprintf(all_domain[i].domain_name, "domain_%d", i);
		all_domain[i].domain_id = i;
		for (int j = 0; j < MAX_CHANNEL_COUNT; j++)
		{
			all_domain[i].channel[j].channel_id = j;
			sprintf(all_domain[i].channel[j].channel_name, "channel_%d", j);
			all_domain[i].channel[j].room_count = MAX_ROOM_COUNT;
			pthread_mutex_init(&all_domain[i].channel[j].lock, NULL);//每个频道下的锁初始化
			for (int k = 0; k < MAX_ROOM_COUNT; k++) //初始化每一个房间
			{
				if (k < 5)//初始化创建5个空房间
				{
					all_domain[i].channel[j].room[k].used = true;
					all_domain[i].channel[j].room[k].player_in_room_count = 0;
					all_domain[i].channel[j].room[k].room_state = ROOM_STATE_NOT_FIGHTING;//房间刚刚创好，还没有人
					all_domain[i].channel[j].room[k].max_allowed_player_count = MAX_PLYAER_COUNT;//该房间允许的最大的玩家数量
				}
				else
				{
					all_domain[i].channel[j].room[k].used = false;
					all_domain[i].channel[j].room[k].player_in_room_count = 0;
					//all_domain[i].channel[j].room[k].room_state = ROOM_STATE_NOT_FIGHTING;//房间刚刚创好，还没有人
				}

				for (int z = 0; z < MAX_PLYAER_COUNT; z++)
				{
					all_domain[i].channel[j].room[k].position[z].used = false;
				}

				pthread_mutex_init(&all_domain[i].channel[j].room[k].lock, NULL);//每个房间的锁初始化
				
			}
		}
	}
}

void room_mgr::copy_player_t(player_t * dst, player_t * ori)
{
	memcpy((void*)dst, (void*)ori, sizeof(player_t));
}

void room_mgr::copy_position_t(position_t * dst, position_t * ori)
{
	memcpy((void*)dst, (void*)ori, sizeof(position_t));
}

int room_mgr::cmp_player_t(player_t & p1, player_t & p2)
{
	return strcmp(p1.player_name, p2.player_name);
}

void room_mgr::position_t_init(position_t &po)
{
	po.used = true;//这个位置有玩家了
	po.hero = GAME_HERO_NO_HERO; //初始化为没选择英雄
	po.player_state = PLAYER_STATE_NOT_READY;//初始化状态为没准备好
}


room_mgr* room_mgr::get_instance()
{
	if (room_mgr::room_manager != NULL)
	{
		return room_manager;
	}
	room_mgr::room_manager = new room_mgr();
	return room_mgr::room_manager;

}


int room_mgr::get_all_domain(domain_info_t* domain_info, int max_len, int &out_len)
{

	if (max_len <=0)
	{
		out_len= 0;
		return ROOM_CODE_SERVER_ERROR;
	}
	if (max_len <= MAX_DOMAIN_COUNT)
	{
		for (int i = 0; i < max_len; i++)
		{
			domain_info[i].id = all_domain[i].domain_id;
			memcpy(domain_info[i].name, all_domain[i].domain_name, MAX_DOMAIN_NAME_LEN);
			out_len= max_len;
			//printf("name is %s,id is %d\n", domain_info[i].name, domain_info[i].id);
		}
		return ROOM_CODE_OK;
	}
	else
	{
		for (int i = 0; i < MAX_DOMAIN_COUNT; i++)
		{
			domain_info[i].id = all_domain[i].domain_id;
			memcpy(domain_info[i].name, all_domain[i].domain_name, MAX_DOMAIN_NAME_LEN);
			out_len= MAX_DOMAIN_COUNT;
		}
		return ROOM_CODE_OK;
	}
	return ROOM_CODE_SERVER_ERROR;
}


int room_mgr::get_all_channel(int domain_id, channel_info_t* channel_info, int max_len, int &out_len)
{
	if (max_len <= 0)
	{
		out_len = 0;
		return ROOM_CODE_SERVER_ERROR;
	}
	if (domain_id<0 || domain_id>MAX_DOMAIN_COUNT - 1)
	{
		return ROOM_CODE_INVALID_DOMAIN;
	}
	if (max_len <= MAX_CHANNEL_COUNT)
	{
		for (int i = 0; i < max_len; i++)
		{
			channel_info[i].id = all_domain[domain_id].channel[i].channel_id;
			memcpy(channel_info[i].name, all_domain[domain_id].channel[i].channel_name, MAX_CHANNEL_NAME_LEN);
			out_len= max_len;
			
		}
		return ROOM_CODE_OK;
	}
	else
	{
		for (int i = 0; i < MAX_CHANNEL_COUNT; i++)
		{
			channel_info[i].id = all_domain[domain_id].channel[i].channel_id;
			memcpy(channel_info[i].name, all_domain[domain_id].channel[i].channel_name, MAX_CHANNEL_NAME_LEN);
			out_len= MAX_CHANNEL_COUNT;
			
		}
		return ROOM_CODE_OK;
	}
	return ROOM_CODE_SERVER_ERROR;
}

//返回room_info的最大填充索引.-1表示一个也没有 0表示有一个,依次类推
int room_mgr::get_all_room(int domain_id, int channel_id, room_info_t* room_info, int max_len, int &out_len)
{

	if (domain_id<0 || domain_id>MAX_DOMAIN_COUNT - 1)
	{
		return ROOM_CODE_INVALID_DOMAIN;
	}
	if (channel_id<0 || channel_id>MAX_CHANNEL_COUNT - 1)
	{
		return ROOM_CODE_INVALID_CHANNEL;
	}
	if (max_len <= 0)
	{
		return ROOM_CODE_SERVER_ERROR;
	}
	//先给频道上个锁
	pthread_mutex_lock(&all_domain[domain_id].channel[channel_id].lock);
	int total=0;
	int room_index = 0;
	channel_t* chan = &all_domain[domain_id].channel[channel_id];
	while (total <= max_len && room_index < MAX_ROOM_COUNT)
	{
		//pthread_mutex_lock(&chan->room[room_index].lock);
		if (chan->room[room_index].used)
		{
			room_info[total].id = chan->room[room_index].room_id;
			memcpy(room_info[total].name, chan->room[room_index].room_name, MAX_ROOM_NAME_LEN);
			memcpy(room_info[total].comment, chan->room[room_index].room_comment, MAX_ROOM_COMMENT_LEN);
			room_info[total].player_count_in_it = chan->room[room_index].player_in_room_count;
			room_info[total].room_state = chan->room[room_index].room_state;
			total++;
		}
		//pthread_mutex_unlock(&chan->room[room_index].lock);
		room_index++;
	}
	pthread_mutex_unlock(&all_domain[domain_id].channel[channel_id].lock);
	out_len= total;
	return ROOM_CODE_OK;
}


/*
玩家加入房间
todo:记录一个玩家在哪个房间里面，便于快速查找
*/
int room_mgr::enter_room(int domain_id, int channel_id, int room_id, player_t& player, enter_room_info_t &enter_info)
{
	if (domain_id<0 || domain_id>MAX_DOMAIN_COUNT - 1)
	{
		return ROOM_CODE_INVALID_DOMAIN;
	}
	if (channel_id<0 || channel_id>MAX_CHANNEL_COUNT - 1)
	{
		return ROOM_CODE_INVALID_CHANNEL;
	}
	if (room_id<0 || room_id>MAX_ROOM_COUNT - 1)
	{
		return ROOM_CODE_INVALID_ROOM;
	}

	int count = 0;
	int i;
	bool entered = false;
	room_t* room = &all_domain[domain_id].channel[channel_id].room[room_id];
	if (!room->used)
	{

	}
	pthread_mutex_lock(&room->lock);
	if (room->room_state == ROOM_STATE_BEING_FIGHTING)//房间正在战斗
	{
		pthread_mutex_unlock(&room->lock);
		return ROOM_CODE_ENTER_ROOM_FAIL_FOR_ROOM_IS_FIGHTING;
	}


	for (i = 0; i < room->max_allowed_player_count; i++)
	{
		if (!room->position[i].used)
		{
			room->position[i].used = true;
			room->position[i].player_state = PLAYER_STATE_NOT_READY;
			room->position[i].hero = GAME_HERO_NO_HERO;//还没选英雄
			copy_player_t(&room->position[i].player, &player);//
			enter_info.pos = i;
			room->player_in_room_count++;
			entered = true;
		}
	}

	if (entered)//房间没满可以加入房间
	{
		for (i = 0; i < room->max_allowed_player_count; i++)//把房间里面的剩余人拷贝进去
		{
			copy_position_t(&enter_info.orin[i], &room->position[i]);
		}
		pthread_mutex_unlock(&room->lock);
		return ROOM_CODE_OK;
	}

	//房间满了加入失败
	pthread_mutex_unlock(&room->lock);
	return ROOM_CODE_ENTER_ROOM_FAIL_FOR_ROOM_IS_FULL;
}


/*
玩家离开房间
todo:删除记录一个玩家在哪个房间里面，便于快速查找
*/
int room_mgr::leave_room(int domain_id, int channel_id, int room_id, player_t& player, leave_room_info_t& leaver_info)
{
	if (domain_id<0 || domain_id>MAX_DOMAIN_COUNT - 1)
	{
		return ROOM_CODE_INVALID_DOMAIN;
	}
	if (channel_id<0 || channel_id>MAX_CHANNEL_COUNT - 1)
	{
		return ROOM_CODE_INVALID_CHANNEL;
	}
	if (room_id<0 || room_id>MAX_ROOM_COUNT - 1)
	{
		return ROOM_CODE_INVALID_ROOM;
	}
	int left_count = 0;//房间里面剩余的玩家个数
	room_t* room = &all_domain[domain_id].channel[channel_id].room[room_id];

	if (!room->used)
	{

	}


	position_t* po = room->position;
	//给房间加锁
	pthread_mutex_lock(&room->lock);

	leaver_info.is_room_master = false;//初始化不是房主离开房间

    //轮询每一个位置上的玩家，把剩余的人拷贝到leave_info里面
	for (int i = 0; i < room->max_allowed_player_count; i++)
	{
		if (po[i].used)//这个位置有玩家
		{
			if (cmp_player_t(po[i].player, player) != 0)//当前位置不是要离开房间的人
			{
				copy_player_t(&leaver_info.left_player[left_count], &po[i].player);
				left_count++;
			}
			else//当前位置是要离开的玩家
			{
				po[i].used = false;//把当前位置空出来
				if (cmp_player_t(po[i].player, room->room_owner) == 0)//如果是房主要离开房间
				{
					leaver_info.is_room_master = true;
				}
			}
		}
	}
	if (leaver_info.is_room_master = true)//如果是房主离开了房间
	{
		if (left_count == 0)//玩家离开房间后，房间没人了，销毁房间
		{
			room->used = false;
			room->player_in_room_count = 0;
		}
		else//房间里面还有人，设置新的房主
		{
			for (int i = 0; i < room->max_allowed_player_count; i++)
			{
				if (po[i].used)
				{
					//让第一个人成为新的房主
					copy_player_t(&room->room_owner, &po[i].player);
					copy_player_t(&leaver_info.next_master, &po[i].player);
					break;
				}
			}
		}

	}
	pthread_mutex_unlock(&room->lock);
	return ROOM_CODE_OK;
}


/*
玩家创建房间并且加入房间
todo:记录一个玩家在哪个房间里面，便于快速查找
*/
int room_mgr::create_room_and_enter(int domain_id, int channel_id, room_create_info_t & info, int & room_id, int & room_pos)
{
	if (domain_id<0 || domain_id>MAX_DOMAIN_COUNT - 1)
	{
		return ROOM_CODE_INVALID_DOMAIN;
	}
	if (channel_id<0 || channel_id>MAX_CHANNEL_COUNT - 1)
	{
		return ROOM_CODE_INVALID_CHANNEL;
	}


	//判断参数的合法性
	if (info.max_allowed_player_count <= 0)
	{
		return ROOM_CODE_PARAM_ERROR;
	}


	//先给频道上个锁
	channel_t *chan = &all_domain[domain_id].channel[channel_id];
	room_t *room;
	pthread_mutex_lock(&chan->lock);
	for (int i = 0; i < MAX_ROOM_COUNT; i++)
	{
		if (!chan->room[i].used)
		{
			chan->room_count++;
			//pthread_mutex_lock(room->lock);
			room = &chan->room[i];
			room->used = true;
			room->room_level = info.level;
			room->room_type = info.room_type;
			room->max_allowed_player_count = info.max_allowed_player_count;
			copy_player_t(&room->room_owner, &info.creator);
			room->room_state = ROOM_STATE_NOT_FIGHTING;
			memcpy(room->room_comment, info.comment, MAX_ROOM_COMMENT_LEN);
			for (int k = 0; k < room->max_allowed_player_count; k++)
			{
				room->position[k].used = false;
			}
			//把第一个位置给创建者
			room->position[0].used = true;
			copy_player_t(&room->position[0].player, &info.creator);
			room->position[0].player_state = PLAYER_STATE_NOT_READY;
			room->position[0].hero = GAME_HERO_NO_HERO;//还没选任何英雄
			room_pos = 0;

			//把房主也给创建者
			copy_player_t(&room->room_owner, &info.creator);

			//pthread_mutex_unlock(room->lock);
			pthread_mutex_unlock(&chan->lock);
			return ROOM_CODE_OK;
		}
	}
	pthread_mutex_unlock(&chan->lock);
	//当前频道下房间已经创满了
	return ROOM_CODE_CREATE_ROOM_FAIL_FOR_NOT_ALLOWED_TO_CREATE_MORE_ROOM_IN_THIS_CHANNEL;
}

int room_mgr::destroy_room(int domain_id, int channel_id, int room_id, player_t& player)
{
	if (domain_id<0 || domain_id>MAX_DOMAIN_COUNT - 1)
	{
		return ROOM_CODE_INVALID_DOMAIN;
	}
	if (channel_id<0 || channel_id>MAX_CHANNEL_COUNT - 1)
	{
		return ROOM_CODE_INVALID_CHANNEL;
	}
	if (room_id<0 || room_id>MAX_ROOM_COUNT - 1)
	{
		return ROOM_CODE_INVALID_ROOM;
	}
	room_t* room = &all_domain[domain_id].channel[channel_id].room[room_id];
	pthread_mutex_lock(&room->lock);
	if (!room->used)
	{

	}
	if (cmp_player_t(room->room_owner, player) != 0)//要销毁房间的不是房主
	{
		pthread_mutex_unlock(&room->lock);
		return ROOM_CODE_PERMISSION_DENIED;
	}

	room->used = false;//销毁房间
	pthread_mutex_unlock(&room->lock);
	return ROOM_CODE_OK;
}
