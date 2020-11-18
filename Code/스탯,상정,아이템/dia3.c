#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include "dia3.h"

void prtstat(Stat stat)
{
    int num = 50,i;
    char str[30]={0};
    int my=stat.plnum;

    space(num, "=");
    printf("\n");
    printf(" %s  HP:%d/%d  MP:%d/%d   공격력:%d/%d  방어력: %d", stat.name[my], stat.hp[my][0], stat.hp[my][1], stat.mp[my][0], stat.mp[my][1], stat.atk[0], stat.atk[1], stat.def);
    printf("\n");
    for (i = 0; i < 3; i++)
    {
        if (stat.con[my][i][0] != 0)
        {
            itemcode(str, stat.con[my][i][0]);
            printf("☣:%s-%d턴   ", str, stat.con[my][i][1]);
        }
    }
    for (i = 0; i < 5; i++)
    {
        if (stat.buf[my][i][0] != 0)
        {
            itemcode(str, stat.buf[my][i][0]);
            printf("🦾:%s-%d턴   ", str, stat.buf[my][i][1]);
        }
    }
    printf("\n");
    space(num, "=");
    printf("\n");
}

int prtside(Stat *stat, Item *item, int *map_mode, int *ctr_mode, int *input, int line)
{
    
    prtequip(stat, item, map_mode, ctr_mode, input, line);
    prtinven(stat, item, map_mode, ctr_mode, input, line);
    prtskill(stat, item, map_mode, ctr_mode, input, line);
    if(*ctr_mode>=2 && *ctr_mode<=4)
    {
        *input = 0;
    }
}

int prtequip(Stat *stat, Item *item, int *map_mode, int *ctr_mode, int *input, int line)
{
    int i = 0;
    int temp = 0;
    int wlen = 0, wlen2=0;
    int rlen = 0, rlen2=0;
    int cnt = 0;
    char str[30];
    char info[200]={0};
    static int curser = 0, sel_curser = 0;
    static int sel = 0;
    int num ,num2=25;
    static int show_mode=0;

    if (*input == KEY_U || *input == KEY_u)
    {
        switch (show_mode)
        {
        case 0:
            show_mode = 1;
            *ctr_mode = 2;
            break;
        case 1:
            show_mode = 0;
            *ctr_mode = 0;
            *input=0;
            break;
        }
    }
    else if((*input == KEY_I || *input == KEY_i) && show_mode==1)
    {
        show_mode=0;
    }
    else if((*input == KEY_K || *input == KEY_k) && show_mode==1)
    {
        show_mode=0;
    }
    else if(*input == KEY_TAB)
    {
        *ctr_mode=0;
        *input=0;
    }
    if(show_mode==0)
    {
        return 0;
    }

    if (*ctr_mode == 2)
    {
        if (sel == 0) //장비템 선택 모드
        {
            switch (*input)
            {
            case KEY_DOWN:
                if (curser <= 2)
                {
                    curser += 2;
                }
                else if (curser < 7)
                {
                    curser++;
                }
                break;

            case KEY_UP:
                if (curser <= 4 && curser > 1)
                {
                    curser -= 2;
                }
                else if (curser > 0)
                {
                    curser--;
                }
                break;

            case KEY_RIGHT:
                if (curser < 7)
                {
                    curser++;
                }
                break;

            case KEY_LEFT:
                if (curser > 0)
                {
                    curser--;
                }
                break;
            case KEY_SPACE:
            case KEY_ENTER:
                if(item->equip[curser]!=0)
                    sel = 1;
                break;
            }
        }
        else //선택된 장비템 메뉴 모드
        {
            switch (*input)
            {
            case KEY_DOWN:
            case KEY_RIGHT:
                if (sel_curser < 2)
                {
                    sel_curser++;
                }
                break;

            case KEY_UP:
            case KEY_LEFT:
                if (sel_curser > 0)
                {
                    sel_curser--;
                }
                break;

            case KEY_SPACE:
            case KEY_ENTER:
                if (sel_curser == 0) //장비템 해제
                {
                    temp = item_get(item, item->equip[curser], 1);
                    if (temp != -1)
                    {
                        if (item->equip[curser] >= 504 && item->equip[curser] <= 512)
                        {
                            item_stat(stat,item, item->equip[0],1);
                            item->equip[0] = 0;
                            item->equip[1] = 0;
                        }
                        else
                        {
                            item_stat(stat,item, item->equip[curser],1);
                            item->equip[curser] = 0;
                        }
                    }
                    sel = 0;
                    sel_curser = 0;
                }
                else if (sel_curser == 1) //장비템 버림
                {
                    item->equip[curser] = 0;
                }
                sel = 0;
                break;
            case KEY_ESC:
                sel = 0;
                break;
            }
        }
    }

    /* if(*ctr_mode==2)
    {
        *input=0;
    } */
    ////오른손 무기 이름 길이
    temp = item->equip[0];
    if (temp == 0)
    {
        wlen = 4;
    }
    else
    {
        itemcode(str, temp);
        wlen = strlen(str);
        for (i = 0; i < wlen; i++)
        {
            if (str[i] < 0)
            {
                cnt++;
            }
        }
        if (cnt != 0)
        {
            wlen -= (cnt / 3);
        }
    }
    cnt = 0;
    ////왼손 무기 이름 길이
    temp = item->equip[1];
    if (temp == 0)
    {
        wlen2 = 4;
    }
    else
    {
        itemcode(str, temp);
        wlen2 = strlen(str);
        for (i = 0; i < wlen2; i++)
        {
            if (str[i] < 0)
            {
                cnt++;
            }
        }
        if (cnt != 0)
        {
            wlen2 -= (cnt / 3);
        }
    }
    cnt = 0;
    ////오른손 반지 이름 길이
    temp = item->equip[2];
    if (temp == 0)
    {
        rlen = 4;
    }
    else
    {
        itemcode(str, temp);
        rlen = strlen(str);
        for (i = 0; i < rlen; i++)
        {
            if (str[i] < 0)
            {
                cnt++;
            }
        }
        if (cnt != 0)
        {
            rlen -= (cnt / 3);
        }
    }
    cnt = 0;
    //왼손 반지 이름 길이
    temp = item->equip[3];
    if (temp == 0)
    {
        rlen2 = 4;
    }
    else
    {
        itemcode(str, temp);
        rlen2 = strlen(str);
        for (i = 0; i < rlen2; i++)
        {
            if (str[i] < 0)
            {
                cnt++;
            }
        }
        if (cnt != 0)
        {
            rlen2 -= (cnt / 3);
        }
    }
    cnt = 0;
    ////이름 길이에 따라 창 크기 결정
    num=rlen+rlen2;
    if(num<(wlen+wlen2))
    {
        num=wlen+wlen2;
    }
    num+=num2;

    switch (line)
    {
    case 0:
        printf(" 장비창");
        break;
    case 1:
        space(num, "=");
        break;
    case 2: //무기 출력
        temp = item->equip[0];
        itemcode(str, temp);
        if (curser == 0)
        {
            printf(">오른손 무기:");
        }
        else
        {
            printf(" 오른손 무기:");
        }
        if (temp != 0)
        {
            printf("%s ", str);
        }
        else
        {
            printf("없음 ");
        }

        temp = item->equip[1];
        itemcode(str, temp);
        space(rlen - wlen, " ");
        if (curser == 1)
        {
            printf(">왼손 무기:");
        }
        else
        {
            printf(" 왼손 무기:");
        }
        if (temp != 0)
        {
            printf("%s", str);
        }
        else
        {
            printf("없음");
        }
        break;
    case 3: //반지 출력
        if (curser == 2)
        {
            printf(">오른손 반지:");
        }
        else
        {
            printf(" 오른손 반지:");
        }
        temp = item->equip[2];
        itemcode(str, temp);
        if (temp != 0)
        {
            printf("%s ", str);
        }
        else
        {
            printf("없음 ");
        }

        temp = item->equip[3];
        space(wlen - rlen, " ");
        itemcode(str, temp);
        if (curser == 3)
        {
            printf(">왼손 반지:");
        }
        else
        {
            printf(" 왼손 반지:");
        }
        if (temp != 0)
        {
            printf("%s", str);
        }
        else
        {
            printf("없음");
        }
        break;
    case 4: //머리 출력
        temp = item->equip[4];
        itemcode(str, temp);
        if (curser == 4)
        {
            printf(">머리:");
        }
        else
        {
            printf(" 머리:");
        }
        if (temp != 0)
        {
            printf("%s", str);
        }
        else
        {
            printf("없음");
        }
        break;
    case 5: //몸통 출력
        temp = item->equip[5];
        itemcode(str, temp);
        if (curser == 5)
        {
            printf(">몸통:");
        }
        else
        {
            printf(" 몸통:");
        }
        if (temp != 0)
        {
            printf("%s", str);
        }
        else
        {
            printf("없음");
        }
        break;
    case 6: //장갑 출력
        temp = item->equip[6];
        itemcode(str, temp);

        if (curser == 6)
        {
            printf(">장갑:");
        }
        else
        {
            printf(" 장갑:");
        }
        if (temp != 0)
        {
            printf("%s", str);
        }
        else
        {
            printf("없음");
        }
        break;
    case 7: //신발 출력
        temp = item->equip[7];
        itemcode(str, temp);
        if (curser == 7)
        {
            printf(">신발:");
        }
        else
        {
            printf(" 신발:");
        }
        if (temp != 0)
        {
            printf("%s", str);
        }
        else
        {
            printf("없음");
        }
        break;
    case 9:
        printf(" 소지금:%dG", stat->mon);
        break;
    case 10:
        space(num, "=");
        break;
    case 11:
        iteminf(info, item->equip[curser], 0);
        printf("%s", info);
        break;
    case 12:
        iteminf(info, item->equip[curser], 1);
        printf("%s", info);
        break;
    case 14:
        if (sel == 1)
        {
            if (sel_curser == 0)
            {
                printf(">");
            }
            else
            {
                printf(" ");
            }
            printf("해제하기");
        }
        break;
    case 15:
        if (sel == 1)
        {
            if (sel_curser == 1)
            {
                printf(">");
            }
            else
            {
                printf(" ");
            }
            printf("버리기");
        }
        break;
    case 16:
        if (sel == 1)
        {
            if (sel_curser == 2)
            {
                printf(">");
            }
            else
            {
                printf(" ");
            }
            printf("취소");
        }
        break;
    default:
        break;
    }
}

