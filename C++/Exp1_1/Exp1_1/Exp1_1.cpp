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
	//�ļ�ָ�룬���Զ�ȡ�����ļ�������
	FILE *fp;
	//�������ļ�
	fp = fopen("config001", "r");
	class Sender sender;//���ͷ�
	class Receiver receiver;//���շ�
	char buf[BUF_SIZE];//�����������Դ洢��ȡ������
	if (fgets(buf, BUF_SIZE, fp) == NULL)
	{
		printf("Get InfoString1 Failed!\n");
		exit(-1);
	}
	//ȥ����ȡ����������'\n'
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
	//�ر��ļ�ָ��
	fclose(fp);
	//��ӡ��Ҫ����Ϣ������̨
	sender.Print();
	receiver.Print();

	system("pause");
}
