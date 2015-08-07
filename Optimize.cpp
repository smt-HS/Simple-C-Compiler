#include "Optimize.h"
#include "mips.h"
#include <stack>
using namespace std;

/*InterCode optInterCode[MAX_CODE_LEN];//优化后的四元式
int optInterCodeIdx = 0;

bool no_more = true;
void genOptInterCode(InterCode tmp)
{
	optInterCode[optInterCodeIdx++] = tmp;
	return;
}
void genOptInterCode(Operator op, int lop, int rop, int dop)
{
	optInterCode[optInterCodeIdx].op = op;
	optInterCode[optInterCodeIdx].lopr = lop;
	optInterCode[optInterCodeIdx].ropr = rop;
	optInterCode[optInterCodeIdx].dopr = dop;
	optInterCode[optInterCodeIdx].str = NULL;
	//optInterCode[optInterCodeIdx].blockNo = blockNo;
	optInterCodeIdx++;
}*/

/*#define MAX_DAG_NODE_NUM 3000
//DAG图结构
typedef struct
{
	int interCodeVarNo;//四元式编号
	int dagNodeVar;//dag图节点编号
}Inter_DAG;
Inter_DAG interDAG[MAX_DAG_NODE_NUM];
int interDAGIdx = 0;
typedef struct
{
	Operator op;//操作符
	int left;//左节点dag编号
	int right;
	bool leftExists;
	int repInterCodeVarNo;//代表中间变量
	bool isInStack;
}DAGNode;//DAG图节点信息
DAGNode dagNodeInfo[MAX_DAG_NODE_NUM];
int dagNodeIdx = 1;//0号节点保留给常数0
int fatherNodeCt = 0;//中间结点计数

int dagMatrix[MAX_DAG_NODE_NUM][MAX_DAG_NODE_NUM];//DAG图

void updateInterDAG(int interCodeVar,int dagNodeNo)//更新InterDAG
{
	for (int i = 0; i < interDAGIdx; i++)
	{
		if (interDAG[i].interCodeVarNo == interCodeVar)
		{
			interDAG[i].dagNodeVar = dagNodeNo;
			return;
		}
	}
	interDAG[interDAGIdx].interCodeVarNo = interCodeVar;
	interDAG[interDAGIdx].dagNodeVar = dagNodeNo;
	/*if (dagNode[dagNodeNo].repInterCodeVarNo == 0)
	{
		dagNode[dagNodeNo].repInterCodeVarNo = interCodeVar;
		dagNode[dagNodeNo].op = NoOperator;
	}*/
