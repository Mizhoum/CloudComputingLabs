#include "httpsever.h"
#include <cstring>
#include <linux/tcp.h>

//判断请求类型
void httpsever::Bad_Request(string method,string url){
	string string1;
	if(method!="GET"&&method!="POST"){
		string string1="<htmL><title>501 Not Implemented</title>c<body bgcolor=ffffff>\n Not Implemented\n<p>Does not implement this method: "+method+"\n,<r><em>HTTP Web Server </em>\n</body></html>\n";
		string tmp="Http/1.1 501 Not Implemented\r\nContent-Type: text/html\r\nContent-Length:"+to_string(string1.length())+"\r\n\r\n";
		string finalentity=tmp+string1;
		char outstring[1024];
		sprintf(outstring,"%s",finalentity.c_str());//输出到命令行
		write(sockfd,outstring,strlen(outstring));//sockfd是描述符,类似于open函数
	}
	else{
		if(method=="GET"){
			 string1="<html><title>404 Not Found</title><body bgcolor=ffffff>\nNot Found\n<p>Could not find this file: "+url+"</p>\n<hr><em>HTTP Web Sever</em>\n</body></html>\n";
		}
		if(method=="POST"){
			 string1="<html><title>404 Not Found</title><body bgcolor=ffffff>\nNot Found\n<hr><em>HTTP Web Sever</em>\n</body></html>\n";
		}
		string tmp="Http/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length:"+to_string(string1.length())+"\r\n\r\n";
		string finalentity=tmp+string1;	
		char outstring[1024];
		sprintf(outstring,"%s",finalentity.c_str());//输出到命令行
		write(sockfd,outstring,strlen(outstring));//sockfd是描述符,类似于open函数
	}	
}

//根据请求类型具体处理
void httpsever::solve(){
    string zone_s;
    bool status;
    char zone[BUFFER_SIZE];
    while(1){
        status=true;//持久连接标志
        char zone[BUFFER_SIZE];
        int rc=recv(sockfd,zone,BUFFER_SIZE-1,0);
        if(rc>0){
            //zone[rc]='\0';
            zone_s=string(zone);
            //处理所有的http请求报文
            while(zone_s.find("HTTP/1.1")!=string::npos){
                //首先判断zone_s中的有没有完整报文
                int request_end_pos=0;//请求除主体外报文尾部
                //int entity_pos=0;//请求体的起始位置
                
                if((request_end_pos=zone_s.find("\r\n\r\n"))!=-1){
                    string request="";//http请求报文
                    //接下来判断是否有请求体，以及请求体是否完整
                    request_end_pos+=4;
                    request=zone_s.substr(0,request_end_pos);
                   
                    int head_end_pos=request.find("Content-Length");
		            int entity_pos=request.length();//实体主体起始位置
                    if(head_end_pos!=-1){//存在请求体
                        string num;
                        head_end_pos+=15;
                        while(request[head_end_pos]!='\r'){
                            num+=request[head_end_pos++];
                        }
                        int entity_l=atoi(num.c_str());//首部行长度
                        //int entity_pos=request.length();//实体主体起始位置
                        if((zone_s.length()-request.length())>=entity_l){//有实体主体
                            request+=zone_s.substr(request.length(),entity_l);
                            request_end_pos+=entity_l;          
                        }
                        else continue;
                    }
                    zone_s=zone_s.substr(request_end_pos);//根据rc得到的缓冲内不再需要它
                    //得到完整http请求报文

                    string method,url;
                    request_end_pos=0;
                   // int entity_pos=0;//实体主体起始位置

                    while(request[request_end_pos]!=' '){
                        method+=request[request_end_pos++];
                    }//提取方法

                    if(method!="GET"&&method!="POST"){
                        Bad_Request(method,url);
                        continue;
                    }//若既不是GET也不是POST，返回501

                    ++request_end_pos;
                    while(request[request_end_pos]!=' '){
                        url+=request[request_end_pos++];
                    }
                    ++request_end_pos;//提取URL

                    if(method=="GET"){
                        get_method(method,url);
                    }
                    else if(method=="POST"){   
                        if(url!="/Post_show"){
                            Bad_Request(method,url);
                            continue;
                        }
                        string entity=request.substr(entity_pos,request.length()-entity_pos);

                        //请求体按照Name=xxx&ID=xxx排列时才处理
                        int namepos=entity.find("Name="),idpos=entity.find("&ID=");
                        if(namepos==-1||idpos==-1||idpos<=namepos){//请求体中存在Name和ID并且按照Name、ID排列
                            Bad_Request(method,url);
                            continue;
                        }
                       /* if(entity.find("=",idpos+4)!=string::npos){
                            Bad_Request(method,url);
                            continue;
                        }*/                       
                        string name,id;                        
                        name=entity.substr(namepos+5,idpos-namepos-5);
                        id=entity.substr(idpos+4);
                        post(name,id);
                    }    
                }
            }
          
        }
        else{
            //检测到客户端tcp连接关闭
            if(rc<=0) break;    
        }
    }
   sleep(1);
   close(sockfd);
}

//GET处理
void httpsever::get_method(string method,string url){
	int len=url.length();
	string tmp="./src";
	if(url.find(".")==string::npos){
		if(url[len-1]=='/'||url.length()==0){
			tmp+=url+"index.html";
		}
		else tmp+=url+"/index.html";
	}
	else tmp+=url;
	//cout<<tmp<<endl;
	int fd=open(tmp.c_str(),O_RDONLY);//若所有欲核查的权限都通过了检查则返回0值，表示成功，只要有一个权限被禁止则返回-1。
	if(fd>=0){
	    struct stat stat_buf;
   	    fstat(fd,&stat_buf);//通过文件名filename获取文件信息，并保存在buf所指的结构体stat中
	    char outstring[1024];
	    sprintf(outstring,"Http/1.1 200 OK\r\nContent-Length:%d\r\nContent-Type: text/html\r\n\r\n",stat_buf.st_size);//输出到命令行
	    write(sockfd,outstring,strlen(outstring));//sockfd是描述符,类似于open函数
	    sendfile(sockfd,fd,0,stat_buf.st_size);	 		
	}
	else{
        Bad_Request(method,url);		
	}
}

//POST处理
void httpsever::post(string name,string id){
    string en="<html><title>POST Method</title><body bgcolor=ffffff>\n";
    string en1="Your name: "+name+"\nYour id: "+id+"\n"+"<hr><em>HTTP Web Server</em>\n</body></html>\n";
    en+=en1;
    string tmp="HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+to_string(en.length())+"\r\n\r\n";
    string finalentity=tmp+en;
    char outstring[1024];
    sprintf(outstring,"%s",finalentity.c_str());
    write(sockfd, outstring, strlen(outstring));
}