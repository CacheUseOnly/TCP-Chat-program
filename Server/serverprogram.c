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

int main(int argc, char* argv[]) {
	/*if (argc > 1 && (strcmp(argv[1],"-help") == 0)) {
		printf("Usage: server [Port number] [Buffer size] [Backlog number]\n");
		printf("\t-Port: input 0 to skip this parameter, 20800 by default.\n");
		printf("\t-Buffer size: input 0 to skip this parameter, 256 by default.\n");
		printf("\t-Backlog number: 5 by default.\n");
		return 0;
	}*/

	int port, buffersize, sockfd/*socket file descriptor*/, newsockfd, backlog;
	struct sockaddr_in serv_addr, cli_addr;

	printf("Starting server program...\n");

	//Set port, buffer and backlog
	printf("Binding port: %d...", atoi(argv[1]));
	if (argc == 2) port = DEFAULT_PORT;
	else if (strcmp(argv[1], "0") == 0) port = DEFAULT_PORT;
	else {
		if (atoi(argv[1]) < 257) {
			perror("Cannot use system reserved ports.");
			return 0;
		}
		else {
			port = atoi(argv[1]);
		}
	}
	printf(" Done\n");

	printf("Initializing buffer: %d...", atoi(argv[2]));
	if (argc < 3) buffersize = DEFAULT_BUFFERSIZE;
	else if (atoi(argv[2]) == 0) buffersize = DEFAULT_BUFFERSIZE;
	else buffersize = atoi(argv[2]);
	char** buffer = malloc(buffersize * sizeof(char*));
	printf(" Done\n");

	printf("Setting backlog numbers: %d...", atoi(argv[3]));
	if (argc < 4) backlog = DEFAULT_BACKLOG;
	else {
		if (atoi(argv[3]) == 0) {
			perror("Backlog must be greater than 0.");
			return 0;
		}
		backlog = atoi(argv[2]);
	}
	printf(" Done\n");

	printf("Opening Sockets...");
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("Error opening socket.");
		return 0;
	}
	printf(" Done\n");

#ifndef _WIN32
	bzero((char*)& serv_addr, sizeof(serv_addr));	//Initialize and fill serv_addr with 0
#else
	memset(&serv_addr, 0, sizeof(serv_addr));
#endif

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	printf("Binding...");
	if (bind(sockfd, (struct sockaddr*) & serv_addr, \
		sizeof(serv_addr)) < 0)	perror("ERROR on binding");
	listen(sockfd, backlog);
	printf(" Done\n");

	printf("Accepting...\n");
#ifndef _WIN32
	socklen_t len = sizeof(cli_addr);
#else
	int len = sizeof(cli_addr);
#endif // !_WIN32
	newsockfd = accept(sockfd, (struct sockaddr*) & cli_addr, &len);

	if (newsockfd == -1) perror("ERROR on accept");
#ifndef _WIN32
	bzero(buffer, buffersize);
#else
	memset(&buffer, 0, buffersize);
#endif

	printf("Listening...\n");
	while (1) {
		if (read(newsockfd, buffer, buffersize) < 0) {
			perror("Error reading from socket.");
		}
		printf("%s", *buffer);
	}

#ifndef _WIN32
	close(sockfd);
#else
	closesocket(sockfd);
#endif // !_WIN32
}
