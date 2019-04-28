#include<stdio.h>
#include<string>
#include<string.h>
#include<iostream>
#include"Stuffing.h"
#define BUF_SIZE 1024//�������ݵĻ�������С
using namespace std;


int main() {
	//�ļ�ָ�룬���Զ�ȡ�����ļ�������
	FILE *fp;
	//�������ļ�
	fp = fopen("config002", "r");

	BitStuffing bits;//λ���
	ByteStuffing bytes;//�ֽ����
	char buf[BUF_SIZE];//�����������Դ洢��ȡ������
	if (fgets(buf, BUF_SIZE, fp) == NULL) {
		printf("Get InfoString1 Failed!\n");
		exit(-1);
	}
	//ȥ����ȡ����������'\n'
	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\0';
	//��ȡ����λ����ԭʼ��֡������Ϣ
	char BitStrName[BUF_SIZE] = "InfoString1=";
	bits.setMessageRow(BitStrName, buf);

	if (fgets(buf, BUF_SIZE, fp) == NULL) {
		printf("Get FlagString Failed!\n");
		exit(-1);
	}
	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\0';
	//��ȡflag��־
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
	//��ȡ�����ֽ�����ԭʼ��֡������Ϣ
	char ByteStrName[BUF_SIZE] = "InfoString1=";
	bytes.setMessageRow(ByteStrName, buf);

	if (fgets(buf, BUF_SIZE, fp) == NULL) {
		printf("Get FlagString Failed!\n");
		exit(-1);
	}
	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\0';
	//��ȡflag��־
	char ByteFlagName[BUF_SIZE] = "FlagString=";
	bytes.setFlag(ByteFlagName, buf);

	if (fgets(buf, BUF_SIZE, fp) == NULL) {
		printf("Get EscString Failed!\n");
		exit(-1);
	}
	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\0';
	//��ȡEsc����
	char EscName[BUF_SIZE] = "EscString=";
	bytes.setEscString(EscName, buf);

	bytes.Stuffing();
	bytes.Destuffing();
	bytes.Print();
	//�ر��ļ�ָ��
	fclose(fp);
	system("pause");
}