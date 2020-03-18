#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include "fun.h"

/*
int main(int argc, const char *argv[])
{
	FILE *fp;
	infor_t *c;
	infor_t cli = {"20150001","lili","w","110","2015.03.24"};
	infor_t cli1 = {"20150002","lili12","w","110","2015.03.24"};
	infor_t cli2 = {"20150003","lili123","w","110","2015.03.24"};
	infor_t cli3 = {"20150004","lili1234","w","110","2015.03.24"};
	infor_t cli4 = {"20150005","huang yong sheng ","M","123456","2015.09.12"};

	fp = fopen("./1.txt","a+");

//	c = &cli;
//	server_insert(fp,c);

//	c = &cli1;
//	server_insert(fp,c);
//	c = &cli2;
//	server_insert(fp,c);

//	c = &cli4;
//	if(-1 == server_insert(fp,c))
//		puts("the id has had");
//	else
//		puts("insert success");

//	server_delete(fp,"20150004");
//	c = &cli4;
//	server_modify(fp,c);


	if(NULL != (c = server_search(fp,"20150005"))){
		printf("id: %s\tname: %s\tsex: %s\tnum: %s\tdate: %s\n",
				c->id,c->name,c->sex,c->num,c->date);
		free(c);
	}
	else
		puts("no found");		
	
	return 0;
}
*/
#define SRC "./record.txt"
int server_insert(infor_t *c){
	FILE *fp;
	fp = fopen(SRC,"a+b");
	infor_t tmp;
	fseek(fp,0,SEEK_SET);
	int offset;	
	while(1) {
		if(fread(&tmp,sizeof(tmp),1,fp)<=0)break;
		if(strcmp(tmp.id,c->id) == 0) {
			fclose(fp);
			return -1;
		}
	}
	fwrite(c,1,sizeof(infor_t),fp);
	fclose(fp);
	return 0;
}

int server_delete(char _id[]) {	
        FILE *fp;
        fp = fopen(SRC,"r+");
        infor_t tmp;
        fseek(fp,0,SEEK_SET);
	int i;
	int read_num;
	int offset;
	i=0;
        while(1) {
		if(read_num = fread(&tmp,sizeof(tmp),1,fp)<=0)break;
                if(strcmp(tmp.id,_id) == 0) {
			offset=sizeof(infor_t)*i;
			fseek(fp,offset,SEEK_SET);	
			strcpy(tmp.id,"aaaaaaaaa");
			fwrite(&tmp,1,sizeof(infor_t),fp);
			fclose(fp);
                        return 0;
                }
		i++;
        }
	fclose(fp);
        return -1;

}

int server_modify(infor_t *c) {
        FILE *fp;
        fp = fopen(SRC,"r+");
        infor_t tmp;
	int i;
        fseek(fp,0,SEEK_SET);
        int read_num;
        int offset;
	i=0;
        while(1) {
                if(read_num = fread(&tmp,sizeof(tmp),1,fp)<=0)break;
                if(strcmp(tmp.id,c->id) == 0) {
                        offset=sizeof(infor_t)*i;
                        fseek(fp,offset,SEEK_SET);
                        fwrite(c,1,sizeof(infor_t),fp);
                        fclose(fp);
                        return 0;
                }
		i++;
        }
        fclose(fp);
        return -1;
}

infor_t *server_search(char id[]) {
	FILE *fp;
        fp = fopen(SRC,"r");
        infor_t tmp;
	infor_t *ret;
	ret=malloc(sizeof(infor_t));
        fseek(fp,0,SEEK_SET);
        int read_num;
        while(1) {
                if(read_num = fread(&tmp,sizeof(infor_t),1,fp)<=0)break;
                if(strcmp(tmp.id,id) == 0) {
			strcpy(ret->id,tmp.id);
			strcpy(ret->name,tmp.name);
			strcpy(ret->sex,tmp. sex);
			strcpy(ret->num,tmp.num);
			strcpy(ret->date,tmp.date);
                        return ret;
                }
        }
        fclose(fp);
	return NULL;
}


