/* 智交 2051995 朱泽凯 */
#include <iostream>
#include "cmd_console_tools.h"
#include "hanoi.h"
using namespace std;


/* -----------------------------------------

	 本文件功能：
	1、放被 hanoi_main.cpp/hanoi_menu.cpp 中的各函数调用的菜单各项对应的执行函数

	 本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、不允许定义静态全局变量（全局变量的使用准则是：少用、慎用、能不用尽量不用）
	3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------- */


   /***************************************************************************
	 函数名称：
	 功    能：总菜单，选择汉诺塔模式
	 输入参数：const int lst ：上一次的 loop(用于判断是否需要清屏)
	 返 回 值：-1.	未知的模式
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

	 说    明：
   ***************************************************************************/
int MENU(const int lstloop, int* N_, int* startz_, int* endz_)
{
	if (lstloop >= 0) {
		cct_cls();
		puts("---------------------------------");
		puts("1.基本解");
		puts("2.基本解(步数记录)");
		puts("3.内部数组显示(横向)");
		puts("4.内部数组显示(纵向+横向)");
		puts("5.图形解-预备-画三个圆柱");
		puts("6.图形解-预备-在起始柱上画n个盘子");
		puts("7.图形解-预备-第一次移动");
		puts("8.图形解-自动移动版本");
		puts("9.图形解-游戏版");
		puts("0.退出");
		puts("---------------------------------");
		cout << ("[请选择:] ");
	}

	int mx, my, mact, key1, key2, ret = -1;

	cct_read_keyboard_and_mouse(mx, my, mact, key1, key2);

	ret = (key2 == 0 && key1 >= '0' && key1 <= '9') ? (key1 - '0') : (-1);

	if (ret > 0)		//若是合法模式，则回显
		putchar(key1);

	menu_game_prepare(ret, N_, startz_, endz_);

	return ret;
}


/***************************************************************************
  函数名称：
  功    能：游戏开始时的输出界面显示，并调用相应的hanoi程序
  输入参数：const int mode      ：游戏模式，具体与 MENU 注释一致
			int& N_             ：盘子个数
			int& startz_		：起始柱
			int& endz_			：目标柱
  返 回 值：0.正常运作
  说    明：
***************************************************************************/
int menu_game_prepare(const int mode, int* N_, int* startz_, int* endz_)
{
	
	if (mode <= 0)
		return 0;

	/* 以下是需要进行操作的 mode */

		/* 初始化一下，以防出错 */
	*N_ = MaxN;
	*startz_ = 0;
	*endz_ = 1;

	if (mode <= 4 || mode >= 6) {		//需要输入参数的几个模式
		cct_gotoxy(0, Y_MENU_GM_ST);

		*N_ = input_N();

		*startz_ = input_startz();

		*endz_ = input_endz(*startz_);

		if (mode == 4 || mode == 8)
			input_Time_mark();
	}

	//printf("%d %d %d %d \n", N_, startz_, endz_, Time_mark);
	//char ccch = getchar();
	return 0;
}
