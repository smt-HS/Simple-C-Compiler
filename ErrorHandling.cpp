#include "ErrorHandling.h"
int  totalError = 0;
/*void error(int index,char endFlag)
{
	cerr << "error:\t" <<token<< "\t:" << errorInfo[index] << endl;
	while (ch != endFlag  && ch!=EOF)
		getch();
	//retract();
}*/
void jumpToSemicn()
{
	while (symbol != SEMICN)
		getNextLegalSym();
}
void jumpToStatement()
{
	getNextLegalSym();
	while (true)
	{
		if (symbol == IFTK || symbol == WHILETK || symbol == LBRACE || symbol == IDEN
			|| symbol == SCANFTK || symbol == PRINTFTK || symbol == RETURNTK)
			break;
		getNextLegalSym();
	}
}
void jumpToStatementOrFuncDefine()
{
	while (true)
	{
		if (symbol == IFTK || symbol == WHILETK || symbol == LBRACE || symbol == IDEN
			|| symbol == SCANFTK || symbol == PRINTFTK || symbol == RETURNTK ||
			symbol == VOIDTK || symbol == CHARTK || symbol == INTTK)
			break;
		getNextLegalSym();
	}
}
void jumpToRparent()
{
	int ct=0;
	while (true)
	{
		if (symbol == RPARENT&&ct == 0)
			break;
		if (symbol == LPARENT)
			ct++;
		else if (symbol == RPARENT)
			ct--;
		getNextLegalSym();
	}
		
}
void jumpToFuncSymbol()
{
	while (symbol != INTTK&&symbol != CHARTK&&symbol!=VOIDTK)
		getNextLegalSym();
}
void jumpToRbrace()
{
	int ct = 0;
	while (true)
	{
		if (symbol == RBRACE && ct == 0)
			break;
		if (symbol == LPARENT)
			ct++;
		else if (symbol == RPARENT)
			ct--;
		getNextLegalSym();
	}
		
}
int error(int no,int subNo/*,char* fromFile=__FILE__,int fromLine=__LINE__*/)
{
	totalError++;
	int r=0;//返回值，为0则编译继续进行，1则重新读单词
	//fprintf(stderr, "第%d行:\t%s\t%s\n", line, token, errorInfo[no]);
	cerr << "Line number:" << line << "\t" << token << "\t" << errorInfo[no]<<"\t"<<no /*<<"\t"<<fromFile<<"\t"<<fromLine*/<< endl;;
	switch (no)
	{
	case 0://单词长度超限 
		while (isLetter() || isDigit())
			getch();
		retract();
		r = 0;
		break;
	case 1://未知运算符
		r = 1;
		break;
	case 2://缺少单引号，继续进行
		while (ch != '\'')
		{
			if (isEOF())
				error(6);
			getch();
		}
		r = 0;
		break;
	case 3://未知语句
		jumpToStatement();
		r = 0;
		break;
	case 4://字符串中出现非法字符
		r = 0;
		break;
	case 5://缺少}
		jumpToStatement();
		break;
	case 6://程序不完整
		break;
	case 7://常量未初始化
	case 8://常量初始化类型错误
		jumpToSemicn();
		break;
	case 9://非法的自定义标识符
		switch (subNo)
		{
		case 0:
			jumpToSemicn();
			break;
		case 1:
			jumpToStatementOrFuncDefine();
			break;
		case 2:
			jumpToRparent();
			break;
		case 3:
			jumpToStatement();
			break;
		case 4:
			jumpToFuncSymbol();
			break;
		case 5:
			jumpToSemicn();
			getNextLegalSym();
			break;
		}
		break;
	case 10://缺少','
		switch (subNo)
		{
		case 0:
			jumpToSemicn();
			break;
		case 1:
			jumpToSemicn();
			getNextLegalSym();
			break;
		}
		
		break;
	case 11://未知数据类型
		jumpToSemicn();
		break;
	case 12://非零整数第一个数字不得为0
		r = 0;
		break;
	case 13://未正确初始化数组长度
		jumpToSemicn();
		break;
	case 14://缺少']'
		switch (subNo)
		{
		case 0:
			jumpToSemicn(); break;
		case 1:
			jumpToSemicn();
			getNextLegalSym();
			break;
		}
		break;
	case 15://缺少类型标识符
		jumpToRparent(); break;
	case 16://缺少 ')'
		switch (subNo)
		{
		case 0:
			jumpToRparent(); 
			getNextLegalSym();
			break;
		case 1:
			jumpToStatement(); break;
		case 2:
			jumpToRparent();
			break;
		case 3:
			jumpToSemicn();
			getNextLegalSym();
			break;
		case 4:
			jumpToRbrace();
			getNextLegalSym();
			break;
		}
		break;
	case 17://缺少'{'
		switch (subNo)
		{
		case 0:
			jumpToStatement();
			break;
		case 1:
			jumpToRbrace();
			break;
		case 2: jumpToRbrace();
			getNextLegalSym();
			break;
		}
		break;
	case 18://非法的变量定义
		jumpToStatement();
		break;
	case 19://参数列表中存在非法单词
		jumpToRparent();
		break;
	case 20://缺少'('
		switch (subNo)
		{
		case 0:
			jumpToStatement();
			break;
		case 1:
			jumpToRparent();
			break;
		case 2:
			jumpToSemicn();
			getNextLegalSym();
			break;
		case 3:
			jumpToRbrace();
			getNextLegalSym();
			break;
		}
		break;
	case 21://标识符未定义
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 22://试图对非数组变量进行数组操作
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 23://此处不能调用没有返回值的函数
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 24://字符常量为非法字符
		r = 0;
		break;
	case 25://赋值语句左值不可修改
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 26://缺少'['
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 27://缺少'='
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 28://该变量/常量无法接收输入
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 29://待输出部分无法识别
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 30://缺少';'
		//只报错，不跳读，相当于自动添加上了分号
		break;
	case 31://case 后必须为常量
		jumpToRbrace();
		getNextLegalSym();
		break;
	case 32://缺少冒号
		jumpToRbrace();
		getNextLegalSym();
		break;
	case 33://形参实参不符合
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 34://函数缺少返回值
		break;//只报错
	case 35://程序中存在无法识别的部分（如main函数结束之后还有代码）
		break;//只报错
	case 36://函数/变量重定义
		break;//只报错
	case 37://数组下标越界
		break;//只报错
	}
	
	if (no == 6)
	{
		system("pause");
		exit(-1);
	}
		
	return r;
}