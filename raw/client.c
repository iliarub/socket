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
#define DATA 200
struct ip *CreateIPHeader(char *srcip, char *destip)
{
    struct ip *ip_header;

    ip_header = malloc(sizeof(struct ip));

    ip_header->ip_v = 4;
    ip_header->ip_hl = 5;
    ip_header->ip_tos = 0;
    ip_header->ip_len = htons(sizeof(struct ip) + sizeof(struct udphdr) + 6);
    ip_header->ip_id = htons(111);
    ip_header->ip_off = 0;
    ip_header->ip_ttl = 111;
    ip_header->ip_p = IPPROTO_UDP;
    ip_header->ip_sum = 0;
    inet_pton(AF_INET, srcip, &ip_header->ip_src);
    inet_pton(AF_INET, destip, &ip_header->ip_dst);


    return (ip_header);
}

int main()
{
	char buff[DATA]={0};
	char buff2[DATA]={0};
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
	int val=1;
	setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val));

	struct ip *iph, iph2;
	struct udphdr *udph, udph2;
	udph->uh_sport=htons(123);		/* source port */
	udph->uh_dport=htons(1455);		/* destination port */
	udph->uh_ulen=htons(14);				/* udp length */
	udph->uh_sum=htons(0);

	iph=CreateIPHeader("127.0.0.1", "127.0.0.1");

	memcpy(buff, iph, sizeof(struct ip));
	memcpy(buff+sizeof(struct ip), udph, sizeof(struct udphdr));
	char *tmp="hello";
	memcpy(buff+sizeof(struct ip) + sizeof(struct udphdr), tmp, sizeof(tmp));

	if(sendto(fd, buff, sizeof(buff), 0, (struct sockaddr*)&serv, sizeof(serv))==-1)
		perror("send");
	int i=0;
	int size = sizeof(client);
	while(i==0)
	{
		int len;
		if((len =recvfrom(fd, buff2, 500, 0, (struct sockaddr*)&serv, &size))==-1)
		perror("recv");

		memcpy(&iph2, buff2, sizeof(struct ip));
		memcpy(&udph2, buff2+sizeof(struct ip), sizeof(struct udphdr));
		if(htons(123)==udph2.uh_dport)
		{
			char srcip[32];
			inet_ntop(AF_INET, &iph2.ip_dst, srcip, 16);
			if(strcmp(srcip, "127.0.0.1")==0) i++;
		}
	}
	printf("%s\n", buff2+28);

	return 1;
}
