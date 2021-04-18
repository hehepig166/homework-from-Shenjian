/* 朱泽凯 2051995 济美 */
#define _CRT_SECURE_NO_WARNINGS
#include "90-02-b2.h"
#include "../include/cmd_gmw_tools.h"
#include "../include/cmd_console_tools.h"
#include "../include/hehepigIO.h"
#include <iostream>
#include <cstdio>
#include <Windows.h>
using namespace std;

static const int fx[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

inline bool inZone(int x, int y, int mx, int my)
{
    return (x >= 0 && x < mx &&y >= 0 && y < my);
}

static BLOCK_DISPLAY_INFO bdi[] = {
    {1, 1, COLOR_BLACK, "1"},
    {2, 2, COLOR_BLACK, "2"},
    {3, 3, COLOR_BLACK, "3"},
    {4, 4, COLOR_BLACK, "4"},
    {5, 5, COLOR_BLACK, "5"},
    {6, 6, COLOR_BLACK, "6"},
    {7, 9, COLOR_BLACK, "7"},
    {8, 10, COLOR_BLACK, "8"},
    {9, 11, COLOR_BLACK, "9"},
    {10, 12, COLOR_BLACK, "10"},

    {1 | BIT_FOCUS, 1, COLOR_HWHITE, "1"},
    {2 | BIT_FOCUS, 2, COLOR_HWHITE, "2"},
    {3 | BIT_FOCUS, 3, COLOR_HWHITE, "3"},
    {4 | BIT_FOCUS, 4, COLOR_HWHITE, "4"},
    {5 | BIT_FOCUS, 5, COLOR_HWHITE, "5"},
    {6 | BIT_FOCUS, 6, COLOR_HWHITE, "6"},
    {7 | BIT_FOCUS, 9, COLOR_HWHITE, "7"},
    {8 | BIT_FOCUS, 10, COLOR_HWHITE,  "8"},
    {9 | BIT_FOCUS, 11, COLOR_HWHITE, "9"},
    {10 | BIT_FOCUS, 12, COLOR_HWHITE, "10"},

    {1 | BIT_SELECTED, 1, COLOR_WHITE, "1"},
    {2 | BIT_SELECTED, 2, COLOR_WHITE, "2"},
    {3 | BIT_SELECTED, 3, COLOR_WHITE, "3"},
    {4 | BIT_SELECTED, 4, COLOR_WHITE, "4"},
    {5 | BIT_SELECTED, 5, COLOR_WHITE, "5"},
    {6 | BIT_SELECTED, 6, COLOR_WHITE, "6"},
    {7 | BIT_SELECTED, 9, COLOR_WHITE, "7"},
    {8 | BIT_SELECTED, 10, COLOR_WHITE, "8"},
    {9 | BIT_SELECTED, 11, COLOR_WHITE, "9"},
    {10 | BIT_SELECTED, 12, COLOR_WHITE, "10"},

    {1 | BIT_FOCUS | BIT_SELECTED, 1, COLOR_HWHITE, "1"},
    {2 | BIT_FOCUS | BIT_SELECTED, 2, COLOR_HWHITE, "2"},
    {3 | BIT_FOCUS | BIT_SELECTED, 3, COLOR_HWHITE, "3"},
    {4 | BIT_FOCUS | BIT_SELECTED, 4, COLOR_HWHITE, "4"},
    {5 | BIT_FOCUS | BIT_SELECTED, 5, COLOR_HWHITE, "5"},
    {6 | BIT_FOCUS | BIT_SELECTED, 6, COLOR_HWHITE, "6"},
    {7 | BIT_FOCUS | BIT_SELECTED, 9, COLOR_HWHITE, "7"},
    {8 | BIT_FOCUS | BIT_SELECTED, 10, COLOR_HWHITE, "8"},
    {9 | BIT_FOCUS | BIT_SELECTED, 11, COLOR_HWHITE, "9"},
    {10 | BIT_FOCUS | BIT_SELECTED, 12, COLOR_HWHITE, "10"},

    {BDI_VALUE_END, COLOR_WHITE, COLOR_WHITE, "  "}
};

int HCS_GAME::reset(int _row, int _col, int _winNum)
{
    row = _row;
    col = _col;
    winNum = _winNum;
    score = 0;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            graph[i][j] = rand() % 4 + 1;

    gmw_init(&CGI, row, col);
    gmw_set_frame_style(&CGI, 6, 3, false);
    gmw_set_frame_color(&CGI, COLOR_WHITE, COLOR_HWHITE);
    gmw_set_block_border_switch(&CGI, true);
    gmw_set_ext_rowcol(&CGI, 1);
    gmw_set_status_line_color(&CGI, TOP_STATUS_LINE, -1, -1, -1, COLOR_RED);
    gmw_draw_frame(&CGI);

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            gmw_draw_block(&CGI, i, j, graph[i][j], bdi);

    char msg[50];
    sprintf(msg, "合成目标[%d]  当前得分[%d]", winNum, score);
    gmw_status_line(&CGI, TOP_STATUS_LINE, msg);

    return 0;
}

//和消灭星星一模一样
int HCS_GAME::dfs(int rowIndex, int colIndex)
{

    if ((graph[rowIndex][colIndex] & BITS_VAL) == 0)
        return 0;

    int ret = 1;
    vis[rowIndex][colIndex] = 1;
    for (int k = 0, ki, kj; k < 4; k++)
        if (inZone(ki = rowIndex + fx[k][0], kj = colIndex + fx[k][1], row, col))
            if (vis[ki][kj] == 0 && (graph[ki][kj] & BITS_VAL) == (graph[rowIndex][colIndex] & BITS_VAL))
                ret += dfs(ki, kj);
    return ret;
}

//和消灭星星一模一样
int HCS_GAME::select(int rowIndex, int colIndex)
{
    int ret;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++) {

            //vis 先全置零
            vis[i][j] = 0;

            //清除之前弃选的SELECTED
            if (graph[i][j] & BIT_SELECTED) {
                graph[i][j] &= graph[i][j] ^ BIT_SELECTED;
                gmw_draw_block(&CGI, i, j, graph[i][j], bdi);
            }
        }

    //dfs
    ret = dfs(rowIndex, colIndex);

    //该点可以选择才选择，更新图像
    if (ret > 1) {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                if (vis[i][j]) {
                    graph[i][j] |= BIT_SELECTED;
                    gmw_draw_block(&CGI, i, j, graph[i][j], bdi);
                }

        char tmp[50];
        sprintf(tmp, "数字[%d] 数量[%d] 可加分[%d]", graph[rowIndex][colIndex] & BITS_VAL, ret, ret * ((graph[rowIndex][colIndex] & BITS_VAL) + 1));
        gmw_status_line(&CGI, TOP_STATUS_LINE, tmp);

    }

    return ret;
}

