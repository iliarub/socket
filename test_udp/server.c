#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

int main()
{
	struct sockaddr_in serv, client;
	serv.sin_family=AF_INET;
	serv.sin_port=htons(1455);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	char buff[255]="messge from server";
	int fd, fd_client;
	char tmp[255];
	int size = sizeof(client);
	fd= socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	bind(fd, (struct sockaddr*)&serv, sizeof(serv));
	recvfrom(fd, tmp, 255, 0,(struct sockaddr*)&client , &size);
	sendto(fd, buff, 255, 0,(struct sockaddr*) &client, sizeof(client));
	recvfrom(fd, buff, 255, 0,(struct sockaddr*)&client , &size);
	printf("%s\n", buff);
	close(fd_client);
	close(fd);
	return 1;
}
