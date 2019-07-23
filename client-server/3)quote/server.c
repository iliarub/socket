
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
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>



struct msgbuf
{
	long p;
	char mmsg[255];
};


void *pthread_server(void* ptr)
{
	long int ttime;
	struct msgbuf buf;
	char buff[255];
	int fd, ds;
	key_t key;
	key=ftok(".", 'a');//очередь на обслуживание
	if((ds=msgget(key, 0))==-1)
	{
		printf("error quote");

	}

	while(1)
	{
		msgrcv(ds, &buf, sizeof(buf.mmsg), 0, 0);
		int port = atoi(buf.mmsg);
		ttime=time(NULL);
		strcpy(buff, ctime(&ttime));

		struct sockaddr_in client;
			client.sin_family=AF_INET;
			client.sin_port=htons(port);
			client.sin_addr.s_addr=inet_addr("127.0.0.1");

		fd= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		while(connect(fd, (struct sockaddr*)&client, sizeof(struct sockaddr_in))==-1);
		send(fd, buff, 255, 0);
	}
}
void ex(int sig)
{
	//msgctl(ds, IPC_RMID, 0);
	exit(1);
}

int main(void )
{
	int ds;
	char buff[255];
	signal(SIGINT, ex);
	key_t key;
	pthread_t tid[3];
	struct msgbuf buf;
	key=ftok(".", 'a');//очередь на обслуживание
	if((ds=msgget(key, IPC_CREAT|0666))==-1)
	{
		printf("error quote");
		return 1;
	}

	for(int i=0; i<1; i++)
		pthread_create(&tid[i], NULL, pthread_server, 0);


	struct sockaddr_in serv;
	serv.sin_family=AF_INET;
	serv.sin_port=htons(1455);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");

	int fd;
	fd= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(fd, (struct sockaddr*)&serv, sizeof(serv));
	listen(fd, 5);


	while(1)
	{
		int size=sizeof(struct sockaddr_in);
		accept(fd, (struct sockaddr*)&serv, &size);

		while (recv(fd, (char*)&buff, 255, 0)==-1);
		buf.p=1;
		strcpy(buf.mmsg, buff);
		msgsnd(ds, &buf, sizeof(buf.mmsg), 0);
	}
	return 0;
}
