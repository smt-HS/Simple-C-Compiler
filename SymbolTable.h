#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "LexicalAnalysis.h"
#define NSYMTAB 1000
#define ARGLEN 20//参数列表最大长度
enum Type//符号表中的类型
{
	IntVar,CharVar,IntConst,CharConst,IntFunc,CharFunc,VoidFunc
};
enum Arg//参数类型
{
	Int,Char
};
struct SymTab
{
	char name[LEN];
	Type type;//整形变量、字符变量、整形函数......
	int dim_argLen;
	//if type=IntVar or CharVar,dim=1 means it's an array
	//if type=(Char|Int|Void)Func, dim is the length of the function's args
	union Value
	{
		int constValue;
		int length;//数组长度
		Arg argList[ARGLEN];
	}value;
	int no;//四元式变量编号，初始化为-1，调用或引用时才进行编号
};
int enter(char* n, Type t, int d, int v[]);
void enterFunction();
void exitFunction();
bool lookupSymbolTab(char * name,SymTab &tmp);//在符号表中查找name，成功返回true，失败返回false
void modifyArg(int, int, int[]);
int generateInterVar();//生成四元式中间变量编号
#endif