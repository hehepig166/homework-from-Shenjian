/* 朱泽凯 2051995 济美 */
#pragma once

#include "../include/cmd_gmw_tools.h"

#define BITS_VAL 15
#define BIT_IS_BOMB 16
#define BIT_SHOWED 32
#define BIT_MARKED 64
#define FAIL_MARKED 128

class MINE_GAME {

    CONSOLE_GRAPHICS_INFO CGI;

    //存扫雷地图信息
    int **graph;

    //行数列数
    int row;
    int col;

    //还剩雷数
    int cntBomb;
    int cntMark;

    //时间信息
    clock_t timeStart;
    clock_t timeEnd;

public:

    //初始化，显示框架
    MINE_GAME(int _row, int _col, int _cntBomb);

    //析构，释放graph的空间
    ~MINE_GAME();

    //根据所给的起点创建地图(放雷)，保证起点不是雷
    int setBombs(int rowIndex, int colIndex);

    //点开该点，并从该点开始向四周扩撒（图形界面也更新）
    //若是雷则不更新图形
    //返回值是该点是否是雷（1表示是，0表示不是）
    int clickZone(int rowIndex, int colIndex);

    //切换该点标记状态（原来是标记则切换为未标记）
    //已经SHOW了的就不进行标记操作了
    int markSwitch(int rowIndex, int colIndex);

    //检查，只有当该点以点开且周围标记数与周围雷数相等时才检查（不相等时也返回0）
    //若周围有标错的则返回1
    //若标记数量等于周围雷数量，则没标记的的点开，可扩散的扩散，返回扩散后的检查
    //扩散完了都没错，则返回0
    //返回值：是否游戏结束
    int checkMark(int rowIndex, int colIndex);

    //显示所有地雷
    void showBombs();

    //在上状态栏显示雷数
    int showCnt();

    //在上状态栏显示用时
    int showTime();

    //-1:左键单击
    //-2:右键单击
    //-3:滚轮单击
    //正数:ASCII
    //0:未知
    int GetOpr(int &_oprRow, int &_oprCol);

    void lower_hint(const char *info)
    {
        gmw_status_line(&CGI, LOWER_STATUS_LINE, info);
    }

    int win();
};