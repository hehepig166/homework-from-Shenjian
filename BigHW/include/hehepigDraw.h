/* 朱泽凯 2051995 济美 */
#pragma once

#define MAX_ROW 20
#define MAX_COL 20

/// <summary>
/// 矩阵地图的一个单元的信息
/// </summary>
class hehepig_block {

public:
    int Val;        // 块内物件信息
    int BgColor;    // 背景颜色
    int FtColor;    // 文字颜色

    hehepig_block(int Val=0, int BgColor=0, int FtColor=7) : Val(Val), BgColor(BgColor), FtColor(FtColor) {}

    bool operator!=(const hehepig_block B) {
        return (this->Val != B.Val ||
            this->BgColor != B.BgColor ||
            this->FtColor != B.FtColor
            );

    }
};

/// <summary>
/// 色块界面-低级（像素级）
/// </summary>
class hehepig_block_graph {
    int FrameMode=1;
    int BlockSize=3;
    int Row=1;
    int Col=1;

    int LUCornerX=0;
    int LUCornerY=0;

    hehepig_block Blocks[MAX_ROW][MAX_COL];    // 色块信息


public:

    void GotoPrint(int X, int Y, const char* Str = 0, int BGCol = 0, int FtCol = 7, int MaxLen = -1, int Rep = 1);
    void GotoPrintChar(int X, int Y, const char Char, int BGCol = 0, int FtCol = 7);
    void GotoPrintInt(int X, int Y, const int Int, int BGCol = 0, int FtCol = 7);
    void PrintFrame();
    void PrintBlock(int X, int Y);

    void Init(int _Row, int _Col, int _FrameMode, int _BlockSize, int _LUX, int _LUY);
    void ModifyBlock(int X, int Y, hehepig_block B);

    hehepig_block GetBlock(int X, int Y) { return Blocks[X][Y]; }

    int ReadKeyMouse(int& X, int& Y, int& MAction, int &k1, int &k2);

    void ChangeFrameMode(int _FM);
    void ChangeBlockSize(int _BS);
    //void ChangeMode(const char* Option, int Val);
    //void LogError(const char* Str);

};


/// <summary>
/// 矩阵地图界面-高级（棋盘格级）
/// </summary>
class hehepig_block_map {
    int Row;        // 行数
    int Col;        // 列数

    int FrameMode;   // 边框模式
    int BlockSize;      //色块边长

    int LUX;
    int LUY;
    int RDX;
    int RDY;

    

    hehepig_block_graph* G;

public:

    hehepig_block A[MAX_ROW][MAX_COL];      // 地图矩阵信息

    hehepig_block_map() :Row(0), Col(0), FrameMode(1), BlockSize(3), LUX(0), LUY(0), G(0), RDX(0), RDY(0) {}
    hehepig_block_map(int Row, int Col, int FrameMode=1);
    
    void CalcRD();

    void ChangeFrameMode(int FM) {
        G->ChangeFrameMode(FM);
        FrameMode = FM;
        CalcRD();
    }
    void ChangeBlockSize(int BS) {
        G->ChangeBlockSize(BS);
        BlockSize = BS;
        CalcRD();
    }


    // 数组模式

    void DigitalLogError(const char* Str);      // 输出提示信息
    void DigitalPrint();                        // 输出地图



    // 色块模式

    void GraphicalInit(int _Row, int _Col, int _FrameMode, int _BlockSize);
    void GraphicalUpgrade();
    void GetXY(int& X, int& Y);
    void GraphicalLogError(const char* Str, int addr=0);
    

};