int prtinven(Stat *stat, Item *item, int *map_mode, int *ctr_mode, int *input, int line)
{
    int i,j;
    int temp = line - 2;
    int cnt=0;
    int temp_code = 0;
    char str[30], str2[30];
    char info[200]={0};
    static int sel = 0;
    static int curser = 0, sel_curser = 0;
    int num2 = 22;  //갯수, 소지금 정렬
    int num3 = 8;   //정렬 사이 거리
    int num = num3+3;   //경계선 길이
    int price = 0;
    int max = 0;
    int max_line = 0;
    int item_num = 0;
    static int show_mode=0;

    if (*input == KEY_I || *input == KEY_i)
    {
        switch (show_mode)
        {
        case 0:
            show_mode = 1;
            *ctr_mode = 3;
            break;
        case 1:
            show_mode = 0;
            *ctr_mode = 0;
            *input = 0;

            break;
        }
    }
    else if ((*input == KEY_K || *input == KEY_k) && show_mode == 1)
    {
        show_mode = 0;
    }
    else if ((*input == KEY_U || *input == KEY_u) && show_mode == 1)
    {
        show_mode = 0;
    }
    else if (*input == KEY_TAB)
    {
        *ctr_mode = 0;
        *input = 0;
    }
    if (show_mode == 0)
    {
        return 0;
    }

    for (i = 0; i < 50; i++)
    {
        if (i % 2 == 0)
        {
            itemcode(str, item->inven[i][0]);
            itemcode(str2, item->inven[i + 1][0]);
            if (strlen(str) + strlen(str2) > max_line)
            {
                max_line = strlen(str) + strlen(str2);
            }
        }
        for (j = 0; j < strlen(str); j++)
        {
            if (str[j] & 0x80)
            {
                cnt += 1;
            }
        }
        cnt = strlen(str) - cnt / 3;
        if (item->inven[i][0] > 700)
        {
            cnt += 3;
        }
        if (cnt > max)
        {
            max = cnt;
        }
        if (item->inven[i][0] == 0)
            break;
    }
    i--;
    if (curser > i)
    {
        curser = 0;
    }
    num+=max_line;

    if (*ctr_mode == 3)
    {
        if (sel == 0) //소지품 선택 모드
        {
            switch (*input)
            {
            case KEY_DOWN:
                if (curser < i - 1)
                {
                    curser += 2;
                }
                else if (curser < i)
                {
                    curser++;
                }
                break;

            case KEY_UP:
                if (curser > 1)
                {
                    curser -= 2;
                }
                else if (curser > 0)
                {
                    curser--;
                }
                break;

            case KEY_RIGHT:
                if (curser < i)
                {
                    curser++;
                }
                break;

            case KEY_LEFT:
                if (curser > 0)
                {
                    curser--;
                }
                break;
            case KEY_SPACE:
            case KEY_ENTER:
                if (item->inven[curser][0] != 0)
                {
                    sel = 1;
                    break;
                }
            }
        }
        else //선택된 소지품 메뉴 모드
        {
            switch (*input)
            {
            case KEY_DOWN:
            case KEY_RIGHT:
                if (sel_curser < 2)
                {
                    sel_curser++;
                }
                break;

            case KEY_UP:
            case KEY_LEFT:
                if (sel_curser > 0)
                {
                    sel_curser--;
                }
                break;

            case KEY_SPACE:
            case KEY_ENTER:
                if (sel_curser == 0) //아이템 사용
                {
                    temp_code = item->inven[curser][0];
                    if (item->inven[curser][1] == 1) //해당 아이템 사용시 사라질 수 있는경우
                    {
                        sel = 0;
                        sel_curser = 0;
                    }
                    temp = item_use(stat, item, item->inven[curser][0], 1);
                    if (temp != -1) //아이템이 제대로 사용되지 못한 경우
                    {
                        sel = 0;
                        sel_curser = 0;
                    }
                }
                else if (sel_curser == 1) //아이템 판매, 버림
                {
                    if (item->inven[curser][0] < 100 && *map_mode == 3) //잡화점
                    {
                        itemprice(&price, item->inven[curser][0]);
                        item_num = item->inven[curser][1];
                        temp = item_del(item, item->inven[curser][0], 1);
                        if (temp != -1)
                        {
                            ch_stat(stat, 7, price / 4, 0);
                            if (item_num == 1)
                            {
                                sel = 0;
                            }
                        }
                    }
                    else if (item->inven[curser][0] > 100 && item->inven[curser][0] < 600 && *map_mode == 2)    //대장간
                    {
                        itemprice(&price, item->inven[curser][0]);
                        item_num = item->inven[curser][1];

                        temp = item_del(item, item->inven[curser][0], 1);
                        if (temp != -1)
                        {
                            ch_stat(stat, 7, price / 4, 0);
                            if (item_num == 1)
                            {
                                sel = 0;
                            }
                        }
                    }
                    else if (item->inven[curser][0] > 600 && item->inven[curser][0] < 700 && *map_mode == 3)    //잡화점
                    {
                        itemprice(&price, item->inven[curser][0]);
                        item_num = item->inven[curser][1];

                        temp = item_del(item, item->inven[curser][0], 1);
                        if (temp != -1)
                        {
                            ch_stat(stat, 7, price / 4, 0);
                            if (item_num == 1)
                            {
                                sel = 0;
                            }
                        }
                    }
                    else if (item->inven[curser][0] > 700 && *map_mode == 4)    //스킬상점
                    {
                        itemprice(&price, item->inven[curser][0]);
                        item_num = item->inven[curser][1];

                        temp = item_del(item, item->inven[curser][0], 1);
                        if (temp != -1)
                        {
                            ch_stat(stat, 7, price / 4, 0);
                            if (item_num == 1)
                            {
                                sel = 0;
                            }
                        }
                    }
                    else
                    {
                        temp = item_del(item, item->inven[curser][0], item->inven[curser][1]);
                        sel = 0;
                        sel_curser = 0;
                    }
                }
                else //취소
                {
                    sel = 0;
                    sel_curser = 0;
                }
                break;

            case KEY_ESC:
                sel = 0;
                break;
            }
        }
    }

    cnt=0;
    switch (line)
    {
    case 0:
        printf(" 소지품");
        break;

    case 1:
        space(num, "=");
        break;
    }
    temp = (line - 2) * 2;
    if (line >= 2 && temp <= i)
    {
        if (item->inven[temp][0] != 0)
        {
            itemcode(str, item->inven[temp][0]);
            if (curser == temp)
            {
                printf(">");
            }
            else
            {
                printf(" ");
            }
            if (item->inven[temp][0] > 700)
            {
                printf("%s 책 %d개", str, item->inven[temp][1]);
            }
            else
            {
                printf("%s %d개", str, item->inven[temp][1]);
            }
            for (j = 0; j < strlen(str); j++)
            {
                if (str[j] & 0x80)
                {
                    cnt += 1;
                }
            }
            cnt = strlen(str) - cnt / 3;
            if(item->inven[temp][0]>700)
            {
                cnt+=3;
            }
            space(num3 + (max - cnt), " ");
        }
        if (item->inven[temp + 1][0] != 0)
        {
            itemcode(str, item->inven[temp + 1][0]);
            if (curser == temp+1)
            {
                printf(">");
            }
            else
            {
                printf(" ");
            }
            if (item->inven[temp+1][0] > 700)
            {
                printf("%s 책 %d개", str, item->inven[temp+1][1]);
            }
            else
            {
                printf("%s %d개", str, item->inven[temp+1][1]);
            }
        }
    }
    if (line == i / 2 + 4)
    {
        //space(num3*5, " ");
        printf(" 갯수:%2d/50", i + 1);
    }
    if (line == i / 2 + 5)
    {
        //space(num3*5, " ");
        printf(" 소지금:%dG", stat->mon);
    }
    if (line == i / 2 + 6)
    {
        space(num, "=");
    }
    if (line == i / 2 + 7)
    {
        iteminf(info, item->inven[curser][0], 0);
        printf("%s", info);
    }
    if (line == i / 2 + 8)
    {
        iteminf(info, item->inven[curser][0], 1);
        printf("%s", info);
    }
    if (line == i / 2 + 10)
    {
        if (sel == 1)
        {
            if (item->inven[curser][0] > 100 && item->inven[curser][0]<700)
            {
                if (sel_curser == 0)
                {
                    printf(">");
                }
                else
                {
                    printf(" ");
                }
                printf("장비하기");
            }
            else if (item->inven[curser][0] < 100 || item->inven[curser][0]>700)
            {
                if (sel_curser == 0)
                {
                    printf(">");
                }
                else
                {
                    printf(" ");
                }
                printf("사용하기");
            }
        }
    }
    if (line == i / 2 + 11)
    {
        if (sel == 1)
        {
            if (sel_curser == 1)
            {
                printf(">");
            }
            else
            {
                printf(" ");
            }
            if (item->inven[curser][0] < 100 && *map_mode == 3)
            {
                printf("판매하기");
            }
            else if (item->inven[curser][0] > 100 && item->inven[curser][0] < 600 && *map_mode == 2)
            {
                printf("판매하기");
            }
            else if (item->inven[curser][0] > 600 && item->inven[curser][0] < 700 && *map_mode == 3)
            {
                printf("판매하기");
            }
            else if (item->inven[curser][0] > 700 && *map_mode == 4)
            {
                printf("판매하기");
            }
            else
            {
                printf("버리기");
            }
        }
    }
    if (line == i / 2 + 12)
    {
        if (sel == 1)
        {
            if (sel_curser == 2)
            {
                printf(">");
            }
            else
            {
                printf(" ");
            }
            printf("취소");
        }
    }
}

