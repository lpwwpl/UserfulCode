#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "comm.h"
int sockfd;
data_t data;
data_t ret;
char buff[255];
int send_information(void) {
	if (-1 == send(sockfd, &data, sizeof(data), 0))
		error_exit("send");

	if (-1 == recv(sockfd, &ret, sizeof(ret), 0))
		error_exit("recv");

	return 0;
}

int main() {
	sockfd = socket(AF_INET,SOCK_STREAM,0);  
	struct sockaddr_in serveraddr;  
	bzero((char *)&serveraddr,sizeof(serveraddr));  
	serveraddr.sin_family = AF_INET;  
	serveraddr.sin_port = htons(8888);  
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");  
	if(connect(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0){  
        	printf("connect error\n");  
        	exit(1);  
	} 
	while(1) {
		printmainmenu();
	}

	return EXIT_SUCCESS;
}
int query(char *id) {
        send_information();
	if(ret.flag == FLAG_ERR) {
		printf("%s\n",ret.ret);
		return 0;
	} else {
		show(ret.infor);
	}
	return 1;
}
void show(infor_t infor) {
	printf("empno: %s           name:  %s           sex:   %s           num:  %s           date:   %s\n",infor.id,infor.name,infor.sex,infor.num,infor.date);
	printf("\n");
}
void printmainmenu() {
	printf("*****************\n");	
	printf("1、增加员工信息\n");
 	printf("2、删除员工信息\n");
    	printf("3、查找员工信息\n");
    	printf("4、修改员工信息\n");
    	printf("5、退出系统\n");
	char opt;
	opt = getchar();
	getchar();
        switch(opt) {
                case '1':
                        //char temp[255];
                        printf("********************\n");
                        printf("请输入员工号(10位)：");
                        if(NULL == gets(data.infor.id))
				perror("id");
                        printf("请输入员工姓名(20位)：");
                        if(NULL == gets(data.infor.name))
				perror("name");
                        printf("请输入员工性别(4位)：");
                        if(NULL == gets(data.infor.sex))
				perror("sex");
                        printf("请输入员工手机号(11位)：");
                        if(NULL == gets(data.infor.num))
				perror("num");;
                        printf("请输入员工入职时间(12位)：");
                        if(NULL == gets(data.infor.date))
				perror("date");
                        data.flag = FLAG_ADD;
			send_information();
			printf("%s\n",ret.ret);
                        break;
                case '2':
			printf("***********************\n");
			printf("请输入要删除的员工号：");
                        if(NULL == gets(data.infor.id))
				perror("id");
			data.flag = FLAG_DEL;
			send_information();
			printf("%s\n",ret.ret);
                        break;
                case '3':
			printf("***********************\n");
			printf("请输入要查询的员工号:");
                        if(NULL == gets(data.infor.id))
				perror("id");
			data.flag = FLAG_SEARCH;
			query(data.infor.id);
                        break;
                case '4':
			printf("***********************\n");
			printf("请输入要修改的员工号:");
			gets(data.infor.id);
			data.flag = FLAG_SEARCH;
			if(1 == query(data.infor.id)) {
				printf("Please input name:");
				if(NULL == gets(data.infor.name))
					error_exit("name");
				printf("Please input sex:");
				if(NULL == gets(data.infor.sex))
					error_exit("sex");
				printf("Please input telephone num:");
				if(NULL == gets(data.infor.num))
					error_exit("tel");
				printf("Please input time of in company:");
				if(NULL == gets(data.infor.date))
					error_exit("date");
				data.flag=FLAG_MODIFY;
				send_information();
			}
			printf("%s\n",ret.ret);
                        break;
                case '5':
			data.flag=FLAG_LOGOUT;
			send_information();
			if(ret.flag=FLAG_ACK) {
				printf("%s\n",ret.ret);
				close(sockfd);
				exit(EXIT_SUCCESS);
			} else {
				
			}
                        break;
                default:
                        break;
        }
}

