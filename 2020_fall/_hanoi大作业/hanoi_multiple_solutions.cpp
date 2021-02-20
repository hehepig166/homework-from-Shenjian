/* 智交 2051995 朱泽凯 */
#include <iostream>
#include <cstdio>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include "cmd_console_tools.h"
#include "hanoi.h"
using namespace std;

static int st[3][MaxN + 3], top[3];
static int Time_mark;
static int CNT;




/* -----------------------------------------

	 本文件功能：
	1、放被 hanoi_main.cpp/hanoi_menu.cpp 中的各函数调用的菜单各项对应的执行函数

	 本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
	3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------- */


   /* 栈 */
int stack_pop(const int x)
{
	return top[x]>0 ? st[x][--top[x]] : -1;
}
void stack_push(const int x, const int y)
{
	st[x][top[x]++] = y;
}
int stack_top(const int x)
{
	return top[x]>0 ? st[x][top[x] - 1] : MaxN+1;
}
void stack_print(const int x)
{
	cout << char(x + 'A') << ":";
	for (int i = 0; i < top[x]; i++)
		cout << setw(3) << st[x][i];
	for (int i = top[x]; i < MaxN; i++)
		cout <<"   ";
}


/***************************************************************************
  函数名称：
  功    能：将字符 x 转化为数字
  输入参数：
  返 回 值：若为大小写字母，则返回对应的 0~25 ，否则返回 -1
  说    明：
***************************************************************************/
int alpha_to_num(const char x)
{
	if (x >= 'a' && x <= 'z')
		return (x - 'a');
	if (x >= 'A' && x <= 'Z')
		return (x - 'A');
	return -1;
}

/***************************************************************************
  函数名称：
  功    能：根据 tag 来相应延时
  输入参数：const int tag
  返 回 值：
  说    明：若tag为非负数，则根据对应 mode 和 Time_mark 来延时
			若tag为负数，则延时 -tag*10 毫秒
***************************************************************************/
void pause_(const int tag)
{
	if (tag <0)	{
		Sleep(-tag*10);
		return;
	}

	if (tag>=0 && tag <= 3)
		return;
	


	switch (Time_mark) {
		case 0:
			for (char ch = _getch(); ch != '\r'; ch = _getch())
				continue;
			break;
		case 1:
			Sleep(1000);
			break;
		case 2:
			Sleep(400);
			break;
		case 3:
			Sleep(200);
			break;
		case 4:
			Sleep(100);
			break;
		case 5:
			Sleep(10);
			break;
	}
}


/***************************************************************************
  函数名称：
  功    能：（带错误处理地）读入盘子个数
  输入参数：
  返 回 值：输入的盘子个数（0~MaxN）
  说    明：
***************************************************************************/
int input_N()
{
	int N;
	for (N = -1; N < 1 || N>MaxN; )
	{
		cout << "请输入汉诺塔的层数(1-" << MaxN << ")：" << endl;
		cin >> N;
		cin.clear();
		cin.ignore(65535, '\n');
	}
	return N;
}

/***************************************************************************
  函数名称：
  功    能：（带错误处理地）读入起始柱编号
  输入参数：
  返 回 值：起始柱的编号（0~2）
  说    明：
***************************************************************************/
int input_startz()
{
	int startz;
	char Ch;
	for (startz = Ch = -1; ; ) {
		puts("请输入起始柱(A-C)：");
		cin >> Ch;
		cin.clear();
		cin.ignore(65535, '\n');

		startz = alpha_to_num(Ch);
		if (startz >= 0 && startz < 3)
			break;
	}
	return startz;
}


/***************************************************************************
  函数名称：
  功    能：（带错误处理地）读入目标柱编号
  输入参数：
  返 回 值：目标柱的编号（0~2）
  说    明：
***************************************************************************/
int input_endz(const int startz)
{
	int endz;
	char Ch;
	for (endz = Ch = -1; ;) {
		puts("请输入目标柱(A-C)：");
		cin >> Ch;
		cin.clear();
		cin.ignore(65535, '\n');

		endz = alpha_to_num(Ch);
		if (endz >= 0 && endz < 3) {
			if (endz != startz)
				break;
			else
				cout << "目标柱(" << (char)('A' + endz) << ")不能与起始柱(" << (char)('A' + endz) << ")相同" << endl;
		}
	}
	return endz;
}