int prtskill(Stat *stat, Item *item, int *map_mode, int *ctr_mode, int *input, int line)
{
    int i = 0, j = 0, k = 0;
    int temp = 0;
    char str[30] = {0};
    static int curser = 0;
    static int sel = 0;
    static int sel_curser = 0;
    static int mode = 0;
    int num = 30, num2 = 5;
    int max = 0;
    static int show_mode = 0;
    char info[200]={0};

    if(*input == KEY_K || *input == KEY_k)
    {
        switch (show_mode)
        {
        case 0:
            show_mode = 1;
            *ctr_mode = 4;
            break;
        case 1:
            show_mode = 0;
            *ctr_mode = 0;
            *input = 0;
            break;
        }
    }
    else if((*input == KEY_I || *input == KEY_i) && show_mode==1)
    {
        show_mode=0;
    }
    else if((*input == KEY_U || *input == KEY_u) && show_mode==1)
    {
        show_mode = 0;
    }
    else if (*input == KEY_TAB)
    {
        *ctr_mode = 0;
        *input = 0;
    }
    if(show_mode==0)
    {
        return 0;
    }

    for (i = 0; i < 10; i++)
    {
        itemcode(str, item->skill[i]);
        if (strlen(str) > max)
        {
            max = strlen(str);
        }
        if (item->skill[i] == 0)
            break;
    }
    i--;
    if (curser > i && mode == 0)
    {
        curser = i;
    }

    for (j = 0; j < 5; j++)
    {
        if (item->slot[j] == 0)
            break;
    }
    j--;
    if (mode == 1)
    {
        if (curser > j)
        {
            curser = j;
        }
        if (sel_curser > 2)
        {
            sel_curser = 1;
        }
    }

    if (*ctr_mode == 4)
    {
        if (sel == 0) //스킬 선택 모드
        {
            switch (*input)
            {
            case KEY_DOWN:
                if (curser < i)
                {
                    curser++;
                }
                break;

            case KEY_UP:
                if (curser > 0)
                {
                    curser--;
                }
                break;

            case KEY_RIGHT:
                if (j >= 0)
                {
                    mode = 1;
                }
                break;
            case KEY_LEFT:
                mode = 0;
                break;
            case KEY_SPACE:
            case KEY_ENTER:
                if (i >= 0)
                {
                    if (mode == 0 && item->skill[curser] != 0)
                    {
                        sel = 1;
                    }
                    else if (mode == 1 && item->slot[curser] != 0)
                    {
                        sel = 1;
                    }
                }
                break;
            }
        }
        else //선택된 스킬 모드
        {
            switch (*input)
            {
            case KEY_DOWN:
            case KEY_RIGHT:
                if (mode == 0)
                {
                    if (sel_curser < 2)
                    {
                        sel_curser++;
                    }
                }
                else
                {
                    if (sel_curser < 1)
                    {
                        sel_curser++;
                    }
                }
                break;
            case KEY_UP:
            case KEY_LEFT:
                if (sel_curser > 0)
                {
                    sel_curser--;
                }
                break;
            case KEY_SPACE:
            case KEY_ENTER:
                if (sel_curser == 0) //스킬 장착 or 슬롯 비우기
                {
                    if (mode == 0)
                    {
                        skill_on(item, item->skill[curser]);
                    }
                    else
                    {
                        slot_del(item, item->slot[curser]);
                        if (j == 0)
                        {
                            mode = 0;
                        }
                    }
                }
                else if (sel_curser == 1) //스킬 잊기 or 슬롯 선택 취소
                {
                    if (mode == 0)
                    {
                        skill_del(item, item->skill[curser]);
                    }
                    else
                    {
                        sel = 0;
                    }
                }
                sel = 0;
                sel_curser = 0;
                break;
            case KEY_ESC:
                sel = 0;
                sel_curser = 0;
                break;
            }
        }
    }
    /* if(*ctr_mode==4)
    {
        *input=0;
    } */

    switch (line)
    {
    case 0:
        printf(" 스킬창");
        break;
    case 1:
        space(num, "=");
        break;
    }
    if (line > 1 && line - 2 <= i)
    {
        if (mode == 0 && curser == line - 2)
        {
            printf(">");
        }
        else
        {
            printf(" ");
        }
        itemcode(str, item->skill[line - 2]);
        printf("%s", str);
        for (k = 0; k < strlen(str); k++)
        {
            if (str[k] < 0)
            {
                temp += 1;
            }
        }
        temp = strlen(str) - temp / 3;
        space(num2 + (max - temp), " ");
        if (mode == 1 && curser == line - 2)
        {
            printf(">");
        }
        else
        {
            printf(" ");
        }
        if (line - 2 <= j)
        {
            itemcode(str, item->slot[line - 2]);
            printf("%s", str);
        }
    }
    if (line == i + 4)
    {
        printf("스킬:%2d/10", i + 1);
        space(num - 21, " ");
        printf("슬롯:%d/5", j + 1);
    }
    if (line == i + 5)
    {
        space(num, "=");
    }
    if (line == i + 6)
    {
        if (mode == 0)
        {
            iteminf(info, item->skill[curser], 0);
            printf("%s", info);
        }
        if (mode == 1)
        {
            iteminf(info, item->slot[curser], 0);
            printf("%s", info);
        }
    }
    if (line == i + 7)
    {
        if (mode == 0)
        {
            iteminf(info, item->skill[curser], 1);
            printf("%s", info);
        }
        if (mode == 1)
        {
            iteminf(info, item->slot[curser], 1);
            printf("%s", info);
        }
    }
    if (sel == 1)
    {
        if (mode == 0)
        {
            if (line == i + 9)
            {
                if (sel_curser == 0)
                {
                    printf(">");
                }
                else
                {
                    printf(" ");
                }
                printf("장착하기");
            }
            if (line == i + 10)
            {
                if (sel_curser == 1)
                {
                    printf(">");
                }
                else
                {
                    printf(" ");
                }
                printf("잊기");
            }
            if (line == i + 11)
            {
                if (sel_curser == 2)
                {
                    printf(">");
                }
                else
                {
                    printf(" ");
                }
                printf("취소");
            }
        }
        else
        {
            if (line == i + 9)
            {
                if (sel_curser == 0)
                {
                    printf(">");
                }
                else
                {
                    printf(" ");
                }
                printf("해제하기");
            }
            if (line == i + 10)
            {
                if (sel_curser == 1)
                {
                    printf(">");
                }
                else
                {
                    printf(" ");
                }
                printf("취소");
            }
        }
    }
}

void ch_stat(Stat *stat, int code, int num, int mode)
{
    int my = stat->plnum;
    double temp;

    switch (code)
    {
    case 0: //플레이어 번호
        stat->plnum = num;
        break;
    case 1: //현재 체력
        if (mode == 0)
        {
            stat->hp[my][0] += num;
        }
        else if (mode == 1)
        {
            stat->hp[my][0] += (stat->hp[my][1]) * num / 100;
        }
        if (stat->hp[my][0] > stat->hp[my][1])
        {
            stat->hp[my][0] = stat->hp[my][1];
        }
        if (stat->hp[my][0] < 0)
        {
            stat->hp[my][0] = 0;
        }
        break;
    case 2: //최대 체력
        if (mode == 0)
        {
            stat->hp[my][1] += num;
        }
        else if (mode == 1)
        {
            stat->hp[my][1] += (stat->hp[my][1]) * num / 100;
        }
        if (stat->hp[my][1] < 0)
            stat->hp[my][1] = 1;
        break;
    case 3: //현재 마나
        if (mode == 0)
        {
            stat->mp[my][0] += num;
        }
        else if (mode == 1)
        {
            stat->mp[my][0] += (stat->mp[my][1]) * num / 100;
        }
        if (stat->mp[my][0] > stat->mp[my][1])
        {
            stat->mp[my][0] = stat->mp[my][1];
        }
        if (stat->mp[my][0] < 0)
        {
            stat->mp[my][0] = 0;
        }
        break;
    case 4: //최대 마나
        if (mode == 0)
        {
            stat->mp[my][1] += num;
        }
        else if (mode == 1)
        {
            stat->mp[my][1] += (stat->mp[my][1]) * num / 100;
        }
        if (stat->mp[my][1] < 0)
            stat->mp[my][1] = 1;
        break;
    case 5: //최소 공격력
        if (mode == 0)
        {
            stat->atk[0] += num;
        }
        else if (mode == 1)
        {
            stat->atk[0] += (stat->atk[0]) * num / 100;
        }
        if (stat->atk[0] > stat->atk[1])
        {
            stat->atk[0] = stat->atk[1];
        }
        if (stat->atk[0] < 0)
        {
            stat->atk[0] = 0;
        }
        break;
    case 6: //최대 공격력
        if (mode == 0)
        {
            stat->atk[1] += num;
        }
        else if (mode == 1)
        {
            stat->atk[1] += (stat->atk[1]) * num / 100;
        }
        if (stat->atk[1] < 0)
            stat->atk[1] = 1;
        break;
    case 7: //돈
        if (mode == 0)
        {
            stat->mon += num;
        }
        else if (mode == 1)
        {
            stat->mon += (stat->mon) * num / 100;
        }
        if (stat->mon < 0)
        {
            stat->mon = 0;
        }
        break;
    case 8: //방어력
        if (mode == 0)
        {
            stat->def += num;
        }
        else if (mode == 1)
        {
            stat->def += (stat->def) * num / 100;
        }
        if (stat->def < 0)
        {
            stat->def = 0;
        }

    }
}

