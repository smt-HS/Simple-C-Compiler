#ifndef INTER_CODE_H
#define INTER_CODE_H
#include <string>
#include <iostream>
#include "LexicalAnalysis.h"

#define MAX_CODE_LEN 5000
using namespace std;
/*
四元式操作符分类
算术运算：（可能发生栈区内存分配）
	Add,Sub,Multi,Div
访存取值:（可能发生栈区内存分配）
	IntGet CharGet
访存赋值：
	IntSet CharSet
I/O:
	PRINTF SCANF
内存分配：
	数据区：	//可以与其他四元式交换顺序
		ConstInt ConstChar GlobVarInt GlobVarChar LdIm
	栈：		LocalVarInt LocalVarChar PARA
跳转：//基本块结束标志
	无条件：Call RET GOTO FuncDefEnd  MainEnd
	有条件：BGR BNE BLS BLE BGE BEQ
标签生成://基本块开始标志
	GenLab MainBegin FuncDefBegin

*/
enum Operator
{
	Add, Sub, Multi, Div,
	IntSet,//整形变量赋值  a[1]=b --> IntSet  b 1 a    a=b --> IntSet b 0 a
	IntGet,//整形变量取值  b=a[1] --> IntGet  a 1 b    a=b --> IntGet b 0 a
	CharSet,//字符变量赋值 a[1]=b --> CharSet  b 1 a    a=b --> CharSet b 0 a
	CharGet,//字符变量取值  b=a[1] --> CharGet  a 1 b    a=b --> CharGet b 0 a
	Call,////函数调用  f(); -->  Call f -1/0/1 返回值编号 -1：void型 0:int型 1：char型
	LdIm,//载入立即数   3 --> LdIm 3  0/1 generateInterValue 0 为整数 1为字符
	GenLab,//生成标签  当前位置生成标签 GenLab  -1 -1 lab
	BGR,//大于则跳转 if(t1>t2)  -->  BGR t1 t2 lab
	BNE,//不等则跳转 if(t1!=t2) -->  BNE t1 t2 lab
	BLS,//小于则跳转
	BLE,//小于等于则跳转
	BGE,//大于等于则跳转
	BEQ,//等于则跳转
	GOTO,//无条件跳转 GOTO -1 -1 lab
	PARA,//传递参数  向编号为fno的函数传递参数t : PARA t fno -1
	SCANF,//读入数据 scanf(a)  --> SCANF  -1 -1 a
	PRINTF,//输出数据 printf(a)    PRINTF  a -1 -1
	RET,//返回值     return (a)--> RET a -1 -1    return--> RET -1 -1 -1
	ConstInt,//整数常量定义   const int a=5;   ConstInt  1 5 a
	ConstChar,//字符常量定义    const char a='c';  ConstChar 1 'c' a
	GlobVarInt,//全局整形变量	int a ;   GlobVarInt 1 -1 a;
	GlobVarChar,//全局字符变量	char a[20]; GlobVarChar 20 -1 a;
	LocalVarInt,// 整形变量定义    int a ;   LocalVarInt 1 -1 a;
	LocalVarChar,//字符变量定义	char a[20]; LocalVarChar 20 -1 a; 
	FuncDefBegin,//函数定义开始    int f();    FuncDefBegin -1/0/1 n f  -1/0/1: -1：void型 0:int型 1：char型  n:参数个数
	FuncDefEnd,//函数定义结束		             FuncDefEnd    -1 -1 f
	MainBegin, //主函数开始					 MainBegin -1 -1 -1
	MainEnd,   //主函数结束					 MainEnd  -1 -1 -1
	MOVE,		//赋值(只会在优化后的四元式中出现) b=a MOVE a -1 b
};
const char OperatorStr[][15] =
{
	"Add", "Sub", "Multi", "Div",
	"IntSet",//整形变量赋值  a[1]=b --> IntSet  b 1 a    a=b --> IntSet b 0 a
	"IntGet",//整形变量取值  b=a[1] --> IntGet  a 1 b    a=b --> IntGet b 0 a
	"CharSet",//字符变量赋值 a[1]=b --> CharSet  b 1 a    a=b --> CharSet b 0 a
	"CharGet",//字符变量取值  b=a[1] --> CharGet  a 1 b    a=b --> CharGet b 0 a
	"Call",//函数调用  f(); -->  Call f -1/0/1 返回值编号 -1：void型 0:int型 1：char型
	"LdIm",//载入立即数   3 --> LdIm 3  -1 generateInterValue
	"GenLab",//生成标签  当前位置生成标签 GenLab  -1 -1 lab
	"BGR",//大于则跳转 if(t1>t2)  -->  BGR t1 t2 lab
	"BNE",//不等则跳转 if(t1!=t2) -->  BNE t1 t2 lab
	"BLS",//小于则跳转
	"BLE",//小于等于则跳转
	"BGE",//大于等于则跳转
	"BEQ",//等于则跳转
	"GOTO",//无条件跳转 GOTO -1 -1 lab
	"PARA",//传递参数  向编号为fno的函数传递参数t : PARA t fno -1
	"SCANF",//读入数据 scanf(a)  --> SCANF  -1 -1 a
	"PRINTF",//输出数据 printf(a)    PRINTF  -1 -1 a
	"RET",//返回值     return (a)--> RET -1 -1 a    return--> RET -1 -1 -1 
	"ConstInt",//整数常量定义   const int a=5;   ConstInt  1 5 a
	"ConstChar",//字符常量定义    const char a='c';  ConstChar 1 'c' a
	"GlobVarInt",//全局整形变量	int a ;   GlobVarInt 1 -1 a;
	"GlobVarChar",//全局字符变量	char a[20]; GlobVarChar 20 -1 a;
	"LocalVarInt",// 整形变量定义    int a ;   LocalVarInt 1 -1 a;
	"LocalVarChar",//字符变量定义	char a[20]; LocalVarChar 20 -1 a; 
	"FuncDefBegin",//函数定义开始    int f();    FuncDefBegin -1/0/1 n f  -1/0/1 返回值编号 -1：void型 0:int型 1：char型  n:参数个数
	"FuncDefEnd",//函数定义结束		             FuncDefEnd    -1 -1 f
	"MainBegin", //主函数开始
	"MainEnd",//主函数结束
	"MOVE"
};

typedef struct
{
	Operator op;//运算符
	//char slopr[VARLEN];//左操作数
	//char sropr[VARLEN];//右操作数
	//char dopr[VARLEN];//目的操作数
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
bool isMemAlloc(Operator op);//工具函数，判断当前四元式运算符是否是数据段的内存分配指令
bool isBlockBegin(Operator op);
bool isIO(Operator op);
bool isArithmetic(Operator op);
bool isSetGet(Operator op);
#endif