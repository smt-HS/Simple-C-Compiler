#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H

#include <iostream>

#define LEN 100//单词最大长度
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
	IDEN,		//标识符
	INTCON,		//整形常量
	CHARCON,	//字符常量
	STRCON,		//字符串
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
	"IDEN",		//标识符
	"INTCON",		//整形常量
	"CHARCON",	//字符常量
	"STRCON",		//字符串
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
extern char token[LEN];//当前读到的单词
extern Symbol symbol;//单词种类
extern FILE *ifp, *ofp;//输入输出文件指针
extern int line;//当前行号
int getSym(bool OpPrior=false);//获得下一个标识符,OpPrior:遇到有符号整数，优先读出符号
int getch();//获得下一字符
void retract();
int isDigit();
int isLetter();
int isEOF();
extern char ch;//当前字符
#endif // !LEXICAL_ANALYSIS_H
