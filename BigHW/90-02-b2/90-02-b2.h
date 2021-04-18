/* ���� 2051995 ���� */
#pragma once
#include "../include//cmd_gmw_tools.h"

#define MAXROW 20
#define MAXCOL 20

#define BIT_FOCUS 64
#define BIT_SELECTED 128
#define BITS_VAL 31

class HCS_GAME {

    //��ͼ��Ϣ
    int graph[MAXROW][MAXCOL];
    int vis[MAXROW][MAXCOL];

    int row;
    int col;

    //Ŀ������
    int winNum;

    //ÿ���������ӵķ֣� num*cnt
    int score;

    CONSOLE_GRAPHICS_INFO CGI;

public:

    //��ʼ���������ֲ���ʾͼ��
    int reset(int _row, int _col, int _winNum);

    //������������ͨ���С����������������һ��
    int dfs(int rowIndex, int colIndex);

    //ѡ�񣬺�������������һ��
    int select(int rowIndex, int colIndex);

    //��������������������һ��
    int confirm();

    //�޸�,������֣����ı�BIT_FOCUS��״̬�������val�Ϸ���
    int setVal(int rowIndex, int colIndex, int val);
    int getVal(int rowIndex, int colIndex)
    {
        return graph[rowIndex][colIndex] & BITS_VAL;
    }

    //���Ʋ���������
    int down();

    //�����Ƿ�Ӯ�ˣ����Ƿ����Max��
    bool checkWin();

    //-1:�������
    //key_left / key_up / key_right / key_down �� hehepigIO
    //1~127:ASCII
    //0:δ֪
    //����������һ��
    int getOpr(int &_oprRow, int &_oprCol);

    int isSelected(int rowIndex, int colIndex);

    //valȱʡ����(0����1)�����л�
    int focusSwitch(int rowIndex, int colIndex, int cal = -1);

    //�ӷ�
    //���ؼ����ķ�
    int addScore(int addi);
};