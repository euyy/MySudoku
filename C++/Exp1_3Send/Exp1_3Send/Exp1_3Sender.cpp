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
class Filter {
public:
	int FilterError;
	int FilterLost;
public:
	void InitFilter(int err, int lost) {
		FilterError = err;
		FilterLost = lost;
	}
	bool isError(int i) {
		srand(time(0) + i);
		if ((rand() % FilterError) == FilterError / 2 - 1)
			return true;
		else return false;
	}
	bool isLost(int i) {
		srand(time(0) + i);
		if ((rand() % FilterLost) == FilterLost / 2)
			return true;
		else return false;
	}
};
class CRC {
private:
	string GenXString = "10001000000100001";			//多项式的二进制串表示
public:
	int GenX_len = 17;				//多项式的二进制串长度
public:
	//生成0填充的长度为len的字符串
	string GenerateSuffix(int len) {
		string str = "";
		for (int i = 0; i < len; i++) {
			str += "0";
		}
		return str;
	}
	//字符串异或操作
	string strXORstr(string str1, string str2) {
		string res = "";
		for (int i = 1; i < str1.size(); i++) {
			if (str1[i] == str2[i]) {
				res += "0";
			}
			else res += "1";
		}
		return res;
	}
	//计算str除以GenXString的余数
	string CalculateRemainder(string str) {
		int ptail = GenX_len;
		string remainder = str.substr(0, GenX_len);
		for (; ptail < str.size(); ptail++) {
			if (remainder[0] == '0') {
				remainder.erase(0, 1);
				remainder += str[ptail];
			}
			else {
				remainder = strXORstr(remainder, GenXString);
				remainder += str[ptail];
			}
		}
		remainder.erase(0, 1);
		return remainder;
	}
	string GenerateMessageSending(string message) {
		string suf = GenerateSuffix(GenX_len - 1);
		string str = message + suf;
		string remainder = CalculateRemainder(str);
		string res = message + remainder;
		return res;
	}
};
class Message :public CRC {
public:
	char myPacket[50][50] = { "package1","package2","package3","package4","package1","package2","package3","package4" };
	int PacketNum = 8;
public:
	string Pack(int i, int sn) {
		string msg = to_string(sn) + myPacket[i];
		string bitString = string2bitstring(msg);
		string packetSending = GenerateMessageSending(bitString);
		return packetSending;
	}
	string string2bitstring(string str) {
		string res = "";
		int len = str.size();
		for (int i = 0; i < len; i++) {
			res += char2bitstring(str[i]);
		}
		return res;
	}
	string char2bitstring(char c) {
		string res = "";
		unsigned char k = 0x80;
		for (int i = 0; i < 8; i++, k >>= 1) {
			if (c & k) {
				res += "1";
			}
			else {
				res += "0";
			}
		}
		return res;
	}
};

class mySocket {
public:
	WSADATA wsadata;
	SOCKET sockSend;
	SOCKET sockRecv;
	sockaddr_in recvAddr;
	sockaddr_in sendAddr;

public:
	void InitSocket(int port) {

		WSAStartup(0x0202, &wsadata);

		char lv_name[50];
		gethostname(lv_name, 50);
		hostent * lv_pHostent;
		lv_pHostent = (hostent *)malloc(sizeof(hostent));
		if (NULL == (lv_pHostent = gethostbyname(lv_name)))
		{
			printf("get Hosrname Fail \n");
			exit(-1);
		}
		SOCKADDR_IN lv_sa;
		lv_sa.sin_family = AF_INET;
		lv_sa.sin_port = htons(6000);
		memcpy(&lv_sa.sin_addr.S_un.S_addr, lv_pHostent->h_addr_list[0], lv_pHostent->h_length);

		sockSend = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		sockRecv = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		recvAddr.sin_family = AF_INET;
		recvAddr.sin_port = htons(port + 1);
		recvAddr.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(lv_sa.sin_addr));

		sendAddr.sin_family = AF_INET;
		sendAddr.sin_port = htons(port);
		sendAddr.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(lv_sa.sin_addr));

		bind(sockRecv, (sockaddr*)&recvAddr, sizeof(recvAddr));

		int t = 2000;
		setsockopt(sockRecv, SOL_SOCKET, SO_RCVTIMEO, (char*)&t, sizeof(timeval));
	}
};

class Sender {
private:
	mySocket skt;
	Message msg;
	Filter flt;
public:
	void Init() {
		FILE *fp;
		fp = fopen("config003", "r");
		char buf[BUF_SIZE];
		//获得端口号，初始化Socket
		if (fgets(buf, BUF_SIZE, fp) == NULL) {
			printf("Get UDPPort Failed!\n");
			exit(-1);
		}
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = '\0';
		char* ptr;
		if ((ptr = strchr(buf, '=')) != NULL) {
			ptr = ptr + 1;
		}
		int port = atoi(ptr);
		skt.InitSocket(port);

		//获得Error和Lost值，初始化Filter
		if (fgets(buf, BUF_SIZE, fp) == NULL) {
			printf("Get FilterError Failed!\n");
			exit(-1);
		}
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = '\0';
		if ((ptr = strchr(buf, '=')) != NULL) {
			ptr = ptr + 1;
		}
		int err = atoi(ptr);

		if (fgets(buf, BUF_SIZE, fp) == NULL) {
			printf("Get FilterLost Failed!\n");
			exit(-1);
		}
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = '\0';
		if ((ptr = strchr(buf, '=')) != NULL) {
			ptr = ptr + 1;
		}
		int lost = atoi(ptr);
		flt.InitFilter(err, lost);
		fclose(fp);
	}
	void sendMessage() {
		char  buf[BUF_SIZE];
		int32_t nSendAddLen = sizeof(skt.sendAddr);
		int32_t nRecvAddLen = sizeof(skt.recvAddr);

		int sn = 0;
		string packageSending;
		for (int i = 0; i < msg.PacketNum; i++) {
			cout << "---------------------------------------" << endl;
			while (true) {
				packageSending = msg.Pack(i, sn);
				cout << "frame to send " << sn << " data no " << i << endl;
				if (flt.isLost(sn)) {
					printf("Frame Lost!\n");
				}
				else if (flt.isError(sn)) {
					printf("Frame Error!\n");
					packageSending = packageSending + msg.char2bitstring('0');
					strcpy(buf, packageSending.c_str());
					sendto(skt.sockSend, buf, strlen(buf), 0, (sockaddr *)&skt.sendAddr, nSendAddLen);
				}
				else {
					strcpy(buf, packageSending.c_str());
					sendto(skt.sockSend, buf, strlen(buf), 0, (sockaddr *)&skt.sendAddr, nSendAddLen);
				}

				if (recvfrom(skt.sockRecv, buf, BUF_SIZE, 0, (sockaddr *)&skt.recvAddr, &nRecvAddLen) <= 0) {
					//cout << WSAGetLastError() << endl;
					cout << "timeout sn " << sn << " no " << i << endl;
				}
				else {
					int ack = buf[0] - '0';
					if (ack == (1 - sn)) {
						sn = 1 - sn;
						cout << "ack get " << ack << endl;
						break;
					}
					else {
						cout << "wrong sn " << sn << " no " << i << endl;
					}
				}
			}
		}
		closesocket(skt.sockSend);
		WSACleanup();
	}
};

int main() {

	Sender sender;
	sender.Init();
	sender.sendMessage();

	system("pause");
	return 0;
}