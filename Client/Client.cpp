// Client.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Client.h"
#include <iostream>
WSADATA wsaData;

int main(int argc, char *argv[])
{
	int iResult;
	RaggSock sock = RaggSock();
	sock.family(AF_INET);
	// Resolve the server address and port
	/*iResult = getaddrinfo(argv[1], DEFAULT_PORT, &sock.hints, &sock.result);*/
	iResult = sock.getInfo(argv[1], DEFAULT_PORT);
	//if (iResult != 0) {
	//	printf("getaddrinfo failed: %d\n", iResult);
	//	WSACleanup();
	//	return 1;
	//}

	SOCKET ConnectSocket = INVALID_SOCKET;
	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	sock.ptr = sock.result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(sock.ptr->ai_family, sock.ptr->ai_socktype, sock.ptr->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(sock.result);
		WSACleanup();
		return 1;
	}

	// Connect to server.
	iResult = connect(ConnectSocket, sock.ptr->ai_addr, (int)sock.ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		//std::cout << "Unable to connect to" << sock.ptr->ai_addr << std::endl;
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(sock.result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	int recvbuflen = DEFAULT_BUFLEN;

	char *sendbuf = (char*) "this is a test";
	char recvbuf[DEFAULT_BUFLEN];

	//int iResult;

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection for sending since no more data will be sent
	// the client can still use the ConnectSocket for receiving data
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive data until the server closes the connection
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			recvbuf[iResult] = 0;
			std::cout << "From server: " << recvbuf << std::endl;
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);
    return 0;
}

