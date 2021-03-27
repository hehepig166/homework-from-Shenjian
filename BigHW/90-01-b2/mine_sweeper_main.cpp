/* 朱泽凯 2051995 济美 */
#include "../include/cmd_console_tools.h"
#include "../90-01-b2/mine_sweeper.h"
using namespace std;


int main()
{
	do {
		cct_cls();
		cct_setfontsize("新宋体", 26); //Truetype字体只需要给出一个参数高度即可
		cct_setconsoleborder(120, 25,120,200);
	} while (PLAY(MENU()));

	return 0;
}