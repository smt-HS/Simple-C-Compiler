#ifndef SYNTAX_ANALYSIS_H
#define SYNTAX_ANALYSIS_H
int expression();
int statement();
int compoundStatement(bool needEnter=true);
void getNextLegalSym(bool EOFForbid = true,bool OpPrior=false);//EOFForbid:读到EOF即退出，OpPrior:遇到带符号的整数，先把符号读出
int program();


#endif // !SYNTAX_ANALYSIS_H