/***************************************************************************
  函数名称：
  功    能：（带错误处理地）读入Time_mark
  输入参数：
  返 回 值：Time_mark 的值
  说    明：
***************************************************************************/
int input_Time_mark()
{
	for (Time_mark = -1; Time_mark < 0 || Time_mark>5; ) {
		puts("请输入移动速度(0-5: 0-按回车单步演示 1-延时最长 5-延时最短)");
		cin >> Time_mark;
		cin.clear();
		cin.ignore(65535, '\n');
	}

	return Time_mark;
}


/***************************************************************************
  函数名称：
  功    能：游戏初始化
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void init(const int mode, const int N, const int startz, const int endz)
{
	if (mode>=4)
		cct_cls();

	cct_setcolor(COLOR_BG, COLOR_FG);

	for (int i = 0; i < 3; i++)
		top[i] = 0;

	for (int i = N; i > 0; i--)
		stack_push(startz, i);

	CNT = 0;
	if (mode != 4 && mode != 8)
		Time_mark = 5;

	if (mode == 8 || mode==9)
		draw_init(4, N, startz, endz, Y_2);		//若是图形，下面还得初始化一个数字盘

	if (mode >= 4)
		draw_init(mode,N,startz,endz,Y_1);		//初始化主体图案
	
	if (mode == 8 || mode == 4 || mode==9)		//这些情况需要显示“初始”状态，所以暂停一下
		pause_(mode);
}

/***************************************************************************
  函数名称：
  功    能：接受移动操作
  输入参数：
  返 回 值：串长度
  说    明：
***************************************************************************/
int input_opr(char s[])
{
	int len = 0,mx,my,mact,key1,key2;
	while (1) {
		cct_read_keyboard_and_mouse(mx, my, mact, key1, key2);
		if (key2 != 0)		//复合键
			continue;
		else if (key1 == '\b') {	//退格
			if (len == 0)
				continue;
			len--;
			putchar('\b'), putchar(' '), putchar('\b');
		}
		else if (len == 10 || key1 == '\r') {		//清除缓冲区
			s[len] = '\r';
			for (int i = len; i > 0; i--)
				putchar('\b'), putchar(' '), putchar('\b');
			break;
		}
		else if (key1 >= 33 && key1 < 127) {	//显式字符
			putchar(key1);
			s[len++] = key1;
		}
	}
	return len;
}


/***************************************************************************
  函数名称：
  功    能：检查移动操作
  输入参数：
  返 回 值：0	错误输入
			1	正确输入，移动盘子
			-1	正确输入，中止游戏
  说    明：
***************************************************************************/
int check_opr(const int len, const int st, const int ed)
{
	int tag=0;

	if (len == 1 && st == ('q' - 'a')) {		//退出游戏
		tag = -1;
	}
	else if (len == 2 && st >= 0 && st < 3 && ed >= 0 && ed < 3) {	//判断能否移动
		if (top[st] == 0) {		//源柱为空
			cct_gotoxy(0, Y_2 + 8);
			cout << "错误：源柱为空";
			pause_(-100);
			cct_gotoxy(0, Y_2 + 8);
			cout << "              ";
		}
		else if (stack_top(st) > stack_top(ed)) {	//大压小
			cct_gotoxy(0, Y_2 + 8);
			cout << "错误：大压小";
			pause_(-100);
			cct_gotoxy(0, Y_2 + 8);
			cout << "            ";
		}
		else if (st == ed) {	//我压我自己
			cct_gotoxy(0, Y_2 + 8);
			cout << "错误：自压";
			pause_(-100);
			cct_gotoxy(0, Y_2 + 8);
			cout << "          ";
		}
		else {	//可以移动
			tag = 1;
		}
	}

	return tag;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：mode
			   1.基本解
			   2.基本解(步数记录)
			   3.内部数组显示(横向)
			   4.内部数组显示(纵向+横向)
			   5.图形解-预备-画三个圆柱
			   6.图形解-预备-在起始柱上画n个盘子
			   7.图形解-预备-第一次移动
			   8.图形解-自动移动版本
			   9.图形解-游戏版
			   0.退出
***************************************************************************/
void play(const int mode, const int N, const int startz, const int endz)
{
	cct_setcursor(CURSOR_INVISIBLE);		//隐藏光标

	if (mode <= 0)
		return;

	/* 以下是需要进行操作的 mode */

	init(mode, N, startz, endz);

	switch (mode) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 8:
			solve(N, startz, endz, mode);
			break;
		case 7:
			move(startz, endz, mode);
			break;
		case 9:
			static int st,ed,len, mx, my, mact, key1, key2,ready_to_move;
			static char s[23];
			
			cct_gotoxy(0, Y_2 + 7);
			cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出) ：  ";

			while (1) {

				ready_to_move = 0;

				while (!ready_to_move) {	//一直要求输入操作，直到收到可以移动(>0) 或游戏中止(<0) 的操作

					cct_setcursor(CURSOR_VISIBLE_NORMAL);		//光标显示

					cct_gotoxy(60, Y_2 + 7);
					len = input_opr(s);		//读取操作
					
					cct_setcursor(CURSOR_INVISIBLE);	//光标隐藏

					st = alpha_to_num(s[0]);
					ed = alpha_to_num(s[1]);

					ready_to_move = check_opr(len, st, ed);

				}
				
				if (ready_to_move < 0) {		//游戏中止
					cct_gotoxy(0, Y_2 + 8);
					puts("游戏中止 :(");
					break;
				}
				else {
					move(st, ed, mode);
					if (top[endz] == N) {
						cct_gotoxy(0, Y_2 + 8);
						puts("游戏结束 :)");
						break;
					}
				}
			}

	}

	puts("\n按回车键键继续");
	for (char chhh = _getch(); chhh != '\r'; chhh = _getch())
		continue;

	cct_setcursor(CURSOR_VISIBLE_NORMAL);		//复原光标
}


