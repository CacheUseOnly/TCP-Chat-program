/*
 * A simple chat program server using system dependent socket
 * If compling on Windows then use Winsock 
 */

#include "server.h"

int main(int argc, char* argv[]) {
	if (argc > 1 && (strcmp(argv[1],"-h") == 0)) {
		printHelp();
		return 0;
	}

	int port, buffersize, sockfd/*socket file descriptor*/, newsockfd, backlog;
	struct sockaddr_in serv_addr, cli_addr;

	printf("Starting server program...\n");

	//Set port, buffer and backlog
	if (argc == 1) {
		printf("Binding port with default setting: 8080...");
		port = DEFAULT_PORT;
	}
	else if (strcmp(argv[1], "0") == 0) {
		printf("Binding port with default setting: 8080...");
		port = DEFAULT_PORT;
	} 
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

	if (argc < 3 | atoi(argv[2]) == 0) {
		printf("Initializing buffer with default setting: 256...");
		buffersize = DEFAULT_BUFFERSIZE;
	}
	else {
		printf("Initializing buffer: %d...", atoi(argv[2]));
		buffersize = atoi(argv[2]);
	}
	char** buffer = malloc(buffersize * sizeof(char*));
	printf(" Done\n");

	if (argc < 4) {
		printf("Setting backlog numbers with default setting: %d...");
		backlog = DEFAULT_BACKLOG;
	}
	else {
		if (atoi(argv[3]) == 0) {
			perror("Backlog must be greater than 0.");
			return 0;
		}
		printf("Setting backlog numbers: 5...");
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