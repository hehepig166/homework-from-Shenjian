/* 朱泽凯 智交 2051995 */
#include <iostream>
#include <conio.h>
#include <ctime>
#include <iomanip>
#include "cmd_console_tools.h"
#include "mine_sweeper.h"
using namespace std;



int my_getch(const int L, const int R)
{
	int mx, my, mact, k1, k2;
	while (1) {
		cct_read_keyboard_and_mouse(mx, my, mact, k1, k2);
		if (k2 == 0 && k1 >= L && k1 <= R)
			break;
	}
	return k1;
}

/*********************************************************************
功能：	以 *map 为 M[0][0]
		随机出一个含有 bombcnt 个炸弹，高 X 长 Y 的地图
		其中保证 M[X][Y] 肯定不是炸弹

注：	有效地图下标从 1 开始
		XY默认都为 1
		不进行参数合理性的判断，若参数不合理可能会出错
		炸弹处 M 值为 -1
		其它处 M 值为 周围炸弹数
**********************************************************************/
void newmap(int map[][Maxlen], const int Xlen, const int Ylen, int bombcnt, const int X, const int Y)
{

	srand((unsigned int)(time(0)));

	for (int i = 0; i <= Xlen; i++)
		for (int j = 0; j <= Ylen; j++)
			map[i][j] = 0;

	for (int x, y, OK; bombcnt > 0; ) {
		x = rand() % Xlen + 1;
		y = rand() % Ylen + 1;

		OK = 1;
		for (int i = 0; i < 9 && OK; i++)
			OK = x + fx[i][0] != X && y + fx[i][1] != Y;

		if (!OK || map[x][y] < 0)
			continue;

		bombcnt--;
		map[x][y] = -1;
		for (int f = 0, xx, yy; f < 8; f++) {
			xx = x + fx[f][0];
			yy = y + fx[f][1];

			map[xx][yy] += map[xx][yy] >= 0;
		}
	}
	
	return;
#undef M
}

/*******************************
tag=1	内部数组简单版
tag=2	内部数组完整版
tag=3	伪图形简单版	//挪移到 get_graph_opr 里了
tag=4	伪图形完整版	//挪移到 get_graph_opr 里了

ret.mode
		0:打开
		1:插/收旗
		2:运行时间
		3:退出
		4:检查

********************************/
OPERATION get_opr(int tag,const GMDATA &D)
{
	OPERATION ret;
	if (tag == 1 || tag == 2) {		//键盘输入版
		int mx, my, mact, k1, k2;

		if (tag == 2) {
			puts("");
			puts("特殊输入：& - 游戏已运行时间");
			puts("          ! - 标记/取消标记（例： !A1）");
			puts("          % - 检查（要周围标记数符合才行）（例：%Ab）");
		}
		cout <<("请输入（先行后列，严格区分大小写），q/Q 退出： ");
		while (1) {

			cct_read_keyboard_and_mouse(mx, my, mact, k1, k2=1);
			if (k2 != 0)
				continue;

			if (k1 == '&' && tag==2) {	//时间
				ret.mode = OPR_TIME;
				cout << '&';
				return ret;
			}		
			else if ((k1 == '!' || k1=='%') && tag == 2) {
				ret.mode = (k1=='!' ? OPR_FLAG : OPR_CHEK);
				cout << char(k1);
				ret.X = my_getch('A', 'A' + D.Xlen - 1) - 'A' + 1;
				cout << char(ret.X + 'A'-1);
				while (cct_read_keyboard_and_mouse(mx, my, mact, k1, k2)) {
					if (k2 != 0)
						continue;
					else if (k1 >= '1' && k1 <= '9') {
						ret.Y = k1 - '0';
						cout << char(k1);
					}
					else if (k1 >= 'a' && k1 <= 'a' + D.Ylen - 10) {
						ret.Y = k1 - 'a' + 10;
						cout << char(k1);
					}
					else
						continue;
					break;
				}
				return ret;
			}
			else if (k1 == 'q' || k1 == 'Q') {
				ret.mode = OPR_EXIT;
				cout << char(k1);
				return ret;
			}
			else if (k1 >= 'A' && k1 <= D.Xlen + 'A') {
				ret.mode = OPR_OPEN;
				cout << char(k1);
				ret.X = k1-'A'+1;
				while (cct_read_keyboard_and_mouse(mx, my, mact, k1, k2)) {
					if (k2 != 0)
						continue;
					else if (k1 >= '1' && k1 <= '9') {
						ret.Y = k1 - '0';
						cout << char(k1);
					}
					else if (k1 >= 'a' && k1 <= 'a' + D.Ylen - 10) {
						ret.Y = k1 - 'a' + 10;
						cout << char(k1);
					}
					else
						continue;

					break;
				}
				return ret;
			}
		}
	}
	else {
		ret.mode = 0;
		return ret;
	}
}

