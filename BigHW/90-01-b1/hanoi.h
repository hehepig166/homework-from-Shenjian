/* 朱泽凯 2051995 济美 */
#pragma once

/* -----------------------------------------

	 本文件功能：
	1、为了保证 hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp 能相互访问函数的函数声明
	2、一个以上的cpp中用到的宏定义（#define）或全局只读（const）变量，个数不限
	3、可以参考 cmd_console_tools.h 的写法（认真阅读并体会）
   ----------------------------------------- */

/* 一些简化了的cct宏定义替换 */


/* 定义游戏开始界面输入提示起始行 */
#define Y_MENU_GM_ST (16)

/* 定义游戏结束界面输入提示起始行 */
#define Y_MENU_GM_ED (38)

/* 定义 N ，即盘子数量地最大值 */
const int MaxN = 10;

/* 制图相关 */
const int Y_1 = MaxN + 5;
const int Y_2 = 2 * MaxN + 8;
const int X_4[3] = { 30,40,50 };
const int X_8[3] = { (1 + 1 + MaxN) ,(1 + (MaxN * 2 + 10) + MaxN) ,(1 + (MaxN * 2 + 10) * 2 + MaxN) };
const int H_TOP = MaxN + 4;		//提起最大高度
const int H_ZHU = MaxN + 2;		//柱子高度

/* 颜色 */
#define COLOR_BG (0)
#define COLOR_FG (7)
#define COLOR_ZHU (14)
const int COLOR_PAN[20] = { 0,1,2,3,4,5,6,8,9,10,11,12,13 };

/* 一些基础函数 */
int alpha_to_num(const char x);
void pause_(const int mode);

/* 栈相关 */
int stack_pop(const int x);
void stack_push(const int x, const int y);
int stack_top(const int x);
void stack_print(const int x);

/* hanoi_menu 中的函数 */
int MENU(const int lstloop, int* N_, int* startz_, int* endz_);
int menu_game_prepare(const int mode, int* N_, int* startz_, int* endz_);

/* 用于输入的一些函数 */
int input_N();
int input_startz();
int input_endz(const int startz);
int input_Time_mark();
int input_opr(char s[]);
int check_opr(const int len, const int st, const int ed);

/* 汉诺塔具体操作流程中用到的函数 */
void init(const int mode, const int N, const int startz, const int endz);
void play(const int mode, const int N, const int startz, const int endz);
void solve(const int n, const int from_, const int to_, const int mode);
void move(const int from_, const int to_, const int mode);

/* 画汉诺塔相关函数 */
void draw_init(const int mode, const int N, const int startz, const int endz, const int Y_ = Y_1);
void draw_move(const int X, const int from_, const int hf, const int to_, const int ht,  const int Y_, const int tag );
void draw_pan_add(const int pan, const int x, const int h, const int Y_);
void draw_pan_del(const int pan, const int x, const int h, const int Y_, const int tag);