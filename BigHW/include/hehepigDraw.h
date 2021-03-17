/* 朱泽凯 2051995 济美 */
#pragma once

#define MAX_ROW 20
#define MAX_COL 20

/// <summary>
/// 矩阵地图的一个单元
/// </summary>
class hehepig_block {

public:
    int Val;        // 块内物件信息
    int BgColor;    // 背景颜色
    int FtColor;    // 文字颜色

    hehepig_block(int Val=0, int BgColor=0, int FtColor=7) : Val(Val), BgColor(BgColor), FtColor(FtColor) {}
};


/// <summary>
/// 矩阵地图界面
/// </summary>
class hehepig_block_map {
    int Row;        // 行数
    int Col;        // 列数
    int FrameMode;   // 边框模式

public:

    hehepig_block A[MAX_ROW][MAX_COL];      // 地图矩阵信息

    hehepig_block_map() :Row(0), Col(0), FrameMode(0) {}
    hehepig_block_map(int Row, int Col, int FrameMode=0);

    /// 输出提示信息
    void DigitalLogError(const char* Str);

    /// <summary>
    /// 打印地图
    /// </summary>
    /// <param name="Mode">
    ///     模式参数
    ///     {0: " * " 默认，输出数字Val，是啥输啥}
    ///     {1: " * " Val值为0则输出'*'，否则输出'0'}
    /// </param>
    void DigitalPrint(int Mode=0);

};

