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
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &sock.hints, &sock.result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
    return 0;
}

