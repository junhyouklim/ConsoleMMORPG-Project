#include "Shop_G.h"

// 잡화상점 UI
void Shop_G(Stat *stat, Item *item, int *input, int *ctr_mode)
{
    static int show_mode = 0;       // 상점 출력 구분하는 변수
    static int cursor = 0;          // 상점 커서 변수
    static int buy_cur = 0;         // 구매 선택시 Yes NO 커서 변수
    static int buy_mode = 0;        // 구매시 출력 구분하는 변수
    char shop_ui[MAP_Y][200] = {0}; // 출력용 배열 선언
    int start_y = 7;                // 방향키로 선택가능한 메뉴들 시작하는 y좌표
    int menu_cnt;                   // 각 show_mode별 선택지 갯수
    int i;

    if (stat->mode == 3 && *ctr_mode == 0)
    {
        // 각 show_mode별 선택지 방향키이동
        if (*input >= KEY_UP && *input <= KEY_LEFT)
            Shop_G_Move(&cursor, stat->mode, show_mode, input, &buy_mode, &buy_cur);
        // 각 show_mode별 선택지 엔터 or 스페이스바로 선택시
        if (*input == KEY_ENTER || *input == KEY_SPACE)
            Shop_G_Select(&cursor, &stat->mode, &show_mode, input, &buy_mode, &buy_cur);
    }
    // 구매 선택지에서 YES 선택시
    if (buy_mode == 2)
    {
        Shop_G_Buy(stat, item, &buy_mode, show_mode, cursor);
    }

    // show_mode에 따라 상점 출력부분 설정
    Init_Shop_G(shop_ui, show_mode, start_y, &menu_cnt, cursor, buy_mode, buy_cur);

    if (stat->mode == 3)
    {
        // 출력
        for (i = 0; i < MAP_Y; i++)
        {
            if (i == 0)
                Prt_Fix_MapX(0, "top"); // 시작 부분
            else if (i == MAP_Y - 1)
                Prt_Fix_MapX(0, "bot"); // 끝 부분
            else if (i == start_y - 3)
                Prt_Fix_MapX(6, shop_ui[i]); // < 잡화 상점 > 출력 부분
            else if (i == start_y - 1)
                Prt_Fix_MapX(5, shop_ui[i]); // [ 품목 ] 출력 부분
            else if (i >= start_y && i <= start_y + menu_cnt + 6)
                Prt_Fix_MapX(4, shop_ui[i]);
            else
                Prt_Fix_MapX(0, ""); // 따로 설정 안한부분은 50칸 공백으로 출력
            printf("   ");
            prtside(stat, item, &stat->mode, ctr_mode, input, i); // 사이드 UI 출력 부분
            printf("\n");
        }
    }
}

// 상점 출력 배열을 조건에 맞게 셋팅 하는 함수
void Init_Shop_G(char (*shop_ui)[200], int show_mode, int start_y, int *menu_cnt, int cursor, int buy_mode, int buy_cur)
{
    char temp[200] = {0};
    char temp_name[30] = {0}; // 잡화 이름
    int temp_price = 0;       // 잡화 가격
    int pr_cnt = 0;           // 품목별 갯수
    int pr_code = 0;          // 시작 코드 ex) 포션:001, 반지:601
    int i;
    int set_cur;

    strcpy(shop_ui[start_y - 3], "< 잡화 상점 >");
    switch (show_mode)
    {
    case 0:            // 상점 입장
        *menu_cnt = 2; // 물품 구매, 나가기
        strcpy(shop_ui[start_y], "들어가기");
        strcpy(shop_ui[start_y + 2], "나가기");
        break;
    case 1:            // 구매
        *menu_cnt = 3; // 포션, 반지, 뒤로
        strcpy(shop_ui[start_y], "포션");
        strcpy(shop_ui[start_y + 1], "반지");
        strcpy(shop_ui[start_y + 3], "뒤로가기");
        break;
    case 2: // 구매 > 포션
    case 3: // 구매 > 반지
        if (show_mode == 2)
        { // 704 ~ 712
            strcpy(shop_ui[start_y - 1], "[ 포션 ]");
            *menu_cnt = 6; // 포션 5개 , 뒤로
            pr_cnt = 5;    // 포션 5개
            pr_code = 1; // 시작하는 코드
        }
        if (show_mode == 3)
        { // 713 ~ 715
            strcpy(shop_ui[start_y - 1], "[ 반지 ]");
            *menu_cnt = 9; // 반지 8개 , 뒤로
            pr_cnt = 8;     // 반지 8개
            pr_code = 601;  // 시작하는 코드
        }

        // 물품 이름과 가격 출력 배열에 저장
        for (i = 0; i < pr_cnt; i++)
        {
            itemcode(temp_name, pr_code + i);                                 // 아이템 이름 가져옴
            Prt_Fix_Item(temp_name);                                          // 아이템 이름 출력사이즈 동일하게 맞춤
            itemprice(&temp_price, pr_code + i);                              // 아이템 가격 가져옴
            sprintf(shop_ui[start_y + i], "%s %5d G", temp_name, temp_price); // 형식 지정해서 출력 배열에 저장
        }
        strcpy(shop_ui[start_y + pr_cnt + 1], "뒤로");

        // 하단에 아이템 설명
        for (i = 0; i < *menu_cnt - 1; i++)
        {
            if (cursor == i)
            {
                iteminf(temp, pr_code + i, 0); // 아이템설명 첫번째줄
                strcpy(shop_ui[start_y + pr_cnt + 3], temp);
                iteminf(temp, pr_code + i, 1); // 아이템설명 두번째 줄
                strcpy(shop_ui[start_y + pr_cnt + 4], temp);
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
                itemcode(temp_name, pr_code + i); // 아이템 이름 가져옴
                sprintf(shop_ui[start_y + *menu_cnt + 5], "[%s] 을(를) 구매 하시겠습니까?", temp_name);
                if (buy_cur == 0)
                    strcpy(shop_ui[start_y + *menu_cnt + 6], "> YES    NO");
                if (buy_cur == 1)
                    strcpy(shop_ui[start_y + *menu_cnt + 6], "  YES  > NO");
            }
        }
    }
    if (buy_mode == 3)
        strcpy(shop_ui[start_y + *menu_cnt + 5], "구매 완료");
    if (buy_mode == 4)
        strcpy(shop_ui[start_y + *menu_cnt + 5], "인벤토리 공간이 없습니다");
    if (buy_mode == 5)
        strcpy(shop_ui[start_y + *menu_cnt + 5], "소지금이 부족합니다");
}

