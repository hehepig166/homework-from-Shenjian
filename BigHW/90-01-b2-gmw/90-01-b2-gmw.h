/* ���� 2051995 ���� */
#pragma once

#include "../include/cmd_gmw_tools.h"

#define BITS_VAL 15
#define BIT_IS_BOMB 16
#define BIT_SHOWED 32
#define BIT_MARKED 64
#define FAIL_MARKED 128

class MINE_GAME {

    CONSOLE_GRAPHICS_INFO CGI;

    //��ɨ�׵�ͼ��Ϣ
    int **graph;

    //��������
    int row;
    int col;

    //��ʣ����
    int cntBomb;
    int cntMark;

    //ʱ����Ϣ
    clock_t timeStart;
    clock_t timeEnd;

public:

    //��ʼ������ʾ���
    MINE_GAME(int _row, int _col, int _cntBomb);

    //�������ͷ�graph�Ŀռ�
    ~MINE_GAME();

    //������������㴴����ͼ(����)����֤��㲻����
    int setBombs(int rowIndex, int colIndex);

    //�㿪�õ㣬���Ӹõ㿪ʼ������������ͼ�ν���Ҳ���£�
    //�������򲻸���ͼ��
    //����ֵ�Ǹõ��Ƿ����ף�1��ʾ�ǣ�0��ʾ���ǣ�
    int clickZone(int rowIndex, int colIndex);

    //�л��õ���״̬��ԭ���Ǳ�����л�Ϊδ��ǣ�
    //�Ѿ�SHOW�˵ľͲ����б�ǲ�����
    int markSwitch(int rowIndex, int colIndex);

    //��飬ֻ�е��õ��Ե㿪����Χ���������Χ�������ʱ�ż�飨�����ʱҲ����0��
    //����Χ�б����򷵻�1
    //���������������Χ����������û��ǵĵĵ㿪������ɢ����ɢ��������ɢ��ļ��
    //��ɢ���˶�û���򷵻�0
    //����ֵ���Ƿ���Ϸ����
    int checkMark(int rowIndex, int colIndex);

    //��ʾ���е���
    void showBombs();

    //����״̬����ʾ����
    int showCnt();

    //����״̬����ʾ��ʱ
    int showTime();

    //-1:�������
    //-2:�Ҽ�����
    //-3:���ֵ���
    //����:ASCII
    //0:δ֪
    int GetOpr(int &_oprRow, int &_oprCol);

    void lower_hint(const char *info)
    {
        gmw_status_line(&CGI, LOWER_STATUS_LINE, info);
    }

    int win();
};