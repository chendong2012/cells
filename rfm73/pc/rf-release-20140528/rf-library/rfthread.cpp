#include<sys/socket.h>
#include <unistd.h>	//for close function
#include <string.h> 	//for bzero function
#include<stdio.h>
#include<sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<netinet/in.h>
#include <stdlib.h>

#include <pthread.h>
#include "user_activity.h"
#include "rfthread.h"
#define SERV_PORT 5555
#define SERV_IP "xxx.xxx.xxx.xxx"//mx27 board ip
#define BACKLOG 10 	//the counts of connect can keep in wait queen
#define MAXBUFSIZE 200
static char buf[MAXBUFSIZE]; 	//receive buf
/*此线程的功能，是一个服务器
平时处理等待状态，一旦有客户端
连接，把客户的数据读出来,然后根据数据, 分析,发送到指定的对像去,然后,
等待接收回来的信息,最后,把数据发,回给客户端,然后,又开始睡觉,就这样不
断循环*/

/*服务器的端口为:5555*/

/*如何使用此类：
1、给出使用这个线程的列表：
2、有多个用户要用这个线程
*/
rfthread::rfthread(user_activity *users[], int count)
{
	int i = 0;
	printf("hello i am rfthread\n");
	m_usercount = count; 
	for (i = 0; i < count; i++) {
		m_users[i] = users[i];
	}
}

static void *thread_main(void *ptr)
{
	unsigned char mytestbuf[32];
	unsigned char len1;
	unsigned char saddr,sport,raddr,rport, id;	
	rfthread *p = (rfthread *)ptr;
	int i = 0;
	
	memset(mytestbuf, 0, 32);

	int sockfd,sockfd_client;
	socklen_t sin_size; // used in accept(),but i don't know what it means

	printf("#####################################################\n");
	printf("socket receive text        by pafone  30th,April,2009\n");
	printf("server ip:%s port:%d         \n",SERV_IP,SERV_PORT);
	printf("#####################################################\n");

	struct sockaddr_in serv_addr,client_sockaddr; //server ip info
	int serverport;
	serverport = SERV_PORT;

	if(-1 == (sockfd = socket(AF_INET,SOCK_STREAM,0)) ) {
		perror("error in create socket\n");
		exit(1);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(serverport);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&serv_addr.sin_zero,8);

	//bind , the ip and port information is aready in the sockaddr
	if(-1 == bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))) {
		perror("bind error\n");
		exit(2);
	}
	printf("bind seccessful\n");

	if(-1 == listen(sockfd,BACKLOG)) {
		perror("lisenning\n");
		exit(3);
	}
	printf("the server is listenning...\n");

/*每来一个客户就转一次*/
	while(1) {
		//accept
		printf("before accept:sockfd_client is %d\n",sockfd_client);
		if(-1 == (sockfd_client = accept(sockfd,(struct sockaddr*)&client_sockaddr,&sin_size))) {
			perror("accept\n");
			exit(4);
		}

		printf("accept connect.\n");
		int recvbytes;
		if(-1 == (recvbytes = recv(sockfd_client,buf,MAXBUFSIZE,0))) {
			if(-1 == recvbytes) {
				perror("receive\n");
				exit(5);
			}
		}
		printf("%dbytes receive from connect:%s\n",recvbytes, buf);

		/*这里要定制化
		  首先是把数据，发给对像，然后等返回的结果。

		*/

		/*??????????????????*/
		/*通信协议的问题：
		*/
		int ret;
		for (i = 0; i < p->m_usercount; i++) {
			ret = p->m_users[i]->send_net_package((unsigned char *)buf, (unsigned char *)&recvbytes);
			if (ret == 1) {
				break;
			}
		}
		if (ret == 1) {
			/*把结果返回给客户端*/
			if(send(sockfd_client, buf, recvbytes, 0)<0) {
				printf("Send File: Failed\n");
				exit(6);
			}
		} else {
			if(send(sockfd_client, "no ack", 6, 0)<0) {
				printf("Send File: Failed\n");
				exit(7);
			}
		}
		/*把这次连接关掉*/
		close(sockfd_client);
		/*开始下一次通信*/
	}
	close(sockfd);
}

int rfthread::init_ok()
{
        int ret;
        pthread_t id;

        ret = pthread_create(&id, NULL, thread_main, this);
        if(ret) {
                printf("create pthread_main error\n");
		return 1;
        }
	return 0;
}

