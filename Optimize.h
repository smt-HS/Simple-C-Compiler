#ifndef OPTIMIZE_H
#define OPTIMIZE_H
#include "InterCode.h"
void optimize();
void mipsOptimize();
void divideBlock();
void dagOptimize();
/*extern InterCode optInterCode[MAX_CODE_LEN];//优化后的四元式
extern int optInterCodeIdx;*/
#endif