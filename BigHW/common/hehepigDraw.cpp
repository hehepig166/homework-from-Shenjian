/* ���� 2051995 ���� */
#include "../include/hehepigDraw.h"
#include "../include/cmd_console_tools.h"
#include "../include/cmd_console_tools.h"
#include <iostream>
#include <iomanip>
using namespace std;


/// ========================================================================
/// hehepig_graph
/// �X�j�[
/// �d�p�g
/// �^�m�a
/// �X�T�[
/// �U�p�U
/// �^�T�a
/// ========================================================================

#define HG hehepig_graph
#define ErrX (Row*BlockSize+4)

/// <summary>
/// ȥ���� (X,Y) ��(��ULCorner��Χ��� X �е� Y ��)����ַ��� Str (0�����)����� Maxlen(-1��ԭ�������򳬳��ضϣ����㲹�ո�)���ظ� Rep ��
/// </summary>
void HG::GotoPrint(int X, int Y, const char* Str, int BGCol, int FtCol, int MaxLen, int Rep) {
    cct_gotoxy(LUCornerY + Y * 2 + 2, LUCornerX + X + 1);

    if (Str != 0) {
        while (Rep--) {
            cct_setcolor(BGCol, FtCol);

            for (int i = 0; (MaxLen == -1 && Str[i]) || (i < MaxLen); i++)
                putchar(Str[i] ? Str[i] : ' ');

            cct_setcolor();
        }
    }
}
void HG::GotoPrintChar(int X, int Y, const char Char, int BGCol, int FtCol) {
    cct_gotoxy(LUCornerY + Y * 2 + 2, LUCornerX + X + 1);
    putchar(Char);
}
void HG::GotoPrintInt(int X, int Y, const int Int, int BGCol, int FtCol) {
    cct_gotoxy(LUCornerY + Y * 2 + 2, LUCornerX + X + 1);
    cout << Int;
}


#define COL_BF(HB) HB.BgColor, HB.FtColor

/// <summary>
/// ������ʽ this->Blocks[X][Y] �����õ���λ��
/// </summary>
void HG::PrintBlock(int X, int Y) {
    int tmpX = X * BlockSize + 1, tmpY = Y * BlockSize + 1;

    //��
    for (int i = BlockSize-2; i >0; i--) {
        GotoPrint(tmpX, tmpY + i, "�T", COL_BF(Blocks[X][Y]));
        GotoPrint(tmpX + BlockSize - 1, tmpY + i, "�T", COL_BF(Blocks[X][Y]));
        GotoPrint(tmpX + i, tmpY, "�U", COL_BF(Blocks[X][Y]));
        GotoPrint(tmpX + i, tmpY + BlockSize - 1, "�U", COL_BF(Blocks[X][Y]));
    }
    GotoPrint(tmpX, tmpY, "�X", COL_BF(Blocks[X][Y]));
    GotoPrint(tmpX, tmpY + BlockSize - 1, "�[", COL_BF(Blocks[X][Y]));
    GotoPrint(tmpX + BlockSize - 1, tmpY, "�^", COL_BF(Blocks[X][Y]));
    GotoPrint(tmpX + BlockSize - 1, tmpY + BlockSize - 1, "�a", COL_BF(Blocks[X][Y]));

    //��
    for (int i = BlockSize - 2; i > 0; i--)
        for (int j = BlockSize - 2; j > 0; j--)
            GotoPrint(tmpX + i, tmpY + j, "��", COL_BF(Blocks[X][Y]));


}

