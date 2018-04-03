// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\RaggSock\RaggSock.h"
//#include "Server.h"

int main()
{
	int iResult;
	RaggSock sock = RaggSock();
	sock.family(AF_INET)->socktype(SOCK_STREAM)->protocol(IPPROTO_TCP)->flags(AI_PASSIVE);

	// Resolve the local address and port to be used by the server
	//iResult = getaddrinfo(NULL, DEFAULT_PORT, &sock.hints, &sock.result);
	iResult = sock.getInfo(NULL, DEFAULT_PORT);
	//if (iResult != 0) {
	//	printf("getaddrinfo failed: %d\n", iResult);
	//	WSACleanup();
	//	return 1;
	//}
	printf("IP: %d\n", iResult);
	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(sock.result->ai_family, sock.result->ai_socktype, sock.result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(sock.result);
		WSACleanup();
		return 1;
	}

	iResult = bind(ListenSocket, sock.result->ai_addr, (int)sock.result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(sock.result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	freeaddrinfo(sock.result);

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;
	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
#define DEFAULT_BUFLEN 512

	char recvbuf[DEFAULT_BUFLEN];
	int iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while (iResult > 0);
    return 0;


}

