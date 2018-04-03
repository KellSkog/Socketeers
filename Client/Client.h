#pragma once
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "..\RaggSock\RaggSock.h"
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char * argv[]);
