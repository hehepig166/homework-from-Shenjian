/* ���� �ǽ� 2051995 */
#include <iostream>
#include <conio.h>
#include <ctime>
#include <iomanip>
#include "cmd_console_tools.h"
#include "mine_sweeper.h"
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

/*********************************************************************
���ܣ�	�� *map Ϊ M[0][0]
		�����һ������ bombcnt ��ը������ X �� Y �ĵ�ͼ
		���б�֤ M[X][Y] �϶�����ը��

ע��	��Ч��ͼ�±�� 1 ��ʼ
		XYĬ�϶�Ϊ 1
		�����в��������Ե��жϣ���������������ܻ����
		ը���� M ֵΪ -1
		������ M ֵΪ ��Χը����
**********************************************************************/
void newmap(int map[][Maxlen], const int Xlen, const int Ylen, int bombcnt, const int X, const int Y)
{

	srand((unsigned int)(time(0)));

	for (int i = 0; i <= Xlen; i++)
		for (int j = 0; j <= Ylen; j++)
			map[i][j] = 0;

	for (int x, y, OK; bombcnt > 0; ) {
		x = rand() % Xlen + 1;
		y = rand() % Ylen + 1;

		OK = 1;
		for (int i = 0; i < 9 && OK; i++)
			OK = x + fx[i][0] != X && y + fx[i][1] != Y;

		if (!OK || map[x][y] < 0)
			continue;

		bombcnt--;
		map[x][y] = -1;
		for (int f = 0, xx, yy; f < 8; f++) {
			xx = x + fx[f][0];
			yy = y + fx[f][1];

			map[xx][yy] += map[xx][yy] >= 0;
		}
	}
	
	return;
#undef M
}

/*******************************
tag=1	�ڲ�����򵥰�
tag=2	�ڲ�����������
tag=3	αͼ�μ򵥰�	//Ų�Ƶ� get_graph_opr ����
tag=4	αͼ��������	//Ų�Ƶ� get_graph_opr ����

ret.mode
		0:��
		1:��/����
		2:����ʱ��
		3:�˳�
		4:���

********************************/
OPERATION get_opr(int tag,const GMDATA &D)
{
	OPERATION ret;
	if (tag == 1 || tag == 2) {		//���������
		int mx, my, mact, k1, k2;

		if (tag == 2) {
			puts("");
			puts("�������룺& - ��Ϸ������ʱ��");
			puts("          ! - ���/ȡ����ǣ����� !A1��");
			puts("          % - ��飨Ҫ��Χ��������ϲ��У�������%Ab��");
		}
		cout <<("�����루���к��У��ϸ����ִ�Сд����q/Q �˳��� ");
		while (1) {

			cct_read_keyboard_and_mouse(mx, my, mact, k1, k2=1);
			if (k2 != 0)
				continue;

			if (k1 == '&' && tag==2) {	//ʱ��
				ret.mode = OPR_TIME;
				cout << '&';
				return ret;
			}		
			else if ((k1 == '!' || k1=='%') && tag == 2) {
				ret.mode = (k1=='!' ? OPR_FLAG : OPR_CHEK);
				cout << char(k1);
				ret.X = my_getch('A', 'A' + D.Xlen - 1) - 'A' + 1;
				cout << char(ret.X + 'A'-1);
				while (cct_read_keyboard_and_mouse(mx, my, mact, k1, k2)) {
					if (k2 != 0)
						continue;
					else if (k1 >= '1' && k1 <= '9') {
						ret.Y = k1 - '0';
						cout << char(k1);
					}
					else if (k1 >= 'a' && k1 <= 'a' + D.Ylen - 10) {
						ret.Y = k1 - 'a' + 10;
						cout << char(k1);
					}
					else
						continue;
					break;
				}
				return ret;
			}
			else if (k1 == 'q' || k1 == 'Q') {
				ret.mode = OPR_EXIT;
				cout << char(k1);
				return ret;
			}
			else if (k1 >= 'A' && k1 <= D.Xlen + 'A') {
				ret.mode = OPR_OPEN;
				cout << char(k1);
				ret.X = k1-'A'+1;
				while (cct_read_keyboard_and_mouse(mx, my, mact, k1, k2)) {
					if (k2 != 0)
						continue;
					else if (k1 >= '1' && k1 <= '9') {
						ret.Y = k1 - '0';
						cout << char(k1);
					}
					else if (k1 >= 'a' && k1 <= 'a' + D.Ylen - 10) {
						ret.Y = k1 - 'a' + 10;
						cout << char(k1);
					}
					else
						continue;

					break;
				}
				return ret;
			}
		}
	}
	else {
		ret.mode = 0;
		return ret;
	}
}