int itemcode(char *str, int code)
{
    Item_name item_name = {{ITEM_501_NAME, ITEM_502_NAME, ITEM_503_NAME, ITEM_504_NAME, ITEM_505_NAME, ITEM_506_NAME, ITEM_507_NAME, ITEM_508_NAME, ITEM_509_NAME, ITEM_510_NAME, ITEM_511_NAME, ITEM_512_NAME, ITEM_513_NAME, ITEM_514_NAME, ITEM_515_NAME, ITEM_516_NAME, ITEM_517_NAME},
                           {ITEM_101_NAME, ITEM_102_NAME, ITEM_103_NAME, ITEM_104_NAME, ITEM_105_NAME, ITEM_106_NAME, ITEM_107_NAME, ITEM_108_NAME, ITEM_109_NAME, ITEM_110_NAME},
                           {ITEM_201_NAME, ITEM_202_NAME, ITEM_203_NAME, ITEM_204_NAME, ITEM_205_NAME, ITEM_206_NAME, ITEM_207_NAME, ITEM_208_NAME, ITEM_209_NAME, ITEM_210_NAME},
                           {ITEM_301_NAME, ITEM_302_NAME, ITEM_303_NAME, ITEM_304_NAME, ITEM_305_NAME, ITEM_306_NAME, ITEM_307_NAME, ITEM_308_NAME, ITEM_309_NAME, ITEM_310_NAME},
                           {ITEM_401_NAME, ITEM_402_NAME, ITEM_403_NAME, ITEM_404_NAME, ITEM_405_NAME, ITEM_406_NAME, ITEM_407_NAME, ITEM_408_NAME, ITEM_409_NAME, ITEM_410_NAME},
                           {ITEM_601_NAME, ITEM_602_NAME, ITEM_603_NAME, ITEM_604_NAME, ITEM_605_NAME, ITEM_606_NAME, ITEM_607_NAME, ITEM_608_NAME, ITEM_609_NAME, ITEM_610_NAME},
                           {ITEM_001_NAME, ITEM_002_NAME, ITEM_003_NAME, ITEM_004_NAME, ITEM_005_NAME, ITEM_006_NAME},
                           {SKILL_701_NAME, SKILL_702_NAME, SKILL_703_NAME, SKILL_704_NAME, SKILL_705_NAME, SKILL_706_NAME, SKILL_707_NAME, SKILL_708_NAME, SKILL_709_NAME, SKILL_710_NAME, SKILL_711_NAME, SKILL_712_NAME, SKILL_713_NAME, SKILL_714_NAME, SKILL_715_NAME, SKILL_716_NAME, SKILL_717_NAME, SKILL_718_NAME, SKILL_719_NAME, SKILL_720_NAME, SKILL_721_NAME, SKILL_722_NAME}};
    if (code == 0)
    {
        return 0;
    }
    else if (code < 100) //소비아이템
    {
        code = code - 1;
        strcpy(str, item_name.use[code]);
        return 0;
    }
    else if (code < 200) //머리
    {
        code = code % 100 - 1;
        strcpy(str, item_name.helm[code]);
        return 0;
    }
    else if (code < 300) //갑옷
    {
        code = code % 200 - 1;
        strcpy(str, item_name.armor[code]);
        return 0;
    }
    else if (code < 400) //신발
    {
        code = code % 300 - 1;
        strcpy(str, item_name.shoe[code]);
        return 0;
    }
    else if (code < 500) //장갑
    {
        code = code % 400 - 1;
        strcpy(str, item_name.glove[code]);
        return 0;
    }
    else if (code < 600) //무기
    {
        code = code % 500 - 1;
        strcpy(str, item_name.weapon[code]);
        return 0;
    }
    else if (code < 700) //반지
    {
        code = code % 600 - 1;
        strcpy(str, item_name.ring[code]);
        return 0;
    }
    else if (code < 800) //스킬
    {
        code = code % 700 - 1;
        strcpy(str, item_name.skill[code]);
        return 0;
    }

    return -1;
}

int itemprice(int *price, int code)
{
    Item_price item_price = {{ITEM_501_PRI, ITEM_502_PRI, ITEM_503_PRI, ITEM_504_PRI, ITEM_505_PRI, ITEM_506_PRI, ITEM_507_PRI, ITEM_508_PRI, ITEM_509_PRI, ITEM_510_PRI, ITEM_511_PRI, ITEM_512_PRI, ITEM_513_PRI, ITEM_514_PRI, ITEM_515_PRI, ITEM_516_PRI, ITEM_517_PRI},
                             {ITEM_101_PRI, ITEM_102_PRI, ITEM_103_PRI, ITEM_104_PRI, ITEM_105_PRI, ITEM_106_PRI, ITEM_107_PRI, ITEM_108_PRI, ITEM_109_PRI, ITEM_110_PRI},
                             {ITEM_201_PRI, ITEM_202_PRI, ITEM_203_PRI, ITEM_204_PRI, ITEM_205_PRI, ITEM_206_PRI, ITEM_207_PRI, ITEM_208_PRI, ITEM_209_PRI, ITEM_210_PRI},
                             {ITEM_301_PRI, ITEM_302_PRI, ITEM_303_PRI, ITEM_304_PRI, ITEM_305_PRI, ITEM_306_PRI, ITEM_307_PRI, ITEM_308_PRI, ITEM_309_PRI, ITEM_310_PRI},
                             {ITEM_401_PRI, ITEM_402_PRI, ITEM_403_PRI, ITEM_404_PRI, ITEM_405_PRI, ITEM_406_PRI, ITEM_407_PRI, ITEM_408_PRI, ITEM_409_PRI, ITEM_410_PRI},
                             {ITEM_601_PRI, ITEM_602_PRI, ITEM_603_PRI, ITEM_604_PRI, ITEM_605_PRI, ITEM_606_PRI, ITEM_607_PRI, ITEM_608_PRI, ITEM_609_PRI, ITEM_610_PRI},
                             {ITEM_001_PRI, ITEM_002_PRI, ITEM_003_PRI, ITEM_004_PRI, ITEM_005_PRI, ITEM_006_PRI},
                             {SKILL_701_PRI, SKILL_702_PRI, SKILL_703_PRI, SKILL_704_PRI, SKILL_705_PRI, SKILL_706_PRI, SKILL_707_PRI, SKILL_708_PRI, SKILL_709_PRI, SKILL_710_PRI, SKILL_711_PRI, SKILL_712_PRI, SKILL_713_PRI, SKILL_714_PRI, SKILL_715_PRI, SKILL_716_PRI, SKILL_717_PRI, SKILL_718_PRI, SKILL_719_PRI, SKILL_720_PRI, SKILL_721_PRI, SKILL_722_PRI}};
    //char temp[30]={0};
    if (code == 0)
    {
        return 0;
    }
    else if (code < 100) //소비아이템
    {
        code = code - 1;
        *price = item_price.use[code];
        return 0;
    }
    else if (code < 200) //머리
    {
        code = code % 100 - 1;
        *price = item_price.helm[code];
        return 0;
    }
    else if (code < 300) //갑옷
    {
        code = code % 200 - 1;
        *price = item_price.armor[code];
        return 0;
    }
    else if (code < 400) //신발
    {
        code = code % 300 - 1;
        *price = item_price.shoe[code];
        return 0;
    }
    else if (code < 500) //장갑
    {
        code = code % 400 - 1;
        *price = item_price.glove[code];
        return 0;
    }
    else if (code < 600) //무기
    {
        code = code % 500 - 1;
        *price = item_price.weapon[code];
        return 0;
    }
    else if (code < 700) //반지
    {
        code = code % 600 - 1;
        *price = item_price.ring[code];
        return 0;
    }
    else if (code < 800) //스킬
    {
        code = code % 700 - 1;
        *price = item_price.skill[code];
        return 0;
    }

    return -1;
}

