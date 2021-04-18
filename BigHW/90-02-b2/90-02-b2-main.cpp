/* 朱泽凯 2051995 济美 */

#include "90-02-b2.h"
#include "../include/hehepigIO.h"
#include "../include/cmd_console_tools.h"
#include <iostream>
#include <Windows.h>
using namespace std;

void beginHint()
{
    puts("合成十说明");
    puts("");
    puts("============================================");
    puts("支持键盘鼠标操作\n");
    puts("鼠标点击进行操作，键盘使用方向键与回车等其它键进行操作\n");
    puts("键盘操作设置了个边界，不循环了（便于快速到边界）\n");
    puts("选择（回车或鼠标左键点击）某一块后，若可以合成，会把合成区域高亮显示");
    puts("若再次选择的是高亮区域内的块，会将该区域消除，并在这次选择的位置合成一个大1的数；若再次选择的其它区域的块，则视为重新选择合成区域，上一次选择作废废\n");
    puts("分数规则：每次合成所得分：新生成的数*消除的区域大小");
    puts("============================================\n");
    puts("游戏中按 q 可以退出\n");

    cout << "按q退出，其他键继续..." << endl;
    int opr = GetKey();
    if (opr == 'q' || opr == 'Q') {
        exit(0);
    }

}

int main()
{

    beginHint();

    HCS_GAME HG;

    int r, c, n;
    int opr, oprRow, oprCol;

    while (1) {

        oprRow = oprCol = 0;
        cct_cls();
        r = GetInt("输入行数[5..15]\n >", 4, 15);
        c = GetInt("输入列数[5..15]\n >", 4, 15);
        n = GetInt("输入合成目标数[5..10]\n >", 5, 10);

        HG.reset(r, c, n);

        while (1) {
            opr = HG.getOpr(oprRow, oprCol);

            if (opr == -1 || opr == '\r') {     //左键单击或回车
                
                if (HG.isSelected(oprRow, oprCol)) {    //确认，消除
                    int tmp = HG.getVal(oprRow, oprCol) +1 ;    //将要添加的数字
                    int cnt;

                    cnt = HG.confirm();   //消除

                    HG.addScore(tmp * cnt);

                    for (int i = 0; i < 5; i++) {       //反馈动画
                        HG.setVal(oprRow, oprCol, 0);
                        Sleep(50);
                        HG.setVal(oprRow, oprCol, tmp);
                        Sleep(50);
                    }
                    HG.setVal(oprRow, oprCol, tmp);     //合成

                    HG.down();  //下降补充
                }
                else {  //选择
                    HG.select(oprRow, oprCol);
                }
            }
            else if (opr == key_left) {
                HG.focusSwitch(oprRow, oprCol, 0);
                oprCol = max(oprCol - 1, 0);
                HG.focusSwitch(oprRow, oprCol, 1);
            }
            else if (opr == key_right) {
                HG.focusSwitch(oprRow, oprCol, 0);
                oprCol = min(oprCol + 1, c - 1);
                HG.focusSwitch(oprRow, oprCol, 1);
            }
            else if (opr == key_up) {
                HG.focusSwitch(oprRow, oprCol, 0);
                oprRow = max(oprRow - 1, 0);
                HG.focusSwitch(oprRow, oprCol, 1);
            }
            else if (opr == key_down) {
                HG.focusSwitch(oprRow, oprCol, 0);
                oprRow = min(oprRow + 1, r - 1);
                HG.focusSwitch(oprRow, oprCol, 1);
            }
            else if (opr == 'q' || opr == 'Q') {    //退出
                cct_cls();
                break;
            }

            if (HG.checkWin()) {
                break;
            }
        }

        cout << "按q退出，其他键新局" << endl;
        opr = GetKey();
        if (opr == 'q' || opr == 'Q') {
            break;
        }

    }
}