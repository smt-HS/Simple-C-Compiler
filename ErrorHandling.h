#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H
#include <string>
#include <iostream>
#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
using namespace std;

extern int totalError;
int error(int no,int subNo=0);
void jumpToRbrace();
const string errorInfo[] =
{
	"The word is too long",//"���ʳ��ȳ���",//0
	"Unknown Operator",//"δ֪�����",//1
	"' is missing",//"ȱ��'",//2
	"Unknown statement",//"δ֪���",//3
	"Illegal char in string",//"�ַ����г��ַǷ��ַ�",//4
	"} is missing",//"ȱ��}",//5
	"Incomplete program",//"��������",//6
	"Constant is not initialized",//"����δ��ʼ��",//7
	"constant is not initialized with right type",//"������ʼ�����ʹ���",//8
	"Illegal customized IDEN",//"�Ƿ����Զ����ʶ��",//9
	", is missing",//"ȱ��','",//10
	"Unknown data type",//"δ֪��������",//11
	"The first digit of a non-zero number can not be 0",//"����������һ�����ֲ���Ϊ0",//12
	"Illegal initialization of array length",//"δ��ȷ��ʼ�����鳤��",//13
	"] is missing",//"ȱ��']'",//14
	"Type identifier is missing",//"ȱ�����ͱ�ʶ��",//15
	") is missing",//"ȱ�� ')' ",//16
	"{ is missing",//"ȱ��'{'",//17
	"Illegal varible definition",//"�Ƿ��ı�������",//18
	"Illegal word in parameter list",//"�����б��д��ڷǷ�����",//19
	"( is missing",//"ȱ��'('",//20
	"Undefined identifier",//"��ʶ��δ����",//21
	"Illegal Operation on non-array varible",//"��ͼ�Է�������������������",//22
	"void function can not be called here",//"�˴����ܵ���û�з���ֵ�ĺ���",//23
	"Illegal char constant",//"�ַ�����Ϊ�Ƿ��ַ�",//24
	"Left value can not be changed",//"��ֵ�����ֵ�����޸�",//25
	"[ is missing",//"ȱ��'['",//26
	"= is missing",//"ȱ��'='",//27
	"The identifier can not accept input",//"�ñ���/�����޷���������",//28
	"Unknown part of output",//"����������޷�ʶ��",//29
	"; is missing ",//"ȱ��';'",//30
	"constant is needed after case",//"case �����Ϊ����",//31
	": is missing",//"ȱ��ð��",//32
	"Wrong parameter list",//"�β�ʵ�β�����",33
	"return value error",//"����ȱ�ٷ���ֵ",34;
	"unknown part of Program",//"�����д����޷�ʶ��Ĳ��֣���main��������֮���д��룩"35
	"Function/Varible redefined",//"����/�����ض���"//36
	"index out of range",//�����±�Խ��//37
}; 
#endif // !ERROR_HANDLING_H
