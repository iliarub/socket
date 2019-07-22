
/*
 ============================================================================
 Name        : test.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <signal.h>
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
int ds1, ds2;

struct msgbuf
{
	long p;
	char mmsg[255];
};


void *pthread_server(void* ptr)
{
	long int ttime;
	struct msgbuf buf, tmp;

	while(1)
	{
		msgrcv(ds2, &buf, sizeof(buf.mmsg), 0, 0);
		buf.p = atol(buf.mmsg);
		ttime=time(NULL);
		strcpy(buf.mmsg, ctime(&ttime));
		msgsnd(ds1, &buf, sizeof(buf.mmsg), 0);
	}
}
void ex(int sig)
{
	msgctl(ds1, IPC_RMID, 0);
	msgctl(ds2, IPC_RMID, 0);
	exit(1);
}

int main(void )
{
	signal(SIGINT, ex);
	key_t key1, key2;
	pthread_t tid[3];
	struct msgbuf buf;
	key1=ftok("server", 'a');//очередь на сервер
	key2=ftok("server", 'b');//очередь на обслуживание
	if((ds1=msgget(key1, IPC_CREAT|0666))==-1)
	{
		printf("error ds1");
		return 1;
	}
	if((ds2=msgget(key2, IPC_CREAT|0666))==-1)
	{
		printf("error ds2");
		return 1;
	}
	for(int i=0; i<3; i++)
		pthread_create(&tid[i], NULL, pthread_server, 0);
	while(1)
	{
		msgrcv(ds1, (void*) &buf, sizeof( buf.mmsg), 1, 0);
		msgsnd(ds2,(void*) &buf, sizeof(buf.mmsg), 0);
	}
	return 0;
}