/*	interDAGIdx++;
	return;
}
int getDAGChildNodeNo(int interCodeVarNo)
{
	if (interCodeVarNo == 0)
		return 0;
	for (int i = 0; i < interDAGIdx; i++)
	{
		if (interDAG[i].interCodeVarNo == interCodeVarNo)
			return interDAG[i].dagNodeVar;
	}
	int dagNodeNo = dagNodeIdx++;
	interDAG[interDAGIdx].interCodeVarNo = interCodeVarNo;
	interDAG[interDAGIdx].dagNodeVar = dagNodeNo;
	interDAGIdx++;
	dagNodeInfo[dagNodeNo].repInterCodeVarNo = interCodeVarNo;
	dagNodeInfo[dagNodeNo].leftExists = false;
	return dagNodeNo;
}
void addDAGFatherNode(int dest, int left, int right, Operator op)
{
	int dagNodeNo = dagNodeIdx++;
	dagNodeInfo[dagNodeNo].left = left;
	dagNodeInfo[dagNodeNo].right = right;
	dagNodeInfo[dagNodeNo].leftExists = true;
	dagNodeInfo[dagNodeNo].op = op;
	dagNodeInfo[dagNodeNo].repInterCodeVarNo = dest;
	dagNodeInfo[dagNodeNo].isInStack = false;
	dagMatrix[left][dagNodeNo] = 1;
	dagMatrix[right][dagNodeNo] = 1;
	updateInterDAG(dest, dagNodeNo);
	fatherNodeCt++;
}
void addIntoDAG(InterCode tmp)
{
	Operator op = tmp.op;
	int left = tmp.lopr;
	int right = tmp.ropr;
	int dest = tmp.dopr;
	left = getDAGChildNodeNo(left);
	right = getDAGChildNodeNo(right);
	
	for (int i = 1; i < dagNodeIdx; i++)
	{
		if ((dagMatrix[left][i] == dagMatrix[right][i] == 1) &&
			dagNodeInfo[i].op == tmp.op&&dagNodeInfo[i].leftExists == true &&
			dagNodeInfo[i].left == left)
		{
			updateInterDAG(dest, i);
			return;
		}
	}
	addDAGFatherNode(dest, left, right, op);
}
bool noFatherNode(int dagNo)
{
	if (!dagNodeInfo[dagNo].leftExists||dagNodeInfo[dagNo].isInStack)
	{
		return false;
	}
	for (int i = 0; i < dagNodeIdx; i++)
	{
		if (dagMatrix[dagNo][i] != 0)
		{
			return false;
		}
	}
	return true;
}
void pushUpdate(int dagNo)
{
	for (int i = 0; i < dagNodeIdx; i++)
		dagMatrix[i][dagNo] = 0;
	dagNodeInfo[dagNo].isInStack = true;
}
void exportNode(int dagNo)
{
	int dest = dagNodeInfo[dagNo].repInterCodeVarNo;
	int left = dagNodeInfo[dagNo].left;
	int right = dagNodeInfo[dagNo].right;
	Operator op = dagNodeInfo[dagNo].op;
	left = dagNodeInfo[left].repInterCodeVarNo;
	right = dagNodeInfo[right].repInterCodeVarNo;
	genOptInterCode(op, left, right, dest);
	for (int i = 0; i < interDAGIdx; i++)
	{
		if (interDAG[i].dagNodeVar == dagNo&&interDAG[i].interCodeVarNo != dest)
		{
			genOptInterCode(MOVE, dest, -1, interDAG[i].interCodeVarNo);
		}
	}
}
void exportDAG()
{
	if (no_more)
		return;
	stack<int> dagStack;
	int left;
	while (dagStack.size() != fatherNodeCt)
	{
		for (int i = 0; i < dagNodeIdx; i++)
		{
			if (noFatherNode(i))
			{
				dagStack.push(i);
				pushUpdate(i);
				left = dagNodeInfo[i].left;
				while (noFatherNode(left))
				{
					dagStack.push(left);
					pushUpdate(left);
					left = dagNodeInfo[left].left;
				}
			}
		}
	}
	int k;
	while (!dagStack.empty())
	{
		k = dagStack.top();
		dagStack.pop();
		exportNode(k);
	}
	no_more = true;
}
void clearDAG()
{
	interDAGIdx = 0;
	dagNodeIdx = 1;
	memset(dagMatrix, 0, sizeof(dagMatrix));
	return;
}*/



//根据入口语句划分基本块
void divideBlock()
{
	int blockNo = 0;
	InterCode tmp;
	for (int i = 0; i < interCodeIndex; i++)
	{
		tmp = interCode[i];
		interCode[i].valid = true;
		if (isBlockBegin(interCode[i].op))
		{//语句序列的第一条语句属于入口语句
		 //能够跳转到的第一条语句属于入口语句
			blockNo++;
		}
		interCode[i].blockNo = blockNo;
		//紧跟在跳转语句之后的第一条语句属于入口语句
		if ( isJump(tmp.op))
		{
			blockNo++;
		}
	}
}
/*//DAG优化
int  dagInBlock(int beginIndex,int blockNo)
{//在同一个基本块内，由于语法限制，只有可能在最开始处出现内存分配类指令
	//memset(dagNodeTab, 0, sizeof(dagNodeTab));
	clearDAG();
	int index = beginIndex;
	while (interCode[index].blockNo == blockNo&&index<interCodeIndex)
	{
		//对于内存分配或者基本块开始标志
		if (isMemAlloc(interCode[index].op)||isBlockBegin(interCode[index].op))
		{
			genOptInterCode(interCode[index]);
			index++;
		}
		else if (isJump(interCode[index].op)||isIO(interCode[index].op))
		{//出现跳转指令，基本块必然结束,导出当前dag图
		 //出现IO指令，基本块继续，但是为了处理方便，此时也导出dag图
			//...
			exportDAG();
			clearDAG();
			genOptInterCode(interCode[index]);
			index++;
			cerr << "succeed" << endl;
		}
		else
		{
			addIntoDAG(interCode[index]);
			no_more = false;
			index++;
		}

	}
	if (index == interCodeIndex)
	{
		exportDAG();
		clearDAG();
	}
	return index;
}*/

