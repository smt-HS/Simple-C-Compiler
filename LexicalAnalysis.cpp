#include "LexicalAnalysis.h"
#include "ErrorHandling.h"

char ch;//当前读到的字符
//char token[LEN];//当前读到的单词
int lexicalIndex;//token下标
//Symbol symbol;//单词种类

char token[LEN];//当前读到的单词
Symbol symbol;//单词种类
FILE *ifp, *ofp;//输入输出文件指针
int line = 1;//当前行号
int isWhite()
{
	return (ch==' '||ch=='\t'||ch=='\n');
}

void clearToken()
{
	memset(token, 0, sizeof(token));
	lexicalIndex = 0;
}

int getch()
{
	
	ch=fgetc(ifp);
	//printf("%c\n", ch);
	if (ch == '\n') 
	{
		line++;
		//cout << line <<endl;
	}
	return ch;
}
int isLetter()
{
	return (ch == '_' || ch >= 'a'&&ch <= 'z' || ch >= 'A'&&ch <= 'Z');
}
int isDigit()
{
	return isdigit(ch);
}
int isEOF()
{
	return ch == EOF;
}
void retract()
{
	/*fpos_t pos;
	fgetpos(ifp, &pos);
	pos--;
	fsetpos(ifp, &pos);*/
	ungetc(ch, ifp);
	if (ch == '\n')
		line--;
}
void catToken()
{
	if (lexicalIndex == LEN - 1)
	{
		error(0);
	}
	token[lexicalIndex++] = ch;
}
int punctSym()//处理非字母，非数字
{
	catToken();//存储当前读到的字符
	switch (ch)
	{
	case '<':
		getch();
		if (ch == '=')
		{
			catToken();
			symbol = LEQ;
			return 0;
		}
		else
		{
			retract();
			symbol = LSS;
			return 0;
		}
	case '=':
		getch();
		if (ch == '=')
		{
			catToken();
			symbol = EQL;
			return 0;
		}
		else
		{
			retract();
			symbol = ASSIGN;
			return 0;
		}
	case '>':
		getch();
		if (ch == '=')
		{
			catToken();
			symbol = GEQ;
			return 0;
		}
		else
		{
			retract();
			symbol = GRE;
			return 0;
		}
	case '{': 
		symbol = LBRACE;
		return 0;
	case '}':
		symbol = RBRACE;
		return 0;
	case '[':
		symbol = LBRACK;
		return 0;
	case ']':
		symbol = RBRACK;
		return 0;
	case '(':
		symbol = LPARENT;
		return 0;
	case ')':
		symbol = RPARENT;
		return 0;
	case '+':
		symbol = PLUS;
		return 0;
	case '-':
		symbol = MINU;
		return 0;
	case '*':
		symbol = MULT;
		return 0;
	case ';':
		symbol = SEMICN;
		return 0;
	case ':':
		symbol = COLON;
		return 0;
	case ',':
		symbol = COMMA;
		return 0;
	case '!':
		getch();
		if (ch == '=')
		{
			catToken();
			symbol = NQL;
			return 0;
		}
		else
		{
			//retract();
			return error(1);
			//clearToken();
			
		}
	case '/':
		getch();
		if (ch == '/')
		{
			while (ch != '\n')
				getch();
			clearToken();
			return 1;
		}
		else if (ch == '*')
		{
			do
			{
				do
				{
					getch();
				} while (ch!='*');
				getch();
				if (ch == '/')
				{
					break;
				}
			} while (ch!=EOF);
			clearToken();
			return 1;
		}
		else
		{
			symbol = DIV;
			retract();
			return 0;
		}
	case '\'':
		clearToken();
		getch();
		catToken();
		if (!isLetter() && !isDigit() &&
			(ch != '+') && (ch != '-') && (ch != '*') && (ch != '/'))
		{
			error(24);
			//getch();
			//return 1;
		}
		
		symbol = CHARCON;
		getch();
		if (ch != '\'')
		{
			return error(2);
		}
		return 0;
	case '"':
		clearToken();
		getch();
		while (ch != '"')
		{
			if (!(ch == 33 || ch == 32 || ch >= 35 && ch <= 126))
			{
				error(4);
			}
			catToken();
			getch();
			if (ch == EOF)
			{
				error(6);
			}
		}
		symbol = STRCON;
		return 0;
	default:
		return error(1);
		
	}
}
void reserve()
{
	int i ;
	for (i = 0; i < ReserveWordsNum; i++)
	{
		if (strcmp(reserveWords[i], token) == 0)
		{
			symbol = (Symbol)i;
			return;
		}
	}
	symbol = IDEN;
	for (int i = 0; i < lexicalIndex; i++)
	{
		if (token[i] >= 'A' && token[i] <= 'Z')
		{
			token[i] = token[i] + 'a' - 'A';
		}
	}
}
bool theFirstDigitOfNumberISLegal()
{
	int i = 0;
	if (token[i] == '+' || token[i] == '-')
	{
		i++;
		return !(token[i] == '0'&&token[i + 1] != '0'&&token[i + 1]!=0);
	}
	else
		return !(token[i] == '0'&&token[i + 1] != '0'&&token[i + 1] != 0);
}
int getSym(bool OpPrior)//文档结束返回-1,合法成分返回0，否则返回1
{
	int sigflag=0, negflag=0;
	clearToken();
	getch();
	while (isWhite())//滤去空白、换行等等
	{
		getch();
	}
	if (isEOF())
	{
		return -1;
	}
	if (isLetter())
	{
		while (isLetter()||isDigit())
		{
			catToken();
			getch();
		}
		reserve();
		retract();
		return 0;
	}
	else if (isDigit()||ch=='+'||ch=='-')//整数常量
	{
		catToken();
		if (ch == '+' || ch == '-')
		{
			if (OpPrior)
			{
				clearToken();
				return punctSym();
			}
			sigflag = 1;
			if (ch == '-')
				negflag = 1;
		}
		getch();
		if (sigflag == 1&&!isDigit())
		{
			if (negflag == 1)
				symbol = MINU;
			else
				symbol = PLUS;
			retract();
			return 0;
		}
		while (isDigit())
		{
			catToken();
			getch();
		}
		retract();
		if (!theFirstDigitOfNumberISLegal())
		{
			return error(12);
			
		}
		symbol = INTCON;
		return 0;
	}
	else
	{
		return punctSym();
	}

}

