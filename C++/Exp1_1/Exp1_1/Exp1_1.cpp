#include <string.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "CRC.h"
#define BUF_SIZE 1024
using namespace std;


int main()
{
	//文件指针，用以读取配置文件中数据
	FILE *fp;
	//打开配置文件
	fp = fopen("config001", "r");
	class Sender sender;//发送方
	class Receiver receiver;//接收方
	char buf[BUF_SIZE];//缓冲区，用以存储读取的数据
	if (fgets(buf, BUF_SIZE, fp) == NULL)
	{
		printf("Get InfoString1 Failed!\n");
		exit(-1);
	}
	//去除读取的数据最后的'\n'
	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\0';
	char Str1Name[BUF_SIZE] = "InfoString1=";
	sender.setMessageRow(Str1Name, buf);

	if (fgets(buf, BUF_SIZE, fp) == NULL)
	{
		printf("Get GenXString Failed!\n");
		exit(-1);
	}
	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\0';
	char GenXName[BUF_SIZE] = "GenXString=";
	sender.setGenXString(GenXName, buf);
	receiver.setGenXString(GenXName, buf);

	sender.GenerateMessageSending();

	if (fgets(buf, BUF_SIZE, fp) == NULL)
	{
		printf("Get InfoString2 Failed!\n");
		exit(-1);
	}
	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\0';
	char Str2Name[BUF_SIZE] = "InfoString2=";
	receiver.setMessageReceived(Str2Name, buf);
	//关闭文件指针
	fclose(fp);
	//打印需要的信息到控制台
	sender.Print();
	receiver.Print();

	system("pause");
}
