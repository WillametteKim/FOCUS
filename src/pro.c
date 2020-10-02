/*
	메인 메뉴를 관장하는 부분으로, 메뉴를 선택받으면 각각 에 대한 알맞은 함수를 호출하는 역할을 한다.
	차단 리스트 추가, 차단 리스트 보기, 현재 패킷 보기 등의 함수들을 입력에 따라 호출하고,
	깔끔한 UI를 제공하기 위해 콘솔 창을 조작하는 역할도 수행한다.
	처음 main의 시작과 함께, bittwist 함수를 생성한 thread에 할당해 주므로써
	프로그램이 켜지는 동안 bridge의 기능, 그리고 패킷 캡쳐를 백그라운드에서 계속해서 진행하도록
	구성했다.
*/
#include <stdio.h>
#include <stdlib.h>
#include "conDB.h"
#include "def.h"
#include "thread.h"
#include <pthread.h>  //thread create
#include <signal.h>		// thread kill

/* 차단 */
void chadan();
void delete(int line);
void set_list_db(int obt);
/* 차단 리스트 보기 */
void show_list();
void get_list_db();
/* 현재 패킷 보기 */
void show_state();
void menu_timer();

void semiClear(int line);
void devideline(int line);
int max(int a, int b);

/* 전역 변수 */
pthread_t thread;
int thr_id = 0;
int h= 0;
int m= 0;

void main()
{
	int menu;

	mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex, NULL );

	thr_id = pthread_create(&thread, NULL, bittwist, 0);

	while (1) {
		system("clear");
		printf("\033[%dm%s\033[0m\n", 93, "<< Menu >>");
		printf("1. Config Ban List\n");
		printf("2. Show Ban List\n");
		printf("3. Show current packet\n");
		printf("4. Set alarm\n");
		printf("5. Exit\n");

		printf("Select menu : ");
		scanf("%d", &menu);

		switch (menu)
		{
		case 1: chadan(); break;
		case 2: show_list(); break;
		case 3: show_state(); break;
		case 4: menu_timer(); break;
		case 5:
		  pthread_kill(thread, 0);
			exit(0);
		default:
			break;
		}
	}
	return;
}

void chadan()
{
	int port;
	char name[10], ip[16];
	int menu;

	system("clear");
  printf("\033[%dm%s\033[0m\n", 93, "<<Add ban>>");
	printf("1. IP\n");
	printf("2. PORT\n");
	printf("3. Go to main\n");
	printf("Select menu : ");
	scanf("%d", &menu);

	switch (menu)
	{
	case 1:
		printf("IP : ");
		scanf("%s", ip);
		printf("Nickname : ");
		scanf("%s", name);
		add_ban_ip(ip, name);
		break;
	case 2:
		printf("PORT : ");
		scanf("%d", &port);
		printf("Nickname : ");
		scanf("%s", name);
		add_ban_port(port, name);
		break;
	default:
		break;
	}
	return;
}

void delete(int line)
{
	int menu, num=0;

	semiClear(line);
	printf("1. IP\n");
	printf("2. PORT\n");
	printf("3. Back\n");
	printf("Select menu : ");
	scanf("%d", &menu);
	switch (menu) {
		case 1:
			semiClear(line);
		 	printf("IP - select NUM : ");
			scanf("%d", &num);
			del_ban_ip(num);
			break;
		case 2:
			semiClear(line);
			printf("PORT - select NUM : ");
			scanf("%d", &num);
			del_ban_port(num); break;
		case 3: break;
	}
	return;
}
void show_list()
{
	int size_ip_list = 10;
	int size_port_list = 10;
	int i, menu = 1;
	int line, line1, line2;

	while (menu) {

		system("clear");
    printf("\033[%dm%s\033[0m\n", 93, "\t<<== IP Ban List ==>>");
		line1 = db_show_ip();

		fprintf(stdout, "\033[1;50H");
    printf("\033[%dm%s\033[0m\n", 93, "\t<<== PORT Ban List ==>>");
		line2 = db_show_port();
		line = max(line1, line2) + 3;

		devideline(line);
		fprintf(stdout, "\033[%d;1H", line);
		printf("1. Add Ban\n");
		printf("2. Delete Ban\n");
		printf("3. Go to Main\n");
		printf("Select menu : ");
		scanf("%d", &menu);

		switch (menu) {
		case 1: chadan(); break;
		case 2:
		delete(line); break;
		case 3: menu = 0; break;
		}
	}
	return;
}

void show_state()
{
	int menu;
	char select;
	system("clear");

	while(1){
		system("clear");
		printf("\033[%dm%s\033[0m\n", 93, "   <<== Current Packet List ==>>");
		db_show_packet();
		printf("1. Clean PACKET List\n");
		printf("2. Back to Main\n");
		printf("Select menu : ");
		scanf("%d", &menu);

		switch (menu) {
			case 1:
				printf("Really? (y/n)");
				scanf(" %c", &select);
				if(select == 'y') db_del_packet();
				break;
			case 2: return;
		}
	}

	return;
}

void menu_timer()
{
	int menu = 1;
	int pid;
	system("clear");
	printf("\033[%dm%s\033[0m\n", 93, "<< Set Timer >>");

	while(menu){
		if(h){
			printf("\033[0;36m%s >> %d : %d\033[0m\n", "Alarm", h, m);
			printf("1. Reset alarm\n");
			printf("2. Go back menu\n");
			printf("Select menu : ");
			scanf("%d", &menu);
			if(menu==2) menu = 0;
			else if(menu==1) h = 0;
		} else{
			printf("Set Hour : ");
			scanf("%d", &h);
			printf("Set Minute : ");
			scanf("%d", &m);
			pid = fork();
			if(pid == 0){
				alarm2(h, m);
				exit(0);
			}
			system("clear");
		}
	}

	return;
}

void semiClear(int line)
{
	fprintf(stdout, "\033[%d;1H", line);
	printf("                      \n");
	printf("                      \n");
	printf("                      \n");
	printf("                      \n");
	fprintf(stdout, "\033[%d;1H", line);
	return;
}
void devideline(int line)
{
	int i ;
	for(i=2;i<line;i++){
		fprintf(stdout, "\033[%d;43H", i);
		printf("\033[0;36m%s\033[0m\n", "||");
	}
}
int max(int a, int b)
{
	int c;
	if(a > b) c = a;
	else c =b;
	return c;
}
