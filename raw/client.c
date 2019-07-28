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
	char buff2[400]={0};
	struct sockaddr_in serv, client;
	serv.sin_family=AF_INET;
	serv.sin_port=htons(1455);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	client.sin_family=AF_INET;
	client.sin_port=htons(123);
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

	int fd;
	if((fd= socket(AF_INET, SOCK_RAW, IPPROTO_UDP))==-1)
		perror("create socket");
	if(bind(fd, (struct sockaddr*)&client, sizeof(client))==-1)
		perror("bind");


	struct ip iph;
	struct udphdr *udph, udph2;
	udph->uh_sport=htons(123);		/* source port */
	udph->uh_dport=htons(1455);		/* destination port */
	udph->uh_ulen=htons(14);				/* udp length */
	udph->uh_sum=htons(0);

	memcpy(buff, udph, sizeof(struct udphdr));
	char *tmp="hello";
	memcpy(buff+sizeof(struct udphdr), tmp, sizeof(tmp));

	if(sendto(fd, buff, sizeof(buff), 0, (struct sockaddr*)&serv, sizeof(serv))==-1)
		perror("send");
	int i=0;
	int size = sizeof(client);
	while(i==0)
	{
		int len;
		if((len =recvfrom(fd, buff2, 400, 0, (struct sockaddr*)&serv, &size))==-1)
		perror("recv");

		memcpy(&iph, buff2, sizeof(struct ip));
		memcpy(&udph2, buff2+sizeof(struct ip), sizeof(struct udphdr));
		if(htons(123)==udph2.uh_dport) i++;
	}

	printf("%s\n", buff2+28);
	return 1;
}