// 상점에서 방향키 커서 이동
void Shop_G_Move(int *cursor, int map_mode, int show_mode, int *input, int *buy_mode, int *buy_cur)
{
    int max_menu;

    if (show_mode == 0)
        max_menu = 1; // 0:물품 구매, 1:나가기
    if (show_mode == 1)
        max_menu = 2; // 0:포션, 1:반지, 2:뒤로
    if (show_mode == 2)
        max_menu = 5; // 0 ~ 4: 포션, 5:뒤로
    if (show_mode == 3)
        max_menu = 8; // 0 ~ 7: 반지, 8:뒤로

    if (*buy_mode >= 3 && *buy_mode <= 5)
        *buy_mode = 0;

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
void Shop_G_Select(int *cursor, int *map_mode, int *show_mode, int *input, int *buy_mode, int *buy_cur)
{
    if (*buy_mode == 0)
    { // 상점 모드에따라 메뉴가 다르기 때문에 선택시 각종 모드 변경
        switch (*show_mode)
        {
        case 0: // 상점 입장 > cursor = 0:구매하기, 1:나가기
            if (*cursor == 0)
                *show_mode = 1, *cursor = 0;
            if (*cursor == 1)
                *map_mode = 0, *cursor = 0;
            break;
        case 1: // 구매 > cursor = 0:포션 1:반지 2:뒤로가기
            if (*cursor == 0)
                *show_mode = 2, *cursor = 0;
            if (*cursor == 1)
                *show_mode = 3, *cursor = 0;
            if (*cursor == 2)
                *show_mode = 0, *cursor = 0;
            break;
        case 2: // 구매 > 포션 > cursor = 0 ~ 4: 포션  5:뒤로
            if (*cursor >= 0 && *cursor <= 4)
                *buy_mode = 1;
            if (*cursor == 5)
                *show_mode = 1, *cursor = 0;
            break;
        case 3: // 구매 > 반지 > cursor = 0 ~ 7: 반지 8:뒤로
            if (*cursor >= 0 && *cursor <= 7)
                *buy_mode = 1;
            if (*cursor == 8)
                *show_mode = 1, *cursor = 0;
            break;
        }
    }
    else
    {
        if (*buy_mode == 1) // 아이템에서 엔터or스페이스바 했을떄
        {
            if (*buy_cur == 0) // 구매 > YES
            {
                *buy_mode = 2; // 값 2 > Shop_G_Buy 함수 실행 조건
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
void Shop_G_Buy(Stat *stat, Item *item, int *buy_mode, int show_mode, int cursor)
{
    int temp, i;
    int pr_cnt, pr_code, pick_code;
    int price = 0, num = 1;

    if (show_mode == 2)
    {                // 001~005
        pr_cnt = 5;  // 포션 5개
        pr_code = 1; // 시작하는 코드
    }
    if (show_mode == 3)
    {                  // 601~608
        pr_cnt = 8;    // 반지 8개
        pr_code = 601; // 시작하는 코드
    }

    // 구매창에서 선택한 아이템 코드 찾음
    for (i = 0; i < pr_cnt; i++)
    {
        if (cursor == i)
            pick_code = pr_code + i;
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

