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
	int r=0;//����ֵ��Ϊ0�����������У�1�����¶�����
	//fprintf(stderr, "��%d��:\t%s\t%s\n", line, token, errorInfo[no]);
	cerr << "Line number:" << line << "\t" << token << "\t" << errorInfo[no]<<"\t"<<no /*<<"\t"<<fromFile<<"\t"<<fromLine*/<< endl;;
	switch (no)
	{
	case 0://���ʳ��ȳ��� 
		while (isLetter() || isDigit())
			getch();
		retract();
		r = 0;
		break;
	case 1://δ֪�����
		r = 1;
		break;
	case 2://ȱ�ٵ����ţ���������
		while (ch != '\'')
		{
			if (isEOF())
				error(6);
			getch();
		}
		r = 0;
		break;
	case 3://δ֪���
		jumpToStatement();
		r = 0;
		break;
	case 4://�ַ����г��ַǷ��ַ�
		r = 0;
		break;
	case 5://ȱ��}
		jumpToStatement();
		break;
	case 6://��������
		break;
	case 7://����δ��ʼ��
	case 8://������ʼ�����ʹ���
		jumpToSemicn();
		break;
	case 9://�Ƿ����Զ����ʶ��
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
	case 10://ȱ��','
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
	case 11://δ֪��������
		jumpToSemicn();
		break;
	case 12://����������һ�����ֲ���Ϊ0
		r = 0;
		break;
	case 13://δ��ȷ��ʼ�����鳤��
		jumpToSemicn();
		break;
	case 14://ȱ��']'
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
	case 15://ȱ�����ͱ�ʶ��
		jumpToRparent(); break;
	case 16://ȱ�� ')'
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
	case 17://ȱ��'{'
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
	case 18://�Ƿ��ı�������
		jumpToStatement();
		break;
	case 19://�����б��д��ڷǷ�����
		jumpToRparent();
		break;
	case 20://ȱ��'('
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
	case 21://��ʶ��δ����
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 22://��ͼ�Է�������������������
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 23://�˴����ܵ���û�з���ֵ�ĺ���
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 24://�ַ�����Ϊ�Ƿ��ַ�
		r = 0;
		break;
	case 25://��ֵ�����ֵ�����޸�
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 26://ȱ��'['
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 27://ȱ��'='
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 28://�ñ���/�����޷���������
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 29://����������޷�ʶ��
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 30://ȱ��';'
		//ֻ�������������൱���Զ�������˷ֺ�
		break;
	case 31://case �����Ϊ����
		jumpToRbrace();
		getNextLegalSym();
		break;
	case 32://ȱ��ð��
		jumpToRbrace();
		getNextLegalSym();
		break;
	case 33://�β�ʵ�β�����
		jumpToSemicn();
		getNextLegalSym();
		break;
	case 34://����ȱ�ٷ���ֵ
		break;//ֻ����
	case 35://�����д����޷�ʶ��Ĳ��֣���main��������֮���д��룩
		break;//ֻ����
	case 36://����/�����ض���
		break;//ֻ����
	case 37://�����±�Խ��
		break;//ֻ����
	}
	
	if (no == 6)
	{
		system("pause");
		exit(-1);
	}
		
	return r;
}