/***************************************************************************
  函数名称：
  功    能：递归求解移动步骤
  输入参数：int n
			int from_
			int to_
			int mode
  返 回 值：
  说    明：将 from_柱 最上的 n 个盘子移到 to_柱 上
			注：已知两个不同柱子 a 和 b （0~2）
				则剩下那个柱子是 (a+b)*2%3
***************************************************************************/
void solve(const int n, const int from_, const int to_, const int mode)
{
	if (n == 0)
		return;
	int tmp = (from_ + to_) * 2 % 3;
	solve(n - 1, from_, tmp, mode);
	move(from_, to_, mode);
	solve(n - 1, tmp, to_, mode);
}



/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void move(const int from_, const int to_, const int mode)
{
	int X = stack_top(from_),hf,ht;

	hf = top[from_];		//获取移动前的高度
	stack_push(to_, stack_pop(from_));	//栈中真实的移动
	ht = top[to_];			//获取移动后的高度

	switch (mode) {
		case 1:
			draw_move(X, from_, 0, to_, 0, Y_1,1);		//基本解输出
			putchar('\n');
			//cout << setw(2) << X << "# " << char(from_ + 'A') << "---->" << char(to_ + 'A') << endl;
			break;

		case 4:
			draw_move(X,from_, hf, to_, ht, Y_1,3);		//数字盘画
			cct_gotoxy(0, Y_1 + 5);
		case 2:
		case 3:
			cout << "第" << setw(4) << (++CNT) << " 步: ";	//步数输出
			draw_move(X, from_, 0, to_, 0,  Y_1,1);			//基本解输出

			if (mode == 3 || mode==4)
				draw_move(X, from_, 0, to_, 0, Y_1, 2);		//栈输出

			putchar('\n');
			pause_(mode);
			break;

		case 7:
			draw_move(X, from_, hf, to_, ht, Y_1, 4);	//图画盘
			break;
		case 8:
		case 9:
			draw_move(X, from_, hf, to_, ht, Y_2, 3);		//修改数字盘
			cout << "第" << setw(4) << (++CNT) << " 步: ";	//步数输出
			draw_move(X, from_, 0, to_, 0, Y_2, 1);			//基本解输出
			draw_move(X, from_, 0, to_, 0, Y_2, 2);		//栈输出
			draw_move(X, from_, hf, to_, ht, Y_1, 4);	//修改画盘
			pause_(mode);
			cct_gotoxy(0, Y_2 + 5);
			break;


	}

	
}


/***************************************************************************
  函数名称：
  功    能：初始化图案
  输入参数：即为相应的参数
  返 回 值：
  说    明：
***************************************************************************/

