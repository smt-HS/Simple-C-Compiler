#ifndef INTER_CODE_H
#define INTER_CODE_H
#include <string>
#include <iostream>
#include "LexicalAnalysis.h"

#define MAX_CODE_LEN 5000
using namespace std;
/*
��Ԫʽ����������
�������㣺�����ܷ���ջ���ڴ���䣩
	Add,Sub,Multi,Div
�ô�ȡֵ:�����ܷ���ջ���ڴ���䣩
	IntGet CharGet
�ô渳ֵ��
	IntSet CharSet
I/O:
	PRINTF SCANF
�ڴ���䣺
	��������	//������������Ԫʽ����˳��
		ConstInt ConstChar GlobVarInt GlobVarChar LdIm
	ջ��		LocalVarInt LocalVarChar PARA
��ת��//�����������־
	��������Call RET GOTO FuncDefEnd  MainEnd
	��������BGR BNE BLS BLE BGE BEQ
��ǩ����://�����鿪ʼ��־
	GenLab MainBegin FuncDefBegin

*/
enum Operator
{
	Add, Sub, Multi, Div,
	IntSet,//���α�����ֵ  a[1]=b --> IntSet  b 1 a    a=b --> IntSet b 0 a
	IntGet,//���α���ȡֵ  b=a[1] --> IntGet  a 1 b    a=b --> IntGet b 0 a
	CharSet,//�ַ�������ֵ a[1]=b --> CharSet  b 1 a    a=b --> CharSet b 0 a
	CharGet,//�ַ�����ȡֵ  b=a[1] --> CharGet  a 1 b    a=b --> CharGet b 0 a
	Call,////��������  f(); -->  Call f -1/0/1 ����ֵ��� -1��void�� 0:int�� 1��char��
	LdIm,//����������   3 --> LdIm 3  0/1 generateInterValue 0 Ϊ���� 1Ϊ�ַ�
	GenLab,//���ɱ�ǩ  ��ǰλ�����ɱ�ǩ GenLab  -1 -1 lab
	BGR,//��������ת if(t1>t2)  -->  BGR t1 t2 lab
	BNE,//��������ת if(t1!=t2) -->  BNE t1 t2 lab
	BLS,//С������ת
	BLE,//С�ڵ�������ת
	BGE,//���ڵ�������ת
	BEQ,//��������ת
	GOTO,//��������ת GOTO -1 -1 lab
	PARA,//���ݲ���  ����Ϊfno�ĺ������ݲ���t : PARA t fno -1
	SCANF,//�������� scanf(a)  --> SCANF  -1 -1 a
	PRINTF,//������� printf(a)    PRINTF  a -1 -1
	RET,//����ֵ     return (a)--> RET a -1 -1    return--> RET -1 -1 -1
	ConstInt,//������������   const int a=5;   ConstInt  1 5 a
	ConstChar,//�ַ���������    const char a='c';  ConstChar 1 'c' a
	GlobVarInt,//ȫ�����α���	int a ;   GlobVarInt 1 -1 a;
	GlobVarChar,//ȫ���ַ�����	char a[20]; GlobVarChar 20 -1 a;
	LocalVarInt,// ���α�������    int a ;   LocalVarInt 1 -1 a;
	LocalVarChar,//�ַ���������	char a[20]; LocalVarChar 20 -1 a; 
	FuncDefBegin,//�������忪ʼ    int f();    FuncDefBegin -1/0/1 n f  -1/0/1: -1��void�� 0:int�� 1��char��  n:��������
	FuncDefEnd,//�����������		             FuncDefEnd    -1 -1 f
	MainBegin, //��������ʼ					 MainBegin -1 -1 -1
	MainEnd,   //����������					 MainEnd  -1 -1 -1
	MOVE,		//��ֵ(ֻ�����Ż������Ԫʽ�г���) b=a MOVE a -1 b
};
const char OperatorStr[][15] =
{
	"Add", "Sub", "Multi", "Div",
	"IntSet",//���α�����ֵ  a[1]=b --> IntSet  b 1 a    a=b --> IntSet b 0 a
	"IntGet",//���α���ȡֵ  b=a[1] --> IntGet  a 1 b    a=b --> IntGet b 0 a
	"CharSet",//�ַ�������ֵ a[1]=b --> CharSet  b 1 a    a=b --> CharSet b 0 a
	"CharGet",//�ַ�����ȡֵ  b=a[1] --> CharGet  a 1 b    a=b --> CharGet b 0 a
	"Call",//��������  f(); -->  Call f -1/0/1 ����ֵ��� -1��void�� 0:int�� 1��char��
	"LdIm",//����������   3 --> LdIm 3  -1 generateInterValue
	"GenLab",//���ɱ�ǩ  ��ǰλ�����ɱ�ǩ GenLab  -1 -1 lab
	"BGR",//��������ת if(t1>t2)  -->  BGR t1 t2 lab
	"BNE",//��������ת if(t1!=t2) -->  BNE t1 t2 lab
	"BLS",//С������ת
	"BLE",//С�ڵ�������ת
	"BGE",//���ڵ�������ת
	"BEQ",//��������ת
	"GOTO",//��������ת GOTO -1 -1 lab
	"PARA",//���ݲ���  ����Ϊfno�ĺ������ݲ���t : PARA t fno -1
	"SCANF",//�������� scanf(a)  --> SCANF  -1 -1 a
	"PRINTF",//������� printf(a)    PRINTF  -1 -1 a
	"RET",//����ֵ     return (a)--> RET -1 -1 a    return--> RET -1 -1 -1 
	"ConstInt",//������������   const int a=5;   ConstInt  1 5 a
	"ConstChar",//�ַ���������    const char a='c';  ConstChar 1 'c' a
	"GlobVarInt",//ȫ�����α���	int a ;   GlobVarInt 1 -1 a;
	"GlobVarChar",//ȫ���ַ�����	char a[20]; GlobVarChar 20 -1 a;
	"LocalVarInt",// ���α�������    int a ;   LocalVarInt 1 -1 a;
	"LocalVarChar",//�ַ���������	char a[20]; LocalVarChar 20 -1 a; 
	"FuncDefBegin",//�������忪ʼ    int f();    FuncDefBegin -1/0/1 n f  -1/0/1 ����ֵ��� -1��void�� 0:int�� 1��char��  n:��������
	"FuncDefEnd",//�����������		             FuncDefEnd    -1 -1 f
	"MainBegin", //��������ʼ
	"MainEnd",//����������
	"MOVE"
};

typedef struct
{
	Operator op;//�����
	//char slopr[VARLEN];//�������
	//char sropr[VARLEN];//�Ҳ�����
	//char dopr[VARLEN];//Ŀ�Ĳ�����
	int lopr;
	int ropr;
	int dopr;
	char *str;
	//bool lisCon, riscon;
	int blockNo;
	bool valid;
}InterCode;
extern InterCode interCode[MAX_CODE_LEN];
extern int interCodeIndex;
int genLabNo();
int genInterCode(Operator op, int lop = -1, int rop = -1, int dop = -1/*,bool lisCon=false,bool risCon=false*/);
void genInterCode(Operator op, char* str,int dop);

void printInterCode(char* file, InterCode inter[], int length);
void modifyFuncDefBegin(int paraNum, int fno);
bool isJump(Operator op);
bool isMemAlloc(Operator op);//���ߺ������жϵ�ǰ��Ԫʽ������Ƿ������ݶε��ڴ����ָ��
bool isBlockBegin(Operator op);
bool isIO(Operator op);
bool isArithmetic(Operator op);
bool isSetGet(Operator op);
#endif