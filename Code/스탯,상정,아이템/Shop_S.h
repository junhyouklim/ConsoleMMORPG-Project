//////////////////////////////////////////////////////////////////////////// 합칠때 넘길 함수들 
void Shop_S(Stat *stat, Item *item, int *input, int *ctr_mode);
void Shop_S_Move(int *cursor, int map_mode, int show_mode, int *input, int *buy_mode, int *buy_cur);
void Shop_S_Select(int *cursor, int *map_mode, int *show_mode, int *input, int *buy_mode, int *buy_cur);
void Shop_S_Buy(Stat *stat, Item *item, int *buy_mode, int show_mode, int cursor);
void Init_Shop_S(char (*shop_ui)[200], int show_mode, int start_y, int *menu_cnt, int cursor, int buy_mode, int buy_cur);
void Prt_Fix_Item(char *item_name); // 아이템 출력 사이즈 고정 
void Prt_Fix_MapX(int start, char *str); // 가로 고정 사이즈로 출력
//////////////////////////////////////////////////////////////////////////////////////////////