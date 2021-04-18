/* 朱泽凯 2051995 济美 */
#include "../include/hehepigDraw.h"
#include "../include/cmd_console_tools.h"
#include "../include/cmd_console_tools.h"
#include <iostream>
#include <iomanip>
using namespace std;


/// ========================================================================
/// hehepig_block_graph
/// ╔╦╗
/// ╠╬╣
/// ╚╩╝
/// ╔═╗
/// ║╬║
/// ╚═╝
/// ========================================================================

#define HBG hehepig_block_graph
#define ErrX (Row*BlockSize+4)

/// <summary>
/// 去窗口 (X,Y) 处(从ULCorner范围起第 X 行第 Y 列)输出字符串 Str (0则不输出)，宽度 Maxlen(-1则原长，否则超出截断，不足补空格)，重复 Rep 次
/// </summary>
void HBG::GotoPrint(int X, int Y, const char* Str, int BGCol, int FtCol, int MaxLen, int Rep) {
    cct_gotoxy(LUCornerY + Y * 2 + 2, LUCornerX + X + 1);   // +2 +1 是因为标尺

    if (Str != 0) {
        while (Rep--) {
            cct_setcolor(BGCol, FtCol);

            for (int i = 0; (MaxLen == -1 && Str[i]) || (i < MaxLen); i++)
                putchar(Str[i] ? Str[i] : ' ');

            cct_setcolor();
        }
    }
}
void HBG::GotoPrintChar(int X, int Y, const char Char, int BGCol, int FtCol) {
    cct_gotoxy(LUCornerY + Y * 2 + 2, LUCornerX + X + 1);
    putchar(Char);
}
void HBG::GotoPrintInt(int X, int Y, const int Int, int BGCol, int FtCol) {
    cct_gotoxy(LUCornerY + Y * 2 + 2, LUCornerX + X + 1);
    cout << Int;
}


#define COL_BF(HB) HB.BgColor, HB.FtColor

/// <summary>
/// 更新显式 this->Blocks[X][Y] 到它该到的位置
/// </summary>
void HBG::PrintBlock(int X, int Y) {

    if (FrameMode == 1) {
        int tmpX = X * BlockSize + 1, tmpY = Y * BlockSize + 1;     //+1 是因为标尺
        if (BlockSize == 1) {
            GotoPrint(tmpX, tmpY, "  ", COL_BF(Blocks[X][Y]));    //===============
            //cct_setcolor(Blocks[X][Y].BgColor, Blocks[X][Y].FtColor);
            //cout <<setw(2) << Blocks[Y][Y].Val;
            //cct_setcolor();
            return;
        }
        //框
        for (int i = BlockSize - 2; i > 0; i--) {
            GotoPrint(tmpX, tmpY + i, "═", COL_BF(Blocks[X][Y]));
            GotoPrint(tmpX + BlockSize - 1, tmpY + i, "═", COL_BF(Blocks[X][Y]));
            GotoPrint(tmpX + i, tmpY, "║", COL_BF(Blocks[X][Y]));
            GotoPrint(tmpX + i, tmpY + BlockSize - 1, "║", COL_BF(Blocks[X][Y]));
        }
        GotoPrint(tmpX, tmpY, "╔", COL_BF(Blocks[X][Y]));
        GotoPrint(tmpX, tmpY + BlockSize - 1, "╗", COL_BF(Blocks[X][Y]));
        GotoPrint(tmpX + BlockSize - 1, tmpY, "╚", COL_BF(Blocks[X][Y]));
        GotoPrint(tmpX + BlockSize - 1, tmpY + BlockSize - 1, "╝", COL_BF(Blocks[X][Y]));

        //内
        for (int i = BlockSize - 2; i > 0; i--)
            for (int j = BlockSize - 2; j > 0; j--)
                GotoPrint(tmpX + i, tmpY + j, "蛤", COL_BF(Blocks[X][Y]));
    }
    else if (FrameMode == 2) {
        int tmpX = X * (1+BlockSize) + 1, tmpY = Y * (1+BlockSize) + 1;     //后面的 +1 是因为标尺
        //框
        for (int i = BlockSize - 2; i > 0; i--) {
            GotoPrint(tmpX, tmpY + i, "═", COL_BF(Blocks[X][Y]));
            GotoPrint(tmpX + BlockSize - 1, tmpY + i, "═", COL_BF(Blocks[X][Y]));
            GotoPrint(tmpX + i, tmpY, "║", COL_BF(Blocks[X][Y]));
            GotoPrint(tmpX + i, tmpY + BlockSize - 1, "║", COL_BF(Blocks[X][Y]));
        }
        GotoPrint(tmpX, tmpY, "╔", COL_BF(Blocks[X][Y]));
        GotoPrint(tmpX, tmpY + BlockSize - 1, "╗", COL_BF(Blocks[X][Y]));
        GotoPrint(tmpX + BlockSize - 1, tmpY, "╚", COL_BF(Blocks[X][Y]));
        GotoPrint(tmpX + BlockSize - 1, tmpY + BlockSize - 1, "╝", COL_BF(Blocks[X][Y]));

        //内
        for (int i = BlockSize - 2; i > 0; i--)
            for (int j = BlockSize - 2; j > 0; j--)
                GotoPrint(tmpX + i, tmpY + j, "蛤", COL_BF(Blocks[X][Y]));
    }
    else {
        puts("Unknown FrameMode in hehepig_block_graph.PrintBlock");
    }


}

