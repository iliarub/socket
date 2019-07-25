#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/ip.h>
#include <netinet/udp.h>
#include <malloc.h>
#include <string.h>
int main()
{
	char buff[50]={0};
	char buff2[50]={0};
	struct sockaddr_in serv;
	serv.sin_family=AF_INET;
	serv.sin_port=htons(1455);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");

	int fd;
	if((fd= socket(AF_INET, SOCK_RAW, IPPROTO_UDP))==-1)
		perror("create socket");
	if(bind(fd, (struct sockaddr*)&serv, sizeof(serv))==-1)
		perror("bind");


	struct ip *iph;
	struct udphdr *udph, *udph2;
	udph->uh_sport=htons(1455);		/* source port */
	udph->uh_dport=htons(123);		/* destination port */
	udph->uh_ulen=htons(14);				/* udp length */
	udph->uh_sum=htons(0);

	memcpy(buff, udph, sizeof(struct udphdr));
	char *tmp="hello";
	memcpy(buff+sizeof(struct udphdr), tmp, sizeof(tmp));

	if(sendto(fd, buff, sizeof(buff), 0, (struct sockaddr*)&serv, sizeof(serv))==-1)
		perror("send");
	int i=0;
	while(i==0)
	{
		if(recv(fd, buff2, 255, 0)==-1)
		perror("recv");
		memcpy(udph2, buff2+20, sizeof(struct udphdr));
		if(htons(123)==udph2->uh_dport) i++;
	}

	printf("%s\n", buff);
	scanf("%s", buff);
	send(fd, buff, 255, 0);
	return 1;
}
