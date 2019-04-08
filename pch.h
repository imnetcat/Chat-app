
#ifndef PCH_H
#define PCH_H


#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <WinSock2.h> // Для сокетов
#include <ws2tcpip.h> // Для сокетов
#include <iphlpapi.h> // Для GetIpAddrTable

#pragma comment(lib, "Ws2_32.lib") // Для сокетов
#pragma comment(lib, "Iphlpapi.lib") // Для GetIpAddrTable

using namespace std;

#endif //PCH_H
