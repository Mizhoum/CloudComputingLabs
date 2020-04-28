#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
using namespace std;

#define BUFFER_SIZE 10000

class  httpsever{
private:
    int sockfd; //套接字描述符
public:
    httpsever(){}
    httpsever(int id):sockfd(id){}
    ~httpsever(){}
    void solve();
    void Judge(string method,string url);
    void get(string method,string url);
    void post(string name,string id);
};

#endif