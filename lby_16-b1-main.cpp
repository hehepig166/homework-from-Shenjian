/* 2053642 ��18 ��� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "16-b1-TString.h"
#include "16-b1-TStringAdv.h"
using namespace std;

#if defined(__linux) || defined(__linux__)
#include <sys/time.h>
/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void wait_for_enter()
{
	cout << endl << "���س�������";
	while (getchar() != '\n')
		;
	cout << endl << endl;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
long get_cur_msec(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

#else
#include <Windows.h>
#include <conio.h>
/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void wait_for_enter()
{
	cout << endl << "���س�������";
	while (_getch() != '\r')
		;
	cout << endl << endl;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
unsigned long long get_cur_msec(void)
{
	return GetTickCount(); //ȡ��ǰϵͳʱ�䣬��λ����
}

#endif

char* big_buffer(int len) {
	char* ret = new(nothrow) char[1 + len];
	if (!ret) {
		cerr << "big_buffer(int): bad alloc" << endl;
		return NULL;
	}
	for (int i = 0; i < len; i++)
		ret[i] = rand() % 94 + 33;
	ret[len] = '\0';
	return ret;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main()
{
	srand((unsigned)time(NULL));
	int ts_passed;
	while (1) {
		cout << "�Ƿ�����TString�Ĳ��Բ���? (0:��,1:��):";
		cin >> ts_passed;
		if (!cin || (ts_passed < 0 || ts_passed > 1)) {
			cin.clear();
			cin.ignore(1 << 12, '\n');
			continue;
		}
		break;
	}

	if (ts_passed == 0) {
		const TStringAdv s1;                 //s1ΪNULL
		const TStringAdv s2("teststr2");      //s2Ϊ"teststr2"
		const TStringAdv s3 = "teststr3";       //s3Ϊ"teststr3"
		const TStringAdv s4 = NULL;            //s4ΪNULL
		const TStringAdv s5 = "";              //s5ΪNULL

		cout << "������󲢳�ʼ������(NULL���ַ���������ʼ��)" << endl;

		cout << "s1Ӧ����<NULL>��  ʵ�������" << s1 << endl;
		cout << "s2Ӧ����teststr2��ʵ�������" << s2 << endl;
		cout << "s3Ӧ����teststr3��ʵ�������" << s3 << endl;
		cout << "s4Ӧ����<NULL>��  ʵ�������" << s4 << endl;
		cout << "s5Ӧ����<NULL>��  ʵ�������" << s5 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		const TStringAdv s0 = "teststr1";
		const char* str1 = "teststr2";
		const char  str2[] = "teststr3";
		const char* str3 = NULL;
		const char  str4[] = "";
		const TStringAdv s1 = s0, s2 = str1, s3 = str2, s4 = str3, s5 = str4;

		cout << "������󲢳�ʼ������(TString�����ַ�ָ�롢�ַ�����)" << endl;

		cout << "s1Ӧ����teststr1��ʵ�������" << s1 << endl;
		cout << "s2Ӧ����teststr2��ʵ�������" << s2 << endl;
		cout << "s3Ӧ����teststr3��ʵ�������" << s3 << endl;
		cout << "s4Ӧ����<NULL>��  ʵ�������" << s4 << endl;
		cout << "s5Ӧ����<NULL>��  ʵ�������" << s5 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1;

		cout << "����cin����" << endl;

		cout << "���ڼ���������Hello" << endl;
		cin >> s1;
		cout << "s1Ӧ����Hello��ʵ�������" << s1 << endl;

		cout << "���ڼ���������Hello 123" << endl;
		cin >> s1;
		cout << "s1Ӧ����Hello��ʵ�������" << s1 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("Hello"), s2;

		cout << "��ֵ��������" << endl;

		cout << "s1Ӧ����Hello�� ʵ�������" << s1 << endl;

		s2 = s1;
		cout << "s2Ӧ����Hello�� ʵ�������" << s2 << endl;

		s1 = "Hi";
		cout << "s1Ӧ����Hi��    ʵ�������" << s1 << endl;

		s2 = "";
		cout << "s2Ӧ����<NULL>��ʵ�������" << s2 << endl;

		s1 = NULL;
		cout << "s1Ӧ����<NULL>��ʵ�������" << s1 << endl;

		s1 = "Hello";
		cout << "s1Ӧ����Hello�� ʵ�������" << (s1 = s1) << endl;
		cout << "s1Ӧ����Hello�� ʵ�������" << (s1 = s1 = s1 = s1 = s1) << endl;
		cout << "s1Ӧ����Hello�� ʵ�������" << (((s1 = s1) = s1) = s1) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s0, s1("tong"), s2("ji"), s3;

		cout << "����(+)����(����TString��)" << endl;

		s3 = s1 + s2;
		cout << "s3ӦΪtongji��ʵ�������" << s3 << endl;

		s3 = s2 + s1;
		cout << "s3ӦΪjitong��ʵ�������" << s3 << endl;

		s3 = s1 + s0;
		cout << "s3ӦΪtong��  ʵ�������" << s3 << endl;

		s3 = s0 + s2;
		cout << "s3ӦΪji��    ʵ�������" << s3 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("tong"), s2, s3;

		cout << "����(+)����(TString����ַ�������)" << endl;

		s3 = s1 + "ji";
		cout << "s3ӦΪtongji��ʵ�������" << s3 << endl;

		s3 = "ji" + s1;
		cout << "s3ӦΪjitong��ʵ�������" << s3 << endl;

		s3 = s1 + "";
		cout << "s3ӦΪtong��  ʵ�������" << s3 << endl;

		s3 = s1 + (char*)NULL;
		cout << "s3ӦΪtong��  ʵ�������" << s3 << endl;

		s3 = "" + s1;
		cout << "s3ӦΪtong��  ʵ�������" << s3 << endl;

		s3 = (char*)NULL + s1;
		cout << "s3ӦΪtong��  ʵ�������" << s3 << endl;

		s3 = s2 + (char*)NULL;
		cout << "s3ӦΪ<NULL>��ʵ�������" << s3 << endl;

		s3 = (char*)NULL + s2;
		cout << "s3ӦΪ<NULL>��ʵ�������" << s3 << endl;

		s3 = s2 + "";
		cout << "s3ӦΪ<NULL>��ʵ�������" << s3 << endl;

		s3 = "" + s2;
		cout << "s3ӦΪ<NULL>��ʵ�������" << s3 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("tong"), s3;
		const char* str1 = "ji", * str2 = "", * str3 = NULL;

		cout << "����(+)����(TString����ַ�ָ��)" << endl;

		s3 = s1 + str1;
		cout << "s3ӦΪtongji��ʵ�������" << s3 << endl;

		s3 = str1 + s1;
		cout << "s3ӦΪjitong��ʵ�������" << s3 << endl;

		s3 = s1 + str2;
		cout << "s3ӦΪtong��  ʵ�������" << s3 << endl;

		s3 = str2 + s1;
		cout << "s3ӦΪtong��  ʵ�������" << s3 << endl;

		s3 = s1 + str3;
		cout << "s3ӦΪtong��  ʵ�������" << s3 << endl;

		s3 = str3 + s1;
		cout << "s3ӦΪtong��  ʵ�������" << s3 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("tong"), s3;
		char str1[] = "ji", str2[] = "";

		cout << "����(+)����(TString����ַ�����)" << endl;

		s3 = s1 + str1;
		cout << "s3ӦΪtongji��ʵ�������" << s3 << endl;

		s3 = str1 + s1;
		cout << "s3ӦΪjitong��ʵ�������" << s3 << endl;

		s3 = s1 + str2;
		cout << "s3ӦΪtong��  ʵ�������" << s3 << endl;

		s3 = str2 + s1;
		cout << "s3ӦΪtong��  ʵ�������" << s3 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("Hello"), s3;
		char c = '!';

		cout << "����(+)����(TString����ַ�)" << endl;

		s3 = s1 + c;
		cout << "s3ӦΪHello!��ʵ�������" << s3 << endl;

		s3 = c + s1;
		cout << "s3ӦΪ!Hello��ʵ�������" << s3 << endl;

		s1 = "ello";
		cout << "s3ӦΪHello!��ʵ�������" << 'H' + s1 + c << endl;

		s1 = "hina";
		cout << "s3ӦΪChina!��ʵ�������" << (s1 = 'C' + s1 + c) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		const TStringAdv s1("��"), s3 = "��", s5 = "Ҳ";
		TStringAdv s;
		const char* str2 = "��", str4[] = "��";
		const char c = '!';

		cout << "����(+)�ۺϲ���(TString���TString�ࡢ�ַ�ָ�롢�ַ����顢�ַ�)" << endl;
		s = s1 + str2 + s3 + str4 + s5 + c;
		cout << "sӦ���ǿ�������Ҳ!��ʵ�������" << s << endl;

		cout << "sӦ����Ҳ��������!��ʵ�������" << (s = s5 + s1 + str2 + s3 + str4[0] + str4[1] + c) << endl;
		cout << "sӦ������Ҳ������!��ʵ�������" << (s = str4 + s5 + s1.c_str() + str2 + s3 + c) << endl;
		cout << "sӦ��������Ҳ����!��ʵ�������" << (s = s3 + str4 + s5 + s1 + str2[0] + &str2[1] + c) << endl;
		cout << "sӦ����������Ҳ��!��ʵ�������" << (s = str2 + s3 + str4 + s5.c_str() + s1.c_str() + c) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("tong"), s2 = "ji";
		const char* str3 = "tong", str4[] = "ji";

		cout << "������(+=)�򵥲���(TString���TString�ࡢ�ַ�ָ�롢�ַ����顢�ַ�)" << endl;

		s1 += s2;
		cout << "s1ӦΪtongji��ʵ�������" << s1 << endl;

		s1 = "tong";
		s2 += s1;
		cout << "s2ӦΪjitong��ʵ�������" << s2 << endl;

		s1 = "tong";
		s1 += "ji";
		cout << "s1ӦΪtongji��ʵ�������" << s1 << endl;

		s1 = "ji";
		s1 += str3;
		cout << "s1ӦΪjitong��ʵ�������" << s1 << endl;

		s1 = "tong";
		s1 += str4;
		cout << "s1ӦΪtongji��ʵ�������" << s1 << endl;

		s1 = "tong";
		s1 += 'j';
		s1 += 'i';
		cout << "s1ӦΪtongji��ʵ�������" << s1 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("��"), s3 = "��", s5 = "Ҳ";
		TStringAdv s;
		const char* str2 = "��", str4[] = "��";
		const char c = '!';

		cout << "������(+=)�ۺϲ���(TString���TString�ࡢ�ַ�ָ�롢�ַ����顢�ַ�)" << endl;

		s = "1.";
		s += s1 + str2 + s3 + str4 + s5 + c;
		cout << "sӦ����1.��������Ҳ!��ʵ�������" << s << endl;

		s = "2.";
		cout << "sӦ����2.Ҳ��������!��ʵ�������" << (s += s5 + s1 + str2 + s3 + str4[0] + str4[1] + c) << endl;

		s = NULL;
		cout << "sӦ����3.��Ҳ������!��ʵ�������" << ((s = "3.") += str4 + s5 + s1.c_str() + str2 + s3 + c) << endl;

		s = NULL;
		cout << "sӦ����4.����Ҳ����!��ʵ�������" << (((s += "4." + s3) += (str4 + s5)) += (((s1 += str2[0]) += &str2[1]) += c)) << endl;

		s = NULL;
		s += '5';
		s += '.';
		s1 = "��";
		cout << "sӦ����5.������Ҳ��!��ʵ�������" << ((((((s += str2) += s3) += str4) += s5.c_str()) += s1.c_str()) += c) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("This is a pencil"), s2("is"), s3;

		cout << "ɾ��(-)����(TString���TString��)" << endl;

		cout << "s1ӦΪTh is a pencil��ʵ�������" << (s1 = s1 - s2) << endl;
		s3 = s1 - s2;
		cout << "s3ӦΪTh  a pencil��  ʵ�������" << s3 << endl;

		s1 = "This is a pencil";
		cout << "s1ӦΪTh  a pencil��  ʵ�������" << s1 - s2 - s2 - s2 - s2 - s2 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("This is a pencil"), s2;
		const char* str1 = "is";
		const char str2[] = "is";

		cout << "ɾ��(-)����(TString���charָ�뼰����)" << endl;

		cout << "s1ӦΪTh is a pencil��ʵ�������" << (s1 = s1 - str1) << endl;
		s2 = s1 - str1;
		cout << "s2ӦΪTh  a pencil��  ʵ�������" << s2 << endl;

		s1 = "This is a pencil";
		cout << "s1ӦΪTh is a pencil��ʵ�������" << (s1 = s1 - str2) << endl;
		s2 = s1 - str2;
		cout << "s2ӦΪTh  a pencil��  ʵ�������" << s2 << endl;

		s1 = "This is a pencil";
		cout << "s1ӦΪTh  a pencil��  ʵ�������" << s1 - str1 - str1 - str1 - str1 - str1 << endl;

		s1 = "This is a pencil";
		cout << "s1ӦΪTh  a pencil��  ʵ�������" << s1 - str2 - str2 - str2 - str2 - str2 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("This is a pencil"), s2;
		char c1 = 'i';

		cout << "ɾ��(-)����(TString���char�ַ�)" << endl;

		cout << "s1ӦΪThs is a pencil��ʵ�������" << (s1 = s1 - c1) << endl;
		s2 = s1 - c1;
		cout << "s2ӦΪThs s a pencil�� ʵ�������" << s2 << endl;

		s1 = "This is a pencil";
		cout << "s2ӦΪThs s a pencl��  ʵ�������" << (s1 - c1 - c1 - c1 - c1 - c1 - c1 - c1) << endl;

		s1 = "This is a pencil";
		cout << "s1ӦΪThi is a pencil��ʵ�������" << (s1 = s1 - 's') << endl;
		s2 = s1 - 's';
		cout << "s2ӦΪThi i a pencil�� ʵ�������" << s2 << endl;

		s1 = "This is a pencil";
		cout << "s2ӦΪThi i a pencil�� ʵ�������" << (s1 - 's' - 's' - 's' - 's' - 's' - 's') << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1;
		const TStringAdv s2 = "is";
		const char* str3 = "pen", str4[] = "a";
		const char c = ' ';

		cout << "��ɾ��(-=)�򵥲���(TString���TString�ࡢ�ַ�ָ�롢�ַ����顢�ַ�)" << endl;

		s1 = "This is a red pencil";
		cout << "s1ӦΪTh is a red pencil�� ʵ�������" << (s1 -= s2) << endl;

		s1 = "This is a red pencil";
		cout << "s1ӦΪThis is a red cil��  ʵ�������" << (s1 -= str3) << endl;

		s1 = "This is a red pencil";
		cout << "s1ӦΪThis is  red pencil��ʵ�������" << (s1 -= str4) << endl;

		s1 = "This is a red pencil";
		cout << "s1ӦΪThisis a red pencil��ʵ�������" << (s1 -= c) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1;
		const TStringAdv s2 = "is";
		const char* str3 = "pen", str4[] = "a";
		const char c = ' ';

		cout << "��ɾ��(-=)�ۺϲ���(TString���TString�ࡢ�ַ�ָ�롢�ַ����顢�ַ�)" << endl;

		s1 = "This is a red pencil";
		cout << "s1ӦΪTh  a red pencil��   ʵ�������" << ((s1 -= s2) -= s2) << endl;

		s1 = "This is a red pencil";
		cout << "s1ӦΪThis is  red cil��   ʵ�������" << ((s1 -= str3) -= str4) << endl;

		s1 = "This is a red pencil";
		cout << "s1ӦΪThisisaredpencil��   ʵ�������" << ((((s1 -= c) -= c) -= c) -= c) << endl;

		s1 = "This is a red pencil";
		cout << "s1ӦΪTh��                 ʵ�������" << ((((((((((((s1 -= s2) -= s2) -= str3) -= str4) -= c) -= c) -= c) -= c) -= "red") -= 'c') -= 'i') -= 'l') << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("Hello"), s2;

		cout << "����(*)����(TString���int)" << endl;

		s2 = s1 * 2;
		cout << "s2ӦΪHelloHello��     ʵ�������" << s2 << endl;
		cout << "s1ӦΪHelloHelloHello��ʵ�������" << (s1 = s1 * 3) << endl;

		TStringAdv s3;
		cout << "s3ӦΪ<NULL>��         ʵ�������" << (s3 * 5) << endl;
		cout << "s3ӦΪ<NULL>��         ʵ�������" << (s3 = s3 * 5) << endl;

		int i, n = 100;
		s1 = "Hello";
		s2 = s1 * n;
		for (i = 1; i < n; i++)
			s2 = s2 - s1;
		cout << "s2ӦΪHello��          ʵ�������" << s2 << endl;

		s1 = "Hello";
		s2 = s1 * n;
		for (i = 0; i < n; i++)
			s2 = s2 - 'l';
		for (i = 0; i < n; i++)
			s2 = s2 - 'l';
		for (i = 0; i < n; i++)
			s2 = s2 - 'o';
		for (i = 0; i < n; i++)
			s2 = s2 - 'H';
		for (i = 1; i < n; i++)  //ע�⣬ֻ�����ѭ���Ǵ�1��ʼ������1��
			s2 = s2 - 'e';
		cout << "s2ӦΪe��              ʵ�������" << s2 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1;

		cout << "�Ը���(*=)����(TString���int)" << endl;

		s1 = "Hello";
		cout << "s1ӦΪHelloHello��                       ʵ�������" << (s1 *= 2) << endl;
		s1 += '!';
		cout << "s1ӦΪHelloHello!HelloHello!HelloHello!��ʵ�������" << (s1 *= 3) << endl;

		s1 = "Hello";
		cout << "s1ӦΪHelloHello!HelloHello!HelloHello!��ʵ�������" << (((s1 *= 2) += '!') *= 3) << endl;

		//		�ر���ԣ����������е�ע�ͣ������������ᱨ�����������˵�����������⣬������Ԥ�ڵ����壨Ϊʲô����ϸ��!!!��
		//		cout << "s1ӦΪHelloHello!HelloHello!HelloHello!��ʵ�������" << (((s1 *= 2) + '!') *= 3) << endl;

		s1 = NULL;
		cout << "s1ӦΪ<NULL>��                           ʵ�������" << (s1 *= 100) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1;

		cout << "������(+=)/��ɾ��(-=)/�Ը���(*=)�ۺϲ���" << endl;

		s1 = "Hello";
		cout << "s1ӦΪHello!Hello!Hello!��ʵ�������" << ((s1 += '!') *= 3) << endl;

		s1 = "Hello";
		cout << "s1ӦΪHeo!Heo!Heo!Heo!��  ʵ�������" << (((s1 += '!') -= "ll") *= 4) << endl;

		s1 = "Hello";
		cout << "s1ӦΪHellHello!��        ʵ�������" << (((s1 *= 2) -= 'o') += '!') << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("This is a pencil");

		cout << "��ת(!)����" << endl;

		cout << " s1ӦΪThis is a pencil��ʵ�������" << s1 << endl;
		cout << "!s1ӦΪlicnep a si sihT��ʵ�������" << !s1 << endl;
		cout << " s1��ΪThis is a pencil��ʵ�������" << s1 << endl;

		s1 = NULL;
		cout << " s1ӦΪ<NULL>��          ʵ�������" << s1 << endl;
		cout << "!s1ӦΪ<NULL>��          ʵ�������" << !s1 << endl;
		cout << " s1��Ϊ<NULL>��          ʵ�������" << s1 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "house", s2 = "horse", s3 = "house", s4 = "", s5 = NULL;

		cout << "�Ƚ��������(����TString��)" << endl;

		cout << "��1=" << s1 << " ��2=" << s2 << "  > ���ӦΪ1��ʵ�ʣ�" << (s1 > s2) << endl;
		cout << "��1=" << s1 << " ��2=" << s2 << "  >=���ӦΪ1��ʵ�ʣ�" << (s1 >= s2) << endl;
		cout << "��1=" << s1 << " ��2=" << s2 << "  < ���ӦΪ0��ʵ�ʣ�" << (s1 < s2) << endl;
		cout << "��1=" << s1 << " ��2=" << s2 << "  <=���ӦΪ0��ʵ�ʣ�" << (s1 <= s2) << endl;
		cout << "��1=" << s1 << " ��2=" << s2 << "  ==���ӦΪ0��ʵ�ʣ�" << (s1 == s2) << endl;
		cout << "��1=" << s1 << " ��2=" << s2 << "  !=���ӦΪ1��ʵ�ʣ�" << (s1 != s2) << endl;

		cout << "��1=" << s1 << " ��3=" << s3 << "  > ���ӦΪ0��ʵ�ʣ�" << (s1 > s3) << endl;
		cout << "��1=" << s1 << " ��3=" << s3 << "  >=���ӦΪ1��ʵ�ʣ�" << (s1 >= s3) << endl;
		cout << "��1=" << s1 << " ��3=" << s3 << "  < ���ӦΪ0��ʵ�ʣ�" << (s1 < s3) << endl;
		cout << "��1=" << s1 << " ��3=" << s3 << "  <=���ӦΪ1��ʵ�ʣ�" << (s1 <= s3) << endl;
		cout << "��1=" << s1 << " ��3=" << s3 << "  ==���ӦΪ1��ʵ�ʣ�" << (s1 == s3) << endl;
		cout << "��1=" << s1 << " ��3=" << s3 << "  !=���ӦΪ0��ʵ�ʣ�" << (s1 != s3) << endl;

		cout << "��1=" << s1 << " ��4=" << s4 << " > ���ӦΪ1��ʵ�ʣ�" << (s1 > s4) << endl;
		cout << "��1=" << s1 << " ��4=" << s4 << " >=���ӦΪ1��ʵ�ʣ�" << (s1 >= s4) << endl;
		cout << "��1=" << s1 << " ��4=" << s4 << " < ���ӦΪ0��ʵ�ʣ�" << (s1 < s4) << endl;
		cout << "��1=" << s1 << " ��4=" << s4 << " <=���ӦΪ0��ʵ�ʣ�" << (s1 <= s4) << endl;
		cout << "��1=" << s1 << " ��4=" << s4 << " ==���ӦΪ0��ʵ�ʣ�" << (s1 == s4) << endl;
		cout << "��1=" << s1 << " ��4=" << s4 << " !=���ӦΪ1��ʵ�ʣ�" << (s1 != s4) << endl;

		cout << "��1=" << s1 << " ��5=" << s5 << " > ���ӦΪ1��ʵ�ʣ�" << (s1 > s5) << endl;
		cout << "��1=" << s1 << " ��5=" << s5 << " >=���ӦΪ1��ʵ�ʣ�" << (s1 >= s5) << endl;
		cout << "��1=" << s1 << " ��5=" << s5 << " < ���ӦΪ0��ʵ�ʣ�" << (s1 < s5) << endl;
		cout << "��1=" << s1 << " ��5=" << s5 << " <=���ӦΪ0��ʵ�ʣ�" << (s1 <= s5) << endl;
		cout << "��1=" << s1 << " ��5=" << s5 << " ==���ӦΪ0��ʵ�ʣ�" << (s1 == s5) << endl;
		cout << "��1=" << s1 << " ��5=" << s5 << " !=���ӦΪ1��ʵ�ʣ�" << (s1 != s5) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "house";

		cout << "�Ƚ��������(TString����ַ�������)" << endl;

		cout << "��1=" << s1 << " ����=horse" << "  > ���ӦΪ1��ʵ�ʣ�" << (s1 > "horse") << endl;
		cout << "��1=" << s1 << " ����=horse" << "  >=���ӦΪ1��ʵ�ʣ�" << (s1 >= "horse") << endl;
		cout << "��1=" << s1 << " ����=horse" << "  < ���ӦΪ0��ʵ�ʣ�" << (s1 < "horse") << endl;
		cout << "��1=" << s1 << " ����=horse" << "  <=���ӦΪ0��ʵ�ʣ�" << (s1 <= "horse") << endl;
		cout << "��1=" << s1 << " ����=horse" << "  ==���ӦΪ0��ʵ�ʣ�" << (s1 == "horse") << endl;
		cout << "��1=" << s1 << " ����=horse" << "  !=���ӦΪ1��ʵ�ʣ�" << (s1 != "horse") << endl;

		cout << "��1=" << s1 << " ����=house" << "  > ���ӦΪ0��ʵ�ʣ�" << (s1 > "house") << endl;
		cout << "��1=" << s1 << " ����=house" << "  >=���ӦΪ1��ʵ�ʣ�" << (s1 >= "house") << endl;
		cout << "��1=" << s1 << " ����=house" << "  < ���ӦΪ0��ʵ�ʣ�" << (s1 < "house") << endl;
		cout << "��1=" << s1 << " ����=house" << "  <=���ӦΪ1��ʵ�ʣ�" << (s1 <= "house") << endl;
		cout << "��1=" << s1 << " ����=house" << "  ==���ӦΪ1��ʵ�ʣ�" << (s1 == "house") << endl;
		cout << "��1=" << s1 << " ����=house" << "  !=���ӦΪ0��ʵ�ʣ�" << (s1 != "house") << endl;

		cout << "��1=" << s1 << " ����=" << "       > ���ӦΪ1��ʵ�ʣ�" << (s1 > "") << endl;
		cout << "��1=" << s1 << " ����=" << "       >=���ӦΪ1��ʵ�ʣ�" << (s1 >= "") << endl;
		cout << "��1=" << s1 << " ����=" << "       < ���ӦΪ0��ʵ�ʣ�" << (s1 < "") << endl;
		cout << "��1=" << s1 << " ����=" << "       <=���ӦΪ0��ʵ�ʣ�" << (s1 <= "") << endl;
		cout << "��1=" << s1 << " ����=" << "       ==���ӦΪ0��ʵ�ʣ�" << (s1 == "") << endl;
		cout << "��1=" << s1 << " ����=" << "       !=���ӦΪ1��ʵ�ʣ�" << (s1 != "") << endl;

		cout << "��1=" << s1 << " ����=<NULL>" << " > ���ӦΪ1��ʵ�ʣ�" << (s1 > NULL) << endl;
		cout << "��1=" << s1 << " ����=<NULL>" << " >=���ӦΪ1��ʵ�ʣ�" << (s1 >= NULL) << endl;
		cout << "��1=" << s1 << " ����=<NULL>" << " < ���ӦΪ0��ʵ�ʣ�" << (s1 < NULL) << endl;
		cout << "��1=" << s1 << " ����=<NULL>" << " <=���ӦΪ0��ʵ�ʣ�" << (s1 <= NULL) << endl;
		cout << "��1=" << s1 << " ����=<NULL>" << " ==���ӦΪ0��ʵ�ʣ�" << (s1 == NULL) << endl;
		cout << "��1=" << s1 << " ����=<NULL>" << " !=���ӦΪ1��ʵ�ʣ�" << (s1 != NULL) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = NULL, s2 = "";

		/* �յ�TString��NULL��"" ����Ϊ��ȣ������ϣ�NULL�ǵ�ַ��""�Ǵ���û�пɱ��ԣ����Ǵ˴������⴦�� */
		cout << "��1=" << s1 << " ��2=" << s2 << "  > ���ӦΪ0��ʵ�ʣ�" << (s1 > s2) << endl;
		cout << "��1=" << s1 << " ��2=" << s2 << "  >=���ӦΪ1��ʵ�ʣ�" << (s1 >= s2) << endl;
		cout << "��1=" << s1 << " ��2=" << s2 << "  < ���ӦΪ0��ʵ�ʣ�" << (s1 < s2) << endl;
		cout << "��1=" << s1 << " ��2=" << s2 << "  <=���ӦΪ1��ʵ�ʣ�" << (s1 <= s2) << endl;
		cout << "��1=" << s1 << " ��2=" << s2 << "  ==���ӦΪ1��ʵ�ʣ�" << (s1 == s2) << endl;
		cout << "��1=" << s1 << " ��2=" << s2 << "  !=���ӦΪ0��ʵ�ʣ�" << (s1 != s2) << endl;

		cout << "��1=" << s1 << " ����=" << "       > ���ӦΪ0��ʵ�ʣ�" << (s1 > "") << endl;
		cout << "��1=" << s1 << " ����=" << "       >=���ӦΪ1��ʵ�ʣ�" << (s1 >= "") << endl;
		cout << "��1=" << s1 << " ����=" << "       < ���ӦΪ0��ʵ�ʣ�" << (s1 < "") << endl;
		cout << "��1=" << s1 << " ����=" << "       <=���ӦΪ1��ʵ�ʣ�" << (s1 <= "") << endl;
		cout << "��1=" << s1 << " ����=" << "       ==���ӦΪ1��ʵ�ʣ�" << (s1 == "") << endl;
		cout << "��1=" << s1 << " ����=" << "       !=���ӦΪ0��ʵ�ʣ�" << (s1 != "") << endl;

		cout << "��1=" << s1 << " ����=<NULL>" << " > ���ӦΪ0��ʵ�ʣ�" << (s1 > NULL) << endl;
		cout << "��1=" << s1 << " ����=<NULL>" << " >=���ӦΪ1��ʵ�ʣ�" << (s1 >= NULL) << endl;
		cout << "��1=" << s1 << " ����=<NULL>" << " < ���ӦΪ0��ʵ�ʣ�" << (s1 < NULL) << endl;
		cout << "��1=" << s1 << " ����=<NULL>" << " <=���ӦΪ1��ʵ�ʣ�" << (s1 <= NULL) << endl;
		cout << "��1=" << s1 << " ����=<NULL>" << " ==���ӦΪ1��ʵ�ʣ�" << (s1 == NULL) << endl;
		cout << "��1=" << s1 << " ����=<NULL>" << " !=���ӦΪ0��ʵ�ʣ�" << (s1 != NULL) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "house";
		const char* str2 = "horse", * str3 = "house", * str4 = "", * str5 = NULL;

		cout << "�Ƚ��������(TString����ַ�ָ��)" << endl;

		cout << "��1=" << s1 << " ��2=" << str2 << "  > ���ӦΪ1��ʵ�ʣ�" << (s1 > str2) << endl;
		cout << "��1=" << s1 << " ��2=" << str2 << "  >=���ӦΪ1��ʵ�ʣ�" << (s1 >= str2) << endl;
		cout << "��1=" << s1 << " ��2=" << str2 << "  < ���ӦΪ0��ʵ�ʣ�" << (s1 < str2) << endl;
		cout << "��1=" << s1 << " ��2=" << str2 << "  <=���ӦΪ0��ʵ�ʣ�" << (s1 <= str2) << endl;
		cout << "��1=" << s1 << " ��2=" << str2 << "  ==���ӦΪ0��ʵ�ʣ�" << (s1 == str2) << endl;
		cout << "��1=" << s1 << " ��2=" << str2 << "  !=���ӦΪ1��ʵ�ʣ�" << (s1 != str2) << endl;

		cout << "��1=" << s1 << " ��3=" << str3 << "  > ���ӦΪ0��ʵ�ʣ�" << (s1 > str3) << endl;
		cout << "��1=" << s1 << " ��3=" << str3 << "  >=���ӦΪ1��ʵ�ʣ�" << (s1 >= str3) << endl;
		cout << "��1=" << s1 << " ��3=" << str3 << "  < ���ӦΪ0��ʵ�ʣ�" << (s1 < str3) << endl;
		cout << "��1=" << s1 << " ��3=" << str3 << "  <=���ӦΪ1��ʵ�ʣ�" << (s1 <= str3) << endl;
		cout << "��1=" << s1 << " ��3=" << str3 << "  ==���ӦΪ1��ʵ�ʣ�" << (s1 == str3) << endl;
		cout << "��1=" << s1 << " ��3=" << str3 << "  !=���ӦΪ0��ʵ�ʣ�" << (s1 != str3) << endl;

		cout << "��1=" << s1 << " ��4=" << str4 << "       > ���ӦΪ1��ʵ�ʣ�" << (s1 > str4) << endl;
		cout << "��1=" << s1 << " ��4=" << str4 << "       >=���ӦΪ1��ʵ�ʣ�" << (s1 >= str4) << endl;
		cout << "��1=" << s1 << " ��4=" << str4 << "       < ���ӦΪ0��ʵ�ʣ�" << (s1 < str4) << endl;
		cout << "��1=" << s1 << " ��4=" << str4 << "       <=���ӦΪ0��ʵ�ʣ�" << (s1 <= str4) << endl;
		cout << "��1=" << s1 << " ��4=" << str4 << "       ==���ӦΪ0��ʵ�ʣ�" << (s1 == str4) << endl;
		cout << "��1=" << s1 << " ��4=" << str4 << "       !=���ӦΪ1��ʵ�ʣ�" << (s1 != str4) << endl;

		cout << "��1=" << s1 << " ��5=<NULL>" << " > ���ӦΪ1��ʵ�ʣ�" << (s1 > str5) << endl;
		cout << "��1=" << s1 << " ��5=<NULL>" << " >=���ӦΪ1��ʵ�ʣ�" << (s1 >= str5) << endl;
		cout << "��1=" << s1 << " ��5=<NULL>" << " < ���ӦΪ0��ʵ�ʣ�" << (s1 < str5) << endl;
		cout << "��1=" << s1 << " ��5=<NULL>" << " <=���ӦΪ0��ʵ�ʣ�" << (s1 <= str5) << endl;
		cout << "��1=" << s1 << " ��5=<NULL>" << " ==���ӦΪ0��ʵ�ʣ�" << (s1 == str5) << endl;
		cout << "��1=" << s1 << " ��5=<NULL>" << " !=���ӦΪ1��ʵ�ʣ�" << (s1 != str5) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "house";
		char str2[] = "horse", str3[] = "house", str4[] = "";

		cout << "�Ƚ��������(TString����ַ�ָ��)" << endl;

		cout << "��1=" << s1 << " ��2=" << str2 << " > ���ӦΪ1��ʵ�ʣ�" << (s1 > str2) << endl;
		cout << "��1=" << s1 << " ��2=" << str2 << " >=���ӦΪ1��ʵ�ʣ�" << (s1 >= str2) << endl;
		cout << "��1=" << s1 << " ��2=" << str2 << " < ���ӦΪ0��ʵ�ʣ�" << (s1 < str2) << endl;
		cout << "��1=" << s1 << " ��2=" << str2 << " <=���ӦΪ0��ʵ�ʣ�" << (s1 <= str2) << endl;
		cout << "��1=" << s1 << " ��2=" << str2 << " ==���ӦΪ0��ʵ�ʣ�" << (s1 == str2) << endl;
		cout << "��1=" << s1 << " ��2=" << str2 << " !=���ӦΪ1��ʵ�ʣ�" << (s1 != str2) << endl;

		cout << "��1=" << s1 << " ��3=" << str3 << " > ���ӦΪ0��ʵ�ʣ�" << (s1 > str3) << endl;
		cout << "��1=" << s1 << " ��3=" << str3 << " >=���ӦΪ1��ʵ�ʣ�" << (s1 >= str3) << endl;
		cout << "��1=" << s1 << " ��3=" << str3 << " < ���ӦΪ0��ʵ�ʣ�" << (s1 < str3) << endl;
		cout << "��1=" << s1 << " ��3=" << str3 << " <=���ӦΪ1��ʵ�ʣ�" << (s1 <= str3) << endl;
		cout << "��1=" << s1 << " ��3=" << str3 << " ==���ӦΪ1��ʵ�ʣ�" << (s1 == str3) << endl;
		cout << "��1=" << s1 << " ��3=" << str3 << " !=���ӦΪ0��ʵ�ʣ�" << (s1 != str3) << endl;

		cout << "��1=" << s1 << " ��4=" << str4 << "      > ���ӦΪ1��ʵ�ʣ�" << (s1 > str4) << endl;
		cout << "��1=" << s1 << " ��4=" << str4 << "      >=���ӦΪ1��ʵ�ʣ�" << (s1 >= str4) << endl;
		cout << "��1=" << s1 << " ��4=" << str4 << "      < ���ӦΪ0��ʵ�ʣ�" << (s1 < str4) << endl;
		cout << "��1=" << s1 << " ��4=" << str4 << "      <=���ӦΪ0��ʵ�ʣ�" << (s1 <= str4) << endl;
		cout << "��1=" << s1 << " ��4=" << str4 << "      ==���ӦΪ0��ʵ�ʣ�" << (s1 == str4) << endl;
		cout << "��1=" << s1 << " ��4=" << str4 << "      !=���ӦΪ1��ʵ�ʣ�" << (s1 != str4) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "tong", s2;

		cout << "�󳤶Ȳ���(length()����)" << endl;

		cout << "s1Ϊtong��  ����ӦΪ4��ʵ�ʣ�" << s1.length() << endl;
		cout << "s2Ϊ<NULL>������ӦΪ0��ʵ�ʣ�" << s2.length() << endl;
		s2 = s1 + "ji";
		cout << "s2Ϊtongji������ӦΪ6��ʵ�ʣ�" << s2.length() << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "tong", s2;

		cout << "�󳤶Ȳ���(TStringLen()����)" << endl;

		cout << "s1Ϊtong��  ����ӦΪ4��ʵ�ʣ�" << TStringLen(s1) << endl;
		cout << "s2Ϊ<NULL>������ӦΪ0��ʵ�ʣ�" << TStringLen(s2) << endl;
		cout << "��Ϊtongji������ӦΪ6��ʵ�ʣ�" << TStringLen(s1 + (s2 = "ji")) << endl;
		cout << "��Ϊtongji������ӦΪ6��ʵ�ʣ�" << TStringLen(s1 + "ji") << endl;
		cout << "��Ϊtongji������ӦΪ6��ʵ�ʣ�" << TStringLen(s2 = s1 + "ji") << endl;
		cout << "��Ϊtong��  ����ӦΪ4��ʵ�ʣ�" << TStringLen(s1 + (char*)NULL) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "house";
		TStringAdv s2[4] = { "yangpu", "sipingroad", "tongji", "shanghai" };
		int i, j;
		char ch = 'P';

		cout << "[]���������" << endl;

		cout << "��1��" << s1 << "�����δ�ӡ�ַ��Ľ���ǣ�";
		for (i = 0; i < s1.length(); i++)
			cout << s1[i] << ' ';
		cout << endl << endl;

		cout << "��1��" << s1 << "�����δ�ӡ�ַ�intֵ�Ľ���ǣ���Խ�磩��" << endl;
		for (i = -4; i < s1.length() + 4; i++)
			cout << int(s1[i]) << ' ';
		cout << endl << endl;

		s1[0] = s1[0] - 32;
		cout << "��1��" << s1 << "�����δ�ӡ�ַ��Ľ���ǣ�";
		for (i = 0; i < s1.length(); i++)
			cout << s1[i] << ' ';
		cout << endl << endl;

		s1[1] = 'A';
		s1[2] = ch;
		ch += 32;
		s1[3] = ch;
		s1[4] = 'Y';
		cout << "��1��" << s1 << "�����δ�ӡ�ַ��Ľ���ǣ�";
		for (i = 0; i < s1.length(); i++)
			cout << s1[i] << ' ';
		cout << endl << endl;

		cout << "��2������4���ַ��������δ�ӡ�ַ��Ľ���ǣ�" << endl;
		for (i = 0; i < 4; i++) {
			cout << setw(16) << s2[i] << " => ";
			for (j = 0; j < s2[i].length(); j++)
				cout << s2[i][j] << ' ';
			cout << endl;
		}
		cout << endl;

		cout << "��2������4���ַ���������ĸ��д�����δ�ӡ�ַ��Ľ���ǣ�" << endl;
		for (i = 0; i < 4; i++) {
			s2[i][0] -= 32;
			cout << setw(16) << s2[i] << " => ";
			for (j = 0; j < s2[i].length(); j++)
				cout << s2[i][j] << ' ';
			cout << endl;
		}
		cout << endl;

		wait_for_enter();
	}

	if (1) {
		const static char* standard[] = {
			"",
			"<NULL>", "2", "teststr3", "testst4",
			"teststr5", "teststr3&teststr3",
			"teststr3@4", "teststr5*sub3",
			"<NULL>", "<NULL>"
		};

		TStringAdv s0;

		char			s1 = '\0';
		char			s2 = '2';
		const char		s3[] = "teststr3";
		const char*		s4 = "testst4\0r4";
		char			s5[] = "teststr5";
		TString			s6(TString(s3) + '&' + TString(s3));
		TStringAdv		s7 = TString(s3) + TStringAdv("@4");
		TStringAdv		s8(TStringAdv(s5) + '*' + "sub3");
		const char*		s9 = NULL;
		TStringAdv		s10 = NULL;

		cout << "TStringAdv-assign��������" << endl;

		s0.assign(s1);
		cout << "s1��׼���" << setw(20) << standard[1] << " s1ʵ�����" << setw(20) << s0 << endl;
		s0.assign(s2);
		cout << "s2��׼���" << setw(20) << standard[2] << " s2ʵ�����" << setw(20) << s0 << endl;
		s0.assign(s3);
		cout << "s3��׼���" << setw(20) << standard[3] << " s3ʵ�����" << setw(20) << s0 << endl;
		s0.assign(s4);
		cout << "s4��׼���" << setw(20) << standard[4] << " s4ʵ�����" << setw(20) << s0 << endl;
		s0.assign(s5);
		cout << "s5��׼���" << setw(20) << standard[5] << " s5ʵ�����" << setw(20) << s0 << endl;
		s0.assign(s6);
		cout << "s6��׼���" << setw(20) << standard[6] << " s6ʵ�����" << setw(20) << s0 << endl;
		s0.assign(s7);
		cout << "s7��׼���" << setw(20) << standard[7] << " s7ʵ�����" << setw(20) << s0 << endl;
		s0.assign(s8);
		cout << "s8��׼���" << setw(20) << standard[8] << " s8ʵ�����" << setw(20) << s0 << endl;
		s0.assign(s9);
		cout << "s9��׼���" << setw(20) << standard[9] << " s9ʵ�����" << setw(20) << s0 << endl;
		s0.assign(s10);
		cout << "sA��׼���" << setw(20) << standard[10] << " sAʵ�����" << setw(20) << s0 << endl;

		wait_for_enter();
	}

	if (1) {
		const static char* standard[] = {
			"",
			"teststr1", "teststr2", "teststr3", "teststr4",
			"teststr(2+3)", "teststr(2+4)", "teststr(2+5)", "teststr(2+6)"
		};

		cout << "TStringAdv-append��������(���������2-3��ͣ��)" << endl;

		TStringAdv			s1 = TStringAdv().append("tes").append("ts\0t").append("tr1");
		TStringAdv			s2 = TStringAdv("te").append(TString("st")).append(TStringAdv("st")).append("r2");
		TStringAdv			s3 = TStringAdv("");
		TStringAdv			s4 = TStringAdv().append(TString()).append("t\0s").append("est")
			.append('s').append(TStringAdv("t\0r")).append("r4");
		char s3_ref[9] = "teststr3";
		for (int i = 0; i < 800; i++)
			s3.append(i % 100 ? '\0' : s3_ref[i / 100]);
		
		TStringAdv s5 = "teststr(2", s6 = s5, s7 = TStringAdv(s6), s8 = TString(s7);
		s5.append("+3)");
		s6.append(TString("+4)"));
		s7.append((const char*)(NULL));
		s7.append(TStringAdv(TString("+5)")));
		s8.append('+');
		s8.append("6)");

		int s9_flag = 1;
		TStringAdv s9;
		char* s9_buf;
		for (int i = 1; i <= 256; i++) {
			s9_buf = big_buffer(65536);
			s9.append(s9_buf);
			for (int j = 1, pos; j <= 100; j++) {
				pos = 32768 * (rand() % 2) + rand() % 32768;
				s9_flag &= int(s9[pos + 65536 * (i - 1)] == s9_buf[pos]);
			}
			delete[] s9_buf;
		}

		cout << "s1��׼���" << setw(15) << standard[1] << " s1ʵ�����" << setw(15) << s1 << endl;
		cout << "s2��׼���" << setw(15) << standard[2] << " s2ʵ�����" << setw(15) << s2 << endl;
		cout << "s3��׼���" << setw(15) << standard[3] << " s3ʵ�����" << setw(15) << s3 << endl;
		cout << "s4��׼���" << setw(15) << standard[4] << " s4ʵ�����" << setw(15) << s4 << endl;
		cout << "s5��׼���" << setw(15) << standard[5] << " s5ʵ�����" << setw(15) << s5 << endl;
		cout << "s6��׼���" << setw(15) << standard[6] << " s6ʵ�����" << setw(15) << s6 << endl;
		cout << "s7��׼���" << setw(15) << standard[7] << " s7ʵ�����" << setw(15) << s7 << endl;
		cout << "s8��׼���" << setw(15) << standard[8] << " s8ʵ�����" << setw(15) << s8 << endl;
		cout << "s9���۳���" << setw(15) << 16777216    << " s9ʵ�ʳ���" << setw(15) << TStringLen(s9) << endl;
		cout << "s9 - ����������" << (s9_flag ? "ͨ��" : "��ͨ��������append����ʵ��") << endl;

		wait_for_enter();
	}

	if (1) {
		const static char* standard[] = {
			"",
			"I'm a C++owboy.",
			"I loathe Visual Basic.",
			"I hate Java.",
			"I boycott Python.",
			"And I am proud to be a C++ programmer."
		};
		cout << "TStringAdv-insert��������" << endl;

		TStringAdv			s1 = "I'm a Cowboy.";
		TStringAdv			s2 = "I Visual Basic.";
		TStringAdv			s3 = "I Java.";
		TStringAdv			s4 = "I Python.";
		TStringAdv			s5 = "And I C++.";

		s1.insert("++", 8);
		s2.insert("loat", 3).insert("he", 7).insert(' ', 9);
		s3.insert(TStringAdv().insert("hate ", 1), 3);
		s4.insert("love", -1).insert("am obsessed with ", 1024).insert("boycott ", 3);
		s5.insert("am proud to be a ", 7).insert((const char*)NULL, 6);
		s5.insert(" programmer", TStringLen(s5));

		cout << setw(15) << "s1��׼���" << setw(45) << standard[1] << endl << setw(15) << "s1ʵ�����" << setw(45) << s1 << endl;
		cout << setw(15) << "s2��׼���" << setw(45) << standard[2] << endl << setw(15) << "s2ʵ�����" << setw(45) << s2 << endl;
		cout << setw(15) << "s3��׼���" << setw(45) << standard[3] << endl << setw(15) << "s3ʵ�����" << setw(45) << s3 << endl;
		cout << setw(15) << "s4��׼���" << setw(45) << standard[4] << endl << setw(15) << "s4ʵ�����" << setw(45) << s4 << endl;
		cout << setw(15) << "s5��׼���" << setw(45) << standard[5] << endl << setw(15) << "s5ʵ�����" << setw(45) << s5 << endl;

		cout << "������һЩ������insert������ͼ����������֤������������ȷ������ɣ�" << endl;

		cout << setw(15) << "s1��ȷ���" << setw(45) << standard[1] << endl;
		cout << setw(15) << "s1Խ�����-��1" << setw(45) << TStringAdv(s1).insert("am", -1) << endl;
		cout << setw(15) << "s1Խ�����-��2" << setw(45) << TStringAdv(s1).insert('*', TStringLen(s1) + 10000) << endl;
		cout << setw(15) << "s1����β��" << setw(45) << TStringAdv(s1).insert('\0', TStringLen(s1) / 2) << endl;
		cout << setw(15) << "s1����մ�" << setw(45) << TStringAdv(s1).insert("", (int)(0.4 * TStringLen(s1))) << endl;
		cout << setw(15) << "s1����NULL" << setw(45) << TStringAdv(s1).insert((const char*)(NULL), (int)(0.4 * TStringLen(s1))) << endl;

		wait_for_enter();
	}

	if (1) {
		const static char* standard[] = {
			"",
			"CandySaga", "Tongji", "Division",
			"SchoolOfEngineering", "DeptOfComputingMachinery",
			"CSPrfession"
		};
		cout << "TStringAdv-erase��������" << endl;

		TStringAdv			s1 = "CandyCrushSaga";
		TStringAdv			s2 = "TongjiUniv";
		TStringAdv			s3 = "JiadingDivision";
		TStringAdv			s4 = "SchoolOfElectricEngineering";
		TStringAdv			s5 = "DeptOfComputingMachinery";
		TStringAdv			s6 = "CSProfession";

		cout << setw(15) << "s1��׼���" << setw(45) << standard[1] << endl << setw(15) << "s1ʵ�����" << setw(45) << s1.erase("Crush") << endl;
		cout << setw(15) << "s2��׼���" << setw(45) << standard[2] << endl << setw(15) << "s2ʵ�����" << setw(45) << s2.erase("Univ") << endl;
		cout << setw(15) << "s3��׼���" << setw(45) << standard[3] << endl << setw(15) << "s3ʵ�����" << setw(45) << s3.erase("Jiading\0xiaoqu") << endl;
		cout << setw(15) << "s4��׼���" << setw(45) << standard[4] << endl << setw(15) << "s4ʵ�����" << setw(45) << s4.erase(TString("Electric")) << endl;
		cout << setw(15) << "s5��׼���" << setw(45) << standard[5] << endl << setw(15) << "s5ʵ�����" << setw(45) << s5.erase(TStringAdv("Computer")) << endl;
		cout << setw(15) << "s6��׼���" << setw(45) << standard[6] << endl << setw(15) << "s6ʵ�����" << setw(45) << s6.erase('o') << endl;

		cout << "������һЩ������erase������ͼ����������֤���������ȷ������ɣ�" << endl;

		cout << setw(15) << "s5��ȷ���" << setw(45) << standard[5] << endl;
		cout << setw(15) << "s5ȥ��β��" << setw(45) << TStringAdv(s5).erase('\0') << endl;
		cout << setw(15) << "s5ȥ���մ�" << setw(45) << TStringAdv(s5).erase("") << endl;
		cout << setw(15) << "s5ȥ��NULL" << setw(45) << TStringAdv(s5).erase((const char*)NULL) << endl;
		cout << setw(15) << "s5ȥ����Ч��" << setw(45) << TStringAdv(s5).erase("\03" + s5) << endl;

		wait_for_enter();
	}

	if (1) {
		const static char* standard[] = {
			"",
			"hangha", "tong", "ngpu", "sipingroad", "_freshman"
		};

		cout << "TStringAdv-substr��������" << endl;

		TStringAdv			s1 = "shanghai";
		TStringAdv			s2 = TString("tongji");
		TStringAdv			s3 = TStringAdv("yangpu");
		TStringAdv			s4 = "sipingroad";
		TStringAdv			s5 = "school_for_freshmen";

		cout << setw(15) << "s1��׼���" << setw(45) << standard[1] << endl << setw(15) << "s1ʵ�����" << setw(45) << s1.substr(2, 6) << endl;
		cout << setw(15) << "s2��׼���" << setw(45) << standard[2] << endl << setw(15) << "s2ʵ�����" << setw(45) << s2.substr(1, 4) << endl;
		cout << setw(15) << "s3��׼���" << setw(45) << standard[3] << endl << setw(15) << "s3ʵ�����" << setw(45) << s3.substr(3) << endl;
		cout << setw(15) << "s4��׼���" << setw(45) << standard[4] << endl << setw(15) << "s4ʵ�����" << setw(45) << s4.substr(1, 8388608) << endl;
		cout << setw(15) << "s5��׼���" << setw(45) << standard[5] << endl << setw(15) << "s5ʵ�����" << setw(45) << s5.substr(11, 536870912) << endl;

		cout << "������һЩ������substr������ͼ����������֤���������ȷ������ɣ�" << endl;

		cout << setw(15) << "s5��ȷ���" << setw(45) << "<NULL>" << endl;
		cout << setw(15) << "s5��Ԥ������1" << setw(45) << TStringAdv(s5).substr(0, 9) << endl;
		cout << setw(15) << "s5��Ԥ������2" << setw(45) << TStringAdv(s5).substr(-1, 2) << endl;
		cout << setw(15) << "s5��Ԥ������3" << setw(45) << TStringAdv(s5).substr(3, 0) << endl;
		cout << setw(15) << "s5��Ԥ������4" << setw(45) << TStringAdv(s5).substr(1048576, 0) << endl;

		wait_for_enter();
	}

	if (1) {
		cout << "TStringAdv-at��������" << endl;

		TStringAdv			s1 = "shanghai";
		TStringAdv			s2 = TString("tongji");
		TStringAdv			s3 = TStringAdv("yangpu");
		TStringAdv			s4 = "sipingroad";

		cout << setw(15) << "s1��׼���" << setw(45) << s1 << endl;
		cout << setw(15) << "s2��׼���" << setw(45) << s2 << endl;
		cout << setw(15) << "s3��׼���" << setw(45) << s3 << endl;
		cout << setw(15) << "s4��׼���" << setw(45) << s4 << endl;

		cout << setw(15) << "s1���ַ�չ��";
		for (int i = 0, _l = TStringLen(s1); i < _l; i++)
			cout << ' ' << s1[i];
		cout << endl;

		cout << setw(15) << "s2���ַ�չ��";
		for (int i = 0, _l = TStringLen(s2); i < _l; i++)
			cout << ' ' << s2[i];
		cout << endl;

		cout << setw(15) << "s3���ַ�չ��";
		for (int i = 0, _l = TStringLen(s3); i < _l; i++)
			cout << ' ' << s3[i];
		cout << endl;

		cout << setw(15) << "s4���ַ�չ��";
		for (int i = 0, _l = TStringLen(s4); i < _l; i++)
			cout << ' ' << s4[i];
		cout << endl;

		wait_for_enter();
	}

	if (1) {
		cout << "TStringAdv-�ۺϲ���" << endl;

		TStringAdv			s1, s2, s3, s4, s5;

		s1.assign("sipingroad").append("-1239").substr(7).erase('-')
			.append("-tju").erase("1239");
		s2.assign("I like Python\0hub").insert("dis", 3).erase("Python").append("Java")
			.append("Script").erase("Java").append(" Language.");
		(s3.assign("-")) *= 127;
		int s3_cnt = 0;
		while (s3.length() > 1) {
			if (s3.length() % 2 == 0)
				s3.erase(TStringAdv("-") * (s3.length() / 2));
			else
				s3.append(s3 * 2 + "-");
			s3_cnt++;
		}

		s4.assign("/home").append("/com&*^&*^&mon").append("/16-b2").append("/16-b2-main.cpp")
			.erase("&*^&*^&").insert("/u2053642", 6);

		cout << setw(15) << "s1����ֵ" << setw(60) << "sipingroad-1239" << endl;
		cout << setw(15) << "s1ʵ��ֵ" << setw(60) << s1 << endl;
		cout << setw(15) << "s2����ֵ" << setw(60) << "I dislike Script Language." << endl;
		cout << setw(15) << "s2ʵ��ֵ" << setw(60) << s2 << endl;
		cout << setw(15) << "s3�����������ֵ" << setw(60) << "46" << endl;
		cout << setw(15) << "s3�������ʵ��ֵ" << setw(60) << s3_cnt << endl;
		cout << setw(15) << "s4����ֵ" << setw(60) << "/home/u2053642/common/16-b2/16-b2-main.cpp" << endl;
		cout << setw(15) << "s4ʵ��ֵ" << setw(60) << s4 << endl;

		wait_for_enter();
	}

#if !(defined(__linux) || defined(__linux__))
	system("pause"); //Linux����Ҫ
#endif

	return 0;
}
