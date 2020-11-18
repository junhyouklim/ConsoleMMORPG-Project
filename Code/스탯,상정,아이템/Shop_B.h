#include "Shop_S.h"

int Shop_B(Item *item,Stat *stat,int *input,int *ctr_mode);
int Item_buy(Item *item,Stat *stat,int code,int num,char *str);
void InputNMove(int *input,int *select,int *select1,int show,int choice,int buy);
void SaveTEXT(char nameNprice[][100],int *codenum,int num,int select,int cnt);
int InputNSpace(Stat *stat,int *select,int *select1,int *choice,int *cnt,int *show,int *num,int *buy);
void CopyTYPE(char *type,int show);
int SelectBuy(Item *item,Stat *stat,char *str,int *codenum,int *choice,int *code,int select,int num);
void CopyMENU(int show,char menu[][100],int select);