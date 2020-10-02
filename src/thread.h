/*
  DB의 접근 권한을 각각의 함수에서 얻어내게 하기위해
  mutex만을 위한 헤더를 작성했다.
*/
#include <pthread.h>

pthread_mutex_t *mutex;
