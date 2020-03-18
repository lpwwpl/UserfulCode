#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "comm.h"
#include "employee.h"
#include <pthread.h>
#include "thread_pool.h"
#include <sys/epoll.h>

#include "fun.h"
#define PORT 8888
#define MAX_EVENT_NUMBER 1024
#define TCP_BUFFER_SIZE 512
#define UDP_BUFFER_SIZE 1024
#define THREAD_NUMBER 10
int setnonblocking( int fd )
{//设置非阻塞
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}
void addfd( int epollfd, int fd )
{//注册套接字句柄
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN|EPOLLET;
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
}

void *handle_tcp_mes(void*arg);
int main() {  
  
	int sockfd = socket(AF_INET, SOCK_STREAM,0);  
	if(sockfd < 0){  
        	perror("socket");  
        	exit(1); 
	} 
	struct sockaddr_in serveraddr;  
	bzero((char *)&serveraddr,sizeof(serveraddr));  
	serveraddr.sin_family = AF_INET;  
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);  
	serveraddr.sin_port = htons(PORT);  
	if(bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0){  
        	perror("connect");  
        	exit(1);  
	}  
  
	if(listen(sockfd,1024) < 0){  
        	perror("listen error");  
        	exit(1);  
	}  	

	thread_pool_t pool;
	pool = thread_pool_create(THREAD_NUMBER);
	struct epoll_event events[MAX_EVENT_NUMBER];
	int epollfd = epoll_create(5);
	addfd(epollfd,sockfd);

	while(1){  
		int number = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
		if(number<0)
		{
			printf("epoll failure\n");
			break;
		}
		int i;
		long arg;
		for(i=0;i<number;i++)
		{
			int sfd = events[i].data.fd;
			if(sfd == sockfd)
			{
				struct sockaddr_in client_addr;
				socklen_t client_addrLen = sizeof(client_addr);
				int connfd = accept(sockfd,(struct sockaddr*)&client_addr,&client_addrLen);
				addfd(epollfd,connfd);
				printf("new conn\n");
			}
			else if(events[i].events & EPOLLIN)
			{
				arg = sfd;
				thread_pool_add_task(pool,handle_tcp_mes,(void*)arg);
				printf("handle\n");
			}
			else
			{
				printf("something else happened\n");
			}
		}
		
	}  
	thread_pool_destroy(pool);
	close(sockfd);
	EXIT_SUCCESS;  
}  

void *handle_tcp_mes(void* arg) {
	int connfd =(long)arg;
	data_t data;
	data_t reply;
	infor_t *infor;
	infor = malloc(sizeof(infor_t));
	reply.flag=FLAG_ERR;
	while(1) {
		recv(connfd,&data, sizeof(data), 0);
		
		switch(data.flag) {
			case FLAG_ADD:
				if(server_insert(&data.infor)==0) {
					reply.flag = FLAG_ACK;
					strcpy(reply.ret,"add success");
				} else {
					strcpy(reply.ret,"add fail");
				}
				send(connfd,&reply,255,0);
				break;
			case FLAG_DEL:
				if(server_delete(data.infor.id)==0) {
					reply.flag = FLAG_ACK;
					strcpy(reply.ret,"del success");
				} else {
					strcpy(reply.ret,"del fail");
				}
				send(connfd,&reply,255,0);
				break;
			case FLAG_SEARCH:
				infor = server_search(&(data.infor));
				printf("%s",infor->id);
				fflush(stdout);
				if(NULL == infor) {
					reply.flag = FLAG_ERR;
					strcpy(reply.ret,"search fail,no such user");
				} else {
					strcpy(reply.infor.id ,infor->id);	
					strcpy(reply.infor.name, infor->name);
					strcpy(reply.infor.sex, infor->sex);
					strcpy(reply.infor.num, infor->num);
					strcpy(reply.infor.date,infor->date);
					reply.flag = FLAG_ACK;
				}
				send(connfd,&reply,255,0);
				break;
			case FLAG_MODIFY:
				if(server_modify(&(data.infor))==0) {
					reply.flag = FLAG_ACK;
					strcpy(reply.ret,"modify success");
				} else {
					strcpy(reply.ret,"modify fail");
				}
				send(connfd,&reply,255,0);
				break;
			case FLAG_LOGOUT:
				if(data.flag==FLAG_LOGOUT) {
					reply.flag = FLAG_ACK;
					strcpy(reply.ret,"logout success");
				} else {
					reply.flag = FLAG_ERR;
					strcpy(reply.ret,"logout fail");
				}
				send(connfd,&reply,255,0);
				break;
			default:
				break;
		}
	}
	return NULL;    
}
