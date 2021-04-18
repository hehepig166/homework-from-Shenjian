/* 朱泽凯 2051995 济美 */
#pragma once
#include "../include/cmd_gmw_tools.h"

#define MAXROW 20
#define MAXCOL 20

#define BIT_FOCUS 64
#define BIT_SELECTED 128
#define BITS_VAL 31

class STAR_GAME {

    //地图信息
    int graph[MAXROW][MAXCOL];
    int vis[MAXROW][MAXCOL];

    //行列
    int row;
    int col;

    int ScoreOld;
    int score;

    CONSOLE_GRAPHICS_INFO CGI;

public:

    //初始化，放星星并显示图案
    int reset(int _row, int _col);

    //搜索，返回连通块大小
    int dfs(int rowIndex, int colIndex);

    //选择，并以该点搜索向四周搜索，该点以及周围相同的点vis置1，其它点置0
    //若该点是0（即已经没有星星了），那么不执行，返回0
    //返回值：包含该点，周围与该点相同的个数
    int select(int rowIndex, int colIndex);

    //根据BIT_SELECTED确认并消除当前选择了的区域（&BIT_SELECTED）
    //返回值：消除的个数
    int confirm();

    //下移
    int down();

    //左移
    int left();

    //若无法消除了，返回1，否则返回0
    bool checkFail();

    //-1:左键单击
    //key_left / key_up / key_right / key_down 见 hehepigIO
    //1~127:ASCII
    //0:未知
    int getOpr(int &_oprRow, int &_oprCol);

    int isSelected(int rowIndex, int colIndex);

    //val缺省或不是(0或者1)则是切换
    int focusSwitch(int rowIndex, int colIndex, int val = -1);
};