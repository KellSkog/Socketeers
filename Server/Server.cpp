// Server.cpp : Defines the entry point for the console application.
//
//#include <windows.h>
#include <tchar.h>
//#include <strsafe.h>
#include "stdafx.h"
#include "..\RaggSock\RaggSock.h"
#include <iostream>

DWORD WINAPI MyThreadFunction(LPVOID lpParam);

int main()
{
	int iResult;
	RaggSock sock = RaggSock();
	sock.family(AF_INET)->socktype(SOCK_STREAM)->protocol(IPPROTO_TCP)->flags(AI_PASSIVE);

	// Resolve the local address and port to be used by the server
	//iResult = getaddrinfo(NULL, DEFAULT_PORT, &sock.hints, &sock.result);
	iResult = sock.getInfo(NULL, "80");
	//if (iResult != 0) {
	//	printf("getaddrinfo failed: %d\n", iResult);
	//	WSACleanup();
	//	return 1;
	//}

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
#define MAX_THREADS 2
	SOCKET ClientSocket[MAX_THREADS];
	HANDLE  hThreadArray[MAX_THREADS];
	DWORD   dwThreadIdArray[MAX_THREADS];
	// Accept a client socket

	for(int i = 0; i < MAX_THREADS; i++ ){
		ClientSocket[i] = accept(ListenSocket, NULL, NULL);
		if (ClientSocket[i] > 0) {
			hThreadArray[i] = CreateThread(NULL, 0, MyThreadFunction, &ClientSocket[i], 0, &dwThreadIdArray[i]);
		}
		else {std::cout << "Failed con:" << ClientSocket[i] << std::endl;}
	}
	std::cout << "Done:" << ClientSocket[0] << ClientSocket[1] << std::endl;
	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, 20000);//Wait for 20 seconds
	//if (ClientSocket == INVALID_SOCKET) {
	//	printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
	//	return 1;
	//}

    return 0;
}

#define DEFAULT_BUFLEN 512
DWORD WINAPI MyThreadFunction(LPVOID lpParam) {
	char recvbuf[DEFAULT_BUFLEN];// , sendbuf[DEFAULT_BUFLEN];
	int iSendResult, iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	SOCKET *sock = (SOCKET*) lpParam;
	// Receive until the peer shuts down the connection
	std::cout << "Socket: " << *sock << std::endl;
	do {

		iResult = recv(*sock, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			//printf("Bytes received: %d\n", iResult);
			recvbuf[iResult] = 0;
			printf("%s\n%d\n", recvbuf, iResult);
			// Echo the buffer back to the sender
			//	iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			//if (iSendResult == SOCKET_ERROR) {
			//	printf("send failed: %d\n", WSAGetLastError());
			//	closesocket(ClientSocket);
			//	WSACleanup();
			//	return 1;
			//}
			//printf("Bytes sent: %d\n", iSendResult);
			printf("Ok: %d\n", send(*sock, "HTTP/1.1 200 OK\n", 17, 0));
			printf("Cont: %d\n", send(*sock, "Content-type:text/html\n", 24, 0));
			printf("Newlines: %d\n", send(*sock, "\n\n", 3, 0));

			break;
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		//else {
		//	printf("recv failed: %d\n", WSAGetLastError());
		//	closesocket(ClientSocket);
		//	WSACleanup();
		//	return 1;
		//}

	} while (iResult > 0);
	closesocket(*sock);
	return 0;
}