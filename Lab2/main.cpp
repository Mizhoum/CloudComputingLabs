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
#include "httpsever.h"
using namespace std;
#define PTHREAD_NUM 10

pthread_mutex_t mutex;   
pthread_cond_t cond;   //唤醒消息队列等待线程
queue<httpsever> queue1; //工作队列
int pthread_num; //线程数
pthread_t *pthread_pool;//线程池数组

void* run(void* arg)
{
	while(true){
        pthread_mutex_lock(&mutex);
	//cout<<"OK"<<endl;
        while(queue1.empty()){
            pthread_cond_wait(&cond,&mutex);
        }
        httpsever http=queue1.front();
        queue1.pop();
        pthread_mutex_unlock(&mutex);
        http.solve();
	//cout<<"end"<<endl;
    }
}

int main(int argc, char *argv[])
{
	pthread_mutex_init(&mutex,NULL);
    	cond = PTHREAD_COND_INITIALIZER;
	if(argc < 5)
	{
		printf("need ip and port\n");
		return 1;
	}
	//获取输入的ip、端口和线程数
	string ip = argv[2];
	//cout<<ip<<endl;
	int port = atoi(argv[4]);
	if(argc==7)
		pthread_num = atoi(argv[6]);
	else pthread_num = PTHREAD_NUM;
	
	//创建线程池
	pthread_pool = new pthread_t[pthread_num];
	if(!pthread_pool)
		throw std::exception();
	for(int i=0;i<pthread_num;i++)
	{
		if(pthread_create(pthread_pool+i, NULL, run, NULL) != 0);
		if(pthread_detach(pthread_pool[i]))//分离线程
		{
			delete [] pthread_pool;
			throw std::exception();
		}
	}
	//设置服务器的socket_in
	int sock;
	int connetionfd;
	struct sockaddr_in server_address;
	memset(&server_address,0,sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	

	sock = socket(AF_INET,SOCK_STREAM,0);
	assert(sock>=0);
	//绑定
	int ret = bind(sock, (struct sockaddr*)&server_address, sizeof(server_address));
	assert(ret!=-1);
	//监听
	ret = listen(sock, 10);
	assert(ret!=-1);
	

	while(1)
	{
		struct sockaddr_in client;
		socklen_t client_addrlen = sizeof(client);
		connetionfd = accept(sock, (struct sockaddr*)&client, &client_addrlen);
		
		assert(connetionfd>=0);
		//加入工作队列
		httpsever http(connetionfd);
		pthread_mutex_lock(&mutex);
    		queue1.push(http);
    		pthread_cond_signal(&cond);//唤醒休眠线程
   		pthread_mutex_unlock(&mutex);
	}
	close(sock);

	return 0;
}

