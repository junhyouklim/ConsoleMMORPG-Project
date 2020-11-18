#include "item.h"

typedef struct Stat
{
    int plnum;
    char name[8][20];
    int hp[8][2];
    int mp[8][2];
    int atk[2];
    int def;
    int mon;
    int con[8][3][2];
    int buf[8][5][2];
    int mode;
    int party[5];
    int boss_cnt;
} Stat;

typedef struct Item
{
    int equip[8];   //오른손 무기0, 왼손 무기1, 오른손 반지2, 왼손 반지3, 머리,4 몸통5, 장갑6, 신발7
    int inven[50][2];
    int skill[10];
    int slot[5];
} Item;

typedef struct Boss
{
    int hp[2];
    int atk[2];
    int def;
    int con[2];
    int drop_item[2];
    int dmg;
    int rdmg;
}Boss;

typedef struct Map
{
    int pmap[35][50];
    int plyx[2];
    int mapnum;
}Map;

typedef struct Chat
{
    int sig;
    char name_from[30];
    char name_to[30];
    char msg[100];
}Chat;

typedef struct P
{
    Stat stat;
    Item item;
}P;

typedef struct S
{
    Boss boss;
    Map map;
}S;

typedef struct C
{
    Chat chat;
}C;

typedef struct Total
{
    P p;
    S s;
    C c;
} Total;

typedef struct Item_name
{
    char weapon[17][30];
    char helm[10][30];
    char armor[10][30];
    char shoe[10][30];
    char glove[10][30];
    char ring[10][30];
    char use[6][30];
    char skill[22][30];
} Item_name;

typedef struct Item_price
{
    int weapon[17];
    int helm[10];
    int armor[10];
    int shoe[10];
    int glove[10];
    int ring[10];
    int use[6];
    int skill[22];
} Item_price;

//0:최대 HP 1:최소 HP 2:HP회복량 3:MP회복량 4:최소공격력 5:최대공격력 6:방어력
typedef struct Item_st
{
    int weapon[17][7];
    int helm[10][7];
    int armor[10][7];
    int shoe[10][7];
    int glove[10][7];
    int ring[10][7];
    int use[6][7];
    int skill[22][7];
} Item_st;

typedef struct
{
    Total tot;
    int input;
    int plr_x;
    int plr_y;
} Test;

///기반함수
int getkey(int is_echo, int timeout);
int getch(void);                //키보드 입력
void space(int num, char *alp); //문자 반복 생성함수
int randn(int min,int max);    //랜덤값 생성
//출력 함수
void prtstat(Stat stat);                                                                  //상단 스탯 출력
int prtside(Stat *stat, Item *item, int *map_mode, int *ctr_mode, int *input, int line);  //사이드바 통합 출력
int prtequip(Stat *stat, Item *item, int *map_mode, int *ctr_mode, int *input, int line); //장비창 출력
int prtinven(Stat *stat, Item *item, int *map_mode, int *ctr_mode, int *input, int line); //소지품창 출력
int prtskill(Stat *stat, Item *item, int *map_mode, int *ctr_mode, int *input, int line); //스킬창 출력
//스탯 관리 함수
//0:플레이어 번호 1:현재hp 2:최대hp 3:현재mp 4:최대mp 5:최소공격력 6:최대공격력 7:돈 8:방어력
//mode 0:고정값 1:퍼센트값
void ch_stat(Stat *stat, int code, int num, int mode);
//아이템 처리 함수
int item_get(Item *item, int code, int num);               //아이템 인벤토리에 추가
int item_del(Item *item, int code, int num);               //아이템 제거
int item_use(Stat *stat, Item *item, int code, int num);   //아이템 사용
int item_stat(Stat *stat, Item *item, int code, int mode); //아이템 장,탈착시 스탯 변화
int itemcode(char *str, int code);                         //아이템 이름 반환
int itemprice(int *price, int code);                       //아이템 가격 반환
int iteminf(char *str, int code, int line);                //아이템 설명 출력

//스킬 처리 함수
int skill_get(Item *item, int code); //스킬 배우기
int skill_del(Item *item, int code); //스킬 잊기
int skill_on(Item *item, int code);  //스킬 장착하기
int slot_del(Item *item, int code);  //슬롯 비우기