int dagInBlock(int begin,int blockNo)
{
	int index = begin;
	Operator op,tmpOp;
	int left, right, dest,predest;
	int i;
	while (index < interCodeIndex&&interCode[index].blockNo == blockNo)
	{
		op = interCode[index].op;
		if (isArithmetic(op)&&interCode[index].valid)
		{
			left = interCode[index].lopr;
			right = interCode[index].ropr;
			dest = interCode[index].dopr;
			i = index + 1;
			while (i < interCodeIndex&&interCode[i].blockNo == blockNo)
			{
				if ((isArithmetic(interCode[i].op) ||
					isSetGet(interCode[i].op) ||
					interCode[i].op == SCANF) && interCode[i].valid)
				{
					if (interCode[i].dopr == left ||
						interCode[i].dopr == right ||
						interCode[i].dopr == dest)
						break;
				}
				if (interCode[i].op == op&&
					interCode[i].lopr == left&&
					interCode[i].ropr == right&& interCode[i].valid)
				{
					interCode[i].valid = false;
					//interCode[i].op = MOVE;
					//interCode[i].lopr = dest;
					//interCode[i].ropr = -1;
					//cerr << "---------------------------四元式优化" << endl;
					predest = dest;
					dest = interCode[i].dopr;
					for (int j = i + 1; j < interCodeIndex&&interCode[j].blockNo == blockNo; j++)
					{
						if (interCode[j].valid == false)
							continue;
						tmpOp = interCode[j].op;
						if (isArithmetic(tmpOp)||isSetGet(tmpOp)||
							tmpOp == BGR || tmpOp == BGE || tmpOp == BEQ ||
							tmpOp == BLE || tmpOp == BLS || tmpOp == BNE)
						{
							if (interCode[j].lopr == dest)
								interCode[j].lopr = predest;
							if (interCode[j].ropr == dest)
								interCode[j].ropr = predest;
						}
						if (tmpOp==PARA||tmpOp==PRINTF||tmpOp==RET)
						{
							if (interCode[j].lopr == dest)
								interCode[j].lopr = predest;
						}
						if ((isArithmetic(interCode[j].op) ||
							isSetGet(interCode[j].op) ||
							interCode[j].op == SCANF))
						{
							if (interCode[j].dopr == dest)
								break;

						}
					}
				}
				i++;
			}
		}
		index++;
	}
	return index;
}
void dagOptimize()
{
	int preBlockNo = -1;
	int i = 0;
	while (i < interCodeIndex)
	{
		if (interCode[i].blockNo != preBlockNo)
		{
			preBlockNo = interCode[i].blockNo;
			i = dagInBlock(i,preBlockNo);
		}
		else
		{
			i++;
		}
	}
}


void optimize()
{
	divideBlock();
	dagOptimize();
}
void mipsOptimize()
{
	for (int i = 1; i < mipsIdx; i++)
	{
		if ((strcmp(mips[i - 1].op, "sw") == 0) && (strcmp(mips[i].op, "lw") == 0)&&
			(strcmp(mips[i-1].rd,mips[i].rd)==0)&&(strcmp(mips[i-1].rs,mips[i].rs)==0))
		{
			mips[i].valid = false;
			//cerr << "------------------mips优化" << endl;
		}
	}
}