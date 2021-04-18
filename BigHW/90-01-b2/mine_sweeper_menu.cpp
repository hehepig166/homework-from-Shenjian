/* 朱泽凯 2051995 济美 */
#include <iostream>
#include "../include/cmd_console_tools.h"
#include "../90-01-b2/mine_sweeper.h"
#include "../include/hehepigMenu.h"
using namespace std;

/****************************************************************
* 功能：	读入一个 ASCII 码属于 L~R 范围的字符（单字符），输出来
* 返回值：	读入的字符的 ASCII 码
* 注：		不包含双字符（如方向键、fx之类）
*			但包含 Esc (ASCII= 0x1b)
* ***************************************************************/

GMDATA MENU()
{

    GMDATA ret;

    /*
    cct_cls();
    puts("------------------------------------------");
    puts("1.选择难度并显示内部数组");
    puts("2.输入初始位置并显示被打开的初始区域");
    puts("3.内部数组基础版");
    puts("4.内部数组完整版（标记、运行时间）");
    puts("5.画出伪图形化框架并显示内部数据");
    puts("6.检测鼠标位置和合法性（(在合法区域)左键单击退出）");
    puts("7.鼠标选择初始位置并显示被打开的初始区域");
    puts("8.伪图形界面基础版");
    puts("9.伪图形界面完整版");
    puts("0.退出游戏");
    puts("------------------------------------------");
    cout << ("[请选择] ：");
    ret.mode = my_getch('0', '9') - '0';
    cout << ret.mode;
    */
    cct_cls();
    const char *Str[] = {
        "选择难度并显示内部数组",
        "输入初始位置并显示被打开的初始区域",
        "内部数组基础版",
        "内部数组完整版（标记、运行时间）",
        "画出伪图形化框架并显示内部数据",
        "检测鼠标位置和合法性（(在合法区域)左键单击退出）",
        "鼠标选择初始位置并显示被打开的初始区域",
        "伪图形界面基础版",
        "伪图形界面完整版",
        "退出游戏"
    };
    ret.mode = Menu_char(10, "1234567890", Str) - '0';
    if (ret.mode == 0)
        return ret;

    /*
    cct_cls();
    puts("请选择难度：");
    puts("1.初级(9 * 9 - 10颗雷)");
    puts("2.中级(16 * 16 - 40颗雷)");
    puts("3.高级(16 * 30 - 99颗雷)");
    cout << ("请输入[1..3]：");
    ret.level = my_getch('1', '3') - '0';
    */
    cct_cls();
    Str[0] = "初级(9 * 9 - 10颗雷)";
    Str[1] = "中级(16 * 16 - 40颗雷)";
    Str[2] = "高级(16 * 30 - 99颗雷)";
    ret.level = Menu_char(3, "123", Str) - '0';
    cout << ret.level;


    init_gmdata(ret);

    return ret;
}

void init_gmdata(GMDATA &ret)
{
    switch (ret.level)
    {
        case 1:
            ret.Xlen = ret.Ylen = 9;
            ret.bombnum = 10;
            break;
        case 2:
            ret.Xlen = ret.Ylen = 16;
            ret.bombnum = 40;
            break;
        case 3:
            ret.Xlen = 16;
            ret.Ylen = 30;
            ret.bombnum = 99;
            break;
    }
}