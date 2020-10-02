/*
  DB Ban list에 항목을 추가하거나 제거하는 함수가 있는 코드.
  port/ip 각각에 대한 밴 목록 추가 함수가 있고,
  각각 제거 함수가 2개 씩 있어서 총 4개의 함수를 담고있다.
  형식은 모두 같이, DB를 열고, query를 보내고, db를 닫는 형식으로, DB는 list.db를 사용한다.
  쿼리문은 Insert (삽입), Delete(삭제)의 차이가 있다.
*/
#include "conDB.h"

void add_ban_port(unsigned short port, char* nickname)
{
  sqlite3 *db;
  char *err_msg = 0;
  int rc = sqlite3_open("list.db", &db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return;
  }
  sqlite3_busy_timeout(db, 50000);
  char *sql = sqlite3_mprintf("INSERT INTO PORT_LIST VALUES('%d', '%q')", port, nickname);
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK ) {

    fprintf(stderr, "SQL error: %s\n", err_msg);

    sqlite3_free(err_msg);
    sqlite3_close(db);

    return;
  }
  sqlite3_close(db);
  return;
}

void add_ban_ip(char* IPaddr, char* nickname)
{
  sqlite3 *db;
  char *err_msg = 0;
  int rc = sqlite3_open("list.db", &db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);

    return;
  }

  char *sql = sqlite3_mprintf("INSERT INTO IP_LIST VALUES('%q', '%q')", IPaddr, nickname);
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK ) {

    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);

    return;
  }
  sqlite3_close(db);
  return;
}

void del_ban_port(int num)
{
  sqlite3 *db;
  char *err_msg = 0;
  int rc = sqlite3_open("list.db", &db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);

    return;
  }

  char *sql = sqlite3_mprintf("DELETE FROM PORT_LIST WHERE rowid=%d", num);
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

  if (rc != SQLITE_OK ) {

    fprintf(stderr, "SQL error: %s\n", err_msg);

    sqlite3_free(err_msg);
    sqlite3_close(db);

    return;
  }
  sqlite3_exec(db, "VACUUM", 0, 0, &err_msg);
  sqlite3_close(db);
  return;
}
void del_ban_ip(int num)
{
  sqlite3 *db;
  char *err_msg = 0;
  int rc = sqlite3_open("list.db", &db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);

    return;
  }

  char *sql = sqlite3_mprintf("DELETE FROM IP_LIST WHERE rowid=%d", num);
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK ) {

    fprintf(stderr, "SQL error: %s\n", err_msg);

    sqlite3_free(err_msg);
    sqlite3_close(db);

    return;
  }
  sqlite3_exec(db, "VACUUM", 0, 0, &err_msg);

  sqlite3_close(db);
  return;
}