/*
/// <summary>
/// 根据 this->FrameMode 显式出边框
/// ╔═╗
/// ║  ║
/// ╚═╝
/// ╔╦╗
/// ╠╬╣
/// ╚╩╝
/// ╔═╗
/// ║╬║
/// ╚═╝
/// </summary>
*/
void HBG::PrintFrame() {

    /// ╔═╗
    /// ║  ║
    /// ╚═╝
    if (FrameMode == 1) {
        //四角
        GotoPrint(0, 0, "╔", 7, 0);
        GotoPrint(0, Col * BlockSize + 1, "╗", 7, 0);
        GotoPrint(Row * BlockSize + 1, 0, "╚", 7, 0);
        GotoPrint(Row * BlockSize + 1, Col * BlockSize + 1, "╝", 7, 0);

        //四边
        for (int i = Col * BlockSize, xx = Row * BlockSize + 1; i > 0; i--) {
            GotoPrint(0, i, "═", 7, 0);
            GotoPrint(xx, i, "═", 7, 0);
        }
        for (int i = Row * BlockSize, yy = Col * BlockSize + 1; i > 0; i--) {
            GotoPrint(i, 0, "║", 7, 0);
            GotoPrint(i, yy, "║", 7, 0);
        }

        //标尺
        cct_setcolor(0, COLOR_YELLOW);
        for (int i = 0; i < Row; i++)
            GotoPrintChar(i * BlockSize + BlockSize / 2 + 1, -1, 'A' + i);
        for (int i = 0; i < Col; i++)
            GotoPrintInt(-1, i * BlockSize + BlockSize / 2 + 1, i);
        cct_setcolor();
    }

    /// ╔╦╗
    /// ╠╬╣
    /// ╚╩╝
    else if (FrameMode == 2) {
        int bs = (BlockSize + 1);
        //四角
        GotoPrint(0, 0, "╔", 7, 0);
        GotoPrint(0, Col * bs, "╗", 7, 0);
        GotoPrint(Row * bs, 0, "╚", 7, 0);
        GotoPrint(Row * bs, Col * bs, "╝", 7, 0);

        //四边
        for (int i = Col * bs-1, xx = Row * bs; i > 0; i--) {
            (i % bs == 0) ? GotoPrint(0, i, "╦", 7, 0) : GotoPrint(0, i, "═", 7, 0);
            (i % bs == 0) ? GotoPrint(xx, i, "╩", 7, 0) : GotoPrint(xx, i, "═", 7, 0);
        }
        for (int i = Row * bs-1, yy = Col * bs; i > 0; i--) {
            
            (i % bs == 0) ? GotoPrint(i, 0, "╠", 7, 0) : GotoPrint(i, 0, "║", 7, 0);
            (i % bs == 0) ? GotoPrint(i, yy, "╣", 7, 0) : GotoPrint(i, yy, "║", 7, 0);
        }

        //内边
        for (int i=Row; i>0; i--)
            for (int j = Col; j > 0; j--) {

                for (int k = 1; k <= BlockSize; k++) {
                    if (i < Row)    //块下边界
                        GotoPrint(i * bs, (j - 1) * bs + k, "═", 7, 0);
                    if (j <Col)    //块右边界
                        GotoPrint((i - 1) * bs + k, j * bs, "║", 7, 0);
                }
                if (i<Row && j<Col)     //块左上角
                    GotoPrint(i * bs, j * bs, "╬", 7, 0);

            }

        //标尺
        cct_setcolor(0, COLOR_YELLOW);
        for (int i = 0; i < Row; i++)
            GotoPrintChar(i * BlockSize + BlockSize / 2 + 1, -1, 'A' + i);
        for (int i = 0; i < Col; i++)
            GotoPrintInt(-1, i * BlockSize + BlockSize / 2 + 1, i);
        cct_setcolor();
    }
    else {
        puts("Unknown FrameMode in hehepig_block_graph.PrintFrame");
        int aaaa = getchar();
        return;
    }
}

