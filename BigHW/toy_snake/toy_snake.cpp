/* 朱泽凯 2051995 济美 */
#include "../include/hehepigDraw.h"
#include "../include/hehepigIO.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <ctime>
using namespace std;

void Play(int _r, int _c) {
    hehepig_block_map MAP(_r, _c);
    static const int fx[4][2] = { {0, 1},{-1, 0},{0, -1},{1, 0} };
    vector<vector<int>> a(_r, vector<int>(_c, -1));
    int nowx = 0, nowy = 0, tailx = 0, taily = 0;
    int nowdir = 0, lstdir = 0;
    int SPD = 10;
    int op, tmp = 0;
    int Score = 0;
    clock_t t_start, t_cur, t_lst;

    MAP.GraphicalInit(_r, _c, 1, 1);
    MAP.A[nowx][nowy] = hehepig_block(2, 5);
    int appleX = rand() % _r, appleY = rand() % _c;
    MAP.A[appleX][appleY] = hehepig_block(3, 4);

    int ZuoBi = 0;

    MAP.GraphicalUpgrade();


    a[nowx][nowy] = nowdir;
    t_start = t_lst = clock();
    while (t_cur = clock()) {


        op = AskKey();
        if (op != key_none)
            printf("\n%d ", op);

        if (op == key_right && (lstdir & 1))
            nowdir = 0;
        else if (op == key_up && ((lstdir & 1) ^ 1))
            nowdir = 1;
        else if (op == key_left && (lstdir & 1))
            nowdir = 2;
        else if (op == key_down && ((lstdir & 1) ^ 1))
            nowdir = 3;
        else if (op == '+' || op == 'x')    //加速
            SPD = min(SPD + 1, 50);
        else if (op == '-' || op == 'z')    //减速
            SPD = max(SPD - 1, 1);
        else if (op == 'c') {   //作弊
            ZuoBi ^= 1;
            printf("\n\n%d", ZuoBi);
        }
        else {
            //blank
        }

        if (op == key_left) {
            int aaa = 0;
        }

        if (double(t_cur - t_lst) / CLOCKS_PER_SEC >= 1.0 / SPD) {

            a[nowx][nowy] = nowdir;

            // 头前进
            MAP.A[nowx][nowy] = hehepig_block(1, 3);
            nowx = (nowx + fx[nowdir][0] + _r) % _r;
            nowy = (nowy + fx[nowdir][1] + _c) % _c;
            MAP.A[nowx][nowy] = hehepig_block(2, 5);
            
            //判定
            if (a[nowx][nowy] == -1) {
                a[nowx][nowy] = 4;  //专门为头设一个值
            }
            else {
                if (ZuoBi) {//作弊，回退
                    MAP.A[nowx][nowy] = hehepig_block(1, 3);
                    nowx = (nowx - fx[nowdir][0] + _r) % _r;
                    nowy = (nowy - fx[nowdir][1] + _c) % _c;
                    MAP.A[nowx][nowy] = hehepig_block(2, 5);
                }
                else {
                    printf("按q重来");
                    GetKey('q', 'q');
                    return;
                }
            }

            //尾巴前进
            if (nowx != appleX || nowy != appleY) { //没吃到苹果，尾巴要前进
                tmp = a[tailx][taily];
                a[tailx][taily] = -1;
                MAP.A[tailx][taily] = hehepig_block(0, 0);

                tailx = (tailx + fx[tmp][0] + _r) % _r;
                taily = (taily + fx[tmp][1] + _c) % _c;
            }
            else {  //吃到苹果，尾巴不用前进，苹果要新建
                Score++;
                while (a[appleX][appleY] != -1) {
                    appleX = rand() % _r;
                    appleY = rand() % _c;
                }
                MAP.A[appleX][appleY] = hehepig_block(3, 4);
            }


            MAP.GraphicalUpgrade();
            printf("SPD: %-3d ZuoBi: %-2d Score:%-3d", SPD, ZuoBi, Score);
            lstdir = nowdir;
            t_lst = t_cur;
        }
    }
}

int main() {
    int r, c;
    while (1) {
        r = GetInt("\nrow (5-20）\n> ", 5, 20);
        c = GetInt("\ncol (5-20) \n >", 5, 20);
        Play(r, c);
    }
}