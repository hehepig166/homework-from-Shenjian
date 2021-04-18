/* 朱泽凯 2051995 济美 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <ctime>
#include <Windows.h>
#include "90-01-b2-gmw.h"
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;
static const int fx[8][2] = { {-1, -1},{-1, 0}, {-1, 1},{0, -1},{0, 1},{1, -1}, {1, 0}, {1, 1} };

#define MINE_RAND   0

inline bool inZone(int x, int y, int mx, int my)
{
    return (x >= 0 && x < mx &&y >= 0 && y < my);
}

/* 定义1-5的数字用何种形式显示在界面上（当前选择项状态+选中后关联项状态） */
const BLOCK_DISPLAY_INFO bdi[] = {
    {0 | BIT_SHOWED, COLOR_HWHITE, COLOR_WHITE, "  "},
    {1 | BIT_SHOWED, COLOR_WHITE, COLOR_BLUE, " 1"},
    {2 | BIT_SHOWED, COLOR_WHITE, COLOR_GREEN, " 2"},
    {3 | BIT_SHOWED, COLOR_WHITE, COLOR_HRED, " 3"},
    {4 | BIT_SHOWED, COLOR_WHITE, COLOR_CYAN, " 4"},
    {5 | BIT_SHOWED, COLOR_WHITE, COLOR_PINK, " 5"},
    {6 | BIT_SHOWED, COLOR_WHITE, COLOR_YELLOW, " 6"},
    {7 | BIT_SHOWED, COLOR_WHITE, COLOR_HBLUE, " 7"},
    {8 | BIT_SHOWED, COLOR_WHITE, COLOR_HGREEN, " 8"},

    {FAIL_MARKED, COLOR_CYAN, COLOR_HRED, "FM"},
    {BIT_IS_BOMB | BIT_SHOWED | BIT_MARKED, COLOR_GREEN, COLOR_BLACK, "⊙"},
    {BIT_IS_BOMB | BIT_SHOWED, COLOR_RED, COLOR_BLACK, "⊙"},
    {BIT_MARKED, COLOR_CYAN, COLOR_RED, "M"},
    {BDI_VALUE_END, COLOR_WHITE, COLOR_YELLOW, NULL} //（未显现）判断结束条件为-999
};

MINE_GAME::MINE_GAME(const int _row, const int _col, int _cntBomb)
{
    row = _row;
    col = _col;
    cntBomb = _cntBomb;
    cntMark = 0;

    //不考虑申请失败
    graph = new int *[row];
    for (int i = 0; i < row; i++)
        graph[i] = new int[col];

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            graph[i][j] = 0;

    timeStart = timeEnd = -1;

    gmw_init(&CGI, row, col);
    gmw_set_frame_color(&CGI, COLOR_HWHITE, COLOR_WHITE);
    gmw_set_frame_style(&CGI, 6, 3, false);
    gmw_set_block_border_switch(&CGI, true);
    gmw_set_status_line_switch(&CGI, TOP_STATUS_LINE, true);
    gmw_set_status_line_switch(&CGI, LOWER_STATUS_LINE, true);
    gmw_draw_frame(&CGI);

    //画雷
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            gmw_draw_block(&CGI, i, j, graph[i][j], bdi);

    showCnt();
}

MINE_GAME::~MINE_GAME()
{
    for (int i = 0; i < row; i++)
        delete[]graph[i];
    delete[]graph;
}

int MINE_GAME::setBombs(int rowIndex, int colIndex)
{

#if MINE_RAND
    srand((unsigned int)time(0));
#endif // MINE_RAND

    //放雷    
    for (int t = 0, bi, bj; t < cntBomb; t++) {
        do {
            bi = rand() % row;
            bj = rand() % col;
        } while ((abs(bi - rowIndex) <= 1 && abs(bj - colIndex) <= 1) || (graph[bi][bj] & BIT_IS_BOMB));

        graph[bi][bj] = BIT_IS_BOMB;

        for (int k = 0, ki, kj; k < 8; k++)
            if (inZone(ki = bi + fx[k][0], kj = bj + fx[k][1], row, col) && !(graph[ki][kj] & BIT_IS_BOMB))
                graph[ki][kj]++;
    }

    //点开
    clickZone(rowIndex, colIndex);

    //时间
    timeStart = clock();
    return 0;
}

int MINE_GAME::clickZone(int rowIndex, int colIndex)
{

    if (graph[rowIndex][colIndex] & BIT_SHOWED)
        return graph[rowIndex][colIndex] & BIT_IS_BOMB;

    graph[rowIndex][colIndex] |= BIT_SHOWED;

    //是雷
    if (graph[rowIndex][colIndex] & BIT_IS_BOMB)
        return 1;

    //不是雷，画
    if (graph[rowIndex][colIndex] & BIT_MARKED) {   //处理mark
        graph[rowIndex][colIndex] ^= BIT_MARKED;
        cntMark--;
    }
    gmw_draw_block(&CGI, rowIndex, colIndex, graph[rowIndex][colIndex], bdi);

    //扩散
    return checkMark(rowIndex, colIndex);
}

