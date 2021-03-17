/* 朱泽凯 2051995 济美 */
#include "../include/hehepigMenu.h"
#include <iostream>
using namespace std;

int main() {
    int ret;

    const char* hintStr[] = {
        "命令行找出可消除项并标识",
        "命令行完成一次消除（分步骤显示）",
        "命令行完成一关（分步骤显示）",
        "伪图形界面下用鼠标选择一个色块（无分隔线）",
        "伪图形界面下用鼠标选择一个色块（有分隔线）",
        "伪图形界面完成一次消除（分步骤）",
        "伪图形界面完整版",
        "退出"
    };

    ret = Menu_char(8, "ABCDEFGQ", hintStr);

    cout << (char)ret << endl;
}