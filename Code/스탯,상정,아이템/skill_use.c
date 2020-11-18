#include "dia3.c"

int skill_use(Stat *stat, Item *item, Boss *boss, int code, int num);
int item_up(Stat *stat, Item *item, int code, int *hp_up, int *mp_up);

int main(void)
{
    Total t={0};
    P *p=&t.p;
    S *s=&t.s;
    Stat *stat=&p->stat;
    Item *item=&p->item;
    Boss *boss=&s->boss;
    int code=0, num, i;
    int mynum=t.p.stat.plnum;
    char str[200]={0};
    int num1=0, num2=0;

    skill_get(item, 701);
    skill_get(item, 702);
    skill_get(item, 703);

    skill_on(item, 701);
    skill_on(item, 702);
    skill_on(item, 703);

    ch_stat(stat, 2, HP_MAX, 0);
    ch_stat(stat, 1, 0, 1);
    ch_stat(stat, 4, MP_MAX, 0);
    ch_stat(stat, 3, 0, 1);
    ch_stat(stat, 6, ATK_MAX, 0);
    ch_stat(stat, 5, ATK_MIN, 0);
    ch_stat(stat, 7, MON, 0);
    
    item->equip[0] = 516; //낡은 한손검
    item->equip[1] = 517; //낡은 한손검
    item->equip[2] = 609; 
    item->equip[3] = 609; 
    item->equip[5] = 210; 


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

    prtstat(*stat);
    for(i=0; i<8; i++)
    {
        item_up(stat,item, item->equip[i], &num1, &num2);
        printf("hp:%d mp:%d\n", num1, num2);
        ch_stat(stat, 1, num1, 1);
        ch_stat(stat, 3, num2, 1);
    }
    prtstat(*stat);
    return 0;
}