/// <summary>
/// 初始化（会显示出来）
/// </summary>
/// <param name="_Row, _Col">行数，列数</param>
/// <param name="_FrameMode">边框模式</param>
/// <param name="_BlockSize">色块边长（正方形）</param>
/// <param name="_LUX, _LUY">总色块窗口左上角所在位置</param>
void HBG::Init(int _Row, int _Col, int _FrameMode, int _BlockSize, int _LUX, int _LUY){

    cct_cls();

    Row = _Row;
    Col = _Col;
    FrameMode = _FrameMode;
    BlockSize = max(1,_BlockSize);

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
/// 修改 (X,Y) 处的色块，显式也会修改，改成 B
/// </summary>
void HBG::ModifyBlock(int X, int Y, hehepig_block B) {
    if (Blocks[X][Y] != B) {
        Blocks[X][Y] = B;
        PrintBlock(X, Y);
    }
    GotoPrint(ErrX, 0);
}

/// <summary>
/// 获取当前键盘鼠标信息（所在色块坐标、是否点击等）
/// </summary>
/// <param name="X,Y">鼠标色块编号将存在这，若鼠标不在色块上则都变成-1</param>
/// <param name="Status">MAction</param>
/// <returns>MouseOrKey</returns>
int HBG::ReadKeyMouse(int& X, int& Y, int& MAction, int &k1, int &k2) {

    cct_enable_mouse();

    int MX, MY, tt;
    int tmpX, tmpY;
    tt=cct_read_keyboard_and_mouse(MY, MX, MAction, k1, k2);

    //注意这里 LU 是用来画坐标尺了

    if (tt == CCT_KEYBOARD_EVENT) {
        //暂空
    }
    else if (tt == CCT_MOUSE_EVENT && MAction!= MOUSE_WHEEL_MOVED_DOWN && MAction!= MOUSE_WHEEL_MOVED_UP) {     //cct有个bug，滚轮滚动会导致XY错误
        if (MX < LUCornerX + 2 || MY < LUCornerY + 4) {
            X = -1;
            Y = -1;
        }
        else {
            //第一步，获取在 HBG 中的像素位置映射，-1是因为标尺
            tmpX = (MX - LUCornerX) - 1;
            tmpY = (MY - LUCornerY) / 2 - 1;

            if (FrameMode == 1) {
                //第二步
                tmpX = (tmpX - 1) / BlockSize;
                tmpY = (tmpY - 1) / BlockSize;
            }
            else if (FrameMode == 2) {
                if (tmpX % (BlockSize + 1) == 0 || tmpY % (BlockSize + 1) == 0) {   //边框上
                    tmpX = -1;
                    tmpY = -1;
                }
                else {
                    tmpX = tmpX / (BlockSize + 1);
                    tmpY = tmpY / (BlockSize + 1);
                }
            }
            else {
                puts("Unknown FrameMode in hehepig_block_graph.ReadKeyMouse\nPress Enter");
                int sasdfdsfsa = getchar();
                return 0;
            }

            X = tmpX;
            Y = tmpY;
        }
        if (X >= Row || Y >= Col) {
            X = -1;
            Y = -1;
        }
    }
    cct_disable_mouse();	//禁用鼠标
    return tt;

}


void HBG::ChangeFrameMode(int _FM) {
    cct_cls();
    FrameMode = _FM;
//以下是重新显示内容

    PrintFrame();
    //Print blocks
    for (int i = 0; i < Row; i++)
        for (int j = 0; j < Col; j++)
            PrintBlock(i, j);

    GotoPrint(ErrX, 0);
}

void HBG::ChangeBlockSize(int _BS) {
    cct_cls();
    BlockSize = _BS;
    //以下是重新显示内容

    PrintFrame();
    //Print blocks
    for (int i = 0; i < Row; i++)
        for (int j = 0; j < Col; j++)
            PrintBlock(i, j);

    GotoPrint(ErrX, 0);
}

#undef COL_BF
#undef HBG





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
    

    CalcRD();
    
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            A[i][j] = hehepig_block(0, COLOR_BLACK, COLOR_WHITE);

}

