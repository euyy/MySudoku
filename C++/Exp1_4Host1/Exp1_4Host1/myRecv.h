#pragma once
#include"common.h"
class CRCRecv {
private:
	string GenXString = "10001000000100001";			//多项式的二进制串表示
public:
	int GenX_len = 17;				//多项式的二进制串长度
public:

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
};
class MessageRecv :public CRCRecv {
public:
	char msg[50][50];
	int msgNum = 0;
public:
	string UnPack(string message) {
		int num = message.size() / 8;
		string res = "";
		for (int i = 0; i < num; i++) {
			int tmp = 0;
			for (int j = 0; j < 8; j++) {
				tmp = tmp * 2 + message[i * 8 + j] - '0';
			}
			res += (char)tmp;
		}
		return res;
	}
	bool checkReceiveMessage(string msg) {
		string remainder = CalculateRemainder(msg);
		string zero_string = GenerateSuffix(GenX_len - 1);
		return (remainder == zero_string);
	}
	bool Check(string msg1, int sn) {
		if (checkReceiveMessage(msg1) == false) {
			return false;
		}
		string message = msg1.substr(0, msg1.size() - GenX_len + 1);
		message = UnPack(message);
		cout << "ack get " << message[0] << endl;
		if ((message[0] - '0') == sn) {
			message.erase(0, 1);
			strcpy(msg[msgNum++], message.c_str());
			return true;
		}
		else return false;
	}
	string GenerateSuffix(int len) {
		string str = "";
		for (int i = 0; i < len; i++) {
			str += "0";
		}
		return str;
	}
};

class mySocketRecv {
public:
	WSADATA wsadata;
	SOCKADDR_IN sendAddr;
	SOCKADDR_IN recvAddr;
	SOCKET      sockRecv;
	SOCKET      sockSend;
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


		sockRecv = socket(AF_INET, SOCK_DGRAM, 0);

		sendAddr.sin_family = AF_INET;
		sendAddr.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(lv_sa.sin_addr));
		sendAddr.sin_port = htons(port + 1);
		recvAddr.sin_family = AF_INET;
		recvAddr.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(lv_sa.sin_addr));
		recvAddr.sin_port = htons(port);

		bind(sockRecv, (SOCKADDR*)&recvAddr, sizeof(SOCKADDR));
		int t = 2000;
		setsockopt(sockRecv, SOL_SOCKET, SO_RCVTIMEO, (char*)&t, sizeof(timeval));
	}
};

class Receiver {
private:
	mySocketRecv skt;
	MessageRecv msg;
	Filter flt;
public:
	void Init() {
		FILE *fp;
		fp = fopen("E:\\learning\\test\\computer_network-master\\python\\config003", "r");
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
		skt.InitSocket(port + 10);

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

	}
	void recvMessage() {
		char  buf[BUF_SIZE];
		ZeroMemory(buf, BUF_SIZE);

		int nSendAddLen = sizeof(skt.sendAddr);
		int nRecvAddLen = sizeof(skt.recvAddr);

		int sn = 0;//窗口中的序号
		int no = 0;//当前发送的帧序号
		string packetRecv;
		while (true) {
			sn = no % (MAX_SEQ + 1);
			cout << "frame expected " << sn << endl;
			if (recvfrom(skt.sockRecv, buf, BUF_SIZE, 0, (sockaddr*)&skt.recvAddr, &nRecvAddLen) > 0) {
				packetRecv = buf;
				if (msg.Check(packetRecv, sn) == true) {
					cout << "right frame sn " << sn << endl;
					no++;
					sn = no % (MAX_SEQ + 1);
					string msg = to_string(sn);
					strcpy(buf, msg.c_str());
					sendto(skt.sockSend, buf, strlen(buf), 0, (sockaddr *)&skt.sendAddr, nSendAddLen);
					cout << "ack send " << sn << endl;
					cout << "---------------------------------------" << endl;
				}
				else {
					cout << "wrong sn" << endl;
				}
			}
			else {
				cout << "timeout sn " << sn << " no " << no << endl;
				string msg = to_string(sn);
				strcpy(buf, msg.c_str());
				sendto(skt.sockSend, buf, strlen(buf), 0, (sockaddr *)&skt.sendAddr, nSendAddLen);
				cout << "ack send " << sn << endl;
				cout << "---------------------------------------" << endl;
			}

		}
		closesocket(skt.sockRecv);
		closesocket(skt.sockSend);
		WSACleanup();
	}
};

void myReceive() {
	Receiver receiver;
	receiver.Init();
	receiver.recvMessage();
}