int iteminf(char *str ,int code, int line)
{
    switch (code)
    {
    case 1:
        if (line == 0)
        {
            sprintf(str, ITEM_001_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_001_INF2);
        }
        break;
    case 2:
        if (line == 0)
        {
            sprintf(str, ITEM_002_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_002_INF2);
        }
        break;
    case 3:
        if (line == 0)
        {
            sprintf(str, ITEM_003_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_003_INF2);
        }
        break;
    case 4:
        if (line == 0)
        {
            sprintf(str, ITEM_004_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_004_INF2);
        }
        break;
    case 5:
        if (line == 0)
        {
            sprintf(str, ITEM_005_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_005_INF2);
        }
        break;
    case 6:
        if (line == 0)
        {
            sprintf(str, ITEM_006_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_006_INF2);
        }
        break;
    case 101:
        if (line == 0)
        {
            sprintf(str, ITEM_101_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_101_INF2);
        }
        break;
    case 102:
        if (line == 0)
        {
            sprintf(str, ITEM_102_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_102_INF2);
        }
        break;
    case 103:
        if (line == 0)
        {
            sprintf(str, ITEM_103_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_103_INF2);
        }
        break;
    case 104:
        if (line == 0)
        {
            sprintf(str, ITEM_104_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_104_INF2);
        }
        break;
    case 105:
        if (line == 0)
        {
            sprintf(str, ITEM_105_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_105_INF2);
        }
        break;
    case 106:
        if (line == 0)
        {
            sprintf(str, ITEM_106_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_106_INF2);
        }
        break;
    case 107:
        if (line == 0)
        {
            sprintf(str, ITEM_107_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_107_INF2);
        }
        break;
    case 108:
        if (line == 0)
        {
            sprintf(str, ITEM_108_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_108_INF2);
        }
        break;
    case 109:
        if (line == 0)
        {
            sprintf(str, ITEM_109_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_109_INF2);
        }
        break;
    case 110:
        if (line == 0)
        {
            sprintf(str, ITEM_110_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_110_INF2);
        }
        break;
    case 201:
        if (line == 0)
        {
            sprintf(str, ITEM_201_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_201_INF2);
        }
        break;
    case 202:
        if (line == 0)
        {
            sprintf(str, ITEM_202_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_202_INF2);
        }
        break;
    case 203:
        if (line == 0)
        {
            sprintf(str, ITEM_203_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_203_INF2);
        }
        break;
    case 204:
        if (line == 0)
        {
            sprintf(str, ITEM_204_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_204_INF2);
        }
        break;
    case 205:
        if (line == 0)
        {
            sprintf(str, ITEM_205_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_205_INF2);
        }
        break;
    case 206:
        if (line == 0)
        {
            sprintf(str, ITEM_206_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_206_INF2);
        }
        break;
    case 207:
        if (line == 0)
        {
            sprintf(str, ITEM_207_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_207_INF2);
        }
        break;
    case 208:
        if (line == 0)
        {
            sprintf(str, ITEM_208_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_208_INF2);
        }
        break;
    case 209:
        if (line == 0)
        {
            sprintf(str, ITEM_209_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_209_INF2);
        }
        break;
    case 210:
        if (line == 0)
        {
            sprintf(str, ITEM_210_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_210_INF2);
        }
        break;
    case 301:
        if (line == 0)
        {
            sprintf(str, ITEM_301_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_301_INF2);
        }
        break;
    case 302:
        if (line == 0)
        {
            sprintf(str, ITEM_302_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_302_INF2);
        }
        break;
    case 303:
        if (line == 0)
        {
            sprintf(str, ITEM_303_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_303_INF2);
        }
        break;
    case 304:
        if (line == 0)
        {
            sprintf(str, ITEM_304_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_304_INF2);
        }
        break;
    case 305:
        if (line == 0)
        {
            sprintf(str, ITEM_305_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_305_INF2);
        }
        break;
    case 306:
        if (line == 0)
        {
            sprintf(str, ITEM_306_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_306_INF2);
        }
        break;
    case 307:
        if (line == 0)
        {
            sprintf(str, ITEM_307_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_307_INF2);
        }
        break;
    case 308:
        if (line == 0)
        {
            sprintf(str, ITEM_308_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_308_INF2);
        }
        break;
    case 309:
        if (line == 0)
        {
            sprintf(str, ITEM_309_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_309_INF2);
        }
        break;
    case 310:
        if (line == 0)
        {
            sprintf(str, ITEM_310_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_310_INF2);
        }
        break;
    case 401:
        if (line == 0)
        {
            sprintf(str, ITEM_401_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_401_INF2);
        }
        break;
    case 402:
        if (line == 0)
        {
            sprintf(str, ITEM_402_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_402_INF2);
        }
        break;
    case 403:
        if (line == 0)
        {
            sprintf(str, ITEM_403_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_403_INF2);
        }
        break;
    case 404:
        if (line == 0)
        {
            sprintf(str, ITEM_404_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_404_INF2);
        }
        break;
    case 405:
        if (line == 0)
        {
            sprintf(str, ITEM_405_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_405_INF2);
        }
        break;
    case 406:
        if (line == 0)
        {
            sprintf(str, ITEM_406_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_406_INF2);
        }
        break;
    case 407:
        if (line == 0)
        {
            sprintf(str, ITEM_407_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_407_INF2);
        }
        break;
    case 408:
        if (line == 0)
        {
            sprintf(str, ITEM_408_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_408_INF2);
        }
        break;
    case 409:
        if (line == 0)
        {
            sprintf(str, ITEM_409_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_409_INF2);
        }
        break;
    case 410:
        if (line == 0)
        {
            sprintf(str, ITEM_410_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_410_INF2);
        }
        break;
    case 501:
        if (line == 0)
        {
            sprintf(str, ITEM_501_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_501_INF2);
        }
        break;
    case 502:
        if (line == 0)
        {
            sprintf(str, ITEM_502_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_502_INF2);
        }
        break;
    case 503:
        if (line == 0)
        {
            sprintf(str, ITEM_503_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_503_INF2);
        }
        break;
    case 504:
        if (line == 0)
        {
            sprintf(str, ITEM_504_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_504_INF2);
        }
        break;
    case 505:
        if (line == 0)
        {
            sprintf(str, ITEM_505_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_505_INF2);
        }
        break;
    case 506:
        if (line == 0)
        {
            sprintf(str, ITEM_506_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_506_INF2);
        }
        break;
    case 507:
        if (line == 0)
        {
            sprintf(str, ITEM_507_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_507_INF2);
        }
        break;
    case 508:
        if (line == 0)
        {
            sprintf(str, ITEM_508_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_508_INF2);
        }
        break;
    case 509:
        if (line == 0)
        {
            sprintf(str, ITEM_509_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_509_INF2);
        }
        break;
    case 510:
        if (line == 0)
        {
            sprintf(str, ITEM_510_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_510_INF2);
        }
        break;
    case 511:
        if (line == 0)
        {
            sprintf(str, ITEM_511_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_511_INF2);
        }
        break;
    case 512:
        if (line == 0)
        {
            sprintf(str, ITEM_512_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_512_INF2);
        }
        break;
    case 513:
        if (line == 0)
        {
            sprintf(str, ITEM_513_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_513_INF2);
        }
        break;
    case 514:
        if (line == 0)
        {
            sprintf(str, ITEM_514_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_514_INF2);
        }
        break;
    case 515:
        if (line == 0)
        {
            sprintf(str, ITEM_515_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_515_INF2);
        }
        break;
    case 516:
        if (line == 0)
        {
            sprintf(str, ITEM_516_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_516_INF2);
        }
        break;
    case 517:
        if (line == 0)
        {
            sprintf(str, ITEM_517_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_517_INF2);
        }
        break;
    case 601:
        if (line == 0)
        {
            sprintf(str, ITEM_601_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_601_INF2);
        }
        break;
    case 602:
        if (line == 0)
        {
            sprintf(str, ITEM_602_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_602_INF2);
        }
        break;
    case 603:
        if (line == 0)
        {
            sprintf(str, ITEM_603_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_603_INF2);
        }
        break;
    case 604:
        if (line == 0)
        {
            sprintf(str, ITEM_604_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_604_INF2);
        }
        break;
    case 605:
        if (line == 0)
        {
            sprintf(str, ITEM_605_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_605_INF2);
        }
        break;
    case 606:
        if (line == 0)
        {
            sprintf(str, ITEM_606_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_606_INF2);
        }
        break;
    case 607:
        if (line == 0)
        {
            sprintf(str, ITEM_607_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_607_INF2);
        }
        break;
    case 608:
        if (line == 0)
        {
            sprintf(str, ITEM_608_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_608_INF2);
        }
        break;
    case 609:
        if (line == 0)
        {
            sprintf(str, ITEM_609_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_609_INF2);
        }
        break;
    case 610:
        if (line == 0)
        {
            sprintf(str, ITEM_610_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, ITEM_610_INF2);
        }
        break;
    case 701:
        if (line == 0)
        {
            sprintf(str, SKILL_701_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_701_INF2);
        }
        break;
    case 702:
        if (line == 0)
        {
            sprintf(str, SKILL_702_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_702_INF2);
        }
        break;
    case 703:
        if (line == 0)
        {
            sprintf(str, SKILL_703_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_703_INF2);
        }
        break;
    case 704:
        if (line == 0)
        {
            sprintf(str, SKILL_704_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_704_INF2);
        }
        break;
    case 705:
        if (line == 0)
        {
            sprintf(str, SKILL_705_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_705_INF2);
        }
        break;
    case 706:
        if (line == 0)
        {
            sprintf(str, SKILL_706_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_706_INF2);
        }
        break;
    case 707:
        if (line == 0)
        {
            sprintf(str, SKILL_707_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_707_INF2);
        }
        break;
    case 708:
        if (line == 0)
        {
            sprintf(str, SKILL_708_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_708_INF2);
        }
        break;
    case 709:
        if (line == 0)
        {
            sprintf(str, SKILL_709_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_709_INF2);
        }
        break;
    case 710:
        if (line == 0)
        {
            sprintf(str, SKILL_710_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_710_INF2);
        }
        break;
    case 711:
        if (line == 0)
        {
            sprintf(str, SKILL_711_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_711_INF2);
        }
        break;
    case 712:
        if (line == 0)
        {
            sprintf(str, SKILL_712_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_712_INF2);
        }
        break;
    case 713:
        if (line == 0)
        {
            sprintf(str, SKILL_713_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_713_INF2);
        }
        break;
    case 714:
        if (line == 0)
        {
            sprintf(str, SKILL_714_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_714_INF2);
        }
        break;
    case 715:
        if (line == 0)
        {
            sprintf(str, SKILL_715_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_715_INF2);
        }
        break;
    case 716:
        if (line == 0)
        {
            sprintf(str, SKILL_716_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_716_INF2);
        }
        break;
    case 717:
        if (line == 0)
        {
            sprintf(str, SKILL_717_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_717_INF2);
        }
        break;
    case 718:
        if (line == 0)
        {
            sprintf(str, SKILL_718_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_718_INF2);
        }
        break;
    case 719:
        if (line == 0)
        {
            sprintf(str, SKILL_719_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_719_INF2);
        }
        break;
    case 720:
        if (line == 0)
        {
            sprintf(str, SKILL_720_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_720_INF2);
        }
        break;
    case 721:
        if (line == 0)
        {
            sprintf(str, SKILL_721_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_721_INF2);
        }
        break;
    case 722:
        if (line == 0)
        {
            sprintf(str, SKILL_722_INF1);
        }
        else if (line == 1)
        {
            sprintf(str, SKILL_722_INF2);
        }
        break;
    }
}

int item_get(Item *item, int code, int num)
{
    int i = 0;

    for (i = 0; i < 50; i++)
    {
        if (item->inven[i][0] == 0 || item->inven[i][0] == code)
            break;
    }
    if (i >= 50)
    {
        return -1;
    }
    item->inven[i][0] = code;
    item->inven[i][1] += num;

    return 0;
}

int item_del(Item *item, int code, int num)
{
    int i = 0;

    for (i = 0; i < 50; i++)
    {
        if (item->inven[i][0] == code)
            break;
    }
    if (i >= 50)
    {
        return -1;
    }

    if (item->inven[i][1] < num)
    {
        return -1;
    }
    else
    {
        item->inven[i][1] -= num;
        if (item->inven[i][1] == 0)
        {
            for (; i < 49; i++)
            {
                if(item->inven[i+1][0]!=0)
                {
                    item->inven[i][0] = item->inven[i + 1][0];
                    item->inven[i][1] = item->inven[i + 1][1];
                }
                else
                {
                    break;
                }
            }
            item->inven[i][0]=0;
            item->inven[i][1]=0;
        }
    }

    return 0;
}

