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
	"The word is too long",//"单词长度超限",//0
	"Unknown Operator",//"未知运算符",//1
	"' is missing",//"缺少'",//2
	"Unknown statement",//"未知语句",//3
	"Illegal char in string",//"字符串中出现非法字符",//4
	"} is missing",//"缺少}",//5
	"Incomplete program",//"程序不完整",//6
	"Constant is not initialized",//"常量未初始化",//7
	"constant is not initialized with right type",//"常量初始化类型错误",//8
	"Illegal customized IDEN",//"非法的自定义标识符",//9
	", is missing",//"缺少','",//10
	"Unknown data type",//"未知数据类型",//11
	"The first digit of a non-zero number can not be 0",//"非零整数第一个数字不得为0",//12
	"Illegal initialization of array length",//"未正确初始化数组长度",//13
	"] is missing",//"缺少']'",//14
	"Type identifier is missing",//"缺少类型标识符",//15
	") is missing",//"缺少 ')' ",//16
	"{ is missing",//"缺少'{'",//17
	"Illegal varible definition",//"非法的变量定义",//18
	"Illegal word in parameter list",//"参数列表中存在非法单词",//19
	"( is missing",//"缺少'('",//20
	"Undefined identifier",//"标识符未定义",//21
	"Illegal Operation on non-array varible",//"试图对非数组变量进行数组操作",//22
	"void function can not be called here",//"此处不能调用没有返回值的函数",//23
	"Illegal char constant",//"字符常量为非法字符",//24
	"Left value can not be changed",//"赋值语句左值不可修改",//25
	"[ is missing",//"缺少'['",//26
	"= is missing",//"缺少'='",//27
	"The identifier can not accept input",//"该变量/常量无法接收输入",//28
	"Unknown part of output",//"待输出部分无法识别",//29
	"; is missing ",//"缺少';'",//30
	"constant is needed after case",//"case 后必须为常量",//31
	": is missing",//"缺少冒号",//32
	"Wrong parameter list",//"形参实参不符合",33
	"return value error",//"函数缺少返回值",34;
	"unknown part of Program",//"程序中存在无法识别的部分（如main函数结束之后还有代码）"35
	"Function/Varible redefined",//"函数/变量重定义"//36
	"index out of range",//数组下标越界//37
}; 
#endif // !ERROR_HANDLING_H
