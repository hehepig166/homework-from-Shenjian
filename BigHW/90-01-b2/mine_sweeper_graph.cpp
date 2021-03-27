/* 朱泽凯 2051995 济美 */
#include <iostream>
#include <iomanip>
#include "../include/cmd_console_tools.h"
#include "../90-01-b2/mine_sweeper.h"
using namespace std;

#define _GTXY(x,y) cct_gotoxy((y)*2+2,x+4)

void graph_show(const GMDATA &D)
{
	for (int i = 1; i <= D.Xlen; i++)
		for (int j = 1; j <= D.Ylen; j++) {
			if (D.tag[i][j] == 0) {		//未开封
				graph_draw_num(i, j, ' ', COL_BG[0]);
			}
			else if (D.tag[i][j] == 1) {	//插旗
				graph_draw_num(i, j, ' ', COL_BG[1]);
			}
			else if (D.tag[i][j] == 2) {	//显示
				if (D.map[i][j] == 0)
					graph_draw_num(i, j, ' ', COL_BG[2]);
				else if (D.map[i][j] > 0)
					graph_draw_num(i, j, char(D.map[i][j] + '0'), COL_BG[2], COL_FG[D.map[i][j]]);
				else if (D.map[i][j] < 0)
					graph_draw_num(i, j, '*', COL_BG[2], COLOR_RED);
			}
		}
	if (D.mode == 8 || D.mode == 9) {
		cct_gotoxy(0, 1);
		cout << "总雷数-标记数：" << setw(4) << D.bombnum - D.flagnum;
	}
	cct_gotoxy(0, 0);
}

void graph_draw_frame(const GMDATA& D)
{
	
	cct_cls();

	cct_setfontsize("Terminal", 16, 8);
	cct_setconsoleborder(D.Ylen*6+12, D.Xlen*3+12);

	/* 标尺 */
	for (int i = 1; i <= D.Ylen; i++) {
		_GTXY(-1, i*3-1);
		cout << i;
	}
	for (int i = 1; i <= D.Xlen; i++) {
		_GTXY(i * 3 -1, -1);
		cout << char(i + 'A' - 1);
	}

	/* 边框 */
	int bg, fg;
	cct_getcolor(bg, fg);

	cct_setcolor(COLOR_HWHITE,COLOR_BLACK);
	const int Mx = D.Xlen * 3, My = D.Ylen * 3;
	for (int x = D.Xlen*3; x>=0; x--) {
		for (int y = D.Ylen*3; y>=0; y--) {
			_GTXY(x, y);
			if (x == Mx && y == My)
				puts("╝");
			else if (x == Mx && y == 0)
				puts("╚");
			else if (x == 0 && y == My)
				puts("╗");
			else if (x == 0 && y == 0)
				puts("╔");
			else if (y == My && x % 3 == 0)
				puts("╣");
			else if (y == 0 && x % 3 == 0)
				puts("╠");
			else if (x == Mx && y % 3 == 0)
				puts("╩");
			else if (x == 0 && y % 3 == 0)
				puts("╦");
			else if (x % 3 == 0 && y % 3 == 0)
				puts("╬");
			else if (y % 3 == 0)
				puts("║");
			else if (x % 3 == 0)
				puts("═");
			
			else if (x % 3 == 1 && y % 3 == 1)
				graph_draw_num(x / 3+1, y / 3+1);
			
		}
	}
	cct_setcolor(bg,fg);
	
	
	//graph_draw_num(8, 5,'#');
	_GTXY(D.Xlen * 3 + 2, 0);
	puts("");
	puts("提示0：按 Esc 退出当前局");
	puts("提示1：左右键同时按快速检查");
	if (D.mode==9)
		puts("提示2：空格键查看计时");

	cct_gotoxy(0, 0);
}


/*****************
功能：	给编号为 (X,Y) 的雷区画上信息
注：	编号从1开始
		BGCOL
		FGCOL

点(X,Y)的
	
	左上角
		_GTXY(X*3-2,Y*3-2)
	数字区
		_GTXY(X*3-1,Y*3-1)
*****************/
void graph_draw_num(const int X, const int Y, const char C, const int BGCOL, const int FGCOL)
{
	int bg, fg;
	cct_getcolor(bg, fg);

	cct_setcolor(BGCOL, FGCOL);

	_GTXY(X*3-2,Y*3-2);
	puts("    ");
	_GTXY(X * 3 - 1, Y * 3 - 2);
	cout << "  " << C << " ";

	cct_setcolor(bg,fg);
}

