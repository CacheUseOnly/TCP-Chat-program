#include "server.h"

void printHelp()
{
	printf("Usage: server [Port number] [Buffer size] [Backlog number]\n");
	printf("\t-Port: input 0 to skip this parameter, 20800 by default.\n");
	printf("\t-Buffer size: input 0 to skip this parameter, 256 by default.\n");
	printf("\t-Backlog number: 5 by default.\n");
}