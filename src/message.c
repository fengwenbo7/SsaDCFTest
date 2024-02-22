/*
*  DCFTest_socket的代码实现
*/

#include "message.h"

#define FAILED -1
#define PASSED 0

/* 客户端发送消息函数 */
int DCFTest_msg(char *ip, char *msg, char *rec)
{
	/*声明套接字和链接服务器地址*/
    int sockfd;
	pthread_t recv_tid , send_tid;
    struct sockaddr_in servaddr;

	/*(1) 创建套接字*/
    if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
    {
        // perror("socket error");
        // exit(1);
		printf("\033[31m[ FAILED ]\033[0m socket error.\n");
		return FAILED;
    }//if

    /*(2) 设置链接服务器地址结构*/
    bzero(&servaddr , sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if(inet_pton(AF_INET , ip , &servaddr.sin_addr) < 0)
    {
        // exit(1);
        printf("\033[31m[ FAILED ]\033[0m inet_pton error for %s\n", ip);
		return FAILED;
    }//if

    /*(3) 发送链接服务器请求*/
    if( connect(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
    {
        // perror("connect error");
        // exit(1);
		printf("\033[31m[ FAILED ]\033[0m connect error.\n");
		return FAILED;
    }//if

	/*客户端发送消息*/
	if(send(sockfd , msg , strlen(msg) , 0) == -1)
	{
		// perror("send error.\n");
		exit(1);
		printf("\033[31m[ FAILED ]\033[0m send error.\n");
		return FAILED;
	}//if

	/*处理接收客户端消息函数*/
	if (rec == NULL)
	{
		// perror("recv error.\n");
		exit(1);
		printf("\033[31m[ FAILED ]\033[0m rec was not allocated space.\n");
		return FAILED;
	}
	memset(rec , 0 , MAX_LINE);
	int n;
	if((n = recv(sockfd , rec , MAX_LINE , 0)) == -1)
	{
		// perror("recv error.\n");
		// exit(1);
		printf("\033[31m[ FAILED ]\033[0m recv error.\n");
		return FAILED;
	}//if
	
	rec[n] = '\0';
	return PASSED;
}

/* 处理接收客户端消息函数 */
void *recv_message(void *fd)
{
	int sockfd = *(int *)fd;
	
	char buf[MAX_LINE];
	memset(buf , 0 , MAX_LINE);
	int n;
	if((n = recv(sockfd , buf , MAX_LINE , 0)) == -1)
	{
		// perror("recv error.\n");
		printf("\033[31m[ FAILED ]\033[0m recv error.\n");
		exit(1);
	}//if
		
	buf[n] = '\0';		

	// printf("%s\n", buf);

	/*处理服务器发送消息*/
	unsigned long long readIndex = -1;
	sscanf(buf, "%ld", &readIndex);
	char msg[MAX_LINE] = "";
	DCFTest_read(1, readIndex, msg, 1024);
	
	if(send(sockfd , msg , strlen(msg) , 0) == -1)
	{
		// perror("send error.\n");
		printf("\033[31m[ FAILED ]\033[0m send error.\n");
		exit(1);
	}//if	

}

/* 后端处理接收客户端消息函数 */
int DCFTest_back(const char* ip_addr)
{
	//声明套接字
	int listenfd , connfd;
	socklen_t clilen;
	//声明线程ID
	pthread_t recv_tid , send_tid;

	//定义地址结构
	struct sockaddr_in servaddr , cliaddr;
	
	/*(1) 创建套接字*/
	if((listenfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
	{
		// perror("socket error.\n");
		// exit(1);
		printf("\033[31m[ FAILED ]\033[0m socket error.\n");
		return FAILED;
	}//if

	/*(2) 初始化地址结构*/
	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip_addr);
	servaddr.sin_port = htons(PORT);

	/*(3) 绑定套接字和端口*/
	if(bind(listenfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
	{
		// perror("bind error.\n");
		// exit(1);
		printf("\033[31m[ FAILED ]\033[0m bind error.\n");
		return FAILED;
	}//if

	/*(4) 监听*/
	if(listen(listenfd , LISTENQ) < 0)
	{
		// perror("listen error.\n");
		// exit(1);
		printf("\033[31m[ FAILED ]\033[0m listen error.\n");
		return FAILED;
	}//if

	printf("\033[32m[ PASSED ]\033[0m msg receiving module started.\n");

	int count = 0;
	/*(5) 接受客户请求，并创建线程处理*/
    while(1)
	{
		clilen = sizeof(cliaddr);
		if((connfd = accept(listenfd , (struct sockaddr *)&cliaddr , &clilen)) < 0)
		{
			// perror("accept error.\n");
			// exit(1);
			printf("\033[31m[ FAILED ]\033[0m accept error.\n");
			return FAILED;
		}//if
		
		// printf("\n");
		// printf("\033[32m[ ------ ]\033[0m got connection from %s\n", inet_ntoa(cliaddr.sin_addr));
		// printf("\033[32m[ ------ ]\033[0m %s: ", inet_ntoa(cliaddr.sin_addr));

		/*创建子线程处理该客户链接接收消息*/
		if(pthread_create(&recv_tid , NULL , recv_message, &connfd) == -1)
		{
			// perror("pthread create error.\n");
			// exit(1);
			printf("\033[31m[ FAILED ]\033[0m pthread create error.\n");
			return FAILED;
		}//if
	}
}