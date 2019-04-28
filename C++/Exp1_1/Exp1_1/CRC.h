#pragma once
#include <string.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 1024
using namespace std;
class Person
{
private:
	string GenXString;			//多项式的二进制串表示
public:
	int GenX_len;				//多项式的二进制串长度
public:
	//设置GenXString值
	void setGenXString(char name[], char buf[]) {
		string pre = name;
		string str = buf;
		str.erase(0, pre.size());
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = '\0';
		GenXString = str;
		GenX_len = str.size();
	}
	//获得GenXString值
	string getGenXString() {
		return GenXString;
	}
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
		string zero = GenerateSuffix(GenX_len);
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
class Sender :public Person {
private:
	string message_row;			//未处理的需要传输的二进制串
	string message_sending;		//CRC处理后的发送的二进制串
public:
	//设置message_row值
	void setMessageRow(char name[], char buf[]) {
		string pre = name;
		string str = buf;
		str.erase(0, pre.size());
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = '\0';
		message_row = str;
	}
	//生成发送的二进制串
	void GenerateMessageSending() {
		string suf = GenerateSuffix(GenX_len - 1);
		string str = message_row + suf;
		string remainder = CalculateRemainder(str);
		message_sending = message_row + remainder;
	}
	//获得message_sending的值
	string getMessageSending() {
		return message_sending;
	}
	//获得message_row的值
	string getMessageRow() {
		return message_row;
	}
	//打印需要输出的发送方的信息
	void Print() {
		cout << "InfoString1" << endl;
		cout << message_row << endl;
		cout << "GenXString" << endl;
		cout << getGenXString() << endl;
		cout << "Message" << endl;
		cout << message_sending << endl;
	}
};


class Receiver :public Person {
private:
	string message_received;			//收到的二进制串
	string remainder;					//根据收到的二进制串计算得到的余数
public:
	//设置message_received的值
	void setMessageReceived(char name[], char buf[]) {
		string pre = name;
		string str = buf;
		str.erase(0, pre.size());
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = '\0';
		message_received = str;
	}
	//检查收到的二进制串是否正确，即检查余数是否为0
	bool Check() {
		remainder = CalculateRemainder(message_received);
		string zero_string = GenerateSuffix(GenX_len - 1);
		return (remainder == zero_string);
	}
	//获得message_received的值
	string getMessageReceived() {
		return message_received;
	}
	//获得余数
	string getRemainder() {
		return remainder;
	}
	//打印需要输出的接收方的信息
	void Print() {
		string remainder_print;
		int pos;
		if ((pos = remainder.find('1')) == -1) {
			remainder_print = "0";
		}
		else remainder_print = remainder.substr(pos);
		cout << "InfoString2" << endl;
		cout << message_received << endl;
		cout << "GenXString" << endl;
		cout << getGenXString() << endl;
		cout << "Remainder" << endl;
		cout << remainder_print << endl;
		cout << "isSuccess" << endl;
		cout << boolalpha << Check() << endl;
	}
};