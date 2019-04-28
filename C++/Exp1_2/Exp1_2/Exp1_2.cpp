#include<stdio.h>
#include<string>
#include<string.h>
#include<iostream>
#include"Stuffing.h"
#define BUF_SIZE 1024//读入数据的缓冲区大小
using namespace std;


int main() {
	//文件指针，用以读取配置文件中数据
	FILE *fp;
	//打开配置文件
	fp = fopen("config002", "r");

	BitStuffing bits;//位填充
	ByteStuffing bytes;//字节填充
	char buf[BUF_SIZE];//缓冲区，用以存储读取的数据
	if (fgets(buf, BUF_SIZE, fp) == NULL) {
		printf("Get InfoString1 Failed!\n");
		exit(-1);
	}
	//去除读取的数据最后的'\n'
	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\0';
	//读取用于位填充的原始的帧数据信息
	char BitStrName[BUF_SIZE] = "InfoString1=";
	bits.setMessageRow(BitStrName, buf);

	if (fgets(buf, BUF_SIZE, fp) == NULL) {
		printf("Get FlagString Failed!\n");
		exit(-1);
	}
	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\0';
	//读取flag标志
	char BitFlagName[BUF_SIZE] = "FlagString=";
	bits.setFlag(BitFlagName, buf);

	bits.Stuffing();
	bits.Destuffing();
	bits.Print();

	if (fgets(buf, BUF_SIZE, fp) == NULL) {
		printf("Get InfoString1 Failed!\n");
		exit(-1);
	}
	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\0';
	//读取用于字节填充的原始的帧数据信息
	char ByteStrName[BUF_SIZE] = "InfoString1=";
	bytes.setMessageRow(ByteStrName, buf);

	if (fgets(buf, BUF_SIZE, fp) == NULL) {
		printf("Get FlagString Failed!\n");
		exit(-1);
	}
	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\0';
	//读取flag标志
	char ByteFlagName[BUF_SIZE] = "FlagString=";
	bytes.setFlag(ByteFlagName, buf);

	if (fgets(buf, BUF_SIZE, fp) == NULL) {
		printf("Get EscString Failed!\n");
		exit(-1);
	}
	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\0';
	//读取Esc符号
	char EscName[BUF_SIZE] = "EscString=";
	bytes.setEscString(EscName, buf);

	bytes.Stuffing();
	bytes.Destuffing();
	bytes.Print();
	//关闭文件指针
	fclose(fp);
	system("pause");
}