int skill_use(Stat *stat, Item *item, Boss *boss, int code, int num)
{
    int temp = 0, i;
    int mynum = stat->plnum;

    switch (code)
    {
    case 701: //화염구
        if (stat->mp[mynum][0] >= SKILL_701_MP_M)
        {
            ch_stat(stat, 3, -SKILL_701_MP_M, 0);
            boss->dmg = randn(stat->atk[0], stat->atk[1]) * SKILL_701_DMG;
        }
        else
        {
            return -1;
        }
        break;
    case 702: //정신자극
        ch_stat(stat, 3, SKILL_702_MP_M, 0);
        break;
    case 703: //응급처치
        ch_stat(stat, 1, SKILL_703_HP_M, 0);
        break;
    case 704: //회전베기
        if (stat->mp[mynum][0] >= SKILL_704_MP_M)
        {
            ch_stat(stat, 3, -SKILL_704_MP_M, 0);
            boss->dmg = randn(stat->atk[0], stat->atk[1]) * SKILL_704_DMG;
            temp=randn(1,100);
            if(temp<=5)
            {
                for(i=0; i<8; i++)
                {
                    stat->hp[i][0]-=boss->dmg/5;
                    if(stat->hp[i][0]<0)
                    {
                        stat->hp[i][0]=0;
                    }
                }
            }
        }
        else
        {
            return -1;
        }
        break;
    case 705:   //방패밀치기
        if (stat->hp[mynum][0] >= stat->hp[mynum][1]/100*SKILL_705_HP_P)
        {
            ch_stat(stat, 1, -SKILL_705_HP_P, 1);
            boss->dmg = stat->hp[mynum][1]/5+stat->def;
        }
        else
        {
            return -1;
        }
        break;
    case 706:   //팔란의 화살비
        if (stat->mp[mynum][0] >= SKILL_706_MP_M)
        {
            ch_stat(stat, 3, -SKILL_706_MP_M, 0);
            temp=randn(0,stat->atk[1]*SKILL_706_DMG);
            boss->dmg = temp;
        }
        else
        {
            return -1;
        }
        break;
    case 707:   //그냥 쌘 기술
        if (stat->mp[mynum][0] >= stat->mp[mynum][1]/100*SKILL_707_MP_P)
        {
            ch_stat(stat, 3, -SKILL_707_MP_P, 1);
            boss->dmg = stat->atk[1] * SKILL_707_DMG;
        }
        else
        {
            return -1;
        }
        break;
    case 708:   //파괴광선
        if (stat->mp[mynum][0] >= SKILL_708_MP_M)
        {
            ch_stat(stat, 3, -SKILL_708_MP_M, 1);
            for (i = 0; i < 3; i++)
            {
                if (stat->con[mynum][i][0] == 708 || stat->con[mynum][i][0] == 0)
                    break;
            }
            if (i < 3)
            {
                stat->con[mynum][i][0] = 708;
                stat->con[mynum][i][1] = SKILL_708_DUR;
            }
            else
            {
                stat->con[mynum][0][0] = 708;
                stat->con[mynum][0][1] = SKILL_708_DUR;
            }
        }
        else
        {
            return -1;
        }
        break;
    case 709: //D.N.P
        ch_stat(stat, 1, -SKILL_709_HP_P, 1);
        boss->dmg = stat->atk[1] * SKILL_709_DMG;
        break;
    case 710:   //은화살
        if (stat->mp[mynum][0] >= SKILL_710_MP_M)
        {
            ch_stat(stat, 3, -SKILL_710_MP_M, 0);
            boss->dmg=randn(stat->atk[0], stat->atk[1]) * SKILL_710_DMG;
            boss->con[0] = 710;
            boss->con[1] = SKILL_711_DUR;
        }
        else
        {
            return -1;
        }
        break;
    case 711:   //맹독폭발
        if (stat->mp[mynum][0] >= SKILL_711_MP_M)
        {
            ch_stat(stat, 3, -SKILL_711_MP_M, 0);
            boss->con[0] = 711;
            boss->con[1] = SKILL_711_DUR;
        }
        else
        {
            return -1;
        }
        break;
    case 712:   //염제불기둥
        if (stat->mp[mynum][0] >= SKILL_712_MP_M)
        {
            ch_stat(stat, 3, -SKILL_712_MP_M, 0);
            boss->dmg=randn(stat->atk[0], stat->atk[1]) * SKILL_712_DMG;
            boss->con[0] = 712;
            boss->con[1] = SKILL_712_DUR;
        }
        else
        {
            return -1;
        }
        break;
    case 713:   //생명의 샘
        if (stat->mp[mynum][0] >= SKILL_713_MP_M)
        {
            ch_stat(stat, 3, -SKILL_713_MP_M, 0);
            /* for (i = 0; i < 8; i++)
            {
                stat->hp[i][0] += randn(stat->atk[0], stat->atk[1]) * SKILL_713_DMG;
                if (stat->hp[i][0] > stat->hp[i][1])
                {
                    stat->hp[i][1] = stat->hp[i][0];
                }
            } */
        }
        else
        {
            return -1;
        }
        break;
    case 714:   //정화
        if (stat->mp[mynum][0] >= SKILL_714_MP_M)
        {
            ch_stat(stat, 3, -SKILL_714_MP_M, 0);
            for (i = 0; i < 3; i++)
            {
                stat->con[num][i][0]=0;
                stat->con[num][i][1]=0;
            }
        }
        else
        {
            return -1;
        }
        break;
    case 715:   //부활
        //미구현
        break;
    case 716:   //괴력무쌍
        if (stat->mp[mynum][0] >= SKILL_716_MP_M)
        {
            ch_stat(stat, 3, -SKILL_716_MP_M, 0);
            for(i=0; i<5; i++)
            {
                if (stat->buf[mynum][i][0] == 716 || stat->buf[mynum][i][0] == 0)
                    break;
            }
            if (i < 5)
            {
                stat->buf[mynum][i][0] = 716;
                stat->buf[mynum][i][1] = SKILL_716_DUR;
            }
            else
            {
                stat->buf[mynum][0][0] = 716;
                stat->buf[mynum][0][1] = SKILL_716_DUR;
            }
        }
        else
        {
            return -1;
        }
        break;
    case 717:   //내면의 힘
        if (stat->mp[mynum][0] >=  stat->mp[mynum][1]/100*SKILL_717_MP_P && stat->hp[mynum][0] >=  stat->hp[mynum][1]/100*SKILL_717_HP_P )
        {
            ch_stat(stat, 3, -SKILL_717_MP_P, 1);
            ch_stat(stat, 1, -SKILL_717_HP_P, 1);
            for (i = 0; i < 5; i++)
            {
                if (stat->buf[mynum][i][0] == 717 || stat->buf[mynum][i][0] == 0)
                    break;
            }
            if (i < 5)
            {
                stat->buf[mynum][i][0] = 717;
                stat->buf[mynum][i][1] = SKILL_717_DUR;
            }
            else
            {
                stat->buf[mynum][0][0] = 717;
                stat->buf[mynum][0][1] = SKILL_717_DUR;
            }
        }
        else
        {
            return -1;
        }
        break;
    case 718:   //피의 욕망
        if (stat->hp[mynum][0] >=  stat->hp[mynum][1]/100*SKILL_718_HP_P )
        {
            ch_stat(stat, 1, -SKILL_718_HP_P, 1);
            for (i = 0; i < 5; i++)
            {
                if (stat->buf[mynum][i][0] == 717 || stat->buf[mynum][i][0] == 0)
                    break;
            }
            if (i < 5)
            {
                stat->buf[mynum][i][0] = 717;
                stat->buf[mynum][i][1] = SKILL_717_DUR;
            }
            else
            {
                stat->buf[mynum][0][0] = 717;
                stat->buf[mynum][0][1] = SKILL_717_DUR;
            }
            //파티부분 확인
        }
        else
        {
            return -1;
        }
        break;
    case 719:   //바위피부
        if (stat->hp[mynum][0] >=  SKILL_719_MP_M)
        {
            ch_stat(stat, 3, -SKILL_719_MP_M, 0);
            for (i = 0; i < 5; i++)
            {
                if (stat->buf[mynum][i][0] == 719 || stat->buf[mynum][i][0] == 0)
                    break;
            }
            if (i < 5)
            {
                stat->buf[mynum][i][0] = 719;
                stat->buf[mynum][i][1] = SKILL_719_DUR;
            }
            else
            {
                stat->buf[mynum][0][0] = 719;
                stat->buf[mynum][0][1] = SKILL_719_DUR;
            }
        }
        else
        {
            return -1;
        }
        break;
    case 720:   //보호막
        if (stat->hp[mynum][0] >=  SKILL_720_MP_M)
        {
            ch_stat(stat, 3, -SKILL_720_MP_M, 0);
            for (i = 0; i < 5; i++)
            {
                if (stat->buf[mynum][i][0] == 720 || stat->buf[mynum][i][0] == 0)
                    break;
            }
            if (i < 5)
            {
                stat->buf[mynum][i][0] = 720;
                stat->buf[mynum][i][1] = SKILL_720_DUR;
            }
            else
            {
                stat->buf[mynum][0][0] = 720;
                stat->buf[mynum][0][1] = SKILL_720_DUR;
            }
        }
        else
        {
            return -1;
        }
        break;
    case 721:   //섬광
        if (stat->hp[mynum][0] >=  SKILL_721_MP_M)
        {
            ch_stat(stat, 3, -SKILL_721_MP_M, 0);
            boss->con[0]=721;
            boss->con[1]=SKILL_721_DUR;
        }
        else
        {
            return -1;
        }
        break;
    case 722:   //도발
        if (stat->hp[mynum][0] >=  SKILL_722_MP_M)
        {
            ch_stat(stat, 3, -SKILL_722_MP_M, 0);
            for (i = 0; i < 3; i++)
            {
                if (stat->con[mynum][i][0] == 722 || stat->con[mynum][i][0] == 0)
                    break;
            }
            if (i < 3)
            {
                stat->con[mynum][i][0] = 722;
                stat->con[mynum][i][1] = SKILL_722_DUR;
            }
            else
            {
                stat->con[mynum][0][0] = 722;
                stat->con[mynum][0][1] = SKILL_722_DUR;
            }
        }
        else
        {
            return -1;
        }
        break;  
    default:
        return -1;
        break;
    }

    return 0;
}

