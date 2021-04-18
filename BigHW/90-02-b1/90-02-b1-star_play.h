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
/// ���� Draw�� �� Play�� ����ɫ��ģʽ������������Ϸ
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