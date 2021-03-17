/* 朱泽凯 2051995 济美 */
#include "./90-02-b1-star_play.h"
#include "../include/cmd_console_tools.h"
#include "../include/hehepigIO.h"
#include "../include/hehepigDraw.h"
#include "../include/hehepigMenu.h"
#include <ctime>
#include <iostream>
using namespace std;

#define MaxN 10
#define ColNum 5
#define RAN(x,y) (rand()%((y)-(x)+1)+(x))

const int HBMCOL[] = { COLOR_WHITE, COLOR_PINK, COLOR_GREEN, COLOR_YELLOW, COLOR_RED, COLOR_CYAN };

/// <summary>
/// 以给出的点为起点找值一样的连通块
/// </summary>
/// <param name="x, y">点的位置</param>
/// <param name="Row, Col">地图大小</param>
/// <param name="Map">地图值矩阵</param>
/// <param name="vis">提供的用于记录的矩阵（连通的赋值为1，不连通的为0）</param>
/// <param name="cnt">用于计数</param>
void FindConnected(int x, int y, int Row, int Col, int Map[][MaxN], int vis[][MaxN], int* cnt, int sides = 4) {
    static int fx[8][2] = { {0,1},{1,0},{0,-1},{-1,0},{-1,-1},{-1,1},{1,-1},{1,1} };
    vis[x][y] = 1;
    (*cnt)++;

    for (int F = 0; F < sides; F++) {
        int xx = x + fx[F][0];
        int yy = y + fx[F][1];
        if (xx >= 0 && xx < Row && yy >= 0 && yy < Col && !vis[xx][yy] && Map[xx][yy] == Map[x][y])
            FindConnected(xx, yy, Row, Col, Map, vis, cnt, sides);
    }
}


/// <summary>
/// 内涵一个 int 矩阵，各种函数对该矩阵操作
/// </summary>
class Play {
public:
    int Map[MaxN][MaxN];
    int Row;
    int Col;

    int vis[MaxN][MaxN];    // 辅助搜索用的数组
    int ConCnt;             // 存搜索到的个数

    int OldScore;           // 上一次操作后的总得分
    int Score;              // 总得分
    int Num;                // 还剩的星星数

    /// <summary>
    /// 读入行列，新建地图
    /// </summary>
    void Init() {
        //srand(time(0));

        Row = GetInt("行（3-10） > ", 3, MaxN);
        cout << "  !行数：" << Row << endl;
        Col = GetInt("列（3-10） > ", 3, MaxN);
        cout << "  !列数：" << Col << endl;
        for (int i = 0; i < Row; i++)
            for (int j = 0; j < Col; j++) {
                vis[i][j] = 0;
                Map[i][j] = RAN(1, ColNum);
            }

        OldScore = 0;
        Score = 0;
        Num = 0;
    }

    /// <summary>
    /// 搜索与点 (x,y) 相连且相同的，返回个数（个数不包含该点）
    /// </summary>
    int Find(int x, int y) {

        if (Map[x][y] == 0)
            return 0;

        for (int i = 0; i < Row; i++)
            for (int j = 0; j < Col; j++)
                vis[i][j] = 0;
        ConCnt = 0;    //-1是为了让最后得到的不包含自己

        FindConnected(x, y, Row, Col, Map, vis, &ConCnt);
        return ConCnt;
    }

    /// <summary>
    /// 根据上一次 FindConnected 得出的 vis 中的消掉星星（A中对应的值变为0）
    /// </summary>
    void Confirm() {
        for (int i = 0; i < Row; i++)
            for (int j = 0; j < Col; j++)
                if (vis[i][j])
                    Map[i][j] = vis[i][j] = 0;

        Num -= ConCnt;
        OldScore = Score;
        Score += (ConCnt * ConCnt * 5);
        
    }

    /// <summary>
    /// 整理A，往下掉消空位
    /// </summary>
    void Down() {
        for (int j = 0, cur = Row - 1; j < Col; j++, cur = Row - 1) {
            for (int i = Row - 1; i >= 0; i--)
                if (Map[i][j])
                    Map[cur--][j] = Map[i][j];
            while (cur >= 0)
                Map[cur--][j] = 0;
        }
    }

    /// <summary>
    /// 整理A，往左挪消空位
    /// </summary>
    void Left() {
        int tmp[MaxN] = { 0 };
        int i, j, jj, cnt, tcnt = 0;
        for (j = 0, cnt = 0; j < Col; j++) {
            for (i = tcnt = 0; i < Row; i++)
                tcnt += (Map[i][j] != 0);
            tmp[j] = cnt;
            cnt += (tcnt == 0);
        }
        for (j = 0, jj = j - tmp[j]; j < Col; j++, jj = j - tmp[j])
            for (i = 0; i < Row; i++)
                Map[i][jj] = Map[i][j];

        while (jj < Col) {
            for (i = 0; i < Row; i++)
                Map[i][jj] = 0;
        }

    }

