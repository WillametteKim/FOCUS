#include "alarm.h"

/* pro.c에서 호출되는 함수. 시간과 분을 받아 알람을 울린다.*/
int alarm2(int h, int m)
{
  int time = set_timer(h, m);
  signal(SIGALRM, sigint_hanlder);
  alarm(time);
  while(1){
    pause();
    break;
  }
}
/* omxplayer를 사용하기위해 signal을 보내는 함수다. */
void sigint_hanlder(int signo){
  system("omxplayer ./example.mp3 >> log.txt");
}
/* 현재 시간이 설정된 시간과 같으면 sigint_handler를 통해 알람을 울린다 */
int set_timer(int h, int m){
  time_t rawtime;
  struct tm * timeinfo;
  int cur = 0, set = 0, diff = 0;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  cur = (timeinfo->tm_hour)*60+(timeinfo->tm_min);
  set = h*60 + m;
  diff = set - cur;
  if (diff < 0){
    diff += 1440;
  }
  printf("cur : %d : %d", (timeinfo->tm_hour), (timeinfo->tm_min));
  printf("diff : %d\n", diff);
  return diff*60;
}
