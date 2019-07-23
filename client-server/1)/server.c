
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
	char buff[255];
	int *fd;
	fd=(int*)ptr;

	ttime=time(NULL);
	strcpy(buff, ctime(&ttime));
	send(*fd, buff, 255, 0);
	close(*fd);

}
void ex(int sig)
{
	exit(1);
}

int main(void )
{

	pthread_t tid[100]={0};




	struct sockaddr_in serv;
	serv.sin_family=AF_INET;
	serv.sin_port=htons(1455);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");

	int fd, fd_client[100]={0};
	fd= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(fd, (struct sockaddr*)&serv, sizeof(serv));
	listen(fd, 5);


	while(1)
	{
		int size=sizeof(serv);
		for(int i=0;i<100; i++)
		{
			pthread_join(tid[i], NULL);
			while((fd_client[i] =accept(fd, (struct sockaddr*)&serv, &size))==-1);
			pthread_create(&tid[i], NULL, pthread_server, &fd_client[i]);
			if(i==99) i=0;
		}

	}
	return 0;
}
