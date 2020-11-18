#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include "dia.h"

int main(void)
{
    Total total={0};
    Stat *stat = &total.p.stat;
    Item *item = &total.p.item;
    int mynum = total.p.stat.plnum;
    int input, i = 0;
    int map_mode = 0;
    int ctr_mode = 0;

    skill_get(item, 701);
    skill_get(item, 702);
    skill_get(item, 703);

    skill_on(item, 701);
    skill_on(item, 702);
    skill_on(item, 703);

    ch_stat(stat, 2, HP_MAX, 0);
    ch_stat(stat, 1, 50, 1);
    ch_stat(stat, 4, MP_MAX, 0);
    ch_stat(stat, 3, 50, 1);
    ch_stat(stat, 6, ATK_MAX, 0);
    ch_stat(stat, 5, ATK_MIN, 0);
    ch_stat(stat, 7, MON, 0);
    
    item->equip[0] = 501; //낡은 한손검
    item_stat(stat, item, 501, 0);

    strcpy(stat->name[mynum], "나");

    item_get(item, 1, 2);
    item_get(item, 2, 3);
    item_get(item, 3, 2);
    item_get(item, 501, 2);
    item_get(item, 513, 2);
    item_get(item, 507, 2);
    item_get(item, 511, 2);
    item_get(item, 504, 2);
    item_get(item, 706, 2);
    item_get(item, 603, 1);
    item_get(item, 602, 2);
    item_get(item, 603, 2);
    item_get(item, 604, 2);
    item_get(item, 605, 2);
    item_get(item, 606, 2);
    item_get(item, 201, 2);
    item_get(item, 302, 2);
    item_get(item, 404, 2);

    while (!(input == KEY_ESC))
    {
        system("clear");
        prtstat(*stat);
        if(total.p.stat.mode==2)
        {
            Shop_B(item, stat, &input, &ctr_mode);
        }
        if(total.p.stat.mode==3)
        {
            Shop_G(stat, item, &input, &ctr_mode);
        }
        if(total.p.stat.mode==4)
        {
            Shop_S(stat, item,&input, &ctr_mode);
        }
        if(total.p.stat.mode==0)
        {
            printf("map\n");
        }
        input = getch();
        if(input==KEY_2)
        {
            total.p.stat.mode=2;
        }
        else if(input==KEY_3)
        {
            total.p.stat.mode=3;
        }
        else if(input==KEY_4)
        {
            total.p.stat.mode=4;
        }
        usleep(16*1000);
    }
    return 0;


}