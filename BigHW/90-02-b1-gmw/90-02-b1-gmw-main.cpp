/* 朱泽凯 2051995 济美 */
#include "90-02-b1-gmw.h"
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "../include/hehepigIO.h"
#include <Windows.h>
using namespace std;

void test()
{
    STAR_GAME SG;

    while (1) {
        SG.reset(5, 4);
        getchar();

        for (int i = 0; i < 2; i++) {
            SG.focusSwitch(2, 3);
            getchar();

            SG.select(2, 3);
            getchar();

            SG.focusSwitch(2, 3);
            SG.focusSwitch(4, 1);
            getchar();

            SG.select(4, 1);
            getchar();

            SG.confirm();
            getchar();

            SG.down();
            getchar();
        }

    }
}

int main()
{

    //test();

    STAR_GAME SG;

    int r, c;
    int opr, oprRow, oprCol;

    while (1) {

        oprRow = oprCol = 0;
        cct_cls();
        puts("消灭星星\n");
        puts("支持键盘和鼠标操作，键盘操作设置了个边界，不循环了（便于快速到边界）");
        puts("游戏中按 q 可以退出\n");
        r = GetInt("输入行数[4..10]\n >", 4, 10);
        c = GetInt("输入列数[4..10]\n >", 4, 10);

        SG.reset(r, c);

        while (1) {
            opr = SG.getOpr(oprRow, oprCol);

            if (opr == -1 || opr == '\r') {    //左键单击或回车

                if (SG.isSelected(oprRow, oprCol)) {    //确认
                    SG.confirm();
                    SG.down();
                    SG.left();
                }
                else {      //选择
                    SG.select(oprRow, oprCol);
                }
            }
            else if (opr == key_left) {
                SG.focusSwitch(oprRow, oprCol, 0);
                oprCol = max(oprCol - 1, 0);
                SG.focusSwitch(oprRow, oprCol, 1);
            }
            else if (opr == key_right) {
                SG.focusSwitch(oprRow, oprCol, 0);
                oprCol = min(oprCol + 1, c - 1);
                SG.focusSwitch(oprRow, oprCol, 1);
            }
            else if (opr == key_up) {
                SG.focusSwitch(oprRow, oprCol,0);
                oprRow = max(oprRow - 1, 0);
                SG.focusSwitch(oprRow, oprCol,1);
            }
            else if (opr == key_down) {
                SG.focusSwitch(oprRow, oprCol,0);
                oprRow = min(oprRow + 1, r - 1);
                SG.focusSwitch(oprRow, oprCol,1);
            }
            else if (opr == 'q' || opr == 'Q') {    //退出
                break;
            }

            if (SG.checkFail()) {
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