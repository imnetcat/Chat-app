
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <WinSock2.h> // Для сокетов
#include <ws2tcpip.h> // Для сокетов
#include <iphlpapi.h> // Для GetIpAddrTable

#pragma comment(lib, "Ws2_32.lib") // Для сокетов
#pragma comment(lib, "Iphlpapi.lib") // Для GetIpAddrTable

#ifdef __GNUC__
   extern "C" {
   WINSOCK_API_LINKAGE  INT WSAAPI inet_pton( INT Family, PCSTR pszAddrString, PVOID pAddrBuf);
   WINSOCK_API_LINKAGE  PCSTR WSAAPI inet_ntop(INT  Family, PVOID pAddr, PSTR pStringBuf, size_t StringBufSize);
   }
#endif

using namespace std;

