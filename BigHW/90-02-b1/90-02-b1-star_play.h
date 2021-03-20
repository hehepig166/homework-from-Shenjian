/* 朱泽凯 2051995 济美 */
#pragma once

#include "../include/hehepigDraw.h"

//play.cpp

class Play;

/// <summary>
/// 借助 Draw库 与 Play类 进行数组模式的消灭星星游戏
/// </summary>
class DigitalPlay {
private:
    Play* P=0;
    hehepig_block_map HBM;

    int Row=0;
    int Col=0;

    void GetXY(int& x, int& y);
    void PrintVis();

public:
    void Init();
    void Begin(int Mode);
};

/// <summary>
/// 借助 Draw库 与 Play类 进行色块模式的消灭星星游戏
/// </summary>
class GraphicalPlay {
private:
    Play* P;
    hehepig_block_map HBM;

    int Row;
    int Col;


public:
    void Init();
    void Begin(int Mode);
    void GetXY(int& X, int& Y);
};