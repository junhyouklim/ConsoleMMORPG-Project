#include "Shop_B.h"

int Shop_B(Item *item,Stat *stat,int *input,int *ctr_mode)
{
    static int show=0; //0:초기화면 1:장비종류선택 2:무기 3:머리 4:몸통 5:신발 6:장갑 7:방어구부위선택창
    static int select=0,select1=0; //input 저장 변수
    static int choice=0; //구매 선택 변수
    static int cnt=0,num=0,buy=0; //cnt: 장비 갯수 변수 num: code 지정 변수 buy:구매 상태 확인 변수
    static char type[50]; //장비 타이틀 저장 배열
    static char buy_chk[50]; //구매 확인 배열
    int i,j=0,code,chk=0;
    int codenum[5]={101,201,301,401,501};
    char nameNprice[20][100];
    char menu[10][100]={0};
    char info[100];

    if(*ctr_mode==0)
    {
        if(*input!=KEY_SPACE) InputNMove(input,&select,&select1,show,choice,buy);
        else chk=InputNSpace(stat,&select,&select1,&choice,&cnt,&show,&num,&buy);
        code=select+codenum[num];
    }
    if(stat->mode!=0)
    {
        CopyTYPE(type,show); //장비 부위 타이틀 복사
        if(chk!=0)
        {
            buy=Item_buy(item,stat,code,1,buy_chk);
        }
        CopyMENU(show,menu,select);
        if(show>=0&&show<3)
        {
            for(i=0;i<MAP_Y;i++)
            {
                if(i==0) Prt_Fix_MapX(0, "top");
                else if(i==1) Prt_Fix_MapX(0, " ");
                else if(i==2) Prt_Fix_MapX(20, menu[0]);
                else if(i==3) Prt_Fix_MapX(0, " ");
                else if(i==4) Prt_Fix_MapX(23, menu[1]);
                else if(i==5) Prt_Fix_MapX(23, menu[2]);
                else if(i==6) Prt_Fix_MapX(23, menu[3]);
                else if(i==7) Prt_Fix_MapX(23, menu[4]);
                else if(i==8) Prt_Fix_MapX(23, menu[5]);
                else if(i==MAP_Y-1) Prt_Fix_MapX(0, "bot");
                else Prt_Fix_MapX(0, " ");
                space(3," ");
                prtside(stat,item, &stat->mode, ctr_mode, input, i);  //사이드바 통합 출력
                printf("\n");
            }
        }
        else
        {
            //구매
            SaveTEXT(nameNprice,codenum,num,select,cnt);
            //장비 종류 선택
            for(i=0;i<MAP_Y;i++)
            {
                if(i==0) Prt_Fix_MapX(0, "top");
                else if(i==1) Prt_Fix_MapX(16, "=============================");
                else if(i==2) Prt_Fix_MapX(21, type); 
                else if(i==3) Prt_Fix_MapX(16, "=============================");
                else if(i==5) Prt_Fix_MapX(19, nameNprice[0]);
                else if(i==6) Prt_Fix_MapX(19, nameNprice[1]);
                else if(i==7) Prt_Fix_MapX(19, nameNprice[2]);
                else if(i==9) Prt_Fix_MapX(19, nameNprice[3]);
                else if(i==10) Prt_Fix_MapX(19, nameNprice[4]);
                else if(i==11) Prt_Fix_MapX(19, nameNprice[5]);
                else if(i==13) Prt_Fix_MapX(19, nameNprice[6]);
                else if(i==14) Prt_Fix_MapX(19, nameNprice[7]);
                else if(i==15) Prt_Fix_MapX(19, nameNprice[8]);
                else if(i==17)
                {
                    if(cnt>=10) Prt_Fix_MapX(19, nameNprice[9]);
                    else Prt_Fix_MapX(0, " ");
                } 
                else if(i==18)
                {
                    if(cnt>=10)Prt_Fix_MapX(19, nameNprice[10]);
                    else if (select1==0&&buy!=0) Prt_Fix_MapX(19, buy_chk);
                    else if (choice==1&&select1==0) Prt_Fix_MapX(19, "> 구매");
                    else if (choice==1&&select1==1) Prt_Fix_MapX(19, " 구매");
                    else if(select==cnt) Prt_Fix_MapX(0, " ");
                    else if(cnt<=10)
                    {
                        for(j=0;j<cnt;j++)
                        {
                            if(select==j)
                            {
                                iteminf(info ,code,0);
                                Prt_Fix_MapX(10, info);
                            }
                        }
                    }
                }
                else if(i==19)
                {
                    if(cnt>=10)Prt_Fix_MapX(19, nameNprice[11]);
                    else if (select1==0&&buy!=0) Prt_Fix_MapX(0, " ");
                    else if (choice==1&&select1==1) Prt_Fix_MapX(19, "> 뒤로가기");
                    else if (choice==1&&select1==0) Prt_Fix_MapX(19, " 뒤로가기");
                    else if(select==cnt) Prt_Fix_MapX(0, " ");
                    else if(cnt<=10)
                    {    
                        for(j=0;j<cnt;j++)
                        {
                            if(select==j)
                            {
                                iteminf(info ,code,1);
                                Prt_Fix_MapX(10, info);
                            }
                        }
                    }
                }
                else if(i==21)
                {
                    if(cnt>=10)Prt_Fix_MapX(19, nameNprice[12]);
                    else Prt_Fix_MapX(0, " ");
                }
                else if(i==22)
                {
                    if(cnt>=10)Prt_Fix_MapX(19, nameNprice[13]);
                    else Prt_Fix_MapX(0, " ");
                }
                else if(i==23)
                {
                    if(cnt>=10)Prt_Fix_MapX(19, nameNprice[14]);
                    else Prt_Fix_MapX(0, " ");
                }
                else if(i==25)
                {
                    if (cnt>=10&&select1==0&&buy!=0) Prt_Fix_MapX(19, buy_chk);
                    else if (show==3&&choice==1&&select1==0) Prt_Fix_MapX(19, "> 구매");
                    else if (show==3&&choice==1&&select1==1) Prt_Fix_MapX(19, " 구매");
                    else if(select==cnt) Prt_Fix_MapX(0, " ");
                    else if(cnt>=10)
                    {
                        for(j=0;j<cnt;j++)
                        {
                            if(select==j) 
                            {
                                iteminf(info ,code,0);
                                Prt_Fix_MapX(10, info);
                            }

                        }
                    }
                    else Prt_Fix_MapX(0, " ");
                }
                else if(i==26)
                {
                    if (cnt>=10&&select1==0&&buy!=0) Prt_Fix_MapX(0, " ");
                    else if (show==3&&choice==1&&select1==1) Prt_Fix_MapX(19, "> 뒤로가기");
                    else if (show==3&&choice==1&&select1==0) Prt_Fix_MapX(19, " 뒤로가기");
                    else if(select==cnt) Prt_Fix_MapX(0, " ");
                    else if(cnt>=10)
                    {    
                        for(j=0;j<cnt;j++)
                        {
                            if(select==j) 
                            {
                                iteminf(info ,code,1);
                                Prt_Fix_MapX(10, info);
                            }
                        }
                    }
                    else Prt_Fix_MapX(0, " ");
                }
                else if(i==32)
                {
                    if(select==cnt) Prt_Fix_MapX(3, "> 뒤로가기");
                    else Prt_Fix_MapX(3, "  뒤로가기");
                } 
                else if(i==MAP_Y-1) Prt_Fix_MapX(0, "bot");
                else Prt_Fix_MapX(0, " ");
                space(3," ");
                prtside(stat,item, &stat->mode, ctr_mode, input, i);  //사이드바 통합 출력
                printf("\n");
            }
        }
        *input=0;
    }
    return 0;
}

