/* 朱泽凯 智交 2051995 */
#include <iostream>
#include "cmd_console_tools.h"
#include "mine_sweeper.h"
using namespace std;

/****************************************************************
* 功能：	读入一个 ASCII 码属于 L~R 范围的字符（单字符），输出来
* 返回值：	读入的字符的 ASCII 码
* 注：		不包含双字符（如方向键、fx之类）
*			但包含 Esc (ASCII= 0x1b)
* ***************************************************************/

GMDATA MENU()
{

	GMDATA ret;

	cct_cls();
	puts("------------------------------------------");
	puts("1.选择难度并显示内部数组");
	puts("2.输入初始位置并显示被打开的初始区域");
	puts("3.内部数组基础版");
	puts("4.内部数组完整版（标记、运行时间）");
	puts("5.画出伪图形化框架并显示内部数据");
	puts("6.检测鼠标位置和合法性（(在合法区域)左键单击退出）");
	puts("7.鼠标选择初始位置并显示被打开的初始区域");
	puts("8.伪图形界面基础版");
	puts("9.伪图形界面完整版");
	puts("0.退出游戏");
	puts("------------------------------------------");
	cout << ("[请选择] ：");
	ret.mode = my_getch('0', '9') - '0';
	cout << ret.mode;
	if (ret.mode == 0)
		return ret;

	cct_cls();
	puts("请选择难度：");
	puts("1.初级(9 * 9 - 10颗雷)");
	puts("2.中级(16 * 16 - 40颗雷)");
	puts("3.高级(16 * 30 - 99颗雷)");
	cout << ("请输入[1..3]：");
	ret.level = my_getch('1', '3') - '0';
	cout << ret.level;


	init_gmdata(ret);

	return ret;
}

void init_gmdata(GMDATA &ret)
{
	switch (ret.level) {
		case 1:
			ret.Xlen = ret.Ylen = 9;
			ret.bombnum = 10;
			break;
		case 2:
			ret.Xlen = ret.Ylen = 16;
			ret.bombnum = 40;
			break;
		case 3:
			ret.Xlen = 16;
			ret.Ylen = 30;
			ret.bombnum = 99;
			break;
	}
}