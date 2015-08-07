#include "Optimize.h"
#include "mips.h"
#include <stack>
using namespace std;

/*InterCode optInterCode[MAX_CODE_LEN];//�Ż������Ԫʽ
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
//DAGͼ�ṹ
typedef struct
{
	int interCodeVarNo;//��Ԫʽ���
	int dagNodeVar;//dagͼ�ڵ���
}Inter_DAG;
Inter_DAG interDAG[MAX_DAG_NODE_NUM];
int interDAGIdx = 0;
typedef struct
{
	Operator op;//������
	int left;//��ڵ�dag���
	int right;
	bool leftExists;
	int repInterCodeVarNo;//�����м����
	bool isInStack;
}DAGNode;//DAGͼ�ڵ���Ϣ
DAGNode dagNodeInfo[MAX_DAG_NODE_NUM];
int dagNodeIdx = 1;//0�Žڵ㱣��������0
int fatherNodeCt = 0;//�м������

int dagMatrix[MAX_DAG_NODE_NUM][MAX_DAG_NODE_NUM];//DAGͼ

void updateInterDAG(int interCodeVar,int dagNodeNo)//����InterDAG
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



//���������仮�ֻ�����
void divideBlock()
{
	int blockNo = 0;
	InterCode tmp;
	for (int i = 0; i < interCodeIndex; i++)
	{
		tmp = interCode[i];
		interCode[i].valid = true;
		if (isBlockBegin(interCode[i].op))
		{//������еĵ�һ���������������
		 //�ܹ���ת���ĵ�һ���������������
			blockNo++;
		}
		interCode[i].blockNo = blockNo;
		//��������ת���֮��ĵ�һ���������������
		if ( isJump(tmp.op))
		{
			blockNo++;
		}
	}
}
/*//DAG�Ż�
int  dagInBlock(int beginIndex,int blockNo)
{//��ͬһ���������ڣ������﷨���ƣ�ֻ�п������ʼ�������ڴ������ָ��
	//memset(dagNodeTab, 0, sizeof(dagNodeTab));
	clearDAG();
	int index = beginIndex;
	while (interCode[index].blockNo == blockNo&&index<interCodeIndex)
	{
		//�����ڴ������߻����鿪ʼ��־
		if (isMemAlloc(interCode[index].op)||isBlockBegin(interCode[index].op))
		{
			genOptInterCode(interCode[index]);
			index++;
		}
		else if (isJump(interCode[index].op)||isIO(interCode[index].op))
		{//������תָ��������Ȼ����,������ǰdagͼ
		 //����IOָ����������������Ϊ�˴����㣬��ʱҲ����dagͼ
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
					//cerr << "---------------------------��Ԫʽ�Ż�" << endl;
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
			//cerr << "------------------mips�Ż�" << endl;
		}
	}
}