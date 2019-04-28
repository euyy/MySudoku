#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include <string.h>
#include <string>
#include <time.h>
#define BUF_SIZE 1024
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
using namespace std;
#pragma comment(lib, "ws2_32.lib")
const int MAX_SEQ = 7;
int MAX_BUFSIZE = 7;
int timeForSend = 500;


class Filter {
public:
	int FilterError;
	int FilterLost;
public:
	void InitFilter(int err, int lost) {
		FilterError = err;
		FilterLost = lost;
	}
	bool isError() {
		srand(time(0));
		if ((rand() % FilterError) == FilterError / 2 - 1)
			return true;
		else return false;
	}
	bool isLost() {
		srand(time(0));
		if ((rand() % FilterLost) == FilterLost / 2)
			return true;
		else return false;
	}
};