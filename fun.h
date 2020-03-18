#ifndef FUN_H
#define FUN_H
#include "comm.h"
extern int server_modify(infor_t *c);
extern infor_t *server_search(char id[]);
extern int server_delete(char _id[]); 
extern int server_insert(infor_t *c);
#endif