//和消灭星星一模一样
int HCS_GAME::confirm()
{
    int cnt = 0;
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            if (graph[i][j] & BIT_SELECTED) {
                cnt++;
                graph[i][j] = 0;
                gmw_draw_block(&CGI, i, j, 0, bdi);
            }
    return cnt;
}

int HCS_GAME::setVal(int rowIndex, int colIndex, int val)
{
    graph[rowIndex][colIndex] = val | (graph[rowIndex][colIndex] & (graph[rowIndex][colIndex] ^ BITS_VAL));
    gmw_draw_block(&CGI, rowIndex, colIndex, graph[rowIndex][colIndex], bdi);
    return 0;
}

//与消灭星星不同之处在于，空出来的要补上数字
int HCS_GAME::down()
{
    for (int j = 0, cur = row - 1; j < col; j++, cur = row - 1) {
        for (int i = row - 1; i >= 0; i--)
            if (graph[i][j] & BITS_VAL) {
                graph[cur][j] = graph[i][j];
                gmw_move_block(&CGI, i, j, graph[i][j], 0, bdi, UP_TO_DOWN, cur - i);
                cur--;
            }
        while (cur >= 0) {
            graph[cur][j] = (rand() % 4 + 1) | (graph[cur][j] & (graph[cur][j] ^ BITS_VAL));
            gmw_draw_block(&CGI, cur, j, graph[cur][j], bdi);
            cur--;
        }
    }
    return 0;
}

bool HCS_GAME::checkWin()
{
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            if ((graph[i][j] & BITS_VAL) == winNum) {
                char tmp[50];
                sprintf(tmp, "赢了，得分[%d]", score);
                gmw_status_line(&CGI, TOP_STATUS_LINE, "", tmp);
                return true;
            }

    return false;
}

//和消灭星星那里一模一样
int HCS_GAME::getOpr(int &_oprRow, int &_oprCol)
{
    int oprRow = _oprRow, oprCol = _oprCol, lr, lc;
    int MAction, k1, k2;
    int opr;

    while (1) {

        lr = oprRow;
        lc = oprCol;

        opr = gmw_read_keyboard_and_mouse(&CGI, MAction, oprRow, oprCol, k1, k2);
        if (opr == CCT_KEYBOARD_EVENT) {    //键盘事件，对引用不做改变，直接返回值
            if (k2 == 0) {
                return k1;
            }
            else if (k1 == 0xe0) {
                switch (k2) {
                    case KB_ARROW_UP:
                        return key_up;
                    case KB_ARROW_DOWN:
                        return key_down;
                    case KB_ARROW_LEFT:
                        return key_left;
                    case KB_ARROW_RIGHT:
                        return key_right;
                }
            }
            else {
                return 0;
            }
        }
        else {  //鼠标事件
            if (MAction == MOUSE_LEFT_BUTTON_CLICK) {
                _oprRow = oprRow;
                _oprCol = oprCol;
                return -1;
            }
            else if (MAction == MOUSE_ONLY_MOVED) {
                _oprRow = oprRow;
                _oprCol = oprCol;
                focusSwitch(lr, lc, 0);
                gmw_draw_block(&CGI, lr, lc, graph[lr][lc], bdi);
                focusSwitch(oprRow, oprCol, 1);
                gmw_draw_block(&CGI, oprRow, oprCol, graph[oprRow][oprCol], bdi);
            }
        }
    }

    return 0;
}

//和消灭星星一样
int HCS_GAME::isSelected(int rowIndex, int colIndex)
{
    return graph[rowIndex][colIndex] & BIT_SELECTED;
}

//和消灭星星一样
int HCS_GAME::focusSwitch(int rowIndex, int colIndex, int val)
{
    if (val == 0)
        graph[rowIndex][colIndex] &= graph[rowIndex][colIndex] ^ BIT_FOCUS;
    else if (val == 1)
        graph[rowIndex][colIndex] |= BIT_FOCUS;
    else
        graph[rowIndex][colIndex] ^= BIT_FOCUS;
    gmw_draw_block(&CGI, rowIndex, colIndex, graph[rowIndex][colIndex], bdi);
    return 0;
}

int HCS_GAME::addScore(int addi)
{
    char msg[50];
    score += addi;
    sprintf(msg, "合成目标[%d]  当前得分[%d] 上次加分[%d]", winNum, score, addi);
    gmw_status_line(&CGI, TOP_STATUS_LINE, msg);

    return score;
}