#ifndef SYNTAX_ANALYSIS_H
#define SYNTAX_ANALYSIS_H
int expression();
int statement();
int compoundStatement(bool needEnter=true);
void getNextLegalSym(bool EOFForbid = true,bool OpPrior=false);//EOFForbid:����EOF���˳���OpPrior:���������ŵ��������Ȱѷ��Ŷ���
int program();


#endif // !SYNTAX_ANALYSIS_H
