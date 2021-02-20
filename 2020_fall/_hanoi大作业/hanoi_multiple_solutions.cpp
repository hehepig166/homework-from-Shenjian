/* �ǽ� 2051995 ���� */
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

	 ���ļ����ܣ�
	1���ű� hanoi_main.cpp/hanoi_menu.cpp �еĸ��������õĲ˵������Ӧ��ִ�к���

	 ���ļ�Ҫ��
	1�����������ⲿȫ�ֱ�����const��#define�������Ʒ�Χ�ڣ�
	2�������徲̬ȫ�ֱ�����������Ҫ��������Ҫ�����ĵ���ʾ��ȫ�ֱ�����ʹ��׼���ǣ����á����á��ܲ��þ������ã�
	3����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
	4���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��

   ----------------------------------------- */


   /* ջ */
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
  �������ƣ�
  ��    �ܣ����ַ� x ת��Ϊ����
  ���������
  �� �� ֵ����Ϊ��Сд��ĸ���򷵻ض�Ӧ�� 0~25 �����򷵻� -1
  ˵    ����
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
  �������ƣ�
  ��    �ܣ����� tag ����Ӧ��ʱ
  ���������const int tag
  �� �� ֵ��
  ˵    ������tagΪ�Ǹ���������ݶ�Ӧ mode �� Time_mark ����ʱ
			��tagΪ����������ʱ -tag*10 ����
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
  �������ƣ�
  ��    �ܣ�����������أ��������Ӹ���
  ���������
  �� �� ֵ����������Ӹ�����0~MaxN��
  ˵    ����
***************************************************************************/
int input_N()
{
	int N;
	for (N = -1; N < 1 || N>MaxN; )
	{
		cout << "�����뺺ŵ���Ĳ���(1-" << MaxN << ")��" << endl;
		cin >> N;
		cin.clear();
		cin.ignore(65535, '\n');
	}
	return N;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����������أ�������ʼ�����
  ���������
  �� �� ֵ����ʼ���ı�ţ�0~2��
  ˵    ����
***************************************************************************/
int input_startz()
{
	int startz;
	char Ch;
	for (startz = Ch = -1; ; ) {
		puts("��������ʼ��(A-C)��");
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
  �������ƣ�
  ��    �ܣ�����������أ�����Ŀ�������
  ���������
  �� �� ֵ��Ŀ�����ı�ţ�0~2��
  ˵    ����
***************************************************************************/
int input_endz(const int startz)
{
	int endz;
	char Ch;
	for (endz = Ch = -1; ;) {
		puts("������Ŀ����(A-C)��");
		cin >> Ch;
		cin.clear();
		cin.ignore(65535, '\n');

		endz = alpha_to_num(Ch);
		if (endz >= 0 && endz < 3) {
			if (endz != startz)
				break;
			else
				cout << "Ŀ����(" << (char)('A' + endz) << ")��������ʼ��(" << (char)('A' + endz) << ")��ͬ" << endl;
		}
	}
	return endz;
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�����������أ�����Time_mark
  ���������
  �� �� ֵ��Time_mark ��ֵ
  ˵    ����
***************************************************************************/
int input_Time_mark()
{
	for (Time_mark = -1; Time_mark < 0 || Time_mark>5; ) {
		puts("�������ƶ��ٶ�(0-5: 0-���س�������ʾ 1-��ʱ� 5-��ʱ���)");
		cin >> Time_mark;
		cin.clear();
		cin.ignore(65535, '\n');
	}

	return Time_mark;
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ���Ϸ��ʼ��
  ���������
  �� �� ֵ��
  ˵    ����
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
		draw_init(4, N, startz, endz, Y_2);		//����ͼ�Σ����滹�ó�ʼ��һ��������

	if (mode >= 4)
		draw_init(mode,N,startz,endz,Y_1);		//��ʼ������ͼ��
	
	if (mode == 8 || mode == 4 || mode==9)		//��Щ�����Ҫ��ʾ����ʼ��״̬��������ͣһ��
		pause_(mode);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������ƶ�����
  ���������
  �� �� ֵ��������
  ˵    ����
***************************************************************************/
int input_opr(char s[])
{
	int len = 0,mx,my,mact,key1,key2;
	while (1) {
		cct_read_keyboard_and_mouse(mx, my, mact, key1, key2);
		if (key2 != 0)		//���ϼ�
			continue;
		else if (key1 == '\b') {	//�˸�
			if (len == 0)
				continue;
			len--;
			putchar('\b'), putchar(' '), putchar('\b');
		}
		else if (len == 10 || key1 == '\r') {		//���������
			s[len] = '\r';
			for (int i = len; i > 0; i--)
				putchar('\b'), putchar(' '), putchar('\b');
			break;
		}
		else if (key1 >= 33 && key1 < 127) {	//��ʽ�ַ�
			putchar(key1);
			s[len++] = key1;
		}
	}
	return len;
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ƶ�����
  ���������
  �� �� ֵ��0	��������
			1	��ȷ���룬�ƶ�����
			-1	��ȷ���룬��ֹ��Ϸ
  ˵    ����
***************************************************************************/
int check_opr(const int len, const int st, const int ed)
{
	int tag=0;

	if (len == 1 && st == ('q' - 'a')) {		//�˳���Ϸ
		tag = -1;
	}
	else if (len == 2 && st >= 0 && st < 3 && ed >= 0 && ed < 3) {	//�ж��ܷ��ƶ�
		if (top[st] == 0) {		//Դ��Ϊ��
			cct_gotoxy(0, Y_2 + 8);
			cout << "����Դ��Ϊ��";
			pause_(-100);
			cct_gotoxy(0, Y_2 + 8);
			cout << "              ";
		}
		else if (stack_top(st) > stack_top(ed)) {	//��ѹС
			cct_gotoxy(0, Y_2 + 8);
			cout << "���󣺴�ѹС";
			pause_(-100);
			cct_gotoxy(0, Y_2 + 8);
			cout << "            ";
		}
		else if (st == ed) {	//��ѹ���Լ�
			cct_gotoxy(0, Y_2 + 8);
			cout << "������ѹ";
			pause_(-100);
			cct_gotoxy(0, Y_2 + 8);
			cout << "          ";
		}
		else {	//�����ƶ�
			tag = 1;
		}
	}

	return tag;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����mode
			   1.������
			   2.������(������¼)
			   3.�ڲ�������ʾ(����)
			   4.�ڲ�������ʾ(����+����)
			   5.ͼ�ν�-Ԥ��-������Բ��
			   6.ͼ�ν�-Ԥ��-����ʼ���ϻ�n������
			   7.ͼ�ν�-Ԥ��-��һ���ƶ�
			   8.ͼ�ν�-�Զ��ƶ��汾
			   9.ͼ�ν�-��Ϸ��
			   0.�˳�
***************************************************************************/
void play(const int mode, const int N, const int startz, const int endz)
{
	cct_setcursor(CURSOR_INVISIBLE);		//���ع��

	if (mode <= 0)
		return;

	/* ��������Ҫ���в����� mode */

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
			cout << "�������ƶ�������(������ʽ��AC=A���˵������ƶ���C��Q=�˳�) ��  ";

			while (1) {

				ready_to_move = 0;

				while (!ready_to_move) {	//һֱҪ�����������ֱ���յ������ƶ�(>0) ����Ϸ��ֹ(<0) �Ĳ���

					cct_setcursor(CURSOR_VISIBLE_NORMAL);		//�����ʾ

					cct_gotoxy(60, Y_2 + 7);
					len = input_opr(s);		//��ȡ����
					
					cct_setcursor(CURSOR_INVISIBLE);	//�������

					st = alpha_to_num(s[0]);
					ed = alpha_to_num(s[1]);

					ready_to_move = check_opr(len, st, ed);

				}
				
				if (ready_to_move < 0) {		//��Ϸ��ֹ
					cct_gotoxy(0, Y_2 + 8);
					puts("��Ϸ��ֹ :(");
					break;
				}
				else {
					move(st, ed, mode);
					if (top[endz] == N) {
						cct_gotoxy(0, Y_2 + 8);
						puts("��Ϸ���� :)");
						break;
					}
				}
			}

	}

	puts("\n���س���������");
	for (char chhh = _getch(); chhh != '\r'; chhh = _getch())
		continue;

	cct_setcursor(CURSOR_VISIBLE_NORMAL);		//��ԭ���
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ��ݹ�����ƶ�����
  ���������int n
			int from_
			int to_
			int mode
  �� �� ֵ��
  ˵    ������ from_�� ���ϵ� n �������Ƶ� to_�� ��
			ע����֪������ͬ���� a �� b ��0~2��
				��ʣ���Ǹ������� (a+b)*2%3
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void move(const int from_, const int to_, const int mode)
{
	int X = stack_top(from_),hf,ht;

	hf = top[from_];		//��ȡ�ƶ�ǰ�ĸ߶�
	stack_push(to_, stack_pop(from_));	//ջ����ʵ���ƶ�
	ht = top[to_];			//��ȡ�ƶ���ĸ߶�

	switch (mode) {
		case 1:
			draw_move(X, from_, 0, to_, 0, Y_1,1);		//���������
			putchar('\n');
			//cout << setw(2) << X << "# " << char(from_ + 'A') << "---->" << char(to_ + 'A') << endl;
			break;

		case 4:
			draw_move(X,from_, hf, to_, ht, Y_1,3);		//�����̻�
			cct_gotoxy(0, Y_1 + 5);
		case 2:
		case 3:
			cout << "��" << setw(4) << (++CNT) << " ��: ";	//�������
			draw_move(X, from_, 0, to_, 0,  Y_1,1);			//���������

			if (mode == 3 || mode==4)
				draw_move(X, from_, 0, to_, 0, Y_1, 2);		//ջ���

			putchar('\n');
			pause_(mode);
			break;

		case 7:
			draw_move(X, from_, hf, to_, ht, Y_1, 4);	//ͼ����
			break;
		case 8:
		case 9:
			draw_move(X, from_, hf, to_, ht, Y_2, 3);		//�޸�������
			cout << "��" << setw(4) << (++CNT) << " ��: ";	//�������
			draw_move(X, from_, 0, to_, 0, Y_2, 1);			//���������
			draw_move(X, from_, 0, to_, 0, Y_2, 2);		//ջ���
			draw_move(X, from_, hf, to_, ht, Y_1, 4);	//�޸Ļ���
			pause_(mode);
			cct_gotoxy(0, Y_2 + 5);
			break;


	}

	
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʼ��ͼ��
  �����������Ϊ��Ӧ�Ĳ���
  �� �� ֵ��
  ˵    ����
***************************************************************************/

void draw_init(const int mode, const int N, const int startz, const int endz, const int Y_)
{
	int bgc, fgc;
	cct_getcolor(bgc, fgc);

	if (mode == 4){
		cct_gotoxy(0, 0);
		cout << "�� " << char(startz + 'A') << " �ƶ��� " << char(endz + 'A') << "���� " << setw(2) << N << " �㣬��ʱ����Ϊ " << Time_mark << endl;
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
		cout << "��ʼ:                   ";
		for (int i = 0; i < 3; i++)
			stack_print(i);

	}

	if (mode >= 5) {	//��֧��
		for (int i = 0; i < 3; i++)		//���ӵ���
			cct_showch(X_8[i] - MaxN - 1, Y_, ' ', COLOR_ZHU, fgc, MaxN * 2 + 3);

		for (int j = 1, k = H_ZHU; j <= k; j++) {	//����
			for (int i = 0; i < 3; i++) {
				cct_showch(X_8[i], Y_ - j, ' ', COLOR_ZHU, fgc, 1);
				pause_(-1);
			}
		}
	}

	if (mode >= 6) {	//��ʼ���� N ������
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
  �������ƣ�
  ��    �ܣ�Ϊ�ƶ���ͼ
  ���������tag
				1.������
				2.ջ
				3.������
				4.ͼ����
  �� �� ֵ��
  ˵    ����
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
			for (int i = hf, j = H_TOP-1; i <= j; i++) {	//����
				draw_pan_del(X, X_8[from_], i, Y_,H_ZHU-i);
				//pause_(-1);
				draw_pan_add(X, X_8[from_], i + 1, Y_);
				pause_(Time_mark ? Time_mark * 4 - 23 : -3);
			}

			for (int i = X_8[from_], j = X_8[to_], k = ((j - i > 0) ? 1 : -1); i != j; i += k){		//ƽ��
				draw_pan_del(X, i, H_TOP, Y_,H_ZHU-i);
				//pause_(-1);
				draw_pan_add(X, i + k, H_TOP, Y_);
				pause_(Time_mark?Time_mark * 4 - 21:-2);
			}

			for (int i = H_TOP ; i > ht; i--) {		//����
				draw_pan_del(X, X_8[to_], i, Y_,H_ZHU-i);
				//pause_(-1);
				draw_pan_add(X, X_8[to_], i - 1, Y_);
				pause_(Time_mark ? Time_mark * 4 - 23 : -3);
			}
			cct_gotoxy(0, Y_ + 5);
			break;
	}
	//printf("$�ƶ�$ %d -> %d\n", from_, to_);
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
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