/**********************************
emmm

***********************************/
void get_graph_opr(const GMDATA &D,OPERATION &opr)
{
	cct_enable_mouse();

	int mx, my, mact, k1, k2, tmp;
	const int Mx = D.Xlen * 3, My = D.Ylen * 3;

	while (1) {
		tmp = cct_read_keyboard_and_mouse(mx, my, mact, k1, k2=1);
		mx = mx / 2 - 1;
		my = my  - 4;
		swap(mx, my);		//为了配合数组x为行编号，y为列编号

		if (tmp == CCT_KEYBOARD_EVENT) {
			if (k2 == 0 && k1 == 0x1b) {	//退出(Esc键)
				opr.mode = OPR_EXIT;
				return;
			}
			if (k2 == 0 && k1 == 0x20) {	//显示时间
				opr.mode = OPR_TIME;
				return;
			}
		}
		else if (tmp == CCT_MOUSE_EVENT) {
			_GTXY(D.Xlen * 3 + 1, 0);

			if (mx % 3 == 0 || my % 3 == 0 || mx<0 || mx>Mx || my<0 || my>My) {
				cout << "【当前光标位置】 非法           ";
				continue;
			}
			else {	//合法位置

				//转化为首项为1的编号
				mx = mx / 3 + 1;
				my = my / 3 + 1;
				cout << "【当前光标位置】 " <<"行"<< setw(2) << char(mx+'A'-1) <<"  列" << setw(2) << my;

				if (mact == MOUSE_LEFT_BUTTON_CLICK || mact == MOUSE_RIGHT_BUTTON_CLICK || mact == MOUSE_LEFTRIGHT_BUTTON_CLICK) {
					opr.X = mx;
					opr.Y = my;
					if (mact == MOUSE_LEFTRIGHT_BUTTON_CLICK)
						opr.mode = OPR_CHEK;
					else if (mact == MOUSE_LEFT_BUTTON_CLICK)
						opr.mode = OPR_OPEN;
					else if (mact == MOUSE_RIGHT_BUTTON_CLICK)
						opr.mode = OPR_FLAG;

					return;
				}
			}

			/*
			switch (mact) {
				case MOUSE_ONLY_MOVED:
					cout << "移动          " << endl;
					//showch(X, Y, '*');不打印任何内容
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					cout << "按下左键      " << endl;
					break;
				case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:	//双击左键
					cout << "双击左键      " << endl;
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					cout << "按下右键      " << endl;
					break;
				case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//双击右键
					cout << "双击右键      " << endl;
					break;
				case MOUSE_LEFTRIGHT_BUTTON_CLICK:		//同时按下左右键
					cout << "同时按下左右键" << endl;
					break;
				case MOUSE_WHEEL_CLICK:
					cout << "按下滚轮      " << endl;
					break;
				case MOUSE_WHEEL_MOVED_UP:
					cout << "滚轮向上移动  " << endl;
					break;
				case MOUSE_WHEEL_MOVED_DOWN:
					cout << "滚轮向下移动  " << endl;
					break;
				case MOUSE_NO_ACTION:
				default:
					cout << "其它操作" << endl;
					break;
			} //end of switch(maction)

			*/
		}
		else if (tmp == CCT_KEYBOARD_EVENT) {

		}

	}

	cct_disable_mouse();	//禁用鼠标
}

void graph_show_bombs(const GMDATA& D)
{
	for (int i=1; i<=D.Xlen; i++)
		for (int j=1; j<=D.Ylen; j++)
			if (D.map[i][j] < 0) {
				//graph_draw_num(i, j, '*', COLOR_BLACK, COLOR_RED);
				
				if (D.tag[i][j] == 1)
					graph_draw_num(i, j, '*', COLOR_GREEN, COLOR_RED);
				else
					graph_draw_num(i, j, '*', COLOR_BLACK, COLOR_RED);
				
			}
			else if (D.tag[i][j] == 1) {	//标错的雷
				graph_draw_num(i, j, ' ', COLOR_RED, COLOR_RED);
			}
}