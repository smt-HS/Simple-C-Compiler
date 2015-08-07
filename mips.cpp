#define _CRT_SECURE_NO_WARNINGS

#include "mips.h"
#include "InterCode.h"
#include "ErrorHandling.h"
#include "Optimize.h"
#define FP_INIT 4

bool optimized = false;
int nodeIndex=0;
int address = 12;
Node nodeTab[NODE_TABLE_LEN];

MIPS mips[MIPS_LEN];
int mipsIdx;

#define ROUND(a, n)	(((((int)(a))+(n)-1)) & ~((n)-1))//对齐
bool isInMain = false;
void printNodeTable()
{
	FILE *fp = fopen("nodeTable.txt", "w");
	for (int i = 0; i < nodeIndex; i++)
	{
		fprintf(fp, "%d\t%s\t%d\t%d\t%s\n", 
			nodeTab[i].no, nodeTypeStr[nodeTab[i].type], nodeTab[i].length, nodeTab[i].address,nodeTab[i].name);
	}
	fclose(fp);
}
void genMIPS(char* op, char* rd, char* rs, char* rt)
{
	strcpy(mips[mipsIdx].op, op);
	strcpy(mips[mipsIdx].rd, rd);
	strcpy(mips[mipsIdx].rs, rs);
	strcpy(mips[mipsIdx].rt, rt);
	mips[mipsIdx].valid = true;
	mipsIdx++;
}
bool isInt(NodeType type)
{
	return (type == ConInt || type == GlobInt || type == VarInt);
}

