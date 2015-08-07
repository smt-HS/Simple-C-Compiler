#ifndef MIPS_H
#define MIPS_H
#include "ErrorHandling.h"
#define NAMELEN 100
#define NODE_TABLE_LEN 1000
#define WORD_LEN 10
#define MIPS_LEN 3000

extern const char mips_file[];
void generateMIPS();
void genMIPS(char* op, char* rd = "", char* rs = "", char* rt = "");
enum NodeType
{
	ConInt,
	ConChar,
	STRING,
	GlobInt,
	GlobChar,
	//Label,
	VarInt,
	VarChar,
};
const char nodeTypeStr[][10] =
{
	"ConInt",
	"ConChar",
	"STRING",
	"GlobInt",
	"GlobChar",
	//Label,
	"VarInt",
	"VarChar",
};
typedef struct 
{
	int no;
	char name[NAMELEN];
	NodeType type;
	//int fno;
	int length;
	int address;
	int regNo;
}Node;
extern Node nodeTab[NODE_TABLE_LEN];
extern int nodeIndex;
void printNodeTable();

typedef struct
{
	char op[50];
	char rd[WORD_LEN];
	char rs[WORD_LEN];
	char rt[WORD_LEN];
	bool valid;
}MIPS;
extern MIPS mips[MIPS_LEN];
extern int mipsIdx;
#endif