/*
ret.mode
		0:打开
		1:插/收旗
		2:运行时间
		3:退出

*/
void move(OPERATION opr, GMDATA &D)
{
	if (opr.mode == 3)
		return;


	if (opr.mode == 0 && D.tag[opr.X][opr.Y]!=2) {		//打开

		if (D.tag[opr.X][opr.Y] == 1)	//消去旗子
			D.flagnum--;

		D.tag[opr.X][opr.Y] = 2;

		if (D.map[opr.X][opr.Y] == 0) {
			for (int X = opr.X, Y = opr.Y, f = 0; f < 8; f++) {
				opr.X = X + fx[f][0];
				opr.Y = Y + fx[f][1];

				if (opr.X<1 || opr.X>D.Xlen || opr.Y<1 || opr.Y>D.Ylen)
					continue;

				if (D.tag[opr.X][opr.Y] != 2)
					move(opr,D);		//dfs
			}
		}
		
	}
	if (opr.mode == 1 && D.tag[opr.X][opr.Y]!=2) {		//插/收旗
		D.tag[opr.X][opr.Y] = (D.tag[opr.X][opr.Y] + 1) % 2;

		if (D.tag[opr.X][opr.Y] == 0)
			D.flagnum--;
		else if (D.tag[opr.X][opr.Y] == 1)
			D.flagnum++;
	}
	if (opr.mode == 2 && (D.mode==4 || D.mode==9)) {		//运行时间
		double tmp = clock();
		puts("");

		if (D.mode == 8 || D.mode == 9)
			cct_gotoxy(0, 0);

		cout << "已运行时间  " << setiosflags(ios::fixed) << setprecision(3) << ((double)(tmp - D.start_time) / CLOCKS_PER_SEC) << "秒                     " << endl;
	}
	
	if (opr.mode == 4 && D.tag[opr.X][opr.Y]==2) {		//打开过了才检查
		int tmp=0;
		OPERATION tmpopr;
		for (int i = 0; i < 8; i++)		//统计周围标记数
			tmp += (opr.X>=1 && opr.X<=D.Xlen && opr.Y>=1 && opr.Y<=D.Ylen && D.tag[opr.X + fx[i][0]][opr.Y + fx[i][1]] == 1);

		if (tmp == D.map[opr.X][opr.Y]) {	//周围标记数符合才检查
			for (int i = 0; i < 8; i++) {
				tmpopr.X = opr.X + fx[i][0];
				tmpopr.Y = opr.Y + fx[i][1];

				if (tmpopr.X<1 || tmpopr.X>D.Xlen || tmpopr.Y<1 || tmpopr.Y>D.Ylen)		//确保在界内
					continue;

				if (D.tag[tmpopr.X][tmpopr.Y] == 0) {
					tmpopr.mode = OPR_OPEN;			//检查就意味着周围不是雷的都可以打开
					move(tmpopr, D);
					tmpopr.mode = OPR_CHEK;
					move(tmpopr, D);

				}
			}
				
		}
	}
	

}


/********
返回值	0:	正常
		1:	赢了
		2:	输了
********/
int check(const GMDATA &D)
{
	int cnt = 0, rcnt = 0;

	for (int i=1; i<=D.Xlen; i++)
		for (int j = 1; j <= D.Ylen; j++) {
			if (D.tag[i][j] == 2) {
				if (D.map[i][j] < 0)
					return 2;
			}
			else {
				rcnt += (D.tag[i][j] == 1 && D.map[i][j] < 0);
				cnt++;
			}
		}

	return (cnt == D.bombnum || rcnt==D.bombnum);
}