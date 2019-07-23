
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
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <sys/types.h>
#include <pthread.h>



void* to_serv(void* ptr)
{
	char buff[255];
	struct sockaddr_in serv, client;
	serv.sin_family=AF_INET;
	serv.sin_port=htons(1455);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	int fd, fd_client;

	fd= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	while(connect(fd, (struct sockaddr*)&serv, sizeof(serv))==-1);
	recv(fd, buff, 255, 0);
	printf("%s\n",buff);
	close(fd);
}



int main(void )
{
	int MAX=10;
	int tmp[MAX];
	pthread_t tid[MAX];
	for(int i=0; i<MAX; i++)
	{
		tmp[i]=i+1;
		pthread_create(&tid[i], NULL, to_serv, &tmp[i]);
	}

	for(int i=0; i<MAX; i++)
	{
		pthread_join(tid[i], NULL);
	}
	return 0;
}
