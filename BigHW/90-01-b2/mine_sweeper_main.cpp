/* ���� 2051995 ���� */
#include "../include/cmd_console_tools.h"
#include "../90-01-b2/mine_sweeper.h"
using namespace std;


int main()
{
	do {
		cct_cls();
		cct_setfontsize("������", 26); //Truetype����ֻ��Ҫ����һ�������߶ȼ���
		cct_setconsoleborder(120, 25,120,200);
	} while (PLAY(MENU()));

	return 0;
}