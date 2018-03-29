#pragma once
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define DEFAULT_PORT "27015"
#pragma comment(lib, "Ws2_32.lib")

class RaggSock
{
public:
	WSADATA wsaData;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	RaggSock();
	~RaggSock();
};

