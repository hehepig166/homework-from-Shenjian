/* 朱泽凯 2051995 济美 */
#include "../include/hehepigDraw.h"
#include "../include/cmd_console_tools.h"
#include <iostream>
#include <iomanip>
using namespace std;





/// ========================================================================
/// hehepig_block_map
/// 
/// ========================================================================

#define HBM hehepig_block_map

HBM::hehepig_block_map(int R, int C, int F) {
    Row = R;
    Col = C;
    FrameMode = F;

    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            A[i][j] = hehepig_block(0, COLOR_BLACK, COLOR_WHITE);

}




/// =====================================================
/// 数组显示模式相关函数
/// 
/// =====================================================


void HBM::DigitalLogError(const char* Str) {
    cout << Str;
}

/// <summary>
/// 在当前光标处开始根绝 Mode 输出 X 所指的内容
/// </summary>
/// <param name="X">指向一个 hehepig_block 对象</param>
/// <param name="Mode">
///     模式参数
///     {0: " * " 默认，输出数字Val，是啥输啥}
///     {1: " * " Val值为0则输出'0'，否则输出'*'}
/// </param>
void _DigitPrint(hehepig_block* X, int Mode) {
    if (Mode == 0) {
        cct_setcolor(X->BgColor, X->FtColor);
        cout <<setw(2) << (X->Val) <<" ";
    }
    else if (Mode == 1) {
        cct_setcolor(X->BgColor, X->FtColor);
        cout << ((X->Val) ? " * " : " 0 ");
    }

    // 恢复默认属性
    cct_setcolor();
}



void HBM::DigitalPrint(int Mode) {
    cout << "  | ";
    for (int i = 0; i < Col; i++)
        cout << " " << i << " ";
    cout << endl;

    cout << "--+-";
    cout << setw(3 * Col) << setfill('-') << "" <<setfill(' ');
    cout << endl;

    for (int i = 0; i < Row; i++) {
        cout << (char)('A' + i) << " | ";
        for (int j = 0; j < Col; j++)
            _DigitPrint(&A[i][j], Mode);
        cout << endl;

    }

    cout << endl;
}