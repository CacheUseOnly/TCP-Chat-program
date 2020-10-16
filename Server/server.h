#ifndef _WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#else
#include <WinSock2.h>
#endif // !_WIN32
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define DEFAULT_PORT 8080
#define DEFAULT_BUFFERSIZE 256
#define DEFAULT_BACKLOG 5

int bindPort(int port);
int initBuffer(int buffSize);
int setBacklog(int num);
int openSock();
int binding();
int accept();
int listen();
void printHelp();