#define _CRT_SECURE_NO_WARNINGS
#include "SymbolTable.h"
#include "LexicalAnalysis.h"
#include "ErrorHandling.h"
#include <stdlib.h>
#include <iostream>
using namespace std; 
SymTab symbolTable[NSYMTAB];
int symTabIndex = 0;
int beforeEnterIntoFunction=symTabIndex;
int no=1;//四元式变量编号
bool lookUpOnInsert(char *name, SymTab &tmp)
{
	for (int i = beforeEnterIntoFunction; i < symTabIndex; i++)
	{
		if (strcmp(symbolTable[i].name, name) == 0)
		{
			if (symbolTable[i].no == -1)
				symbolTable[i].no = no++;
			tmp = symbolTable[i];
			return true;
		}
	}
	return false;
}
bool isFuncType(Type t)
{
	return (t == VoidFunc || t == CharFunc || t == VoidFunc);
}
int enter(char* n,Type t,int d,int v[])
{
	SymTab tmp;
	if (lookUpOnInsert(n, tmp))
	{
		/*if (tmp.type==IntVar||tmp.type==CharVar||tmp.type==IntConst||tmp.type==CharConst)
		error(36);//只报错*/
		if (isFuncType(tmp.type) && isFuncType(t))
		{
			error(36);
		}
		if (!isFuncType(tmp.type) && !isFuncType(t))
		{
			error(36);//只报错
		}

	}
	strcpy(symbolTable[symTabIndex].name, n);
	symbolTable[symTabIndex].type = t;
	symbolTable[symTabIndex].dim_argLen = d;
	if (d == 1)
	{
		symbolTable[symTabIndex].value.length = v[0];
	}
	else if (t == IntConst || t == CharConst)
	{	
		symbolTable[symTabIndex].value.constValue = v[0];
	}
	else if (t == IntFunc || t == CharFunc || t == VoidFunc)
	{
		for (int i = 0; i < symbolTable[symTabIndex].dim_argLen; i++)
			symbolTable[symTabIndex].value.argList[i] = (Arg)v[i];
	}
	symbolTable[symTabIndex].no = -1;
	//cerr << n << "\t 写入符号表！\t类型：" << t << "\t维数/参数表长度:" << d << endl;
	
	return symTabIndex++;
}
void modifyArg(int index, int len, int v[])
{
	symbolTable[index].dim_argLen = len;
	for (int i = 0; i < len;i++)
	{
		symbolTable[index].value.argList[i] = (Arg)v[i];
	}
	return;
}
void enterFunction()
{
	beforeEnterIntoFunction = symTabIndex;
	//cerr << "entering  " << beforeEnterIntoFunction<<endl;
}
void exitFunction()
{
	symTabIndex = beforeEnterIntoFunction;
	//cerr << "exiting" << beforeEnterIntoFunction<< endl;
}
bool lookupSymbolTab(char* name, SymTab &tmp)
{
	for (int i = symTabIndex - 1; i >= 0; i--)
	{
		if (strcmp(symbolTable[i].name, name) == 0)
		{
			if (symbolTable[i].no == -1)
				symbolTable[i].no = no++;
			tmp = symbolTable[i];
			return true;
		}
	}
	return false;
}
int generateInterVar()
{
	return no++;
}