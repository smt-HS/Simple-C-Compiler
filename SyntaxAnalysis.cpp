#define _CRT_SECURE_NO_WARNINGS
#include "SyntaxAnalysis.h"
#include "LexicalAnalysis.h"
#include "ErrorHandling.h"
#include "InterCode.h"
#include "SymbolTable.h"
#include <vector>

int arg[ARGLEN];//函数的参数表、常量值
char name[LEN];//当前读到的自定义变量名/函数名
int argLen;//函数参数表长度
SymTab tmpTab;//查找符号表的返回结果
bool needReturnValue; bool returnValueExists;
//bool wholeArrayAllowed=false;//允许返回整个数组
bool isTypeSymbol(Type &t)//判断当前读到的symbol是不是类型标识符char和int
{
	if (symbol == INTTK || symbol == CHARTK)
	{
		if (symbol == INTTK)
			t = IntVar;
		else
			t = CharVar;
		return true;
	}
	return false;
}
Type Symbol2Type()
{
	if (symbol == INTTK)
		return IntVar;
	else if (symbol == CHARTK)
		return CharVar;
}
bool isFuncSymbol(Type &t)//判断当前token中的单词是否是函数返回值标志(int,char,void)
{
	if (symbol == INTTK)
		t = IntFunc;
	else if (symbol == CHARTK)
		t = CharFunc;
	else if (symbol == VOIDTK)
		t = VoidFunc;
	else
		return false;
	return true;
}
bool isArgPara(Arg &a)//判断当前token中的单词是否是参数表中允许的类型
{
	if (symbol == INTTK || symbol == CHARTK)
	{
		if (symbol == INTTK)
			a = Int;
		else
			a = Char;
		return true;
	}
	return false;
}
bool isUnsignedInt()//判断是否是无符号整数
{
	return ((token[0] != '+') &&(token[0] = '-')) ;
}
bool isRelationOperator(Operator &op)//判断当前symbol是否是关系运算符
{
	switch (symbol)
	{
	case LSS:op = BLS;
		break;
	case LEQ:op = BLE;
		break; 
	case GRE:op = BGR;
		break;
	case GEQ:op = BGE;
		break;
	case EQL:op = BEQ;
		break;
	case NQL:op = BNE;
		break;
	default:
		return false;
	}
	return true;

}
void getNextLegalSym(bool EOFForbid,bool OpPrior)
{
	int r;
	while ((r = getSym(OpPrior)) != 0)
	{
		if (r == -1 && EOFForbid)
		{
			error(6);
		}
	}
}
int constantDefine()//常量定义;    进入时尚未读入任何单词，退出时已读入后续第一个单词（即';'）
{
	/*＜常量定义＞::= int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
                   |char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}*/
	getNextLegalSym();
	if (symbol == INTTK)
	{
		do
		{
			getNextLegalSym();
			if (symbol != IDEN)
			{
				error(9);//跳读到分号
				return -1;
			}
				
			strcpy(name, token);
			getNextLegalSym();
			if (symbol != ASSIGN)
			{
				error(7);//跳读到分号
				return -1;
			}
				
			getNextLegalSym();
			if (symbol != INTCON)
			{
				error(8);//跳读到分号
				return -1;
			}
				
			arg[0] = atoi(token);
			enter(name, IntConst, 0, arg);
			lookupSymbolTab(name, tmpTab);
			genInterCode(ConstInt, 1, arg[0], tmpTab.no/*,true,true*/);
			getNextLegalSym();
			if (symbol == SEMICN)
				break;
			else if (symbol != COMMA)
			{
				error(10);//跳读到分号
				return -1;
			}
				
		} while (true);
	}
	else if (symbol == CHARTK)
	{
		do
		{
			getNextLegalSym();
			if (symbol != IDEN)
			{
				error(9);//跳读到分号
				return -1;
			}
				
			strcpy(name, token);
			getNextLegalSym();
			if (symbol != ASSIGN)
			{
				error(7);//跳读到分号
				return -1;
			}
				
			getNextLegalSym();;
			if (symbol != CHARCON)
			{
				error(8);//跳读到分号
				return -1;
			}
				
			arg[0] = (int)(token[0]);
			//cerr <<arg[0]<< token[0] <<endl;
			enter(name, CharConst, 0, arg);
			lookupSymbolTab(name, tmpTab);
			genInterCode(ConstChar, 1, arg[0], tmpTab.no/*, true, true*/);
			getNextLegalSym();
			if (symbol == SEMICN)
				break;
			else if (symbol != COMMA)
			{
				error(10);//跳读到分号
				return -1;
			}
				
		} while (true);
	}
	else
	{
		error(11);//跳读到分号
		return -1;
	}
	return 0;
}
int constantDeclare()//常量说明，读入第一个const以后进入，返回时已读入下一语句的第一个单词
{
	while (symbol == CONSTTK)
	{
		constantDefine();//退出时已读入分号,不管是否出错
		getNextLegalSym();
	}
	return 0;
}
int varibleDefine(Type t,bool isGlobal=false)//变量定义，进入时已读入第一个变量之后的逗号或者左中括号,返回时读到分号
{
	//unsigned int num;
	if (symbol != LBRACK)//不是数组
	{
		enter(name, t, 0, arg);
		lookupSymbolTab(name, tmpTab);
		if (t == IntVar)
		{
			if (isGlobal)
				genInterCode(GlobVarInt, 1, -1, tmpTab.no/*, true*/);
			else
				genInterCode(LocalVarInt, 1, -1, tmpTab.no/*, true*/);
		}
		else if (t==CharVar)
		{
			if (isGlobal)
				genInterCode(GlobVarChar, 1, -1, tmpTab.no/*, true*/);
			else
				genInterCode(LocalVarChar, 1, -1, tmpTab.no/*, true*/);
		}
	}
	else
	{
		getNextLegalSym();
		if (symbol != INTCON&&!isUnsignedInt())
		{
			error(13);//跳读到分号
			return -1;
		}
		else
		{
			arg[0] = atoi(token);
			getNextLegalSym();
			if (symbol != RBRACK)
			{
				error(14);//跳读到分号
				return -1;
			}
			else
			{
				enter(name, t, 1, arg);
				lookupSymbolTab(name, tmpTab);
				if (t == IntVar)
				{
					if (isGlobal)
						genInterCode(GlobVarInt, arg[0], -1, tmpTab.no/*, true*/);
					else
						genInterCode(LocalVarInt, arg[0], -1, tmpTab.no/*, true*/);
				}
				else if (t == CharVar)
				{
					if (isGlobal)
						genInterCode(GlobVarChar, arg[0], -1, tmpTab.no/*, true*/);
					else
						genInterCode(LocalVarChar, arg[0], -1, tmpTab.no/*, true*/);
				}
			}
			
		}
		getNextLegalSym();
	}
	
	while (true)
	{
		if (symbol == SEMICN)
			break;
		else if (symbol != COMMA)
		{
			error(10);//跳读到分号
			return -1;
		}
		getNextLegalSym();
		if (symbol != IDEN)
		{
			error(9);
			return -1;
		}
		strcpy(name, token);
		getNextLegalSym();
		if (symbol != LBRACK)
		{
			enter(name, t, 0, arg);
			lookupSymbolTab(name, tmpTab);
			if (t == IntVar)
			{
				if (isGlobal)
					genInterCode(GlobVarInt, 1, -1, tmpTab.no/*, true*/);
				else
					genInterCode(LocalVarInt, 1, -1, tmpTab.no/*, true*/);
			}
			else if (t == CharVar)
			{
				if (isGlobal)
					genInterCode(GlobVarChar, 1, -1, tmpTab.no/*, true*/);
				else
					genInterCode(LocalVarChar, 1, -1, tmpTab.no/*, true*/);
			}
		}
		else
		{
			getNextLegalSym();
			if (symbol != INTCON&&!isUnsignedInt())
			{
				error(13);
				return -1;
			}
			arg[0] = atoi(token);
			getNextLegalSym();
			if (symbol != RBRACK)
			{
				error(14);
				return -1;
			}
			else
			{
				enter(name, t, 1, arg);
				lookupSymbolTab(name, tmpTab);
				if (t == IntVar)
				{
					if (isGlobal)
						genInterCode(GlobVarInt, arg[0], -1, tmpTab.no/*, true*/);
					else
						genInterCode(LocalVarInt, arg[0], -1, tmpTab.no/*, true*/);
				}
				else if (t == CharVar)
				{
					if (isGlobal)
						genInterCode(GlobVarChar, arg[0], -1, tmpTab.no/*, true*/);
					else
						genInterCode(LocalVarChar, arg[0], -1, tmpTab.no/*, true*/);
				}
				getNextLegalSym();
			}
		}
	}
	return 0;
}
int varibleDeclare()//变量说明，进入时已读入第一个单词（int/char），返回时已读入下一语句的第一个单词
{//＜变量说明＞::= ＜变量定义＞;{＜变量定义＞;}
 //＜变量定义＞::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’ )}
	Type t;
	while (isTypeSymbol(t))
	{
		getNextLegalSym();
		if (symbol != IDEN)
		{
			error(9,3);//跳读到statement开头
			//getNextLegalSym();
			return -1;
		}
		strcpy(name, token);
		getNextLegalSym();
		if (symbol == COMMA || symbol == SEMICN || symbol == LBRACK)
		{
			varibleDefine(t);//返回时读到分号
			getNextLegalSym();
		}
		else
		{
			error(18);
			return -1;
		}
	}
	return 0;
}
int argList(int funcIndex,int fno)//进入时已读入左括号，返回时已读入右括号，并填完符号表
{
	Arg a;
	Type t;
	int index = 0;//参数表长度
	char paraname[LEN];
	getNextLegalSym();
	while (symbol!=RPARENT)
	{

		if (!isArgPara(a))
		{
			error(15);
			return -1;
		}
		t = Symbol2Type();
		arg[index++] = (int)a;
		getNextLegalSym();
		if (symbol != IDEN)
		{
			error(9, 2);//跳读到右括号
			return -1;
		}
		strcpy(paraname, token);
		enter(paraname, t, 0, arg);
		lookupSymbolTab(paraname, tmpTab);
		if (t == IntVar)
		{
			genInterCode(LocalVarInt, 1, -1, tmpTab.no/*, true*/);
		}
		else if (t == CharVar)
		{
			genInterCode(LocalVarChar, 1, -1, tmpTab.no/*, true*/);
		}
		getNextLegalSym();
		if (symbol != RPARENT&&symbol != COMMA)
		{
			error(19);//跳读到右括号
			return -1;
		}
		if (symbol == RPARENT)
			break;
		else if (symbol = COMMA)
			getNextLegalSym();
	}
	if (symbol != RPARENT)
	{
		error(16,2);//跳读到右括号
		return -1;
	}
		
	modifyArg(funcIndex, index, arg);
	modifyFuncDefBegin(index, fno);
	return 0;
}
int callFunc(bool permitVoid)//函数调用,进入时已读入标识符,退出时读完右括号,返回函数返回值所在变量编号,void返回0
{//＜有返回值函数调用语句＞::= ＜标识符＞‘(’＜值参数表＞‘)’
//＜无返回值函数调用语句＞:: = ＜标识符＞‘(’＜值参数表＞‘)’
	if (permitVoid == false && tmpTab.type == VoidFunc)
	{
		error(23);//跳读到分号之后第一个单词
		return -1;
	}
	int fno = tmpTab.no;
	Type type = tmpTab.type;
	int len = tmpTab.dim_argLen;
	int ct = 0;
	getNextLegalSym();
	if (symbol != LPARENT)
	{
		error(20,2);//跳读到分号之后第一个单词
		return -1;
	}
	getNextLegalSym();
	vector <int> paraArray;
	while (true)
	{
		if (symbol == RPARENT)
			break;
		else if (symbol == COMMA)
			getNextLegalSym();
		int r = expression();
		if (r == -1)//出错，已跳读到分号之后第一个单词
			return -1;
		//...
		//genInterCode(PARA, r, fno, -1);
		paraArray.push_back(r);
		ct++;
	}
	if (ct != len)
	{
		error(33);//跳读到分号之后第一个单词
		return -1;
	}
	else
	{
		vector<int>::iterator it;
		for (it = paraArray.begin(); it != paraArray.end(); it++)
		{
			genInterCode(PARA, *it, fno, -1);
		}
		paraArray.clear();
	}
	if (type != VoidFunc)
	{
		int r = generateInterVar();
		int ss;
		if (type == IntFunc)
			ss = 0;
		else
			ss = 1;

		genInterCode(Call, fno, ss, r);
		return r;
	}
	else
	{
		genInterCode(Call, fno, -1, 0);
		return 0;
	}

	
}
void check(int len,int dest)
{
	if (interCode[interCodeIndex - 1].op == LdIm&&
		interCode[interCodeIndex - 1].dopr == dest&&
		interCode[interCodeIndex - 1].lopr >= len)
		error(37);
}
int factor()/*因子,进入时已读入第一个单词,返回时已读入后续的第一个单词,返回最后的中间变量编号*/
{//读入后续单词时必须算符优先  getNextLegalSym(true,true)
	int r;
	if (symbol == LPARENT)//‘(’＜表达式＞‘)’
	{
		getNextLegalSym();
		r=expression();
		if (r == -1)//已跳读到分号之后第一个单词
			return -1;
		if (symbol != RPARENT)
		{
			error(16,3);//读到分号之后第一个单词
			return -1;
		}
		getNextLegalSym(true,true);
		//...
		return r;
	}
	else if (symbol == INTCON)//＜整数＞
	{
		//...
		int num = atoi(token);
		int r = generateInterVar();
		genInterCode(LdIm, num, 0, r/*, true*/);
		getNextLegalSym(true,true);
		return r;
	}
	else if (symbol == CHARCON)//＜字符＞
	{
		//...
		int num = (int)token[0];
		int r = generateInterVar();
		genInterCode(LdIm, num, 1, r/*, true*/);
		getNextLegalSym(true,true);
		return r;
	}
	else if (symbol == IDEN)
	{
		if (!lookupSymbolTab(token,tmpTab))
		{
			error(21);//跳读到分号之后一个单词
			return -1;
		}
		if (tmpTab.type==VoidFunc||
			tmpTab.type == IntFunc ||
			tmpTab.type == CharFunc)
		{
			r=callFunc(false);
			if (r == -1)//出错，已跳读到分号后第一个单词
				return -1;
			getNextLegalSym(true,true);
			return r;
		}
		else if (tmpTab.type == IntVar || tmpTab.type == CharVar||
				tmpTab.type==IntConst||tmpTab.type==CharConst)
		{
			int no = tmpTab.no;
			Type t = tmpTab.type;
			int l = tmpTab.value.length;
			getNextLegalSym(true,true);
			if (symbol == LBRACK)
			{
				if (tmpTab.dim_argLen < 1)
				{
					error(22);//跳读到分号之后一个单词
					//getNextLegalSym();
					return -1;
					//...
				}
				else
				{
					getNextLegalSym();
					int i=expression();
					if (i == -1)//出错，已跳读到分号之后第一个单词
						return -1;
					if (symbol != RBRACK)
					{
						error(14,1);//读到分号之后一个单词
						//getNextLegalSym();
						return -1;
					}
					check(l, i);
					r = generateInterVar();
					if (t == IntVar)
					{
						genInterCode(IntGet, no, i, r);
					}
					else if (t == CharVar)
					{
						genInterCode(CharGet, no, i, r);
					}
					
					getNextLegalSym(true,true);
					return r;
				}
			}
			else
			{
				return no;
			}
		}
	}
	//return 0;
}
int term()//项，进入时已读入第一个单词,返回时已读入后续第一个单词
{//＜项＞::=＜因子＞{＜乘法运算符＞＜因子＞}
	int pre, next;
	Symbol s;
	pre=factor();
	if (pre == -1)//出错，已跳读到分号之后第一个单词
		return -1;
	while (symbol == MULT || symbol == DIV)
	{
		s = symbol;
		getNextLegalSym(true, false);
		next=factor();
		if (next == -1)//出错，已跳读到分号之后第一个单词
			return -1;

		if (s == MULT)
		{
			pre = genInterCode(Multi, pre, next, generateInterVar());
		}
		else
		{
			pre = genInterCode(Div, pre, next, generateInterVar());
		}
	}
	return pre;
}
int expression()//进入时已读入第一个单词,返回时已读入后续第一个单词
{//＜表达式＞::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
//调用前getNextSym(true,false)
	int pre, next;
	Symbol s=PLUS;
	if (symbol == PLUS || symbol == MINU)//说明第一个单词必定不是整数
	{
		s = symbol;
		getNextLegalSym(true,false);//此处第二个参数必须为false
	}
	pre=term();
	if (pre == -1)//出错，已跳读到分号之后第一个单词
		return -1;
	if (s == MINU)
	{
		//int zero = genInterCode(LdIm, 0, 0, generateInterVar());
		pre = genInterCode(Sub, 0, pre, generateInterVar()/*, true*/);
	}
		
	while (symbol == PLUS || symbol == MINU)
	{
		s = symbol;
		getNextLegalSym(true, false);
		next=term();
		if (next == -1)//出错，已跳读到分号之后第一个单词
			return -1;
		if (s == PLUS)
			pre = genInterCode(Add, pre, next, generateInterVar());
		else
		{
			pre = genInterCode(Sub, pre, next, generateInterVar());
		}
	}
	return pre;
}
int condition()//条件,进入时未读入任何单词，返回时已读入后续第一个单词,正常返回lab2,出错返回-1
{//＜条件＞::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
	int r,t;
	int lab1, lab2;
	Operator op;
	getNextLegalSym();
	r=expression();
	if (r == -1)//出错，已跳读到分号之后第一个单词
		return -1;
	lab1 = genLabNo();
	if (isRelationOperator(op))
	{
		getNextLegalSym();
		t=expression();
		if (t == -1)//出错，已跳读到分号之后第一个单词
			return -1;
		genInterCode(op, r, t, lab1);
	}
	else
	{
		//非0则跳转
		//int zero = genInterCode(LdIm, 0, 0, generateInterVar());
		genInterCode(BNE, r, 0, lab1/*, false, true*/);
	}
	lab2 = genLabNo();
	genInterCode(GOTO, -1, -1, lab2);
	genInterCode(GenLab, -1, -1, lab1);
	return lab2;
}
int caseStatement(int expr,int lab1)//进入时已读入case,返回时已读入后续第一个单词,返回lab(i+1，1)
{//＜情况子语句＞  ::=  case＜常量＞：＜语句＞
	getNextLegalSym();
	genInterCode(GenLab, -1, -1, lab1);
	int num;
	if (symbol == INTCON)
		num = atoi(token);
	else if (symbol == CHARCON)
		num = (int)token[0];
	else
	{
		error(31);//跳读到后大括号之后第一个单词
		return -1;
	}
	
	int lab2 = genLabNo();
	int numNo = genInterCode(LdIm, num, 0, generateInterVar());
	genInterCode(BEQ, expr, numNo, lab2/*, false, true*/);
	lab1 = genLabNo();
	genInterCode(GOTO, -1, -1, lab1);
	genInterCode(GenLab, -1, -1, lab2);
	getNextLegalSym();
	if (symbol != COLON)
	{
		error(32);//跳读到后大括号之后第一个单词
		return -1;
	}
	else
		getNextLegalSym();

	int r=statement();
	if (r == -1)//出错
	{
		jumpToRbrace();
		getNextLegalSym();//跳读到后大括号之后第一个单词
		return -1;
	}
	return lab1;
}
int caseList(int expr,int endLab)//进入时已读入case，返回时已读入后续第一个单词
{//＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}

	int lab1= genLabNo();
	while (symbol == CASETK)
	{
		lab1=caseStatement(expr,lab1);
		if (lab1 == -1)//出错，已跳读到大括号之后第一个单词
			return -1;
		genInterCode(GOTO, -1, -1, endLab);
	}
	return lab1;
}
int statement()//语句，进入已读入该条语句第一个单词，返回时已读入后续第一个单词
{
	if (symbol == IFTK)//＜条件语句＞
	{//＜条件语句＞  ::=  if ‘(’＜条件＞‘)’＜语句＞［else＜语句＞］
		getNextLegalSym();
		if (symbol != LPARENT)
		{
			error(20,2);//跳读到分号之后第一个单词
			return -1;
		}
			
		int lab2=condition();
		if (lab2 == -1)//出错，已跳读到分号之后第一个单词
			return -1;
		if (symbol != RPARENT)
		{
			error(16,3);//跳读到分号之后第一个单词
			return -1;
		}
			
		getNextLegalSym();
		int r=statement();
		if (r == -1)//出错，已跳读到合适位置
			return -1;
		if (symbol == ELSETK)
		{
			int lab3 = genLabNo();
			genInterCode(GOTO, -1, -1, lab3);
			genInterCode(GenLab, -1, -1, lab2);
			getNextLegalSym();
			r=statement();
			if (r == -1)//出错，已跳读到合适位置
				return -1;
			genInterCode(GenLab, -1, -1, lab3);
		}
		else//没有else
		{
			genInterCode(GenLab, -1, -1, lab2);
		}
	}
	else if (symbol == WHILETK)//＜循环语句＞
	{//＜循环语句＞::=  while ‘(’＜条件＞‘)’＜语句＞
		int lab2, lab3;
		lab3 = genLabNo();
		genInterCode(GenLab, -1, -1, lab3);
		getNextLegalSym();
		if (symbol != LPARENT)
		{
			error(20,2);//跳读到分号之后第一个单词
			return -1;
		}
			
		lab2=condition();
		if (lab2 == -1)//出错，已跳读到分号之后第一个单词
			return -1;
		if (symbol != RPARENT)
		{
			error(16, 3);//跳读到分号之后第一个单词
			return -1;
		}
			
		getNextLegalSym();
		int r=statement();
		if (r == -1)//出错，已跳读到合适位置
			return -1;
		genInterCode(GOTO, -1, -1, lab3);
		genInterCode(GenLab, -1, -1, lab2);
	}
	else if (symbol == LBRACE)//‘{’＜语句列＞‘}’
	{
		getNextLegalSym();
		statement();
		while (symbol != RBRACE)
		{
			statement();
		}
		getNextLegalSym();
	}
	else if (symbol == IDEN)//＜有返回值函数调用语句＞;|＜无返回值函数调用语句＞;｜＜赋值语句＞;
	{//＜赋值语句＞::=＜标识符＞＝＜表达式＞|
		//			 ＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
		if (!lookupSymbolTab(token,tmpTab))
		{
			error(21);//跳读到分号之后第一个单词
			return -1;
		}
		if (tmpTab.type == VoidFunc || tmpTab.type == CharFunc || tmpTab.type == IntFunc)
		{//＜有返回值函数调用语句＞;|＜无返回值函数调用语句＞;｜＜赋值语句＞;
			int r=callFunc(true);//正常返回读完右括号
			if (r == -1)//出错，跳读到分号之后第一个单词
				return -1;
			getNextLegalSym();
			if (symbol != SEMICN)
				error(30);//不跳读，相当于自动添加分号，该错误单词则进入下一轮statement
			else//正常的话再读下一个单词
			getNextLegalSym();
		}
		else if (tmpTab.type == IntVar || tmpTab.type == CharVar)
		{
			int no = tmpTab.no;
			Type type = tmpTab.type;
			if (tmpTab.dim_argLen > 0)//‘[’＜表达式＞‘]’=＜表达式＞
			{
				int l = tmpTab.value.length;
				getNextLegalSym();
				if (symbol != LBRACK)
				{
					error(26);//跳读到分号之后第一个单词
					return -1;
				}
				getNextLegalSym();
				int r=expression();//出错，已跳读到分号之后第一个单词
				if (r == -1)
					return -1;
				if (symbol != RBRACK)
				{
					error(14, 1);//跳读到分号之后第一个单词
					return -1;
				}
				check(l, r);
				getNextLegalSym();
				if (symbol != ASSIGN)
				{
					error(27);//跳读到分号之后第一个单词
					return -1;
				}
					
				getNextLegalSym();
				int t=expression();//跳读到分号之后第一个单词
				if (t == -1)
					return -1;
				if (type == IntVar)
				{
					genInterCode(IntSet, t, r, no);
				}
				else if (type == CharVar)
				{
					genInterCode(CharSet, t, r, no);
				}
				//genInterCode(ArrSet, t, r, no);
			}
			else//＝＜表达式＞
			{
				getNextLegalSym();
				if (symbol != ASSIGN)
				{
					error(27);//跳读到分号之后第一个单词
					return -1;
				}
					
				getNextLegalSym();
				int r=expression();
				if (r == -1)//跳读到分号之后第一个单词
					return -1;
				//genInterCode(MV, r, -1, no);
				if (type == IntVar)
				{
					//int zero = genInterCode(LdIm, 0, 0, generateInterVar());
					genInterCode(IntSet, r, 0, no/*,false,true*/);
				}
				else if (type == CharVar)
				{
					//int zero = genInterCode(LdIm, 0, 0, generateInterVar());
					genInterCode(CharSet, r, 0, no/*,false,true*/);
				}
			}
			if (symbol != SEMICN)
				error(30);
			else
				getNextLegalSym();
		}
		else
		{
			error(25);//跳读到分号后第一个单词
			return -1;
		}
	}
	else if (symbol == SCANFTK)//＜读语句＞;
		//＜读语句＞::=scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
	{
		getNextLegalSym();
		if (symbol != LPARENT)
		{
			error(20,2);//跳读到分号之后第一个单词
			return -1;
		}
			
		getNextLegalSym();
		if (symbol != IDEN)
		{
			error(9, 5);//跳读到分号之后第一个单词
			return -1;
		}
			
		
		if (!lookupSymbolTab(token,tmpTab))
		{
			error(21);//跳读到分号之后第一个单词
			return -1;
		}
		if (tmpTab.type!=CharVar&&tmpTab.type!=IntVar)
		{
			error(28);//跳读到分号之后第一个单词
			return -1;
		}
		else
		{
			if (tmpTab.dim_argLen != 0&&tmpTab.type!=CharVar)
			{
				error(28);//跳读到分号之后第一个单词
				return -1;
			}
			int no = tmpTab.no;
			genInterCode(SCANF, -1, -1, no);
				
		}
		getNextLegalSym();
		while (true)
		{
			if (symbol == RPARENT)
				break;
			else if (symbol != COMMA)
			{
				error(10,1);//跳读到分号之后第一个单词
				return-1;
			}
			getNextLegalSym();
			if (symbol != IDEN)
			{
				error(9,3);//跳读到statement第一个单词
				return -1;
			}
				
			if (!lookupSymbolTab(token,tmpTab))
			{
				error(21);//跳读到分号之后第一个单词
				return -1;
			}
			if (tmpTab.type != CharVar&&tmpTab.type != IntVar)
			{
				error(28);//跳读到分号之后第一个单词
				return -1;
			}
			else
			{
				if (tmpTab.dim_argLen != 0 && tmpTab.type != CharVar)
				{
					error(28);//跳读到分号之后第一个单词
					return -1;
				}
				int no = tmpTab.no;
				genInterCode(SCANF, -1, -1, no);
			}
			getNextLegalSym();
		}
		getNextLegalSym();
		if (symbol != SEMICN)
			error(30);
		else
		getNextLegalSym();//读入后续语句第一个单词
	}
	else if (symbol == PRINTFTK)//＜写语句＞;
	{/*＜写语句＞::= printf ‘(’ ＜字符串＞,＜表达式＞ ‘)’|
		//			printf ‘(’＜字符串＞ ‘)’ |
			//		printf ‘(’＜表达式＞‘)*/

		getNextLegalSym();
		if (symbol != LPARENT)
		{
			error(20,2);//跳读到分号之后第一个单词
			return -1;
		}
		//wholeArrayAllowed = true;
		getNextLegalSym();
		if (symbol == STRCON)
		{
			genInterCode(PRINTF, token,generateInterVar());
			getNextLegalSym();
			if (symbol == COMMA)
			{
				getNextLegalSym();
				int r=expression();
				if (r == -1)//出错，已跳读到分号之后第一个单词
					return -1;
				if (symbol != RPARENT)
				{
					error(16,3);//出错，已跳读到分号之后第一个单词
					return -1;
				}
				genInterCode(PRINTF, r, -1, -1);
				getNextLegalSym();
			}
			else if (symbol == RPARENT)
			{
				getNextLegalSym();
			}
			else
			{
				error(29);//跳读到分号之后第一个单词
				return -1;
			}
				
		}
		else
		{
			int r=expression();
			if (r == -1)//出错，跳读到分号之后第一个单词
				return -1;
			if (symbol != RPARENT)
			{
				error(16,3);//出错，跳读到分号之后第一个单词
				return -1;
			}
			genInterCode(PRINTF, r, -1, -1);
			getNextLegalSym();
		}

		if (symbol != SEMICN)
			error(30);
		else
		getNextLegalSym();
		//wholeArrayAllowed = false;
	}
	else if (symbol == RETURNTK)//＜返回语句＞;
	{//＜返回语句＞::=return[‘(’＜表达式＞‘)’] 
		getNextLegalSym();
		if (symbol == LPARENT)
		{
			getNextLegalSym();
			int r=expression();
			if (r == -1)//出错，跳读到分号之后第一个单词
				return -1;
			returnValueExists = true;
			if (symbol != RPARENT)
			{
				error(16,3);//跳读到分号之后第一个单词
				return -1;
			}
			genInterCode(RET, r, -1, -1);
			getNextLegalSym();
		}
		else
		{
			genInterCode(RET, -1, -1, -1);
		}
		if (symbol != SEMICN)
			error(30);
		else
			getNextLegalSym();


	}
	else if (symbol == SWITCHTK)//＜情况语句＞
	{//＜情况语句＞::=switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞ ‘}’
		getNextLegalSym();
		if (symbol != LPARENT)
		{
			error(20,3);//跳读到后大括号之后第一个单词
			return -1;
		}
			
		getNextLegalSym();
		int r=expression();
		if (r == -1)//出错，读到分号之后第一个单词
		{
			jumpToRbrace();
			getNextLegalSym();
			return -1;//再次跳读到大括号之后第一个单词
		}
		if (symbol != RPARENT)
		{
			error(16,4);//跳读到大括号之后第一个单词
			return -1;
		}
			
		getNextLegalSym();
		if (symbol != LBRACE)
		{
			error(17,2);//跳读到大括号之后第一个单词
			return -1;
		}
		getNextLegalSym();
		int endLab = genLabNo();
		int t=caseList(r,endLab);
		if (t == -1)//出错，已跳读到大括号之后第一个单词
			return -1;
		if (symbol != RBRACE)
		{
			error(5);
			return -1;
		}
		genInterCode(GenLab, -1, -1, t);
		genInterCode(GenLab, -1, -1, endLab);
		getNextLegalSym();
	}
	else if (symbol == SEMICN)//＜空＞; 
	{
		getNextLegalSym();
	}
	else
	{
		error(3);
	}
	return 0;
}
int compoundStatement(bool needEnter)//复合语句(即函数体)，进入时已读入左大括号,返回时已读入右大括号
{
	if(needEnter)
		enterFunction();

	getNextLegalSym();
	constantDeclare();//返回时读入varibleDeclare或statement第一个单词
	varibleDeclare();//返回时读入statement第一个单词
	//getNextLegalSym();
	while (symbol != RBRACE)
	{
		statement();
	}
	exitFunction();
	return 0;
}

