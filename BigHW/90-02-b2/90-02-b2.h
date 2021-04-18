/* 朱泽凯 2051995 济美 */
#pragma once
#include "../include//cmd_gmw_tools.h"

#define MAXROW 20
#define MAXCOL 20

#define BIT_FOCUS 64
#define BIT_SELECTED 128
#define BITS_VAL 31

class HCS_GAME {

    //地图信息
    int graph[MAXROW][MAXCOL];
    int vis[MAXROW][MAXCOL];

    int row;
    int col;

    //目标数字
    int winNum;

    //每次消除，加的分： num*cnt
    int score;

    CONSOLE_GRAPHICS_INFO CGI;

public:

    //初始化，放数字并显示图案
    int reset(int _row, int _col, int _winNum);

    //搜索，返回连通块大小，和消灭星星那里一样
    int dfs(int rowIndex, int colIndex);

    //选择，和消灭星星那里一样
    int select(int rowIndex, int colIndex);

    //消除，和消灭星星那里一样
    int confirm();

    //修改,获得数字，不改变BIT_FOCUS等状态，不检查val合法性
    int setVal(int rowIndex, int colIndex, int val);
    int getVal(int rowIndex, int colIndex)
    {
        return graph[rowIndex][colIndex] & BITS_VAL;
    }

    //下移并补充数字
    int down();

    //返回是否赢了（即是否出现Max）
    bool checkWin();

    //-1:左键单击
    //key_left / key_up / key_right / key_down 见 hehepigIO
    //1~127:ASCII
    //0:未知
    //和消灭星星一样
    int getOpr(int &_oprRow, int &_oprCol);

    int isSelected(int rowIndex, int colIndex);

    //val缺省或不是(0或者1)则是切换
    int focusSwitch(int rowIndex, int colIndex, int cal = -1);

    //加分
    //返回加完后的分
    int addScore(int addi);
};