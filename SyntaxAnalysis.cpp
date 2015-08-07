#define _CRT_SECURE_NO_WARNINGS
#include "SyntaxAnalysis.h"
#include "LexicalAnalysis.h"
#include "ErrorHandling.h"
#include "InterCode.h"
#include "SymbolTable.h"
#include <vector>

int arg[ARGLEN];//�����Ĳ���������ֵ
char name[LEN];//��ǰ�������Զ��������/������
int argLen;//������������
SymTab tmpTab;//���ҷ��ű�ķ��ؽ��
bool needReturnValue; bool returnValueExists;
//bool wholeArrayAllowed=false;//��������������
bool isTypeSymbol(Type &t)//�жϵ�ǰ������symbol�ǲ������ͱ�ʶ��char��int
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
bool isFuncSymbol(Type &t)//�жϵ�ǰtoken�еĵ����Ƿ��Ǻ�������ֵ��־(int,char,void)
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
bool isArgPara(Arg &a)//�жϵ�ǰtoken�еĵ����Ƿ��ǲ����������������
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
bool isUnsignedInt()//�ж��Ƿ����޷�������
{
	return ((token[0] != '+') &&(token[0] = '-')) ;
}
bool isRelationOperator(Operator &op)//�жϵ�ǰsymbol�Ƿ��ǹ�ϵ�����
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
int constantDefine()//��������;    ����ʱ��δ�����κε��ʣ��˳�ʱ�Ѷ��������һ�����ʣ���';'��
{
	/*���������壾::= int����ʶ��������������{,����ʶ��������������}
                   |char����ʶ���������ַ���{,����ʶ���������ַ���}*/
	getNextLegalSym();
	if (symbol == INTTK)
	{
		do
		{
			getNextLegalSym();
			if (symbol != IDEN)
			{
				error(9);//�������ֺ�
				return -1;
			}
				
			strcpy(name, token);
			getNextLegalSym();
			if (symbol != ASSIGN)
			{
				error(7);//�������ֺ�
				return -1;
			}
				
			getNextLegalSym();
			if (symbol != INTCON)
			{
				error(8);//�������ֺ�
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
				error(10);//�������ֺ�
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
				error(9);//�������ֺ�
				return -1;
			}
				
			strcpy(name, token);
			getNextLegalSym();
			if (symbol != ASSIGN)
			{
				error(7);//�������ֺ�
				return -1;
			}
				
			getNextLegalSym();;
			if (symbol != CHARCON)
			{
				error(8);//�������ֺ�
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
				error(10);//�������ֺ�
				return -1;
			}
				
		} while (true);
	}
	else
	{
		error(11);//�������ֺ�
		return -1;
	}
	return 0;
}
int constantDeclare()//����˵���������һ��const�Ժ���룬����ʱ�Ѷ�����һ���ĵ�һ������
{
	while (symbol == CONSTTK)
	{
		constantDefine();//�˳�ʱ�Ѷ���ֺ�,�����Ƿ����
		getNextLegalSym();
	}
	return 0;
}
int varibleDefine(Type t,bool isGlobal=false)//�������壬����ʱ�Ѷ����һ������֮��Ķ��Ż�����������,����ʱ�����ֺ�
{
	//unsigned int num;
	if (symbol != LBRACK)//��������
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
			error(13);//�������ֺ�
			return -1;
		}
		else
		{
			arg[0] = atoi(token);
			getNextLegalSym();
			if (symbol != RBRACK)
			{
				error(14);//�������ֺ�
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
			error(10);//�������ֺ�
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
int varibleDeclare()//����˵��������ʱ�Ѷ����һ�����ʣ�int/char��������ʱ�Ѷ�����һ���ĵ�һ������
{//������˵����::= ���������壾;{���������壾;}
 //���������壾::= �����ͱ�ʶ����(����ʶ����|����ʶ������[�����޷�����������]��){,(����ʶ����|����ʶ������[�����޷�����������]�� )}
	Type t;
	while (isTypeSymbol(t))
	{
		getNextLegalSym();
		if (symbol != IDEN)
		{
			error(9,3);//������statement��ͷ
			//getNextLegalSym();
			return -1;
		}
		strcpy(name, token);
		getNextLegalSym();
		if (symbol == COMMA || symbol == SEMICN || symbol == LBRACK)
		{
			varibleDefine(t);//����ʱ�����ֺ�
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
int argList(int funcIndex,int fno)//����ʱ�Ѷ��������ţ�����ʱ�Ѷ��������ţ���������ű�
{
	Arg a;
	Type t;
	int index = 0;//��������
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
			error(9, 2);//������������
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
			error(19);//������������
			return -1;
		}
		if (symbol == RPARENT)
			break;
		else if (symbol = COMMA)
			getNextLegalSym();
	}
	if (symbol != RPARENT)
	{
		error(16,2);//������������
		return -1;
	}
		
	modifyArg(funcIndex, index, arg);
	modifyFuncDefBegin(index, fno);
	return 0;
}
int callFunc(bool permitVoid)//��������,����ʱ�Ѷ����ʶ��,�˳�ʱ����������,���غ�������ֵ���ڱ������,void����0
{//���з���ֵ����������䣾::= ����ʶ������(����ֵ��������)��
//���޷���ֵ����������䣾:: = ����ʶ������(����ֵ��������)��
	if (permitVoid == false && tmpTab.type == VoidFunc)
	{
		error(23);//�������ֺ�֮���һ������
		return -1;
	}
	int fno = tmpTab.no;
	Type type = tmpTab.type;
	int len = tmpTab.dim_argLen;
	int ct = 0;
	getNextLegalSym();
	if (symbol != LPARENT)
	{
		error(20,2);//�������ֺ�֮���һ������
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
		if (r == -1)//�������������ֺ�֮���һ������
			return -1;
		//...
		//genInterCode(PARA, r, fno, -1);
		paraArray.push_back(r);
		ct++;
	}
	if (ct != len)
	{
		error(33);//�������ֺ�֮���һ������
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
int factor()/*����,����ʱ�Ѷ����һ������,����ʱ�Ѷ�������ĵ�һ������,���������м�������*/
{//�����������ʱ�����������  getNextLegalSym(true,true)
	int r;
	if (symbol == LPARENT)//��(�������ʽ����)��
	{
		getNextLegalSym();
		r=expression();
		if (r == -1)//���������ֺ�֮���һ������
			return -1;
		if (symbol != RPARENT)
		{
			error(16,3);//�����ֺ�֮���һ������
			return -1;
		}
		getNextLegalSym(true,true);
		//...
		return r;
	}
	else if (symbol == INTCON)//��������
	{
		//...
		int num = atoi(token);
		int r = generateInterVar();
		genInterCode(LdIm, num, 0, r/*, true*/);
		getNextLegalSym(true,true);
		return r;
	}
	else if (symbol == CHARCON)//���ַ���
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
			error(21);//�������ֺ�֮��һ������
			return -1;
		}
		if (tmpTab.type==VoidFunc||
			tmpTab.type == IntFunc ||
			tmpTab.type == CharFunc)
		{
			r=callFunc(false);
			if (r == -1)//�������������ֺź��һ������
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
					error(22);//�������ֺ�֮��һ������
					//getNextLegalSym();
					return -1;
					//...
				}
				else
				{
					getNextLegalSym();
					int i=expression();
					if (i == -1)//�������������ֺ�֮���һ������
						return -1;
					if (symbol != RBRACK)
					{
						error(14,1);//�����ֺ�֮��һ������
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
int term()//�����ʱ�Ѷ����һ������,����ʱ�Ѷ��������һ������
{//���::=�����ӣ�{���˷�������������ӣ�}
	int pre, next;
	Symbol s;
	pre=factor();
	if (pre == -1)//�������������ֺ�֮���һ������
		return -1;
	while (symbol == MULT || symbol == DIV)
	{
		s = symbol;
		getNextLegalSym(true, false);
		next=factor();
		if (next == -1)//�������������ֺ�֮���һ������
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
int expression()//����ʱ�Ѷ����һ������,����ʱ�Ѷ��������һ������
{//�����ʽ��::= �ۣ������ݣ��{���ӷ�����������}
//����ǰgetNextSym(true,false)
	int pre, next;
	Symbol s=PLUS;
	if (symbol == PLUS || symbol == MINU)//˵����һ�����ʱض���������
	{
		s = symbol;
		getNextLegalSym(true,false);//�˴��ڶ�����������Ϊfalse
	}
	pre=term();
	if (pre == -1)//�������������ֺ�֮���һ������
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
		if (next == -1)//�������������ֺ�֮���һ������
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
int condition()//����,����ʱδ�����κε��ʣ�����ʱ�Ѷ��������һ������,��������lab2,������-1
{//��������::=  �����ʽ������ϵ������������ʽ���������ʽ�� //���ʽΪ0����Ϊ�٣�����Ϊ��
	int r,t;
	int lab1, lab2;
	Operator op;
	getNextLegalSym();
	r=expression();
	if (r == -1)//�������������ֺ�֮���һ������
		return -1;
	lab1 = genLabNo();
	if (isRelationOperator(op))
	{
		getNextLegalSym();
		t=expression();
		if (t == -1)//�������������ֺ�֮���һ������
			return -1;
		genInterCode(op, r, t, lab1);
	}
	else
	{
		//��0����ת
		//int zero = genInterCode(LdIm, 0, 0, generateInterVar());
		genInterCode(BNE, r, 0, lab1/*, false, true*/);
	}
	lab2 = genLabNo();
	genInterCode(GOTO, -1, -1, lab2);
	genInterCode(GenLab, -1, -1, lab1);
	return lab2;
}
int caseStatement(int expr,int lab1)//����ʱ�Ѷ���case,����ʱ�Ѷ��������һ������,����lab(i+1��1)
{//���������䣾  ::=  case��������������䣾
	getNextLegalSym();
	genInterCode(GenLab, -1, -1, lab1);
	int num;
	if (symbol == INTCON)
		num = atoi(token);
	else if (symbol == CHARCON)
		num = (int)token[0];
	else
	{
		error(31);//�������������֮���һ������
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
		error(32);//�������������֮���һ������
		return -1;
	}
	else
		getNextLegalSym();

	int r=statement();
	if (r == -1)//����
	{
		jumpToRbrace();
		getNextLegalSym();//�������������֮���һ������
		return -1;
	}
	return lab1;
}
int caseList(int expr,int endLab)//����ʱ�Ѷ���case������ʱ�Ѷ��������һ������
{//�������   ::=  ���������䣾{���������䣾}

	int lab1= genLabNo();
	while (symbol == CASETK)
	{
		lab1=caseStatement(expr,lab1);
		if (lab1 == -1)//������������������֮���һ������
			return -1;
		genInterCode(GOTO, -1, -1, endLab);
	}
	return lab1;
}
int statement()//��䣬�����Ѷ����������һ�����ʣ�����ʱ�Ѷ��������һ������
{
	if (symbol == IFTK)//��������䣾
	{//��������䣾  ::=  if ��(������������)������䣾��else����䣾��
		getNextLegalSym();
		if (symbol != LPARENT)
		{
			error(20,2);//�������ֺ�֮���һ������
			return -1;
		}
			
		int lab2=condition();
		if (lab2 == -1)//�������������ֺ�֮���һ������
			return -1;
		if (symbol != RPARENT)
		{
			error(16,3);//�������ֺ�֮���һ������
			return -1;
		}
			
		getNextLegalSym();
		int r=statement();
		if (r == -1)//����������������λ��
			return -1;
		if (symbol == ELSETK)
		{
			int lab3 = genLabNo();
			genInterCode(GOTO, -1, -1, lab3);
			genInterCode(GenLab, -1, -1, lab2);
			getNextLegalSym();
			r=statement();
			if (r == -1)//����������������λ��
				return -1;
			genInterCode(GenLab, -1, -1, lab3);
		}
		else//û��else
		{
			genInterCode(GenLab, -1, -1, lab2);
		}
	}
	else if (symbol == WHILETK)//��ѭ����䣾
	{//��ѭ����䣾::=  while ��(������������)������䣾
		int lab2, lab3;
		lab3 = genLabNo();
		genInterCode(GenLab, -1, -1, lab3);
		getNextLegalSym();
		if (symbol != LPARENT)
		{
			error(20,2);//�������ֺ�֮���һ������
			return -1;
		}
			
		lab2=condition();
		if (lab2 == -1)//�������������ֺ�֮���һ������
			return -1;
		if (symbol != RPARENT)
		{
			error(16, 3);//�������ֺ�֮���һ������
			return -1;
		}
			
		getNextLegalSym();
		int r=statement();
		if (r == -1)//����������������λ��
			return -1;
		genInterCode(GOTO, -1, -1, lab3);
		genInterCode(GenLab, -1, -1, lab2);
	}
	else if (symbol == LBRACE)//��{��������У���}��
	{
		getNextLegalSym();
		statement();
		while (symbol != RBRACE)
		{
			statement();
		}
		getNextLegalSym();
	}
	else if (symbol == IDEN)//���з���ֵ����������䣾;|���޷���ֵ����������䣾;������ֵ��䣾;
	{//����ֵ��䣾::=����ʶ�����������ʽ��|
		//			 ����ʶ������[�������ʽ����]��=�����ʽ��
		if (!lookupSymbolTab(token,tmpTab))
		{
			error(21);//�������ֺ�֮���һ������
			return -1;
		}
		if (tmpTab.type == VoidFunc || tmpTab.type == CharFunc || tmpTab.type == IntFunc)
		{//���з���ֵ����������䣾;|���޷���ֵ����������䣾;������ֵ��䣾;
			int r=callFunc(true);//�������ض���������
			if (r == -1)//�����������ֺ�֮���һ������
				return -1;
			getNextLegalSym();
			if (symbol != SEMICN)
				error(30);//���������൱���Զ���ӷֺţ��ô��󵥴��������һ��statement
			else//�����Ļ��ٶ���һ������
			getNextLegalSym();
		}
		else if (tmpTab.type == IntVar || tmpTab.type == CharVar)
		{
			int no = tmpTab.no;
			Type type = tmpTab.type;
			if (tmpTab.dim_argLen > 0)//��[�������ʽ����]��=�����ʽ��
			{
				int l = tmpTab.value.length;
				getNextLegalSym();
				if (symbol != LBRACK)
				{
					error(26);//�������ֺ�֮���һ������
					return -1;
				}
				getNextLegalSym();
				int r=expression();//�������������ֺ�֮���һ������
				if (r == -1)
					return -1;
				if (symbol != RBRACK)
				{
					error(14, 1);//�������ֺ�֮���һ������
					return -1;
				}
				check(l, r);
				getNextLegalSym();
				if (symbol != ASSIGN)
				{
					error(27);//�������ֺ�֮���һ������
					return -1;
				}
					
				getNextLegalSym();
				int t=expression();//�������ֺ�֮���һ������
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
			else//�������ʽ��
			{
				getNextLegalSym();
				if (symbol != ASSIGN)
				{
					error(27);//�������ֺ�֮���һ������
					return -1;
				}
					
				getNextLegalSym();
				int r=expression();
				if (r == -1)//�������ֺ�֮���һ������
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
			error(25);//�������ֺź��һ������
			return -1;
		}
	}
	else if (symbol == SCANFTK)//������䣾;
		//������䣾::=scanf ��(������ʶ����{,����ʶ����}��)��
	{
		getNextLegalSym();
		if (symbol != LPARENT)
		{
			error(20,2);//�������ֺ�֮���һ������
			return -1;
		}
			
		getNextLegalSym();
		if (symbol != IDEN)
		{
			error(9, 5);//�������ֺ�֮���һ������
			return -1;
		}
			
		
		if (!lookupSymbolTab(token,tmpTab))
		{
			error(21);//�������ֺ�֮���һ������
			return -1;
		}
		if (tmpTab.type!=CharVar&&tmpTab.type!=IntVar)
		{
			error(28);//�������ֺ�֮���һ������
			return -1;
		}
		else
		{
			if (tmpTab.dim_argLen != 0&&tmpTab.type!=CharVar)
			{
				error(28);//�������ֺ�֮���һ������
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
				error(10,1);//�������ֺ�֮���һ������
				return-1;
			}
			getNextLegalSym();
			if (symbol != IDEN)
			{
				error(9,3);//������statement��һ������
				return -1;
			}
				
			if (!lookupSymbolTab(token,tmpTab))
			{
				error(21);//�������ֺ�֮���һ������
				return -1;
			}
			if (tmpTab.type != CharVar&&tmpTab.type != IntVar)
			{
				error(28);//�������ֺ�֮���һ������
				return -1;
			}
			else
			{
				if (tmpTab.dim_argLen != 0 && tmpTab.type != CharVar)
				{
					error(28);//�������ֺ�֮���һ������
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
		getNextLegalSym();//�����������һ������
	}
	else if (symbol == PRINTFTK)//��д��䣾;
	{/*��д��䣾::= printf ��(�� ���ַ�����,�����ʽ�� ��)��|
		//			printf ��(�����ַ����� ��)�� |
			//		printf ��(�������ʽ����)*/

		getNextLegalSym();
		if (symbol != LPARENT)
		{
			error(20,2);//�������ֺ�֮���һ������
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
				if (r == -1)//�������������ֺ�֮���һ������
					return -1;
				if (symbol != RPARENT)
				{
					error(16,3);//�������������ֺ�֮���һ������
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
				error(29);//�������ֺ�֮���һ������
				return -1;
			}
				
		}
		else
		{
			int r=expression();
			if (r == -1)//�����������ֺ�֮���һ������
				return -1;
			if (symbol != RPARENT)
			{
				error(16,3);//�����������ֺ�֮���һ������
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
	else if (symbol == RETURNTK)//��������䣾;
	{//��������䣾::=return[��(�������ʽ����)��] 
		getNextLegalSym();
		if (symbol == LPARENT)
		{
			getNextLegalSym();
			int r=expression();
			if (r == -1)//�����������ֺ�֮���һ������
				return -1;
			returnValueExists = true;
			if (symbol != RPARENT)
			{
				error(16,3);//�������ֺ�֮���һ������
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
	else if (symbol == SWITCHTK)//�������䣾
	{//�������䣾::=switch ��(�������ʽ����)�� ��{��������� ��}��
		getNextLegalSym();
		if (symbol != LPARENT)
		{
			error(20,3);//�������������֮���һ������
			return -1;
		}
			
		getNextLegalSym();
		int r=expression();
		if (r == -1)//���������ֺ�֮���һ������
		{
			jumpToRbrace();
			getNextLegalSym();
			return -1;//�ٴ�������������֮���һ������
		}
		if (symbol != RPARENT)
		{
			error(16,4);//������������֮���һ������
			return -1;
		}
			
		getNextLegalSym();
		if (symbol != LBRACE)
		{
			error(17,2);//������������֮���һ������
			return -1;
		}
		getNextLegalSym();
		int endLab = genLabNo();
		int t=caseList(r,endLab);
		if (t == -1)//������������������֮���һ������
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
	else if (symbol == SEMICN)//���գ�; 
	{
		getNextLegalSym();
	}
	else
	{
		error(3);
	}
	return 0;
}
int compoundStatement(bool needEnter)//�������(��������)������ʱ�Ѷ����������,����ʱ�Ѷ����Ҵ�����
{
	if(needEnter)
		enterFunction();

	getNextLegalSym();
	constantDeclare();//����ʱ����varibleDeclare��statement��һ������
	varibleDeclare();//����ʱ����statement��һ������
	//getNextLegalSym();
	while (symbol != RBRACE)
	{
		statement();
	}
	exitFunction();
	return 0;
}

int functionDefine(Type t)//�������壬����ʱ�Ѷ�����С����,����ʱ�Ѷ����Ҵ�����
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
	argList(funcIndex,fno);//������С����
	getNextLegalSym();
	if (symbol != LBRACE)
	{
		error(17,1);//�������Ҵ�����
		return -1;
	}
	compoundStatement(false);
	if (!(needReturnValue&&returnValueExists||!needReturnValue&&!returnValueExists))
	{
		error(34);
		//needReturnValue = false;//�ָ�
	}
	genInterCode(FuncDefEnd, -1, -1, fno);
	return 0;
}
int program()
//������:: = �ۣ�����˵�����ݣۣ�����˵������{ ���з���ֵ�������壾 | ���޷���ֵ�������壾 }����������
{
	Type t;
	bool varibleDefinePermit = true;
	getNextLegalSym();
	constantDeclare();//����ʱ�Ѷ���ֺź��һ������
	while (isTypeSymbol(t)&&varibleDefinePermit)
	//���Դ������������ͬʱ������һ������˵�����з���ֵ�ģ�Ҳһ������
	//����ѭ��ʱ�Ѷ�����һ������
	{
		getNextLegalSym();
		if (symbol != IDEN)
		{
			error(9,4);//������int,char,void
			//���ڻ��п����б���˵�����ʲ�����
		}
		else
		{
			strcpy(name, token);
			getNextLegalSym();
			if (symbol == COMMA || symbol == SEMICN || symbol == LBRACK)//����˵��
			{
				varibleDefine(t,true);//����ʱ�����ֺ�
				getNextLegalSym();
			}
			else if (symbol = LPARENT)//����˵��
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
	while (isFuncSymbol(t))//{���з���ֵ�������壾|���޷���ֵ�������壾}
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
	//�������������Ѷ���main
	//����������::= void main��(����)�� ��{����������䣾��}��
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
