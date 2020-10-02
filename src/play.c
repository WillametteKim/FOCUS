#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	int pid;
	pid = fork();
	if(pid == 0){
		printf("I'm your child\n");
		execlp("/usr/bin/omxplayer"," ", "./example.mp3", NULL);
		_exit(0);
	}
	else{
		//system("omxplayer /home/pi/Downloads/example.mp3");
		printf("I'm your father\n");
	}
	return 0;
}
