#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "InterCode.h"
using namespace std;
InterCode interCode[MAX_CODE_LEN];
int interCodeIndex = 0;
int labIndex = 1;
int genInterCode(Operator op, int lop , int rop, int dop)
{
	interCode[interCodeIndex].op = op;
	interCode[interCodeIndex].lopr = lop;
	interCode[interCodeIndex].ropr = rop;
	interCode[interCodeIndex].dopr = dop;
	/*interCode[interCodeIndex].lisCon = lisCon;
	interCode[interCodeIndex].riscon = risCon;*/
	interCode[interCodeIndex].str = NULL;
	interCode[interCodeIndex].blockNo = -1;
	interCode[interCodeIndex].valid = true;
	interCodeIndex++;
	return dop;
}
void modifyFuncDefBegin(int paraNum,int fno)
{
	for (int i = interCodeIndex - 1; i >= 0; i--)
	{
		if (interCode[i].op == FuncDefBegin
			&&interCode[i].dopr == fno)
		{
			interCode[i].ropr = paraNum;
			return;
		}
	}
}
void genInterCode(Operator op, char* str, int dop)
{
	interCode[interCodeIndex].op = op;
	interCode[interCodeIndex].str = new char[LEN];
	sprintf(interCode[interCodeIndex].str, "%s", str);
	interCode[interCodeIndex].lopr = dop;
	interCode[interCodeIndex].valid = true;
	interCodeIndex++;
	return;
}
int genLabNo()
{
	return labIndex++;

}

void printInterCode(char* file,InterCode inter[],int length)
{
	cerr << "正在输出四元式到" <<file<< endl;
	InterCode tmp;
	ofstream out(file, ios::out);
	for (int i = 0; i < length; i++)
	{
		if (!inter[i].valid)
			continue;
		tmp = inter[i];
		out << setw(15)<<left<<OperatorStr[tmp.op]<<"\t"<<right
			<< setw(4) << tmp.lopr << "\t" 
			<< setw(4) << tmp.ropr << "\t"
			<< setw(4) << tmp.dopr << "\t"
			<< setw(4) << tmp.blockNo << endl;
	}
	out.close();
	cerr << "四元式已输出到"<<file << endl;
	return;
}
bool isJump(Operator op)//工具函数，判断当前四元式运算符是否为跳转类运算符
{
	return (op == Call || op == GOTO || op == RET ||
		op == FuncDefEnd || op == MainEnd ||
		op == BGR || op == BNE || op == BLS ||
		op == BLE || op == BGE || op == BEQ);
}
bool isMemAlloc(Operator op)//工具函数，判断当前四元式运算符是否是内存分配指令
{
	return (op == ConstInt||op==ConstChar||
			op==GlobVarInt||op==GlobVarChar||
			op==LdIm||op==LocalVarInt||op==LocalVarChar);
}
bool isBlockBegin(Operator op)
{
	return (op == GenLab || op == MainBegin || op == FuncDefBegin);
}
bool isIO(Operator op)
{
	return (op == PRINTF || op == SCANF);
}
bool isArithmetic(Operator op)
{
	return (op == Add || op == Sub || op == Multi || op == Div);
}
bool isSetGet(Operator op)
{
	return (op == IntGet || op == IntSet || op == CharSet || op == CharGet);
}