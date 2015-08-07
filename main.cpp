#define _CRT_SECURE_NO_WARNINGS
#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "InterCode.h"
#include "mips.h"
#include "Optimize.h"
#include <cstdlib>
#include <cstdio>

extern char token[LEN];//当前读到的单词
extern Symbol symbol;//单词种类
extern FILE *ifp, *ofp;//输入输出文件指针

void init()
{
	char s[100];
	ifp=ofp = NULL;

	printf("请输入源文件文件名：\n");
	gets(s);
	ifp = fopen(s, "r");
	//ofp = fopen("12211053_token.txt", "w");
}
void clear()
{
	if (ifp!=NULL)
		fclose(ifp);
	if (ofp != NULL)
		fclose(ofp);
}
int main()
{
	init();
	program();
	clear();
	/*divideBlock();
	printInterCode("InterCode.txt", interCode, interCodeIndex);
	dagOptimize();
	printInterCode("InterCode-opt.txt", optInterCode,optInterCodeIdx);
	generateMIPS();*/
	generateMIPS();
	printNodeTable();
	system("pause");
	return 0;
}