#pragma once
#include"common.h"
class CRCSend {
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
class MessageSend :public CRCSend {
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

class mySocketSend {
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

		setsockopt(sockRecv, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeForSend, sizeof(timeval));
	}
};

class Sender {
private:
	mySocketSend skt;
	MessageSend msg;
	Filter flt;
	clock_t stime[MAX_SEQ + 1];
	int first_waiting_frame_no = 0;
	int waiting_frame_count = 0;
	int timeout = 5000;
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
	int modnum(int x, int y) {
		int res = (x - y) % (MAX_SEQ + 1);
		if (res >= 0) {
			return res;
		}
		else {
			return (res + MAX_SEQ + 1);
		}

	}

	void sendMessage() {
		char  buf[BUF_SIZE];
		int32_t nSendAddLen = sizeof(skt.sendAddr);
		int32_t nRecvAddLen = sizeof(skt.recvAddr);

		int sn = 0;//窗口中的序号
		int no = 0;//当前发送的帧序号
		int successNum = 0;//发送成功的数
		string packageSending;
		while (successNum != msg.PacketNum)
		{
			clock_t nowtime = clock();
			//超时
			if (waiting_frame_count > 0) {
				if ((nowtime - stime[first_waiting_frame_no]) >= timeout) {
					no = no - waiting_frame_count;
					sn = no % (MAX_SEQ + 1);
					waiting_frame_count = 0;
				}
			}
			sn = no % (MAX_SEQ + 1);
			cout << "---------------------------------------" << endl;


			if ((waiting_frame_count == MAX_SEQ + 1) || (no == msg.PacketNum));//窗口满或发送完，不发送
			else
			{
				packageSending = msg.Pack(no, sn);
				cout << "frame to send " << sn << " data no " << no << endl;
				if (flt.isLost()) {//发送丢失
					printf("Frame Lost!\n");
				}
				else if (flt.isError()) {//发送错误的包
					printf("Frame Error!\n");
					packageSending = packageSending + msg.char2bitstring('0');
					strcpy(buf, packageSending.c_str());
					stime[sn] = clock();
					sendto(skt.sockSend, buf, strlen(buf), 0, (sockaddr *)&skt.sendAddr, nSendAddLen);
					no++;
					waiting_frame_count++;
				}
				else {//正确发送
					strcpy(buf, packageSending.c_str());
					stime[sn] = clock();
					sendto(skt.sockSend, buf, strlen(buf), 0, (sockaddr *)&skt.sendAddr, nSendAddLen);
					no++;
					waiting_frame_count++;
				}
			}
			//收到了ack
			if (recvfrom(skt.sockRecv, buf, BUF_SIZE, 0, (sockaddr *)&skt.recvAddr, &nRecvAddLen) > 0) {
				int ack = buf[0] - '0';
				cout << "ack get " << ack << endl;
				waiting_frame_count = waiting_frame_count - modnum(ack, first_waiting_frame_no);
				successNum = successNum + modnum(ack, first_waiting_frame_no) + 1;
				first_waiting_frame_no = (ack + 1) % (MAX_SEQ + 1);
			}
		}
		closesocket(skt.sockSend);
		WSACleanup();
	}
};
void mySend() {
	Sender sender;
	sender.Init();
	sender.sendMessage();
}