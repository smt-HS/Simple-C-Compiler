#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "LexicalAnalysis.h"
#define NSYMTAB 1000
#define ARGLEN 20//�����б���󳤶�
enum Type//���ű��е�����
{
	IntVar,CharVar,IntConst,CharConst,IntFunc,CharFunc,VoidFunc
};
enum Arg//��������
{
	Int,Char
};
struct SymTab
{
	char name[LEN];
	Type type;//���α������ַ����������κ���......
	int dim_argLen;
	//if type=IntVar or CharVar,dim=1 means it's an array
	//if type=(Char|Int|Void)Func, dim is the length of the function's args
	union Value
	{
		int constValue;
		int length;//���鳤��
		Arg argList[ARGLEN];
	}value;
	int no;//��Ԫʽ������ţ���ʼ��Ϊ-1�����û�����ʱ�Ž��б��
};
int enter(char* n, Type t, int d, int v[]);
void enterFunction();
void exitFunction();
bool lookupSymbolTab(char * name,SymTab &tmp);//�ڷ��ű��в���name���ɹ�����true��ʧ�ܷ���false
void modifyArg(int, int, int[]);
int generateInterVar();//������Ԫʽ�м�������
#endif