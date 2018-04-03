#include "RaggSock.h"

RaggSock::RaggSock()
{
	
	result = NULL; ptr = NULL;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		//return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	//ptr = result;
	//ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
}
RaggSock *RaggSock::family(int fam) {
	hints.ai_family = fam;
	return this;
}
RaggSock *RaggSock::socktype(int socktype) {
	hints.ai_socktype = socktype;
	return this;
}
RaggSock *RaggSock::protocol(int protocol) {
	hints.ai_protocol = protocol;
	return this;
}
RaggSock *RaggSock::flags(int flags) {
	hints.ai_flags = flags;
	return this;
}
int RaggSock::getInfo(char *argv, const char * port) {
	int iResult = getaddrinfo(argv, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		exit(1);
		//return 1;
	}
	return iResult;
}
RaggSock::~RaggSock()
{
}
