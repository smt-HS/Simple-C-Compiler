#define _CRT_SECURE_NO_WARNINGS
#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "InterCode.h"
#include "mips.h"
#include "Optimize.h"
#include <cstdlib>
#include <cstdio>

extern char token[LEN];//��ǰ�����ĵ���
extern Symbol symbol;//��������
extern FILE *ifp, *ofp;//��������ļ�ָ��

void init()
{
	char s[100];
	ifp=ofp = NULL;

	printf("������Դ�ļ��ļ�����\n");
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