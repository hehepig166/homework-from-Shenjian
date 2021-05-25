/* 2053642 信18 李博宇 */
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void wait_for_enter()
{
	cout << endl << "按回车键继续";
	while (getchar() != '\n')
		;
	cout << endl << endl;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void wait_for_enter()
{
	cout << endl << "按回车键继续";
	while (_getch() != '\r')
		;
	cout << endl << endl;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
unsigned long long get_cur_msec(void)
{
	return GetTickCount(); //取当前系统时间，单位毫秒
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main()
{
	srand((unsigned)time(NULL));
	int ts_passed;
	while (1) {
		cout << "是否跳过TString的测试部分? (0:否,1:是):";
		cin >> ts_passed;
		if (!cin || (ts_passed < 0 || ts_passed > 1)) {
			cin.clear();
			cin.ignore(1 << 12, '\n');
			continue;
		}
		break;
	}

	if (ts_passed == 0) {
		const TStringAdv s1;                 //s1为NULL
		const TStringAdv s2("teststr2");      //s2为"teststr2"
		const TStringAdv s3 = "teststr3";       //s3为"teststr3"
		const TStringAdv s4 = NULL;            //s4为NULL
		const TStringAdv s5 = "";              //s5为NULL

		cout << "定义对象并初始化测试(NULL及字符串常量初始化)" << endl;

		cout << "s1应该是<NULL>，  实际输出：" << s1 << endl;
		cout << "s2应该是teststr2，实际输出：" << s2 << endl;
		cout << "s3应该是teststr3，实际输出：" << s3 << endl;
		cout << "s4应该是<NULL>，  实际输出：" << s4 << endl;
		cout << "s5应该是<NULL>，  实际输出：" << s5 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		const TStringAdv s0 = "teststr1";
		const char* str1 = "teststr2";
		const char  str2[] = "teststr3";
		const char* str3 = NULL;
		const char  str4[] = "";
		const TStringAdv s1 = s0, s2 = str1, s3 = str2, s4 = str3, s5 = str4;

		cout << "定义对象并初始化测试(TString对象及字符指针、字符数组)" << endl;

		cout << "s1应该是teststr1，实际输出：" << s1 << endl;
		cout << "s2应该是teststr2，实际输出：" << s2 << endl;
		cout << "s3应该是teststr3，实际输出：" << s3 << endl;
		cout << "s4应该是<NULL>，  实际输出：" << s4 << endl;
		cout << "s5应该是<NULL>，  实际输出：" << s5 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1;

		cout << "重载cin测试" << endl;

		cout << "请在键盘上输入Hello" << endl;
		cin >> s1;
		cout << "s1应该是Hello，实际输出：" << s1 << endl;

		cout << "请在键盘上输入Hello 123" << endl;
		cin >> s1;
		cout << "s1应该是Hello，实际输出：" << s1 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("Hello"), s2;

		cout << "赋值操作测试" << endl;

		cout << "s1应该是Hello， 实际输出：" << s1 << endl;

		s2 = s1;
		cout << "s2应该是Hello， 实际输出：" << s2 << endl;

		s1 = "Hi";
		cout << "s1应该是Hi，    实际输出：" << s1 << endl;

		s2 = "";
		cout << "s2应该是<NULL>，实际输出：" << s2 << endl;

		s1 = NULL;
		cout << "s1应该是<NULL>，实际输出：" << s1 << endl;

		s1 = "Hello";
		cout << "s1应该是Hello， 实际输出：" << (s1 = s1) << endl;
		cout << "s1应该是Hello， 实际输出：" << (s1 = s1 = s1 = s1 = s1) << endl;
		cout << "s1应该是Hello， 实际输出：" << (((s1 = s1) = s1) = s1) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s0, s1("tong"), s2("ji"), s3;

		cout << "连接(+)测试(两个TString类)" << endl;

		s3 = s1 + s2;
		cout << "s3应为tongji，实际输出：" << s3 << endl;

		s3 = s2 + s1;
		cout << "s3应为jitong，实际输出：" << s3 << endl;

		s3 = s1 + s0;
		cout << "s3应为tong，  实际输出：" << s3 << endl;

		s3 = s0 + s2;
		cout << "s3应为ji，    实际输出：" << s3 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("tong"), s2, s3;

		cout << "连接(+)测试(TString类和字符串常量)" << endl;

		s3 = s1 + "ji";
		cout << "s3应为tongji，实际输出：" << s3 << endl;

		s3 = "ji" + s1;
		cout << "s3应为jitong，实际输出：" << s3 << endl;

		s3 = s1 + "";
		cout << "s3应为tong，  实际输出：" << s3 << endl;

		s3 = s1 + (char*)NULL;
		cout << "s3应为tong，  实际输出：" << s3 << endl;

		s3 = "" + s1;
		cout << "s3应为tong，  实际输出：" << s3 << endl;

		s3 = (char*)NULL + s1;
		cout << "s3应为tong，  实际输出：" << s3 << endl;

		s3 = s2 + (char*)NULL;
		cout << "s3应为<NULL>，实际输出：" << s3 << endl;

		s3 = (char*)NULL + s2;
		cout << "s3应为<NULL>，实际输出：" << s3 << endl;

		s3 = s2 + "";
		cout << "s3应为<NULL>，实际输出：" << s3 << endl;

		s3 = "" + s2;
		cout << "s3应为<NULL>，实际输出：" << s3 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("tong"), s3;
		const char* str1 = "ji", * str2 = "", * str3 = NULL;

		cout << "连接(+)测试(TString类和字符指针)" << endl;

		s3 = s1 + str1;
		cout << "s3应为tongji，实际输出：" << s3 << endl;

		s3 = str1 + s1;
		cout << "s3应为jitong，实际输出：" << s3 << endl;

		s3 = s1 + str2;
		cout << "s3应为tong，  实际输出：" << s3 << endl;

		s3 = str2 + s1;
		cout << "s3应为tong，  实际输出：" << s3 << endl;

		s3 = s1 + str3;
		cout << "s3应为tong，  实际输出：" << s3 << endl;

		s3 = str3 + s1;
		cout << "s3应为tong，  实际输出：" << s3 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("tong"), s3;
		char str1[] = "ji", str2[] = "";

		cout << "连接(+)测试(TString类和字符数组)" << endl;

		s3 = s1 + str1;
		cout << "s3应为tongji，实际输出：" << s3 << endl;

		s3 = str1 + s1;
		cout << "s3应为jitong，实际输出：" << s3 << endl;

		s3 = s1 + str2;
		cout << "s3应为tong，  实际输出：" << s3 << endl;

		s3 = str2 + s1;
		cout << "s3应为tong，  实际输出：" << s3 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("Hello"), s3;
		char c = '!';

		cout << "连接(+)测试(TString类和字符)" << endl;

		s3 = s1 + c;
		cout << "s3应为Hello!，实际输出：" << s3 << endl;

		s3 = c + s1;
		cout << "s3应为!Hello，实际输出：" << s3 << endl;

		s1 = "ello";
		cout << "s3应为Hello!，实际输出：" << 'H' + s1 + c << endl;

		s1 = "hina";
		cout << "s3应为China!，实际输出：" << (s1 = 'C' + s1 + c) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		const TStringAdv s1("可"), s3 = "清", s5 = "也";
		TStringAdv s;
		const char* str2 = "以", str4[] = "心";
		const char c = '!';

		cout << "连接(+)综合测试(TString类和TString类、字符指针、字符数组、字符)" << endl;
		s = s1 + str2 + s3 + str4 + s5 + c;
		cout << "s应该是可以清心也!，实际输出：" << s << endl;

		cout << "s应该是也可以清心!，实际输出：" << (s = s5 + s1 + str2 + s3 + str4[0] + str4[1] + c) << endl;
		cout << "s应该是心也可以清!，实际输出：" << (s = str4 + s5 + s1.c_str() + str2 + s3 + c) << endl;
		cout << "s应该是清心也可以!，实际输出：" << (s = s3 + str4 + s5 + s1 + str2[0] + &str2[1] + c) << endl;
		cout << "s应该是以清心也可!，实际输出：" << (s = str2 + s3 + str4 + s5.c_str() + s1.c_str() + c) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("tong"), s2 = "ji";
		const char* str3 = "tong", str4[] = "ji";

		cout << "自连接(+=)简单测试(TString类和TString类、字符指针、字符数组、字符)" << endl;

		s1 += s2;
		cout << "s1应为tongji，实际输出：" << s1 << endl;

		s1 = "tong";
		s2 += s1;
		cout << "s2应为jitong，实际输出：" << s2 << endl;

		s1 = "tong";
		s1 += "ji";
		cout << "s1应为tongji，实际输出：" << s1 << endl;

		s1 = "ji";
		s1 += str3;
		cout << "s1应为jitong，实际输出：" << s1 << endl;

		s1 = "tong";
		s1 += str4;
		cout << "s1应为tongji，实际输出：" << s1 << endl;

		s1 = "tong";
		s1 += 'j';
		s1 += 'i';
		cout << "s1应为tongji，实际输出：" << s1 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("可"), s3 = "清", s5 = "也";
		TStringAdv s;
		const char* str2 = "以", str4[] = "心";
		const char c = '!';

		cout << "自连接(+=)综合测试(TString类和TString类、字符指针、字符数组、字符)" << endl;

		s = "1.";
		s += s1 + str2 + s3 + str4 + s5 + c;
		cout << "s应该是1.可以清心也!，实际输出：" << s << endl;

		s = "2.";
		cout << "s应该是2.也可以清心!，实际输出：" << (s += s5 + s1 + str2 + s3 + str4[0] + str4[1] + c) << endl;

		s = NULL;
		cout << "s应该是3.心也可以清!，实际输出：" << ((s = "3.") += str4 + s5 + s1.c_str() + str2 + s3 + c) << endl;

		s = NULL;
		cout << "s应该是4.清心也可以!，实际输出：" << (((s += "4." + s3) += (str4 + s5)) += (((s1 += str2[0]) += &str2[1]) += c)) << endl;

		s = NULL;
		s += '5';
		s += '.';
		s1 = "可";
		cout << "s应该是5.以清心也可!，实际输出：" << ((((((s += str2) += s3) += str4) += s5.c_str()) += s1.c_str()) += c) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("This is a pencil"), s2("is"), s3;

		cout << "删除(-)测试(TString类和TString类)" << endl;

		cout << "s1应为Th is a pencil，实际输出：" << (s1 = s1 - s2) << endl;
		s3 = s1 - s2;
		cout << "s3应为Th  a pencil，  实际输出：" << s3 << endl;

		s1 = "This is a pencil";
		cout << "s1应为Th  a pencil，  实际输出：" << s1 - s2 - s2 - s2 - s2 - s2 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("This is a pencil"), s2;
		const char* str1 = "is";
		const char str2[] = "is";

		cout << "删除(-)测试(TString类和char指针及数组)" << endl;

		cout << "s1应为Th is a pencil，实际输出：" << (s1 = s1 - str1) << endl;
		s2 = s1 - str1;
		cout << "s2应为Th  a pencil，  实际输出：" << s2 << endl;

		s1 = "This is a pencil";
		cout << "s1应为Th is a pencil，实际输出：" << (s1 = s1 - str2) << endl;
		s2 = s1 - str2;
		cout << "s2应为Th  a pencil，  实际输出：" << s2 << endl;

		s1 = "This is a pencil";
		cout << "s1应为Th  a pencil，  实际输出：" << s1 - str1 - str1 - str1 - str1 - str1 << endl;

		s1 = "This is a pencil";
		cout << "s1应为Th  a pencil，  实际输出：" << s1 - str2 - str2 - str2 - str2 - str2 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("This is a pencil"), s2;
		char c1 = 'i';

		cout << "删除(-)测试(TString类和char字符)" << endl;

		cout << "s1应为Ths is a pencil，实际输出：" << (s1 = s1 - c1) << endl;
		s2 = s1 - c1;
		cout << "s2应为Ths s a pencil， 实际输出：" << s2 << endl;

		s1 = "This is a pencil";
		cout << "s2应为Ths s a pencl，  实际输出：" << (s1 - c1 - c1 - c1 - c1 - c1 - c1 - c1) << endl;

		s1 = "This is a pencil";
		cout << "s1应为Thi is a pencil，实际输出：" << (s1 = s1 - 's') << endl;
		s2 = s1 - 's';
		cout << "s2应为Thi i a pencil， 实际输出：" << s2 << endl;

		s1 = "This is a pencil";
		cout << "s2应为Thi i a pencil， 实际输出：" << (s1 - 's' - 's' - 's' - 's' - 's' - 's') << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1;
		const TStringAdv s2 = "is";
		const char* str3 = "pen", str4[] = "a";
		const char c = ' ';

		cout << "自删除(-=)简单测试(TString类和TString类、字符指针、字符数组、字符)" << endl;

		s1 = "This is a red pencil";
		cout << "s1应为Th is a red pencil， 实际输出：" << (s1 -= s2) << endl;

		s1 = "This is a red pencil";
		cout << "s1应为This is a red cil，  实际输出：" << (s1 -= str3) << endl;

		s1 = "This is a red pencil";
		cout << "s1应为This is  red pencil，实际输出：" << (s1 -= str4) << endl;

		s1 = "This is a red pencil";
		cout << "s1应为Thisis a red pencil，实际输出：" << (s1 -= c) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1;
		const TStringAdv s2 = "is";
		const char* str3 = "pen", str4[] = "a";
		const char c = ' ';

		cout << "自删除(-=)综合测试(TString类和TString类、字符指针、字符数组、字符)" << endl;

		s1 = "This is a red pencil";
		cout << "s1应为Th  a red pencil，   实际输出：" << ((s1 -= s2) -= s2) << endl;

		s1 = "This is a red pencil";
		cout << "s1应为This is  red cil，   实际输出：" << ((s1 -= str3) -= str4) << endl;

		s1 = "This is a red pencil";
		cout << "s1应为Thisisaredpencil，   实际输出：" << ((((s1 -= c) -= c) -= c) -= c) << endl;

		s1 = "This is a red pencil";
		cout << "s1应为Th，                 实际输出：" << ((((((((((((s1 -= s2) -= s2) -= str3) -= str4) -= c) -= c) -= c) -= c) -= "red") -= 'c') -= 'i') -= 'l') << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("Hello"), s2;

		cout << "复制(*)测试(TString类和int)" << endl;

		s2 = s1 * 2;
		cout << "s2应为HelloHello，     实际输出：" << s2 << endl;
		cout << "s1应为HelloHelloHello，实际输出：" << (s1 = s1 * 3) << endl;

		TStringAdv s3;
		cout << "s3应为<NULL>，         实际输出：" << (s3 * 5) << endl;
		cout << "s3应为<NULL>，         实际输出：" << (s3 = s3 * 5) << endl;

		int i, n = 100;
		s1 = "Hello";
		s2 = s1 * n;
		for (i = 1; i < n; i++)
			s2 = s2 - s1;
		cout << "s2应为Hello，          实际输出：" << s2 << endl;

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
		for (i = 1; i < n; i++)  //注意，只有这个循环是从1开始，少做1次
			s2 = s2 - 'e';
		cout << "s2应为e，              实际输出：" << s2 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1;

		cout << "自复制(*=)测试(TString类和int)" << endl;

		s1 = "Hello";
		cout << "s1应为HelloHello，                       实际输出：" << (s1 *= 2) << endl;
		s1 += '!';
		cout << "s1应为HelloHello!HelloHello!HelloHello!，实际输出：" << (s1 *= 3) << endl;

		s1 = "Hello";
		cout << "s1应为HelloHello!HelloHello!HelloHello!，实际输出：" << (((s1 *= 2) += '!') *= 3) << endl;

		//		特别测试：打开下面这行的注释，期望编译器会报错，如果不报错，说明定义有问题，不符合预期的语义（为什么？仔细想!!!）
		//		cout << "s1应为HelloHello!HelloHello!HelloHello!，实际输出：" << (((s1 *= 2) + '!') *= 3) << endl;

		s1 = NULL;
		cout << "s1应为<NULL>，                           实际输出：" << (s1 *= 100) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1;

		cout << "自连接(+=)/自删除(-=)/自复制(*=)综合测试" << endl;

		s1 = "Hello";
		cout << "s1应为Hello!Hello!Hello!，实际输出：" << ((s1 += '!') *= 3) << endl;

		s1 = "Hello";
		cout << "s1应为Heo!Heo!Heo!Heo!，  实际输出：" << (((s1 += '!') -= "ll") *= 4) << endl;

		s1 = "Hello";
		cout << "s1应为HellHello!，        实际输出：" << (((s1 *= 2) -= 'o') += '!') << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1("This is a pencil");

		cout << "反转(!)测试" << endl;

		cout << " s1应为This is a pencil，实际输出：" << s1 << endl;
		cout << "!s1应为licnep a si sihT，实际输出：" << !s1 << endl;
		cout << " s1仍为This is a pencil，实际输出：" << s1 << endl;

		s1 = NULL;
		cout << " s1应为<NULL>，          实际输出：" << s1 << endl;
		cout << "!s1应为<NULL>，          实际输出：" << !s1 << endl;
		cout << " s1仍为<NULL>，          实际输出：" << s1 << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "house", s2 = "horse", s3 = "house", s4 = "", s5 = NULL;

		cout << "比较运算测试(两个TString类)" << endl;

		cout << "串1=" << s1 << " 串2=" << s2 << "  > 结果应为1，实际：" << (s1 > s2) << endl;
		cout << "串1=" << s1 << " 串2=" << s2 << "  >=结果应为1，实际：" << (s1 >= s2) << endl;
		cout << "串1=" << s1 << " 串2=" << s2 << "  < 结果应为0，实际：" << (s1 < s2) << endl;
		cout << "串1=" << s1 << " 串2=" << s2 << "  <=结果应为0，实际：" << (s1 <= s2) << endl;
		cout << "串1=" << s1 << " 串2=" << s2 << "  ==结果应为0，实际：" << (s1 == s2) << endl;
		cout << "串1=" << s1 << " 串2=" << s2 << "  !=结果应为1，实际：" << (s1 != s2) << endl;

		cout << "串1=" << s1 << " 串3=" << s3 << "  > 结果应为0，实际：" << (s1 > s3) << endl;
		cout << "串1=" << s1 << " 串3=" << s3 << "  >=结果应为1，实际：" << (s1 >= s3) << endl;
		cout << "串1=" << s1 << " 串3=" << s3 << "  < 结果应为0，实际：" << (s1 < s3) << endl;
		cout << "串1=" << s1 << " 串3=" << s3 << "  <=结果应为1，实际：" << (s1 <= s3) << endl;
		cout << "串1=" << s1 << " 串3=" << s3 << "  ==结果应为1，实际：" << (s1 == s3) << endl;
		cout << "串1=" << s1 << " 串3=" << s3 << "  !=结果应为0，实际：" << (s1 != s3) << endl;

		cout << "串1=" << s1 << " 串4=" << s4 << " > 结果应为1，实际：" << (s1 > s4) << endl;
		cout << "串1=" << s1 << " 串4=" << s4 << " >=结果应为1，实际：" << (s1 >= s4) << endl;
		cout << "串1=" << s1 << " 串4=" << s4 << " < 结果应为0，实际：" << (s1 < s4) << endl;
		cout << "串1=" << s1 << " 串4=" << s4 << " <=结果应为0，实际：" << (s1 <= s4) << endl;
		cout << "串1=" << s1 << " 串4=" << s4 << " ==结果应为0，实际：" << (s1 == s4) << endl;
		cout << "串1=" << s1 << " 串4=" << s4 << " !=结果应为1，实际：" << (s1 != s4) << endl;

		cout << "串1=" << s1 << " 串5=" << s5 << " > 结果应为1，实际：" << (s1 > s5) << endl;
		cout << "串1=" << s1 << " 串5=" << s5 << " >=结果应为1，实际：" << (s1 >= s5) << endl;
		cout << "串1=" << s1 << " 串5=" << s5 << " < 结果应为0，实际：" << (s1 < s5) << endl;
		cout << "串1=" << s1 << " 串5=" << s5 << " <=结果应为0，实际：" << (s1 <= s5) << endl;
		cout << "串1=" << s1 << " 串5=" << s5 << " ==结果应为0，实际：" << (s1 == s5) << endl;
		cout << "串1=" << s1 << " 串5=" << s5 << " !=结果应为1，实际：" << (s1 != s5) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "house";

		cout << "比较运算测试(TString类和字符串常量)" << endl;

		cout << "串1=" << s1 << " 常量=horse" << "  > 结果应为1，实际：" << (s1 > "horse") << endl;
		cout << "串1=" << s1 << " 常量=horse" << "  >=结果应为1，实际：" << (s1 >= "horse") << endl;
		cout << "串1=" << s1 << " 常量=horse" << "  < 结果应为0，实际：" << (s1 < "horse") << endl;
		cout << "串1=" << s1 << " 常量=horse" << "  <=结果应为0，实际：" << (s1 <= "horse") << endl;
		cout << "串1=" << s1 << " 常量=horse" << "  ==结果应为0，实际：" << (s1 == "horse") << endl;
		cout << "串1=" << s1 << " 常量=horse" << "  !=结果应为1，实际：" << (s1 != "horse") << endl;

		cout << "串1=" << s1 << " 常量=house" << "  > 结果应为0，实际：" << (s1 > "house") << endl;
		cout << "串1=" << s1 << " 常量=house" << "  >=结果应为1，实际：" << (s1 >= "house") << endl;
		cout << "串1=" << s1 << " 常量=house" << "  < 结果应为0，实际：" << (s1 < "house") << endl;
		cout << "串1=" << s1 << " 常量=house" << "  <=结果应为1，实际：" << (s1 <= "house") << endl;
		cout << "串1=" << s1 << " 常量=house" << "  ==结果应为1，实际：" << (s1 == "house") << endl;
		cout << "串1=" << s1 << " 常量=house" << "  !=结果应为0，实际：" << (s1 != "house") << endl;

		cout << "串1=" << s1 << " 常量=" << "       > 结果应为1，实际：" << (s1 > "") << endl;
		cout << "串1=" << s1 << " 常量=" << "       >=结果应为1，实际：" << (s1 >= "") << endl;
		cout << "串1=" << s1 << " 常量=" << "       < 结果应为0，实际：" << (s1 < "") << endl;
		cout << "串1=" << s1 << " 常量=" << "       <=结果应为0，实际：" << (s1 <= "") << endl;
		cout << "串1=" << s1 << " 常量=" << "       ==结果应为0，实际：" << (s1 == "") << endl;
		cout << "串1=" << s1 << " 常量=" << "       !=结果应为1，实际：" << (s1 != "") << endl;

		cout << "串1=" << s1 << " 常量=<NULL>" << " > 结果应为1，实际：" << (s1 > NULL) << endl;
		cout << "串1=" << s1 << " 常量=<NULL>" << " >=结果应为1，实际：" << (s1 >= NULL) << endl;
		cout << "串1=" << s1 << " 常量=<NULL>" << " < 结果应为0，实际：" << (s1 < NULL) << endl;
		cout << "串1=" << s1 << " 常量=<NULL>" << " <=结果应为0，实际：" << (s1 <= NULL) << endl;
		cout << "串1=" << s1 << " 常量=<NULL>" << " ==结果应为0，实际：" << (s1 == NULL) << endl;
		cout << "串1=" << s1 << " 常量=<NULL>" << " !=结果应为1，实际：" << (s1 != NULL) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = NULL, s2 = "";

		/* 空的TString和NULL、"" 都认为相等（本质上，NULL是地址，""是串，没有可比性，但是此处做特殊处理） */
		cout << "串1=" << s1 << " 串2=" << s2 << "  > 结果应为0，实际：" << (s1 > s2) << endl;
		cout << "串1=" << s1 << " 串2=" << s2 << "  >=结果应为1，实际：" << (s1 >= s2) << endl;
		cout << "串1=" << s1 << " 串2=" << s2 << "  < 结果应为0，实际：" << (s1 < s2) << endl;
		cout << "串1=" << s1 << " 串2=" << s2 << "  <=结果应为1，实际：" << (s1 <= s2) << endl;
		cout << "串1=" << s1 << " 串2=" << s2 << "  ==结果应为1，实际：" << (s1 == s2) << endl;
		cout << "串1=" << s1 << " 串2=" << s2 << "  !=结果应为0，实际：" << (s1 != s2) << endl;

		cout << "串1=" << s1 << " 常量=" << "       > 结果应为0，实际：" << (s1 > "") << endl;
		cout << "串1=" << s1 << " 常量=" << "       >=结果应为1，实际：" << (s1 >= "") << endl;
		cout << "串1=" << s1 << " 常量=" << "       < 结果应为0，实际：" << (s1 < "") << endl;
		cout << "串1=" << s1 << " 常量=" << "       <=结果应为1，实际：" << (s1 <= "") << endl;
		cout << "串1=" << s1 << " 常量=" << "       ==结果应为1，实际：" << (s1 == "") << endl;
		cout << "串1=" << s1 << " 常量=" << "       !=结果应为0，实际：" << (s1 != "") << endl;

		cout << "串1=" << s1 << " 常量=<NULL>" << " > 结果应为0，实际：" << (s1 > NULL) << endl;
		cout << "串1=" << s1 << " 常量=<NULL>" << " >=结果应为1，实际：" << (s1 >= NULL) << endl;
		cout << "串1=" << s1 << " 常量=<NULL>" << " < 结果应为0，实际：" << (s1 < NULL) << endl;
		cout << "串1=" << s1 << " 常量=<NULL>" << " <=结果应为1，实际：" << (s1 <= NULL) << endl;
		cout << "串1=" << s1 << " 常量=<NULL>" << " ==结果应为1，实际：" << (s1 == NULL) << endl;
		cout << "串1=" << s1 << " 常量=<NULL>" << " !=结果应为0，实际：" << (s1 != NULL) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "house";
		const char* str2 = "horse", * str3 = "house", * str4 = "", * str5 = NULL;

		cout << "比较运算测试(TString类和字符指针)" << endl;

		cout << "串1=" << s1 << " 串2=" << str2 << "  > 结果应为1，实际：" << (s1 > str2) << endl;
		cout << "串1=" << s1 << " 串2=" << str2 << "  >=结果应为1，实际：" << (s1 >= str2) << endl;
		cout << "串1=" << s1 << " 串2=" << str2 << "  < 结果应为0，实际：" << (s1 < str2) << endl;
		cout << "串1=" << s1 << " 串2=" << str2 << "  <=结果应为0，实际：" << (s1 <= str2) << endl;
		cout << "串1=" << s1 << " 串2=" << str2 << "  ==结果应为0，实际：" << (s1 == str2) << endl;
		cout << "串1=" << s1 << " 串2=" << str2 << "  !=结果应为1，实际：" << (s1 != str2) << endl;

		cout << "串1=" << s1 << " 串3=" << str3 << "  > 结果应为0，实际：" << (s1 > str3) << endl;
		cout << "串1=" << s1 << " 串3=" << str3 << "  >=结果应为1，实际：" << (s1 >= str3) << endl;
		cout << "串1=" << s1 << " 串3=" << str3 << "  < 结果应为0，实际：" << (s1 < str3) << endl;
		cout << "串1=" << s1 << " 串3=" << str3 << "  <=结果应为1，实际：" << (s1 <= str3) << endl;
		cout << "串1=" << s1 << " 串3=" << str3 << "  ==结果应为1，实际：" << (s1 == str3) << endl;
		cout << "串1=" << s1 << " 串3=" << str3 << "  !=结果应为0，实际：" << (s1 != str3) << endl;

		cout << "串1=" << s1 << " 串4=" << str4 << "       > 结果应为1，实际：" << (s1 > str4) << endl;
		cout << "串1=" << s1 << " 串4=" << str4 << "       >=结果应为1，实际：" << (s1 >= str4) << endl;
		cout << "串1=" << s1 << " 串4=" << str4 << "       < 结果应为0，实际：" << (s1 < str4) << endl;
		cout << "串1=" << s1 << " 串4=" << str4 << "       <=结果应为0，实际：" << (s1 <= str4) << endl;
		cout << "串1=" << s1 << " 串4=" << str4 << "       ==结果应为0，实际：" << (s1 == str4) << endl;
		cout << "串1=" << s1 << " 串4=" << str4 << "       !=结果应为1，实际：" << (s1 != str4) << endl;

		cout << "串1=" << s1 << " 串5=<NULL>" << " > 结果应为1，实际：" << (s1 > str5) << endl;
		cout << "串1=" << s1 << " 串5=<NULL>" << " >=结果应为1，实际：" << (s1 >= str5) << endl;
		cout << "串1=" << s1 << " 串5=<NULL>" << " < 结果应为0，实际：" << (s1 < str5) << endl;
		cout << "串1=" << s1 << " 串5=<NULL>" << " <=结果应为0，实际：" << (s1 <= str5) << endl;
		cout << "串1=" << s1 << " 串5=<NULL>" << " ==结果应为0，实际：" << (s1 == str5) << endl;
		cout << "串1=" << s1 << " 串5=<NULL>" << " !=结果应为1，实际：" << (s1 != str5) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "house";
		char str2[] = "horse", str3[] = "house", str4[] = "";

		cout << "比较运算测试(TString类和字符指针)" << endl;

		cout << "串1=" << s1 << " 串2=" << str2 << " > 结果应为1，实际：" << (s1 > str2) << endl;
		cout << "串1=" << s1 << " 串2=" << str2 << " >=结果应为1，实际：" << (s1 >= str2) << endl;
		cout << "串1=" << s1 << " 串2=" << str2 << " < 结果应为0，实际：" << (s1 < str2) << endl;
		cout << "串1=" << s1 << " 串2=" << str2 << " <=结果应为0，实际：" << (s1 <= str2) << endl;
		cout << "串1=" << s1 << " 串2=" << str2 << " ==结果应为0，实际：" << (s1 == str2) << endl;
		cout << "串1=" << s1 << " 串2=" << str2 << " !=结果应为1，实际：" << (s1 != str2) << endl;

		cout << "串1=" << s1 << " 串3=" << str3 << " > 结果应为0，实际：" << (s1 > str3) << endl;
		cout << "串1=" << s1 << " 串3=" << str3 << " >=结果应为1，实际：" << (s1 >= str3) << endl;
		cout << "串1=" << s1 << " 串3=" << str3 << " < 结果应为0，实际：" << (s1 < str3) << endl;
		cout << "串1=" << s1 << " 串3=" << str3 << " <=结果应为1，实际：" << (s1 <= str3) << endl;
		cout << "串1=" << s1 << " 串3=" << str3 << " ==结果应为1，实际：" << (s1 == str3) << endl;
		cout << "串1=" << s1 << " 串3=" << str3 << " !=结果应为0，实际：" << (s1 != str3) << endl;

		cout << "串1=" << s1 << " 串4=" << str4 << "      > 结果应为1，实际：" << (s1 > str4) << endl;
		cout << "串1=" << s1 << " 串4=" << str4 << "      >=结果应为1，实际：" << (s1 >= str4) << endl;
		cout << "串1=" << s1 << " 串4=" << str4 << "      < 结果应为0，实际：" << (s1 < str4) << endl;
		cout << "串1=" << s1 << " 串4=" << str4 << "      <=结果应为0，实际：" << (s1 <= str4) << endl;
		cout << "串1=" << s1 << " 串4=" << str4 << "      ==结果应为0，实际：" << (s1 == str4) << endl;
		cout << "串1=" << s1 << " 串4=" << str4 << "      !=结果应为1，实际：" << (s1 != str4) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "tong", s2;

		cout << "求长度测试(length()函数)" << endl;

		cout << "s1为tong，  长度应为4，实际：" << s1.length() << endl;
		cout << "s2为<NULL>，长度应为0，实际：" << s2.length() << endl;
		s2 = s1 + "ji";
		cout << "s2为tongji，长度应为6，实际：" << s2.length() << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "tong", s2;

		cout << "求长度测试(TStringLen()函数)" << endl;

		cout << "s1为tong，  长度应为4，实际：" << TStringLen(s1) << endl;
		cout << "s2为<NULL>，长度应为0，实际：" << TStringLen(s2) << endl;
		cout << "串为tongji，长度应为6，实际：" << TStringLen(s1 + (s2 = "ji")) << endl;
		cout << "串为tongji，长度应为6，实际：" << TStringLen(s1 + "ji") << endl;
		cout << "串为tongji，长度应为6，实际：" << TStringLen(s2 = s1 + "ji") << endl;
		cout << "串为tong，  长度应为4，实际：" << TStringLen(s1 + (char*)NULL) << endl;

		wait_for_enter();
	}

	if (ts_passed == 0) {
		TStringAdv s1 = "house";
		TStringAdv s2[4] = { "yangpu", "sipingroad", "tongji", "shanghai" };
		int i, j;
		char ch = 'P';

		cout << "[]运算符测试" << endl;

		cout << "串1是" << s1 << "，依次打印字符的结果是：";
		for (i = 0; i < s1.length(); i++)
			cout << s1[i] << ' ';
		cout << endl << endl;

		cout << "串1是" << s1 << "，依次打印字符int值的结果是（有越界）：" << endl;
		for (i = -4; i < s1.length() + 4; i++)
			cout << int(s1[i]) << ' ';
		cout << endl << endl;

		s1[0] = s1[0] - 32;
		cout << "串1是" << s1 << "，依次打印字符的结果是：";
		for (i = 0; i < s1.length(); i++)
			cout << s1[i] << ' ';
		cout << endl << endl;

		s1[1] = 'A';
		s1[2] = ch;
		ch += 32;
		s1[3] = ch;
		s1[4] = 'Y';
		cout << "串1是" << s1 << "，依次打印字符的结果是：";
		for (i = 0; i < s1.length(); i++)
			cout << s1[i] << ' ';
		cout << endl << endl;

		cout << "串2数组是4个字符串，依次打印字符的结果是：" << endl;
		for (i = 0; i < 4; i++) {
			cout << setw(16) << s2[i] << " => ";
			for (j = 0; j < s2[i].length(); j++)
				cout << s2[i][j] << ' ';
			cout << endl;
		}
		cout << endl;

		cout << "串2数组是4个字符串，首字母大写后依次打印字符的结果是：" << endl;
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

		cout << "TStringAdv-assign函数测试" << endl;

		s0.assign(s1);
		cout << "s1标准输出" << setw(20) << standard[1] << " s1实际输出" << setw(20) << s0 << endl;
		s0.assign(s2);
		cout << "s2标准输出" << setw(20) << standard[2] << " s2实际输出" << setw(20) << s0 << endl;
		s0.assign(s3);
		cout << "s3标准输出" << setw(20) << standard[3] << " s3实际输出" << setw(20) << s0 << endl;
		s0.assign(s4);
		cout << "s4标准输出" << setw(20) << standard[4] << " s4实际输出" << setw(20) << s0 << endl;
		s0.assign(s5);
		cout << "s5标准输出" << setw(20) << standard[5] << " s5实际输出" << setw(20) << s0 << endl;
		s0.assign(s6);
		cout << "s6标准输出" << setw(20) << standard[6] << " s6实际输出" << setw(20) << s0 << endl;
		s0.assign(s7);
		cout << "s7标准输出" << setw(20) << standard[7] << " s7实际输出" << setw(20) << s0 << endl;
		s0.assign(s8);
		cout << "s8标准输出" << setw(20) << standard[8] << " s8实际输出" << setw(20) << s0 << endl;
		s0.assign(s9);
		cout << "s9标准输出" << setw(20) << standard[9] << " s9实际输出" << setw(20) << s0 << endl;
		s0.assign(s10);
		cout << "sA标准输出" << setw(20) << standard[10] << " sA实际输出" << setw(20) << s0 << endl;

		wait_for_enter();
	}

	if (1) {
		const static char* standard[] = {
			"",
			"teststr1", "teststr2", "teststr3", "teststr4",
			"teststr(2+3)", "teststr(2+4)", "teststr(2+5)", "teststr(2+6)"
		};

		cout << "TStringAdv-append函数测试(本项可能有2-3秒停顿)" << endl;

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

		cout << "s1标准输出" << setw(15) << standard[1] << " s1实际输出" << setw(15) << s1 << endl;
		cout << "s2标准输出" << setw(15) << standard[2] << " s2实际输出" << setw(15) << s2 << endl;
		cout << "s3标准输出" << setw(15) << standard[3] << " s3实际输出" << setw(15) << s3 << endl;
		cout << "s4标准输出" << setw(15) << standard[4] << " s4实际输出" << setw(15) << s4 << endl;
		cout << "s5标准输出" << setw(15) << standard[5] << " s5实际输出" << setw(15) << s5 << endl;
		cout << "s6标准输出" << setw(15) << standard[6] << " s6实际输出" << setw(15) << s6 << endl;
		cout << "s7标准输出" << setw(15) << standard[7] << " s7实际输出" << setw(15) << s7 << endl;
		cout << "s8标准输出" << setw(15) << standard[8] << " s8实际输出" << setw(15) << s8 << endl;
		cout << "s9理论长度" << setw(15) << 16777216    << " s9实际长度" << setw(15) << TStringLen(s9) << endl;
		cout << "s9 - 抽查检验结果：" << (s9_flag ? "通过" : "不通过，请检查append函数实现") << endl;

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
		cout << "TStringAdv-insert函数测试" << endl;

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

		cout << setw(15) << "s1标准输出" << setw(45) << standard[1] << endl << setw(15) << "s1实际输出" << setw(45) << s1 << endl;
		cout << setw(15) << "s2标准输出" << setw(45) << standard[2] << endl << setw(15) << "s2实际输出" << setw(45) << s2 << endl;
		cout << setw(15) << "s3标准输出" << setw(45) << standard[3] << endl << setw(15) << "s3实际输出" << setw(45) << s3 << endl;
		cout << setw(15) << "s4标准输出" << setw(45) << standard[4] << endl << setw(15) << "s4实际输出" << setw(45) << s4 << endl;
		cout << setw(15) << "s5标准输出" << setw(45) << standard[5] << endl << setw(15) << "s5实际输出" << setw(45) << s5 << endl;

		cout << "下面是一些不符合insert操作意图的用例，保证插入结果符合正确结果即可：" << endl;

		cout << setw(15) << "s1正确结果" << setw(45) << standard[1] << endl;
		cout << setw(15) << "s1越界插入-例1" << setw(45) << TStringAdv(s1).insert("am", -1) << endl;
		cout << setw(15) << "s1越界插入-例2" << setw(45) << TStringAdv(s1).insert('*', TStringLen(s1) + 10000) << endl;
		cout << setw(15) << "s1插入尾零" << setw(45) << TStringAdv(s1).insert('\0', TStringLen(s1) / 2) << endl;
		cout << setw(15) << "s1插入空串" << setw(45) << TStringAdv(s1).insert("", (int)(0.4 * TStringLen(s1))) << endl;
		cout << setw(15) << "s1插入NULL" << setw(45) << TStringAdv(s1).insert((const char*)(NULL), (int)(0.4 * TStringLen(s1))) << endl;

		wait_for_enter();
	}

	if (1) {
		const static char* standard[] = {
			"",
			"CandySaga", "Tongji", "Division",
			"SchoolOfEngineering", "DeptOfComputingMachinery",
			"CSPrfession"
		};
		cout << "TStringAdv-erase函数测试" << endl;

		TStringAdv			s1 = "CandyCrushSaga";
		TStringAdv			s2 = "TongjiUniv";
		TStringAdv			s3 = "JiadingDivision";
		TStringAdv			s4 = "SchoolOfElectricEngineering";
		TStringAdv			s5 = "DeptOfComputingMachinery";
		TStringAdv			s6 = "CSProfession";

		cout << setw(15) << "s1标准输出" << setw(45) << standard[1] << endl << setw(15) << "s1实际输出" << setw(45) << s1.erase("Crush") << endl;
		cout << setw(15) << "s2标准输出" << setw(45) << standard[2] << endl << setw(15) << "s2实际输出" << setw(45) << s2.erase("Univ") << endl;
		cout << setw(15) << "s3标准输出" << setw(45) << standard[3] << endl << setw(15) << "s3实际输出" << setw(45) << s3.erase("Jiading\0xiaoqu") << endl;
		cout << setw(15) << "s4标准输出" << setw(45) << standard[4] << endl << setw(15) << "s4实际输出" << setw(45) << s4.erase(TString("Electric")) << endl;
		cout << setw(15) << "s5标准输出" << setw(45) << standard[5] << endl << setw(15) << "s5实际输出" << setw(45) << s5.erase(TStringAdv("Computer")) << endl;
		cout << setw(15) << "s6标准输出" << setw(45) << standard[6] << endl << setw(15) << "s6实际输出" << setw(45) << s6.erase('o') << endl;

		cout << "下面是一些不符合erase操作意图的用例，保证结果符合正确结果即可：" << endl;

		cout << setw(15) << "s5正确结果" << setw(45) << standard[5] << endl;
		cout << setw(15) << "s5去除尾零" << setw(45) << TStringAdv(s5).erase('\0') << endl;
		cout << setw(15) << "s5去除空串" << setw(45) << TStringAdv(s5).erase("") << endl;
		cout << setw(15) << "s5去除NULL" << setw(45) << TStringAdv(s5).erase((const char*)NULL) << endl;
		cout << setw(15) << "s5去除无效串" << setw(45) << TStringAdv(s5).erase("\03" + s5) << endl;

		wait_for_enter();
	}

	if (1) {
		const static char* standard[] = {
			"",
			"hangha", "tong", "ngpu", "sipingroad", "_freshman"
		};

		cout << "TStringAdv-substr函数测试" << endl;

		TStringAdv			s1 = "shanghai";
		TStringAdv			s2 = TString("tongji");
		TStringAdv			s3 = TStringAdv("yangpu");
		TStringAdv			s4 = "sipingroad";
		TStringAdv			s5 = "school_for_freshmen";

		cout << setw(15) << "s1标准输出" << setw(45) << standard[1] << endl << setw(15) << "s1实际输出" << setw(45) << s1.substr(2, 6) << endl;
		cout << setw(15) << "s2标准输出" << setw(45) << standard[2] << endl << setw(15) << "s2实际输出" << setw(45) << s2.substr(1, 4) << endl;
		cout << setw(15) << "s3标准输出" << setw(45) << standard[3] << endl << setw(15) << "s3实际输出" << setw(45) << s3.substr(3) << endl;
		cout << setw(15) << "s4标准输出" << setw(45) << standard[4] << endl << setw(15) << "s4实际输出" << setw(45) << s4.substr(1, 8388608) << endl;
		cout << setw(15) << "s5标准输出" << setw(45) << standard[5] << endl << setw(15) << "s5实际输出" << setw(45) << s5.substr(11, 536870912) << endl;

		cout << "下面是一些不符合substr操作意图的用例，保证结果符合正确结果即可：" << endl;

		cout << setw(15) << "s5正确结果" << setw(45) << "<NULL>" << endl;
		cout << setw(15) << "s5非预期用例1" << setw(45) << TStringAdv(s5).substr(0, 9) << endl;
		cout << setw(15) << "s5非预期用例2" << setw(45) << TStringAdv(s5).substr(-1, 2) << endl;
		cout << setw(15) << "s5非预期用例3" << setw(45) << TStringAdv(s5).substr(3, 0) << endl;
		cout << setw(15) << "s5非预期用例4" << setw(45) << TStringAdv(s5).substr(1048576, 0) << endl;

		wait_for_enter();
	}

	if (1) {
		cout << "TStringAdv-at函数测试" << endl;

		TStringAdv			s1 = "shanghai";
		TStringAdv			s2 = TString("tongji");
		TStringAdv			s3 = TStringAdv("yangpu");
		TStringAdv			s4 = "sipingroad";

		cout << setw(15) << "s1标准输出" << setw(45) << s1 << endl;
		cout << setw(15) << "s2标准输出" << setw(45) << s2 << endl;
		cout << setw(15) << "s3标准输出" << setw(45) << s3 << endl;
		cout << setw(15) << "s4标准输出" << setw(45) << s4 << endl;

		cout << setw(15) << "s1按字符展开";
		for (int i = 0, _l = TStringLen(s1); i < _l; i++)
			cout << ' ' << s1[i];
		cout << endl;

		cout << setw(15) << "s2按字符展开";
		for (int i = 0, _l = TStringLen(s2); i < _l; i++)
			cout << ' ' << s2[i];
		cout << endl;

		cout << setw(15) << "s3按字符展开";
		for (int i = 0, _l = TStringLen(s3); i < _l; i++)
			cout << ' ' << s3[i];
		cout << endl;

		cout << setw(15) << "s4按字符展开";
		for (int i = 0, _l = TStringLen(s4); i < _l; i++)
			cout << ' ' << s4[i];
		cout << endl;

		wait_for_enter();
	}

	if (1) {
		cout << "TStringAdv-综合测试" << endl;

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

		cout << setw(15) << "s1理论值" << setw(60) << "sipingroad-1239" << endl;
		cout << setw(15) << "s1实际值" << setw(60) << s1 << endl;
		cout << setw(15) << "s2理论值" << setw(60) << "I dislike Script Language." << endl;
		cout << setw(15) << "s2实际值" << setw(60) << s2 << endl;
		cout << setw(15) << "s3运算次数理论值" << setw(60) << "46" << endl;
		cout << setw(15) << "s3运算次数实际值" << setw(60) << s3_cnt << endl;
		cout << setw(15) << "s4理论值" << setw(60) << "/home/u2053642/common/16-b2/16-b2-main.cpp" << endl;
		cout << setw(15) << "s4实际值" << setw(60) << s4 << endl;

		wait_for_enter();
	}

#if !(defined(__linux) || defined(__linux__))
	system("pause"); //Linux不需要
#endif

	return 0;
}
