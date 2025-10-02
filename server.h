#ifndef __SERVER_H__
#define __SERVER_H__

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <sys/select.h>
#define BUFFSIZE 2048
#define DEFAULT_PORT 16555 // 指定端口
#define MAXLINK 2048

// 处理http请求，使得send的内容符合http格式
void setRespond(char *buff);

#endif