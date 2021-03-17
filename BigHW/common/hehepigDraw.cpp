/* ���� 2051995 ���� */
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
/// ������ʾģʽ��غ���
/// 
/// =====================================================


void HBM::DigitalLogError(const char* Str) {
    cout << Str;
}

/// <summary>
/// �ڵ�ǰ��괦��ʼ���� Mode ��� X ��ָ������
/// </summary>
/// <param name="X">ָ��һ�� hehepig_block ����</param>
/// <param name="Mode">
///     ģʽ����
///     {0: " * " Ĭ�ϣ��������Val����ɶ��ɶ}
///     {1: " * " ValֵΪ0�����'0'���������'*'}
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

    // �ָ�Ĭ������
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