int Item_buy(Item *item,Stat *stat,int code,int num,char *str)
{
    int temp;
    int price=0;

    itemprice(&price, code);
    if(stat->mon>=price)
    {
        temp=item_get(item, code, num);
        if(temp==0)
        {
            strcpy(str,"구매 완료");
            stat->mon-=price;
            return 1;
        }
        else 
        {
            strcpy(str,"인벤토리 자리 부족");
            return 2;
        }
    }
    else 
    {
        strcpy(str,"소지금 부족");
        return 3;
    }
    return 0;
}

void InputNMove(int *input,int *select,int *select1,int show,int choice,int buy)
{
    int max;

    if(show==0) max=1;
    else if(show==1) max=2;
    else if(show==2) max=4;
    else if(show==3) max=15;
    else if(show>3&&show<8) max=9;
    
    switch(*input) 
    {
        case KEY_UP:
        {
            if(choice==0&&buy==0)
            {
                (*select)--; 
                if(*select<=0) *select=0;
            }
            else if(choice==1&&buy==0)
            {
                (*select1)--;
                if(*select1<=0) *select1=0;
            }
            break;
        }
        case KEY_DOWN:
        {
            if(choice==0&&buy==0)
            {
                (*select)++;
                if(*select>=max) *select=max;
            }
            else if(choice==1&&buy==0)
            {
                (*select1)++;
                if(*select1>=1) *select1=1;
            }
            break;
        }
        case KEY_LEFT:
        {
            if((show>2&&show<8)&&choice==0&&buy==0)
            {
                (*select)-=3;
                if(*select<=0) *select=0;
            }
            break;
        }
        case KEY_RIGHT:
        {
            if((show>2&&show<8)&&choice==0&&buy==0)
            {
                (*select)+=3;
                if(*select>=max) *select=max;
            }
            break;
        }
    }
}

