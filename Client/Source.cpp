#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "8080"
#define DEFAULT_HOST "cacheuseonly.ddns.net"
#define DEFAULT_BUFFLEN 512

int initSocket(int hb, int lb, WSADATA &wsaData) {
	int iResult;

	std::cout << "Initializing Winsock... ";
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed: " << iResult;
		return 1;
	}
	std::cout << "Done\n";

	return 0;
}

int main() {
	int iResult, bufferlen, recvbuflen;
	std::string input;
	char recvbuf[DEFAULT_BUFFLEN];
	const char* hostaddr;
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL, * ptr = NULL, hints;


	std::cout << "Please input the address of the host: ";
	std::cin >> input;
	if (input == " \n") {
		hostaddr = DEFAULT_HOST;
	}
	else {
		hostaddr = input.c_str();
	}

	initSocket(2, 2, wsaData);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	std::cout << "Resolving the server address and port...\n";
	iResult = getaddrinfo(hostaddr, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	struct sockaddr_in* ip = (struct sockaddr_in*) result->ai_addr;
	printf("\tIPv4 address: %s\n", inet_ntoa(ip->sin_addr));
	std::cout << "\tPort: " << DEFAULT_PORT << std::endl;
	std::cout << "...Done\n";

	std::cout << "Connecting server...\n";
	ptr = result;

	std::cout << "\tCreating socket... ";
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "Done\n";
	}

	std::cout << "\tConnecting " << hostaddr << "... ";
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("\nUnable to connect to server!\n");
		std::cout << "Error: " << WSAGetLastError();
		freeaddrinfo(result);
		WSACleanup();
		closesocket(ConnectSocket);
		return 1;
	}
	else {
		std::cout << "Done\n";
	}
	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message
	freeaddrinfo(result);

	std::cout << "Please input message to be sent: ";
	std::cin >> input;
	const char* sendbuf = input.c_str();
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	do {
		iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFFLEN, 0);
		if (iResult > 0) {
			std::cout << "Message received:\n";
			for (int i = 0; i < DEFAULT_BUFFLEN; ++i) {
				std::cout << recvbuf[i];
			}
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);

	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}