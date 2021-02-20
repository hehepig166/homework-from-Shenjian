/* 朱泽凯 智交 2051995 */
#include <iostream>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "cmd_console_tools.h"
#include "mine_sweeper.h"
using namespace std;

int PLAY(GMDATA gmdata)
{
	cct_cls();

	

	if (gmdata.mode == 0) {		//退出模式
		return 0;
	}
	else if (gmdata.mode >= 1 && gmdata.mode <= 4) {	//数据模式
		play_shuzu(gmdata);
	}
	else if (gmdata.mode >= 5 && gmdata.mode <= 9) {	//伪图形界面模式
		play_graph(gmdata);
		cct_gotoxy(0, 0);
	}

	puts("");
	cout << "按回车继续                           " << endl;
	for (char ccch = _getch(); ccch != '\r'; ccch = _getch())
		continue;
	return 1;
}


/* 数据模式 */
void play_shuzu(GMDATA& gmdata)
{
	OPERATION opr;

	if (gmdata.mode == 1) {
		newmap(gmdata.map, gmdata.Xlen, gmdata.Ylen, gmdata.bombnum);
		for (int i = 1; i <= gmdata.Xlen; i++)
			for (int j = 1; j <= gmdata.Ylen; j++)
				gmdata.tag[i][j] = 2;
		base_show(gmdata);
		return;
	}
	if (gmdata.mode >= 2 && gmdata.mode <= 4) {		//init map
		for (int i = 1; i <= gmdata.Xlen; i++)
			for (int j = 1; j <= gmdata.Ylen; j++)
				gmdata.tag[i][j] = gmdata.map[i][j] = 0;
		gmdata.flagnum = 0;
		
		base_show(gmdata);
		opr = get_opr(1 + (gmdata.mode == 4), gmdata);

		if (opr.mode == OPR_EXIT)
			return;

		newmap(gmdata.map, gmdata.Xlen, gmdata.Ylen, gmdata.bombnum, opr.X, opr.Y);		//新建雷图，开始计时
		gmdata.start_time =double(clock());

		move(opr, gmdata);
		base_show(gmdata);
	}
	if (gmdata.mode == 3 || gmdata.mode == 4) {
		int tmp;
		while ((tmp = check(gmdata)) == 0) {

			opr = get_opr((1 + (gmdata.mode == 4)), gmdata);

			if (opr.mode == OPR_EXIT)
				return;

			move(opr, gmdata);
			base_show(gmdata);

		}

		if (tmp == 1) {		//赢了
			cout << "赢了  " <<"用时(秒)" <<double((clock()-gmdata.start_time)/CLOCKS_PER_SEC) << endl;
		}
		else if (tmp == 2) {	//输了
			cout << "输了                   " << endl;
		}
	}
}

/* 伪图形界面模式 */
void play_graph(GMDATA& gmdata)
{
	OPERATION opr;

	if (gmdata.mode == 5 || gmdata.mode==6) {
		for (int i = 1; i <= gmdata.Xlen; i++)
			for (int j = 1; j <= gmdata.Ylen; j++)
				gmdata.tag[i][j] =2;
		newmap(gmdata.map, gmdata.Xlen, gmdata.Ylen, gmdata.bombnum);
		graph_draw_frame(gmdata);
		graph_show(gmdata);
	}
	if (gmdata.mode == 6) {
		for (opr.mode=-1; opr.mode!=OPR_OPEN; )
			get_graph_opr(gmdata, opr);
		return;
	}

	if (gmdata.mode >= 7 && gmdata.mode <= 9) {		//init map
		for (int i = 1; i <= gmdata.Xlen; i++)
			for (int j = 1; j <= gmdata.Ylen; j++)
				gmdata.tag[i][j] = gmdata.map[i][j] = 0;
		gmdata.flagnum = 0;

		graph_draw_frame(gmdata);

		for (get_graph_opr(gmdata, opr); opr.mode != OPR_OPEN; get_graph_opr(gmdata, opr)) {
			if (opr.mode == OPR_EXIT) {
				return;
			}
			if (opr.mode == OPR_TIME) {
				cct_gotoxy(0, 0);
				puts("还没开始呢           ");
				Sleep(500);
				cct_gotoxy(0, 0);
				puts("                     ");
				continue;
			}
			else {
				move(opr, gmdata);
				graph_show(gmdata);
			}
		}

		newmap(gmdata.map, gmdata.Xlen, gmdata.Ylen, gmdata.bombnum, opr.X, opr.Y);		//新建雷图，开始计时
		gmdata.start_time = double(clock());

		move(opr, gmdata);
		graph_show(gmdata);

		if (gmdata.mode == 9) {
			cct_gotoxy(0, 0);
			puts("开始计时                 ");
		}
	}
	if (gmdata.mode == 8 || gmdata.mode == 9) {
		int tmp;
		while ((tmp = check(gmdata)) == 0) {

			get_graph_opr(gmdata, opr);

			if (opr.mode == OPR_EXIT)
				return;

			move(opr, gmdata);
			graph_show(gmdata);

		}

		if (tmp == 1) {		//赢了

			cct_gotoxy(0, 0);

			cout << "赢了  " << "用时(秒)" << double((clock() - gmdata.start_time) / CLOCKS_PER_SEC) << endl;
		}
		else if (tmp == 2) {	//输了

			graph_show_bombs(gmdata);

			cct_gotoxy(0, 0);

			puts("输了                      ");
		}
	}
	
}

void base_show(const GMDATA &D)
{
	puts("");
	cout << "  |";
	for (int i = 1, j = '1'; i <= D.Ylen; i++, j++) {
		if (j == '9' + 1)
			j = 'a';
		cout << char(j) << ' ';
	}
	puts("");
	cout << "--+---";
	for (int i = 2 * D.Ylen; i; i--)
		cout << '-';
	puts("");

	for (int i = 1; i <= D.Xlen; i++) {
		cout << char(i + 'A' - 1) << " |";
		for (int j = 1; j <= D.Ylen; j++) {
			if (D.tag[i][j] == 0) {		//未开封
				cout << "X ";
			}
			else if (D.tag[i][j] == 1) {	//插旗
				cct_setcolor(COL_BG[1]);
				cout << '#';
				cout << ' ';
				cct_setcolor();
			}
			else if (D.tag[i][j] == 2) {	//显示
				if (D.map[i][j] >= 0) {
					cct_setcolor(COL_BG[2], COL_FG[D.map[i][j]]);
					cout << D.map[i][j];
					cout << ' ';
					cct_setcolor();
				}
				else {
					cct_setcolor(COL_BG[2], COLOR_HRED);
					cout << '*';
					cout << ' ';
					cct_setcolor();
				}
			}
		}
		puts("");
	}
}