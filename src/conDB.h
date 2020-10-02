#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
/*
  DB와 관련된 함수들의 헤더파일이다.
  DB는 list.db와 packet.db가 있다.
*/
// IP or Port Banning function
void add_ban_port(unsigned short port, char* nickname);
void add_ban_ip(char* IPaddr, char* nickname);
void del_ban_port(int num);
void del_ban_ip(int num);
// Show Banned IP or Port List
int db_show_port();
int db_show_ip();
