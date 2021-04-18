/* 朱泽凯 2051995 济美 */
#include "90-02-b1-gmw.h"
#include "../include/cmd_gmw_tools.h"
#include "../include/cmd_console_tools.h"
#include "../include/hehepigIO.h"
#include <ctime>
#include <iostream>
using namespace std;

static const int fx[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

inline bool inZone(int x, int y, int mx, int my)
{
    return (x >= 0 && x < mx &&y >= 0 && y < my);
}

const BLOCK_DISPLAY_INFO bdi[] = {
    {0, COLOR_WHITE, COLOR_WHITE, "  "},
    {1, COLOR_RED, COLOR_BLACK, "★"},
    {2, COLOR_BLUE, COLOR_BLACK, "★"},
    {3, COLOR_GREEN, COLOR_BLACK, "★"},
    {4, COLOR_CYAN, COLOR_BLACK, "★"},
    {5, COLOR_YELLOW, COLOR_BLACK, "★"},

    {0 | BIT_FOCUS, COLOR_WHITE, COLOR_HWHITE, "  "},
    {1 | BIT_FOCUS, COLOR_RED, COLOR_HWHITE, "★"},
    {2 | BIT_FOCUS, COLOR_BLUE, COLOR_HWHITE, "★"},
    {3 | BIT_FOCUS, COLOR_GREEN, COLOR_HWHITE, "★"},
    {4 | BIT_FOCUS, COLOR_CYAN, COLOR_HWHITE, "★"},
    {5 | BIT_FOCUS, COLOR_YELLOW, COLOR_HWHITE, "★"},

    {0 | BIT_SELECTED, COLOR_WHITE, COLOR_HWHITE, "  "},
    {1 | BIT_SELECTED, COLOR_RED, COLOR_HRED, "★"},
    {2 | BIT_SELECTED, COLOR_BLUE, COLOR_HBLUE, "★"},
    {3 | BIT_SELECTED, COLOR_GREEN, COLOR_HGREEN, "★"},
    {4 | BIT_SELECTED, COLOR_CYAN, COLOR_HCYAN, "★"},
    {5 | BIT_SELECTED, COLOR_YELLOW, COLOR_PINK, "★"},

    {0 | BIT_SELECTED | BIT_FOCUS, COLOR_WHITE, COLOR_HWHITE, "  "},
    {1 | BIT_SELECTED | BIT_FOCUS, COLOR_RED, COLOR_HWHITE, "★"},
    {2 | BIT_SELECTED | BIT_FOCUS, COLOR_BLUE, COLOR_HWHITE, "★"},
    {3 | BIT_SELECTED | BIT_FOCUS, COLOR_GREEN, COLOR_HWHITE, "★"},
    {4 | BIT_SELECTED | BIT_FOCUS, COLOR_CYAN, COLOR_HWHITE, "★"},
    {5 | BIT_SELECTED | BIT_FOCUS, COLOR_YELLOW, COLOR_HWHITE, "★"},

    {BDI_VALUE_END, COLOR_WHITE, COLOR_WHITE, "  "}
};

int STAR_GAME::reset(int _row, int _col)
{
    row = _row;
    col = _col;
    ScoreOld = 0;
    score = 0;

    //srand(time(0));

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            graph[i][j] = rand() % 5 + 1;

    gmw_init(&CGI, row, col);
    gmw_set_frame_style(&CGI, 6, 3, true);
    gmw_set_frame_color(&CGI, COLOR_WHITE, COLOR_HWHITE);
    gmw_set_block_border_switch(&CGI, true);
    gmw_draw_frame(&CGI);

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            gmw_draw_block(&CGI, i, j, graph[i][j], bdi);

    return 0;
}

int STAR_GAME::dfs(int rowIndex, int colIndex)
{

    if ((graph[rowIndex][colIndex] & BITS_VAL) == 0)
        return 0;

    int ret = 1;
    vis[rowIndex][colIndex] = 1;
    for (int k = 0, ki, kj; k < 4; k++)
        if (inZone(ki = rowIndex + fx[k][0], kj = colIndex + fx[k][1], row, col))
            if (vis[ki][kj] == 0 && (graph[ki][kj]&BITS_VAL) == (graph[rowIndex][colIndex]&BITS_VAL))
                ret += dfs(ki, kj);
    return ret;
}

int STAR_GAME::select(int rowIndex, int colIndex)
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
        for (int i=0; i<row; i++)
            for (int j = 0; j < col; j++)
                if (vis[i][j]) {
                    graph[i][j] |= BIT_SELECTED;
                    gmw_draw_block(&CGI, i, j, graph[i][j], bdi);
                }
    }

    return ret;
}

int STAR_GAME::confirm()
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

int STAR_GAME::down()
{
    for (int j = 0, cur = row - 1; j < col; j++, cur = row - 1) {
        for (int i = row - 1; i >= 0; i--)
            if (graph[i][j] & BITS_VAL) {
                graph[cur][j] = graph[i][j];
                gmw_move_block(&CGI, i, j, graph[i][j], 0, bdi, UP_TO_DOWN, cur-i);
                cur--;
            }
        while (cur >= 0) {
            if (graph[cur][j] & BITS_VAL) {
                graph[cur][j] = 0;
                gmw_draw_block(&CGI, cur, j, 0, bdi);
            }
            cur--;
        }

    }
    return 0;
}

int STAR_GAME::left()
{
    int tmp[MAXCOL];
    int i, j, jj, cnt, tcnt = 0;
    for (j = 0, cnt = 0; j < col; j++) {
        for (i = tcnt = 0; i < row; i++)
            tcnt += ((graph[i][j] & BITS_VAL) != 0);
        tmp[j] = cnt;
        cnt += (tcnt == 0);
    }

    for (j = jj = 0; j < col; j++) {
        jj = j - tmp[j];
        for (i = 0; i < row; i++) 
            if (j != jj && (graph[i][j]&BITS_VAL)) {
                graph[i][jj] = graph[i][j];
                gmw_move_block(&CGI, i, j, graph[i][j], 0, bdi, RIGHT_TO_LEFT, j - jj);
                graph[i][j] = 0;
            }
    }

    return 0;
}

bool STAR_GAME::checkFail()
{
    for (int i=0; i<row; i++)
        for (int j = 0; j < col; j++) {
            for (int ii = 0; ii < row; ii++)
                for (int jj = 0; jj < col; jj++)
                    vis[ii][jj] = 0;
            if (dfs(i, j) > 1)
                return false;
        }

    gmw_status_line(&CGI, LOWER_STATUS_LINE, "无法消了，游戏结束，祝您愉快 :)");

    return true;
}

int STAR_GAME::getOpr(int &_oprRow, int &_oprCol)
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

int STAR_GAME::isSelected(int rowIndex, int colIndex)
{
    return graph[rowIndex][colIndex]&BIT_SELECTED;
}

int STAR_GAME::focusSwitch(int rowIndex, int colIndex, int val)
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