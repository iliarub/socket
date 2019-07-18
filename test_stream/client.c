#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
int main()
{
	char buff[255];
	struct sockaddr_in serv;
	serv.sin_family=AF_INET;
	serv.sin_port=htons(1455);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");

	int fd;
	fd= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(fd, (struct sockaddr*)&serv, sizeof(serv));
	recv(fd, buff, 255, 0);
	printf("%s\n", buff);
	scanf("%s", buff);
	send(fd, buff, 255, 0);
	return 1;
}
