/* ���� 2051995 ���� */
#pragma once

#define MAX_ROW 20
#define MAX_COL 20

/// <summary>
/// �����ͼ��һ����Ԫ����Ϣ
/// </summary>
class hehepig_block {

public:
    int Val;        // ���������Ϣ
    int BgColor;    // ������ɫ
    int FtColor;    // ������ɫ

    hehepig_block(int Val=0, int BgColor=0, int FtColor=7) : Val(Val), BgColor(BgColor), FtColor(FtColor) {}

    bool operator!=(const hehepig_block B) {
        return (this->Val != B.Val ||
            this->BgColor != B.BgColor ||
            this->FtColor != B.FtColor
            );

    }
};

/// <summary>
/// ɫ�����-�ͼ������ؼ���
/// </summary>
class hehepig_block_graph {
    int FrameMode=1;
    int BlockSize=3;
    int Row=1;
    int Col=1;

    int LUCornerX=0;
    int LUCornerY=0;

    hehepig_block Blocks[MAX_ROW][MAX_COL];    // ɫ����Ϣ


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
/// �����ͼ����-�߼������̸񼶣�
/// </summary>
class hehepig_block_map {
    int Row;        // ����
    int Col;        // ����

    int FrameMode;   // �߿�ģʽ
    int BlockSize;      //ɫ��߳�

    int LUX;
    int LUY;
    int RDX;
    int RDY;

    

    hehepig_block_graph* G;

public:

    hehepig_block A[MAX_ROW][MAX_COL];      // ��ͼ������Ϣ

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


    // ����ģʽ

    void DigitalLogError(const char* Str);      // �����ʾ��Ϣ
    void DigitalPrint();                        // �����ͼ



    // ɫ��ģʽ

    void GraphicalInit(int _Row, int _Col, int _FrameMode, int _BlockSize);
    void GraphicalUpgrade();
    void GetXY(int& X, int& Y);
    void GraphicalLogError(const char* Str, int addr=0);
    

};

