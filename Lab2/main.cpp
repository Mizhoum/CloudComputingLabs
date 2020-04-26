#include <iostream>
#include <cstring>
#include <stdio.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
#include <exception>
#include <queue>
#include <getopt.h>
#include "httpsever.h"
#define MAX_CLIENT_COUNT 20
using namespace std;
#define PTHREAD_NUM 20
void* start(void* arg);
pthread_mutex_t mutex;   
pthread_cond_t cond;   //唤醒消息队列等待线程
queue<httpsever> queue1; //工作队列
int pthread_num; //线程数
pthread_t *pthread_pool;//线程池数组

int main(int argc, char *argv[])
{
	pthread_mutex_init(&mutex,NULL);
    	cond = PTHREAD_COND_INITIALIZER;
	static struct option long_options[] =
    {  
        {"ip", required_argument, NULL, '1'},
        {"port", required_argument, NULL, '1'},
        {"number-thread", required_argument, NULL,'1'},
        {NULL,  0,   NULL, 0},
    }; 	
	if(argc < 5)
	{
		printf(" ip and port are empty\n");
		return 1;
	}//参数错误 
	//获取输入的ip、端口和线程数
	string ip = argv[2];
	//cout<<ip<<endl;
	int portNum = atoi(argv[4]);//获取端口
	int port=portNum;

	//限定端口号为1024~65535
    	if((portNum<1024 || portNum>65535)){
        printf("Error: portNum range is 1024~65535 \n");
        return 1;
    }
	if(argc==7)
		pthread_num = atoi(argv[6]);
	else pthread_num = PTHREAD_NUM;
	
	//创建线程池
	pthread_pool = new pthread_t[pthread_num];
if(!pthread_pool)
		throw std::exception();
	for(int i=0;i<pthread_num;i++)
	{
		if(pthread_create(pthread_pool+i, NULL, start, NULL) != 0);
		if(pthread_detach(pthread_pool[i]))//分离线程
		{
			delete [] pthread_pool;
			throw std::exception();
		}
	}
	//设置服务器的socket_in
	int sock;
	int connetionfd;//欢迎套接字和连接套接字
	struct sockaddr_in server_address;
	memset(&server_address,0,sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//socket,TCP,IPv4网络
	sock = socket(AF_INET,SOCK_STREAM,0);
	assert(sock>=0);
	//绑定
	int ret = bind(sock, (struct sockaddr*)&server_address, sizeof(server_address));
	if(ret < 0){
		printf("bind error\n");
		return 1;
    }
	//监听
	ret = listen(sock, 20);
	if(ret < 0){
		printf("bind error\n");
		return 1;
    }
	

	while(1)
	{
		struct sockaddr_in client;
		socklen_t client_addrlen = sizeof(client);
		connetionfd = accept(sock, (struct sockaddr*)&client, &client_addrlen);
		
		assert(connetionfd>=0);
		//加入工作队列
		httpsever http(connetionfd);
		pthread_mutex_lock(&mutex);
    		queue1.push(http);//添加任务 
    		pthread_cond_signal(&cond);//唤醒休眠线程
   		pthread_mutex_unlock(&mutex);
	}
	close(sock);

	return 0;
}
void* start(void* arg)
{
	while(1){
        pthread_mutex_lock(&mutex);
        while(queue1.empty()){
            pthread_cond_wait(&cond,&mutex);
        }
        httpsever http=queue1.front();
        queue1.pop();
        pthread_mutex_unlock(&mutex);
        http.solve();
    }
}

void reuseAddr(int socketFd){
    int on = 1;
    int ret = setsockopt(socketFd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    if(ret==-1){
        printf( "Error : fail to setsockopt\n");
        exit(1);
    }
}

//tcp服务
int startTcp(char *ipAddr, int portNum){
    int httpdSocket = socket(AF_INET,SOCK_STREAM,0);
    if(httpdSocket==-1){
        printf("Error: can't create socket,errno is %d\n",errno);
        return 1;
    }

    //绑定端口 bind
    struct sockaddr_in tcpServerSockAddr;       //该结构体用来处理网络通信的地址
    memset(&tcpServerSockAddr, 0, sizeof(tcpServerSockAddr));//初始化
    tcpServerSockAddr.sin_family = AF_INET;
    tcpServerSockAddr.sin_port = htons(portNum);        //设置端口  
    tcpServerSockAddr.sin_addr.s_addr = inet_addr(ipAddr);
    reuseAddr(httpdSocket);
    if(bind(httpdSocket,(const struct sockaddr*)&tcpServerSockAddr,     //强制类型转换 sockaddr_in 转换为 sockaddr
        sizeof(tcpServerSockAddr))<0){
        fprintf(stderr, "Error: can't bind port %d,errno is %d\n",portNum,errno);
        return 1;
    }
    if(listen(httpdSocket,MAX_CLIENT_COUNT)<0){           //最大客户数量
        printf("Error: can't listen port %d,error is %d\n",portNum,errno);
        return 1;
    }
    return httpdSocket;
}