void draw_init(const int mode, const int N, const int startz, const int endz, const int Y_)
{
	int bgc, fgc;
	cct_getcolor(bgc, fgc);

	if (mode == 4){
		cct_gotoxy(0, 0);
		cout << "从 " << char(startz + 'A') << " 移动到 " << char(endz + 'A') << "，共 " << setw(2) << N << " 层，延时设置为 " << Time_mark << endl;
		cct_gotoxy(X_4[0]-2,Y_);
		for (int i = X_4[2]-X_4[0]+5; i; i--)
			putchar('=');

		for (int i = 0; i < 3; i++) {
			cct_gotoxy(X_4[i], Y_ + 1);
			putchar(char(i + 'A'));
			for (int j = 1; j <= top[i]; j++){
				cct_gotoxy(X_4[i], Y_ - j);
				cout << (N - j + 1);
			}
		}

		cct_gotoxy(0, Y_ + 5);
		cout << "初始:                   ";
		for (int i = 0; i < 3; i++)
			stack_print(i);

	}

	if (mode >= 5) {	//画支架
		for (int i = 0; i < 3; i++)		//柱子底盘
			cct_showch(X_8[i] - MaxN - 1, Y_, ' ', COLOR_ZHU, fgc, MaxN * 2 + 3);

		for (int j = 1, k = H_ZHU; j <= k; j++) {	//柱子
			for (int i = 0; i < 3; i++) {
				cct_showch(X_8[i], Y_ - j, ' ', COLOR_ZHU, fgc, 1);
				pause_(-1);
			}
		}
	}

	if (mode >= 6) {	//初始放置 N 个盘子
		for (int j = N; j; j--) {
			draw_pan_add(j, X_8[startz], N-j+1,Y_);
			//cct_showch(X_8[startz] - j, Y_ + j - N - 1, ' ', COLOR_PAN[j], fgc, j * 2 + 1);
			pause_(-5);
		}
		pause_(-50);
	}

	cct_setcolor(bgc, fgc);
	if (mode == 5 || mode == 6 || mode == 7) {
		cct_gotoxy(0,Y_ + 5);
		return;
	}
}


/***************************************************************************
  函数名称：
  功    能：为移动画图
  输入参数：tag
				1.基本解
				2.栈
				3.数字盘
				4.图画盘
  返 回 值：
  说    明：
***************************************************************************/
void draw_move(const int X,const int from_,const int hf, const int to_, const int ht, const int Y_, const int tag)
{
	switch (tag) {
		case 1:
			cout << setw(2) << X << "# " << char(from_ + 'A') << "---->" << char(to_ + 'A');
			break;
		case 2:
			cout << "  ";
			for (int i = 0; i < 3; i++)
				stack_print(i);
			break;
		case 3:
			cct_gotoxy(X_4[from_], Y_ - hf);
			puts("    ");
			cct_gotoxy(X_4[to_], Y_ - ht);
			cout << X;
			cct_gotoxy(0, Y_ + 5);
			break;
		case 4:
			for (int i = hf, j = H_TOP-1; i <= j; i++) {	//提起
				draw_pan_del(X, X_8[from_], i, Y_,H_ZHU-i);
				//pause_(-1);
				draw_pan_add(X, X_8[from_], i + 1, Y_);
				pause_(Time_mark ? Time_mark * 4 - 23 : -3);
			}

			for (int i = X_8[from_], j = X_8[to_], k = ((j - i > 0) ? 1 : -1); i != j; i += k){		//平移
				draw_pan_del(X, i, H_TOP, Y_,H_ZHU-i);
				//pause_(-1);
				draw_pan_add(X, i + k, H_TOP, Y_);
				pause_(Time_mark?Time_mark * 4 - 21:-2);
			}

			for (int i = H_TOP ; i > ht; i--) {		//放下
				draw_pan_del(X, X_8[to_], i, Y_,H_ZHU-i);
				//pause_(-1);
				draw_pan_add(X, X_8[to_], i - 1, Y_);
				pause_(Time_mark ? Time_mark * 4 - 23 : -3);
			}
			cct_gotoxy(0, Y_ + 5);
			break;
	}
	//printf("$移动$ %d -> %d\n", from_, to_);
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void draw_pan_add(const int pan, const int x, const int h, const int Y_)
{
	int bgc, fgc;
	cct_getcolor(bgc, fgc);

	cct_showch(x - pan, Y_ -h, ' ', COLOR_PAN[pan],COLOR_FG, pan * 2 + 1);

	cct_setcolor(bgc, fgc);
}

void draw_pan_del(const int pan, const int x, const int h, const int Y_,const int tag)
{
	int bgc, fgc,tmpbg;
	cct_getcolor(bgc, fgc);

	tmpbg = (tag <0) ? COLOR_BG : COLOR_ZHU;
	cct_showch(x - pan, Y_ - h, ' ', COLOR_BG, COLOR_FG, pan * 2 + 1);
	cct_showch(x, Y_ - h, ' ', tmpbg, COLOR_FG, 1);

	cct_setcolor(bgc, fgc);
}