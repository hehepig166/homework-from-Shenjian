/* ���� 2051995 ���� */
#pragma once

/* -----------------------------------------

	 ���ļ����ܣ�
	1��Ϊ�˱�֤ hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp ���໥���ʺ����ĺ�������
	2��һ�����ϵ�cpp���õ��ĺ궨�壨#define����ȫ��ֻ����const����������������
	3�����Բο� cmd_console_tools.h ��д���������Ķ�����ᣩ
   ----------------------------------------- */

/* һЩ���˵�cct�궨���滻 */


/* ������Ϸ��ʼ����������ʾ��ʼ�� */
#define Y_MENU_GM_ST (16)

/* ������Ϸ��������������ʾ��ʼ�� */
#define Y_MENU_GM_ED (38)

/* ���� N �����������������ֵ */
const int MaxN = 10;

/* ��ͼ��� */
const int Y_1 = MaxN + 5;
const int Y_2 = 2 * MaxN + 8;
const int X_4[3] = { 30,40,50 };
const int X_8[3] = { (1 + 1 + MaxN) ,(1 + (MaxN * 2 + 10) + MaxN) ,(1 + (MaxN * 2 + 10) * 2 + MaxN) };
const int H_TOP = MaxN + 4;		//�������߶�
const int H_ZHU = MaxN + 2;		//���Ӹ߶�

/* ��ɫ */
#define COLOR_BG (0)
#define COLOR_FG (7)
#define COLOR_ZHU (14)
const int COLOR_PAN[20] = { 0,1,2,3,4,5,6,8,9,10,11,12,13 };

/* һЩ�������� */
int alpha_to_num(const char x);
void pause_(const int mode);

/* ջ��� */
int stack_pop(const int x);
void stack_push(const int x, const int y);
int stack_top(const int x);
void stack_print(const int x);

/* hanoi_menu �еĺ��� */
int MENU(const int lstloop, int* N_, int* startz_, int* endz_);
int menu_game_prepare(const int mode, int* N_, int* startz_, int* endz_);

/* ���������һЩ���� */
int input_N();
int input_startz();
int input_endz(const int startz);
int input_Time_mark();
int input_opr(char s[]);
int check_opr(const int len, const int st, const int ed);

/* ��ŵ����������������õ��ĺ��� */
void init(const int mode, const int N, const int startz, const int endz);
void play(const int mode, const int N, const int startz, const int endz);
void solve(const int n, const int from_, const int to_, const int mode);
void move(const int from_, const int to_, const int mode);

/* ����ŵ����غ��� */
void draw_init(const int mode, const int N, const int startz, const int endz, const int Y_ = Y_1);
void draw_move(const int X, const int from_, const int hf, const int to_, const int ht,  const int Y_, const int tag );
void draw_pan_add(const int pan, const int x, const int h, const int Y_);
void draw_pan_del(const int pan, const int x, const int h, const int Y_, const int tag);