void SaveTEXT(char nameNprice[][100],int *codenum,int num,int select,int cnt)
{
    int code,i,j=0;
    int pricearr[cnt];
    char temp[cnt][100];

    //코드로 장비명과 가격을 포인터배열에 저장해주는 반복문
    for (i = 0; i < cnt; i++)   
    {
        code = i + codenum[num];   
        itemcode(temp[i], code);
        itemprice(&pricearr[i], code);
        if(select==i) sprintf(nameNprice[i],"> %s %5dG",temp[i],pricearr[i]);
        else sprintf(nameNprice[i]," %s %5dG",temp[i],pricearr[i]);
    } 
}

int InputNSpace(Stat *stat,int *select,int *select1,int *choice,int *cnt,int *show,int *num,int *buy)
{
    int chk=0;

    switch(*show)
    {
        //0:초기화면 1:장비종류선택 2:방어구부위선택창 3:무기 4:머리 5:몸통 6:신발 7:장갑 
        case 0:
        {
            if(*select==0) *show=1; //show0화면에서 거래로 들어가기 
            else stat->mode=0,*select=0; //show0화면에서 나가기
            break;
        }
        case 1: //장비종류 선택창
        {
            if(*select==0) *show=3,*cnt=15,*num=4; //show1화면에서 무기 선택시
            else if(*select==1) *show=2,*select=0; //show1화면에서 방어구 선택시
            else if(*select==2) *show=0,*select=0; //show1화면에서 뒤로가기
            break;
        }
        case 2: //방어구부위 선택창
        {
            if(*select==0) *show=4,*cnt=9,*num=0; //show7화면에서 머리 선택시
            else if(*select==1) *show=5,*select=0,*cnt=9,*num=1; //show7화면에서 몸통 선택시
            else if(*select==2) *show=6,*select=0,*cnt=9,*num=2; //show7화면에서 신발 선택시
            else if(*select==3) *show=7,*select=0,*cnt=9,*num=3; //show7화면에서 장갑 선택시
            else if(*select==4) *show=1,*select=0,*cnt=0; //방어구 목록 화면에서 뒤로가기 선택시
            break;
        }
        case 3: //무기
        case 4: //머리
        case 5: //몸통
        case 6: //신발
        case 7: //장갑
        {
            if(*buy!=0) *buy=0,*choice=0;
            else if(*select!=*cnt&&*choice==0) *choice=1,select=0; //장비 선택화면에서 장비를 선택했을시
            else if(*choice==1&&*select1==0) chk=1;//장비 구매 눌렀을시
            else if(*choice==1&&*select1==1) *choice=0,*select1=0;//구매선택창에서 뒤로가기 눌렀을시
            else if(*select==*cnt) 
            {
                if(*show==3) *show=1,*select=0,*cnt=0; //show3(무기)화면에서 뒤로가기 선택시
                else *show=2,*select=0,*cnt=0; //show4~7(방어구)화면에서 뒤로가기 선택시
            }
            break;
        }
    }

    return chk;
}

void CopyTYPE(char *type,int show)
{
    if(show==3) strcpy(type,"무기 목록"); //무기 타이틀
    else if(show==4) strcpy(type,"머리 목록"); //머리 타이틀
    else if(show==5) strcpy(type,"몸통 목록"); //몸통 타이틀
    else if(show==6) strcpy(type,"신발 목록"); //신발 타이틀
    else if(show==7) strcpy(type,"장갑 목록"); //장갑 타이틀
}

void CopyMENU(int show,char menu[][100],int select)
{
    switch(show)
    {
        case 0:
        {
            strcpy(menu[0],"[ 대  장  간 ]");
            if(select==0) strcpy(menu[1],"> 거래");
            else strcpy(menu[1]," 거래");
            if(select==0) strcpy(menu[2]," 나가기");
            else strcpy(menu[2],"> 나가기");
            break;
        }
        case 1:
        {
            strcpy(menu[0],"[ 거래 장비 선택 ]");
            if(select==0) strcpy(menu[1],"> 무기");
            else strcpy(menu[1]," 무기");
            if(select==1) strcpy(menu[2], "> 방어구");
            else strcpy(menu[2]," 방어구");
            if(select==2) strcpy(menu[3], "> 뒤로가기");
            else strcpy(menu[3]," 뒤로가기");
            break;
        }
        case 2:
        {
            strcpy(menu[0],"[ 장비 부위 선택 ]");
            if(select==0) strcpy(menu[1],"> 머리");
            else strcpy(menu[1]," 머리");
            if(select==1) strcpy(menu[2], "> 몸통");
            else strcpy(menu[2]," 몸통");
            if(select==2) strcpy(menu[3], "> 신발");
            else strcpy(menu[3]," 신발");
            if(select==3) strcpy(menu[4], "> 장갑");
            else strcpy(menu[4]," 장갑");
            if(select==4) strcpy(menu[5], "> 뒤로가기");
            else strcpy(menu[5]," 뒤로가기");
            break;
        }
    }
}