#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

int set_timer(int h, int m);
void sigint_hanlder(int signo);
int alarm2(int h, int m);