    /// <summary>
    /// 检查是否无法移动
    /// </summary>
    bool CheckFail() {
        for (int i = 0; i < Row; i++)
            for (int j = 0; j < Col; j++)
                if (Find(i, j)>1)
                    return false;


        OldScore = Score;
        Score += Num > 10 ? 0 : (10 - Num) * 10;

        return true;
    }


    /// <summary>
    /// 将当前矩阵信息更新到 HBM 中
    /// </summary>
    /// <param name="HBM"></param>
    void Upgrade(hehepig_block_map* HBM) {
        for (int i = 0; i < Row; i++)
            for (int j = 0; j < Col; j++) {
                HBM->A[i][j].Val = Map[i][j];
                HBM->A[i][j].BgColor = HBMCOL[Map[i][j]];
                HBM->A[i][j].FtColor = (Map[i][j] == 0 || vis[i][j]) ? COLOR_WHITE : COLOR_BLACK;
            }
    }
};


///================================================
/// DigitalPlay
/// 
///================================================

void DigitalPlay::Init() {
    static Play _P;
    P = &_P;
    P->Init();
    Row = P->Row;
    Col = P->Col;

    HBM = hehepig_block_map(Row, Col);
    P->Upgrade(&HBM);

    HBM.DigitalLogError("\n前数组：\n");
    HBM.DigitalPrint();

}

void DigitalPlay::GetXY(int& x, int& y) {
    HBM.DigitalLogError("\n输入坐标[例：b2] > ");
    while (1) {
        x = GetKey();
        if (isalpha(x) && (x = (toupper(x) - 'A')) < Row)
            break;
    }
    putchar(x + 'A');   //回显
    y = GetKey('0', '0' + Col - 1, 1) - '0';
}


/*****
A       "命令行找出可消除项并标识",
B       "命令行完成一次消除（分步骤显示）",
C       "命令行完成一关（分步骤显示）",
*****/
void DigitalPlay::Begin(int Mode) {

    Init();

    int nxtX, nxtY;

    while (1) {
        while (1) {
            GetXY(nxtX, nxtY);
            if (P->Find(nxtX, nxtY)>1) {
                HBM.DigitalLogError("\n此处有效，以下是搜索结果：（白色字表示可笑的星星）\n");
                P->Upgrade(&HBM);
                HBM.DigitalPrint();
                break;
            }
            else {
                HBM.DigitalLogError("\n无效输入或此处星星不可消\n");
            }
        }

        if (Mode == 'A') {  //=======================================================A
            HBM.DigitalLogError("\n结束，按q返回菜单\n");
            GetKey('q', 'q');
            return;
        }

        HBM.DigitalLogError("\n是否消灭星星？\n");
        const char* tmpstr[] = { "确认选择操作","重来选择操作" ,"放弃游戏" };
        int opr = Menu_char(3, "YNQ", tmpstr);
        opr = toupper(opr);
        putchar(opr);
        puts("");
        if (opr == 'N')
            continue;
        else if (opr == 'Q') {
            return;
        }
        else if (opr == 'Y') {
            // 确认删除
            P->Confirm();
            P->Upgrade(&HBM);
            HBM.DigitalLogError("\n删除后：\n");
            HBM.DigitalPrint();
            HBM.DigitalLogError("\n按回车键继续（往下块挪）\n");
            GetKey('\r', '\r');

            // 往下块挪
            P->Down();
            P->Upgrade(&HBM);

            for (int i = 0; i < Row; i++) {
                for (int j = 0; j < Col; j++)
                    cout << P->Map[i][j] << " ";
                puts("");
            }

            HBM.DigitalPrint();
            HBM.DigitalLogError("\n按回车键继续（往左列挪）\n");
            GetKey('\r', '\r');

            // 往左列挪
            P->Left();
            P->Upgrade(&HBM);
            HBM.DigitalPrint();
        }

        if (Mode == 'B') {  //=======================================================B
            HBM.DigitalLogError("\n结束，按q返回菜单\n");
            GetKey('q', 'q');
            return;
        }

        if (P->CheckFail()) {
            cct_setcolor(0, COLOR_RED);
            cout << "结束，总得分：" << P->Score << " = " << P->OldScore << " + " << P->Score - P->OldScore << endl;
            HBM.DigitalLogError("\n按q返回菜单\n");
            cct_setcolor();
            GetKey('q', 'q');
            return;
        }
        else {
            cout << "目前得分：" << P->Score << " = "<<P->OldScore <<" + " <<P->Score-P->OldScore <<endl;
        }
    }

}
