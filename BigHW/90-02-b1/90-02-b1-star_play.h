/* ���� 2051995 ���� */
#pragma once

#include "../include/hehepigDraw.h"

//play.cpp

class Play;

/// <summary>
/// ���� Draw�� �� Play�� ��������ģʽ������������Ϸ
/// </summary>
class DigitalPlay {
private:
    Play* P;
    hehepig_block_map HBM;

    int Row;
    int Col;

    void GetXY(int& x, int& y);
    void PrintVis();

public:
    DigitalPlay(){}
    void Init();
    void Begin(int Mode);
};

/// <summary>
/// ���� Draw�� �� Play�� ����ɫ��ģʽ������������Ϸ
/// </summary>
class GraphicalPlay {

    int Nowx;
    int Nowy;
};