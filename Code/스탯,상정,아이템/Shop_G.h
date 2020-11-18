#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

//////////////////////// 합칠때 넘길 함수들
void Shop_G(Stat *stat, Item *item, int *input, int *ctr_mode);
void Shop_G_Move(int *cursor, int map_mode, int show_mode, int *input, int *buy_mode, int *buy_cur);
void Shop_G_Select(int *cursor, int *map_mode, int *show_mode, int *input, int *buy_mode, int *buy_cur);
void Shop_G_Buy(Stat *stat, Item *item, int *buy_mode, int show_mode, int cursor);
void Init_Shop_G(char (*shop_ui)[200], int show_mode, int start_y, int *menu_cnt, int cursor, int buy_mode, int buy_cur);
///////////////////////
