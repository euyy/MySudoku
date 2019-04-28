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
	string GenXString;			//����ʽ�Ķ����ƴ���ʾ
public:
	int GenX_len;				//����ʽ�Ķ����ƴ�����
public:
	//����GenXStringֵ
	void setGenXString(char name[], char buf[]) {
		string pre = name;
		string str = buf;
		str.erase(0, pre.size());
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = '\0';
		GenXString = str;
		GenX_len = str.size();
	}
	//���GenXStringֵ
	string getGenXString() {
		return GenXString;
	}
	//����0���ĳ���Ϊlen���ַ���
	string GenerateSuffix(int len) {
		string str = "";
		for (int i = 0; i < len; i++) {
			str += "0";
		}
		return str;
	}
	//�ַ���������
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
	//����str����GenXString������
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
	string message_row;			//δ�������Ҫ����Ķ����ƴ�
	string message_sending;		//CRC�����ķ��͵Ķ����ƴ�
public:
	//����message_rowֵ
	void setMessageRow(char name[], char buf[]) {
		string pre = name;
		string str = buf;
		str.erase(0, pre.size());
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = '\0';
		message_row = str;
	}
	//���ɷ��͵Ķ����ƴ�
	void GenerateMessageSending() {
		string suf = GenerateSuffix(GenX_len - 1);
		string str = message_row + suf;
		string remainder = CalculateRemainder(str);
		message_sending = message_row + remainder;
	}
	//���message_sending��ֵ
	string getMessageSending() {
		return message_sending;
	}
	//���message_row��ֵ
	string getMessageRow() {
		return message_row;
	}
	//��ӡ��Ҫ����ķ��ͷ�����Ϣ
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
	string message_received;			//�յ��Ķ����ƴ�
	string remainder;					//�����յ��Ķ����ƴ�����õ�������
public:
	//����message_received��ֵ
	void setMessageReceived(char name[], char buf[]) {
		string pre = name;
		string str = buf;
		str.erase(0, pre.size());
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = '\0';
		message_received = str;
	}
	//����յ��Ķ����ƴ��Ƿ���ȷ������������Ƿ�Ϊ0
	bool Check() {
		remainder = CalculateRemainder(message_received);
		string zero_string = GenerateSuffix(GenX_len - 1);
		return (remainder == zero_string);
	}
	//���message_received��ֵ
	string getMessageReceived() {
		return message_received;
	}
	//�������
	string getRemainder() {
		return remainder;
	}
	//��ӡ��Ҫ����Ľ��շ�����Ϣ
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