int item_use(Stat *stat, Item *item, int code, int num)
{
    int temp = 0;
    int w_code = 0;
    int i = 0;
    int my=stat->plnum;

    if (code < 100)
    {
        temp = item_del(item, code, num);
        if (temp == -1)
        {
            return -1;
        }
        switch (code)
        {
        case 1: //회복약
            ch_stat(stat, 1, ITEM_001_HP_P, 1);
            break;
        case 2: //MP포션
            ch_stat(stat, 3, ITEM_002_MP_P, 1);
            break;
        case 3: //엘릭서
            ch_stat(stat, 1, ITEM_003_HP_P, 1);
            ch_stat(stat, 3, ITEM_003_MP_P, 1);
            for (i = 0; i < 3; i++)
            {
                stat->con[my][i][0] = 0;
                stat->con[my][i][1] = 0;
            }
            break;
        case 4: //만병통치약
            for (i = 0; i < 3; i++)
            {
                stat->con[my][i][0] = 0;
                stat->con[my][i][1] = 0;
            }
            break;
        case 5: //피닉스의 깃털
            
            break;
        case 6: //귀환주문서

            break;
        }
    }
    else if (code < 800)
    {
        if (code < 200)
        {
            temp = item_get(item, item->equip[4], 1);
            if (temp != -1)
            {
                item_stat(stat, item, item->equip[4], 1);
                item->equip[4] = code;
                item_stat(stat, item, code, 0);
            }
            else
            {
                return -1;
            }
        }
        else if (code < 300)
        {
            temp = item_get(item, item->equip[5], 1);
            if (temp != -1)
            {
                item_stat(stat, item, item->equip[5], 1);
                item->equip[5] = code;
                item_stat(stat, item, code, 0);
            }
            else
            {
                return -1;
            }
        }
        else if (code < 400)
        {
            temp = item_get(item, item->equip[7], 1);
            if (temp != -1)
            {
                item_stat(stat, item, item->equip[7], 1);
                item->equip[7] = code;
                item_stat(stat, item, code, 0);
            }
            else
            {
                return -1;
            }
        }
        else if (code < 500)
        {
            temp = item_get(item, item->equip[6], 1);
            if (temp != -1)
            {
                item_stat(stat, item, item->equip[6], 1);
                item->equip[6] = code;
                item_stat(stat, item, code, 0);
            }
            else
            {
                return -1;
            }
        }
        else if (code < 600)
        {
            if (code >= 504 && code <= 512) //양손 무기 장착
            {
                if (item->equip[0] < 504 || item->equip[0] > 512)   //이미 낀 무기가 한손
                {
                    w_code = item->equip[0];
                    temp = item_get(item, item->equip[0], 1);   //오른손 탈착
                    if (temp == -1)
                    {
                        return -1;
                    }
                    item_stat(stat, item, item->equip[0], 1);   //오른손 스탯 깎고
                    item->equip[0] = code;  //오른손 장착
                    if (item->equip[1] < 504 || item->equip[1] > 512)   //왼손 탈착
                    {
                        temp = item_get(item, item->equip[1], 1);
                        if (temp == -1)
                        {
                            item_del(item, w_code, 1);
                            item->equip[0]=w_code;
                            return -1;
                        }
                    }
                    item_stat(stat, item, item->equip[1], 1); //왼손 스탯 깎고
                    item->equip[1] = code;                    //왼손 창착
                    item_stat(stat, item, item->equip[1], 0); //양손 스탯 적용
                }
                else   //이미 낀 무기가 양손
                {
                    temp = item_get(item, item->equip[0], 1);   //탈착
                    if (temp == -1)
                    {
                        return -1;
                    }
                    item_stat(stat, item, item->equip[0], 1); //스탯 깎고
                    item->equip[0] = code;                    //양쪽손에 같은 아이템 코드
                    item->equip[1] = code;
                    item_stat(stat, item, item->equip[1], 0); //양손 스탯 적용
                }
            }
            else //한손 무기 장착
            {
                if (item->equip[0] == 0)    //빈 칸에 장착
                {
                    item->equip[0] = code;
                    item_stat(stat, item, item->equip[0], 0); //스탯 적용
                }
                else if (item->equip[1] == 0)   //빈 칸에 장착
                {
                    item->equip[1] = code;
                    item_stat(stat, item, item->equip[1], 0); //스탯 적용

                }
                else    //빈칸이 없을 경우
                {
                    if (item->equip[0] == code && item->equip[1] != code)   //이미 오른손에 장착하려는 한손 무기가 장착되어 있는 경우
                    {
                        item_get(item, item->equip[1], 1);  //왼손 무기 교체
                        if (temp == -1)
                        {
                            return -1;
                        }
                        item_stat(stat, item, item->equip[1], 1);   //왼손 무기 스탯 깎음
                        item->equip[1] = code;
                        item_stat(stat, item, item->equip[1], 0);   //왼손 무기 스탯 적용

                    }
                    else if (item->equip[0] >= 504 && item->equip[0] <= 512)    //장착되어 있는 무기가 양손인경우
                    {
                        item_get(item, item->equip[0], 1);      //아이템 인벤토리에 추가
                        if (temp == -1)
                        {
                            return -1;
                        }
                        item_stat(stat, item, item->equip[1], 1);   //무기 스탯 깎고
                        item->equip[1] = 0;
                        item->equip[0] = code;  //오른손에 한손무기 장착
                        item_stat(stat, item, item->equip[0], 0);   //오른손 무기 스탯 적용
                    }
                    else    //그외의 경우 
                    {
                        item_get(item, item->equip[0], 1);  
                        if (temp == -1)
                        {
                            return -1;
                        }
                        item_stat(stat, item, item->equip[0], 1);   //오른손 무기 스탯 깎음
                        item->equip[0] = code;              //오른손에 무기와 교환
                        item_stat(stat, item, item->equip[0], 0);   //오른손 무기 스탯 적용
                    }
                }
            }
        }
        else if (code < 700)
        {
            if (item->equip[2] == 0)
            {
                item->equip[2] = code;
                item_stat(stat, item, item->equip[2], 0); //오른손 반지 스탯 적용
            }
            else if (item->equip[3] == 0)
            {
                item->equip[3] = code;
                item_stat(stat, item, item->equip[3], 0); //왼손 반지 스탯 적용
            }
            else
            {
                if (code == item->equip[2]) //오른손 반지가 이미 끼려는 반지인 경우
                {
                    item_get(item, item->equip[3], 1);  //왼손 반지와 교체
                    if (temp == -1)
                    {
                        return -1;
                    }
                    item_stat(stat, item, item->equip[3], 1); //왼손 반지 스탯 깎음
                    item->equip[3]=code;
                    item_stat(stat, item, item->equip[3], 0); //왼손 반지 스탯 적용
                }
                else                    //그 외의 경우는 오른손 반지를 교체
                {
                    item_get(item, item->equip[2], 1);
                    if (temp == -1)
                    {
                        return -1;
                    }
                    item_stat(stat, item, item->equip[2], 1); //오른손 반지 스탯 깎음
                    item->equip[2] = code;
                    item_stat(stat, item, item->equip[2], 0); //오른손 반지 스탯 적용
                }
            }
        }
        else if (code < 800)
        {
            temp=skill_get(item, code);
            if(temp==-1)
            {
                return -1;
            }
        }
        temp = item_del(item, code, num);
        if (temp == -1)
        {
            return -1;
        }
    }
}