void addInNodeTable(int no, char* name, NodeType type,int address = 0,int length=1)
{
	nodeTab[nodeIndex].no = no;
	strcpy(nodeTab[nodeIndex].name, name);
	nodeTab[nodeIndex].type = type;
	//nodeTab[nodeIndex].fno = fno;
	nodeTab[nodeIndex].address = address;
	nodeTab[nodeIndex].length = length;
	nodeIndex++;
}
bool lookupNodeTab(int no,Node &node)
{
	for (int i = 0; i < nodeIndex; i++)
	{
		if (nodeTab[i].no == no)
		{
			node = nodeTab[i];
			return true;
		}
	}
	return false;
}
void resetAddress()
{
	address = FP_INIT;
}
int getAddress(int add)
{
	//char s[WORD_LEN];
	add = ROUND(add, 4);
	//fprintf(mips_fp, "addi $sp $sp -%d\n", add);
	//sprintf(s,"-%d", add);
	//genMIPS("addi", "$sp", "$sp", s);
	address += add;
	//cerr << tmp << "\t" << address << endl;
	return address;
}
void dataSegment(InterCode inter[],int length)
{
	InterCode tmp;
	char name[NAMELEN];
	char s[50];
	//fprintf(mips_fp, ".data\n");
	genMIPS(".data", "", "", "");
	for (int i = 0; i < length; i++)
	{
		tmp = inter[i];
		switch (tmp.op)
		{
		case ConstInt:
			sprintf(name, "c%d", tmp.dopr);
			//fprintf(mips_fp, "%s: .word %d\n",name,  tmp.ropr);
			sprintf(s, "%s: .word %d", name, tmp.ropr);
			genMIPS(s, "", "", "");
			addInNodeTable(tmp.dopr, name, ConInt);
			break;
		case ConstChar:
			sprintf(name, "c%d", tmp.dopr);
			//fprintf(mips_fp, "%s: .byte '%c'\n", name, (char)tmp.ropr);
			sprintf(s, "%s: .byte '%c'", name, (char)tmp.ropr);
			genMIPS(s, "", "", "");
			addInNodeTable(tmp.dopr, name, ConChar);
			break;
		case GlobVarInt:
			sprintf(name, "g%d", tmp.dopr);
			//fprintf(mips_fp, "%s:\t .word\n", name);
			sprintf(s, "%s:\t .word 0", name);
			genMIPS(s, "", "", "");
			for (int j = 1; j < tmp.lopr; j++)
			{
				//fprintf(mips_fp, "\t .word\n");
				sprintf(s, "\t .word 0");
				genMIPS(s, "", "", "");
			}
				
			addInNodeTable(tmp.dopr, name, GlobInt,0,tmp.lopr);
			break;
		case GlobVarChar:
			sprintf(name, "g%d", tmp.dopr);
			//fprintf(mips_fp, "%s:\t .byte\n", name);
			sprintf(s, "%s:\t .byte 0", name);
			genMIPS(s, "", "", "");
			for (int j = 1; j < tmp.lopr; j++)
			{
				//fprintf(mips_fp, "\t .byte\n");
				sprintf(s, "\t .byte 0");
				genMIPS(s, "", "", "");
			}
				
			addInNodeTable(tmp.dopr, name, GlobChar,0,tmp.lopr);
			break;
		case PRINTF:
			if (tmp.str == NULL)
				break;
			sprintf(name, "c%d", tmp.lopr);
			//fprintf(mips_fp, "%s: .asciiz \"%s\"\n", name, tmp.str);
			sprintf(s, "%s: .asciiz \"%s\"", name, tmp.str);
			genMIPS(s, "", "", "");
			addInNodeTable(tmp.lopr, name, STRING);
			break;
		case LdIm:
			//载入立即数   3 --> LdIm 3  0/1 generateInterValue 0 为整数 1为字符
			sprintf(name, "c%d", tmp.dopr);
			if (tmp.ropr == 0)
			{
				//fprintf(mips_fp, "%s: .word %d\n", name, tmp.lopr);
				sprintf(s, "%s: .word %d", name, tmp.lopr);
				genMIPS(s, "", "", "");
				addInNodeTable(tmp.dopr, name, ConInt);
			}
			else
			{
				//fprintf(mips_fp, "%s: .byte '%c'\n", name, (char)tmp.lopr);
				sprintf(s, "%s: .byte '%c'", name, (char)tmp.lopr);
				genMIPS(s, "", "", "");
				addInNodeTable(tmp.dopr, name, ConChar);
			}
			break;
		default:
			break;
		}
	}
}
//const char enterFunction[] = "addi $sp $sp -4\nsw $ra 0($sp)\nsw $fp -4($sp)\nmove $fp $sp\naddi $sp $sp -4\n";
//const char exitfunction[] = "lw $ra 0($fp)\nmove $sp $fp\nlw $fp -4($fp)\naddi $sp $sp 4\n";
void enterFunction(int i)
{//"addi $sp $sp -4\nsw $ra 0($sp)\nsw $fp -4($sp)\nmove $fp $sp\naddi $sp $sp -4\n"
	genMIPS("addi", "$sp", "$sp","-4");
	genMIPS("sw", "$ra", "0($sp)", "");
	genMIPS("sw", "$fp", "-4($sp)", "");
	genMIPS("move", "$fp", "$sp","");
	//genMIPS("addi", "$sp", "$sp", "-4");
}
void exitFunction(int i)
{//"lw $ra 0($fp)\nmove $sp $fp\nlw $fp -4($fp)\naddi $sp $sp 4\n";
	genMIPS("lw", "$ra", "0($fp)","");
	genMIPS("move", "$sp", "$fp", "");
	genMIPS("lw", "$fp", "-4($fp)");
	genMIPS("addi", "$sp", "$sp", "4");
}
NodeType loadNode(int nodeNum, char* dest)
{
	char s[50];
	Node tmpNode;
	/*if (isCon)
	{
		fprintf(mips_fp, "li %s %d\n",dest, nodeNum);
		return ConInt;
	}*/
	if (nodeNum == 0)
	{
		//fprintf(mips_fp, "move %s $zero\n", dest);
		genMIPS("move", dest, "$zero", "");
	}
		
	lookupNodeTab(nodeNum, tmpNode);
	if (tmpNode.type == ConInt || tmpNode.type == GlobInt)
	{
		//fprintf(mips_fp, "lw %s %s\n",dest, tmpNode.name);
		genMIPS("lw", dest, tmpNode.name, "");
	}
	else if (tmpNode.type == VarInt)
	{
		//fprintf(mips_fp, "lw %s -%d($fp)\n",dest, tmpNode.address);
		sprintf(s, "-%d($fp)", tmpNode.address);
		genMIPS("lw", dest, s,"");
	}
	else if (tmpNode.type == ConChar || tmpNode.type == GlobChar)
	{
		//fprintf(mips_fp, "lb %s %s\n",dest, tmpNode.name);
		genMIPS("lb", dest, tmpNode.name, "");
	}
	else if (tmpNode.type == VarChar)
	{
		//fprintf(mips_fp, "lb %s -%d($fp)\n",dest, tmpNode.address);
		sprintf(s, "-%d($fp)", tmpNode.address);
		genMIPS("lb", dest, s, "");
	}
	return tmpNode.type;
}

