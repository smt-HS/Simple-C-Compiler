#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H

#include <iostream>

#define LEN 100//������󳤶�
#define ReserveWordsNum 13
enum Symbol {
	CONSTTK,	//const
	INTTK,		//int
	CHARTK,		//char
	VOIDTK,		//void
	MAINTK,		//main
	IFTK,		//if
	ELSETK,		//else
	WHILETK,	//while
	SWITCHTK,	//switch
	CASETK,		//case
	SCANFTK,	//scanf
	PRINTFTK,	//printf
	RETURNTK,	//return
	IDEN,		//��ʶ��
	INTCON,		//���γ���
	CHARCON,	//�ַ�����
	STRCON,		//�ַ���
	PLUS,		//+
	MINU,		//-
	MULT,		//*
	DIV,		// /
	LSS,		//<
	LEQ,		//<=
	GRE,		//>
	GEQ,		//>=
	EQL,		//==
	NQL,		//!=
	ASSIGN,		//=
	SEMICN,		//;
	COMMA,		//,
	COLON,		//:
	QMARK,		//'
	DMARK,		//"
	LPARENT,	//(
	RPARENT,	//)
	LBRACK,		//[
	RBRACK,		//]
	LBRACE,		//{
	RBRACE		//}
};
const char reserveWords[ReserveWordsNum][LEN] =
{
	"const",
	"int",
	"char",
	"void",
	"main",
	"if",
	"else",
	"while",
	"switch",
	"case",
	"scanf",
	"printf",
	"return"
};
const char symbolWords[][LEN] =
{
	"CONSTTK",	//const
	"INTTK",		//int
	"CHARTK",		//char
	"VOIDTK",		//void
	"MAINTK",		//main
	"IFTK",		//if
	"ELSETK",		//else
	"WHILETK",	//while
	"SWITCHTK",	//switch
	"CASETK",		//case
	"SCANFTK",	//scanf
	"PRINTFTK",	//printf
	"RETURNTK",	//return
	"IDEN",		//��ʶ��
	"INTCON",		//���γ���
	"CHARCON",	//�ַ�����
	"STRCON",		//�ַ���
	"PLUS",		//+
	"MINU",		//-
	"MULT",		//*
	"DIV",		// /
	"LSS",		//<
	"LEQ",		//<=
	"GRE",		//>
	"GEQ",		//>=
	"EQL",		//==
	"NQL",		//!=
	"ASSIGN",		//=
	"SEMICN",		//;
	"COMMA",		//,
	"COLON",		//:
	"QMARK",		//'
	"DMARK",		//"
	"LPARENT",	//(
	"RPARENT",	//)
	"LBRACK",		//[
	"RBRACK",		//]
	"LBRACE",		//{
	"RBRACE",		//}
};
extern char token[LEN];//��ǰ�����ĵ���
extern Symbol symbol;//��������
extern FILE *ifp, *ofp;//��������ļ�ָ��
extern int line;//��ǰ�к�
int getSym(bool OpPrior=false);//�����һ����ʶ��,OpPrior:�����з������������ȶ�������
int getch();//�����һ�ַ�
void retract();
int isDigit();
int isLetter();
int isEOF();
extern char ch;//��ǰ�ַ�
#endif // !LEXICAL_ANALYSIS_H
