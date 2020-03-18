#ifndef __COMM_H__
#define __COMM_H__
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "employee.h"
#define error_exit(_errmsg_)	error(EXIT_FAILURE, errno, _errmsg_)
#define DATA_LEN	1024
#define FLAG_LOGIN	'U'
#define FLAG_LOGOUT	'Q'
#define FLAG_ADD	'A'
#define FLAG_DEL	'D'
#define FLAG_SEARCH	'S'
#define FLAG_MODIFY	'M'
#define FLAG_ACK	'R'
#define FLAG_ERR	'E'

struct _data_ {
	int flag;
	union {
	infor_t infor;
	char ret[255];
	};
};
typedef struct _data_ data_t;
#endif

