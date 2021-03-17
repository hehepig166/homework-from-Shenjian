/* ���� 2051995 ���� */
#pragma once

#define MAX_ROW 20
#define MAX_COL 20

/// <summary>
/// �����ͼ��һ����Ԫ
/// </summary>
class hehepig_block {

public:
    int Val;        // ���������Ϣ
    int BgColor;    // ������ɫ
    int FtColor;    // ������ɫ

    hehepig_block(int Val=0, int BgColor=0, int FtColor=7) : Val(Val), BgColor(BgColor), FtColor(FtColor) {}
};


/// <summary>
/// �����ͼ����
/// </summary>
class hehepig_block_map {
    int Row;        // ����
    int Col;        // ����
    int FrameMode;   // �߿�ģʽ

public:

    hehepig_block A[MAX_ROW][MAX_COL];      // ��ͼ������Ϣ

    hehepig_block_map() :Row(0), Col(0), FrameMode(0) {}
    hehepig_block_map(int Row, int Col, int FrameMode=0);

    /// �����ʾ��Ϣ
    void DigitalLogError(const char* Str);

    /// <summary>
    /// ��ӡ��ͼ
    /// </summary>
    /// <param name="Mode">
    ///     ģʽ����
    ///     {0: " * " Ĭ�ϣ��������Val����ɶ��ɶ}
    ///     {1: " * " ValֵΪ0�����'*'���������'0'}
    /// </param>
    void DigitalPrint(int Mode=0);

};

