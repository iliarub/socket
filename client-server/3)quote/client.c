
/*
 ============================================================================
 Name        : test.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>

pthread_mutex_t ml[3];
int ds1;

struct msgbuf
{
	long p;
	char mmsg[255];
};





int main(void )
{
	key_t key1;
	struct msgbuf buf, buf2;

	key1=ftok("server", 'a');//очередь на сервер
	if((ds1=msgget(key1, 0))==-1)
	{
		printf("error open");
		return 1;
	}
	int pid=getpid();
	sprintf(buf.mmsg, "%d", pid);
	buf.p=1;
	msgsnd(ds1,(void*) &buf, sizeof(buf.mmsg), 0);
	msgrcv(ds1, (void*) &buf2, sizeof(buf2.mmsg), pid, 0);
	printf("%s", buf2.mmsg);
	return 0;
}