/// <summary>
/// ���� this->FrameMode ��ʽ���߿�
/// </summary>
void HG::PrintFrame() {
    if (FrameMode == 1) {


        //�Ľ�
        GotoPrint(0, 0, "�X", 7, 0);
        GotoPrint(0, Col * BlockSize + 1, "�[", 7, 0);
        GotoPrint(Row * BlockSize + 1, 0, "�^", 7, 0);
        GotoPrint(Row * BlockSize + 1, Col * BlockSize + 1, "�a", 7, 0);

        //�ı�
        for (int i = Col * BlockSize, xx = Row * BlockSize + 1; i > 0; i--) {
            GotoPrint(0, i, "�T", 7, 0);
            GotoPrint(xx, i, "�T", 7, 0);
        }
        for (int i = Row * BlockSize, yy = Col * BlockSize + 1; i > 0; i--) {
            GotoPrint(i, 0, "�U", 7, 0);
            GotoPrint(i, yy, "�U", 7, 0);
        }

        //���
        cct_setcolor(0, COLOR_YELLOW);
        for (int i = 0; i < Row; i++)
            GotoPrintChar(i * BlockSize + BlockSize / 2 + 1, -1, 'A' + i);
        for (int i = 0; i < Col; i++)
            GotoPrintInt(-1, i * BlockSize + BlockSize / 2 + 1, i);
        cct_setcolor();
    }
}

/// <summary>
/// ��ʼ��������ʾ������
/// </summary>
/// <param name="_Row, _Col">����������</param>
/// <param name="_FrameMode">�߿�ģʽ</param>
/// <param name="_BlockSize">ɫ��߳��������Σ�</param>
/// <param name="_LUX, _LUY">��ɫ�鴰�����Ͻ�����λ��</param>
void HG::Init(int _Row, int _Col, int _FrameMode, int _BlockSize, int _LUX, int _LUY){

    cct_cls();

    Row = _Row;
    Col = _Col;
    FrameMode = _FrameMode;
    BlockSize = max(3,_BlockSize);

    LUCornerX = _LUX;
    LUCornerY = _LUY;

    for (int i = 0; i < Row; i++)
        for (int j = 0; j < Col; j++)
            Blocks[i][j] = hehepig_block(0, 0, 7);

    PrintFrame();
    
    //Print blocks
    for (int i = 0; i < Row; i++)
        for (int j = 0; j < Col; j++)
            PrintBlock(i, j);
    
    
    GotoPrint(ErrX, 0);
    
}

/// <summary>
/// �޸� (X,Y) ����ɫ�飬��ʽҲ���޸ģ��ĳ� B
/// </summary>
void HG::ModifyBlock(int X, int Y, hehepig_block B) {
    if (Blocks[X][Y] != B) {
        Blocks[X][Y] = B;
        PrintBlock(X, Y);
    }
    GotoPrint(ErrX, 0);
}

/// <summary>
/// ��ȡ��ǰ���������Ϣ������ɫ�����ꡢ�Ƿ����ȣ�
/// </summary>
/// <param name="X,Y">���ɫ���Ž������⣬����겻��ɫ�����򶼱��-1</param>
/// <param name="Status">MAction</param>
/// <returns>MouseOrKey</returns>
int HG::ReadKeyMouse(int& X, int& Y, int& MAction, int &k1, int &k2) {

    cct_enable_mouse();

    int MX, MY, tt;
    int tmpX, tmpY;
    tt=cct_read_keyboard_and_mouse(MY, MX, MAction, k1, k2);

    //ע������ LU ���������������

    if (tt == CCT_KEYBOARD_EVENT) {
        //�ݿ�
    }
    else if (tt == CCT_MOUSE_EVENT && MAction!= MOUSE_WHEEL_MOVED_DOWN && MAction!= MOUSE_WHEEL_MOVED_UP) {     //cct�и�bug�����ֹ����ᵼ��XY����
        if (MX < LUCornerX + 2 || MY < LUCornerY + 4) {
            X = -1;
            Y = -1;
        }
        else {
            //��һ��
            tmpX = (MX - LUCornerX) - 1;
            tmpY = (MY - LUCornerY) / 2 - 1;

            //�ڶ���
            tmpX = (tmpX - 1) / BlockSize;
            tmpY = (tmpY - 1) / BlockSize;

            X = tmpX;
            Y = tmpY;
        }
        if (X >= Row || Y >= Col) {
            X = -1;
            Y = -1;
        }
    }
    cct_disable_mouse();	//�������
    return tt;

}

