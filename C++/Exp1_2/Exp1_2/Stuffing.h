#pragma once
#include<stdio.h>
#include<string>
#include<string.h>
#include<iostream>
#define BUF_SIZE 1024//读入数据的缓冲区大小
using namespace std;
class Message {
public:
	string message_row;//原始的帧数据信息
	string message_stuffing;//填充后的信息
	string message_destuffing;//去除填充后的信息
public:
	//设置原始的数据信息
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
	string flag;//位填充的帧起始、结束标志
public:
	//设置位填充的帧起始、结束标志
	void setFlag(char name[], char buf[]) {
		string pre = name;
		string str = buf;
		str.erase(0, pre.size());
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = '\0';
		flag = str;
	}
	//获得位填充的帧起始、结束标志
	string getFlag() {
		return flag;
	}
	//位填充
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
	//去除位填充
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
	//打印需要输出的信息
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
	string flag;//字节填充的帧起始、结束标志
	string EscString;//Esc符号
public:
	//设置字节填充的帧起始、结束标志
	void setFlag(char name[], char buf[]) {
		string pre = name;
		string str = buf;
		str.erase(0, pre.size());
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = '\0';
		flag = str;
	}
	//设置Esc符号
	void setEscString(char name[], char buf[]) {
		string pre = name;
		string str = buf;
		str.erase(0, pre.size());
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = '\0';
		EscString = str;
	}
	//获取Esc符号
	string getFlag() {
		return flag;
	}
	//字节填充
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
	//去除字节填充
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
	//打印需要输出的信息
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