void HBM::CalcRD() {
    if (FrameMode == 1) {   //注意++是因为标尺
        RDX = LUX + Row * BlockSize + 1;
        RDY = LUY + Col * BlockSize + 1;

        RDX++;
        RDY++;
    }
    else if (FrameMode == 2) {  //前面的+1是因为网格，后面的++是因为标尺
        RDX = LUX + Row * (BlockSize + 1) + 1;
        RDY = LUY + Col * (BlockSize + 1) + 1;

        RDX++;
        RDY++;
    }
    else {
        RDX = RDY = 0;
        puts("Unknown FrameMode in hehepig_block_map.CalcRD");
        return;
    }
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
void _DigitPrint(hehepig_block* X) {
    cct_setcolor(X->BgColor, X->FtColor);
    cout <<setw(2) << (X->Val) <<" ";
    
    // 恢复默认属性
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
/// 色块显示模式相关函数
/// 
/// =====================================================

//新建一个画布并画出初始边框
void HBM::GraphicalInit(int _Row, int _Col, int _FrameMode, int _BlockSize) {
    static hehepig_block_graph _G;
    G = &_G;
    G->Init(_Row, _Col, _FrameMode, _BlockSize, 2, 0);
    Row = _Row;
    Col = _Col;

    CalcRD();

    FrameMode = _FrameMode;
    BlockSize = _BlockSize;
}

//更新显示
void HBM::GraphicalUpgrade() {
    for (int i=0; i<Row; i++)
        for (int j=0; j<Col; j++)
            if (A[i][j]!=G->GetBlock(i,j))
                G->ModifyBlock(i,j,A[i][j]);
}



//获取位置，存到X,Y中（由鼠标、键盘无冲控制）
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

            //注意此处是可循环的
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
        printf("%d (%c %d) %d [%d %d]                                     ", (tX>=0 && tY>=0), char(X + 'A'), Y, ss, k1, k2);

        if ((ss == MOUSE_LEFT_BUTTON_CLICK && X==tX && Y==tY) || (k1 == '\r'))
            break;
    }
}

void HBM::GraphicalLogError(const char* Str, int addr) {
    G->GotoPrint(RDX+1, 0, Str);
    puts("");
}

#undef HBM