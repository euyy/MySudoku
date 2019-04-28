#pragma once
#include<stdio.h>
#include<string>
#include<string.h>
#include<iostream>
#define BUF_SIZE 1024//�������ݵĻ�������С
using namespace std;
class Message {
public:
	string message_row;//ԭʼ��֡������Ϣ
	string message_stuffing;//�������Ϣ
	string message_destuffing;//ȥ���������Ϣ
public:
	//����ԭʼ��������Ϣ
	void setMessageRow(char name[], char buf[]) {
		string pre = name;
		string str = buf;
		str.erase(0, pre.size());
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = '\0';
		message_row = str;
	}
};
class BitStuffing :public Message {
private:
	string flag;//λ����֡��ʼ��������־
public:
	//����λ����֡��ʼ��������־
	void setFlag(char name[], char buf[]) {
		string pre = name;
		string str = buf;
		str.erase(0, pre.size());
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = '\0';
		flag = str;
	}
	//���λ����֡��ʼ��������־
	string getFlag() {
		return flag;
	}
	//λ���
	void Stuffing() {
		string substr_before = "11111";
		string substr_after = "111110";
		int pos = 0;
		int len_before = substr_before.size();
		int len_after = substr_after.size();
		message_stuffing = message_row;
		while ((pos = message_stuffing.find(substr_before, pos)) != string::npos)
		{
			message_stuffing.replace(pos, len_before, substr_after);
			pos += len_after;
		}
		message_stuffing = flag + message_stuffing + flag;
	}
	//ȥ��λ���
	void Destuffing() {
		message_destuffing = message_stuffing.substr(flag.size(), message_stuffing.size() - 2 * flag.size());
		string substr_after = "11111";
		string substr_before = "111110";
		int pos = 0;
		int len_before = substr_before.size();
		int len_after = substr_after.size();
		while ((pos = message_destuffing.find(substr_before, pos)) != string::npos)
		{
			message_destuffing.replace(pos, len_before, substr_after);
			pos += len_after;
		}
	}
	//��ӡ��Ҫ�������Ϣ
	void Print() {
		cout << "--------------------Bit Stuffing----------------------" << endl;
		cout << "InfoString1" << endl;
		cout << message_row << endl << endl;
		cout << "FlagString1" << endl;
		cout << flag << endl << endl;
		cout << "Stuffing InfoString" << endl;
		cout << message_stuffing << endl << endl;
		cout << "Destuffing InfoString" << endl;
		cout << message_destuffing << endl << endl;
		cout << "------------------------------------------------------" << endl;
	}
};
class ByteStuffing :public Message {
private:
	string flag;//�ֽ�����֡��ʼ��������־
	string EscString;//Esc����
public:
	//�����ֽ�����֡��ʼ��������־
	void setFlag(char name[], char buf[]) {
		string pre = name;
		string str = buf;
		str.erase(0, pre.size());
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = '\0';
		flag = str;
	}
	//����Esc����
	void setEscString(char name[], char buf[]) {
		string pre = name;
		string str = buf;
		str.erase(0, pre.size());
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = '\0';
		EscString = str;
	}
	//��ȡEsc����
	string getFlag() {
		return flag;
	}
	//�ֽ����
	void Stuffing() {
		string substr_before = EscString;
		string substr_after = EscString + EscString;
		message_stuffing = message_row;
		int pos = 0;
		int len_before = substr_before.size();
		int len_after = substr_after.size();

		while ((pos = message_stuffing.find(substr_before, pos)) != string::npos)
		{
			message_stuffing.replace(pos, len_before, substr_after);
			pos += len_after;
		}

		substr_before = flag;
		substr_after = EscString + flag;
		pos = 0;
		len_before = substr_before.size();
		len_after = substr_after.size();

		while ((pos = message_stuffing.find(substr_before, pos)) != string::npos)
		{
			message_stuffing.replace(pos, len_before, substr_after);
			pos += len_after;
		}
		message_stuffing = flag + message_stuffing + flag;
	}
	//ȥ���ֽ����
	void Destuffing() {
		message_destuffing = message_stuffing.substr(flag.size(), message_stuffing.size() - 2 * flag.size());
		string substr_before = EscString + flag;
		string substr_after = flag;
		int pos = 0;
		int len_before = substr_before.size();
		int len_after = substr_after.size();

		while ((pos = message_destuffing.find(substr_before, pos)) != string::npos)
		{
			message_destuffing.replace(pos, len_before, substr_after);
			pos += len_after;
		}

		substr_before = EscString + EscString;
		substr_after = EscString;
		pos = 0;
		len_before = substr_before.size();
		len_after = substr_after.size();

		while ((pos = message_destuffing.find(substr_before, pos)) != string::npos)
		{
			message_destuffing.replace(pos, len_before, substr_after);
			pos += len_after;
		}
	}
	//��ӡ��Ҫ�������Ϣ
	void Print() {
		cout << "-------------------Byte Stuffing----------------------" << endl;
		cout << "InfoString2" << endl;
		cout << message_row << endl << endl;
		cout << "FlagString2" << endl;
		cout << flag << endl << endl;
		cout << "EscString" << endl;
		cout << EscString << endl << endl;
		cout << "Stuffing InfoString" << endl;
		cout << message_stuffing << endl << endl;
		cout << "Destuffing InfoString" << endl;
		cout << message_destuffing << endl << endl;
		cout << "------------------------------------------------------" << endl;
	}
};