#undef COL_BF
#undef HG





/// ========================================================================
/// hehepig_block_map
/// 
/// ========================================================================

#define HBM hehepig_block_map

HBM::hehepig_block_map(int R, int C, int F) {
    Row = R;
    Col = C;
    FrameMode = F;
    BlockSize = 3;
    G = 0;
    LUX = LUY = 0;

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
void _DigitPrint(hehepig_block* X) {
    cct_setcolor(X->BgColor, X->FtColor);
    cout <<setw(2) << (X->Val) <<" ";
    
    // �ָ�Ĭ������
    cct_setcolor();
}

void HBM::DigitalPrint() {
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
            _DigitPrint(&A[i][j]);
        cout << endl;

    }

    cout << endl;
}


/// =====================================================
/// ɫ����ʾģʽ��غ���
/// 
/// =====================================================

//�½�һ��������������ʼ�߿�
void HBM::GraphicalInit(int _Row, int _Col, int _FrameMode, int _BlockSize) {
    static hehepig_graph _G;
    G = &_G;
    G->Init(_Row, _Col, _FrameMode, _BlockSize, 2, 0);
    Row = _Row;
    Col = _Col;
    FrameMode = _FrameMode;
    BlockSize = _BlockSize;
}

//������ʾ
void HBM::GraphicalUpgrade() {
    for (int i=0; i<Row; i++)
        for (int j=0; j<Col; j++)
            if (A[i][j]!=G->GetBlock(i,j))
                G->ModifyBlock(i,j,A[i][j]);
}



//��ȡλ�ã��浽X,Y�У�����ꡢ�����޳���ƣ�
void HBM::GetXY(int& X, int& Y) {

    int lx, ly;
    lx = X;
    ly = Y;
    while (1) {
        int ss, tt, k1, k2, tX, tY;
        tX = X;
        tY = Y;
        cct_gotoxy(0, 0);
        tt = G->ReadKeyMouse(tX, tY, ss, k1, k2);

        if (tt == CCT_MOUSE_EVENT) {
            if (tX < 0 || tY < 0) {
               // blank
            }
            else {
                X = tX;
                Y = tY;
            }
        }
        else if (tt == CCT_KEYBOARD_EVENT) {

            if (X < 0 || Y < 0) {
                X = Y = 0;
            }

            if (k1 == 224 && k2 == 72 || k1 == 'w')
                X--;
            else if (k1 == 224 && k2 == 75 || k1 == 'a')
                Y--;
            else if (k1 == 224 && k2 == 77 || k1 == 'd')
                Y++;
            else if (k1 == 224 && k2 == 80 || k1 == 's')
                X++;

            //ע��˴��ǿ�ѭ����
            X = (X%Row+Row)%Row;
            Y = (Y%Col+Col)%Col;
        }

        if (lx != X || ly != Y) {
            G->ModifyBlock(X, Y, hehepig_block((A[X][Y].Val), (A[X][Y].BgColor), COLOR_HWHITE));
            if (lx >= 0 && ly >= 0)
                G->ModifyBlock(lx, ly, A[lx][ly]);
            lx = X;
            ly = Y;
        }

        cct_gotoxy(0, 0);
        printf("%d (%c %d) %d [%d %d]                                     ", tt, char(X + 'A'), Y, ss, k1, k2);

        if ((ss == MOUSE_LEFT_BUTTON_CLICK && X==tX && Y==tY) || (k1 == '\r'))
            break;
    }
}

void HBM::GraphicalLogError(const char* Str, int addr) {
    G->GotoPrint(Row * BlockSize + 3 + addr, 0, Str);
    puts("");
}


#undef HBM