int MINE_GAME::markSwitch(int rowIndex, int colIndex)
{
    //已经显示了的就不再进行标记操作
    if (graph[rowIndex][colIndex] & BIT_SHOWED) {
        return 0;
    }
    graph[rowIndex][colIndex] ^= BIT_MARKED;
    if (graph[rowIndex][colIndex] & BIT_MARKED) {
        gmw_draw_block(&CGI, rowIndex, colIndex, BIT_MARKED, bdi);
        cntMark++;
    }
    else {
        gmw_draw_block(&CGI, rowIndex, colIndex, graph[rowIndex][colIndex], bdi);
        cntMark--;
    }

    checkMark(rowIndex, colIndex);

    return 0;
}

int MINE_GAME::checkMark(int rowIndex, int colIndex)
{
    //没点开，不检查
    if (!(graph[rowIndex][colIndex] & BIT_SHOWED))
        return 0;

    int cntMark = 0;
    int GG = 0;   //记录是否游戏结束
    for (int k = 0, ki, kj; k < 8; k++)
        if (inZone(ki = rowIndex + fx[k][0], kj = colIndex + fx[k][1], row, col))
            if (graph[ki][kj] & BIT_MARKED)
                cntMark++;

    //标记数不一样，不检查
    if (cntMark != (graph[rowIndex][colIndex] & BITS_VAL))
        return 0;

    //标记数够，检查，检查方法是直接把自己和周围没标记的点开，标记错的也换个显示
    GG |= clickZone(rowIndex, colIndex);
    for (int k = 0, ki, kj; k < 8; k++) {
        if (inZone(ki = rowIndex + fx[k][0], kj = colIndex + fx[k][1], row, col))
            if (~graph[ki][kj] & BIT_MARKED)  //没标记
                GG |= clickZone(ki, kj);
            else if (~graph[ki][kj] & BIT_IS_BOMB)  //标记了
                gmw_draw_block(&CGI, ki, kj, FAIL_MARKED, bdi);
    }

    return GG;
}

void MINE_GAME::showBombs()
{
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            if (graph[i][j] & BIT_IS_BOMB) {
                graph[i][j] |= BIT_SHOWED;
                gmw_draw_block(&CGI, i, j, graph[i][j], bdi);
            }
}

int MINE_GAME::showCnt()
{
    char tmp[50];
    sprintf(tmp, "总雷数-标记雷数：%d", cntBomb - cntMark);
    gmw_status_line(&CGI, TOP_STATUS_LINE, tmp);
    return 0;
}

int MINE_GAME::showTime()
{
    char tmp[50];
    double t_sec = 1.0 * (clock() - timeStart) / CLOCKS_PER_SEC;
    sprintf(tmp, "目前用时[秒]：%f", t_sec);

    gmw_status_line(&CGI, TOP_STATUS_LINE, tmp);
    Sleep(1000);
    return 0;
}

//-1:左键单击
//-2:右键单击
//-3:中键单击
//正数:ASCII
//0:未知
int MINE_GAME::GetOpr(int &_oprRow, int &_oprCol)
{
    int oprRow = _oprRow, oprCol = _oprCol;
    int MAction, k1, k2;
    int opr;

    while (1) {
        opr = gmw_read_keyboard_and_mouse(&CGI, MAction, oprRow, oprCol, k1, k2);
        if (opr == CCT_KEYBOARD_EVENT) {    //键盘事件
            if (k2 == 0) {
                return k1;
            }
            else {
                return 0;
            }
        }
        else {      //鼠标事件
            if (MAction == MOUSE_LEFT_BUTTON_CLICK) {
                _oprRow = oprRow;
                _oprCol = oprCol;
                return -1;
            }
            if (MAction == MOUSE_RIGHT_BUTTON_CLICK) {
                _oprRow = oprRow;
                _oprCol = oprCol;
                return -2;
            }
            if (MAction == MOUSE_WHEEL_CLICK) {
                _oprRow = oprRow;
                _oprCol = oprCol;
                return -3;
            }
        }
    }

    return 0;
}



int MINE_GAME::win()
{
    if (cntBomb != cntMark)
        return 0;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            if ((graph[i][j] & BIT_IS_BOMB) && !(graph[i][j] & BIT_MARKED))
                return 0;

    char tmp[50];
    double t_sec = 1.0 * (clock() - timeStart) / CLOCKS_PER_SEC;
    sprintf(tmp, "你赢了，用时[秒]：%f", t_sec);
    gmw_status_line(&CGI,TOP_STATUS_LINE,tmp);

    return 1;
}