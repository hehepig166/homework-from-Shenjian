/* ���� 2051995 ���� */
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
			if (D.tag[i][j] == 0) {		//δ����
				graph_draw_num(i, j, ' ', COL_BG[0]);
			}
			else if (D.tag[i][j] == 1) {	//����
				graph_draw_num(i, j, ' ', COL_BG[1]);
			}
			else if (D.tag[i][j] == 2) {	//��ʾ
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
		cout << "������-�������" << setw(4) << D.bombnum - D.flagnum;
	}
	cct_gotoxy(0, 0);
}

void graph_draw_frame(const GMDATA& D)
{
	
	cct_cls();

	cct_setfontsize("Terminal", 16, 8);
	cct_setconsoleborder(D.Ylen*6+12, D.Xlen*3+12);

	/* ��� */
	for (int i = 1; i <= D.Ylen; i++) {
		_GTXY(-1, i*3-1);
		cout << i;
	}
	for (int i = 1; i <= D.Xlen; i++) {
		_GTXY(i * 3 -1, -1);
		cout << char(i + 'A' - 1);
	}

	/* �߿� */
	int bg, fg;
	cct_getcolor(bg, fg);

	cct_setcolor(COLOR_HWHITE,COLOR_BLACK);
	const int Mx = D.Xlen * 3, My = D.Ylen * 3;
	for (int x = D.Xlen*3; x>=0; x--) {
		for (int y = D.Ylen*3; y>=0; y--) {
			_GTXY(x, y);
			if (x == Mx && y == My)
				puts("�a");
			else if (x == Mx && y == 0)
				puts("�^");
			else if (x == 0 && y == My)
				puts("�[");
			else if (x == 0 && y == 0)
				puts("�X");
			else if (y == My && x % 3 == 0)
				puts("�g");
			else if (y == 0 && x % 3 == 0)
				puts("�d");
			else if (x == Mx && y % 3 == 0)
				puts("�m");
			else if (x == 0 && y % 3 == 0)
				puts("�j");
			else if (x % 3 == 0 && y % 3 == 0)
				puts("�p");
			else if (y % 3 == 0)
				puts("�U");
			else if (x % 3 == 0)
				puts("�T");
			
			else if (x % 3 == 1 && y % 3 == 1)
				graph_draw_num(x / 3+1, y / 3+1);
			
		}
	}
	cct_setcolor(bg,fg);
	
	
	//graph_draw_num(8, 5,'#');
	_GTXY(D.Xlen * 3 + 2, 0);
	puts("");
	puts("��ʾ0���� Esc �˳���ǰ��");
	puts("��ʾ1�����Ҽ�ͬʱ�����ټ��");
	if (D.mode==9)
		puts("��ʾ2���ո���鿴��ʱ");

	cct_gotoxy(0, 0);
}


/*****************
���ܣ�	�����Ϊ (X,Y) ������������Ϣ
ע��	��Ŵ�1��ʼ
		BGCOL
		FGCOL

��(X,Y)��
	
	���Ͻ�
		_GTXY(X*3-2,Y*3-2)
	������
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
		swap(mx, my);		//Ϊ���������xΪ�б�ţ�yΪ�б��

		if (tmp == CCT_KEYBOARD_EVENT) {
			if (k2 == 0 && k1 == 0x1b) {	//�˳�(Esc��)
				opr.mode = OPR_EXIT;
				return;
			}
			if (k2 == 0 && k1 == 0x20) {	//��ʾʱ��
				opr.mode = OPR_TIME;
				return;
			}
		}
		else if (tmp == CCT_MOUSE_EVENT) {
			_GTXY(D.Xlen * 3 + 1, 0);

			if (mx % 3 == 0 || my % 3 == 0 || mx<0 || mx>Mx || my<0 || my>My) {
				cout << "����ǰ���λ�á� �Ƿ�           ";
				continue;
			}
			else {	//�Ϸ�λ��

				//ת��Ϊ����Ϊ1�ı��
				mx = mx / 3 + 1;
				my = my / 3 + 1;
				cout << "����ǰ���λ�á� " <<"��"<< setw(2) << char(mx+'A'-1) <<"  ��" << setw(2) << my;

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
					cout << "�ƶ�          " << endl;
					//showch(X, Y, '*');����ӡ�κ�����
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//�������
					cout << "�������      " << endl;
					break;
				case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:	//˫�����
					cout << "˫�����      " << endl;
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//�����Ҽ�
					cout << "�����Ҽ�      " << endl;
					break;
				case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//˫���Ҽ�
					cout << "˫���Ҽ�      " << endl;
					break;
				case MOUSE_LEFTRIGHT_BUTTON_CLICK:		//ͬʱ�������Ҽ�
					cout << "ͬʱ�������Ҽ�" << endl;
					break;
				case MOUSE_WHEEL_CLICK:
					cout << "���¹���      " << endl;
					break;
				case MOUSE_WHEEL_MOVED_UP:
					cout << "���������ƶ�  " << endl;
					break;
				case MOUSE_WHEEL_MOVED_DOWN:
					cout << "���������ƶ�  " << endl;
					break;
				case MOUSE_NO_ACTION:
				default:
					cout << "��������" << endl;
					break;
			} //end of switch(maction)

			*/
		}
		else if (tmp == CCT_KEYBOARD_EVENT) {

		}

	}

	cct_disable_mouse();	//�������
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
			else if (D.tag[i][j] == 1) {	//������
				graph_draw_num(i, j, ' ', COLOR_RED, COLOR_RED);
			}
}