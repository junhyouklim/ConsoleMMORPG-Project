#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include "Shop_S.h"

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//////////////////////[    취 합 용  소 스    ]///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// 스킬 상점 출력
void Shop_S(Stat *stat, Item *item, int *input, int *ctr_mode)
{
    static int show_mode = 0;       // 상점 출력 구분하는 변수
    static int cursor = 0;          // 상점 커서 변수
    static int buy_cur = 0;         // 구매 선택시 Yes NO 커서 변수
    static int buy_mode = 0;        // 구매시 출력 구분하는 변수
    char shop_ui[MAP_Y][200] = {0}; // 출력용 배열 선언
    int start_y = 7;                // 방향키로 선택가능한 메뉴들 시작하는 y좌표
    int menu_cnt;                   // 각 show_mode별 선택지 갯수
    int i;

    if (stat->mode == 4 && *ctr_mode == 0)
    {
        // 각 show_mode별 선택지 방향키이동
        if (*input >= KEY_UP && *input <= KEY_LEFT)
            Shop_S_Move(&cursor, stat->mode, show_mode, input, &buy_mode, &buy_cur);
        // 각 show_mode별 선택지 엔터 or 스페이스바로 선택시 
        if (*input == KEY_ENTER || *input == KEY_SPACE)
            Shop_S_Select(&cursor, &stat->mode, &show_mode, input, &buy_mode, &buy_cur);
    }
    // 구매 선택지에서 YES 선택시
    if (buy_mode == 2) 
    {
        Shop_S_Buy(stat, item, &buy_mode, show_mode, cursor);
    }

    // show_mode에 따라 상점 출력부분 설정
    Init_Shop_S(shop_ui, show_mode, start_y, &menu_cnt, cursor, buy_mode, buy_cur);

    if(stat->mode == 4)
    {
        // 출력
        for (i = 0; i < MAP_Y; i++)
        {
            if (i == 0) Prt_Fix_MapX(0, "top"); // 시작 부분
            else if (i == MAP_Y - 1) Prt_Fix_MapX(0, "bot"); // 끝 부분
            else if (i == start_y - 3) Prt_Fix_MapX(6, shop_ui[i]); // < 스킬북 상점 > 출력 부분
            else if (i == start_y - 1) Prt_Fix_MapX(5, shop_ui[i]); // [ ??? 스킬] 출력 부분 
            else if (i >= start_y && i <= start_y + menu_cnt + 6) Prt_Fix_MapX(4, shop_ui[i]);
            else Prt_Fix_MapX(0, ""); // 따로 설정 안한부분은 50칸 공백으로 출력
            printf("   ");
            prtside(stat, item, &stat->mode, ctr_mode, input, i); // 사이드 UI 출력 부분
            printf("\n");
        } 
    }
}
// 상점에서 방향키 커서 이동 
void Shop_S_Move(int *cursor, int map_mode, int show_mode, int *input, int *buy_mode, int *buy_cur)
{
    int max_menu;

    if (show_mode == 0) max_menu = 1; // 0:스킬북 구매, 1:나가기
    if (show_mode == 1) max_menu = 4; // 0:공격스킬, 1:치유스킬, 2:버프스킬, 3:디버프스킬, 4:뒤로
    if (show_mode == 2) max_menu = 9; // 0 ~ 8: 공격 스킬, 9:뒤로
    if (show_mode == 3) max_menu = 2; // 0 ~ 1: 치유 스킬, 2:뒤로
    if (show_mode == 4) max_menu = 5; // 0 ~ 4: 버프 스킬, 5:뒤로
    if (show_mode == 5) max_menu = 2; // 0 ~ 1: 디버프 스킬, 2:뒤로

    if (*buy_mode >= 3 && *buy_mode <= 5) *buy_mode = 0;
    
    switch (*input)
    {
    case KEY_UP:
        if (*buy_mode == 0)
        {
            *cursor -= 1;
            if (*cursor < 0)
                *cursor = 0;
        }
        break;
    case KEY_DOWN:
        if (*buy_mode == 0)
        {
            *cursor += 1;
            if (*cursor > max_menu)
                *cursor = max_menu;
        }
        break;
    case KEY_LEFT: // 구매 YES , NO 커서 이동
        if (*buy_mode != 0)
        {
            *buy_cur -= 1;
            if (*buy_cur < 0)
                *buy_cur = 0;
        }
        break;
    case KEY_RIGHT: // 구매 YES , NO 커서 이동
        if (*buy_mode != 0)
        {
            *buy_cur += 1;
            if (*buy_cur > 1)
                *buy_cur = 1;
        }
    }
    // 입력 값 초기화가 안되면 쓰레드 돌면서 키 값이 계속 적용됨
    *input = 0;
}
// 상점에서 스페이스바, 엔터 입력으로 선택
void Shop_S_Select(int *cursor, int *map_mode, int *show_mode, int *input, int *buy_mode, int *buy_cur)
{
    
    if (*buy_mode == 0)
    {   // 상점 모드에따라 메뉴가 다르기 때문에 선택시 각종 모드 변경
        switch (*show_mode)
        {
        case 0: // 상점 입장 > cursor = 0:스킬북 구매, 1:나가기
            if (*cursor == 0) *show_mode = 1, *cursor = 0;
            if (*cursor == 1) *map_mode = 0, *cursor = 0;
            break;
        case 1: // 구매 > cursor = 0:공격스킬 1:치유스킬 2:버프스킬 3:디버프 스킬 4:뒤로
            if (*cursor == 0) *show_mode = 2, *cursor = 0;
            if (*cursor == 1) *show_mode = 3, *cursor = 0;
            if (*cursor == 2) *show_mode = 4, *cursor = 0;
            if (*cursor == 3) *show_mode = 5, *cursor = 0;
            if (*cursor == 4) *show_mode = 0, *cursor = 0;
            break;
        case 2: // 구매 > 공격 스킬 > cursor = 0 ~ 8: 공격 스킬  9:뒤로
            if (*cursor >= 0 && *cursor <= 8) *buy_mode = 1;
            if (*cursor == 9) *show_mode = 1, *cursor = 0;
            break;
        case 3: // 구매 > 치유 스킬 > cursor = 0 ~ 1: 치유 스킬 2:뒤로
            if (*cursor >= 0 && *cursor <= 1) *buy_mode = 1;
            if (*cursor == 2) *show_mode = 1, *cursor = 0;
            break;
        case 4: // 구매 > 버프 스킬 > cursor = 0 ~ 4: 버프 스킬 5:뒤로
            if (*cursor >= 0 && *cursor <= 4) *buy_mode = 1;
            if (*cursor == 5) *show_mode = 1, *cursor = 0;
            break;
        case 5: // 구매 > 디버프 스킬 > cursor = 0 ~ 1: 디버프 스킬 2:뒤로
            if (*cursor >= 0 && *cursor <= 1) *buy_mode = 1;
            if (*cursor == 2) *show_mode = 1, *cursor = 0;
            break;
        }
    }
    else
    {
        if (*buy_mode == 1) // 스킬북에서 엔터or스페이스바 했을떄
        {
            if (*buy_cur == 0) // 구매 > YES
            {
                *buy_mode = 2; // 값 2 > Shop_S_Buy 함수 실행 조건
                *buy_cur = 0;
            }
            if (*buy_cur == 1) // 구매 > NO
            {
                *buy_mode = 0;
                *buy_cur = 0;
            }
        }
        else
        {
            *buy_mode = 0;
        }
    }
    // 입력 값 초기화가 안되면 쓰레드 돌면서 키 값이 계속 적용됨
    *input = 0;
}
// 스킬북 구매 가능한지 판단하고 결과 값 buy_mode에 저장하는 함수
void Shop_S_Buy(Stat *stat, Item *item, int *buy_mode, int show_mode, int cursor)
{
    int temp, i;
    int sk_cnt, sk_code, pick_code;
    int price = 0, num = 1;

    if (show_mode == 2)
    {                  // 704 ~ 712
        sk_cnt = 9;    // 공격 스킬 9개
        sk_code = 704; // 시작하는 코드
    }
    if (show_mode == 3)
    {                  // 713 ~ 715
        sk_cnt = 2;    // 치유 스킬 2개
        sk_code = 713; // 시작하는 코드
    }
    if (show_mode == 4)
    {                  // 716 ~ 720
        sk_cnt = 5;    // 버프 스킬 5개
        sk_code = 716; // 시작하는 코드
    }
    if (show_mode == 5)
    {                  // 721 ~ 722
        sk_cnt = 2;    // 디버프 스킬 2개
        sk_code = 721; // 시작하는 코드
    }

    // 구매창에서 선택한 스킬코드 찾음
    for (i = 0; i < sk_cnt; i++)
    {
        if (cursor == i)
            pick_code = sk_code + i;
    }
    // 가격 가져옴
    itemprice(&price, pick_code); 
    // 돈 충분한지 검사 
    if (stat->mon >= price)
    {
        temp = item_get(item, pick_code, num);
        if (temp == 0) // 구매 성공
        {
            *buy_mode = 3; // 구매 성공 출력위해 변수 값 설정
            stat->mon -= price;
        }
        else // 소지품 공간 없음
        {
            *buy_mode = 4; // 소지품 공간 없다고 출력위해 변수 값 설정
        }
    }
    else // 돈 없음
    {
        *buy_mode = 5; // 돈 부족하다고 출력위해 변수 값 설정
    }
}
// 상점 출력 배열을 조건에 맞게 셋팅 하는 함수 
void Init_Shop_S(char (*shop_ui)[200], int show_mode, int start_y, int *menu_cnt, int cursor, int buy_mode, int buy_cur)
{
    char temp[200] = {0};
    char temp_name[30] = {0}; // 스킬북 이름
    int temp_price = 0;     // 스킬북 가격
    int sk_cnt = 0;         // 각 카테고리별 스킬 갯수
    int sk_code = 0;        // 각 카테고리별 시작 스킬 코드 ex) 공격:704, 치유:713
    int i;
    int set_cur;

    strcpy(shop_ui[start_y - 3], "< 스킬북 상점 >");
    switch (show_mode)
    {
    case 0: // 상점 입장
        *menu_cnt = 2; // 스킬북 구매, 나가기
        strcpy(shop_ui[start_y], "스킬북 구매");
        strcpy(shop_ui[start_y + 2], "나가기");
        break;
    case 1: // 구매
        *menu_cnt = 5; // 공격스킬, 치유스킬, 버프스킬, 디버프스킬, 뒤로
        strcpy(shop_ui[start_y], "공격 스킬");
        strcpy(shop_ui[start_y + 1], "치유 스킬");
        strcpy(shop_ui[start_y + 2], "버프 스킬");
        strcpy(shop_ui[start_y + 3], "디버프 스킬");
        strcpy(shop_ui[start_y + 5], "뒤로");
        break;
    case 2: // 구매 > 공격 스킬
    case 3: // 구매 > 치유 스킬
    case 4: // 구매 > 버프 스킬
    case 5: // 구매 > 디버프 스킬
        if (show_mode == 2)
        { // 704 ~ 712
            strcpy(shop_ui[start_y - 1], "[ 공격 스킬 ]");
            *menu_cnt = 10; // 공격 스킬 9개 , 뒤로
            sk_cnt = 9;     // 공격 스킬 9개
            sk_code = 704;  // 시작하는 코드
        }
        if (show_mode == 3)
        { // 713 ~ 715
            strcpy(shop_ui[start_y - 1], "[ 치유 스킬 ]");
            *menu_cnt = 3; // 치유 스킬 2개 , 뒤로
            sk_cnt = 2;    // 치유 스킬 2개
            sk_code = 713; // 시작하는 코드
        }
        if (show_mode == 4)
        { // 716 ~ 720
            strcpy(shop_ui[start_y - 1], "[ 버프 스킬 ]");
            *menu_cnt = 6; // 버프 스킬 5개 , 뒤로
            sk_cnt = 5;    // 버프 스킬 5개
            sk_code = 716; // 시작하는 코드
        }
        if (show_mode == 5)
        { // 721 ~ 722
            strcpy(shop_ui[start_y - 1], "[ 디버프 스킬 ]");
            *menu_cnt = 3; // 디버프 스킬 2개 , 뒤로
            sk_cnt = 2;    // 디버프 스킬 2개
            sk_code = 721; // 시작하는 코드
        }

        // 스킬 이름과 가격 출력 배열에 저장 
        for (i = 0; i < sk_cnt; i++)
        {
            itemcode(temp_name, sk_code + i);    // 아이템 이름 가져옴
            Prt_Fix_Item(temp_name);             // 아이템 이름 출력사이즈 동일하게 맞춤
            itemprice(&temp_price, sk_code + i); // 아이템 가격 가져옴
            sprintf(shop_ui[start_y + i], "%s %5d G", temp_name, temp_price); // 형식 지정해서 출력 배열에 저장
        }
        strcpy(shop_ui[start_y + sk_cnt + 1], "뒤로");

        // 하단에 스킬 설명
        for (i = 0; i < *menu_cnt-1; i++)
        {
            if (cursor == i)
            {
                iteminf(temp, sk_code + i, 0); // 스킬설명 첫번째줄
                strcpy(shop_ui[start_y + sk_cnt + 3], temp);
                iteminf(temp, sk_code + i, 1); // 스킬설명 두번째 줄
                strcpy(shop_ui[start_y + sk_cnt + 4], temp);
            }
        }
        break;
    }
    // 현재 커서 위치에 > 표시 추가
    for (i = 0; i < *menu_cnt; i++)
    {
        if (i != *menu_cnt - 1)
            set_cur = start_y + i;
        else
            set_cur = start_y + i + 1; // 커서의 마지막인 나가기, 뒤로는 한칸 비우고 출력 

        memset(temp, 0, sizeof(temp));
        strcpy(temp, shop_ui[set_cur]);
        if (cursor == i)
            sprintf(shop_ui[set_cur], "> %s", temp); // 현재 커서 위치 표시
        else
            sprintf(shop_ui[set_cur], "  %s", temp); 
    }

    // 하단 구매 관련 부분 설정
    if (buy_mode == 1)
    {
        for (i = 0; i < *menu_cnt; i++)
        {
            if (cursor == i)
            {
                itemcode(temp_name, sk_code + i); // 아이템 이름 가져옴
                sprintf(shop_ui[start_y + *menu_cnt + 5], "[%s] 스킬북을 구매 하시겠습니까?", temp_name);
                if (buy_cur == 0) strcpy(shop_ui[start_y + *menu_cnt + 6], "> YES    NO");
                if (buy_cur == 1) strcpy(shop_ui[start_y + *menu_cnt + 6], "  YES  > NO");
            }
        }
    }
    if (buy_mode == 3) strcpy(shop_ui[start_y + *menu_cnt + 5], "구매 완료");
    if (buy_mode == 4) strcpy(shop_ui[start_y + *menu_cnt + 5], "인벤토리 공간이 없습니다");
    if (buy_mode == 5) strcpy(shop_ui[start_y + *menu_cnt + 5], "소지금이 부족합니다");
}
// 아이템 이름 출력부분 공백 맞추는 함수
void Prt_Fix_Item(char *item_name)
{
    // 아이템이름 20칸으로 고정
    // 한글은 2칸씩 차지, 공백 및 아스키코드 문자는 1칸
    // 한자키를 이용한 특수문자는 사용 x > 판단 불가 
    int max_len = 20; // 수치 변경하면 그 사이즈로 고정 
    int kor=0, other=0, i, len;

    len = strlen(item_name);
    for (i = 0; i < len; i++)
    {
        if (item_name[i] & 0x80)
            kor++;
        else
            other++;
    }
    kor /= 3;
    max_len -= kor * 2;
    max_len -= other;
    
    // max_len에서 부족한 칸 만큼 이름 뒤에 공백 추가 
    for (i = 0; i < max_len; i++)
    {
        item_name[len] = ' ';
        item_name[len + 1] = '\0';
        len++;
    }
}
// 상점 출력할때 가로 사이즈 맞추는 함수
void Prt_Fix_MapX(int start, char *str)
{
    int len = 0;
    int i, kor = 0, other = 0;
    int empty = 0;

    if (!strcmp(str, "top"))
    {
        printf("┌─");
        for (i = 0; i < MAP_X - 2; i++)
            printf("──");
        printf("─┐");
    }
    else if (!strcmp(str, "bot"))
    {
        printf("└─");
        for (i = 0; i < MAP_X - 2; i++)
            printf("──");
        printf("─┘");
    }
    else
    {
        len = strlen(str);
        for (i = 0; i < len; i++)
        {
            if (str[i] & 0x80)
                kor++;
            else
                other++;
        }
        printf("│ ");
        for (i = 0; i < start; i++)
            printf("  ");
        for (i = 0; i < len; i++)
            printf("%c", str[i]);

        empty = MAP_X * 2 - 4; // 100칸 기준으로 양 끝 "│ "(2칸), " │"(2칸) 뺌 
        kor /= 3; // 한글이 3바이트이기 때문에 3으로 나누면 한글 몇 글자인지 알 수 있음
        empty -= (start * 2); // 0 ~ start까지 ("  "); 두 칸씩 잡았기 때문에 *2
        empty -= (kor * 2); // 한글은 한 글자가 출력할때 두 칸씩 차지하기 때문에 *2
        empty -= other; // 나머지는 한 칸씩 차지함   

        for (i = 0; i < empty; i++) // 남은 공간만큼 1칸씩 더 출력 
            printf(" ");

        printf(" │");
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//////////////////////[    취 합 용  소 스    ]///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
