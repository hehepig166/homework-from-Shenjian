/* ���� 2051995 ���� */
#pragma once
#include "../include/cmd_gmw_tools.h"

#define MAXROW 20
#define MAXCOL 20

#define BIT_FOCUS 64
#define BIT_SELECTED 128
#define BITS_VAL 31

class STAR_GAME {

    //��ͼ��Ϣ
    int graph[MAXROW][MAXCOL];
    int vis[MAXROW][MAXCOL];

    //����
    int row;
    int col;

    int ScoreOld;
    int score;

    CONSOLE_GRAPHICS_INFO CGI;

public:

    //��ʼ���������ǲ���ʾͼ��
    int reset(int _row, int _col);

    //������������ͨ���С
    int dfs(int rowIndex, int colIndex);

    //ѡ�񣬲��Ըõ������������������õ��Լ���Χ��ͬ�ĵ�vis��1����������0
    //���õ���0�����Ѿ�û�������ˣ�����ô��ִ�У�����0
    //����ֵ�������õ㣬��Χ��õ���ͬ�ĸ���
    int select(int rowIndex, int colIndex);

    //����BIT_SELECTEDȷ�ϲ�������ǰѡ���˵�����&BIT_SELECTED��
    //����ֵ�������ĸ���
    int confirm();

    //����
    int down();

    //����
    int left();

    //���޷������ˣ�����1�����򷵻�0
    bool checkFail();

    //-1:�������
    //key_left / key_up / key_right / key_down �� hehepigIO
    //1~127:ASCII
    //0:δ֪
    int getOpr(int &_oprRow, int &_oprCol);

    int isSelected(int rowIndex, int colIndex);

    //valȱʡ����(0����1)�����л�
    int focusSwitch(int rowIndex, int colIndex, int val = -1);
};