int functionDefine(Type t)//函数定义，进入时已读入左小括号,返回时已读入右大括号
{
	int funcIndex=enter(name, t, 0, arg);
	lookupSymbolTab(name, tmpTab);
	int fno = tmpTab.no;
	int ss;
	if (t == VoidFunc) ss = -1;
	else if (t == IntFunc) ss = 0;
	else if (t == CharFunc) ss = 1;
	genInterCode(FuncDefBegin, ss, -1, fno); needReturnValue = returnValueExists = false;
	if (t == IntFunc || t == CharFunc)
	{
		needReturnValue = true;
		//returnFuncNo = tmpTab.no;
	}
		
	enterFunction();
	argList(funcIndex,fno);//读入右小括号
	getNextLegalSym();
	if (symbol != LBRACE)
	{
		error(17,1);//跳读到右大括号
		return -1;
	}
	compoundStatement(false);
	if (!(needReturnValue&&returnValueExists||!needReturnValue&&!returnValueExists))
	{
		error(34);
		//needReturnValue = false;//恢复
	}
	genInterCode(FuncDefEnd, -1, -1, fno);
	return 0;
}
int program()
//＜程序＞:: = ［＜常量说明＞］［＜变量说明＞］{ ＜有返回值函数定义＞ | ＜无返回值函数定义＞ }＜主函数＞
{
	Type t;
	bool varibleDefinePermit = true;
	getNextLegalSym();
	constantDeclare();//返回时已读入分号后第一个单词
	while (isTypeSymbol(t)&&varibleDefinePermit)
	//用以处理变量声明，同时，若第一个函数说明是有返回值的，也一并处理
	//跳出循环时已读入下一个单词
	{
		getNextLegalSym();
		if (symbol != IDEN)
		{
			error(9,4);//跳读到int,char,void
			//由于还有可能有变量说明，故不跳出
		}
		else
		{
			strcpy(name, token);
			getNextLegalSym();
			if (symbol == COMMA || symbol == SEMICN || symbol == LBRACK)//变量说明
			{
				varibleDefine(t,true);//返回时读到分号
				getNextLegalSym();
			}
			else if (symbol = LPARENT)//函数说明
			{
				varibleDefinePermit = false;
				if (t == IntVar)
					t = IntFunc;
				else
					t = CharFunc;
				functionDefine(t);
				getNextLegalSym();
			}
		}
		
	}
	while (isFuncSymbol(t))//{＜有返回值函数定义＞|＜无返回值函数定义＞}
	{
		getNextLegalSym();
		if (symbol == MAINTK)
		{
			break;
		}
		else if (symbol != IDEN)
		{
			error(9,4);
		}
		else
		{
			strcpy(name, token);
			getNextLegalSym();
			functionDefine(t);
			getNextLegalSym();
		}
		
	}
	//进入主函数，已读入main
	//＜主函数＞::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
	getNextLegalSym();
	genInterCode(MainBegin, -1, -1, -1); returnValueExists = false;
	if (symbol != LPARENT)
	{
		error(20,1);
		return -1;
	}
	getNextLegalSym();
	if (symbol != RPARENT)
	{
		error(16);
	}
	getNextLegalSym();
	if (symbol != LBRACE)
	{
		error(17,1);
	}
	compoundStatement(); if (returnValueExists) error(34);
	if (getSym() != -1)
	{
		error(35);
	}
	genInterCode(MainEnd);
	return 0;
}
