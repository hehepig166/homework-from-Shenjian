/* 朱泽凯 2051995 济美 */
#include "../include/cmd_console_tools.h"
#include "../include/hehepigMenu.h"
#include "./90-02-b1-star_play.h"
#include <iostream>
using namespace std;

int menu();

int main() {
    while (menu()) {
        cct_cls();
    }
}

int menu() {
    //int cols, lines, buffer_cols, buffer_lines;
    //cct_getconsoleborder(cols, lines, buffer_cols, buffer_lines);   //获取窗口属性


    const char* hintStr[] = {
        "命令行找出可消除项并标识",
        "命令行完成一次消除（分步骤显示）",
        "命令行完成一关（分步骤显示）",
        "伪图形界面下用鼠标选择一个色块（无分隔线）",
        "伪图形界面下用鼠标选择一个色块并完成一局（有分隔线）",
        "伪图形界面完成一次消除（分步骤）",
        "伪图形界面完整版",
        "退出"
    };

    int m = Menu_char(8, "ABCDEFGQ", hintStr);

    m = toupper(m);

    if (m >= 'A' && m <= 'C') {
        cct_cls();
        DigitalPlay().Begin(m);
    }
    else if (m >= 'D' && m <= 'G') {
        cct_cls();
        GraphicalPlay().Begin(m);
    }
    else if (m == 'Q') {
        //
        //cct_setconsoleborder(cols, lines, buffer_cols, buffer_lines);
        return 0;
    }

    //cct_setconsoleborder(cols, lines, buffer_cols, buffer_lines);   //窗口属性复位
    
    return m;
}