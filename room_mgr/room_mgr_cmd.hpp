
#ifndef _ROOM_MGR_CMD_HPP_
#define _ROOM_MGR_CMD_HPP_

void room_mgr_cmd_entry(int fd, int cmd, char* data, int len, bool same_endian);

void room_mgr_get_all_domain(int fd, int len, char*data, bool same_endian);
void room_mgr_get_all_channel(int fd, int len, char*data, bool same_endian);
void room_mgr_get_all_room(int fd, int len, char*data, bool same_endian);
void room_mgr_search_room(int fd, int len, char*data, bool same_endian);
void room_mgr_enter_room(int fd, int len, char*data, bool same_endian);
void room_mgr_leave_room(int fd, int len, char*data, bool same_endian);
void room_mgr_create_room_and_enter(int fd, int len, char*data, bool same_endian);
void room_mgr_create_room(int fd, int len, char*data, bool same_endian);
void room_mgr_destroy_room(int fd, int len, char*data, bool same_endian);
void room_mgr_start_game(int fd, int len, char*data, bool same_endian);
void room_mgr_get_room_of_player(int fd, int len, char*data, bool same_endian);
void room_mgr_change_room_type(int fd, int len, char*data, bool same_endian);
void room_mgr_change_room_level(int fd, int len, char*data, bool same_endian);
void room_mgr_change_room_owner(int fd, int len, char*data, bool same_endian);
void room_mgr_kick_off_player(int fd, int len, char*data, bool same_endian);

#endif