int item_up(Stat *stat, Item *item, int code, int *hp_up, int *mp_up)
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

    if(code==0)
    {
        *hp_up=0;
    }
    else if (code < 100) //소모품
    {
        return 0;
    }
    else if (code < 200) //헬맷
    {
        temp = code % 100 - 1;
        *hp_up = item_st.helm[temp][2];
        *mp_up = item_st.helm[temp][3];
    }
    else if (code < 300) //갑옷
    {
        temp = code % 200 - 1;
        *hp_up = item_st.armor[temp][2];
        *mp_up = item_st.armor[temp][3];
    }
    else if (code < 400) //신발
    {
        temp = code % 300 - 1;
        *hp_up = item_st.shoe[temp][2];
        *mp_up = item_st.shoe[temp][3];
    }
    else if (code < 500) //장갑
    {
        temp = code % 400 - 1;
        *hp_up = item_st.glove[temp][2];
        *mp_up = item_st.glove[temp][3];
    }
    else if (code < 600) //무기
    {
        temp = code % 500 - 1;
        *hp_up = item_st.weapon[temp][2];
        *mp_up = item_st.weapon[temp][3];
    }
    else if (code < 700) //반지
    {
        temp = code % 600 - 1;
        *hp_up = item_st.ring[temp][2];
        *mp_up = item_st.ring[temp][3];
    }

    return 0;
}