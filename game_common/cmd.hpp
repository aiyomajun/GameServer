#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_
#define ENDIAN 0x12345678

//int fd, int len,char* data,bool same_endian
typedef void(*CMD_CALLBACK)(int, int, char*, bool);
typedef struct
{
	int cmd;
	CMD_CALLBACK cmd_callback;
}cmd_handle_t;

typedef struct
{
	int endian;//nomaly it is 0x12345678, if not the same endian it will be 0x78563412
	int cmd;
	int cmd_len;
	char data[0];//command data
}cmd_t;



/*
这里是room_mgr app所能收到的所有内部命令
*/
#define ROOM_MGR_CMD_BEGIN 100
#define ROOM_MGR_CMD_ON_GET_ALL_DOMAIN 100
#define ROOM_MGR_CMD_ON_GET_ALL_CHANNEL 101
#define ROOM_MGR_CMD_ON_GET_ALL_ROOM 102
#define ROOM_MGR_CMD_ON_SEARCH_ROOM 103
#define ROOM_MGR_CMD_ON_ENTER_ROOM 104
#define ROOM_MGR_CMD_ON_LEAVE_ROOM 105
#define ROOM_MGR_CMD_ON_CREATE_ROOM_AND_ENTER 106
#define ROOM_MGR_CMD_ON_CREATE_ROOM 107
#define ROOM_MGR_CMD_ON_DESTROY_ROOM 108
#define ROOM_MGR_CMD_ON_START_GAME 109
#define ROOM_MGR_CMD_ON_GET_ROOM_OF_PLAYER 110
#define ROOM_MGR_CMD_ON_CHANGE_ROOM_TYPE 111
#define ROOM_MGR_CMD_ON_CAHNGE_ROOM_OWNER 112
#define ROOM_MGR_CMD_ON_KICK_PLAYER_OFF 113

/*
这里是room_mgr app接受到命令后发出的回应
*/
#define ROOM_MGR_CMD_ACK_GET_ALL_DOMAIN 300
#define ROOM_MGR_CMD_ACK_GET_ALL_CHANNEL 301
#define ROOM_MGR_CMD_ACK_GET_ALL_ROOM 302
#define ROOM_MGR_CMD_ACK_SEARCH_ROOM 303
#define ROOM_MGR_CMD_ACK_ENTER_ROOM 304
#define ROOM_MGR_CMD_ACK_LEAVE_ROOM 305
#define ROOM_MGR_CMD_ACK_CREATE_ROOM_AND_ENTER 306
#define ROOM_MGR_CMD_ACK_CREATE_ROOM 307
#define ROOM_MGR_CMD_ACK_DESTROY_ROOM 308
#define ROOM_MGR_CMD_ACK_START_GAME 309
#define ROOM_MGR_CMD_ACK_GET_ROOM_OF_PLAYER 310
#define ROOM_MGR_CMD_ACK_CHANGE_ROOM_TYPE 311
#define ROOM_MGR_CMD_ACK_CAHNGE_ROOM_OWNER 312
#define ROOM_MGR_CMD_ACK_KICK_PLAYER_OFF 313



/*
这是server_mgr app所能收到的所有内部命令
*/
#define SERVER_MGR_CMD_BEGIN 600
#define SERVER_MGR_CMD_ON_PLAYER_ENTER_ROOM 601
#define SERVER_MGR_CMD_ON_PLAYER_LEAVE_ROOM 602
#define SERVER_MGR_CMD_ON_ROOM_DESTROYED 603
#define SERVER_MGR_CMD_ON_ROOM_TYPE_CHANGED 604
#define SERVER_MGR_CMD_ON_ROOM_LEVEL_CHANGED 605
#define SERVER_MGR_CMD_ON_GAME_STARTED 606



#endif // !_COMMAND_HPP_
