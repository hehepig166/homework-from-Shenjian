/* ÷Ï‘Ûø≠ ÷«Ωª 2051995 */
#include <iostream>
#include <conio.h>
#include <ctime>
#include <iomanip>
#include "../include/cmd_console_tools.h"
#include "../include/90-01-b2-mine_sweeper.h"
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