void saveNode(int  nodeNum,bool isInt,char *dest)
{
	Node tmpNode;
	char s[50];
	if (!lookupNodeTab(nodeNum, tmpNode))
	{
		/*if (isInt)
		{
			add = getAddress(4);
			addInNodeTable(nodeNum, "", VarInt, add);
			//fprintf(mips_fp, "sw %s -%d($fp)\n",dest, add);
			sprintf(s, "-%d($fp)", add);
			genMIPS("sw", dest, s, "");
		}
		else
		{
			add = getAddress(1);
			addInNodeTable(nodeNum, "", VarChar, add);
			//fprintf(mips_fp, "sb %s -%d($fp)\n",dest, add);
			sprintf(s, "-%d($fp)", add);
			genMIPS("sb", dest, s, "");
		}*/
		cerr << "error" << endl;
	}
	else
	{
		if (tmpNode.type == VarInt)
		{
			//fprintf(mips_fp, "sw %s -%d($fp)\n",dest, tmpNode.address);
			sprintf(s, "-%d($fp)", tmpNode.address);
			genMIPS("sw", dest, s, "");
		}
		else if (tmpNode.type == GlobInt)
		{
			genMIPS("sw", dest, tmpNode.name, "");
		}
		else if ( tmpNode.type == VarChar)
		{
			//fprintf(mips_fp, "sb %s -%d($fp)\n",dest, tmpNode.address);
			sprintf(s, "-%d($fp)", tmpNode.address);
			genMIPS("sb", dest, s, "");
		}
		else if (tmpNode.type == GlobChar)
		{
			genMIPS("sb", dest, tmpNode.name);
		}
	}
}

void varSet(InterCode tmp,bool isInt)
{//CharSet,//字符变量赋值 a[1]=b --> CharSet  b 1 a    a=b --> CharSet b 0 a
//IntSet,//整形变量赋值  a[1]=b --> IntSet  b 1 a    a=b --> IntSet b 0 a
	Node tmpNode;
	char s[50];
	//待赋的值
	loadNode(tmp.lopr, "$t0");
	lookupNodeTab(tmp.dopr, tmpNode);
	//fprintf(mips_fp, "la $t2 -%d($fp)\n", tmpNode.address);//数组基地址
	if (tmpNode.type == VarInt || tmpNode.type == VarChar)
	{
		sprintf(s, "-%d($fp)", tmpNode.address);
		genMIPS("la", "$t2", s, "");
	}
	else if (tmpNode.type==GlobChar||tmpNode.type==GlobInt)
	{
		genMIPS("la", "$t2", tmpNode.name);
	}
	//偏移量
	loadNode(tmp.ropr, "$t1");
	if (isInt&&tmp.ropr != 0)
	{
		//fprintf(mips_fp, "sll $t1 $t1 2\n");
		genMIPS("sll", "$t1", "$t1", "2");
	}
		
	//fprintf(mips_fp, "add $t2 $t2 $t1\n");
	if (tmp.ropr != 0)
	{
		genMIPS("add", "$t2", "$t2", "$t1");
	}
	//赋值

	if (isInt)
	{
		//fprintf(mips_fp, "sw $t0 ($t2)\n");
		genMIPS("sw", "$t0", "($t2)","");
	}
		
	else
	{
		//fprintf(mips_fp, "sb $t0 ($t2)\n");
		genMIPS("sb", "$t0", "($t2)","");
	}
	
	return;
}
void varGet(InterCode tmp,bool isInt)
{//IntGet,//整形变量取值  b=a[1] --> IntGet  a 1 b    a=b --> IntGet b 0 a
//CharGet,//字符变量取值  b=a[1] --> CharGet  a 1 b    a=b --> CharGet b 0 a
	Node tmpNode;
	char s[50];
	
	lookupNodeTab(tmp.lopr, tmpNode);//载入数组基地址
	//fprintf(mips_fp, "la $t0 -%d($fp)\n", tmpNode.address);
	if (tmpNode.type == VarChar || tmpNode.type == VarInt)
	{
		sprintf(s, "-%d($fp)", tmpNode.address);
		genMIPS("la", "$t0", s, "");
	}
	else if (tmpNode.type == GlobChar || tmpNode.type == GlobInt)
	{
		genMIPS("la", "$t0", tmpNode.name);
	}

	loadNode(tmp.ropr, "$t1");//载入偏移量
	if (isInt&&tmp.ropr!=0)
	{
		//fprintf(mips_fp, "sll $t1 $t1 2\n");
		genMIPS("sll", "$t1", "$t1", "2");
	}
		
	//fprintf(mips_fp, "add $t0 $t0 $t1\n");
	if (tmp.ropr!=0)
		genMIPS("add", "$t0", "$t0", "$t1");
	if (isInt)//获取数组元素
	{
		//fprintf(mips_fp, "lw $t2 ($t0)\n");
		genMIPS("lw", "$t2", "($t0)");

	}
	else
	{
		//fprintf(mips_fp, "lb $t2 ($t0)\n");
		genMIPS("lb", "$t2", "($t0)");
	}
	saveNode(tmp.dopr, isInt, "$t2");
	
	return;
}