/*
ret.mode
		0:��
		1:��/����
		2:����ʱ��
		3:�˳�

*/
void move(OPERATION opr, GMDATA &D)
{
	if (opr.mode == 3)
		return;


	if (opr.mode == 0 && D.tag[opr.X][opr.Y]!=2) {		//��

		if (D.tag[opr.X][opr.Y] == 1)	//��ȥ����
			D.flagnum--;

		D.tag[opr.X][opr.Y] = 2;

		if (D.map[opr.X][opr.Y] == 0) {
			for (int X = opr.X, Y = opr.Y, f = 0; f < 8; f++) {
				opr.X = X + fx[f][0];
				opr.Y = Y + fx[f][1];

				if (opr.X<1 || opr.X>D.Xlen || opr.Y<1 || opr.Y>D.Ylen)
					continue;

				if (D.tag[opr.X][opr.Y] != 2)
					move(opr,D);		//dfs
			}
		}
		
	}
	if (opr.mode == 1 && D.tag[opr.X][opr.Y]!=2) {		//��/����
		D.tag[opr.X][opr.Y] = (D.tag[opr.X][opr.Y] + 1) % 2;

		if (D.tag[opr.X][opr.Y] == 0)
			D.flagnum--;
		else if (D.tag[opr.X][opr.Y] == 1)
			D.flagnum++;
	}
	if (opr.mode == 2 && (D.mode==4 || D.mode==9)) {		//����ʱ��
		double tmp = clock();
		puts("");

		if (D.mode == 8 || D.mode == 9)
			cct_gotoxy(0, 0);

		cout << "������ʱ��  " << setiosflags(ios::fixed) << setprecision(3) << ((double)(tmp - D.start_time) / CLOCKS_PER_SEC) << "��                     " << endl;
	}
	
	if (opr.mode == 4 && D.tag[opr.X][opr.Y]==2) {		//�򿪹��˲ż��
		int tmp=0;
		OPERATION tmpopr;
		for (int i = 0; i < 8; i++)		//ͳ����Χ�����
			tmp += (opr.X>=1 && opr.X<=D.Xlen && opr.Y>=1 && opr.Y<=D.Ylen && D.tag[opr.X + fx[i][0]][opr.Y + fx[i][1]] == 1);

		if (tmp == D.map[opr.X][opr.Y]) {	//��Χ��������ϲż��
			for (int i = 0; i < 8; i++) {
				tmpopr.X = opr.X + fx[i][0];
				tmpopr.Y = opr.Y + fx[i][1];

				if (tmpopr.X<1 || tmpopr.X>D.Xlen || tmpopr.Y<1 || tmpopr.Y>D.Ylen)		//ȷ���ڽ���
					continue;

				if (D.tag[tmpopr.X][tmpopr.Y] == 0) {
					tmpopr.mode = OPR_OPEN;			//������ζ����Χ�����׵Ķ����Դ�
					move(tmpopr, D);
					tmpopr.mode = OPR_CHEK;
					move(tmpopr, D);

				}
			}
				
		}
	}
	

}


/********
����ֵ	0:	����
		1:	Ӯ��
		2:	����
********/
int check(const GMDATA &D)
{
	int cnt = 0, rcnt = 0;

	for (int i=1; i<=D.Xlen; i++)
		for (int j = 1; j <= D.Ylen; j++) {
			if (D.tag[i][j] == 2) {
				if (D.map[i][j] < 0)
					return 2;
			}
			else {
				rcnt += (D.tag[i][j] == 1 && D.map[i][j] < 0);
				cnt++;
			}
		}

	return (cnt == D.bombnum || rcnt==D.bombnum);
}