/*
  list.db, packet.db 에 있는 항목들을 보여주는 함수가 있는 코드.
  SELECT 문으로 db의 항목들을 불러오기 때문에 따로 callback함수가 존재한다.
  각 callback함수는 db에서 찾은 항목들 (banned_ip, banned_port, current packet)을
  콘솔창에 출력해주는 역할을 한다.
  패킷을 가져오는 쿼리는 count를 사용해서 각 IP들에 대해 얼마만큼의 패킷들의 이동이 있었는지를
  보여준다. 
*/
#include "conDB.h"
#include "thread.h"

int num;
int line;
int count;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   printf("%3s", argv[0]);
   printf("%19s", argv[1]);
   printf("%13s", argv[2] ? argv[2] : "NULL");
   printf("\n");
   line = atoi(argv[0]);
   return 0;
}
static int callback_port(void *NotUsed, int argc, char **argv, char **azColName){
   fprintf(stdout, "\033[%d;50H",count++);
   printf("%3s", argv[0]);
   printf("%19s", argv[1]);
   printf("%13s", argv[2] ? argv[2] : "NULL");
   printf("\n");
   line = atoi(argv[0]);
   return 0;
}
static int callback_packet(void *NotUsed, int argc, char **argv, char **azColName)
{
  printf("%3d", num++);
  printf("%19s", argv[0]);
  printf("%13d", atoi(argv[1]));
  if(atoi(argv[2]) == 1){
    printf("\033[1;34m%11s\033[0m", "PASSED");
  } else {
    printf("\033[1;31m%11s\033[0m", "BANNED");
  }
  printf("\n");
  return 0;
}

int db_show_port()
{
  sqlite3 *db;
  sqlite3_stmt *res;
  char *err_msg = 0;
  char **result;
  int rc;
  line = 0;
  count = 3;
  /* DB OPEN */
  rc = sqlite3_open("list.db", &db);
  if( rc ){
     fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
     return ;
  }

  /* Get DB Column*/
  char *query = "SELECT rowid, PORT, NAME from PORT_LIST";

  fprintf(stdout, "\033[2;50H");
  printf("%3s|%18s|%12s\n","NUM", "PORT", "NAME");

  pthread_mutex_lock(mutex); // 잠금을 생성한다.
  rc = sqlite3_exec(db, query, callback_port, result, &err_msg);
  pthread_mutex_unlock(mutex); // 잠금을 해제한다.

  if(rc != SQLITE_OK)
  {
    sqlite3_free(err_msg);
  }

  sqlite3_close(db);
  return line;
}

int db_show_ip()
{
  sqlite3 *db;
  sqlite3_stmt *res;
  char *err_msg = 0;
  char **result;
  int rc;
  line = 0;
  /* DB OPEN */
  rc = sqlite3_open("list.db", &db);
  if( rc ){
     fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
     return ;
  }

  /* Get DB Column*/
  char *query = "SELECT rowid, IP, NAME from IP_LIST";

  printf("%3s|%18s|%12s\n","NUM", "IP", "NAME");

  pthread_mutex_lock(mutex); // 잠금을 생성한다.
  rc = sqlite3_exec(db, query, callback, result, &err_msg);
  pthread_mutex_unlock(mutex); // 잠금을 해제한다.

  if(rc != SQLITE_OK)
  {
    sqlite3_free(err_msg);
  }

  sqlite3_close(db);
  return line;
}

int db_show_packet()
{
  sqlite3 *db;
  sqlite3_stmt *res;
  char *err_msg = 0;
  int rc;
  num = 1;
  line = 0;

  rc = sqlite3_open("packet.db", &db);
  if( rc ){
     fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
     return ;
  }

  /* Get DB Column*/
  char *query = "SELECT DST_IP, COUNT(DST_IP), STATE FROM PACKET GROUP BY DST_IP, STATE ORDER BY COUNT(DST_IP) ASC";   // 목적주소, 소스주소, 소스 맥주소, 몇 번 방문했는지.

  printf("%3s|%18s|%12s|%10s\n","NUM", "DST_IP", "COUNT", "STATE");
  rc = sqlite3_exec(db, query, callback_packet, 0, &err_msg);

  if(rc != SQLITE_OK)
  {
    sqlite3_free(err_msg);
  }

  sqlite3_close(db);

  return line;
}

void db_del_packet()
{
  sqlite3 *db;
  sqlite3_stmt *res;
  char *err_msg = 0;
  int rc;
  num = 1;
  line = 0;

  rc = sqlite3_open("packet.db", &db);
  if( rc ){
     fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
     return ;
  }

  /* Get DB Column*/
  char *query = "DELETE FROM PACKET";   // 목적주소, 소스주소, 소스 맥주소, 몇 번 방문했는지.

  pthread_mutex_lock(mutex); // 잠금을 생성한다.
  rc = sqlite3_exec(db, query, callback_packet, 0, &err_msg);
  pthread_mutex_unlock(mutex); // 잠금을 생성한다.

  if(rc != SQLITE_OK)
  {
    sqlite3_free(err_msg);
  }

  sqlite3_close(db);

  return;
}
