/* 朱泽凯 2051995 济美 */
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include "90-01-b2-gmw.h"
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "../include/hehepigIO.h"
using namespace std;


void test()
{
    while (1) {
        MINE_GAME mg(10, 10, 5);
        mg.setBombs(0, 0);
        getchar();

        mg.markSwitch(1, 3);
        mg.markSwitch(3, 4);
        getchar();

        mg.clickZone(3, 4);
        getchar();

        mg.markSwitch(2, 4);
        getchar();

        mg.checkMark(3, 4);
        getchar();

        mg.showBombs();
        getchar();

        mg.showTime();
        getchar();
    }
}


void beginHint()
{
    puts("扫雷");
    puts("");
    puts("操作说明：");
    puts("");
    puts("左键单击：点开（若是雷就输了）");
    puts("右键单击：插旗");
    puts("滚轮单击：检查（在已经点开了的土地上才能检查）");
    puts("键盘 t  ：显示用时");
    puts("键盘 q  ：退出");
    puts("");
    puts("按回车以继续...");
    GetKey('\r', '\r');
}

int main()
{

    beginHint();

    int r=0, c=0, b=0;
    int oprRow =0, oprCol =0, opr=0;
    MINE_GAME *pMG;
    while (1) {
        cct_cls();
        r = GetInt("输入行数[4..18]\n >", 4, 18);
        c = GetInt("输入列数[4..35]\n >", 4, 35);
        char tmp[50];
        sprintf(tmp, "输入雷数[1..%d]\n >", r * c - 9);
        b = GetInt(tmp, 1, r * c - 1);

        pMG = new MINE_GAME(r, c, b);

        while ((opr = pMG->GetOpr(oprRow, oprCol)) != -1) {
            if (opr == -4 || opr == 'q' || opr == 'Q') {
                delete pMG;
                return 0;
            }
            continue;
        }
        pMG->setBombs(oprRow, oprCol);

        while (!pMG->win()) {
            opr = pMG->GetOpr(oprRow, oprCol);

            if (opr == -1) {    //左键单击，点开
                if (pMG->clickZone(oprRow, oprCol))
                    break;
            }
            else if (opr == -2) {   //右键单击，标记
                pMG->markSwitch(oprRow, oprCol);
            }
            else if (opr == -3) {   //滚轮单击，检查
                if (pMG->checkMark(oprRow, oprCol))
                    break;
            }
            else if (opr == 't' || opr == 'T') {    //t，显示时间
                pMG->showTime();
            }
            else if (opr == -4 || opr == 'q' || opr == 'Q') {   //退出
                delete pMG;
                return 0;
                return 0;
            }

            pMG->showCnt();
        }
        pMG->showBombs();

        pMG->lower_hint("游戏结束，按回车新局...");
        GetKey('\r', '\r');

        delete pMG;
    }
    return 0;
}