void scanf(InterCode tmp)
{
	Node tmpNode;
	int addr;
	char *n;
	char s[50];
	lookupNodeTab(tmp.dopr, tmpNode);
	addr = tmpNode.address;
	n = tmpNode.name;
	if (tmpNode.type == GlobInt)
	{
		//fprintf(mips_fp,
		//	"li $v0 5\nsyscall\nsw $v0 %s\n", n);
		genMIPS("li", "$v0", "5");
		genMIPS("syscall");
		genMIPS("sw", "$v0", n);
	}
	else if (tmpNode.type == VarInt)
	{
		//fprintf(mips_fp,
		//	"li $v0 5\nsyscall\nsw $v0 -%d($fp)\n",addr);
		genMIPS("li", "$v0", "5");
		genMIPS("syscall");
		sprintf(s, "-%d($fp)", addr);
		genMIPS("sw", "$v0", s);
	}
	else if (tmpNode.type == GlobChar)
	{
		if (tmpNode.length > 1)
		{
			//fprintf(mips_fp,
			//	"li $v0 8\nla $a0 %s\nli $a1 %d\nsyscall\n", n, tmpNode.length);
			genMIPS("li", "$v0", "8");
			genMIPS("la", "$a0", n);
			sprintf(s, "%d", tmpNode.length);
			genMIPS("li", "$a1", s);
			genMIPS("syscall");
		}
		else
		{
			//fprintf(mips_fp,
			//	"li $v0 12\nsyscall\nsb $v0 %s\n",n);
			genMIPS("li", "$v0", "12");
			genMIPS("syscall");
			genMIPS("sb", "$v0", n);
		}
	}
	else if (tmpNode.type==VarChar)
	{
		if (tmpNode.length > 1)
		{
			//fprintf(mips_fp,
			//	"li $v0 8\nla $a0 -%d($fp)\nli $a1 %d\nsyscall\n", addr, tmpNode.length);
			genMIPS("li", "$v0", "8");
			sprintf(s, "-%d($fp)", addr);
			genMIPS("la", "$a0", s);
			sprintf(s, "%d", tmpNode.length);
			genMIPS("li", "$a1", s);
			genMIPS("syscall");
		}
		else
		{
			//fprintf(mips_fp,
			//	"li $v0 12\nsyscall\nsb $v0 -%d($fp)\n", addr);
			sprintf(s, "-%d($fp)", addr);
			genMIPS("li", "$v0", "12");
			genMIPS("syscall");
			genMIPS("sb", "$v0", s);
		}
	}
}
void printf(InterCode tmp)
{
	Node tmpNode;
	char s[50];
	lookupNodeTab(tmp.lopr, tmpNode);
	if (tmp.str != NULL)
	{
		//fprintf(mips_fp,
			//"li $v0 4\nla $a0 %s\nsyscall\n",tmpNode.name);
		genMIPS("li", "$v0", "4");
		genMIPS("la", "$a0", tmpNode.name);
		genMIPS("syscall");
	}
	else
	{
		if (tmpNode.type == GlobInt||tmpNode.type==ConInt)
		{
			//fprintf(mips_fp,
			//	"li $v0 1\nlw $a0 %s\nsyscall\n", tmpNode.name);
			genMIPS("li", "$v0", "1");
			genMIPS("lw", "$a0", tmpNode.name);
			genMIPS("syscall");
		}
		else if (tmpNode.type == VarInt)
		{
			//fprintf(mips_fp,
				//"li $v0 1\nlw $a0 -%d($fp)\nsyscall\n",tmpNode.address);
			sprintf(s, "-%d($fp)", tmpNode.address);
			genMIPS("li", "$v0", "1");
			genMIPS("lw", "$a0", s);
			genMIPS("syscall");
		}
		else if (tmpNode.type == ConChar || tmpNode.type == GlobChar)
		{
			if (tmpNode.length > 1)
			{
				//fprintf(mips_fp,
				//	"li $v0 4\nla $a0 %s\nsyscall\n",tmpNode.name);
				genMIPS("li", "$v0", "4");
				genMIPS("la", "$a0", tmpNode.name);
				genMIPS("syscall");
			}
			else
			{
				//fprintf(mips_fp,
				//	"li $v0 11\nlw $a0 (%s)\nsyscall\n", tmpNode.name);
				genMIPS("li", "$v0", "11");
				sprintf(s, "%s", tmpNode.name);
				genMIPS("lb", "$a0", s);
				genMIPS("syscall");
			}
		}
		else if (tmpNode.type == VarChar)
		{
			if (tmpNode.length > 1)
			{
				//fprintf(mips_fp,
				//	"li $v0 4\nla $a0 -%d($fp)\nsyscall\n", tmpNode.address);
				sprintf(s, "-%d($fp)", tmpNode.address);
				genMIPS("li", "$v0", "4");
				genMIPS("la", "$a0", s);
				genMIPS("syscall");
			}
			else
			{
				//fprintf(mips_fp,
				//	"li $v0 11\nlw $a0 -%d($fp)\nsyscall\n", tmpNode.address);
				sprintf(s, "-%d($fp)", tmpNode.address);
				genMIPS("li", "$v0", "11");
				genMIPS("lb", "$a0", s);
				genMIPS("syscall");
			}
		}
	}
}
void para(InterCode tmp)
{
	NodeType t;
	t = loadNode(tmp.lopr, "$t0");
	/*if (isInt(t))
		addr = getAddress(4);
	else
		addr = getAddress(1);
	//fprintf(mips_fp,
	//	"sw $t0 -%d($fp)\n",addr);
	char s[50];
	sprintf(s,"0($sp)");*/
	genMIPS("addi", "$sp", "$sp", "-4");
	genMIPS("sw", "$t0", "($sp)");
}
void call(InterCode tmp)
{//Call,//函数调用  f(); -->  Call f -1/0/1 返回值编号 -1：void型 0:int型 1：char型
	int addr;
	char s[50];
	Node tmpNode;
	/*if (tmp.ropr > -1)
	{
		if (tmp.ropr == 0)
			addr = getAddress(4);
		else
			addr = getAddress(1);
	}*/
	//fprintf(mips_fp,
	//	"jal func%d\n",tmp.lopr);
	sprintf(s, "func%d", tmp.lopr);
	genMIPS("jal", s);
	if (tmp.ropr > -1)
	{
		lookupNodeTab(tmp.dopr, tmpNode);
		addr = tmpNode.address;
		if (tmp.ropr == 0)
		{
			//addInNodeTable(tmp.dopr, "", VarInt,addr);
			//fprintf(mips_fp, "sw $v0 -%d($fp)\n", addr);
			//
			sprintf(s, "-%d($fp)", addr);
			genMIPS("sw", "$v0", s);
		}
		else
		{
			//addInNodeTable(tmp.dopr, "", VarChar, addr);
			//fprintf(mips_fp, "sb $v0 -%d($fp)\n", addr);
			sprintf(s, "-%d($fp)", addr);
			genMIPS("sb", "$v0", s);
		}
	}
}
void ret(InterCode tmp)
{
	
	loadNode(tmp.lopr,"$v0");
	exitFunction(0);
	//fprintf(mips_fp, exitfunction);
	if (!isInMain)
		//fprintf(mips_fp, "jr $ra\n");
		genMIPS("jr", "$ra");
}
void updateSp(int index,bool isMain)
{
	int addr;
	char s[50];
	Operator end;
	if (isMain)
		end = MainEnd;
	else
		end = FuncDefEnd;
	for (int i = index; interCode[i].op != end; i++)
	{
		InterCode tmp = interCode[i];
		Node tmpNode;
		NodeType lt, rt;
		switch (tmp.op)
		{
		case LocalVarInt:
			sprintf(s, "int_var%d", tmp.dopr);
			addInNodeTable(tmp.dopr, s, VarInt, getAddress(4 * tmp.lopr), tmp.lopr);
			break;
		case LocalVarChar:
			sprintf(s, "char_var%d", tmp.dopr);
			addInNodeTable(tmp.dopr, s, VarChar, getAddress(tmp.lopr), tmp.lopr);
			break;
		case Call:
			if (tmp.ropr > -1)
			{
				if (tmp.ropr == 0)
				{
					addr = getAddress(4);
					addInNodeTable(tmp.dopr, "", VarInt, addr);
				}
				else
				{
					addr = getAddress(1);
					addInNodeTable(tmp.dopr, "", VarChar, addr);
				}
			}
			break;
		case Add:
		case Sub:
		case Multi:
		case Div:
			if (!lookupNodeTab(tmp.dopr, tmpNode))
			{
				if (tmp.lopr != 0)
				{
					lookupNodeTab(tmp.lopr, tmpNode);
					lt = tmpNode.type;
				}
				else
				{
					lt = ConChar;
				}
				if (tmp.ropr != 0)
				{
					lookupNodeTab(tmp.ropr, tmpNode);
					rt = tmpNode.type;
				}
				else
				{
					rt = ConChar;
				}
				if (isInt(lt) || isInt(rt))
				{
					addInNodeTable(tmp.dopr, "", VarInt, getAddress(4));
				}
				else
				{
					addInNodeTable(tmp.dopr, "", VarChar, getAddress(1));
				}
			}
			break;
		case CharGet:
			if (!lookupNodeTab(tmp.dopr, tmpNode))
			{
				addInNodeTable(tmp.dopr, "", VarChar, getAddress(1));
			}
			break;
		case IntGet:
			if (!lookupNodeTab(tmp.dopr, tmpNode))
			{
				addInNodeTable(tmp.dopr, "", VarInt, getAddress(4));
			}
			break;
		}
	}
	addr = getAddress(0);
	sprintf(s, "-%d", addr);
	genMIPS("addi", "$sp", "$fp", s);
}
void funcDefBegin(InterCode funcbegin,int index)
{
	char s[50];
	
	resetAddress();
	//fprintf(mips_fp, "func%d:\n", tmp.dopr);
	sprintf(s, "func%d:", funcbegin.dopr);
	genMIPS(s);
	//fprintf(mips_fp, enterFunction);
	enterFunction(0);
	updateSp(index, false);
	//...实参拷贝
	int paraNum = funcbegin.ropr;
	
	int src, dst;
	src = paraNum * 4;
	dst = -8;
	while (paraNum--)
	{
		//fprintf(mips_fp,
		//	"lw $t0 %d($fp)\nsw $t0 %d($fp)\n",src,dst);
		sprintf(s, "%d($fp)", src);
		genMIPS("lw", "$t0", s);
		sprintf(s, "%d($fp)", dst);
		genMIPS("sw", "$t0", s);
		src -= 4;
		dst -= 4;
	}
}
void text(InterCode tmp,int index)
{
	char name[NAMELEN];
	char s[50];
	NodeType lt, rt;
	//printSigleInterCode(tmp, mips_fp);
	if (!optimized)
	{
		sprintf(s, "#%s\t%d\t%d\t%d\t", OperatorStr[tmp.op], tmp.lopr, tmp.ropr, tmp.dopr);
		genMIPS(s);
	}
	else
	{
		if (!tmp.valid)
		{
			//cerr << "sth invalid" << endl;
			return;
		}
			
	}
	switch (tmp.op)
	{
	case Add:
		lt = loadNode(tmp.lopr, "$t0");
		rt = loadNode(tmp.ropr, "$t1");
		//fprintf(mips_fp, "add $t2 $t0 $t1\n");
		genMIPS("add", "$t2", "$t0", "$t1");
		saveNode(tmp.dopr, isInt(lt) || isInt(rt), "$t2");
		break;
	case Sub:
		lt = loadNode(tmp.lopr, "$t0");
		rt = loadNode(tmp.ropr, "$t1");
		//fprintf(mips_fp, "sub $t2 $t0 $t1\n");
		genMIPS("sub", "$t2", "$t0", "$t1");
		saveNode(tmp.dopr, isInt(lt) || isInt(rt), "$t2");
		break;
	case Multi:
		lt = loadNode(tmp.lopr, "$t0");
		rt = loadNode(tmp.ropr,  "$t1");
		//fprintf(mips_fp, "mul $t2 $t0 $t1\n");
		genMIPS("mul", "$t2", "$t0", "$t1");
		saveNode(tmp.dopr, isInt(lt) || isInt(rt), "$t2");
		break;
	case Div:
		lt = loadNode(tmp.lopr, "$t0");
		rt = loadNode(tmp.ropr, "$t1");
		//fprintf(mips_fp, "div $t0 $t1\nMFLO $t2\n");
		genMIPS("div", "$t0", "$t1");
		genMIPS("MFLO", "$t2");
		saveNode(tmp.dopr, isInt(lt) || isInt(rt), "$t2");
		break;
	case IntSet:
		varSet(tmp,true);
		break;
	case IntGet:
		varGet(tmp,true);
		break;
	case CharSet:
		varSet(tmp, false);
		break;
	case CharGet:
		varGet(tmp, false);
		break;
	case Call:
		call(tmp);
		break;
	case GenLab:
		sprintf(name, "label%d:", tmp.dopr);
		genMIPS(name);
		//fprintf(mips_fp, "%s:\n", name);
		break;
	case BGR:
		lt = loadNode(tmp.lopr, "$t0");
		rt = loadNode(tmp.ropr,  "$t1");
		//fprintf(mips_fp, "slt $t2 $t1 $t0\nbne $t2 $zero label%d\n", tmp.dopr);
		sprintf(s, "label%d", tmp.dopr);
		genMIPS("slt", "$t2", "$t1", "$t0");
		genMIPS("bne", "$t2", "$zero", s);
		break;
	case BNE:
		lt = loadNode(tmp.lopr,  "$t0");
		rt = loadNode(tmp.ropr,  "$t1");
		//fprintf(mips_fp, "bne $t0 $t1 label%d\n", tmp.dopr);
		sprintf(s, "label%d", tmp.dopr);
		genMIPS("bne", "$t0", "$t1", s);
		break;
	case BLS:
		lt = loadNode(tmp.lopr,  "$t0");
		rt = loadNode(tmp.ropr, "$t1");
		//fprintf(mips_fp, "slt $t2 $t0 $t1\nbne $t2 $zero label%d\n", tmp.dopr);
		sprintf(s, "label%d", tmp.dopr);
		genMIPS("slt", "$t2", "$t0", "$t1");
		genMIPS("bne", "$t2", "$zero", s);
		break;
	case BLE:
		lt = loadNode(tmp.lopr,  "$t0");
		rt = loadNode(tmp.ropr,  "$t1");
		//fprintf(mips_fp, "sub $t2 $t0 $t1\nblez $t2 label%d\n", tmp.dopr);
		sprintf(s, "label%d", tmp.dopr);
		genMIPS("sub","$t2", "$t0", "$t1");
		genMIPS("blez", "$t2", s);
		break;
	case BGE:
		lt = loadNode(tmp.lopr, "$t0");
		rt = loadNode(tmp.ropr, "$t1");
		//fprintf(mips_fp, "sub $t2 $t0 $t1\nbgez $t2 label%d\n", tmp.dopr);
		sprintf(s, "label%d", tmp.dopr);
		genMIPS("sub","$t2", "$t0", "$t1");
		genMIPS("bgez", "$t2", s);
		break;
	case BEQ:
		lt = loadNode(tmp.lopr,  "$t0");
		rt = loadNode(tmp.ropr,  "$t1");
		//fprintf(mips_fp, "beq $t0 $t1 label%d\n", tmp.dopr);
		sprintf(s, "label%d", tmp.dopr);
		genMIPS("beq", "$t0", "$t1", s);
		break;
	case GOTO:
		//fprintf(mips_fp, "j label%d\n", tmp.dopr);
		sprintf(s, "label%d", tmp.dopr);
		genMIPS("j", s);
		break;
	case PARA:
		para(tmp);
		break;
	case SCANF:
		scanf(tmp);
		break;
	case PRINTF:
		printf(tmp);
		break;
	case RET:
		ret(tmp);
		break;
	case LocalVarInt:
		/*len = 4 * tmp.lopr;
		sprintf(name, "int_var%d", tmp.dopr);
		addInNodeTable(tmp.dopr, name, VarInt, getAddress(len),tmp.lopr);*/
		break;
	case LocalVarChar:
		/*len = tmp.lopr;
		sprintf(name, "char_var%d", tmp.dopr);
		addInNodeTable(tmp.dopr, name, VarChar, getAddress(len),tmp.lopr);*/
		break;
	case FuncDefBegin:
		funcDefBegin(tmp,index);
		break;
	case FuncDefEnd:
		//fprintf(mips_fp, exitfunction);
		exitFunction(0);
		//fprintf(mips_fp, "jr $ra\n");
		genMIPS("jr", "$ra");
		break;
	case MainBegin:
		resetAddress();
		//fprintf(mips_fp, "main:\n");
		genMIPS("main:");
		//fprintf(mips_fp, "move $fp $sp\naddi $sp $sp -4\n");
		genMIPS("move", "$fp", "$sp");
		//genMIPS("addi", "$sp", "$sp", "-4");
		updateSp(index, true);
		break;
	case MainEnd:
		//fprintf(mips_fp, "move $sp $fp\n");
		//fprintf(mips_fp,"li $v0 10\nsyscall\n");
		genMIPS("move", "$sp", "$fp");
		genMIPS("li", "$v0", "10");
		genMIPS("syscall");
		break;
	default:
		break;
	}
}
void mainText(InterCode inter[])
{
	isInMain = true;
	int i = 0;;
	while (inter[i].op != MainBegin)
	{
		i++;
	}
	while (inter[i].op != MainEnd)
	{
		text(inter[i],i);
		i++;
	}
	text(inter[i],i);
	isInMain = false;
}
void otherText(InterCode inter[])
{
	int i = 0;
	while (inter[i].op != MainBegin)
	{
		text(inter[i],i);
		i++;
	}
}
void textSegment(InterCode inter[])
{
	//fprintf(mips_fp, ".text\n");
	genMIPS(".text");
	mainText(inter);
	otherText(inter);
}
void printMIPS(char* mips_file)
{
	//const char mips_file[] = "mips.asm";
	FILE *mips_fp;
	mips_fp = fopen(mips_file, "w");
	//cerr << mipsIdx << endl;
	for (int i = 0; i < mipsIdx; i++)
	{
		if (mips[i].valid)
		{
			fprintf(mips_fp, "%s\t%s\t%s\t%s\t\n",
				mips[i].op, mips[i].rd, mips[i].rs, mips[i].rt);
		}
	}
	fclose(mips_fp);
}
void clearMIPS()
{
	mipsIdx = 0;
	memset(mips, 0, sizeof(mips));
	nodeIndex = 0;
	memset(nodeTab, 0, sizeof(nodeTab));
}

void generateMIPS()
{
	cerr << "共有错误" << totalError << "个" << endl;
	if (totalError > 0)
	{
		cerr << "存在错误，生成mips失败" << endl;
		return;
	}
	cerr << "生成mips汇编..." << endl;

	printInterCode("InterCode.txt", interCode, interCodeIndex);
	dataSegment(interCode, interCodeIndex);
	textSegment(interCode);
	printMIPS("mips.asm");
	clearMIPS();

	optimized = true;
	optimize();
	/*dataSegment(interCode, interCodeIndex);
	textSegment(interCode);*/
	printInterCode("InterCode-opt.txt", interCode, interCodeIndex);
	//mipsOptimize();
	//printMIPS("opt.asm");


	cerr << "mips 已生成" << endl;
}