int item_stat(Stat *stat, Item *item, int code, int mode)
{
    int temp=0;
    Item_st item_st = {
        {ITEM_501_HP, ITEM_501_MP, ITEM_501_HP_P, ITEM_501_MP_P, ITEM_501_ATK_MIN, ITEM_501_ATK_MAX, ITEM_501_DF,
         ITEM_502_HP, ITEM_502_MP, ITEM_502_HP_P, ITEM_502_MP_P, ITEM_502_ATK_MIN, ITEM_502_ATK_MAX, ITEM_502_DF,
         ITEM_503_HP, ITEM_503_MP, ITEM_503_HP_P, ITEM_503_MP_P, ITEM_503_ATK_MIN, ITEM_503_ATK_MAX, ITEM_503_DF,
         ITEM_504_HP, ITEM_504_MP, ITEM_504_HP_P, ITEM_504_MP_P, ITEM_504_ATK_MIN, ITEM_504_ATK_MAX, ITEM_504_DF,
         ITEM_505_HP, ITEM_505_MP, ITEM_505_HP_P, ITEM_505_MP_P, ITEM_505_ATK_MIN, ITEM_505_ATK_MAX, ITEM_505_DF,
         ITEM_506_HP, ITEM_506_MP, ITEM_506_HP_P, ITEM_506_MP_P, ITEM_506_ATK_MIN, ITEM_506_ATK_MAX, ITEM_506_DF,
         ITEM_507_HP, ITEM_507_MP, ITEM_507_HP_P, ITEM_507_MP_P, ITEM_507_ATK_MIN, ITEM_507_ATK_MAX, ITEM_507_DF,
         ITEM_508_HP, ITEM_508_MP, ITEM_508_HP_P, ITEM_508_MP_P, ITEM_508_ATK_MIN, ITEM_508_ATK_MAX, ITEM_508_DF,
         ITEM_509_HP, ITEM_509_MP, ITEM_509_HP_P, ITEM_509_MP_P, ITEM_509_ATK_MIN, ITEM_509_ATK_MAX, ITEM_509_DF,
         ITEM_510_HP, ITEM_510_MP, ITEM_510_HP_P, ITEM_510_MP_P, ITEM_510_ATK_MIN, ITEM_510_ATK_MAX, ITEM_510_DF,
         ITEM_511_HP, ITEM_511_MP, ITEM_511_HP_P, ITEM_511_MP_P, ITEM_511_ATK_MIN, ITEM_511_ATK_MAX, ITEM_511_DF,
         ITEM_512_HP, ITEM_512_MP, ITEM_512_HP_P, ITEM_512_MP_P, ITEM_512_ATK_MIN, ITEM_512_ATK_MAX, ITEM_512_DF,
         ITEM_513_HP, ITEM_513_MP, ITEM_513_HP_P, ITEM_513_MP_P, ITEM_513_ATK_MIN, ITEM_513_ATK_MAX, ITEM_513_DF,
         ITEM_514_HP, ITEM_514_MP, ITEM_514_HP_P, ITEM_514_MP_P, ITEM_514_ATK_MIN, ITEM_514_ATK_MAX, ITEM_514_DF,
         ITEM_515_HP, ITEM_515_MP, ITEM_515_HP_P, ITEM_515_MP_P, ITEM_515_ATK_MIN, ITEM_515_ATK_MAX, ITEM_515_DF,
         ITEM_516_HP, ITEM_516_MP, ITEM_516_HP_P, ITEM_516_MP_P, ITEM_516_ATK_MIN, ITEM_516_ATK_MAX, ITEM_516_DF,
         ITEM_517_HP, ITEM_517_MP, ITEM_517_HP_P, ITEM_517_MP_P, ITEM_517_ATK_MIN, ITEM_517_ATK_MAX, ITEM_517_DF},
        {ITEM_101_HP, ITEM_101_MP, ITEM_101_HP_P, ITEM_101_MP_P, ITEM_101_ATK_MIN, ITEM_101_ATK_MAX, ITEM_101_DF,
         ITEM_102_HP, ITEM_102_MP, ITEM_102_HP_P, ITEM_102_MP_P, ITEM_102_ATK_MIN, ITEM_102_ATK_MAX, ITEM_102_DF,
         ITEM_103_HP, ITEM_103_MP, ITEM_103_HP_P, ITEM_103_MP_P, ITEM_103_ATK_MIN, ITEM_103_ATK_MAX, ITEM_103_DF,
         ITEM_104_HP, ITEM_104_MP, ITEM_104_HP_P, ITEM_104_MP_P, ITEM_104_ATK_MIN, ITEM_104_ATK_MAX, ITEM_104_DF,
         ITEM_105_HP, ITEM_105_MP, ITEM_105_HP_P, ITEM_105_MP_P, ITEM_105_ATK_MIN, ITEM_105_ATK_MAX, ITEM_105_DF,
         ITEM_106_HP, ITEM_106_MP, ITEM_106_HP_P, ITEM_106_MP_P, ITEM_106_ATK_MIN, ITEM_106_ATK_MAX, ITEM_106_DF,
         ITEM_107_HP, ITEM_107_MP, ITEM_107_HP_P, ITEM_107_MP_P, ITEM_107_ATK_MIN, ITEM_107_ATK_MAX, ITEM_107_DF,
         ITEM_108_HP, ITEM_108_MP, ITEM_108_HP_P, ITEM_108_MP_P, ITEM_108_ATK_MIN, ITEM_108_ATK_MAX, ITEM_108_DF,
         ITEM_109_HP, ITEM_109_MP, ITEM_109_HP_P, ITEM_109_MP_P, ITEM_109_ATK_MIN, ITEM_109_ATK_MAX, ITEM_109_DF,
         ITEM_110_HP, ITEM_110_MP, ITEM_110_HP_P, ITEM_110_MP_P, ITEM_110_ATK_MIN, ITEM_110_ATK_MAX, ITEM_110_DF},
         {ITEM_201_HP, ITEM_201_MP, ITEM_201_HP_P, ITEM_201_MP_P, ITEM_201_ATK_MIN, ITEM_201_ATK_MAX, ITEM_201_DF,
         ITEM_202_HP, ITEM_202_MP, ITEM_202_HP_P, ITEM_202_MP_P, ITEM_202_ATK_MIN, ITEM_202_ATK_MAX, ITEM_202_DF,
         ITEM_203_HP, ITEM_203_MP, ITEM_203_HP_P, ITEM_203_MP_P, ITEM_203_ATK_MIN, ITEM_203_ATK_MAX, ITEM_203_DF,
         ITEM_204_HP, ITEM_204_MP, ITEM_204_HP_P, ITEM_204_MP_P, ITEM_204_ATK_MIN, ITEM_204_ATK_MAX, ITEM_204_DF,
         ITEM_205_HP, ITEM_205_MP, ITEM_205_HP_P, ITEM_205_MP_P, ITEM_205_ATK_MIN, ITEM_205_ATK_MAX, ITEM_205_DF,
         ITEM_206_HP, ITEM_206_MP, ITEM_206_HP_P, ITEM_206_MP_P, ITEM_206_ATK_MIN, ITEM_206_ATK_MAX, ITEM_206_DF,
         ITEM_207_HP, ITEM_207_MP, ITEM_207_HP_P, ITEM_207_MP_P, ITEM_207_ATK_MIN, ITEM_207_ATK_MAX, ITEM_207_DF,
         ITEM_208_HP, ITEM_208_MP, ITEM_208_HP_P, ITEM_208_MP_P, ITEM_208_ATK_MIN, ITEM_208_ATK_MAX, ITEM_208_DF,
         ITEM_209_HP, ITEM_209_MP, ITEM_209_HP_P, ITEM_209_MP_P, ITEM_209_ATK_MIN, ITEM_209_ATK_MAX, ITEM_209_DF,
         ITEM_210_HP, ITEM_210_MP, ITEM_210_HP_P, ITEM_210_MP_P, ITEM_210_ATK_MIN, ITEM_210_ATK_MAX, ITEM_210_DF},
         {ITEM_301_HP, ITEM_301_MP, ITEM_301_HP_P, ITEM_301_MP_P, ITEM_301_ATK_MIN, ITEM_301_ATK_MAX, ITEM_301_DF,
         ITEM_302_HP, ITEM_302_MP, ITEM_302_HP_P, ITEM_302_MP_P, ITEM_302_ATK_MIN, ITEM_302_ATK_MAX, ITEM_302_DF,
         ITEM_303_HP, ITEM_303_MP, ITEM_303_HP_P, ITEM_303_MP_P, ITEM_303_ATK_MIN, ITEM_303_ATK_MAX, ITEM_303_DF,
         ITEM_304_HP, ITEM_304_MP, ITEM_304_HP_P, ITEM_304_MP_P, ITEM_304_ATK_MIN, ITEM_304_ATK_MAX, ITEM_304_DF,
         ITEM_305_HP, ITEM_305_MP, ITEM_305_HP_P, ITEM_305_MP_P, ITEM_305_ATK_MIN, ITEM_305_ATK_MAX, ITEM_305_DF,
         ITEM_306_HP, ITEM_306_MP, ITEM_306_HP_P, ITEM_306_MP_P, ITEM_306_ATK_MIN, ITEM_306_ATK_MAX, ITEM_306_DF,
         ITEM_307_HP, ITEM_307_MP, ITEM_307_HP_P, ITEM_307_MP_P, ITEM_307_ATK_MIN, ITEM_307_ATK_MAX, ITEM_307_DF,
         ITEM_308_HP, ITEM_308_MP, ITEM_308_HP_P, ITEM_308_MP_P, ITEM_308_ATK_MIN, ITEM_308_ATK_MAX, ITEM_308_DF,
         ITEM_309_HP, ITEM_309_MP, ITEM_309_HP_P, ITEM_309_MP_P, ITEM_309_ATK_MIN, ITEM_309_ATK_MAX, ITEM_309_DF,
         ITEM_310_HP, ITEM_310_MP, ITEM_310_HP_P, ITEM_310_MP_P, ITEM_310_ATK_MIN, ITEM_310_ATK_MAX, ITEM_310_DF},
         {ITEM_401_HP, ITEM_401_MP, ITEM_401_HP_P, ITEM_401_MP_P, ITEM_401_ATK_MIN, ITEM_401_ATK_MAX, ITEM_401_DF,
         ITEM_402_HP, ITEM_402_MP, ITEM_402_HP_P, ITEM_402_MP_P, ITEM_402_ATK_MIN, ITEM_402_ATK_MAX, ITEM_402_DF,
         ITEM_403_HP, ITEM_403_MP, ITEM_403_HP_P, ITEM_403_MP_P, ITEM_403_ATK_MIN, ITEM_403_ATK_MAX, ITEM_403_DF,
         ITEM_404_HP, ITEM_404_MP, ITEM_404_HP_P, ITEM_404_MP_P, ITEM_404_ATK_MIN, ITEM_404_ATK_MAX, ITEM_404_DF,
         ITEM_405_HP, ITEM_405_MP, ITEM_405_HP_P, ITEM_405_MP_P, ITEM_405_ATK_MIN, ITEM_405_ATK_MAX, ITEM_405_DF,
         ITEM_406_HP, ITEM_406_MP, ITEM_406_HP_P, ITEM_406_MP_P, ITEM_406_ATK_MIN, ITEM_406_ATK_MAX, ITEM_406_DF,
         ITEM_407_HP, ITEM_407_MP, ITEM_407_HP_P, ITEM_407_MP_P, ITEM_407_ATK_MIN, ITEM_407_ATK_MAX, ITEM_407_DF,
         ITEM_408_HP, ITEM_408_MP, ITEM_408_HP_P, ITEM_408_MP_P, ITEM_408_ATK_MIN, ITEM_408_ATK_MAX, ITEM_408_DF,
         ITEM_409_HP, ITEM_409_MP, ITEM_409_HP_P, ITEM_409_MP_P, ITEM_409_ATK_MIN, ITEM_409_ATK_MAX, ITEM_409_DF,
         ITEM_410_HP, ITEM_410_MP, ITEM_410_HP_P, ITEM_410_MP_P, ITEM_410_ATK_MIN, ITEM_410_ATK_MAX, ITEM_410_DF},
         {ITEM_601_HP, ITEM_601_MP, ITEM_601_HP_P, ITEM_601_MP_P, ITEM_601_ATK_MIN, ITEM_601_ATK_MAX, ITEM_601_DF,
         ITEM_602_HP, ITEM_602_MP, ITEM_602_HP_P, ITEM_602_MP_P, ITEM_602_ATK_MIN, ITEM_602_ATK_MAX, ITEM_602_DF,
         ITEM_603_HP, ITEM_603_MP, ITEM_603_HP_P, ITEM_603_MP_P, ITEM_603_ATK_MIN, ITEM_603_ATK_MAX, ITEM_603_DF,
         ITEM_604_HP, ITEM_604_MP, ITEM_604_HP_P, ITEM_604_MP_P, ITEM_604_ATK_MIN, ITEM_604_ATK_MAX, ITEM_604_DF,
         ITEM_605_HP, ITEM_605_MP, ITEM_605_HP_P, ITEM_605_MP_P, ITEM_605_ATK_MIN, ITEM_605_ATK_MAX, ITEM_605_DF,
         ITEM_606_HP, ITEM_606_MP, ITEM_606_HP_P, ITEM_606_MP_P, ITEM_606_ATK_MIN, ITEM_606_ATK_MAX, ITEM_606_DF,
         ITEM_607_HP, ITEM_607_MP, ITEM_607_HP_P, ITEM_607_MP_P, ITEM_607_ATK_MIN, ITEM_607_ATK_MAX, ITEM_607_DF,
         ITEM_608_HP, ITEM_608_MP, ITEM_608_HP_P, ITEM_608_MP_P, ITEM_608_ATK_MIN, ITEM_608_ATK_MAX, ITEM_608_DF,
         ITEM_609_HP, ITEM_609_MP, ITEM_609_HP_P, ITEM_609_MP_P, ITEM_609_ATK_MIN, ITEM_609_ATK_MAX, ITEM_609_DF,
         ITEM_610_HP, ITEM_610_MP, ITEM_610_HP_P, ITEM_610_MP_P, ITEM_610_ATK_MIN, ITEM_610_ATK_MAX, ITEM_610_DF}
         };
    //0:최대 HP 1:최소 HP 2:HP회복량 3:MP회복량 4:최소공격력 5:최대공격력 6:방어력

    if (mode == 0)
    {
        if (code < 100) //소모품
        {
            return 0;
        }
        else if (code < 200)    //헬맷
        {
            temp=code%100-1;
            ch_stat(stat, 2, item_st.helm[temp][0], 0);
            ch_stat(stat, 4, item_st.helm[temp][1], 0);
            ch_stat(stat, 6, item_st.helm[temp][5], 0);
            ch_stat(stat, 5, item_st.helm[temp][4], 0);
            ch_stat(stat, 8, item_st.helm[temp][6], 0);
        }
        else if (code < 300)    //갑옷
        {
            temp=code%200-1;
            ch_stat(stat, 2, item_st.helm[temp][0], 0);
            ch_stat(stat, 4, item_st.helm[temp][1], 0);
            ch_stat(stat, 6, item_st.helm[temp][5], 0);
            ch_stat(stat, 5, item_st.helm[temp][4], 0);
            ch_stat(stat, 8, item_st.helm[temp][6], 0);
        }
        else if (code < 400)    //신발
        {
            temp=code%300-1;
            ch_stat(stat, 2, item_st.helm[temp][0], 0);
            ch_stat(stat, 4, item_st.helm[temp][1], 0);
            ch_stat(stat, 6, item_st.helm[temp][5], 0);
            ch_stat(stat, 5, item_st.helm[temp][4], 0);
            ch_stat(stat, 8, item_st.helm[temp][6], 0);
        }
        else if (code < 500)    //장갑
        {
            temp=code%400-1;
            ch_stat(stat, 2, item_st.helm[temp][0], 0);
            ch_stat(stat, 4, item_st.helm[temp][1], 0);
            ch_stat(stat, 6, item_st.helm[temp][5], 0);
            ch_stat(stat, 5, item_st.helm[temp][4], 0);
            ch_stat(stat, 8, item_st.helm[temp][6], 0);
        }
        else if (code < 600)    //무기
        {
            temp=code%500-1;
            if (code >= 504 && code <= 512) //양손
            {
                ch_stat(stat, 2, item_st.weapon[temp][0],0);
                ch_stat(stat, 4, item_st.weapon[temp][1],0);
                ch_stat(stat, 6, item_st.weapon[temp][5],0);
                ch_stat(stat, 5, item_st.weapon[temp][4],0);
                ch_stat(stat, 8, item_st.weapon[temp][6],0);
            }
            else
            {
                if(item->equip[0] == item->equip[1])    //한손
                {
                    ch_stat(stat, 6, item_st.weapon[temp][5], 0);
                    ch_stat(stat, 8, item_st.weapon[temp][6], 0);
                }
                else
                {
                    ch_stat(stat, 2, item_st.weapon[temp][0], 0);
                    ch_stat(stat, 4, item_st.weapon[temp][1], 0);
                    ch_stat(stat, 6, item_st.weapon[temp][5], 0);
                    ch_stat(stat, 5, item_st.weapon[temp][4], 0);
                    ch_stat(stat, 8, item_st.weapon[temp][6], 0);
                }
            }
        }
        else if (code < 700)    //반지
        {
            temp=code%600-1;
            ch_stat(stat, 2, item_st.ring[temp][0], 0);
            ch_stat(stat, 4, item_st.ring[temp][1], 0);
            ch_stat(stat, 6, item_st.ring[temp][5], 0);
            ch_stat(stat, 5, item_st.ring[temp][4], 0);
            ch_stat(stat, 8, item_st.ring[temp][6], 0);
        }
    }
    else if(mode==1)
    {
        if (code < 100)
        {
            return 0;
        }
        else if (code < 200)
        {
            temp=code%100-1;
            ch_stat(stat, 2, -(item_st.helm[temp][0]), 0);
            ch_stat(stat, 4, -(item_st.helm[temp][1]), 0);
            ch_stat(stat, 6, -(item_st.helm[temp][5]), 0);
            ch_stat(stat, 5, -(item_st.helm[temp][4]), 0);
            ch_stat(stat, 8, -(item_st.helm[temp][6]), 0);
        }
        else if (code < 300)
        {
            temp=code%200-1;
            ch_stat(stat, 2, -(item_st.helm[temp][0]), 0);
            ch_stat(stat, 4, -(item_st.helm[temp][1]), 0);
            ch_stat(stat, 6, -(item_st.helm[temp][5]), 0);
            ch_stat(stat, 5, -(item_st.helm[temp][4]), 0);
            ch_stat(stat, 8, -(item_st.helm[temp][6]), 0);
        }
        else if (code < 400)
        {
            temp=code%300-1;
            ch_stat(stat, 2, -(item_st.helm[temp][0]), 0);
            ch_stat(stat, 4, -(item_st.helm[temp][1]), 0);
            ch_stat(stat, 6, -(item_st.helm[temp][5]), 0);
            ch_stat(stat, 5, -(item_st.helm[temp][4]), 0);
            ch_stat(stat, 8, -(item_st.helm[temp][6]), 0);
        }
        else if (code < 500)
        {
            temp=code%400-1;
            ch_stat(stat, 2, -(item_st.helm[temp][0]), 0);
            ch_stat(stat, 4, -(item_st.helm[temp][1]), 0);
            ch_stat(stat, 6, -(item_st.helm[temp][5]), 0);
            ch_stat(stat, 5, -(item_st.helm[temp][4]), 0);
            ch_stat(stat, 8, -(item_st.helm[temp][6]), 0);
        }
        else if (code < 600)
        {
            temp=code%500-1;
            if (code >= 504 && code <= 512)
            {
                ch_stat(stat, 2, -(item_st.weapon[temp][0]),0);
                ch_stat(stat, 4, -(item_st.weapon[temp][1]),0);
                ch_stat(stat, 6, -(item_st.weapon[temp][5]),0);
                ch_stat(stat, 5, -(item_st.weapon[temp][4]),0);
                ch_stat(stat, 8, -(item_st.weapon[temp][6]),0);
            }
            else
            {
                if(item->equip[0] == item->equip[1])
                {
                    ch_stat(stat, 6, -(item_st.weapon[temp][5]), 0);
                    ch_stat(stat, 8, -(item_st.weapon[temp][6]), 0);
                }
                else
                {
                    ch_stat(stat, 2, -(item_st.weapon[temp][0]), 0);
                    ch_stat(stat, 4, -(item_st.weapon[temp][1]), 0);
                    ch_stat(stat, 5, -(item_st.weapon[temp][4]), 0);
                    ch_stat(stat, 6, -(item_st.weapon[temp][5]), 0);
                    ch_stat(stat, 8, -(item_st.weapon[temp][6]), 0);
                }
            }
        }
        else if (code < 700)
        {
            temp=code%600-1;
            ch_stat(stat, 2, -(item_st.ring[temp][0]), 0);
            ch_stat(stat, 4, -(item_st.ring[temp][1]), 0);
            ch_stat(stat, 6, -(item_st.ring[temp][5]), 0);
            ch_stat(stat, 5, -(item_st.ring[temp][4]), 0);
            ch_stat(stat, 8, -(item_st.ring[temp][6]), 0);
        }
    }

    return 0;
}

