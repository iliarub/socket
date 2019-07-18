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
	int size = sizeof(client);
	fd= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	listen(fd, 5);
	bind(fd, (struct sockaddr*)&serv, sizeof(serv));
	fd_client=accept(fd, (struct sockaddr*)&client, &size);
	send(fd_client, buff, 255, 0);
	recv(fd_client, buff, 255, 0);
	printf("%s\n", buff);
	close(fd_client);
	close(fd);
	return 1;
}
