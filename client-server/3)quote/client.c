
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
	int* port;
	port=(int*)ptr;
	char buff[255];
	sprintf(buff, "%d", *port);
	struct sockaddr_in serv, client;
	serv.sin_family=AF_INET;
	serv.sin_port=htons(1455);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	int fd, fd_client;

	fd= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	while(connect(fd, (struct sockaddr*)&serv, sizeof(serv))==-1);
	send(fd, (char*)&buff, 255, 0);

	//close(fd);

	client.sin_family=AF_INET;
	client.sin_port=htons(*port);
	client.sin_addr.s_addr=inet_addr("127.0.0.1");
	bind(fd, (struct sockaddr*)&client, sizeof(client));
	listen(fd, 5);
	int size = sizeof(struct sockaddr_in);
	while((fd_client = accept(fd, (struct sockaddr*)&client, &size))==-1);
	recv(fd_client, buff, 255, 0);
	printf("%d) %s\n", *port ,buff);
	close(fd);
	close(fd_client);
}



int main(void )
{
	int MAX=1;
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