int skill_get(Item *item, int code)
{
    int i = 0;

    for (i = 0; i < 10; i++)
    {
        if (item->skill[i] == 0)
            break;
        if (item->skill[i] == code)
            return -1;
    }
    if (i >= 10)
    {
        return -1;
    }
    item->skill[i] = code;

    return 0;
}

int skill_del(Item *item, int code)
{
    int i = 0;

    for (i = 0; i < 10; i++)
    {
        if (item->skill[i] == code)
            break;
    }
    if (i >= 10)
    {
        return -1;
    }
    else
    {
        item->skill[i] = 0;

        for (; i < 9; i++)
        {
            item->skill[i] = item->skill[i + 1];
        }
    }

    slot_del(item, code);

    return 0;
}

int skill_on(Item *item, int code)
{
    int i = 0;

    for (i = 0; i < 5; i++)
    {
        if (item->slot[i] == 0 || item->slot[i] == code)
            break;
    }
    if (i >= 10)
    {
        return -1;
    }
    else
    {
        item->slot[i] = code;
    }

    return 0;
}

int slot_del(Item *item, int code)
{
    int i = 0;

    for (i = 0; i < 5; i++)
    {
        if (item->slot[i] == code)
            break;
    }
    if (i >= 10)
    {
        return -1;
    }
    else
    {
        item->slot[i] = 0;

        for (; i < 4; i++)
        {
            item->slot[i] = item->slot[i + 1];
        }
    }

    return 0;
}

void space(int num, char *alp)
{
    for (int i = 0; i < num; i++) //num값만큼 빈칸 출력
    {
        printf("%s", alp);
    }
}

int randn(int min, int max)
{
    if (max != 0) //max값이 0이 아닌경우 max에서 min사이의 난수 생성
    {
        return rand() % (max - min + 1) + min;
    }
    else //max값이 0인경우 0반환
    {
        return 0;
    }
}

int getch(void)
{
    int num1 = 0, num2 = 0, num3 = 0;
    int result = 0;

    num1 = getkey(0, 0);
    if (num1 == 27)
    {
        num2 = getkey(0, 1);
        num3 = getkey(0, 1);
    }

    if (num1 == 27 && num2 == -1)
    {
        result = 27;
    }
    else if (num2 == 91)
    {
        return num3;
    }
    else
    {
        result = num1;
    }

    return result;
}

int getkey(int is_echo, int timeout)
{
    int ch;
    struct termios old;
    struct termios current;

    tcgetattr(0, &old);
    current = old;
    current.c_lflag &= ~ICANON;

    if (is_echo)
    {
        current.c_lflag |= ECHO;
    }
    else
    {
        current.c_lflag &= ~ECHO;
    }
    current.c_cc[VMIN] = (!timeout);
    current.c_cc[VTIME] = 1;

    tcsetattr(0, TCSANOW, &current);
    ch = getchar();
    tcsetattr(0, TCSANOW, &old);

    return ch;
}